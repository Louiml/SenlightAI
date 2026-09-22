// Write a C++ function that computes the first 18 numbers of a Fibonacci-like sequence where all initial values are 1, each subsequent term is the sum of the two preceding terms, and then returns the sum of the terms at positions 10 through 19 (where position 0 is the first term). The function must accept an integer parameter `n` and return the sum of the first `n` terms of this sequence from index 10 to index 10+n-1, but if `n` is less than 1 or greater than 10, the function should return 0 (as an invalid request). The sequence is defined as: `f(0)=1, f(1)=1`, and for `i >= 2`, `f(i)=f(i-1)+f(i-2)`. The task is to implement the computation directly without using any external libraries (only standard headers) and without relying on precomputed tables.

#include <cassert>

int main() {
    // n=1: sum of f(10) only. f(10) = 89 (sequence: 1,1,2,3,5,8,13,21,34,55,89,...)
    assert(sumFibFrom10(1) == 89);
    // n=2: f(10)+f(11) = 89 + 144 = 233
    assert(sumFibFrom10(2) == 233);
    // n=3: + f(12)=233 → 89+144+233 = 466
    assert(sumFibFrom10(3) == 466);
    // n=10: sum f(10)..f(19). Compute: 89+144+233+377+610+987+1597+2584+4181+6765 = 17567
    assert(sumFibFrom10(10) == 17567);
    // n=0 invalid
    assert(sumFibFrom10(0) == 0);
    // n=11 invalid
    assert(sumFibFrom10(11) == 0);
    // n negative invalid
    assert(sumFibFrom10(-5) == 0);
    // Check n=4: 89+144+233+377 = 843
    assert(sumFibFrom10(4) == 843);
    return 0;
}

#include <cstdint>

// Compute the sum of terms from index 10 to index 10+n-1 in the sequence
// f(0)=1, f(1)=1, f(i)=f(i-1)+f(i-2). Return 0 if n is not in [1,10].
int64_t sumFibFrom10(int n) {
    if (n < 1 || n > 10) {
        return 0;
    }

    int64_t prev2 = 1; // f(0)
    int64_t prev1 = 1; // f(1)
    int64_t current = 0;
    int64_t sum = 0;

    // Precompute up to index 10 first
    if (10 == 0) {
        sum += prev2;
    } else if (10 == 1) {
        sum += prev1;
    } else {
        current = prev2 + prev1;
        for (int i = 2; i <= 10; ++i) {
            current = prev2 + prev1;
            if (i == 10) {
                sum += current;
                break;
            }
            prev2 = prev1;
            prev1 = current;
        }
    }

    // If n==1, we already added f(10), return
    if (n == 1) return sum;

    // Continue computing and summing terms until index 10+n-1
    prev2 = (10 >= 2) ? prev1 : prev2; // ensure prev2 and prev1 are correctly positioned after index 10
    // After the loop above, prev2 = f(9), prev1 = f(10) at the point we broke.
    // But careful: we broke without updating prev2/prev1 after computing current.
    // Let's recompute properly in a unified way:
    // Simpler: recompute from scratch with explicit loop from 0 to 10+n-1.
    prev2 = 1;
    prev1 = 1;
    sum = 0;
    int64_t value = 1;
    for (int i = 0; i <= 10 + n - 1; ++i) {
        if (i == 0) value = 1;
        else if (i == 1) value = 1;
        else value = prev2 + prev1;
        if (i >= 10) sum += value;
        prev2 = (i >= 1) ? prev1 : prev2;
        prev1 = value;
    }
    return sum;
}

// The sequence is a standard Fibonacci-like recurrence with base case `f(0)=1, f(1)=1`. We need to compute terms from index 10 to `10+n-1` inclusive and sum them. The naive approach is to iterate from `i=0` up to `10+n-1`, maintaining only the last two values to compute each next term in O(1) time per term, accumulating the sum only when `i` is in the desired range. Edge cases: if `n` is invalid (less than 1 or greater than 10), return 0. Also note that the values grow quickly; for small `n` up to 10, the largest term is about `f(19)`, which is roughly 6,765 — well within 32-bit integer range. However, to be safe, use `int64_t` for the sum and term values. The time complexity is O(n+10) = O(n), and space complexity is O(1) since we only store a few variables.
