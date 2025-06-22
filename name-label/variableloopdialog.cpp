#include "variableloopdialog.h"
#include "variable.h"

VariableLoopDialog::VariableLoopDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Tekrar(Döngü) Tanımla");
    resize(500, 200);
    mainLayout = new QVBoxLayout(this);
    loopRowsLayout = new QVBoxLayout;

    mainLayout->addLayout(loopRowsLayout);

   // QPushButton *addButton = new QPushButton("📥 Satır Ekle");
   // mainLayout->addWidget(addButton);

   /* connect(addButton, &QPushButton::clicked, this, [=]() {
        addLoopRowparametre(0, "");
    });*/

    // OK ve Cancel butonları ana layoutun en altında
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &VariableLoopDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &VariableLoopDialog::reject);

}

QStringList VariableLoopDialog::variableLabels() const {
    QStringList list;
    for (const auto &v : Variable::onlineVariableList)
        list <<v.name+"|"+v.label;
    return list;
}

void VariableLoopDialog::addLoopRowparametre(int loopType, const QString &expression)
{
    LoopRow *row = new LoopRow;
    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);

    row->loopTypeCombo = new QComboBox;
    row->loopTypeCombo->addItems({"📥 for", "📥 while"});
    row->loopTypeCombo->setCurrentIndex(loopType);
    layout->addWidget(row->loopTypeCombo);

    row->variableNameEdit = new QLineEdit; row->variableNameEdit->setPlaceholderText("name");
    layout->addWidget(row->variableNameEdit);

    row->variableLabelCombo = new QComboBox;
    //row->variableLabelCombo->addItems(variableLabels());
    for (QString satir : variableLabels()) {
        row->variableLabelCombo->addItem(satir.split("|")[1]);
    }
    layout->addWidget(row->variableLabelCombo);
    if(row->variableLabelCombo->currentIndex()>-1)
    {
    QString satir=variableLabels().at(row->variableLabelCombo->currentIndex());
    row->variableNameEdit->setText(satir.split("|")[0]);
    }

    row->startEdit = new QLineEdit; row->startEdit->setPlaceholderText("start");
    layout->addWidget(row->startEdit);

    row->endEdit = new QLineEdit; row->endEdit->setPlaceholderText("end");
    layout->addWidget(row->endEdit);

    row->stepEdit = new QLineEdit; row->stepEdit->setPlaceholderText("step");
    layout->addWidget(row->stepEdit);

    row->conditionVarCombo = new QComboBox;
   // row->conditionVarCombo->addItems(variableLabels());
    layout->addWidget(row->conditionVarCombo);
    //row->variableLabelCombo->addItems(variableLabels());
    for (QString satir : variableLabels()) {
        row->conditionVarCombo->addItem(satir.split("|")[1]);
    }
    if(row->conditionVarCombo->currentIndex()>-1)
    {
        QString satir=variableLabels().at(row->conditionVarCombo->currentIndex());
        row->variableNameEdit->setText(satir.split("|")[0]);
    }


    row->conditionOpCombo = new QComboBox;
    row->conditionOpCombo->addItems({"<", "<=", ">", ">=", "==", "!="});
    layout->addWidget(row->conditionOpCombo);

    row->conditionValEdit = new QLineEdit;
    row->conditionValEdit->setPlaceholderText("Sayı");
    layout->addWidget(row->conditionValEdit);

    row->removeButton = new QPushButton("🗑️");
    layout->addWidget(row->removeButton);

    loopRowsLayout->addWidget(row->widget);
    loopRows.append(row);

    // parse expression
    QStringList parts = expression.split(' ');
    if (loopType == 0 && parts.size() == 4) { // for i=0 to 10 step 2
        int varIdx = row->variableLabelCombo->findText(parts[0]);
        if (varIdx >= 0) {
            row->variableLabelCombo->setCurrentIndex(varIdx);
            updateVariableName(varIdx,loopRows.indexOf(row));
        }

        row->startEdit->setText(parts[1]);
        row->endEdit->setText(parts[2]);
        row->stepEdit->setText(parts[3]);
    }
    else if ((loopType == 1 || loopType == 2) && parts.size() == 3) { // while i < 10
        int varIdx = row->conditionVarCombo->findText(parts[0]);
        if (varIdx >= 0) row->conditionVarCombo->setCurrentIndex(varIdx);
        int opIdx = row->conditionOpCombo->findText(parts[1]);
        if (opIdx >= 0) row->conditionOpCombo->setCurrentIndex(opIdx);
        row->conditionValEdit->setText(parts[2]);
    }

    connect(row->loopTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
        int idx = loopRows.indexOf(row);
        if (idx >= 0)
            updateLoopRowWidgets(idx);
    });

    connect(row->variableLabelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
        int idx =row->variableLabelCombo->currentIndex();
        int lidx = loopRows.indexOf(row);
        if (idx >= 0)
            updateVariableName(idx,lidx);
    });
    connect(row->conditionVarCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
        int idx =row->conditionVarCombo->currentIndex();
        int lidx = loopRows.indexOf(row);
        if (idx >= 0)
            updateVariableName(idx,lidx);
    });
    connect(row->removeButton, &QPushButton::clicked, this, [=]() {
        loopRowsLayout->removeWidget(row->widget);
        row->widget->deleteLater();
        loopRows.removeOne(row);
    });

    updateLoopRowWidgets(loopRows.indexOf(row));
}
void VariableLoopDialog::updateVariableName(int index,int loopRowIndex)
{
    if (index < 0 || index >= variableLabels().size())
        return;
    QString row = variableLabels().at(index);
    qDebug()<<"seçili değiken "<<index<<row.split("|")[0]<<row.split("|")[1];
    LoopRow *lrow = loopRows.at(loopRowIndex);
    lrow->variableNameEdit->setText(row.split("|")[0]);
}

