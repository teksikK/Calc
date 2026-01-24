#include "MainWindow.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <bitset>

// ===== helpers =====
static QPushButton* makeBtn(const QString& t, MainWindow* r) {
    auto* b = new QPushButton(t);
    b->setFixedSize(52, 36);
    QObject::connect(b, &QPushButton::clicked, r, &MainWindow::onButtonClicked);
    return b;
}

static QWidget* spacer() {
    auto* w = new QWidget();
    w->setFixedSize(52, 36);
    return w;
}

// ============================================================

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Programmer Calculator");

    // ===== DISPLAY =====
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(38);

    bitView = new QLabel();
    bitView->setAlignment(Qt::AlignCenter);
    bitView->setStyleSheet("font-family: monospace;");

    // ===== BASE =====
    rbHex = new QRadioButton("Hex");
    rbDec = new QRadioButton("Dec");
    rbOct = new QRadioButton("Oct");
    rbBin = new QRadioButton("Bin");
    rbDec->setChecked(true);

    connect(rbHex, &QRadioButton::clicked, this, &MainWindow::onBaseChanged);
    connect(rbDec, &QRadioButton::clicked, this, &MainWindow::onBaseChanged);
    connect(rbOct, &QRadioButton::clicked, this, &MainWindow::onBaseChanged);
    connect(rbBin, &QRadioButton::clicked, this, &MainWindow::onBaseChanged);

    auto* baseBox = new QGroupBox();
    auto* baseL = new QVBoxLayout(baseBox);
    baseL->addWidget(rbHex);
    baseL->addWidget(rbDec);
    baseL->addWidget(rbOct);
    baseL->addWidget(rbBin);

    // ===== WORD SIZE =====
    rbQword = new QRadioButton("Qword");
    rbDword = new QRadioButton("Dword");
    rbWord  = new QRadioButton("Word");
    rbByte  = new QRadioButton("Byte");
    rbQword->setChecked(true);

    connect(rbQword, &QRadioButton::clicked, this, &MainWindow::onWordSizeChanged);
    connect(rbDword, &QRadioButton::clicked, this, &MainWindow::onWordSizeChanged);
    connect(rbWord,  &QRadioButton::clicked, this, &MainWindow::onWordSizeChanged);
    connect(rbByte,  &QRadioButton::clicked, this, &MainWindow::onWordSizeChanged);

    auto* wordBox = new QGroupBox();
    auto* wordL = new QVBoxLayout(wordBox);
    wordL->addWidget(rbQword);
    wordL->addWidget(rbDword);
    wordL->addWidget(rbWord);
    wordL->addWidget(rbByte);

    auto* left = new QVBoxLayout();
    left->addWidget(baseBox);
    left->addWidget(wordBox);
    left->addStretch();

    // ===== GRID =====
    auto* grid = new QGridLayout();
    grid->setSpacing(6);

    auto B = [&](int r,int c,const char* t){
        auto* b = makeBtn(t,this);
        grid->addWidget(b, r, c);
        if (QString("0123456789ABCDEF").contains(t))
            registerDigit(b);
    };

    for (int r=0;r<6;r++) grid->addWidget(spacer(), r, 0);

    grid->addWidget(spacer(), 0, 1);
    B(0,2,"Mod"); B(0,3,"A"); B(0,4,"MC"); B(0,5,"MR");
    B(0,6,"MS"); B(0,7,"M+"); B(0,8,"M-");

    B(1,1,"("); B(1,2,")"); B(1,3,"B"); B(1,4,"←");
    B(1,5,"CE"); B(1,6,"C"); B(1,7,"±"); B(1,8,"√");

    B(2,1,"RoL"); B(2,2,"RoR"); B(2,3,"C");
    B(2,4,"7"); B(2,5,"8"); B(2,6,"9"); B(2,7,"/"); B(2,8,"%");

    B(3,1,"Or"); B(3,2,"Xor"); B(3,3,"D");
    B(3,4,"4"); B(3,5,"5"); B(3,6,"6"); B(3,7,"*"); B(3,8,"1/x");

    B(4,1,"Lsh"); B(4,2,"Rsh"); B(4,3,"E");
    B(4,4,"1"); B(4,5,"2"); B(4,6,"3"); B(4,7,"-");

    B(5,1,"Not"); B(5,2,"And"); B(5,3,"F");

    auto* zero = makeBtn("0", this);
    zero->setFixedSize(110, 36);
    registerDigit(zero);
    grid->addWidget(zero, 5, 4, 1, 2);

    B(5,6,"."); B(5,7,"+");

    auto* eq = makeBtn("=", this);
    eq->setFixedSize(52, 78);
    grid->addWidget(eq, 4, 8, 2, 1);

    auto* center = new QHBoxLayout();
    center->addLayout(left);
    center->addLayout(grid);

    auto* main = new QVBoxLayout(this);
    main->addWidget(display);
    main->addWidget(bitView);
    main->addLayout(center);

    setLayout(main);
    setFixedSize(sizeHint());

    onBaseChanged();
    updateBitView();
}

