#include "variableconditiondialog.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>
#include<QApplication>

VariableConditionDialog::VariableConditionDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Koşul Tanımla");
    resize(1200, 200);

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
    connect(addButton, &QPushButton::clicked, this, &VariableConditionDialog::addExpressionRow);

    // Expressions ve addButton aynı layoutta (scroll içinde)
    expressionsLayout->addWidget(addButton);

    // Ana layouta scrollArea ekle
    mainLayout->addWidget(scrollArea);

    // OK ve Cancel butonları ana layoutun en altında
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QStringList VariableConditionDialog::variableLabels() const
{
    QStringList labels;
    for (const VariableRecord &var : Variable::onlineVariableList) {
        labels.append(var.label);
    }
    return labels;
}

void VariableConditionDialog::addExpressionRow()
{
   /* if (!expressionRows.isEmpty()) {
        ExpressionRow *lastRow = expressionRows.last();
        if (lastRow->targetVarCombo->currentText().isEmpty() &&
            lastRow->operationTypeCombo->currentIndex() == 0 &&
            lastRow->constEdit1->text().isEmpty()) {
            return; // Son satır boş, yeni satır ekleme
        }
    }*/
    ExpressionRow *row = new ExpressionRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5, 5, 5, 5);


    // İşlem türü combo
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Sabit Koşulu (var0 > 5)",
        "🧮 Değişken Değişken Koşulu (var0 > var1)",
        "➕ Değişken Sabit Koşulu ve  Değişken Değişken Koşulu (var0 > 5 && var2 > var2)",
        "➕ Değişken Sabit Koşulu ve Değişken Sabit Koşulu (var0 > 5 && var2 > 6)",
        "➕ Değişken Değişken Koşulu ve Değişken Değişken Koşulu (var0 > var1 && var2 > var3)",
    });
    layout->addWidget(row->operationTypeCombo);

    // Hedef değişken combo
    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);

    // operatör1 combo
    row->operator1Combo = new QComboBox(row->widget);
    row->operator1Combo->addItems({"<", "<=", ">", ">=","==","!="});
    layout->addWidget(row->operator1Combo);

    // Hedef değişken combo
    row->var2Combo = new QComboBox(row->widget);
    row->var2Combo->addItems(variableLabels());
    layout->addWidget(row->var2Combo);

    // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);

    // logicOperatör2 combo
    row->logicOperatorCombo = new QComboBox(row->widget);
    row->logicOperatorCombo->addItems({"&&", "||", "!"});
    layout->addWidget(row->logicOperatorCombo);



    // var3 değişken combo
    row->var3Combo = new QComboBox(row->widget);
    row->var3Combo->addItems(variableLabels());
    layout->addWidget(row->var3Combo);

    // operatör2 combo
    row->operator2Combo = new QComboBox(row->widget);
    row->operator2Combo->addItems({"<", "<=", ">", ">=","==","!="});
    layout->addWidget(row->operator2Combo);

    // var4 değişken combo
    row->var4Combo = new QComboBox(row->widget);
    row->var4Combo->addItems(variableLabels());
    layout->addWidget(row->var4Combo);


    // Sabit sayı için edit
    row->constEdit2 = new QLineEdit(row->widget);
    row->constEdit2->setFixedWidth(50);
    row->constEdit2->setPlaceholderText("Sayı2");
    layout->addWidget(row->constEdit2);


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

