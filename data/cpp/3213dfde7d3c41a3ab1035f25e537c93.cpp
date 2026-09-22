Write a C++ function that takes a positive two-digit integer (10–99) and returns the quotient of its tens digit divided by its units digit (i.e., integer division, truncating toward zero). For example, for 84 the result is 8/4 = 2, for 31 the result is 3/1 = 3, and for 50 the result is 5/0 which is undefined — so the function must handle this case by returning a sentinel value of -1 instead. The function should accept the integer by value, be const-correct, and only process valid two‑digit inputs (you may assume the caller passes a number in [10, 99]; if not, behavior is undefined). Write the function with a descriptive name, provide necessary headers, and include comments explaining the algorithm.
// The core algorithm is straightforward: given a two‑digit integer `num`, extract the tens digit as `num / 10` (integer division) and the units digit as `num % 10`. Then perform integer division of the tens digit by the units digit. The only edge case is when the units digit is zero, because division by zero is undefined in C++. In that case, return a sentinel value (e.g., -1) to indicate an invalid operation. For all other valid inputs, the quotient is well‑defined because both digits are in the range 1–9 and the tens digit is always ≥1. Time complexity is O(1) — only a few arithmetic operations. Space complexity is O(1) — no extra data structures are used. The function is deterministic and does not depend on any external state.
#include <stdexcept> // Not needed for the sentinel approach, but kept for clarity if needed.

// Returns the integer quotient of the tens digit divided by the units digit of a two-digit number.
// If the units digit is zero, returns -1 to indicate division by zero (undefined operation).
int tensDividedByUnits(int num) {
    const int tens = num / 10; // Extract tens digit (e.g., for 84, tens = 8)
    const int units = num % 10; // Extract units digit (e.g., for 84, units = 4)
    
    if (units == 0) {
        return -1; // Division by zero is not allowed
    }
    return tens / units; // Integer division truncates toward zero
}
#include <cassert>

int main() {
    // Normal cases
    assert(tensDividedByUnits(84) == 2); // 8/4 = 2
    assert(tensDividedByUnits(31) == 3); // 3/1 = 3
    assert(tensDividedByUnits(99) == 1); // 9/9 = 1
    assert(tensDividedByUnits(10) == -1); // units digit zero
    assert(tensDividedByUnits(20) == -1); // units digit zero
    assert(tensDividedByUnits(55) == 1); // 5/5 = 1
    assert(tensDividedByUnits(92) == 4); // 9/2 = 4 (integer division)
    assert(tensDividedByUnits(73) == 2); // 7/3 = 2 (integer division)
    assert(tensDividedByUnits(41) == 4); // 4/1 = 4
    assert(tensDividedByUnits(77) == 1); // 7/7 = 1
    
    return 0;
}
