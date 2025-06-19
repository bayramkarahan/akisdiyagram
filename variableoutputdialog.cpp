#include "variableoutputdialog.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>
#include<QApplication>
VariableOutputDialog::VariableOutputDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("İşlem Tanımla");
    resize(650, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Scroll alanı içinde expression widget'ları
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollWidget);

    expressionsLayout = new QVBoxLayout(scrollWidget);
    expressionsLayout->setContentsMargins(5,5,5,5);
    expressionsLayout->setSpacing(10);

    // İlk işlem satırı veya boş başlat
    //addExpressionRow();

    // "Yeni İşlem Ekle" butonu
    addButton = new QPushButton("Yeni İşlem Ekle", this);
    connect(addButton, &QPushButton::clicked, this, &VariableOutputDialog::addExpressionRow);

    // Expressions ve addButton aynı layoutta (scroll içinde)
    expressionsLayout->addWidget(addButton);

    // Ana layouta scrollArea ekle
    mainLayout->addWidget(scrollArea);

    // OK ve Cancel butonları ana layoutun en altında
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


QStringList VariableOutputDialog::variableLabels() const
{
    QStringList labels;
    for (const VariableRecord &var : Variable::onlineVariableList) {
        labels.append(var.label);
    }
    return labels;
}

void VariableOutputDialog::addExpressionRow()
{

    ExpressionRow *row = new ExpressionRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5, 5, 5, 5);


    // İşlem türü combo
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Göster (var0)",
        "📥 Değişken Adı İle Göster (var0 = 123)",
        "📥 Sabit Text Göster",
    });
    layout->addWidget(row->operationTypeCombo);


    // var1 combo
    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);

    // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);

    // Kaldırma butonu
    row->removeButton = new QPushButton("🗑️", row->widget);
    row->removeButton->setToolTip("İşlemi Sil");
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    expressionRows.append(row);

    // İlk widget görünürlüğünü ayarla
    updateExpressionRowWidgets(expressionRows.count() - 1);

    // İşlem türü değiştiğinde widgetları güncelle
    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        updateExpressionRowWidgets(expressionRows.indexOf(row));
    });

    // Kaldırma butonu
    connect(row->removeButton, &QPushButton::clicked, this, [=](){
        int idx = expressionRows.indexOf(row);
        if (idx >= 0) {
            expressionRows.removeAt(idx);
            expressionsLayout->removeWidget(row->widget);
            row->widget->deleteLater();
            delete row;
        }
    });
}

void VariableOutputDialog::addExpressionRowparametre(int operationType, const QString &expression)
{


    ExpressionRow* row = new ExpressionRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Göster (var0)",
        "📥 Değişken Adı İle Göster (var0 = 123)",
        "📥 Sabit Text Göster",
    });
    layout->addWidget(row->operationTypeCombo);



    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);

    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);


    row->removeButton = new QPushButton("🗑️", row->widget);
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    // İşlem türünü dışarıdan alıyoruz, combo'yu ayarla
    row->operationTypeCombo->setCurrentIndex(operationType);

    // expression'u parçalayıp ilgili widgetlara ayırmak yerine direkt dolduralım:

    // "=" ile böl


        int targetIndex = row->var1Combo->findText(expression.trimmed());
        if(targetIndex >= 0) row->var1Combo->setCurrentIndex(targetIndex);

        switch(operationType) {
        case 0: // Değişken Değeri Atama (var0)
        {
            int idx = row->var1Combo->findText(expression.trimmed());
            if (idx >= 0) row->var1Combo->setCurrentIndex(idx);
        }
            break;
        case 1: // Değişken Adı ve Değeri(var0 = 5)
        {
            int idx = row->var1Combo->findText(expression.trimmed());
            if (idx >= 0) row->var1Combo->setCurrentIndex(idx);
        }
        break;
        case 2: // Sabit Text Bilgi ()
        {
            row->constEdit1->setText(expression.trimmed());
        }
        break;
         }


    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        int idx = expressionRows.indexOf(row);
        if(idx >= 0) updateExpressionRowWidgets(idx);
    });

    connect(row->removeButton, &QPushButton::clicked, this, [=]() {
        expressionsLayout->removeWidget(row->widget);
        row->widget->deleteLater();
        expressionRows.removeOne(row);
    });

    expressionRows.append(row);

    updateExpressionRowWidgets(expressionRows.indexOf(row));
}

void VariableOutputDialog::updateExpressionRowWidgets(int index)
{
    if (index < 0 || index >= expressionRows.count())
        return;

    ExpressionRow *row = expressionRows.at(index);
    int type = row->operationTypeCombo->currentIndex();

    // Önce tüm ek widgetları gizle
    row->var1Combo->setVisible(false);
    row->constEdit1->setVisible(false);

    switch (type) {
    case 0: // Sabit Değer: var0
        row->var1Combo->setVisible(true);
        break;
    case 1: // Değişken Değer: var0 = 123
        row->var1Combo->setVisible(true);
        break;
    case 2: // Sabit text Değer
        row->constEdit1->setVisible(true);
        break;
    }
}

QList<QPair<int, QString>> VariableOutputDialog::getExpressionsWithType() const
{
    QList<QPair<int, QString>> list;
    for (ExpressionRow *row : expressionRows) {
        QString expr;
        QString target = row->var1Combo->currentText();
        int type = row->operationTypeCombo->currentIndex();

        switch (type) {
        case 0:
            expr = QString("%1").arg(target).arg(row->var1Combo->currentText());
              break;
        case 1:
            expr = QString("%1").arg(target).arg(row->var1Combo->currentText());
            break;
        case 2:
            expr = QString("%1")
                       .arg(row->constEdit1->text());
            break;

        }
        list.append(qMakePair(type, expr));
    }
    return list;
}
