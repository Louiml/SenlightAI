/*
Write a C++ function that, given a non-negative integer `n`, returns the sum of the digits of `n` using recursion. The function must handle the edge case `n = 0` correctly, returning `0`. You must implement the function using only recursive calls (no loops) and the extraction of the last digit via `n % 10` and the quotient via `n / 10`. Ensure the signature is `int digitSum(int n)`.
*/
// Return the sum of the digits of a non-negative integer n using recursion.
int digitSum(int n) {
    // Base case: when n becomes 0, the sum is 0.
    if (n == 0) {
        return 0;
    }
    // Recursive case: sum of last digit plus sum of remaining digits.
    return (n % 10) + digitSum(n / 10);
}
#include <cassert>

int main() {
    assert(digitSum(0) == 0);
    assert(digitSum(5) == 5);
    assert(digitSum(123) == 6);
    assert(digitSum(9999) == 36);
    assert(digitSum(1001) == 2);
    assert(digitSum(2048) == 14);
    assert(digitSum(7) == 7);
    assert(digitSum(10) == 1);
    return 0;
}
// The recursive approach reduces the problem by repeatedly stripping the last digit. The base case occurs when `n` becomes `0`, at which point the sum is `0`. For any `n > 0`, the sum of digits equals `(n % 10)` plus the recursive sum of `n / 10`. This works for all non-negative integers, including `0` (base case directly returns `0`). Negative inputs are not expected per the problem statement, but if passed, the function would recursively process the negative number and eventually hit the base case incorrectly; thus we assume non-negative input. Time complexity is \(O(k)\), where \(k\) is the number of digits in `n`, because each recursive call reduces `n` by one digit. Space complexity is \(O(k)\) due to the call stack depth equal to the number of digits.
