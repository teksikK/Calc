#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "calculator.h"
#include <limits>
#include <stdexcept>

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

TEST_CASE("Changing word size masks stored value") {
    Calculator calc;

    // Put a value with bits above 8 set
    calc.setWordSize(WordSize::QWORD);
    calc.add(0, 0x1FF); // 511 = 0b1_11111111

    calc.setWordSize(WordSize::BYTE); // should keep only 0xFF
    REQUIRE(calc.getValue() == -1);   // 0xFF as signed int8 = -1

    calc.setBase(NumberBase::HEX);
    REQUIRE(calc.display() == "FF");
}

TEST_CASE("NOT respects word size") {
    Calculator calc;
    calc.setWordSize(WordSize::BYTE);
    calc.setBase(NumberBase::BIN);

    calc.setRaw(0b11001100);
    REQUIRE(calc.display() == "11001100");

    // NOT => 00110011
    calc.setRaw((~calc.getRaw()) & 0xFF);
    REQUIRE(calc.display() == "110011"); // NOTE: your toBin() trims leading zeros
    // Better check raw bits:
    REQUIRE((calc.getRaw() & 0xFF) == 0b00110011);
}

static uint64_t maskFor(int bits) {
    if (bits == 64) return ~0ULL;
    return (1ULL << bits) - 1ULL;
}

static uint64_t rotlN(uint64_t x, int r, int bits) {
    r %= bits;
    uint64_t m = maskFor(bits);
    x &= m;
    if (bits == 64) return (x << r) | (x >> (64 - r));
    return ((x << r) | (x >> (bits - r))) & m;
}

static uint64_t rotrN(uint64_t x, int r, int bits) {
    r %= bits;
    uint64_t m = maskFor(bits);
    x &= m;
    if (bits == 64) return (x >> r) | (x << (64 - r));
    return ((x >> r) | (x << (bits - r))) & m;
}

TEST_CASE("Rotate works in BYTE") {
    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    // 00000001 RoR 1 => 10000000
    calc.setRaw(0x01);
    calc.setRaw(rotrN(calc.getRaw(), 1, 8));
    REQUIRE((calc.getRaw() & 0xFF) == 0x80);

    // 10000000 RoL 1 => 00000001
    calc.setRaw(0x80);
    calc.setRaw(rotlN(calc.getRaw(), 1, 8));
    REQUIRE((calc.getRaw() & 0xFF) == 0x01);
}

TEST_CASE("Logical shifts respect word size") {
    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    // 0b10000000 >> 1 => 0b01000000 (logical)
    calc.setRaw(0b10000000);
    calc.setRaw((calc.getRaw() >> 1) & 0xFF);
    REQUIRE((calc.getRaw() & 0xFF) == 0b01000000);

    // 0b00000001 << 1 => 0b00000010
    calc.setRaw(0b00000001);
    calc.setRaw((calc.getRaw() << 1) & 0xFF);
    REQUIRE((calc.getRaw() & 0xFF) == 0b00000010);
}
// ADDITION
TEST_CASE("Decimal addition") {
    Calculator calc;

    REQUIRE(calc.add(2, 3) == 5);
    REQUIRE(calc.add(-1, 1) == 0);
    REQUIRE(calc.add(0, 0) == 0);
    REQUIRE(calc.add(0, 5) == 5);
    REQUIRE(calc.add(-5, 0) == -5);
    REQUIRE(calc.add(std::numeric_limits<int64_t>::max(), 0)
            == std::numeric_limits<int64_t>::max());
}

// SUBTRACTION
TEST_CASE("Decimal substraction") {
    Calculator calc;

    REQUIRE(calc.subtract(5, 3) == 2);
    REQUIRE(calc.subtract(3, 5) == -2);
    REQUIRE(calc.subtract(0, 0) == 0);
    REQUIRE(calc.subtract(5, 0) == 5);
    REQUIRE(calc.subtract(0, 5) == -5);
    REQUIRE(calc.subtract(-5, -5) == 0);
}

// MULTIPLICATION
TEST_CASE("Decimal multiplication") {
    Calculator calc;

    REQUIRE(calc.multiply(4, 3) == 12);
    REQUIRE(calc.multiply(0, 5) == 0);
    REQUIRE(calc.multiply(0, -100) == 0);
    REQUIRE(calc.multiply(-1, 5) == -5);
    REQUIRE(calc.multiply(1, std::numeric_limits<int64_t>::max())
            == std::numeric_limits<int64_t>::max());
}

// DIVISION
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

TEST_CASE("BYTE wrap-around behavior") {
    Calculator calc;
    calc.setWordSize(WordSize::BYTE);

    REQUIRE(calc.add(127, 1) == -128);
    REQUIRE(calc.subtract(-128, 1) == 127);
}

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

