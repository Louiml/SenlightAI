Write a C++ function named `isPerfectSquare` that takes a single non-negative integer `n` and returns `true` if `n` is a perfect square (i.e., there exists an integer `m` such that `m * m == n`), and `false` otherwise. The input `n` is guaranteed to be within the range `[0, 2^31 - 1]`. The function must not use built-in square-root functions, and it must be efficient enough to handle the maximum input value quickly. Focus on an approach using binary search to achieve logarithmic time complexity.

#include <cassert>

int main() {
    assert(isPerfectSquare(0) == true);
    assert(isPerfectSquare(1) == true);
    assert(isPerfectSquare(4) == true);
    assert(isPerfectSquare(16) == true);
    assert(isPerfectSquare(25) == true);
    assert(isPerfectSquare(2147395600) == true); // 46340^2
    assert(isPerfectSquare(2) == false);
    assert(isPerfectSquare(15) == false);
    assert(isPerfectSquare(2147483647) == false); // INT_MAX
    assert(isPerfectSquare(46341) == false); // just above sqrt(INT_MAX)
    return 0;
}

#include <cstdint>

// Returns true if n is a perfect square, false otherwise.
bool isPerfectSquare(int n) {
    if (n < 0) return false;
    int low = 0;
    int high = 46340; // floor(sqrt(INT_MAX))
    while (low <= high) {
        int mid = low + (high - low) / 2;
        std::int64_t square = static_cast<std::int64_t>(mid) * mid;
        if (square == n) return true;
        if (square < n) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

// The problem reduces to determining whether there exists an integer `x` with `x * x == n`. Since `n` is non-negative and fits in a 32-bit signed integer, the maximum possible root is `46340` (because `46341^2 > 2^31 - 1`). A binary search over the range `[0, 46340]` can find the root in `O(log n)` time. At each iteration, compute `mid` and compare `mid * mid` with `n`. Since `mid * mid` may overflow `int` for large `mid`, use `long long` for the multiplication. If `mid * mid == n`, return `true`. If `mid * mid < n`, move the lower bound up; otherwise, move the upper bound down. If the loop ends without finding a match, return `false`. Edge cases include `n = 0` (which is `0^2`) and `n = 1` (which is `1^2`), both handled naturally by the binary search. Time complexity is `O(log 46340)` ≈ `O(1)` effectively, and space complexity is `O(1)`.
