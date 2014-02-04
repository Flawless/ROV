#include "other.h"

//CommandVector::commandVector()
//{

//}

QString makeCommand(ArgList argList)
{
  QString command_char = argList.name;
  for (int i = 0; i < argList.args.size(); ++i) {
       command_char += "." + argList.args[i];
 }
  return command_char;
}


