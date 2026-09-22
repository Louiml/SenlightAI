// Write a C++ function that takes a positive integer \( n \) as input and returns an integer representing the alternating sum of the series \( 1 - 2 + 3 - 4 + 5 - \dots \pm n \), where the sign alternates: odd numbers are added, even numbers are subtracted. The function must handle \( n \geq 1 \). For example, if \( n = 5 \), the series is \( 1 - 2 + 3 - 4 + 5 = 3 \), and if \( n = 4 \), the result is \( 1 - 2 + 3 - 4 = -2 \). Use only integer arithmetic and avoid overflow for reasonable inputs (e.g., within 32‑bit int range). The function should be named `alternatingSeriesSum`, take a single `int` parameter by value, and return an `int`. Provide a self-contained implementation with proper `const` correctness where applicable.
// The task is straightforward: iterate from 1 to \( n \) inclusive, and for each integer \( i \), add \( i \) to the result if \( i \) is odd, otherwise subtract \( i \). This can be done using a loop or, more efficiently, using a mathematical formula: if \( n \) is even, the sum is \( -n/2 \); if \( n \) is odd, the sum is \( (n+1)/2 \). However, a loop is simpler and clearly demonstrates the logic. Edge cases include \( n = 1 \) (result = 1) and \( n = 2 \) (result = -1). The algorithm has \( O(n) \) time complexity if using a loop, but with the formula it is \( O(1) \) time and \( O(1) \) space. The loop version uses a single accumulator variable and no extra space. Since \( n \) is positive and the result fits in a 32‑bit int for reasonable \( n \) (up to about 10^5 for a loop, but the formula works for much larger), we do not need special overflow handling. We will implement the loop approach for clarity, but note the constant-time alternative in comments. For a robust solution, we use a descriptive function signature and avoid global variables.
#include <cstddef> // for size_t (not directly used, but good practice)

// Compute the alternating sum: 1 - 2 + 3 - 4 + ... ± n
// where odd numbers are added and even numbers are subtracted.
// Precondition: n >= 1.
int alternatingSeriesSum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % 2 == 1) {
            sum += i;   // odd: add
        } else {
            sum -= i;   // even: subtract
        }
    }
    return sum;
}
#include <cassert>

int main() {
    assert(alternatingSeriesSum(1) == 1);
    assert(alternatingSeriesSum(2) == -1);
    assert(alternatingSeriesSum(3) == 2);
    assert(alternatingSeriesSum(4) == -2);
    assert(alternatingSeriesSum(5) == 3);
    assert(alternatingSeriesSum(10) == -5);
    assert(alternatingSeriesSum(100) == -50);
    assert(alternatingSeriesSum(101) == 51);
    return 0;
}
