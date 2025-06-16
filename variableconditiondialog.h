#ifndef VARIABLECONDITIONDIALOG_H
#define VARIABLECONDITIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QLabel>
#include <QFrame>
#include<QDialogButtonBox>

class ConditionRow : public QWidget {
    Q_OBJECT
public:
    ConditionRow(const QStringList &variables, QWidget *parent = nullptr);
    QString getExpression() const;
    int getOperationType() const;

private slots:
    void updateRightInputField(const QString &type);

private:
    QComboBox *typeCombo;
    QComboBox *leftCombo;
    QComboBox *opCombo;
    QWidget *rightInputWidget;
    QComboBox *rightCombo;
    QLineEdit *rightLineEdit;

    QStringList variableList;
};

class VariableConditionDialog : public QDialog {
    Q_OBJECT
public:
    explicit VariableConditionDialog(QWidget *parent = nullptr);
    void addExpressionRow(int operationType = 0, const QString &expression = "");
    QList<QPair<int, QString>> getExpressionsWithType() const;

private slots:
    void addRow();
    void removeLastRow();

private:
    QVBoxLayout *rowsLayout;
    QPushButton *addRowButton;
    QPushButton *removeRowButton;
    QDialogButtonBox *buttonBox;

    QStringList getVariableLabels() const;
};

#endif // VARIABLECONDITIONDIALOG_H
