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

class VariableExpressionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableExpressionDialog(QWidget *parent = nullptr);
   // QStringList getExpressions() const;
     void addExpressionRowparametre(int operationType, const QString &expression = QString());

    void addExpressionRow();
    QList<QPair<int, QString>> getExpressionsWithType() const;
private slots:

    void updateExpressionRowWidgets(int index);

private:
    struct ExpressionRow {
        QWidget *widget;
        QComboBox *targetVarCombo;
        QLabel *equalLabel;
        QComboBox *operationTypeCombo;
        QComboBox *var1Combo;
        QComboBox *operatorCombo;
        QComboBox *var2Combo;
        QLineEdit *constEdit1;
         QLineEdit *constEdit2;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<ExpressionRow*> expressionRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEEXPRESSIONDIALOG_H
