// Write a C++ function named `processRandomBilangan` that simulates the logic of the provided snippet: it takes a non-negative integer `input`, and if that integer is less than 5, the function returns the original value unchanged and does not double it. Otherwise (if the integer is 5 or greater), the function returns the doubled value (2 × input). The function must work for any non-negative integer, including 0, and must not rely on randomness or time — it should be deterministic based only on the input parameter. Provide a clean, self-contained implementation with appropriate `const` correctness (the parameter may be passed by value since it is fundamental).
// The main algorithm is a simple conditional transformation: read the input integer, check if it is strictly less than 5. If yes, return it as-is; if no (i.e., it is ≥ 5), multiply by 2 and return the result. Edge cases: input 0–4 all return the same value (since they satisfy `input < 5`); input exactly 5 returns 10; large inputs can be doubled without overflow as long as the input fits in an `int` (use `long long` for extra safety if needed, but the problem specifies `int`). No loops or extra data structures are needed. Time complexity is O(1), space complexity is O(1). The function should be marked `const`-qualified or accept a `const int` if it's a member, but for a free function, we just ensure the parameter is passed by value and we do not modify the original.
// Process a non-negative integer: if it is less than 5, return unchanged; otherwise, return double.
int processRandomBilangan(const int input) {
    if (input < 5) {
        return input;
    }
    return 2 * input;
}
#include <cassert>

int main() {
    // Test values below 5: unchanged
    assert(processRandomBilangan(0) == 0);
    assert(processRandomBilangan(1) == 1);
    assert(processRandomBilangan(4) == 4);

    // Test value equal to 5: doubled
    assert(processRandomBilangan(5) == 10);

    // Test values above 5: doubled
    assert(processRandomBilangan(6) == 12);
    assert(processRandomBilangan(10) == 20);
    assert(processRandomBilangan(100) == 200);

    // Boundary test: 5 is exactly the threshold
    assert(processRandomBilangan(5) == 10);

    // Large value (within int range)
    assert(processRandomBilangan(100000) == 200000);

    return 0;
}
