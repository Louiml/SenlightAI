Write a C++ function named `integerSquareRoot` that accepts a single non-negative integer `n` and returns the integer part of its square root (i.e., the floor of √n) as an `int`. The function must not use any floating-point arithmetic or the `<cmath>`/`<math.h>` library’s `sqrt` function. Instead, implement an integer-based algorithm (for example, binary search or Newton’s method with integers). Handle the edge cases `n = 0` and `n = 1` correctly, and be efficient for large `n` up to 2,147,483,647 (the maximum `int`). The function must be `const`-correct and self-contained, returning an `int`.

// The core problem is computing floor(√n) using only integer operations. The simplest robust approach is binary search on the interval `[0, n]`. For any candidate `mid`, compute `mid * mid` and compare to `n`. However, `mid * mid` can overflow for large `mid` (since `mid` can be up to ~46340 for `n` near INT_MAX, which fits in 64-bit but not 32-bit). To avoid overflow, use a 64-bit type like `long long` for intermediate multiplication, or compare `mid > n / mid` (but integer division loses precision). The recommended method: perform binary search with `long long` for safety. Initialize `low = 0`, `high = n` (or better, `high = 46340` constant, but using `n` works). While `low <= high`, compute `mid = low + (high - low) / 2` to avoid overflow, then `long long square = (long long)mid * mid`. If `square == n`, return `mid`; if `square < n`, move `low = mid + 1` and store `mid` as the best answer so far; else move `high = mid - 1`. After the loop, return the last stored `best` value (or equivalently `high`, because the loop exits with `high < low` and `high` is the floor). Edge cases: `n = 0` returns 0, `n = 1` returns 1. Complexity is O(log n) time and O(1) auxiliary space.

#include <cstdint> // for int64_t

// Returns the integer part of the square root of n (floor(√n)).
// Uses binary search on integers, avoiding floating-point and overflow.
int integerSquareRoot(int n) {
    if (n < 0) {
        // Undefined per contract; handle defensively by returning -1.
        return -1;
    }
    if (n == 0 || n == 1) {
        return n;
    }

    int64_t low = 0;
    int64_t high = n;  // √n ≤ n for n ≥ 1
    int best = 0;

    while (low <= high) {
        int64_t mid = low + (high - low) / 2; // avoids overflow
        int64_t square = mid * mid;           // mid ≤ n ≤ INT_MAX, fits in int64_t

        if (square == static_cast<int64_t>(n)) {
            return static_cast<int>(mid);
        } else if (square < static_cast<int64_t>(n)) {
            best = static_cast<int>(mid);
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return best; // equals high after loop
}

#include <cassert>

int main() {
    assert(integerSquareRoot(0) == 0);
    assert(integerSquareRoot(1) == 1);
    assert(integerSquareRoot(2) == 1);
    assert(integerSquareRoot(3) == 1);
    assert(integerSquareRoot(4) == 2);
    assert(integerSquareRoot(15) == 3);
    assert(integerSquareRoot(16) == 4);
    assert(integerSquareRoot(17) == 4);
    assert(integerSquareRoot(99) == 9);
    assert(integerSquareRoot(100) == 10);
    assert(integerSquareRoot(2147483647) == 46340); // max int
    return 0;
}