void VariableLoopDialog::updateLoopRowWidgets(int index)
{
    if (index < 0 || index >= loopRows.size())
        return;

    LoopRow *row = loopRows.at(index);
    int type = row->loopTypeCombo->currentIndex();
    row->variableNameEdit->setVisible(false);
    row->variableLabelCombo->setVisible(false);
    row->startEdit->setVisible(false);
    row->endEdit->setVisible(false);
    row->stepEdit->setVisible(false);
    row->conditionVarCombo->setVisible(false);
    row->conditionOpCombo->setVisible(false);
    row->conditionValEdit->setVisible(false);
    if(type == 0)
    {
   // row->variableNameEdit->setVisible(true);
    row->variableLabelCombo->setVisible(true);
    row->startEdit->setVisible(true);
    row->endEdit->setVisible(true);
    row->stepEdit->setVisible(true);
    updateVariableName(row->variableLabelCombo->currentIndex(),index);
    }
    if(type == 1)
    {
       // row->variableNameEdit->setVisible(true);
        row->conditionVarCombo->setVisible(true);
        row->conditionOpCombo->setVisible(true);
        row->conditionValEdit->setVisible(true);
        updateVariableName(row->conditionVarCombo->currentIndex(),index);
    }


}

QList<LoopRecord> VariableLoopDialog::getLoopsWithType() const
{
    QList<LoopRecord> list;
    for (LoopRow *row : loopRows) {
        LoopRecord rec;
        rec.loopType = row->loopTypeCombo->currentIndex();

        if (rec.loopType == 0) {
            // for döngüsü
            rec.expression = QString("%1 %2 %3 %4")
                                 .arg(row->variableLabelCombo->currentText())
                                 .arg(row->startEdit->text())
                                 .arg(row->endEdit->text())
                                 .arg(row->stepEdit->text());
            bool ok;
            rec.startValue = row->startEdit->text().toInt(&ok,10);
            rec.endValue = row->endEdit->text().toInt(&ok,10);
            rec.stepValue = row->stepEdit->text().toInt(&ok,10);
            rec.label = row->variableLabelCombo->currentText();
            rec.name = row->variableNameEdit->text();
        } else if (rec.loopType == 1){
            // while
            rec.expression = QString("%1 %2 %3")
                                 .arg(row->conditionVarCombo->currentText())
                                 .arg(row->conditionOpCombo->currentText())
                                 .arg(row->conditionValEdit->text());
            bool ok;
            rec.startValue = row->startEdit->text().toInt(&ok,10);
            rec.endValue = row->conditionValEdit->text().toInt(&ok,10);
            rec.stepValue = row->stepEdit->text().toInt(&ok,10);
            rec.label = row->variableLabelCombo->currentText();
            rec.name = row->variableNameEdit->text();
        }else if (rec.loopType == 2){
           // do while
            rec.expression = QString("%1 %2 %3")
                                 .arg(row->conditionVarCombo->currentText())
                                 .arg(row->conditionOpCombo->currentText())
                                 .arg(row->conditionValEdit->text());
           bool ok;
           rec.startValue = row->startEdit->text().toInt(&ok,10);
           rec.endValue = row->conditionValEdit->text().toInt(&ok,10);
           rec.stepValue = row->stepEdit->text().toInt(&ok,10);
           rec.label = row->variableLabelCombo->currentText();
           rec.name = row->variableNameEdit->text();
        }

        list.append(rec);
    }
    return list;
}
