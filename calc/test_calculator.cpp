#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "calculator.h"

#include <limits>
#include <stdexcept>


// ================= BASE / DISPLAY =================

TEST_CASE("Base formatting for positive numbers") {

    Calculator calc;
    calc.setWordSize(WordSize::QWORD);

    calc.setBase(NumberBase::DEC);
    calc.add(0, 26);
    REQUIRE(calc.display() == "26");

    calc.setBase(NumberBase::HEX);
    REQUIRE(calc.display() == "1A");

    calc.setBase(NumberBase::OCT);
    REQUIRE(calc.display() == "32");

    calc.setBase(NumberBase::BIN);
    REQUIRE(calc.display() == "11010");
}


TEST_CASE("Changing base does not change value") {

    Calculator calc;

    calc.add(0, 255);

    calc.setBase(NumberBase::HEX);
    REQUIRE(calc.display() == "FF");

    calc.setBase(NumberBase::DEC);
    REQUIRE(calc.display() == "255");

    calc.setBase(NumberBase::BIN);
    REQUIRE(calc.display() == "11111111");
}


TEST_CASE("Zero formatting") {

    Calculator calc;

    calc.add(0, 0);

    calc.setBase(NumberBase::HEX);
    REQUIRE(calc.display() == "0");

    calc.setBase(NumberBase::BIN);
    REQUIRE(calc.display() == "0");
}



// ================= WORD SIZE =================

TEST_CASE("Changing word size masks stored value") {

    Calculator calc;

    calc.setWordSize(WordSize::QWORD);
    calc.add(0, 0x1FF);   // 511

    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.getValue() == -1);

    calc.setBase(NumberBase::HEX);
    REQUIRE(calc.display() == "FF");
}

TEST_CASE("Changing word size masks QWORD to WORD") {

    Calculator calc;

    calc.setWordSize(WordSize::QWORD);
    calc.add(0, 0x1FFFF);

    calc.setWordSize(WordSize::WORD);

    REQUIRE(calc.getValue() == -1);
}

TEST_CASE("Changing word size masks QWORD to DWORD") {

    Calculator calc;

    calc.setWordSize(WordSize::QWORD);
    calc.add(0, 0x1FFFFFFFF);

    calc.setWordSize(WordSize::DWORD);

    REQUIRE(calc.getValue() == -1);
}

TEST_CASE("BYTE wrap-around behavior") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.add(127, 1) == -128);
    REQUIRE(calc.subtract(-128, 1) == 127);
}

TEST_CASE("Increasing word size preserves value") {

    Calculator calc;

    calc.setWordSize(WordSize::BYTE);
    calc.add(0, -1); // 0xFF

    calc.setWordSize(WordSize::QWORD);

    REQUIRE(calc.getValue() == -1);
}

TEST_CASE("Minimum negative values") {

    Calculator calc;
    calc.setWordSize(WordSize::QWORD);

    REQUIRE(calc.add(INT64_MIN, 0) == INT64_MIN);
}



// ================= ARITHMETIC =================

// ADD
TEST_CASE("Decimal addition") {

    Calculator calc;

    REQUIRE(calc.add(2, 3) == 5);
    REQUIRE(calc.add(-1, 1) == 0);
    REQUIRE(calc.add(0, 0) == 0);
    REQUIRE(calc.add(0, 5) == 5);
    REQUIRE(calc.add(-5, 0) == -5);

    REQUIRE(calc.add(
        std::numeric_limits<int64_t>::max(), 0)
        == std::numeric_limits<int64_t>::max());
}


// SUB
TEST_CASE("Decimal subtraction") {

    Calculator calc;

    REQUIRE(calc.subtract(5, 3) == 2);
    REQUIRE(calc.subtract(3, 5) == -2);
    REQUIRE(calc.subtract(0, 0) == 0);
    REQUIRE(calc.subtract(5, 0) == 5);
    REQUIRE(calc.subtract(0, 5) == -5);
    REQUIRE(calc.subtract(-5, -5) == 0);
}


// MUL
TEST_CASE("Decimal multiplication") {

    Calculator calc;

    REQUIRE(calc.multiply(4, 3) == 12);
    REQUIRE(calc.multiply(0, 5) == 0);
    REQUIRE(calc.multiply(0, -100) == 0);
    REQUIRE(calc.multiply(-1, 5) == -5);

    REQUIRE(calc.multiply(
        1, std::numeric_limits<int64_t>::max())
        == std::numeric_limits<int64_t>::max());
}

