#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <LSM303.h>

#define DEBUG

#define rs485_pin A5
#define ENGINE_COUNT 3
#define REVERSE_PAUSE_MSEC 1000

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 2);
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
}

void WriteRS485(char buf[], int count)
{
  digitalWrite(rs485_pin, HIGH);
  for (int i=0; i<count; i++)
    Serial.print(buf[i]);
  delay(50); //without delay arduino cut mes
  digitalWrite(rs485_pin, LOW);
}

void setup()  
{ 
  SetupEngine(0,6,7);
  SetupEngine(1,5,4);
  SetupEngine(2,3,2);

  pinMode(rs485_pin, OUTPUT);   
  digitalWrite(rs485_pin, LOW);
  Serial.begin(19200); 
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  Wire.begin();
  compass.init();
  compass.enableDefault();

  WriteRS485("SMTU ROV v 1.11 firmvare\n", 25); 
} 

char buffer[20];
char inChar = -1;
byte charIndx = 0;

void ParseReverseCmd(byte engineNum)
{
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
  WriteRS485("R: ",3);
  char buf[3];
  itoa(ENGINES[engineNum].reverseState,buf,10);
  WriteRS485(buf, 1);    
  WriteRS485(". \n", 3);
#endif
}

void ParsePowerCmd(byte engineNum)
{
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
      WriteRS485("P: ", 13);
      char buf[3];
      itoa(val,buf,10);
      WriteRS485(buf, charIndx - 3);   
      WriteRS485(". \n", 3);
#endif
    }
  } 
}

void SendAccelerometerData()
{
   compass.read();
   
   client.print("X ");
   client.print((int)compass.a.x);
   client.print(" Y: ");
   client.print((int)compass.a.y);
   client.print(" Z: ");
   client.println((int)compass.a.z);   
}

void SendMagnetometerData()
{
   compass.read();
   
   client.print("X ");
   client.print((int)compass.m.x);
   client.print(" Y: ");
   client.print((int)compass.m.y);
   client.print(" Z: ");
   client.println((int)compass.m.z);   
}

void ParseBuffer()
{
  byte engineNum = buffer[2] - '0';
  /*if (engineNum < 0 || engineNum >= ENGINE_COUNT)
  //  return;  
#ifdef DEBUG
  WriteRS485("Engine num: ", 12);
  char buf[3];
  itoa(engineNum,buf,10);
  WriteRS485(buf,1);    
  WriteRS485(". \n", 3);
#endif    */   
  switch (buffer[1])
  {
    case 'r' : ParseReverseCmd(engineNum); break;
    case 'p' : ParsePowerCmd(engineNum); break; 
    case 'a' : SendAccelerometerData(); break;
    case 'm' : SendMagnetometerData(); break;
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
        digitalWrite(rs485_pin, HIGH);
        Serial.println(currentMillis-ENGINES[i].reverseEndTime);
        digitalWrite(rs485_pin, LOW);
        
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
