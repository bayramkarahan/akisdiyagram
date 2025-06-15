#include "variableoutputdialog.h"
#include <QDialogButtonBox>
#include <QSet>

VariableOutputDialog::VariableOutputDialog(QWidget *parent)
    : QDialog(parent)
{
    allVariables = Variable::onlineVariableList;

    setWindowTitle("Çıktı Değişkenleri");
    resize(400, 300);

    mainLayout = new QVBoxLayout(this);
    rowsLayout = new QVBoxLayout();
    mainLayout->addLayout(rowsLayout);

    addButton = new QPushButton("Satır Ekle", this);
    connect(addButton, &QPushButton::clicked, this, [this]() {
        addVariableRow();
    });
    mainLayout->addWidget(addButton);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    //addVariableRow(); // İlk satır
}

void VariableOutputDialog::addVariableRow(const VariableRecord &var)
{
    VariableRow *row = new VariableRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(0, 0, 0, 0);

    row->variableCombo = new QComboBox(row->widget);
    row->variableCombo->addItems(variableLabels());
    if (!var.label.isEmpty()) {
        int index = row->variableCombo->findText(var.label);
        if (index >= 0)
            row->variableCombo->setCurrentIndex(index);
    }
    layout->addWidget(row->variableCombo);

    row->removeButton = new QPushButton("Sil", row->widget);
    layout->addWidget(row->removeButton);

    rowsLayout->addWidget(row->widget);
    variableRows.append(row);

    int rowIndex = variableRows.size() - 1;
    connect(row->removeButton, &QPushButton::clicked, this, [this, rowIndex]() {
        removeVariableRow(rowIndex);
    });
}

void VariableOutputDialog::removeVariableRow(int index)
{
    if (index < 0 || index >= variableRows.size())
        return;

    VariableRow *row = variableRows.takeAt(index);
    rowsLayout->removeWidget(row->widget);
    row->widget->deleteLater();

    // Bağlantıları yeniden kur
    for (int i = 0; i < variableRows.size(); ++i) {
        disconnect(variableRows[i]->removeButton, nullptr, nullptr, nullptr);
        connect(variableRows[i]->removeButton, &QPushButton::clicked, this, [this, i]() {
            removeVariableRow(i);
        });
    }
}

QList<VariableRecord> VariableOutputDialog::getSelectedVariables() const
{
    QList<VariableRecord> selected;

    for (const VariableRow *row : variableRows) {
        QString label = row->variableCombo->currentText();
        for (const VariableRecord &var : allVariables) {
            if (var.label == label) {
                selected.append(var);
                break;
            }
        }
    }

    return selected;
}

QStringList VariableOutputDialog::variableLabels() const
{
    QSet<QString> labels;
    for (const VariableRecord &var : allVariables) {
        labels.insert(var.label);
    }
    return labels.values();
}
