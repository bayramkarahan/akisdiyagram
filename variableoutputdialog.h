#ifndef VARIABLEOUTPUTDIALOG_H
#define VARIABLEOUTPUTDIALOG_H

#include <QDialog>
#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "variable.h"

class VariableOutputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableOutputDialog(QWidget *parent = nullptr);
    void addExpressionRowparametre(int operationType, const QString &expression = QString());
    void addExpressionRow();
    QList<QPair<int, QString>> getExpressionsWithType() const;
private slots:
    void updateExpressionRowWidgets(int index);

private:
    struct ExpressionRow {
        QWidget *widget;
        QComboBox *var1Combo;
        QComboBox *operationTypeCombo;
        QLineEdit *constEdit1;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<ExpressionRow*> expressionRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEOUTPUTDIALOG_H
