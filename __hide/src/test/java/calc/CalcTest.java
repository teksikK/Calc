package calc;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class CalcTest {

    @Test
    void InitTest() {
        Calc calc = new Calc();

        assertEquals(0, calc.getDisplayValue());
    }
}
