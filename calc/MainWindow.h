#pragma once

#include <QWidget>

class QLineEdit;
class QLabel;
class QRadioButton;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

public slots:
    void onButtonClicked();

private:
    QLineEdit* display;
    QLabel* bitView;

    // base
    QRadioButton *rbHex, *rbDec, *rbOct, *rbBin;
    // word size
    QRadioButton *rbQword, *rbDword, *rbWord, *rbByte;
};
