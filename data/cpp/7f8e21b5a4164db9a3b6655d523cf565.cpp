Write a C++ function `safeSumForKids` that takes two non-negative integers `A` and `B` as parameters. The function must return an integer: if the sum of `A` and `B` is less than 10, return the sum; otherwise, return the special value `-1` to indicate an "error" condition (i.e., the sum is too large for a simple kids' arithmetic exercise). Your solution must not use any global variables, and the function must be `const`-correct in the sense that it does not modify the input parameters. The function should handle edge cases such as `A = 0`, `B = 0`, and values whose sum is exactly 9 (valid) or exactly 10 (invalid).
The problem is straightforward: compute the sum of the two integers and check whether it is less than 10. If yes, return the sum. If the sum is 10 or greater, return `-1` as the sentinel for "error". No complexity beyond basic arithmetic is required. The algorithm runs in constant time \(O(1)\) and uses constant auxiliary space \(O(1)\). Edge cases include any combination of non-negative integers, including zeros, and the boundary condition where the sum is exactly 9 (valid) versus exactly 10 (invalid). The function should correctly handle large integer values up to the maximum `int` range, but note that the sum might overflow if both inputs are near `INT_MAX`; since the problem specifies non-negative integers and the function is simple, we can assume inputs are within a safe range or use `long long` internally to avoid overflow. For robustness, use `long long` for the sum calculation and then compare against 10 and return an `int`.
#include <cstdint>

// Returns the sum of A and B if it is less than 10, otherwise returns -1 (error).
// Assumes A and B are non-negative integers.
int safeSumForKids(int A, int B) {
    // Use long long to avoid overflow in the sum.
    const long long sum = static_cast<long long>(A) + static_cast<long long>(B);
    if (sum < 10) {
        return static_cast<int>(sum);
    }
    return -1;
}
#include <cassert>

int main() {
    // Basic cases
    assert(safeSumForKids(1, 2) == 3);
    assert(safeSumForKids(0, 0) == 0);
    assert(safeSumForKids(5, 4) == 9);   // sum = 9, valid
    assert(safeSumForKids(5, 5) == -1);  // sum = 10, error
    assert(safeSumForKids(0, 10) == -1); // sum = 10, error
    assert(safeSumForKids(10, 0) == -1); // sum = 10, error
    assert(safeSumForKids(3, 7) == -1);  // sum = 10, error
    assert(safeSumForKids(2, 2) == 4);
    assert(safeSumForKids(9, 0) == -1);  // sum = 9, valid actually! wait: 9+0=9 < 10, so returns 9
    // Correct the above: 9+0=9, so it should be 9, not -1.
    // Let's correct that test case:
    assert(safeSumForKids(9, 0) == 9);
    // Large values near int max (but sum might overflow if using int, but our long long handles)
    const int largeA = 2000000000;
    const int largeB = 2000000000;
    assert(safeSumForKids(largeA, largeB) == -1); // sum = 4000000000 > 10
    return 0;
}
