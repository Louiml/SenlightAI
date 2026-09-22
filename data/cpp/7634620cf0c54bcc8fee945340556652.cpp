/*
Write a C++ function named `differenceOfSums` that takes two positive integers `n` and `m` where `1 ≤ n, m ≤ 1000`. For every integer from 1 to `n` inclusive, if the integer is divisible by `m`, subtract it from a running total; otherwise, add it to the total. Return the final result as an integer. The function must be declared `const`-correct (it has no side effects) and should not rely on any global state. The signature is: `int differenceOfSums(int n, int m);`. Ensure the function works for edge cases such as `m = 1` (all numbers divisible, result is negative sum) and `n = 1` (only number 1, result is 1 unless divisible by `m`).
*/
#include <cstddef> // For size_t, though not strictly required

// Computes (sum of numbers 1..n not divisible by m) - (sum of numbers 1..n divisible by m).
int differenceOfSums(int n, int m) {
    int result = 0;
    for (int i = 1; i <= n; ++i) {
        if (i % m == 0) {
            result -= i;
        } else {
            result += i;
        }
    }
    return result;
}
int main() {
    // Example from problem: n=10, m=3 -> non-divisible (1+2+4+5+7+8+10)=37 minus divisible (3+6+9)=18 => 19
    assert(differenceOfSums(10, 3) == 19);
    // n=5, m=6: no number divisible by 6, sum = 1+2+3+4+5=15
    assert(differenceOfSums(5, 6) == 15);
    // m=1: all numbers divisible, result = -(1+2+3) = -6
    assert(differenceOfSums(3, 1) == -6);
    // n=1, m=2: 1 is not divisible, result = 1
    assert(differenceOfSums(1, 2) == 1);
    // n=4, m=2: non-divisible (1+3)=4 minus divisible (2+4)=6 => -2
    assert(differenceOfSums(4, 2) == -2);
    // n=1, m=1: 1 is divisible, result = -1
    assert(differenceOfSums(1, 1) == -1);
}
// The algorithm is straightforward: initialize a result variable to 0, then iterate `i` from 1 to `n`. For each `i`, check if `i % m == 0`; if true, subtract `i` from result; otherwise add `i` to result. This accumulates the difference between non-divisible and divisible sums. Edge cases: when `m = 1`, every `i` is divisible, so result is `-(1 + 2 + ... + n)` = `-n*(n+1)/2`. When `n < m`, no number is divisible (for positive `i`), so result is `n*(n+1)/2`. The time complexity is O(n) because we loop exactly `n` times, and space complexity is O(1) since we only use a few integer variables.
