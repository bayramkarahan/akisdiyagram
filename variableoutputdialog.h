#ifndef VARIABLEOUTPUTDIALOG_H
#define VARIABLEOUTPUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include "variable.h"

class VariableOutputDialog : public QDialog {
    Q_OBJECT

public:
    explicit VariableOutputDialog(QWidget *parent = nullptr);
    QList<VariableRecord> getSelectedVariables() const;
    void addVariableRow(const VariableRecord &var = VariableRecord());

private:
    struct VariableRow {
        QWidget *widget;
        QComboBox *variableCombo;
        QPushButton *removeButton;
    };

    QVBoxLayout *mainLayout;
    QVBoxLayout *rowsLayout;
    QPushButton *addButton;
    QDialogButtonBox *buttonBox;

    QList<VariableRecord> allVariables;
    QList<VariableRow*> variableRows;

    void removeVariableRow(int index);
    QStringList variableLabels() const;
};

#endif // VARIABLEOUTPUTDIALOG_H
