Write a C++ function `myCubert` that takes a positive integer `x` and returns the integer cube root of `x`. The integer cube root is the largest integer `n` such that `n^3 ≤ x`. For example, for `x = 16`, the integer cube root is `2` because `2^3 = 8 ≤ 16` and `3^3 = 27 > 16`. The function must handle `x = 1` correctly (returning `1`), and must not use floating-point arithmetic or built-in cube root functions. Implement the algorithm using binary search.

#include <cassert>

int main() {
    assert(integerCubeRoot(1) == 1);
    assert(integerCubeRoot(2) == 1);
    assert(integerCubeRoot(7) == 1);
    assert(integerCubeRoot(8) == 2);
    assert(integerCubeRoot(9) == 2);
    assert(integerCubeRoot(16) == 2);
    assert(integerCubeRoot(26) == 2);
    assert(integerCubeRoot(27) == 3);
    assert(integerCubeRoot(64) == 4);
    assert(integerCubeRoot(100) == 4);
    return 0;
}

#include <cstdint>

// Returns the integer cube root of a positive integer x,
// i.e., the largest n such that n^3 <= x.
int integerCubeRoot(int x) {
    if (x <= 1) {
        return x; // Handles x == 0 and x == 1
    }

    int ans = 0;
    int left = 1;
    int right = x;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Avoid overflow by dividing instead of computing mid^3
        if (mid == x / (mid * mid)) {
            ans = mid;
            break;
        } else if (mid < x / (mid * mid)) {
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return ans;
}

// The algorithm uses binary search on the range `[1, x]` to find the largest integer `n` such that `n³ ≤ x`. At each step, the midpoint `mid` is checked against `x/(mid*mid)` to avoid integer overflow (since `mid³` could overflow 32-bit integers). If `mid == x/(mid*mid)` (i.e., `mid³ == x`), we have found an exact root. If `mid < x/(mid*mid)` (i.e., `mid³ < x`), then `mid` could be a candidate answer, so we store it and move the search to the right half. If `mid > x/(mid*mid)` (i.e., `mid³ > x`), we move to the left half. The binary search terminates in `O(log x)` iterations, and each iteration does constant work, so time complexity is `O(log x)`. Space complexity is `O(1)`. Edge cases: `x = 1` returns `1` because `1*1*1 == 1`; for `x = 0`, the original snippet assumes positive input, but the specification says positive integer, so no need to handle zero.
