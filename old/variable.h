#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>
#include <QList>

class VariableRecord
{
public:
    QString label;
    QString value;
    QString type;
};

class Variable
{
public:
    static QList<VariableRecord> onlineVariableList;
};

#endif // VARIABLE_H