TEST_CASE("Multiplication overflow wrap-around") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.multiply(64, 2) == -128);
}
// DIV
TEST_CASE("Decimal division") {

    Calculator calc;

    REQUIRE(calc.divide(10, 2) == 5);
    REQUIRE(calc.divide(0, 5) == 0);
    REQUIRE(calc.divide(-10, 2) == -5);
    REQUIRE(calc.divide(10, -2) == -5);

    REQUIRE_THROWS_AS(calc.divide(5, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.divide(-5, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.divide(0, 0), std::invalid_argument);
}



// ================= OVERFLOW =================

TEST_CASE("Addition overflow wrap-around") {

    Calculator calc;
    calc.setWordSize(WordSize::QWORD);

    REQUIRE(calc.add(INT64_MAX, 1) == INT64_MIN);
}


TEST_CASE("Subtraction underflow wrap-around") {

    Calculator calc;
    calc.setWordSize(WordSize::QWORD);

    REQUIRE(calc.subtract(INT64_MIN, 1) == INT64_MAX);
}


TEST_CASE("Multiple overflow operations") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    calc.add(127, 1);     // -> -128
    calc.add(-128, 1);    // -> -127

    REQUIRE(calc.getValue() == -127);
}



// ================= BIT OPERATIONS =================

TEST_CASE("Bit AND") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitAnd(0x0C, 0x0A) == 0x08); // 1100 & 1010

    REQUIRE(calc.bitAnd(0xFF, 0x55) == 0x55);
    REQUIRE(calc.bitAnd(0xFF, 0x00) == 0x00);

    REQUIRE(calc.bitAnd(0x00, 0xAA) == 0x00);

    REQUIRE(calc.bitAnd(-1, 0x0F) == 0x0F);
    REQUIRE(calc.bitAnd(-128, 0xFF) == -128);
}



TEST_CASE("Bit OR") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitOr(0x0C, 0x0A) == 0x0E);
    REQUIRE(calc.bitOr(0x00, 0x55) == 0x55);
    REQUIRE(calc.bitOr(0xAA, 0x55) == -1);
    REQUIRE(calc.bitOr(-128, 0x01) == -127);
}


TEST_CASE("Bit XOR") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitXor(0x0C, 0x0A) == 0x06);
    REQUIRE(calc.bitXor(0x55, 0x55) == 0x00);
    REQUIRE(calc.bitXor(0x00, 0xAA) == -86);
    REQUIRE(calc.bitXor(0xFF, 0x0F) == -16);
    REQUIRE(calc.bitXor(-1, 0x0F) == -16);


}



TEST_CASE("Bit NOT respects word size") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitNot(0xF0) == 0x0F);
    REQUIRE(calc.bitNot(0x00) == -1);
    REQUIRE(calc.bitNot(0xFF) == 0x00);
    REQUIRE(calc.bitNot(0x55) == -86); // 10101010
    REQUIRE(calc.bitNot(0x80) == 0x7F);

}




// ================= SHIFT =================

TEST_CASE("Shift left") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.shl(0x01, 1) == 0x02);
    REQUIRE(calc.shl(0x02, 1) == 0x04);
    REQUIRE(calc.shl(0x04, 1) == 0x08);

    REQUIRE(calc.shl(0x80, 1) == 0x00);
    REQUIRE(calc.shl(0xC0, 1) == -128);

    REQUIRE(calc.shl(0x01, 2) == 0x04);
    REQUIRE(calc.shl(0x01, 7) == -128);
    REQUIRE(calc.shl(0x01, 8) == 0x00);

    REQUIRE(calc.shl(0x00, 3) == 0x00);
    REQUIRE(calc.shl(0xFF, 1) == -2);
}



TEST_CASE("Shift right") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.shr(0x02, 1) == 0x01);
    REQUIRE(calc.shr(0x04, 1) == 0x02);
    REQUIRE(calc.shr(0x08, 1) == 0x04);

    REQUIRE(calc.shr(0x01, 1) == 0x00);
    REQUIRE(calc.shr(0x03, 1) == 0x01);

    REQUIRE(calc.shr(0x80, 1) == 0x40);
    REQUIRE(calc.shr(0x80, 7) == 0x01);
    REQUIRE(calc.shr(0x80, 8) == 0x00);

    REQUIRE(calc.shr(0x00, 5) == 0x00);
    REQUIRE(calc.shr(0xFF, 1) == 0x7F);
}




// ================= ROTATE =================

