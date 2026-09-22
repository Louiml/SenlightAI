Write a C++ function `bool areMultiples(int first, int second)` that determines whether two integer values are multiples of one another. The function must return `true` if either number divides the other without a remainder (i.e., `first % second == 0` or `second % first == 0`), and `false` otherwise. The function should handle any integer inputs, including negative numbers and zero. For the purpose of this task, zero is considered a multiple of any non‑zero integer only if the non‑zero integer divides zero (i.e., `first % second == 0` when `second != 0` and `first == 0` returns `true`). However, division by zero is undefined in C++, so the function must explicitly avoid using the modulo operator when the divisor is zero, and it should return `false` in that case. The function must be `const`‑correct and should not rely on any global state.
The main algorithm is straightforward: to check if two integers are multiples of each other, we need to test divisibility in both directions. First, we must handle the edge case where either number is zero. If both are zero, then neither can divide the other (since division by zero is undefined), so we return `false`. If exactly one is zero, the zero value is divisible by any non‑zero divisor (because `0 % x == 0` for any non‑zero `x`), so we return `true` only if the non‑zero number divides zero — which it always does. However, the simpler interpretation is: if one value is zero and the other is not, then the non‑zero value divides zero, so they are multiples. But if the divisor is zero (i.e., we try to check `x % 0`), that is undefined, so we must avoid that branch. The safest approach: first, if either number is zero, return `(first == 0 && second != 0)` or `(second == 0 && first != 0)` — but that actually always returns `true` when exactly one is zero. Actually, careful: if `first = 0`, `second = 5`, then `5 % 0` is invalid, but `0 % 5 == 0` is valid and true. So we can check: if `second != 0` and `first % second == 0`, return true. Similarly, if `first != 0` and `second % first == 0`, return true. If both are zero, both branches are invalid (division by zero), so return false. This yields the correct behavior: `(0,5)` → true, `(5,0)` → true, `(0,0)` → false. For non‑zero values, we simply test `first % second == 0 || second % first == 0`. Edge cases include negative numbers — the modulo operator in C++ works with negatives, and divisibility is unaffected by sign, so `(-6, 3)` → true, `(6, -3)` → true. Time complexity is O(1) for the few modulo operations, and space complexity is O(1).
#include <cstddef> // not necessary, but included for completeness

// Returns true if either value divides the other without a remainder.
bool areMultiples(int first, int second) {
    // Avoid division by zero: if both are zero, neither divides the other.
    if (first == 0 && second == 0) {
        return false;
    }
    // If first is zero and second is non-zero, then second divides first (0 % second == 0).
    if (first == 0) {
        return second != 0;
    }
    // If second is zero and first is non-zero, then first divides second (0 % first == 0).
    if (second == 0) {
        return first != 0;
    }
    // For non-zero values, check divisibility in both directions.
    return (first % second == 0) || (second % first == 0);
}
#include <cassert>

int main() {
    // Non-zero, positive multiples
    assert(areMultiples(12, 4) == true);
    assert(areMultiples(4, 12) == true);
    // Non-multiples
    assert(areMultiples(7, 3) == false);
    // Negative numbers (sign doesn't affect divisibility)
    assert(areMultiples(-12, 4) == true);
    assert(areMultiples(12, -4) == true);
    assert(areMultiples(-7, 3) == false);
    // One zero and one non-zero: zero is a multiple of any non-zero number
    assert(areMultiples(0, 5) == true);
    assert(areMultiples(5, 0) == true);
    // Both zero: undefined division, so return false
    assert(areMultiples(0, 0) == false);
    // Equal non-zero numbers are always multiples
    assert(areMultiples(8, 8) == true);
    // Larger example
    assert(areMultiples(1000000, 1000) == true);
}
