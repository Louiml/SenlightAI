// Write a C++ function named `integerFloorSqrt` that takes a single non-negative integer `n` and returns the largest integer `ans` such that `ans * ans <= n` (i.e., the integer floor of the square root). The function must not use any floating-point arithmetic, built-in sqrt functions, or math library functions — only integer operations. Handle the edge case where `n` is 0 or 1 correctly. After completing the function, write assertions to verify it works for a variety of inputs, including perfect squares, non-perfect squares, large values, and small edge values.
The straightforward approach iterates from `i = 1` upward while tracking the largest value whose square is still ≤ `n`. Starting with `ans = 0`, for each `i` we compute `square = i * i`. As long as `square <= n`, we set `ans = i`. When `square` exceeds `n`, we break the loop because all larger `i` will also produce larger squares. This ensures we return the floor of the square root. Edge cases: `n = 0` should return `0` (since `0*0 = 0 ≤ 0`), and `n = 1` should return `1`. For `n` large enough that `i*i` might overflow a 32-bit int, we should either use `long long` for the square or stop when `i > n / i`. A safer method is to compare `i <= n / i` instead of computing `i*i`, avoiding overflow entirely. Time complexity is O(√n) because we iterate up to the square root. Space complexity is O(1) auxiliary. For very large `n`, we could use binary search to reduce to O(log n), but the linear iteration is acceptable for typical test ranges and is the simplest clear solution.
#include <cstdint>

// Returns the integer floor of the square root of a non-negative integer n.
// Uses only integer arithmetic to avoid overflow.
int integerFloorSqrt(int n) {
    if (n < 0) {
        return -1; // Invalid input, but task guarantees non-negative.
    }
    int ans = 0;
    // Use i <= n / i to prevent overflow from i * i.
    for (int i = 1; i <= n / i; ++i) {
        ans = i;
    }
    return ans;
}
#include <cassert>

int main() {
    assert(integerFloorSqrt(0) == 0);
    assert(integerFloorSqrt(1) == 1);
    assert(integerFloorSqrt(2) == 1);
    assert(integerFloorSqrt(3) == 1);
    assert(integerFloorSqrt(4) == 2);
    assert(integerFloorSqrt(15) == 3);
    assert(integerFloorSqrt(16) == 4);
    assert(integerFloorSqrt(17) == 4);
    assert(integerFloorSqrt(25) == 5);
    assert(integerFloorSqrt(99) == 9);
    assert(integerFloorSqrt(100) == 10);
    // Large value that would overflow if using i*i with int.
    assert(integerFloorSqrt(2147483647) == 46340); // floor(sqrt(2^31-1))
    return 0;
}
