// Write a C++ function that takes two positive integers `num1` and `num2`, and returns an integer representing the result of `num1 * 1000 / num2` using integer arithmetic (truncating any fractional part). The function must handle the case where `num2` is zero by returning 0, and must be implemented without floating-point operations. The result should always be non-negative since inputs are positive. Ensure the function is `const`-correct and uses descriptive names. The function signature should be `int scaledRatio(int num1, int num2)`.

The main algorithm is straightforward: multiply `num1` by 1000, then divide by `num2` using integer division. Because inputs are positive integers, the intermediate multiplication cannot overflow for typical 32-bit ranges (since `num1 * 1000` could overflow if `num1` exceeds ~2.1 million, but for the intended scope we assume inputs are within safe bounds; if broader safety is needed, one could use `long long` internally, but the task implies standard int). The edge case is `num2 == 0` — division by zero is undefined, so we must guard against it and return a sentinel value (0 per specification). No other special cases exist since inputs are positive; duplicates and large values follow the same formula. Time complexity is O(1) and space complexity is O(1) — just a few integer operations and a temporary variable.

#include <cstdint> // for int32_t if needed, but we use int

// Returns (num1 * 1000) / num2 using integer arithmetic.
// Returns 0 if num2 is zero.
int scaledRatio(int num1, int num2) {
    if (num2 == 0) {
        return 0;
    }
    int answer = num1 * 1000;
    answer = answer / num2;
    return answer;
}

#include <cassert>

int main() {
    // Basic case: 2 * 1000 / 4 = 500
    assert(scaledRatio(2, 4) == 500);
    
    // Truncation: 5 * 1000 / 3 = 1666 (1666.666... truncated)
    assert(scaledRatio(5, 3) == 1666);
    
    // Larger numerator: 10 * 1000 / 2 = 5000
    assert(scaledRatio(10, 2) == 5000);
    
    // Edge case: denominator zero returns 0
    assert(scaledRatio(7, 0) == 0);
    
    // Numerator 1: 1 * 1000 / 1 = 1000
    assert(scaledRatio(1, 1) == 1000);
    
    // Exact division: 8 * 1000 / 8 = 1000
    assert(scaledRatio(8, 8) == 1000);
    
    // Large numerator still within int range: 2000 * 1000 / 500 = 4000
    assert(scaledRatio(2000, 500) == 4000);
    
    return 0;
}
