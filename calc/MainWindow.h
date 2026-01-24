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
    // ===== UI =====
    QLineEdit* display;
    QLabel* bitView;

    QRadioButton *rbHex, *rbDec, *rbOct, *rbBin;
    QRadioButton *rbQword, *rbDword, *rbWord, *rbByte;

    // ===== Calculator =====
    Calculator calc;

    enum class Op {
        None,
        Add, Sub, Mul, Div, Mod,
        And, Or, Xor,
        Lsh, Rsh
    };

    Op pendingOp = Op::None;
    bool waitingForValue = true;

    // ===== digit buttons =====
    QMap<QString, QPushButton*> digitButtons;

    // ===== helpers =====
    int64_t parseDisplay() const;
    void applyOperation(int64_t value);
    void updateBitView();
    void updateDigitButtons();
    void registerDigit(QPushButton* b);
};
