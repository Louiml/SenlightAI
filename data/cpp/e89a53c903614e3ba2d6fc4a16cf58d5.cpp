// Write a C++ function that accepts two integers, `a` and `b`, and returns an integer: return `1` if either `a` or `b` is exactly equal to `1`; otherwise, return `0`. This function should be pure (no I/O), use `const` parameters, and handle any integer values, including negative numbers, zero, and large values within the `int` range. The behavior must exactly match the logic of the provided snippet: only test for equality with `1` (no other conditions like `a == 1 || b == 1` is the sole condition), and do not consider other values such as `-1` or `0`. The task tests both typical and edge cases.

// The solution is straightforward: the condition is a logical OR of two equality checks: `a == 1` or `b == 1`. If either holds true, the function returns `1`; otherwise, it returns `0`. No special edge cases exist beyond ensuring the comparison is with the integer literal `1` and that no other logic (like `a == 1 || b == 1 || a == -1`) is introduced. The function is `const` correct by taking parameters by value (since they are small integers) and not modifying them. Time complexity is O(1) and space complexity is O(1), as only constant-time operations are performed on fixed-size inputs.

// Returns 1 if either a or b is equal to 1; otherwise returns 0.
int containsOne(int const a, int const b) {
    if (a == 1 || b == 1) {
        return 1;
    }
    return 0;
}

#include <cassert>

int main() {
    assert(containsOne(1, 0) == 1);
    assert(containsOne(0, 1) == 1);
    assert(containsOne(1, 1) == 1);
    assert(containsOne(2, 3) == 0);
    assert(containsOne(-1, 1) == 1);
    assert(containsOne(-5, -1) == 0);
    assert(containsOne(1, -100) == 1);
    assert(containsOne(0, 0) == 0);
    assert(containsOne(2147483647, 1) == 1);
    assert(containsOne(-2147483648, 0) == 0);
}
