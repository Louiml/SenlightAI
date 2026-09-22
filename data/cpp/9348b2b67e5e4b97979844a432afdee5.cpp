// Implement a standalone C++ function named `computeTribonacci` that, given a nonnegative integer `n`, returns the `n`-th Tribonacci number `T(n)`, where `T(0)=0`, `T(1)=0`, `T(2)=1`, and for `n >= 3`, `T(n) = T(n-1) + T(n-2) + T(n-3)`. The function must use recursive top-down dynamic programming with memoization via a `std::vector` that stores computed values and uses a sentinel constant for "unknown" entries. The return type must be an unsigned 64-bit integer (`uint_fast64_t`). Assume the input `n` is such that the result fits without overflow; for `uint_fast64_t`, the maximum safe `n` is 37 (since `T(37) = 2082876103` fits, but `T(38)` overflows). The function signature must be `uint_fast64_t computeTribonacci(int n)` and must not use global variables or any external libraries beyond `<cstdint>` and `<vector>`. Handle edge cases: `n=0`, `n=1`, and `n=2` return the base values without recursion.
// The solution uses a recursive helper function that takes a reference to a `std::vector<uint_fast64_t>` table and an integer `n`. At each call, it first checks whether the current `n` is within the table's size and if the stored value is not the sentinel `UNKNOWN` (which is set to `UINT_FAST64_MAX` for clarity, since no valid Tribonacci number equals that). If found, return the cached value. Otherwise, resize the table to at least `n+1` elements, initializing new entries to `UNKNOWN`. Then handle base cases: if `n <= 2`, assign `n == 2 ? 1 : 0`. For `n >= 3`, recursively compute `T(n-1)`, `T(n-2)`, and `T(n-3)`, sum them, store the result in the table, and return it. This memoization ensures each subproblem is computed only once. `n=0` and `n=1` both return 0, and `n=2` returns 1. The time complexity is `O(n)` because each index from 0 to `n` is computed at most once, and the space complexity is `O(n)` for the table plus recursion stack depth `O(n)`. Edge cases: negative `n` is not defined by the task, so assume nonnegative input; overflow is explicitly limited by preconditions.
#include <cstdint>
#include <vector>

// Type alias for large unsigned integer type
using bignum = uint_fast64_t;

// Sentinal value for unknown entries in the memo table
const bignum UNKNOWN = UINT_FAST64_MAX;

// Recursive helper function with memoization
bignum trib_recurse(std::vector<bignum>& table, int n) {
    // Check if already computed
    if (static_cast<std::vector<bignum>::size_type>(n) < table.size()
        && table[n] != UNKNOWN) {
        return table[n];
    }

    // Resize table if needed
    if (table.size() <= static_cast<std::vector<bignum>::size_type>(n)) {
        table.resize(n + 1, UNKNOWN);
    }

    bignum result;
    // Base cases
    if (n <= 1) {
        result = 0;
    } else if (n == 2) {
        result = 1;
    } else {
        // Recursive case: T(n) = T(n-1) + T(n-2) + T(n-3)
        result = trib_recurse(table, n - 1)
               + trib_recurse(table, n - 2)
               + trib_recurse(table, n - 3);
    }

    table[n] = result;
    return result;
}

// Public function: returns the n-th Tribonacci number
bignum computeTribonacci(int n) {
    std::vector<bignum> table;
    return trib_recurse(table, n);
}
#include <cassert>

// Global main function for testing
int main() {
    // Base cases
    assert(computeTribonacci(0) == 0);
    assert(computeTribonacci(1) == 0);
    assert(computeTribonacci(2) == 1);

    // Small known values
    assert(computeTribonacci(3) == 1);  // 0+0+1
    assert(computeTribonacci(4) == 2);  // 0+1+1
    assert(computeTribonacci(5) == 4);  // 1+1+2
    assert(computeTribonacci(6) == 7);  // 1+2+4
    assert(computeTribonacci(7) == 13); // 2+4+7

    // Larger valid value (fits in uint_fast64_t)
    assert(computeTribonacci(20) == 66012);
    assert(computeTribonacci(30) == 29249425);
    assert(computeTribonacci(36) == 1088123586);
    assert(computeTribonacci(37) == 2082876103); // Largest safe for uint_fast64_t

    // Verify a sequence property for n=10: T(10)=T(9)+T(8)+T(7)
    uint_fast64_t t9 = computeTribonacci(9);
    uint_fast64_t t8 = computeTribonacci(8);
    uint_fast64_t t7 = computeTribonacci(7);
    assert(computeTribonacci(10) == t9 + t8 + t7);

    return 0;
}
