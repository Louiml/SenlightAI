// Write a C++ function that takes two integers, `a` and `b`, and a third integer `mod` (which is guaranteed to be positive), and returns the result of `(a + b) % mod` using only the `%` operator, but without overflow issues that could occur if `a + b` exceeds the range of `int`. The function should handle all possible `int` values (including negative numbers) correctly, and the result should be in the range `[0, mod-1]`. The function signature should be `int safeAddMod(int a, int b, int mod)`.

#include <cassert>
#include <climits>

int main() {
    // Basic positive cases
    assert(safeAddMod(5, 3, 7) == 1);
    assert(safeAddMod(0, 0, 1) == 0);
    
    // Negative inputs
    assert(safeAddMod(-1, -2, 5) == 2); // (-3) % 5 = -3, +5 = 2
    assert(safeAddMod(-7, 3, 4) == 0); // (-4) % 4 = 0
    
    // Extreme values to avoid overflow
    assert(safeAddMod(INT_MAX, INT_MAX, 1000000007) == (2000000014LL % 1000000007));
    assert(safeAddMod(INT_MIN, INT_MIN, 7) == ((-4294967296LL) % 7 + 7) % 7);
    assert(safeAddMod(INT_MIN, INT_MAX, 13) == ((-1LL) % 13 + 13) % 13);
    
    // Mod = 1 always returns 0
    assert(safeAddMod(123, -456, 1) == 0);
    
    // Exact boundary
    assert(safeAddMod(2, 3, 5) == 0);
    assert(safeAddMod(-2, 3, 5) == 1);
}

#include <cstdint>
#include <limits>

// Computes (a + b) % mod without overflowing int arithmetic.
// Returns a result in the range [0, mod-1].
int safeAddMod(int a, int b, int mod) {
    // Use 64-bit arithmetic to avoid overflow in the addition.
    long long sum = static_cast<long long>(a) + static_cast<long long>(b);
    
    // Take modulo (C++ modulo may yield negative for negative dividend).
    long long result = sum % mod;
    
    // Adjust to non-negative range.
    if (result < 0) {
        result += mod;
    }
    
    // Cast back to int, safe because result is in [0, mod-1] <= INT_MAX.
    return static_cast<int>(result);
}

// The naive approach `(a + b) % mod` can overflow if `a + b` exceeds `INT_MAX` or falls below `INT_MIN`. To avoid this, we can compute the addition using 64-bit arithmetic: cast `a` and `b` to `long long`, sum them, take the modulo with `mod`, and then cast back to `int`. This is safe because `long long` can hold the sum of any two `int` values (max range about ±4.3e9, within 64-bit). The modulo operation in C++ for negative numbers yields a result with the sign of the dividend, so after taking `(sum % mod)`, we must adjust to ensure the result is non-negative: if the result is negative, add `mod` once. The final result will then be in `[0, mod-1]`. Time complexity is `O(1)`, space complexity is `O(1)`. Edge cases: `mod` = 1 always returns 0; negative `a` or `b`; large extremes like `INT_MAX` and `INT_MIN`; and when the adjusted sum is exactly `INT_MIN` (but after modulo and adjustment, the result fits in `int` because `mod` is positive and the final value is less than `mod` which must be ≤ `INT_MAX`? Actually, `mod` can be any positive `int`, so up to `INT_MAX`. The result will be in `[0, mod-1]`, which is always representable as `int`. Also note that `mod` is guaranteed positive, so no division by zero.
