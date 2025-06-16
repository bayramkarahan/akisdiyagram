#include "variableconditiondialog.h"
#include "variable.h"  // Variable::onlineVariableList için
#include<QStackedLayout>
ConditionRow::ConditionRow(const QStringList &variables, QWidget *parent)
    : QWidget(parent), variableList(variables) {
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({
        "var < var", "var > constant", "var < var && var > constant"
    });

    leftCombo = new QComboBox(this);
    leftCombo->addItems(variables);

    opCombo = new QComboBox(this);
    opCombo->addItems({ "<", ">", "<=", ">=", "==", "!=" });

    rightInputWidget = new QWidget(this);
    auto rightLayout = new QStackedLayout(rightInputWidget);
    rightCombo = new QComboBox(this);
    rightCombo->addItems(variables);
    rightLineEdit = new QLineEdit(this);
    rightLineEdit->setPlaceholderText("Enter value");

    rightLayout->addWidget(rightCombo);
    rightLayout->addWidget(rightLineEdit);

    layout->addWidget(typeCombo);
    layout->addWidget(leftCombo);
    layout->addWidget(opCombo);
    layout->addWidget(rightInputWidget);

    connect(typeCombo, &QComboBox::currentTextChanged,
            this, &ConditionRow::updateRightInputField);

    updateRightInputField(typeCombo->currentText());
}

void ConditionRow::updateRightInputField(const QString &type) {
    auto *stack = qobject_cast<QStackedLayout *>(rightInputWidget->layout());
    if (!stack) return;

    if (type.contains("constant"))
        stack->setCurrentWidget(rightLineEdit);
    else
        stack->setCurrentWidget(rightCombo);
}

QString ConditionRow::getExpression() const {
    QString left = leftCombo->currentText();
    QString op = opCombo->currentText();
    QString right;

    auto *stack = qobject_cast<QStackedLayout *>(rightInputWidget->layout());
    if (stack->currentWidget() == rightCombo)
        right = rightCombo->currentText();
    else
        right = rightLineEdit->text();

    return QString("%1 %2 %3").arg(left, op, right);
}

int ConditionRow::getOperationType() const {
    return typeCombo->currentIndex();
}

VariableConditionDialog::VariableConditionDialog(QWidget *parent)
    : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    rowsLayout = new QVBoxLayout();
    QFrame *frame = new QFrame;
    frame->setLayout(rowsLayout);
    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    frame->setLineWidth(1);
    mainLayout->addWidget(frame);

    addRowButton = new QPushButton("Ekle");
    removeRowButton = new QPushButton("Sil");
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(addRowButton);
    btnLayout->addWidget(removeRowButton);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(buttonBox);

    connect(addRowButton, &QPushButton::clicked, this, &VariableConditionDialog::addRow);
    connect(removeRowButton, &QPushButton::clicked, this, &VariableConditionDialog::removeLastRow);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &VariableConditionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &VariableConditionDialog::reject);

    addRow();
}

void VariableConditionDialog::addExpressionRow(int operationType, const QString &expression) {
    Q_UNUSED(expression);  // İsteğe göre parse edilebilir.
    auto row = new ConditionRow(getVariableLabels());
    rowsLayout->addWidget(row);
}

void VariableConditionDialog::addRow() {
    auto row = new ConditionRow(getVariableLabels());
    rowsLayout->addWidget(row);
}

void VariableConditionDialog::removeLastRow() {
    if (rowsLayout->count() > 0) {
        QLayoutItem *item = rowsLayout->takeAt(rowsLayout->count() - 1);
        delete item->widget();
        delete item;
    }
}

QList<QPair<int, QString>> VariableConditionDialog::getExpressionsWithType() const {
    QList<QPair<int, QString>> list;
    for (int i = 0; i < rowsLayout->count(); ++i) {
        auto *row = qobject_cast<ConditionRow *>(rowsLayout->itemAt(i)->widget());
        if (row) {
            list.append({ row->getOperationType(), row->getExpression() });
        }
    }
    return list;
}
QStringList VariableConditionDialog::getVariableLabels() const {
    QStringList list;
    for (const auto &v : Variable::onlineVariableList)
        list << v.label;
    return list;
}

