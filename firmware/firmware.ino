#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <LSM303.h>
#include <Stepper.h>

#define DEBUG

#define ENGINE_COUNT 6
#define REVERSE_PAUSE_MSEC 500

#define QUERY_DEPTH_CMD 'd'
#define QUERY_PITCH_CMD 'p'

#define STEPS 200
#define STEPPER_0 A10
#define STEPPER_1 A11
#define STEPPER_2 A12
#define STEPPER_3 A13

Stepper stepper(STEPS, STEPPER_0, STEPPER_1, STEPPER_2, STEPPER_3);

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 2);
EthernetServer server(80);
EthernetClient client;
LSM303 compass;

class Engine
{
public:
  byte reversePin;
  byte powerPin;
  byte currentPower;
  byte reverseState;
  unsigned long reverseEndTime;
};

Engine ENGINES[ENGINE_COUNT];

void SetupEngine (int engineNum, int powerPin, int reversePin)
{
  ENGINES[engineNum].powerPin = powerPin;
  ENGINES[engineNum].reversePin = reversePin;
  ENGINES[engineNum].currentPower = 0;
  ENGINES[engineNum].reverseState = 0;
  ENGINES[engineNum].reverseEndTime = 0;

  pinMode(reversePin, OUTPUT);   
  analogWrite(ENGINES[engineNum].powerPin, 0); 
  digitalWrite(ENGINES[engineNum].reversePin, LOW);
  
  pinMode(STEPPER_0, OUTPUT);
  pinMode(STEPPER_1, OUTPUT);
  pinMode(STEPPER_2, OUTPUT);
  pinMode(STEPPER_3, OUTPUT);
}

void setup()  
{ 
  SetupEngine(0,2,14);
  SetupEngine(1,3,15);
  SetupEngine(2,4,16);
  SetupEngine(3,5,17);
  SetupEngine(4,6,18);
  SetupEngine(5,7,19);

  Serial.begin(19200);   
  Ethernet.begin(mac, ip);
  server.begin();
  
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  stepper.setSpeed(300);
    
  Serial.print("SMTU ROV v 1.2 firmvare\n"); 
} 

char buffer[20];
char inChar = -1;
byte charIndx = 0;

void ParseReverseCmd(byte engineNum)
{
  if (engineNum < 0 || engineNum >= ENGINE_COUNT)
    return;  
  byte reverseState = buffer[3] - '0';
 
  if (reverseState != 1 && reverseState != 0)
     return;

  if (reverseState != ENGINES[engineNum].reverseState)
  {
    ENGINES[engineNum].reverseState = reverseState;

    analogWrite(ENGINES[engineNum].powerPin, 0); 

    if (reverseState == 1)
      digitalWrite(ENGINES[engineNum].reversePin, HIGH);
    else 
      digitalWrite(ENGINES[engineNum].reversePin, LOW);

    ENGINES[engineNum].reverseEndTime = millis() + REVERSE_PAUSE_MSEC;
  }

#ifdef DEBUG
  Serial.print("Engine num: ");
  Serial.print(engineNum);    
  Serial.print(". \n");
  Serial.print("R: ");
  Serial.print((int)ENGINES[engineNum].reverseState);
  Serial.print(". \n");
#endif
}

void ParsePowerCmd(byte engineNum)
{
  if (engineNum < 0 || engineNum >= ENGINE_COUNT)
    return;  
  if (charIndx <= 6)
  {
    int val = 0;
    switch (charIndx) 
    {
      case 6 : val = (buffer[3]-'0')*100 + (buffer[4]-'0')*10 + (buffer[5]-'0'); break;   
      case 5 : val = (buffer[3]-'0')*10 + (buffer[4]-'0'); break;   
      case 4 : val = (buffer[3]-'0'); break;
      default: return;      
    }
      
    if (0 <= val && val <= 255)
    {
      if (ENGINES[engineNum].reverseEndTime == 0)
	analogWrite(ENGINES[engineNum].powerPin, val);
      
      ENGINES[engineNum].currentPower = val;         

#ifdef DEBUG
      Serial.print("Engine num: ");
      Serial.print(engineNum);    
      Serial.print(". \n");
      Serial.print("P: ");
      Serial.print(val);
      Serial.print('\n');
#endif
    }
  } 
}

char query[10];
void QueryAnswer(char cmd, float value)
{
  client.write("$q");
  client.write(cmd);
  dtostrf(value,2,1,query);
  client.write(query); 
  client.write("!\n\r");
}

char pitch[10]; 
void AnswerPitch()
{
  compass.read();
  float value = compass.a.y/1024.0;
  if (abs(value) > 1)
    value=value/abs(value);
  QueryAnswer(QUERY_PITCH_CMD, asin(value)/3.1415*180);
}

void AnswerDepth()
{
  QueryAnswer(QUERY_DEPTH_CMD, analogRead(A0));
}

void ParseQueryCmd()
{
  switch (buffer[2])
  {
    case QUERY_PITCH_CMD : AnswerPitch(); break;
    case QUERY_DEPTH_CMD : AnswerDepth(); break;
  }

}

#define MANIPULATOR_STEPS 200

void MoveManipulator(char dir)
{
  switch (dir)
  {
    case '1' : stepper.step(MANIPULATOR_STEPS); break;
    case '0' : stepper.step(-MANIPULATOR_STEPS); break;   
  }
  
  digitalWrite(STEPPER_0, 0);
  digitalWrite(STEPPER_1, 0);
  digitalWrite(STEPPER_2, 0);
  digitalWrite(STEPPER_3, 0);  
}


void ParseBuffer()
{
  byte engineNum = buffer[2] - '0';
  
  switch (buffer[1])
  {
    case 'r' : ParseReverseCmd(engineNum); break;
    case 'p' : ParsePowerCmd(engineNum); break; 
    case 'q' : ParseQueryCmd(); break;
    case 'm' : MoveManipulator(buffer[2]); break;
  }
}

void ReadCommand()
{
  if(!client.connected())
    client = server.available();
  while (client.available() > 0) 
  {
    if(charIndx < 19) // One less than the size of the array
    {
      inChar = client.read(); 

      if (inChar == '#')
        charIndx = 0;
        
      if (inChar == '!')
      {
        ParseBuffer();
        charIndx = 0;
      }
        
      buffer[charIndx] = inChar; 
      charIndx++; 
    }
    else
      charIndx = 0;
	ReverseCheck();
  }
}

void ReverseCheck()
{
  unsigned long currentMillis = millis();
  for (byte i = 0; i < ENGINE_COUNT; i++)
  {
    if ( ENGINES[i].reverseEndTime != 0 && currentMillis >= ENGINES[i].reverseEndTime)
    {
	analogWrite(ENGINES[i].powerPin, ENGINES[i].currentPower);	
	ENGINES[i].reverseEndTime = 0;
    }		
  }
}

void loop()  
{
  ReverseCheck();
  ReadCommand(); 
}
