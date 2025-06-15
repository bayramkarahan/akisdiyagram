#include "variableeditordialog.h"
#include <QHeaderView>
#include <QMessageBox>

VariableEditorDialog::VariableEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Variable Editor");
    resize(230, 200);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Variable" << "Value" << "Type");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    // Sütun başlıklarını sabit tutmak istiyorsanız:
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->setColumnWidth(0, 60); // Label
    tableWidget->setColumnWidth(1, 60); // Value
    tableWidget->setColumnWidth(2, 60); // Type

    addButton = new QPushButton("Ekle", this);
    removeButton = new QPushButton("Sil", this);
    //saveButton = new QPushButton("Kaydet", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();
   // buttonLayout->addWidget(saveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &VariableEditorDialog::addVariable);
    connect(removeButton, &QPushButton::clicked, this, &VariableEditorDialog::removeSelectedVariable);
    //connect(saveButton, &QPushButton::clicked, this, &VariableEditorDialog::updateVariablesFromTable);

    loadVariables();
}

void VariableEditorDialog::loadVariables()
{
    tableWidget->setRowCount(0);
    for (const VariableRecord &var : Variable::onlineVariableList) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(var.label));
        tableWidget->setItem(row, 1, new QTableWidgetItem(var.value));

        // ComboBox oluştur
        QComboBox *combo = new QComboBox();
        combo->addItems({"text", "number"});
        combo->setCurrentText(var.type);
        connect(combo, &QComboBox::currentTextChanged, this, &VariableEditorDialog::onTypeChanged);
        tableWidget->setCellWidget(row, 2, combo);
    }
}

void VariableEditorDialog::addVariable()
{
    int row = tableWidget->rowCount();
    QComboBox *combo = new QComboBox();
    combo->addItems({"number", "text"});
    combo->setCurrentText("number");
    connect(combo, &QComboBox::currentTextChanged, this, &VariableEditorDialog::onTypeChanged);

    // Listeye boş VariableRecord da ekle
    VariableRecord rec;
    rec.label ="var"+QString::number(row);
    rec.value = "0";
    rec.type = combo->currentText();

    Variable::onlineVariableList.append(rec);

    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(rec.label));
    tableWidget->setItem(row, 1, new QTableWidgetItem(rec.value));
    tableWidget->setCellWidget(row, 2, combo);

}

void VariableEditorDialog::removeSelectedVariable()
{
    int row = tableWidget->currentRow();
    if (row >= 0)
        tableWidget->removeRow(row);
}

void VariableEditorDialog::updateVariablesFromTable()
{
    Variable::onlineVariableList.clear();
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        VariableRecord rec;
        rec.label = tableWidget->item(row, 0)->text();
        rec.value = tableWidget->item(row, 1)->text();

        QComboBox *combo = qobject_cast<QComboBox *>(tableWidget->cellWidget(row, 2));
        if (combo)
        {
            rec.type = combo->currentText();
            if(combo->currentText()=="number")
            rec.value = "0";
            else
            rec.value = "";

            tableWidget->setItem(row, 1, new QTableWidgetItem(rec.value));
        }

        Variable::onlineVariableList.append(rec);
    }
   // QMessageBox::information(this, "Bilgi", "Değişken listesi güncellendi.");
}

void VariableEditorDialog::onTypeChanged(const QString &type)
{
    qDebug()<<"güncellendi..";
    updateVariablesFromTable();
   /* QComboBox *combo = qobject_cast<QComboBox *>(sender());
    if (!combo)
        return;

    int row = -1;
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        if (tableWidget->cellWidget(i, 2) == combo) {
            row = i;
            break;
        }
    }

    if (row != -1 && row < Variable::onlineVariableList.size()) {
        Variable::onlineVariableList[row].type = type;
    }*/
}

