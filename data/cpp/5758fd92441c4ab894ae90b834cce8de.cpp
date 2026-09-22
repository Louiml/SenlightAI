Write a C++ function `int powerWithMultiplications(int base, int exponent)` that computes `base^exponent` using a divide-and-conquer exponentiation strategy (similar to fast exponentiation) and returns the result. Additionally, the function must count the number of multiplication operations performed internally, excluding the final multiplication by `n` when the exponent is odd, and excluding the base case multiplications; the count should increment only when a recursive call is made (once for every recursive call). The function should handle non-negative integers for both base and exponent, with exponent = 0 returning 1 and making zero multiplications, and exponent = 1 returning the base directly without any recursive call (so zero multiplications). For even exponents, exactly one multiplication is counted per recursive level; for odd exponents, exactly two multiplications are counted per recursive level. The signature must be `int powerWithMultiplications(int base, int exponent)` returning only the computed power, while the multiplication count is tracked via a global `int operationCount` variable that is reset to 0 before each call. The function must not use any standard library math functions (like `std::pow`) and must be self-contained.
The solution follows a recursive divide-and-conquer approach. For a given exponent `k`, if `k == 0`, the result is 1 with no multiplications counted. If `k == 1`, the result is the base itself with no multiplications counted. For `k > 1`, we check parity: if `k` is even, we compute `temp = powerWithMultiplications(base * base, k / 2)` after incrementing the global `operationCount` by 1 (since this recursive call represents a multiplication of `base * base`). The result is `temp`. If `k` is odd, we increment `operationCount` by 2 (one for `base * base` and one for the final multiplication by `base`), then compute `temp = powerWithMultiplications(base * base, k / 2)` and return `temp * base`. The key is to count exactly one multiplication per recursive call for even cases and two per recursive call for odd cases, as specified. Edge cases: exponent = 0 and exponent = 1 must return early without any recursive calls, so operationCount remains 0. Large exponents may cause overflow, but since the problem does not specify limits, we assume standard `int` overflow is acceptable (or the function can be adapted to `long long` if needed, but we stick to `int` per specification). Time complexity is O(log k) because each recursive step halves the exponent. Space complexity is O(log k) due to the recursion stack.
#include <cstddef>

// Global counter for multiplication operations performed during exponentiation.
int operationCount = 0;

// Compute base^exponent using divide-and-conquer.
// Precondition: exponent >= 0.
// Postcondition: operationCount is updated to reflect the number of internal multiplications
// (not counting the base case or the final multiply for odd exponents as per spec).
int powerWithMultiplications(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (exponent == 1) {
        return base;
    }

    if (exponent % 2 == 0) {
        // Even: one multiplication for base*base in the recursive call.
        ++operationCount;
        int halfPower = powerWithMultiplications(base * base, exponent / 2);
        return halfPower;
    } else {
        // Odd: two multiplications – one for base*base, one for *base at return.
        operationCount += 2;
        int halfPower = powerWithMultiplications(base * base, exponent / 2);
        return halfPower * base;
    }
}
#include <cassert>

// Global declaration for testing (must match solution).
extern int operationCount;
extern int powerWithMultiplications(int, int);

int main() {
    // Test 1: exponent 0
    operationCount = 0;
    assert(powerWithMultiplications(5, 0) == 1);
    assert(operationCount == 0);

    // Test 2: exponent 1
    operationCount = 0;
    assert(powerWithMultiplications(7, 1) == 7);
    assert(operationCount == 0);

    // Test 3: exponent 2 (even, one recursive call)
    operationCount = 0;
    assert(powerWithMultiplications(3, 2) == 9);
    assert(operationCount == 1);

    // Test 4: exponent 3 (odd, two recursive calls total)
    operationCount = 0;
    assert(powerWithMultiplications(2, 3) == 8);
    // k=3: odd -> count+=2, then recursively k=1 (base case, no count) -> total 2
    assert(operationCount == 2);

    // Test 5: exponent 4 (even then even)
    operationCount = 0;
    assert(powerWithMultiplications(2, 4) == 16);
    // k=4 even -> count 1, then k=2 even -> count 1 more (total 2)
    assert(operationCount == 2);

    // Test 6: exponent 5 (odd then even then base)
    operationCount = 0;
    assert(powerWithMultiplications(2, 5) == 32);
    // k=5 odd -> +2, then k=2 even -> +1, then k=1 base -> total 3
    assert(operationCount == 3);

    // Test 7: base 1 large exponent (no overflow, result 1)
    operationCount = 0;
    assert(powerWithMultiplications(1, 100) == 1);
    // k=100 even -> many steps, but count will be ~log2(100) ≈ 7
    // We can just check it's > 0 but not required; just ensure result correct.

    // Test 8: base 0 exponent 0 (edge, returns 1)
    operationCount = 0;
    assert(powerWithMultiplications(0, 0) == 1);
    assert(operationCount == 0);

    // Test 9: base 0 exponent positive (should be 0)
    operationCount = 0;
    assert(powerWithMultiplications(0, 5) == 0);
    // odd exponent -> count increments, but result is 0*...=0

    return 0;
}
