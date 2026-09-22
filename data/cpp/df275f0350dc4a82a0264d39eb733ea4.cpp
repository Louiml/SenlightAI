Write a C++ function `sumEvenFibonacciBelowLimit(int limit)` that returns the sum of all even-valued numbers in the Fibonacci sequence whose values do not exceed the given limit. The Fibonacci sequence begins with 0, 1, then each subsequent term is the sum of the two preceding ones. You must consider only even terms (e.g., 0, 2, 8, 34, ...), and if the limit is less than 2, the function should return 0. The function must handle large intermediate values using `long long` internally to avoid overflow, but return an `int` (the sum is guaranteed to fit within an `int` for typical limits). The function should be pure, use const-correctness where appropriate, and contain no input/output logic.

The solution iteratively generates the Fibonacci-like sequence of even terms only. It starts with the first two even numbers: `ef1 = 0` and `ef2 = 2`, and initializes the sum as their sum. Then, using the identity `ef3 = 4 * ef2 + ef1` (which holds for every third Fibonacci number—the even ones), it computes the next even term. The loop continues as long as the next term does not exceed the limit. If `ef3` exceeds the limit, the loop breaks. Edge cases include limits less than 2, for which the function returns 0 (since the smallest even Fibonacci number greater than 0 is 2). Also, if the limit is exactly 2, the sum is 2 (0 + 2). The algorithm runs in O(k) time where k is the number of even Fibonacci numbers up to the limit, which is logarithmic in the limit. Space complexity is O(1).

#include <cstddef>

// Returns the sum of all even Fibonacci numbers not exceeding `limit`.
// Returns 0 if limit < 2.
int sumEvenFibonacciBelowLimit(int limit) {
    if (limit < 2) return 0;

    long long ef1 = 0; // first even Fibonacci number
    long long ef2 = 2; // second even Fibonacci number
    long long sum = ef1 + ef2;

    while (true) {
        long long ef3 = 4 * ef2 + ef1; // next even Fibonacci number
        if (ef3 > limit) break;
        ef1 = ef2;
        ef2 = ef3;
        sum += ef2;
    }

    return static_cast<int>(sum);
}

#include <cassert>

int main() {
    assert(sumEvenFibonacciBelowLimit(0) == 0);
    assert(sumEvenFibonacciBelowLimit(1) == 0);
    assert(sumEvenFibonacciBelowLimit(2) == 2);
    assert(sumEvenFibonacciBelowLimit(8) == 10); // 0+2+8
    assert(sumEvenFibonacciBelowLimit(10) == 10); // 0+2+8
    assert(sumEvenFibonacciBelowLimit(34) == 44); // 0+2+8+34
    assert(sumEvenFibonacciBelowLimit(35) == 44);
    assert(sumEvenFibonacciBelowLimit(100) == 44);
    assert(sumEvenFibonacciBelowLimit(144) == 188); // 0+2+8+34+144
    assert(sumEvenFibonacciBelowLimit(145) == 188);

    return 0;
}
