#ifndef VARIABLEEXPRESSIONDIALOG_H
#define VARIABLEEXPRESSIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "variable.h"
struct ProcessRecord {
    int processType;
    QString expression;
    QString targetName;
    QString targetLabel;
    QString var1Name;
    QString var1Label;
    QString var2Name;
    QString var2Label;
};
class VariableProcessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableProcessDialog(QWidget *parent = nullptr);
   // QStringList getExpressions() const;
     void addExpressionRowparametre(int operationType, const QString &expression = QString());

    void addExpressionRow();
     QList<ProcessRecord> getExpressionsWithType() const;
private slots:

    void updateExpressionRowWidgets(int index);
    void updateVariableTargetName(int index, int loopRowIndex);
    void updateVariableVar1Name(int index, int loopRowIndex);
    void updateVariableVar2Name(int index, int loopRowIndex);
private:
    struct ProcessRow {
        QWidget *widget;
        QLineEdit *variableTargetNameEdit;
        QComboBox *variableTargetCombo;
        QLabel *equalLabel;
        QComboBox *operationTypeCombo;
        QLineEdit *variableVar1NameEdit;
        QComboBox *var1Combo;
        QComboBox *operatorCombo;
        QLineEdit *variableVar2NameEdit;
        QComboBox *var2Combo;
        QLineEdit *constEdit1;
         QLineEdit *constEdit2;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<ProcessRow*> processRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEEXPRESSIONDIALOG_H
