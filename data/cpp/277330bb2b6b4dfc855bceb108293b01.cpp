// Given two integers A and B, write a C++ function `int missingDigit(int A, int B)` that determines the value needed to reach a target sum of 20 when combined with the sum of A and B. Specifically, if the sum of A and B is already 11 or greater, the function should return -1 (indicating the target is unattainable). Otherwise, it should return the number that, when added to (A + B), exactly equals 20 (i.e., `20 - (A + B)`). The function must handle negative and zero inputs, and all standard integer values within the typical 32-bit range.
The core idea is simple arithmetic: first compute the sum S = A + B. The target is to reach 20 by adding some integer X, so X must equal 20 - S. However, if S ≥ 11, then even adding the smallest possible positive integer (1) would exceed or meet 20? Wait—actually the condition in the snippet is `sum < 11` to return `21 - sum`, which is equivalent to `20 - sum + 1`. Let's re-examine. The original snippet: if sum < 11, output 21 - sum; else -1. That means when sum < 11, you return 21 - sum, which is exactly (20 - sum) + 1. So you need to add one more than what's needed to reach 20, implying the target is actually 21? Let's clarify: The snippet's condition is `sum < 11` → return `21 - sum`. So if sum = 10, return 11, and 10 + 11 = 21. So the real target is 21, not 20. The condition says if sum is less than 11, then you can reach 21 by adding 21 - sum. If sum ≥ 11, then it's impossible to reach 21 with a positive integer? But note: if sum = 11, you could add 10 to get 21, but the snippet returns -1 because sum is not < 11. So the actual rule: only when sum < 11, you can add a number to make it exactly 21. So we need to design the function to reflect that rule: if A + B < 11, return 21 - (A + B); else return -1. Edge cases: negative sums? Condition `sum < 11` includes negative sums, so for sum = -5, return 21 - (-5) = 26. That's valid. If sum is exactly 11, return -1. Time complexity is O(1), space O(1). We must handle integer overflow if A and B are near INT_MAX, but sum fits in int for typical ranges; to be safe, use long long for intermediate sum. However the task says "typical 32-bit range", so int is fine, but we can use long long to be safe and robust.
#include <cstdint>

// Return the value to add to (A + B) to equal 21 if possible, else -1.
int missingDigit(int A, int B) {
    const long long sum = static_cast<long long>(A) + B;
    if (sum < 11) {
        return static_cast<int>(21 - sum);
    }
    return -1;
}
#include <cassert>
#include <climits>

int missingDigit(int A, int B);  // forward declaration

int main() {
    // Basic cases
    assert(missingDigit(5, 4) == 12);    // sum=9, 21-9=12
    assert(missingDigit(10, 0) == 11);   // sum=10, 21-10=11
    assert(missingDigit(11, 0) == -1);   // sum=11, not < 11
    assert(missingDigit(20, 1) == -1);   // sum=21, not < 11
    // Negative sums are allowed
    assert(missingDigit(-5, -3) == 29);  // sum=-8, 21-(-8)=29
    // Zero and negative inputs
    assert(missingDigit(0, 0) == 21);
    assert(missingDigit(-1, 1) == 21);   // sum=0
    // Boundary just below 11
    assert(missingDigit(1, 9) == 11);    // sum=10
    assert(missingDigit(2, 8) == 11);    // sum=10
    // Boundary at 10 and 1
    assert(missingDigit(10, 1) == -1);   // sum=11
    // Extreme values (use long long internally)
    assert(missingDigit(INT_MAX, INT_MIN) == -1); // sum = -1, actually -1 < 11 -> 21 - (-1)=22
    // Wait: INT_MAX + INT_MIN = -1, so sum = -1 < 11, return 22
    assert(missingDigit(INT_MAX, INT_MIN) == 22);
    // Another extreme
    assert(missingDigit(INT_MIN, INT_MAX) == 22);
    // Large positive values
    assert(missingDigit(1000000000, 1000000000) == -1); // sum=2000000000 >= 11
    // Large negative values
    assert(missingDigit(-1000000000, -1000000000) == 2000000021); // sum=-2000000000 < 11 -> 21 - (-2000000000)
    return 0;
}
