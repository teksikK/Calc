#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "calculator.h"
#include <limits>
#include <stdexcept>

// ADDITION
TEST_CASE("Decimal addition") {
    Calculator calc;

    REQUIRE(calc.add(2, 3) == 5);
    REQUIRE(calc.add(-1, 1) == 0);
    REQUIRE(calc.add(0, 0) == 0);
    REQUIRE(calc.add(0, 5) == 5);
    REQUIRE(calc.add(-5, 0) == -5);
    REQUIRE(calc.add(std::numeric_limits<double>::max(), 0)
            == std::numeric_limits<double>::max());
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
    REQUIRE(calc.multiply(1, std::numeric_limits<double>::max())
            == std::numeric_limits<double>::max());
}

// DIVISION
TEST_CASE("Decimal division") {
    Calculator calc;

    REQUIRE(calc.divide(10, 2) == 5);
    REQUIRE(calc.divide(0, 5) == 0);
    REQUIRE(calc.divide(-10, 2) == -5);
    REQUIRE(calc.divide(10, -2) == -5);
    REQUIRE(calc.divide(1e-9, 1e9) == Catch::Approx(1e-18));
    REQUIRE_THROWS_AS(calc.divide(5, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.divide(-5, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(calc.divide(0, 0), std::invalid_argument);
}

