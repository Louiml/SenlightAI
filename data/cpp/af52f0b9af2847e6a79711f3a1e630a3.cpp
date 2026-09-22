Given a positive integer `n` (where `n >= 1` and `n <= 10^9`), write a C++ function `int floorDivisionByThree(int n)` that returns the result of the integer division of `n` by 3 (i.e., `n / 3` using integer truncation toward zero). The function must handle all valid inputs without overflow, and since the division is exact and non-negative for positive `n`, the standard integer division operator is sufficient. The task is straightforward: implement the function to compute and return `n / 3` correctly for any `n` in the given range.

#include <cassert>

int floorDivisionByThree(const int n);

int main() {
    assert(floorDivisionByThree(1) == 0);
    assert(floorDivisionByThree(2) == 0);
    assert(floorDivisionByThree(3) == 1);
    assert(floorDivisionByThree(4) == 1);
    assert(floorDivisionByThree(5) == 1);
    assert(floorDivisionByThree(6) == 2);
    assert(floorDivisionByThree(999) == 333);
    assert(floorDivisionByThree(1000) == 333);
    assert(floorDivisionByThree(1000000000) == 333333333);
    return 0;
}

#include <cstdint>

// Returns the integer result of dividing n by 3, truncating toward zero.
// Precondition: n >= 1 and n <= 1e9.
int floorDivisionByThree(const int n) {
    return n / 3;
}

// The problem is trivial: for any positive integer `n`, integer division by 3 using the `/` operator in C++ truncates toward zero, which for positive numbers is equivalent to floor division. The range `1 <= n <= 1e9` fits comfortably within a 32-bit signed integer (`int`), so no overflow occurs in the division itself. The only edge cases to consider are `n = 1`, `n = 2`, and `n = 3`, where the results are 0, 0, and 1 respectively. The solution has time complexity `O(1)` and space complexity `O(1)`, as it performs a single arithmetic operation and returns the result. No special libraries or algorithms are needed; the function simply returns `n / 3`. This task is designed to be an entry-level exercise in writing a simple, correct arithmetic function with appropriate `const` correctness (though the parameter is passed by value, so `const` is optional but can be applied to the parameter to signal non-modification).
