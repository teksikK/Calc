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


TEST_CASE("BYTE wrap-around behavior") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.add(127, 1) == -128);
    REQUIRE(calc.subtract(-128, 1) == 127);
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

    REQUIRE(calc.bitAnd(0b1100, 0b1010) == 0b1000);
}


TEST_CASE("Bit OR") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitOr(0b1100, 0b1010) == 0b1110);
}


TEST_CASE("Bit XOR") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitXor(0b1100, 0b1010) == 0b0110);
}


TEST_CASE("Bit NOT respects word size") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.bitNot(0b11110000) == 0b00001111);
}



// ================= SHIFT =================

TEST_CASE("Shift left") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.shl(0b00000001, 1) == 0b00000010);
    REQUIRE(calc.shl(0b10000000, 1) == 0);
}


TEST_CASE("Shift right") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.shr(0b10000000, 1) == 0b01000000);
    REQUIRE(calc.shr(0b00000001, 1) == 0);
}



// ================= ROTATE =================

TEST_CASE("Rotate left BYTE") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.rol(0x01, 1) == 0x02);
    REQUIRE(calc.rol(0x80, 1) == 0x01);
}


TEST_CASE("Rotate right BYTE") {

    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.ror(0x01, 1) == -128);
    REQUIRE(calc.ror(0x02, 1) == 0x01);
}


// ================= MOD =================

TEST_CASE("Modulo") {

    Calculator calc;

    REQUIRE(calc.mod(10, 3) == 1);
    REQUIRE(calc.mod(-10, 3) == -1);

    REQUIRE_THROWS_AS(calc.mod(5, 0), std::invalid_argument);
}



// ================= SQRT =================

TEST_CASE("Integer sqrt") {

    Calculator calc;

    REQUIRE(calc.isqrt(16) == 4);
    REQUIRE(calc.isqrt(15) == 3);
    REQUIRE(calc.isqrt(1) == 1);
    REQUIRE(calc.isqrt(0) == 0);

    REQUIRE_THROWS_AS(calc.isqrt(-1), std::invalid_argument);
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
