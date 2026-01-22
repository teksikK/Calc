#include "MainWindow.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

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

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Programmer Calculator");

    // ===== DISPLAY =====
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(38);

    bitView = new QLabel(
        "0000 0000 0000 0000 0000 0000 0000 0000\n"
        "63                              32\n"
        "0000 0000 0000 0000 0000 0000 0000 0000\n"
        "31                               0"
    );
    bitView->setAlignment(Qt::AlignCenter);
    bitView->setStyleSheet("font-family: monospace;");

    // ===== LEFT =====
    auto* baseBox = new QGroupBox();
    auto* baseL = new QVBoxLayout(baseBox);
    rbHex = new QRadioButton("Hex");
    rbDec = new QRadioButton("Dec");
    rbOct = new QRadioButton("Oct");
    rbBin = new QRadioButton("Bin");
    rbDec->setChecked(true);
    baseL->addWidget(rbHex);
    baseL->addWidget(rbDec);
    baseL->addWidget(rbOct);
    baseL->addWidget(rbBin);

    auto* wordBox = new QGroupBox();
    auto* wordL = new QVBoxLayout(wordBox);
    rbQword = new QRadioButton("Qword");
    rbDword = new QRadioButton("Dword");
    rbWord  = new QRadioButton("Word");
    rbByte  = new QRadioButton("Byte");
    rbQword->setChecked(true);
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
        grid->addWidget(makeBtn(t,this), r, c);
    };

    // kolumna 0 – pusta
    for (int r=0;r<6;r++) grid->addWidget(spacer(), r, 0);

    // ROW 0
    grid->addWidget(spacer(), 0, 1);
    B(0,2,"Mod"); B(0,3,"A");  B(0,4,"MC"); B(0,5,"MR");
    B(0,6,"MS"); B(0,7,"M+");
    B(0,8,"M-");  // ostatnia kolumna - M-

    // ROW 1
    B(1,1,"("); B(1,2,")"); B(1,3,"B"); B(1,4,"←");
    B(1,5,"CE"); B(1,6,"C"); B(1,7,"±");
    B(1,8,"√");   // ostatnia kolumna - pierwiastek

    // ROW 2
    B(2,1,"RoL"); B(2,2,"RoR"); B(2,3,"C");
    B(2,4,"7"); B(2,5,"8"); B(2,6,"9"); B(2,7,"/");
    B(2,8,"%");   // ostatnia kolumna - procent

    // ROW 3
    B(3,1,"Or"); B(3,2,"Xor"); B(3,3,"D");
    B(3,4,"4"); B(3,5,"5"); B(3,6,"6"); B(3,7,"*");
    B(3,8,"1/x"); // ostatnia kolumna - 1/x

    // ROW 4
    B(4,1,"Lsh"); B(4,2,"Rsh"); B(4,3,"E");
    B(4,4,"1"); B(4,5,"2"); B(4,6,"3"); B(4,7,"-");

    // ROW 5
    B(5,1,"Not"); B(5,2,"And"); B(5,3,"F");
    // "0" zajmuje DWA KOLUMNY (4 i 5)
    auto* zero = makeBtn("0", this);
    zero->setFixedSize(110, 36);
    grid->addWidget(zero, 5, 4, 1, 2);

    B(5,6,"."); B(5,7,"+");

    // "=" — ZAJMUJE DWA WIERSZE (4 i 5, kolumna 8)
    auto* eq = makeBtn("=", this);
    eq->setFixedSize(52, 78);
    grid->addWidget(eq, 4, 8, 2, 1);

    // ===== CENTER =====
    auto* center = new QHBoxLayout();
    center->addLayout(left);
    center->addLayout(grid);

    // ===== MAIN =====
    auto* main = new QVBoxLayout(this);
    main->addWidget(display);
    main->addWidget(bitView);
    main->addLayout(center);

    setLayout(main);
    setFixedSize(sizeHint());
}

void MainWindow::onButtonClicked() {
    if (auto* b = qobject_cast<QPushButton*>(sender()))
        display->setText(b->text());
}