/*
Write a C++ function named `isPerfectSquare` that takes a single non-negative integer `n` as input and returns a boolean value indicating whether `n` is a perfect square. A perfect square is an integer that is the square of another integer (e.g., 0, 1, 4, 9, 16, ...). The function must correctly handle very large inputs up to 10^18, avoid floating-point precision errors, and return `true` if and only if `n` is a perfect square. The input will always be a non-negative integer, so no negative checks are required, but the function should still be robust and efficient.
*/

#include <cstdint>

// Returns true if n is a perfect square, false otherwise.
// Uses binary search for the integer square root to avoid floating-point errors.
bool isPerfectSquare(uint64_t n) {
    if (n == 0 || n == 1) return true;
    
    uint64_t low = 1;
    uint64_t high = n;
    
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        // Avoid overflow: check if mid*mid > n without computing mid*mid directly when mid is large
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            uint64_t square = mid * mid;
            if (square == n) {
                return true;
            } else if (square < n) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return false;
}

#include <cassert>

int main() {
    assert(isPerfectSquare(0) == true);
    assert(isPerfectSquare(1) == true);
    assert(isPerfectSquare(4) == true);
    assert(isPerfectSquare(9) == true);
    assert(isPerfectSquare(16) == true);
    assert(isPerfectSquare(25) == true);
    assert(isPerfectSquare(2) == false);
    assert(isPerfectSquare(3) == false);
    assert(isPerfectSquare(15) == false);
    assert(isPerfectSquare(1000000000000000000ULL) == true); // (10^9)^2
    assert(isPerfectSquare(999999999999999999ULL) == false);
    assert(isPerfectSquare(12345678987654321ULL) == true); // 111111111^2
}

// The standard approach to check if an integer is a perfect square without floating-point errors is to use integer arithmetic. Compute the integer square root by binary search or Newton's method, then square it and compare with `n`. Binary search is straightforward: initialize `low = 0` and `high = n` (or `sqrt(n)` initially if using a fast approximation), then repeatedly narrow the interval based on the middle value's square compared to `n`. Since `n` can be as large as 10^18, care must be taken to avoid overflow when computing `mid * mid` (use `unsigned long long` or `long long` with `__int128` for safety, or use division-based comparison). For simplicity, use `unsigned long long` and check `mid > n / mid` to avoid overflow. The algorithm runs in O(log n) time and O(1) space. Edge cases: `n == 0` and `n == 1` are perfect squares; large perfect squares like `10^18` (which is `(10^9)^2`) must be handled without overflow. The binary search approach handles all non-negative integers correctly and deterministically.
