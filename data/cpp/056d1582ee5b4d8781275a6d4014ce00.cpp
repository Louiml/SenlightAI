/*
Write a C++ function `bool isHappyNumber(int n)` that determines whether a positive integer `n` is a "happy number." A happy number is defined by the following process: starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle that does not include 1. The function should return `true` if `n` is happy, and `false` otherwise. Handle edge cases such as `n = 1` (trivially happy) and very large inputs (e.g., up to `INT_MAX`) efficiently without unbounded iteration.
*/
#include <cstddef>

// Compute the sum of the squares of the digits of a positive integer.
int sumOfSquaresOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        int digit = n % 10;
        sum += digit * digit;
        n /= 10;
    }
    return sum;
}

// Determine if a positive integer is a happy number using Floyd's cycle detection.
bool isHappyNumber(int n) {
    if (n <= 0) {
        return false; // specification expects positive integers
    }

    int slow = n;
    int fast = sumOfSquaresOfDigits(slow); // one step ahead

    while (fast != 1) {
        slow = sumOfSquaresOfDigits(slow);
        fast = sumOfSquaresOfDigits(sumOfSquaresOfDigits(fast));
        if (slow == fast) {
            return false; // cycle detected without reaching 1
        }
    }
    return true;
}
#include <cassert>

int main() {
    // Known happy numbers
    assert(isHappyNumber(1) == true);
    assert(isHappyNumber(19) == true);
    assert(isHappyNumber(7) == true);
    assert(isHappyNumber(100) == true);
    assert(isHappyNumber(13) == true);

    // Known unhappy numbers
    assert(isHappyNumber(2) == false);
    assert(isHappyNumber(3) == false);
    assert(isHappyNumber(4) == false);
    assert(isHappyNumber(5) == false);
    assert(isHappyNumber(6) == false);

    // Edge case: non-positive input should return false
    assert(isHappyNumber(0) == false);
    assert(isHappyNumber(-5) == false);

    // Large value to ensure no overflow
    assert(isHappyNumber(2147483647) == false);

    // A larger happy number (verified: 789 → 49+64+81=194 → 1+81+16=98 → ... eventually 1)
    assert(isHappyNumber(789) == true);
}
// The core algorithm uses Floyd’s cycle detection (tortoise and hare) to detect whether the process enters a cycle. We maintain two pointers: `slow` starts at `n`, and `fast` starts at `sumOfSquares(n)` (one step ahead). In each iteration, `slow` advances by one step (compute sum of squares once), and `fast` advances by two steps (compute sum of squares twice). If at any point `fast` becomes 1, the number is happy. If `slow` and `fast` become equal before reaching 1, a cycle is detected (and since 1 is not in the cycle), the number is unhappy. This avoids using a hash set and works for any integer size without risk of overflow, because the sum of squares of digits of a 10-digit number (max ~9^2*10 = 810) quickly drops into a small range, so the cycle length is small. Edge cases: `n = 1` returns `true` (the loop condition `fast != 1` is false initially, so the while loop is skipped and we return true). Negative numbers are not in the specification, but we can treat any non-positive input as `false` or handle `n <= 0` explicitly to avoid infinite loops (though not required, we can guard with an `if (n <= 0) return false;`). Time complexity: O(log n) steps on average (each step reduces the number significantly), worst-case O(k) where k is the cycle length (bounded, typically ≤ 8). Space complexity: O(1).