TEST_CASE("Rotate left BYTE") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.rol(0x01, 1) == 0x02);
    REQUIRE(calc.rol(0x02, 1) == 0x04);
    REQUIRE(calc.rol(0x04, 1) == 0x08);

    REQUIRE(calc.rol(0x80, 1) == 0x01);
    REQUIRE(calc.rol(0xC0, 1) == -127);


    REQUIRE(calc.rol(0x01, 2) == 0x04);
    REQUIRE(calc.rol(0x01, 3) == 0x08);
    REQUIRE(calc.rol(0x01, 7) == -128);


    REQUIRE(calc.rol(0x55, 8) == 0x55);
    REQUIRE(calc.rol(0xAA, 16) == -86);


    REQUIRE(calc.rol(0x00, 1) == 0x00);
    REQUIRE(calc.rol(0x00, 5) == 0x00);

    REQUIRE(calc.rol(0xFF, 1) == -1);
}



TEST_CASE("Rotate right BYTE") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.ror(0x02, 1) == 0x01);
    REQUIRE(calc.ror(0x04, 1) == 0x02);
    REQUIRE(calc.ror(0x08, 1) == 0x04);

    REQUIRE(calc.ror(0x01, 1) == -128);
    REQUIRE(calc.ror(0x03, 1) == -127);

    REQUIRE(calc.ror(0x08, 3) == 0x01);
    REQUIRE(calc.ror(0x80, 7) == 0x01);

    REQUIRE(calc.ror(0x5A, 8) == 0x5A);
    REQUIRE(calc.ror(0xF0, 16) == -16);


    REQUIRE(calc.ror(0x00, 1) == 0x00);
    REQUIRE(calc.ror(0x00, 6) == 0x00);

    REQUIRE(calc.ror(0xFF, 1) == -1);
}



// ================= MOD =================

TEST_CASE("Modulo") {

    Calculator calc;


    REQUIRE(calc.mod(10, 3) == 1);
    REQUIRE(calc.mod(9, 3) == 0);
    REQUIRE(calc.mod(5, 2) == 1);

    REQUIRE(calc.mod(0, 1) == 0);
    REQUIRE(calc.mod(0, 5) == 0);
    REQUIRE(calc.mod(1, 1) == 0);

    REQUIRE(calc.mod(-10, 3) == -1);
    REQUIRE(calc.mod(10, -3) == 1);
    REQUIRE(calc.mod(-10, -3) == -1);

    REQUIRE(calc.mod(-1, 2) == -1);
    REQUIRE(calc.mod(-5, 2) == -1);

    REQUIRE(calc.mod(INT64_MAX, 2) == 1);
    REQUIRE(calc.mod(INT64_MIN, 2) == 0);

    REQUIRE(calc.mod(INT64_MAX, 1) == 0);
    REQUIRE(calc.mod(INT64_MIN, 1) == 0);

    REQUIRE(calc.mod(5, 100) == 5);
    REQUIRE(calc.mod(-5, 100) == -5);

    REQUIRE_THROWS_AS(calc.mod(5, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.mod(0, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.mod(-5, 0), std::invalid_argument);
}




// ================= SQRT =================

TEST_CASE("Integer sqrt") {

    Calculator calc;

    REQUIRE(calc.isqrt(16) == 4);
    REQUIRE(calc.isqrt(15) == 3);
    REQUIRE(calc.isqrt(1) == 1);
    REQUIRE(calc.isqrt(0) == 0);
    REQUIRE(calc.isqrt(8) == 2);
    REQUIRE(calc.isqrt(10) == 3);
    REQUIRE(calc.isqrt(26) == 5);
    REQUIRE(calc.isqrt(99) == 9);
    int64_t big = 9223372036854775807LL;
    REQUIRE(calc.isqrt(big) == 3037000499);
    REQUIRE_THROWS_AS(calc.isqrt(-1), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.isqrt(-100), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.isqrt(INT64_MIN), std::invalid_argument);
}



// ================= RECIPROCAL =================

TEST_CASE("Reciprocal") {

    Calculator calc;

    REQUIRE(calc.reciprocal(1) == 1);
    REQUIRE(calc.reciprocal(-1) == -1);

    REQUIRE_THROWS_AS(calc.reciprocal(0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.reciprocal(2), std::invalid_argument);
}



// ================= NEGATIVE DISPLAY =================

TEST_CASE("Hex representation of negative numbers") {

    Calculator calc;

    calc.setWordSize(WordSize::QWORD);
    calc.setBase(NumberBase::HEX);

    calc.add(-1, 0);

    REQUIRE(calc.display() == "FFFFFFFFFFFFFFFF");
}


TEST_CASE("Binary representation BYTE") {

    Calculator calc;

    calc.setWordSize(WordSize::BYTE);
    calc.setBase(NumberBase::BIN);

    calc.add(-1, 0);

    REQUIRE(calc.display() == "11111111");
}
