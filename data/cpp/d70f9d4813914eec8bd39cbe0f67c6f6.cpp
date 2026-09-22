Write a C++ function named `addPositiveOnly` that takes two integer parameters and returns their sum. However, if either input is negative, the function should return 0 instead of performing the addition. The solution must be implemented as a standalone, self-contained function with appropriate `const` correctness, and must not include a `main` function or any I/O statements inside the function body. The function should be reusable and testable directly from an external test harness.
The problem is straightforward: compute the sum only when both inputs are non-negative. The main algorithm checks the conditions first. If `n1 < 0 || n2 < 0`, return 0; otherwise return `n1 + n2`. Edge cases include one negative and one non-negative (returns 0), both negative (returns 0), both zero (returns 0), and large positive values that might overflow an `int` — for this task, assume inputs are within the range of `int` so overflow is not a concern. The function is pure and deterministic, with no side effects. Time complexity is \(O(1)\) constant time, and space complexity is \(O(1)\) since we only use a few local variables. No special data structures or algorithms are needed. The solution must respect `const` correctness by marking parameters as `const` or using `const` references if appropriate, but since integers are passed by value, we can simply use `const` on the parameters (though it's optional for ints). The function name should be descriptive and the implementation clear.
#include <cstdint>

// Returns the sum of two non-negative integers. If either input is negative, returns 0.
int addPositiveOnly(const int n1, const int n2) {
    if (n1 < 0 || n2 < 0) {
        return 0;
    }
    return n1 + n2;
}
#include <cassert>

int addPositiveOnly(const int n1, const int n2);

int main() {
    // Both positive
    assert(addPositiveOnly(2, 3) == 5);
    // One negative, one positive
    assert(addPositiveOnly(-1, 5) == 0);
    // Both negative
    assert(addPositiveOnly(-4, -9) == 0);
    // Both zero
    assert(addPositiveOnly(0, 0) == 0);
    // Large positive values
    assert(addPositiveOnly(100000, 200000) == 300000);
    // One zero, one positive
    assert(addPositiveOnly(0, 7) == 7);
    // One zero, one negative
    assert(addPositiveOnly(0, -3) == 0);
}
