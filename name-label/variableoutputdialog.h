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
struct OutputRecord {
    int outputType;
    QString expression;
    QString name;
    QString label;
};
class VariableOutputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableOutputDialog(QWidget *parent = nullptr);
    void addExpressionRowparametre(int operationType, const QString &expression = QString());
    void addExpressionRow();
    QList<OutputRecord> getExpressionsWithType() const;
private slots:
    void updateExpressionRowWidgets(int index);
    void updateVariableName(int index,int loopRowIndex);
private:
    struct OutputRow {
        QWidget *widget;
        QLineEdit *varNameEdit;
        QComboBox *varLabelCombo;
        QComboBox *operationTypeCombo;
        QLineEdit *constEdit1;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<OutputRow*> outputRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEOUTPUTDIALOG_H
