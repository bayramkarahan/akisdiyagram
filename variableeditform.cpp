#include "variableeditform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

VariableEditForm::VariableEditForm(const VariableRecord &record, QWidget *parent)
    : QDialog(parent), currentRecord(record)
{
    setWindowTitle("Değişkeni Düzenle");

    labelEdit = new QLineEdit(record.label, this);
    valueEdit = new QLineEdit(record.value, this);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({"number", "text"});
    typeCombo->setCurrentText(record.type);

    okButton = new QPushButton("Kaydet", this);
    cancelButton = new QPushButton("İptal", this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Label:", labelEdit);
    formLayout->addRow("Value:", valueEdit);
    formLayout->addRow("Type:", typeCombo);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, [=]() {
        currentRecord.label = labelEdit->text();
        currentRecord.value = valueEdit->text();
        currentRecord.type = typeCombo->currentText();
        accept();
    });

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

VariableRecord VariableEditForm::getRecord() const
{
    return currentRecord;
}