// ============================================================

void MainWindow::registerDigit(QPushButton* b)
{
    digitButtons.insert(b->text(), b);
}

int64_t MainWindow::parseDisplay() const
{
    int base =
        rbHex->isChecked() ? 16 :
        rbOct->isChecked() ? 8  :
        rbBin->isChecked() ? 2  : 10;

    return display->text().toLongLong(nullptr, base);
}

void MainWindow::applyOperation(int64_t value)
{
    int64_t cur = calc.getValue();

    switch (pendingOp) {
        case Op::Add: calc.add(cur, value); break;
        case Op::Sub: calc.subtract(cur, value); break;
        case Op::Mul: calc.multiply(cur, value); break;
        case Op::Div: calc.divide(cur, value); break;
        case Op::Mod: calc.subtract(cur, (cur / value) * value); break;
        case Op::And: calc.add(cur & value, 0); break;
        case Op::Or:  calc.add(cur | value, 0); break;
        case Op::Xor: calc.add(cur ^ value, 0); break;
        case Op::Lsh: calc.add(cur << value, 0); break;
        case Op::Rsh: calc.add(cur >> value, 0); break;
        case Op::None: calc.add(0, value); break;
    }

    display->setText(QString::fromStdString(calc.display()));
    updateBitView();
}

void MainWindow::updateBitView()
{
    int wordBits =
        rbByte->isChecked()  ? 8  :
        rbWord->isChecked()  ? 16 :
        rbDword->isChecked() ? 32 : 64;

    std::bitset<64> bits(static_cast<uint64_t>(calc.getValue()));
    QString s;

    for (int i = 63; i >= 0; --i) {
        if (i < wordBits)
            s += QChar(bits[i] ? '1' : '0');
        else
            s += QChar(0x00B7); // ·
        if (i % 4 == 0) s += ' ';
    }

    bitView->setText(s);
}

void MainWindow::updateDigitButtons()
{
    int max =
        rbBin->isChecked() ? 2 :
        rbOct->isChecked() ? 8 :
        rbDec->isChecked() ? 10 : 16;

    for (auto it = digitButtons.begin(); it != digitButtons.end(); ++it) {
        QString k = it.key();
        int v = k[0].isDigit() ? k.toInt() : (k[0].unicode() - 'A' + 10);
        it.value()->setEnabled(v < max);
    }
}

void MainWindow::onButtonClicked()
{
    auto* b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    QString t = b->text();

    if (digitButtons.contains(t)) {
        display->setText(waitingForValue ? t : display->text() + t);
        waitingForValue = false;
        return;
    }

    if (t == "CE") { display->setText("0"); waitingForValue = true; updateBitView(); return; }
    if (t == "C")  { calc = Calculator(); pendingOp = Op::None;
                     display->setText("0"); waitingForValue = true; updateBitView(); return; }

    if (t == "=") {
        if (!waitingForValue) applyOperation(parseDisplay());
        pendingOp = Op::None;
        waitingForValue = true;
        return;
    }

    auto op = [&](Op o){
        if (!waitingForValue) applyOperation(parseDisplay());
        pendingOp = o;
        waitingForValue = true;
    };

    if (t == "+") op(Op::Add);
    if (t == "-") op(Op::Sub);
    if (t == "*") op(Op::Mul);
    if (t == "/") op(Op::Div);
    if (t == "Mod") op(Op::Mod);
    if (t == "And") op(Op::And);
    if (t == "Or")  op(Op::Or);
    if (t == "Xor") op(Op::Xor);
    if (t == "Lsh") op(Op::Lsh);
    if (t == "Rsh") op(Op::Rsh);
}

void MainWindow::onBaseChanged()
{
    if (rbDec->isChecked()) calc.setBase(NumberBase::DEC);
    else if (rbHex->isChecked()) calc.setBase(NumberBase::HEX);
    else if (rbOct->isChecked()) calc.setBase(NumberBase::OCT);
    else if (rbBin->isChecked()) calc.setBase(NumberBase::BIN);

    display->setText(QString::fromStdString(calc.display()));
    updateDigitButtons();
    updateBitView();
}

void MainWindow::onWordSizeChanged()
{
    if (rbByte->isChecked())  calc.setWordSize(WordSize::BYTE);
    if (rbWord->isChecked())  calc.setWordSize(WordSize::WORD);
    if (rbDword->isChecked()) calc.setWordSize(WordSize::DWORD);
    if (rbQword->isChecked()) calc.setWordSize(WordSize::QWORD);

    display->setText(QString::fromStdString(calc.display()));
    updateBitView();
}
