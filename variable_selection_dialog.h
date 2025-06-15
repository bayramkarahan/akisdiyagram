#ifndef VARIABLESELECTIONDIALOG_H
#define VARIABLESELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include "variable.h"

class VariableSelectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit VariableSelectionDialog(QWidget *parent = nullptr);
    QList<VariableRecord> getSelectedVariables() const;
    void addVariableRow(const VariableRecord &var = VariableRecord());

private:
    struct VariableRow {
        QWidget *widget;
        QComboBox *variableCombo;
        QCheckBox *inputCheckBox;
        QLineEdit *valueEdit;
        QPushButton *removeButton;
    };

    QVBoxLayout *mainLayout;
    QVBoxLayout *rowsLayout;
    QPushButton *addButton;

    QList<VariableRecord> allVariables;
    QList<VariableRow*> variableRows;

     void removeVariableRow(int index);
    QStringList variableLabels() const;
};

#endif // VARIABLESELECTIONDIALOG_H
