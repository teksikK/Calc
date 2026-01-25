#pragma once
#include <cstdint>
#include <string>

enum class NumberBase {
    DEC,
    BIN,
    OCT,
    HEX
};

enum class WordSize {
    BYTE  = 8,
    WORD  = 16,
    DWORD = 32,
    QWORD = 64
};

class Calculator {
public:
    Calculator();

    int64_t add(int64_t a, int64_t b);
    int64_t subtract(int64_t a, int64_t b);
    int64_t multiply(int64_t a, int64_t b);
    int64_t divide(int64_t a, int64_t b);

    void setBase(NumberBase b);
    void setWordSize(WordSize w);

    int64_t getValue() const;
    std::string display() const;

    void setRaw(uint64_t v);
    void setValue(int64_t v);
    uint64_t getRaw() const;

private:
    uint64_t raw;
    NumberBase base;
    WordSize wordSize;

    uint64_t mask() const;
    int64_t  signedValue() const;

    std::string toDec() const;
    std::string toBin() const;
    std::string toOct() const;
    std::string toHex() const;
};
