#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>
#include <string>

enum class NumberBase {
    DEC,
    BIN,
    OCT,
    HEX
};

class Calculator {
private:
    uint64_t value;        // aktualna wartość kalkulatora
    NumberBase base;       // aktualny system liczbowy

public:
    Calculator();

    // ===== stare operacje (zostają, ale na uint64_t) =====
    uint64_t add(uint64_t a, uint64_t b);
    uint64_t subtract(uint64_t a, uint64_t b);
    uint64_t multiply(uint64_t a, uint64_t b);
    uint64_t divide(uint64_t a, uint64_t b);

    // ===== obsługa wartości i systemu =====
    void setValue(uint64_t v);
    void setBase(NumberBase b);
    uint64_t getValue() const;

    // ===== wyświetlanie =====
    std::string display() const;

private:
    std::string toDec() const;
    std::string toBin() const;
    std::string toOct() const;
    std::string toHex() const;
};

#endif // CALCULATOR_H