void VariableConditionDialog::addExpressionRowparametre(int operationType, const QString &expression)
{
   /* if (!expressionRows.isEmpty()) {
        ExpressionRow *lastRow = expressionRows.last();
        if (lastRow->targetVarCombo->currentText().isEmpty() &&
            lastRow->operationTypeCombo->currentIndex() == 0 &&
            lastRow->constEdit1->text().isEmpty()&&lastRow->constEdit2->text().isEmpty()) {
            return; // Son satır boş, yeni satır ekleme
        }
    }*/
    ExpressionRow* row = new ExpressionRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);

    // İşlem türü combo
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Sabit Koşulu (var0 > 5)",
        "🧮 Değişken Değişken Koşulu (var0 > var1)",
        "➕ Değişken Sabit Koşulu ve  Değişken Değişken Koşulu (var0 > 5 && var2 > var2)",
        "➕ Değişken Sabit Koşulu ve Değişken Sabit Koşulu (var0 > 5 && var2 > 6)",
        "➕ Değişken Değişken Koşulu ve Değişken Değişken Koşulu (var0 > var1 && var2 > var3)",
    });
    layout->addWidget(row->operationTypeCombo);

    // Hedef değişken combo
    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);

    // operatör1 combo
    row->operator1Combo = new QComboBox(row->widget);
    row->operator1Combo->addItems({"<", "<=", ">", ">=","==","!="});
    layout->addWidget(row->operator1Combo);

    // Hedef değişken combo
    row->var2Combo = new QComboBox(row->widget);
    row->var2Combo->addItems(variableLabels());
    layout->addWidget(row->var2Combo);

    // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);

    // logicOperatör2 combo
    row->logicOperatorCombo = new QComboBox(row->widget);
    row->logicOperatorCombo->addItems({"&&", "||", "!"});
    layout->addWidget(row->logicOperatorCombo);



    // var3 değişken combo
    row->var3Combo = new QComboBox(row->widget);
    row->var3Combo->addItems(variableLabels());
    layout->addWidget(row->var3Combo);

    // operatör2 combo
    row->operator2Combo = new QComboBox(row->widget);
    row->operator2Combo->addItems({"<", "<=", ">", ">=","==","!="});
    layout->addWidget(row->operator2Combo);

    // var4 değişken combo
    row->var4Combo = new QComboBox(row->widget);
    row->var4Combo->addItems(variableLabels());
    layout->addWidget(row->var4Combo);


    // Sabit sayı için edit
    row->constEdit2 = new QLineEdit(row->widget);
    row->constEdit2->setFixedWidth(50);
    row->constEdit2->setPlaceholderText("Sayı2");
    layout->addWidget(row->constEdit2);


    row->removeButton = new QPushButton("🗑️", row->widget);
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    // İşlem türünü dışarıdan alıyoruz, combo'yu ayarla
    row->operationTypeCombo->setCurrentIndex(operationType);

    // expression'u parçalayıp ilgili widgetlara ayırmak yerine direkt dolduralım:

     //" " ile böl
    QStringList parts = expression.split(' ');
    if(parts.size() > 2) {


        switch(operationType) {
        case 0: // Sabit Atama (var0 > 5)
        {
            QString var1 = parts[0].trimmed();
            QString opt1 = parts[1].trimmed();
            QString num1 = parts[2].trimmed();

            int var1Index = row->var1Combo->findText(var1);
            if(var1Index >= 0) row->var1Combo->setCurrentIndex(var1Index);

            int opt1Index = row->operator1Combo->findText(opt1);
            if(opt1Index >= 0) row->operator1Combo->setCurrentIndex(opt1Index);

            row->constEdit1->setText(num1);
        }
            break;
        }
    }
       /* case 1: // Değişken Atama (var = var1)
        {
            int idx = row->var1Combo->findText(rightExpr);
            if (idx >= 0) row->var1Combo->setCurrentIndex(idx);
        }
        break;
        case 2: // İki Değişkenli İşlem (var = var1 + var2)
        {
            // rightExpr örn: "var1 + var2"
            QStringList parts2 = rightExpr.split(QRegExp("\\s*[+\\-*////]\\s*"));
           /* if(parts2.size() == 2) {
                int idx1 = row->var1Combo->findText(parts2[0]);
                int idx2 = row->var2Combo->findText(parts2[1]);
                if(idx1 >= 0) row->var1Combo->setCurrentIndex(idx1);
                if(idx2 >= 0) row->var2Combo->setCurrentIndex(idx2);

                // operatör bul
                QRegExp opRx("\\+|\\-|\\*|\\/");
                if(opRx.indexIn(rightExpr) >= 0) {
                    QString op = opRx.cap(0);
                    int opIdx = row->operatorCombo->findText(op);
                    if(opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);
                }
            }
        }
        break;
        case 3: // Değişken + Sabit İşlem (var = var1 + 5)
        {
            // örnek: "var1 + 6"
            QRegExp expRx("^(\\w+)\\s*([+\\-*////])\\s*(\\d+)$");
           /* if (expRx.exactMatch(rightExpr)) {
                QString var1 = expRx.cap(1);
                QString op = expRx.cap(2);
                QString num = expRx.cap(3);

                int idx1 = row->var1Combo->findText(var1);
                if (idx1 >= 0) row->var1Combo->setCurrentIndex(idx1);

                int opIdx = row->operatorCombo->findText(op);
                if (opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);

                row->constEdit1->setText(num);
            }
        }
        break;
        case 4: // Değişken + Sabit İşlem (var = 3 + 5)
        {
            // örnek: "3 + 6"
            QRegExp expRx("^(\\w+)\\s*([+\\-*////])\\s*(\\d+)$");
          /*  if (expRx.exactMatch(rightExpr)) {
                QString num1 = expRx.cap(1);
                QString op = expRx.cap(2);
                QString num2 = expRx.cap(3);

                // int idx1 = row->var1Combo->findText(var1);
                // if (idx1 >= 0) row->var1Combo->setCurrentIndex(idx1);

                int opIdx = row->operatorCombo->findText(op);
                if (opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);

                row->constEdit1->setText(num1);
                row->constEdit2->setText(num2);

            }
        }
        break;
        }
    }
*/
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

