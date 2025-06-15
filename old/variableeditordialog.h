#ifndef VARIABLEEDITORDIALOG_H
#define VARIABLEEDITORDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "variable.h"
#include <QComboBox>
#include <QDebug>

class VariableEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableEditorDialog(QWidget *parent = nullptr);

private slots:
    void loadVariables();
    void addVariable();
    void removeSelectedVariable();
    void updateVariablesFromTable();
        void onTypeChanged(const QString &type);

private:
    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *saveButton;
};

#endif // VARIABLEEDITORDIALOG_H
