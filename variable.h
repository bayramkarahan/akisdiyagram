#ifndef VARIABLE_H
#define VARIABLE_H
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCompleter>
#include <QStringListModel>
#include <QInputDialog>
#include<QMessageBox>
#include<QDebug>

class VariableRecord {
public:
    QString label;
    QString var1Label;
    QString var2Label;
    QString value;
    QString valueType;
    bool isInput = false;
    QString expression="";
    int operationType;
    int startValue;
    int endValue;
    int stepValue;
    int counter=0;
    QString inputMessage;
    QString outputMessage;
    bool evaluate(const QList<VariableRecord>& vars) {

            qDebug() << "işlem" << operationType<<valueType;
        QStringList parts = expression.split('=');
        if (parts.size() != 2) return false;

        QString rightExpr = parts[1].trimmed();
        bool ok;
        // Eğer tip "text" ise sadece string birleştirme yap
        if (valueType == "text") {
            qDebug() << "işlemt" <<valueType<<operationType;
            // Sayısal işlemler:
            switch (operationType) {
            case 0: { // Sabit atama
                value = rightExpr;
                return true;
            }
            case 1: { // Değişken atama
                for (const auto& v : vars) {
                    if (v.label == rightExpr) {
                            value = rightExpr;
                            return true;
                    }
                }
                return false;
            }
            case 2: { // İki değişkenli işlem
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\w+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString var1 = rx.cap(1);
                QString op = rx.cap(2);
                QString var2 = rx.cap(3);
                QString varVal1="";
                QString varVal2="";
                bool found1 = false, found2 = false;
                for (const auto& v : vars) {
                    if (v.label == var1) {
                        found1 = true;
                        varVal1 = v.value;
                    }
                    if (v.label == var2) {
                        found2 = true;
                        varVal2 = v.value;
                    }
                }
                if (!found1 || !found2) return false;

                QString result ="";
                if (op == "+"){ result = varVal1 + varVal2;
                } else return false;

                value = result;
                return true;
            }
            case 3: { // Değişken + sabit sayı
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\d+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString var1 = rx.cap(1);
                QString op = rx.cap(2);
                QString numStr = rx.cap(3);
                QString varVal1;
                for (const auto& v : vars) {
                    if (v.label == var1) {
                        varVal1 = v.value;
                        break;
                    }
                }

                QString  result = 0;
                if (op == "+") {
                    result = varVal1 + numStr;
                } else return false;
                value = result;
                return true;
            }
            case 4: { // sabit sayı + sabit sayı
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\d+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString numStr1 = rx.cap(1);
                QString op = rx.cap(2);
                QString numStr2 = rx.cap(3);

                QString result = "";
                if (op == "+"){
                    result = numStr1 + numStr2;
                } else return false;

                value = result;
                return true;
            }
            default:
                return false;
            }
        }
        if (valueType == "number") {
            // Sayısal işlemler:
            qDebug() << "işlems" <<valueType<<operationType;
            switch (operationType) {
            case 0: { // Sabit atama
                double val = rightExpr.toDouble(&ok);
                if (!ok) return false;
                value = QString::number(val);
                return true;
            }
            case 1: { // Değişken atama
                for (const auto& v : vars) {
                    if (v.label == rightExpr) {
                        double val = v.value.toDouble(&ok);
                        if (ok) {
                            value = QString::number(val);
                            return true;
                        }
                        return false;
                    }
                }
                return false;
            }
            case 2: { // İki değişkenli işlem
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\w+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString var1 = rx.cap(1);
                QString op = rx.cap(2);
                QString var2 = rx.cap(3);

                double val1 = 0, val2 = 0;
                bool found1 = false, found2 = false;
                for (const auto& v : vars) {
                    if (v.label == var1) {
                        val1 = v.value.toDouble(&ok);
                        found1 = ok;
                    }
                    if (v.label == var2) {
                        val2 = v.value.toDouble(&ok);
                        found2 = ok;
                    }
                }
                if (!found1 || !found2) return false;

                double result = 0;
                if (op == "+") result = val1 + val2;
                else if (op == "-") result = val1 - val2;
                else if (op == "*") result = val1 * val2;
                else if (op == "/") {
                    if (val2 == 0) return false;
                    result = val1 / val2;
                } else return false;

                value = QString::number(result);
                return true;
            }
            case 3: { // Değişken + sabit sayı
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\d+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString var1 = rx.cap(1);
                QString op = rx.cap(2);
                QString numStr = rx.cap(3);

                double val1 = 0;
                for (const auto& v : vars) {
                    if (v.label == var1) {
                        val1 = v.value.toDouble(&ok);
                        if (!ok) return false;
                        break;
                    }
                }

                double val2 = numStr.toDouble(&ok);
                if (!ok) return false;

                double result = 0;
                if (op == "+") result = val1 + val2;
                else if (op == "-") result = val1 - val2;
                else if (op == "*") result = val1 * val2;
                else if (op == "/") {
                    if (val2 == 0) return false;
                    result = val1 / val2;
                } else return false;

                value = QString::number(result);
                return true;
            }
            case 4: { // sabit sayı + sabit sayı
                QRegExp rx(R"((\w+)\s*([\+\-\*/])\s*(\d+))");
                if (!rx.exactMatch(rightExpr)) return false;

                QString numStr1 = rx.cap(1);
                QString op = rx.cap(2);
                QString numStr2 = rx.cap(3);


                double val1 = numStr1.toDouble(&ok);
                if (!ok) return false;

                double val2 = numStr2.toDouble(&ok);
                if (!ok) return false;

                double result = 0;
                if (op == "+") result = val1 + val2;
                else if (op == "-") result = val1 - val2;
                else if (op == "*") result = val1 * val2;
                else if (op == "/") {
                    if (val2 == 0) return false;
                    result = val1 / val2;
                } else return false;

                value = QString::number(result);
                return true;
            }
            default:
                return false;
            }
        }
      }
};

class Variable {
public:
    static QList<VariableRecord> onlineVariableList;
};


#endif // VARIABLE
