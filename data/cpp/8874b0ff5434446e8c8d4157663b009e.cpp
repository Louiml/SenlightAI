// Write a C++ function `digitSum` that takes a non-negative 64-bit integer `x` and returns the sum of its decimal digits. For example, `digitSum(12345)` should return `15`, and `digitSum(0)` should return `0`. The function must handle numbers up to the maximum value of `long long` (i.e., up to 9,223,372,036,854,775,807) correctly without overflow, and it must not use any string conversion or library functions other than basic arithmetic. The function must be `const`-correct with respect to its parameter, and the algorithm should work for all non-negative values including those where division by 10 and modulo operations are used repeatedly.
The simplest approach is to repeatedly extract the last digit using the modulo operator (`% 10`) and add it to a running total, then remove the last digit using integer division (`/ 10`). This continues until the number becomes zero. The key edge case is when `x` is `0`; in that case, the loop should not execute and the function should return `0`. Another edge case is very large `long long` values, but since the sum of digits for the maximum 64-bit integer is at most `9 × 19 = 171` (because there are at most 19 digits in `long long`), the total sum fits comfortably in a 32-bit integer, so using `int` for the sum is safe. The algorithm runs in `O(d)` time, where `d` is the number of decimal digits (at most 19), and uses `O(1)` auxiliary space. The function parameter should be passed by value (or by const reference, but by value is simpler and sufficient for an integer), and the function itself should be marked as `const` if it were a member, but as a free function we just ensure the parameter is const-correct by taking it by value (no mutation inside the function's visible effect).
#include <cstdint>

// Returns the sum of decimal digits of a non-negative 64-bit integer.
// Time: O(number of digits), Space: O(1)
int digitSum(const long long x) {
    long long value = x;  // Local copy to allow modification without changing the caller's variable
    int sum = 0;
    do {
        sum += static_cast<int>(value % 10);
        value /= 10;
    } while (value > 0);
    return sum;
}
#include <cassert>

int main() {
    assert(digitSum(0) == 0);
    assert(digitSum(1) == 1);
    assert(digitSum(9) == 9);
    assert(digitSum(10) == 1);
    assert(digitSum(12345) == 15);
    assert(digitSum(999999999) == 81);
    assert(digitSum(1000000000000000000LL) == 1); // 10^18
    assert(digitSum(9223372036854775807LL) == 88); // maximum long long
    assert(digitSum(1111111111111111111LL) == 19); // 19 ones
    return 0;
}
