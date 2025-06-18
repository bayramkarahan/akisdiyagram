#include "variable_selection_dialog.h"
#include <QDebug>

VariableSelectionDialog::VariableSelectionDialog(QWidget *parent)
    : QDialog(parent)
{


    setWindowTitle("Değişken Seçimi");
    resize(500, 300);

    auto mainLayout = new QVBoxLayout(this);

    rowsLayout = new QVBoxLayout();
    mainLayout->addLayout(rowsLayout);

    addButton = new QPushButton("Satır Ekle", this);
    mainLayout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, [this]() {
        addVariableRow();
    });

    // OK ve Cancel butonları
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // Butonlara erişip metinleri değiştiriyoruz
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
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
    ///row->variableCombo->addItems(variableLabels().constFirst().first);
    for (const auto &pair : variableLabels()) {
        //row->variableCombo->addItem(pair.second);  // veya .first
        row->variableCombo->addItem(pair.second, pair.first);  // Görünen label, data olarak name
    }
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
        var.name = row->variableCombo->currentText();
        var.label = row->variableCombo->currentText();
        var.isInput = row->inputCheckBox->isChecked();
        var.value = row->valueEdit->text();

        // type bilgisi için allVariables'dan eşleştirme
        for (const VariableRecord &v : Variable::onlineVariableList) {
            if (v.name == var.name) {
                var.type = v.type;
                break;
            }
        }

        selected.append(var);
    }
    return selected;
}

QList<QPair<QString, QString>> VariableSelectionDialog::variableLabels() const
{
     QList<QPair<QString, QString>> list;

    for (const VariableRecord &var : Variable::onlineVariableList) {
        //labels.insert(var.label);
        list.append(qMakePair(var.name, var.label));
    }
    return list;
}
