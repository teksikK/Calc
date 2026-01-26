#include "calculator.h"
#include <stdexcept>

// constructor
Calculator::Calculator()
    : raw(0), base(NumberBase::DEC), wordSize(WordSize::QWORD) {}


// helpers
uint64_t Calculator::mask() const {
    int bits = (int)wordSize;

    if (bits == 8)  return 0xFF;
    if (bits == 16) return 0xFFFF;
    if (bits == 32) return 0xFFFFFFFF;
    if (bits == 64) return 0xFFFFFFFFFFFFFFFF;

    return 0;
}

int64_t Calculator::signedValue() const {
    int bits = static_cast<int>(wordSize);
    uint64_t m = mask();
    uint64_t v = raw & m;

    uint64_t signBit = 1ULL << (bits - 1);
    if (v & signBit) {
        return static_cast<int64_t>(v | ~m);
    }
    return static_cast<int64_t>(v);
}
void Calculator::setRaw(uint64_t v) {
    raw = v & mask();
}

void Calculator::setValue(int64_t v) {
    raw = static_cast<uint64_t>(v) & mask();
}

uint64_t Calculator::getRaw() const {
    return raw & mask();
}


// operations
int64_t Calculator::add(int64_t a, int64_t b) {
    raw = (static_cast<uint64_t>(a) + static_cast<uint64_t>(b)) & mask();
    return signedValue();
}

int64_t Calculator::subtract(int64_t a, int64_t b) {
    raw = (static_cast<uint64_t>(a) - static_cast<uint64_t>(b)) & mask();
    return signedValue();
}

int64_t Calculator::multiply(int64_t a, int64_t b) {
    raw = (static_cast<uint64_t>(a) * static_cast<uint64_t>(b)) & mask();
    return signedValue();
}

int64_t Calculator::divide(int64_t a, int64_t b) {
    if (b == 0)
        throw std::invalid_argument("Division by zero");

    raw = static_cast<uint64_t>(a / b) & mask();
    return signedValue();
}

void Calculator::setBase(NumberBase b) {
    base = b;
}

void Calculator::setWordSize(WordSize w) {
    wordSize = w;
    raw &= mask();
}

int64_t Calculator::getValue() const {
    return signedValue();
}


// display
std::string Calculator::display() const {
    switch (base) {
        case NumberBase::DEC: return toDec();
        case NumberBase::BIN: return toBin();
        case NumberBase::OCT: return toOct();
        case NumberBase::HEX: return toHex();
    }
    return "";
}

std::string Calculator::toDec() const {
    return std::to_string(signedValue());
}

std::string Calculator::toBin() const {
    uint64_t v = raw & mask();
    if (v == 0) return "0";

    std::string r;
    while (v) {
        r = char('0' + (v & 1)) + r;
        v >>= 1;
    }
    return r;
}

std::string Calculator::toOct() const {
    uint64_t v = raw & mask();
    if (v == 0) return "0";

    std::string r;
    while (v) {
        r = char('0' + (v & 7)) + r;
        v >>= 3;
    }
    return r;
}

std::string Calculator::toHex() const {
    uint64_t v = raw & mask();
    if (v == 0) return "0";

    std::string r;
    const char* d = "0123456789ABCDEF";
    while (v) {
        r = d[v & 0xF] + r;
        v >>= 4;
    }
    return r;
}
