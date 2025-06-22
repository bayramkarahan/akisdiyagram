#include "variableeditform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

VariableEditForm::VariableEditForm(const VariableRecord &record, QWidget *parent)
    : QDialog(parent), currentRecord(record)
{
    setWindowTitle("Değişkeni Düzenle");
    resize(230, 120);

    labelEdit = new QLineEdit(record.label, this);
    valueEdit = new QLineEdit(record.value, this);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({"number", "text"});
    typeCombo->setCurrentText(record.valueType);

    okButton = new QPushButton("Kaydet", this);
    cancelButton = new QPushButton("İptal", this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Değiken Adı:", labelEdit);
    formLayout->addRow("Değeri:", valueEdit);
    formLayout->addRow("Veri Türü:", typeCombo);

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
        currentRecord.valueType = typeCombo->currentText();
        accept();
    });

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

VariableRecord VariableEditForm::getRecord() const
{
    return currentRecord;
}
