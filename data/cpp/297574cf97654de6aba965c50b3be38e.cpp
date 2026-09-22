// Write a C++ function `sortDescending3(int& a, int& b, int& c)` that takes three integer references, sorts them in strictly non-increasing (descending) order using a helper function that swaps two values if they are out of order, and modifies the original variables so that after the call `a >= b >= c`. The function must not return anything and must work for any integers including negatives, zeros, duplicates, and extreme values (like `INT_MAX`/`INT_MIN`). Use only pass-by-reference and simple comparison/swapping logic — do not use arrays, vectors, or `std::sort`. Ensure the solution is self-contained and does not rely on a `main` function in the submitted solution code.

#include <cassert>
#include <climits>

// declare the function (assume it is defined elsewhere as in the solution)
void sortDescending3(int& a, int& b, int& c);

int main() {
    // Basic descending sort
    int a = 3, b = 7, c = 9;
    sortDescending3(a, b, c);
    assert(a == 9 && b == 7 && c == 3);

    // Already descending
    a = 5; b = 2; c = 1;
    sortDescending3(a, b, c);
    assert(a == 5 && b == 2 && c == 1);

    // All equal
    a = 4; b = 4; c = 4;
    sortDescending3(a, b, c);
    assert(a == 4 && b == 4 && c == 4);

    // Negative numbers
    a = -1; b = -5; c = -3;
    sortDescending3(a, b, c);
    assert(a == -1 && b == -3 && c == -5);

    // Mixed positive/negative/zero
    a = 0; b = -10; c = 5;
    sortDescending3(a, b, c);
    assert(a == 5 && b == 0 && c == -10);

    // Duplicates
    a = 8; b = 8; c = 3;
    sortDescending3(a, b, c);
    assert(a == 8 && b == 8 && c == 3);

    // Extreme values (no overflow)
    a = INT_MAX; b = INT_MIN; c = 0;
    sortDescending3(a, b, c);
    assert(a == INT_MAX && b == 0 && c == INT_MIN);

    // Reverse order
    a = 1; b = 2; c = 3;
    sortDescending3(a, b, c);
    assert(a == 3 && b == 2 && c == 1);

    // Two equal max values
    a = 10; b = 10; c = -2;
    sortDescending3(a, b, c);
    assert(a == 10 && b == 10 && c == -2);

    // All negative, one zero
    a = -5; b = 0; c = -5;
    sortDescending3(a, b, c);
    assert(a == 0 && b == -5 && c == -5);

    return 0;
}

#include  <utility>   // for std::swap (optional, we can implement manually)

// Helper: ensure x is not smaller than y. If x < y, swap them.
void orderDesc(int& x, int& y) {
    if (x < y) {
        // Manual swap (or use std::swap if allowed, but we'll do it manually)
        int tmp = x;
        x = y;
        y = tmp;
    }
}

// Sort three integers in descending order: a >= b >= c.
void sortDescending3(int& a, int& b, int& c) {
    orderDesc(a, b);  // now a >= b
    orderDesc(a, c);  // now a is the largest of all three
    orderDesc(b, c);  // now b >= c and whole set is sorted
}

// The algorithm uses a classic “bubble sort” style exchange on exactly three elements. First, write a helper `orderDesc(int& x, int& y)` that swaps `x` and `y` if `x < y`, ensuring `x` holds the larger value after the call. Then, call this helper three times to guarantee the final order: first compare `a` and `b` so `a` gets the larger of the first two; then compare `a` and `c` so `a` becomes the overall maximum; finally compare `b` and `c` so `b` becomes the middle value and `c` the smallest. After these three comparisons, all possible permutations are correctly sorted. Edge cases: duplicate values are handled naturally (swap condition `x < y` is strict, so equal values remain in place), negative numbers work the same as positives because comparisons are based on value, and extreme values cause no overflow because we only compare and swap, never perform arithmetic. Time complexity is O(1) constant time (exactly three swaps at most), and space complexity is O(1) auxiliary (only a temporary variable inside each swap).
