#include "calculator.h"
#include <stdexcept>

// ===== konstruktor =====
Calculator::Calculator()
    : value(0), base(NumberBase::DEC) {}

// ===== stare operacje (teraz logiczne dla programisty) =====
uint64_t Calculator::add(uint64_t a, uint64_t b) {
    value = a + b;
    return value;
}

uint64_t Calculator::subtract(uint64_t a, uint64_t b) {
    value = a - b;
    return value;
}

uint64_t Calculator::multiply(uint64_t a, uint64_t b) {
    value = a * b;
    return value;
}

uint64_t Calculator::divide(uint64_t a, uint64_t b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    value = a / b;
    return value;
}

// ===== ustawianie i pobieranie =====
void Calculator::setValue(uint64_t v) {
    value = v;
}

void Calculator::setBase(NumberBase b) {
    base = b;
}

uint64_t Calculator::getValue() const {
    return value;
}

// ===== wyświetlanie zależne od systemu =====
std::string Calculator::display() const {
    switch (base) {
        case NumberBase::DEC: return toDec();
        case NumberBase::BIN: return toBin();
        case NumberBase::OCT: return toOct();
        case NumberBase::HEX: return toHex();
    }
    return "";
}

// ===== konwersje =====
std::string Calculator::toDec() const {
    return std::to_string(value);
}

std::string Calculator::toBin() const {
    if (value == 0) return "0";

    uint64_t temp = value;
    std::string result;

    while (temp > 0) {
        result = char('0' + (temp % 2)) + result;
        temp /= 2;
    }
    return result;
}

std::string Calculator::toOct() const {
    if (value == 0) return "0";

    uint64_t temp = value;
    std::string result;

    while (temp > 0) {
        result = char('0' + (temp % 8)) + result;
        temp /= 8;
    }
    return result;
}

std::string Calculator::toHex() const {
    if (value == 0) return "0";

    uint64_t temp = value;
    std::string result;
    const char* digits = "0123456789ABCDEF";

    while (temp > 0) {
        result = digits[temp % 16] + result;
        temp /= 16;
    }
    return result;
}
