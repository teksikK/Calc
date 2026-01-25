#pragma once

#include <QWidget>
#include <QMap>
#include <cstdint>
#include "calculator.h"

class QLineEdit;
class QLabel;
class QPushButton;
class QRadioButton;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

public slots:
    void onButtonClicked();
    void onBaseChanged();
    void onWordSizeChanged();

private:
    // ui
    QLineEdit* display;
    QLabel* bitView;

    QRadioButton *rbHex, *rbDec, *rbOct, *rbBin;
    QRadioButton *rbQword, *rbDword, *rbWord, *rbByte;

    // calculator
    Calculator calc;
    int64_t memory = 0;
    bool hasMemory = false;
    bool error = false;
    QString errorText = "Error";
    void setError(const QString& msg);
    bool isUnaryAllowed(const QString& t) const;
    enum class Op {
        None,
        Add, Sub, Mul, Div, Mod,
        And, Or, Xor,
        Lsh, Rsh,
        Pow
    };
    int wordBits() const;
    Op pendingOp = Op::None;
    bool waitingForValue = true;


    QMap<QString, QPushButton*> digitButtons;

    // helpers
    int64_t parseDisplay() const;
    void applyOperation(int64_t value);
    void updateBitView();
    void updateDigitButtons();
    void registerDigit(QPushButton* b);
};