void VariableConditionDialog::updateExpressionRowWidgets(int index)
{
    if (index < 0 || index >= expressionRows.count())
        return;

    ExpressionRow *row = expressionRows.at(index);
    int type = row->operationTypeCombo->currentIndex();

    // Önce tüm ek widgetları gizle
    row->var1Combo->setVisible(false);
    row->var2Combo->setVisible(false);
    row->var3Combo->setVisible(false);
    row->var4Combo->setVisible(false);
    row->operator1Combo->setVisible(false);
    row->operator2Combo->setVisible(false);
    row->logicOperatorCombo->setVisible(false);
    row->constEdit1->setVisible(false);
    row->constEdit2->setVisible(false);

    switch (type) {
    case 0: // Sabit koşul: var1 > 5
        row->var1Combo->setVisible(true);
        row->operator1Combo->setVisible(true);
        row->constEdit1->setVisible(true);
        break;
    case 1: // Değişken koşul: var1 > var2
        row->var1Combo->setVisible(true);
        row->operator1Combo->setVisible(true);
        row->var2Combo->setVisible(true);
        break;
    case 2: // Sabit koşul ve İki Değişkenli koşul: var0 > 5 && var1 > var2
        row->var1Combo->setVisible(true);
        row->operator1Combo->setVisible(true);
        row->constEdit1->setVisible(true);
        row->logicOperatorCombo->setVisible(true);
        row->var3Combo->setVisible(true);
        row->operator2Combo->setVisible(true);
        row->var4Combo->setVisible(true);
        break;
    case 3: // Sabit koşul ve Sabit koşul İşlem: var0 > 5 && var1 > 6
        row->var1Combo->setVisible(true);
        row->operator1Combo->setVisible(true);
        row->constEdit1->setVisible(true);
        row->logicOperatorCombo->setVisible(true);
        row->var3Combo->setVisible(true);
        row->operator2Combo->setVisible(true);
        row->constEdit2->setVisible(true);
        break;
    case 4: // İki Değişkenli koşul ve İki Değişkenli koşul: var0 > var1 && var2 > var3
        row->var1Combo->setVisible(true);
        row->operator1Combo->setVisible(true);
        row->var2Combo->setVisible(true);
        row->logicOperatorCombo->setVisible(true);
        row->var3Combo->setVisible(true);
        row->operator2Combo->setVisible(true);
        row->var4Combo->setVisible(true);
        break;
    }
}

QList<QPair<int, QString>> VariableConditionDialog::getExpressionsWithType() const
{
    QList<QPair<int, QString>> list;
    for (ExpressionRow *row : expressionRows) {
        QString expr;
        int type = row->operationTypeCombo->currentIndex();

        switch (type) {
        case 0:
            expr = QString("%1 %2 %3").arg(row->var1Combo->currentText())
                       .arg(row->operator1Combo->currentText())
                       .arg(row->constEdit1->text());
            break;
        case 1:
            expr = QString("%1 %2 %3").arg(row->var1Combo->currentText())
                       .arg(row->operator1Combo->currentText())
                       .arg(row->var2Combo->currentText());
            break;
        case 2:
            expr = QString("%1 %2 %3 %4 %5 %6 %7")
                       .arg(row->var1Combo->currentText())
                       .arg(row->operator1Combo->currentText())
                       .arg(row->constEdit1->text())
                       .arg(row->logicOperatorCombo->currentText())
                       .arg(row->var3Combo->currentText())
                       .arg(row->operator2Combo->currentText())
                       .arg(row->var4Combo->currentText());
            break;
        case 3:
            expr = QString("%1 %2 %3 %4 %5 %6 %7")
                       .arg(row->var1Combo->currentText())
                       .arg(row->operator1Combo->currentText())
                       .arg(row->constEdit1->text())
                       .arg(row->logicOperatorCombo->currentText())
                       .arg(row->var3Combo->currentText())
                       .arg(row->operator2Combo->currentText())
                       .arg(row->constEdit1->text());
            break;
        case 4:
            expr = QString("%1 %2 %3 %4 %5 %6 %7")
                       .arg(row->var1Combo->currentText())
                       .arg(row->operator1Combo->currentText())
                       .arg(row->var2Combo->currentText())
                       .arg(row->logicOperatorCombo->currentText())
                       .arg(row->var3Combo->currentText())
                       .arg(row->operator2Combo->currentText())
                       .arg(row->var4Combo->currentText());
            break;
        }
        list.append(qMakePair(type, expr));
    }
    return list;
}
