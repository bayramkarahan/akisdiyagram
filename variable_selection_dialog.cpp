#include "variable_selection_dialog.h"
#include <QDebug>

VariableSelectionDialog::VariableSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    allVariables = Variable::onlineVariableList;

    setWindowTitle("Değişken Seçimi");
    resize(500, 300);

    mainLayout = new QVBoxLayout(this);

    rowsLayout = new QVBoxLayout();
    mainLayout->addLayout(rowsLayout);

    addButton = new QPushButton("Satır Ekle", this);
    mainLayout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, [this]() {
        addVariableRow();
    });

    // OK ve Cancel butonları
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Başlangıçta 1 satır ekle
   // addVariableRow();
}

void VariableSelectionDialog::addVariableRow(const VariableRecord &var)
{
    VariableRow *row = new VariableRow;

    row->widget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(row->widget);
    hLayout->setContentsMargins(0, 0, 0, 0);

    // Değişken seçimi combobox
    row->variableCombo = new QComboBox(row->widget);
    row->variableCombo->addItems(variableLabels());
    if (!var.label.isEmpty()) {
        int idx = row->variableCombo->findText(var.label);
        if (idx >= 0) row->variableCombo->setCurrentIndex(idx);
    }
    hLayout->addWidget(row->variableCombo);

    // Girdi checkbox
    row->inputCheckBox = new QCheckBox("Girdi", row->widget);
    row->inputCheckBox->setChecked(var.isInput);
    hLayout->addWidget(row->inputCheckBox);

    // Value edit
    row->valueEdit = new QLineEdit(row->widget);
    row->valueEdit->setPlaceholderText("Değer");
    row->valueEdit->setText(var.value);
    hLayout->addWidget(row->valueEdit);

    // Sil butonu
    row->removeButton = new QPushButton("Sil", row->widget);
    hLayout->addWidget(row->removeButton);

    rowsLayout->addWidget(row->widget);
    variableRows.append(row);

    int index = variableRows.size() - 1;
    connect(row->removeButton, &QPushButton::clicked, this, [this, index]() {
        removeVariableRow(index);
    });
}

void VariableSelectionDialog::removeVariableRow(int index)
{
    if (index < 0 || index >= variableRows.size())
        return;

    VariableRow *row = variableRows.at(index);
    rowsLayout->removeWidget(row->widget);
    row->widget->deleteLater();
    variableRows.removeAt(index);

    // Bağlantıları güncelle (index için)
    for (int i = 0; i < variableRows.size(); ++i) {
        disconnect(variableRows[i]->removeButton, nullptr, nullptr, nullptr);
        connect(variableRows[i]->removeButton, &QPushButton::clicked, this, [this, i]() {
            removeVariableRow(i);
        });
    }
}

QList<VariableRecord> VariableSelectionDialog::getSelectedVariables() const
{
    QList<VariableRecord> selected;

    for (const VariableRow *row : variableRows) {
        VariableRecord var;
        var.label = row->variableCombo->currentText();
        var.isInput = row->inputCheckBox->isChecked();
        var.value = row->valueEdit->text();

        // type bilgisi için allVariables'dan eşleştirme
        for (const VariableRecord &v : allVariables) {
            if (v.label == var.label) {
                var.type = v.type;
                break;
            }
        }

        selected.append(var);
    }
    return selected;
}

QStringList VariableSelectionDialog::variableLabels() const
{
    QSet<QString> labels;
    for (const VariableRecord &var : allVariables) {
        labels.insert(var.label);
    }
    return labels.values();
}
