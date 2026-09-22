// Write a standalone C++ function `findTargetValue(int n)` that simulates a guessing game where a hidden integer between 1 and `n` (inclusive) must be found using a provided external helper function `int guess(int num)`. The helper returns `1` if the hidden number is higher than `num`, `-1` if lower, and `0` if equal. Your function must use a binary search strategy to determine the hidden value efficiently, handle cases where `n` can be as large as `2^31 - 1` (to avoid integer overflow when computing midpoints), and return the exact hidden number. The function must not rely on any global variables or static state; it only receives `n` and uses the `guess` API. Your implementation should be correct for all valid `n >= 1`, including edge cases like `n = 1` and cases where the hidden number is exactly 1 or exactly `n`.

#include <cassert>

// Mock implementation of guess for testing.
static int hiddenNumber;
int guess(int num) {
    if (num < hiddenNumber) return 1;
    if (num > hiddenNumber) return -1;
    return 0;
}

int main() {
    // Test case 1: n = 1, hidden = 1
    hiddenNumber = 1;
    assert(findTargetValue(1) == 1);

    // Test case 2: n = 10, hidden = 5
    hiddenNumber = 5;
    assert(findTargetValue(10) == 5);

    // Test case 3: n = 10, hidden = 1 (lower boundary)
    hiddenNumber = 1;
    assert(findTargetValue(10) == 1);

    // Test case 4: n = 10, hidden = 10 (upper boundary)
    hiddenNumber = 10;
    assert(findTargetValue(10) == 10);

    // Test case 5: n = 100, hidden = 77
    hiddenNumber = 77;
    assert(findTargetValue(100) == 77);

    // Test case 6: n = 1000000, hidden = 999999
    hiddenNumber = 999999;
    assert(findTargetValue(1000000) == 999999);

    // Test case 7: n = 2147483647, hidden = 1234567890 (tests overflow-safe)
    hiddenNumber = 1234567890;
    assert(findTargetValue(2147483647) == 1234567890);

    // Test case 8: n = 2147483647, hidden = 2147483647 (max boundary)
    hiddenNumber = 2147483647;
    assert(findTargetValue(2147483647) == 2147483647);
}

#include <cstdint>

// Forward declaration of the guess API.
// Returns 1 if the hidden number is higher than num,
// -1 if lower, and 0 if equal.
int guess(int num);

// Find the hidden number between 1 and n using binary search.
// Assumes n >= 1 and the hidden number is within [1, n].
int findTargetValue(int n) {
    int left = 1;
    int right = n;
    while (true) {
        int mid = left + (right - left) / 2; // Prevent overflow
        int result = guess(mid);
        if (result == 0) {
            return mid;
        } else if (result == -1) {
            right = mid - 1;
        } else { // result == 1
            left = mid + 1;
        }
    }
}

// The main algorithm is a classic binary search over the range `[1, n]`. Initialize `left = 1` and `right = n`. In each iteration, compute `mid = left + (right - left) / 2` — this avoids overflow from `(left + right)` which could exceed the maximum 32-bit signed integer. Call `guess(mid)`. If it returns `-1`, the hidden number is lower than `mid`, so set `right = mid - 1`. If it returns `1`, the hidden number is higher, so set `left = mid + 1`. If it returns `0`, we have found the number and return `mid`. The loop continues until `guess(mid)` returns `0`. Edge cases: when `n = 1`, the only possible hidden number is 1, so `mid = 1` and `guess(1)` returns 0 immediately. When the hidden number is at a boundary (1 or n), the binary search still finds it because the range narrows correctly; we never miss it because we adjust `left` or `right` by `±1` after each non-zero guess, ensuring eventual convergence. Time complexity is `O(log n)`, and space complexity is `O(1)` using only a few integer variables. The solution is robust because it only relies on the `guess` API and arithmetic that avoids overflow.
