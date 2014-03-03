#include <QList>
#include <QStringList>

#ifndef OTHER_H
#define OTHER_H

class ArgList
{
public:
//  CommandVector();
  QString name;
  QList<QString> args;
};

class Radius
{
public:
  double   angle;
  int   length;
};




#endif // COMMANDVECTOR_H
