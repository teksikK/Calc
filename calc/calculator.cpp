#include "calculator.h"
#include <stdexcept>
#include <cmath>

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

int64_t Calculator::bitAnd(int64_t a, int64_t b)
{
    raw = (a & b) & mask();
    return signedValue();
}

int64_t Calculator::shl(int64_t a, int n)
{
    int bits = (int)wordSize;
    n %= bits;

    uint64_t v = (uint64_t)a & mask();
    raw = (v << n) & mask();

    return signedValue();
}

int64_t Calculator::bitNot(int64_t a)
{
    raw = (~a) & mask();
    return signedValue();
}

// ================= BIT OPERATIONS =================

int64_t Calculator::bitOr(int64_t a, int64_t b)
{
    raw = (a | b) & mask();
    return signedValue();
}

int64_t Calculator::bitXor(int64_t a, int64_t b)
{
    raw = (a ^ b) & mask();
    return signedValue();
}

// ================= SHIFT =================

int64_t Calculator::shr(int64_t a, int n)
{
    int bits = (int)wordSize;
    n %= bits;

    uint64_t v = (uint64_t)a & mask();
    raw = (v >> n) & mask();

    return signedValue();
}

// ================= ROTATE =================

int64_t Calculator::rol(int64_t a, int n)
{
    int bits = (int)wordSize;
    n %= bits;

    uint64_t v = (uint64_t)a & mask();

    if (bits == 64)
        raw = (v << n) | (v >> (64 - n));
    else
        raw = ((v << n) | (v >> (bits - n))) & mask();

    return signedValue();
}

int64_t Calculator::ror(int64_t a, int n)
{
    int bits = (int)wordSize;
    n %= bits;

    uint64_t v = (uint64_t)a & mask();

    if (bits == 64)
        raw = (v >> n) | (v << (64 - n));
    else
        raw = ((v >> n) | (v << (bits - n))) & mask();

    return signedValue();
}

// ================= MATH =================

int64_t Calculator::mod(int64_t a, int64_t b)
{
    if (b == 0)
        throw std::invalid_argument("DIV/0");

    raw = (a % b) & mask();
    return signedValue();
}

int64_t Calculator::isqrt(int64_t a)
{
    if (a < 0)
        throw std::invalid_argument("N/A");

    int64_t r = static_cast<int64_t>(std::sqrt((long double)a));

    raw = r & mask();
    return signedValue();
}

int64_t Calculator::reciprocal(int64_t a)
{
    if (a == 0)
        throw std::invalid_argument("DIV/0");

    if (1 % a != 0)
        throw std::invalid_argument("N/A");

    raw = (1 / a) & mask();
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
