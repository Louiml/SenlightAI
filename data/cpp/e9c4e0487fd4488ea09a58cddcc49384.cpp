/*
Write a C++ function that takes a positive integer `n` as input and returns the sum of all odd integers from 1 up to and including `n` (i.e., 1 + 3 + 5 + ... + the largest odd number ≤ `n`). The function should be named `sumOddNumbers` and must handle the edge case where `n = 0` by returning `0`. You may assume the input is always a non-negative integer. The function should be `const`-correct and not modify its input.
*/

#include <cstdint>

// Returns the sum of all odd integers from 1 up to and including n.
// For n = 0, returns 0. For n > 0, sums all numbers i where i is odd and i <= n.
int sumOddNumbers(int n) {
    int total = 0;
    for (int i = 1; i <= n; i += 2) {
        total += i;
    }
    return total;
}

#include <cassert>

int sumOddNumbers(int);

int main() {
    assert(sumOddNumbers(0) == 0);
    assert(sumOddNumbers(1) == 1);
    assert(sumOddNumbers(2) == 1);
    assert(sumOddNumbers(3) == 4);
    assert(sumOddNumbers(4) == 4);
    assert(sumOddNumbers(5) == 9);
    assert(sumOddNumbers(6) == 9);
    assert(sumOddNumbers(9) == 25);
    assert(sumOddNumbers(10) == 25);
    assert(sumOddNumbers(100) == 2500); // 1+3+5+...+99 = 2500
    return 0;
}

// The solution iterates over all integers from 1 to `n` in steps of 2 (using a loop that starts at 1 and increments by 2 each time). For each such odd number, it adds it to a running total. This avoids checking divisibility by 2 for every integer, making the loop more efficient. Edge cases: if `n = 0`, the loop condition `i <= n` is false from the start, so the sum remains `0`. If `n = 1`, the loop runs once and returns `1`. For any positive `n`, the sum is computed correctly because the largest odd number ≤ `n` is included (e.g., `n = 5` gives 1+3+5=9, `n = 6` also gives 1+3+5=9). Time complexity is O(n) because the loop runs about ⌈n/2⌉ times (specifically, the number of odd numbers from 1 to n). Space complexity is O(1) since only a few integer variables are used. The function is `const`-safe because it only reads its parameter and does not modify it.
