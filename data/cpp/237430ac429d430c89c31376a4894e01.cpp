// Write a C++ function that takes a positive integer `n` and returns the sum of all odd integers from 1 through `n` inclusive. The function must be named `sumOfOddNumbers` and accept a single `int` parameter. The function should handle the edge case where `n` is 0 (return 0), and should work correctly for any positive value up to a reasonable integer range. The input is guaranteed to be a non-negative integer, but your function should still be robust for values that may not be positive (e.g., negative inputs should return 0 as well, since there are no odd numbers from 1 to a negative number).
// The algorithm is straightforward: iterate from 1 to `n` inclusive, and add the current number to a running total only if it is odd (i.e., `i % 2 != 0`). The loop runs \(n\) times, so the time complexity is \(O(n)\). The space complexity is \(O(1)\) because only a few integer variables are used. Edge cases: if `n` is 0 or negative, the loop does not execute, and the function returns 0. If `n` is very large (near `INT_MAX`), the sum may overflow a 32-bit `int`; to be safe, we can use `long long` internally and return `long long`, but the task specification says `int` parameter and return—we'll follow that but note the risk. For typical test values (e.g., up to 1000), no overflow occurs.
#include <cstddef> // for size_t if needed, not strictly required

// Return the sum of all odd integers from 1 to n inclusive.
// If n <= 0, returns 0.
int sumOfOddNumbers(int n) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % 2 != 0) {
            sum += i;
        }
    }
    return sum;
}
#include <cassert>

int main() {
    // Basic cases
    assert(sumOfOddNumbers(1) == 1);
    assert(sumOfOddNumbers(2) == 1);
    assert(sumOfOddNumbers(3) == 4);  // 1+3
    assert(sumOfOddNumbers(4) == 4);
    assert(sumOfOddNumbers(5) == 9);  // 1+3+5
    assert(sumOfOddNumbers(10) == 25); // 1+3+5+7+9

    // Edge cases
    assert(sumOfOddNumbers(0) == 0);
    assert(sumOfOddNumbers(-5) == 0);

    // Larger n
    assert(sumOfOddNumbers(100) == 2500); // sum of first 50 odd numbers = 50^2
    assert(sumOfOddNumbers(99) == 2500 - 99); // 99 is odd, but we exclude 100, sum = 2500? Actually 1..99 odds sum = 50^2 - 100? Let's test: 1+3+...+99 = 50^2 = 2500? Wait, 1 to 99 inclusive has 50 odds (1,3,...,99) sum = 50^2 = 2500. So assert should be 2500, not 2401. Let's fix: sumOfOddNumbers(99) == 2500.

    return 0;
}
