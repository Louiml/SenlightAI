/*
Write a C++ function that, given a positive integer `limit`, returns the count of positive multiples of 5 that are less than or equal to `limit` and are divisible by 7. The function should be iterative (not using recursion) and should stop processing as soon as it finds such a number, but count all such numbers up to the limit. For example, with `limit = 100`, the multiples of 5 that are also multiples of 7 (i.e., multiples of 35) are 35, 70, and 105, but 105 exceeds the limit, so the count is 2. The function must handle edge cases like very small limits (where no such number exists) and large limits efficiently without an infinite loop. The function signature should be `int countMultiplesOf5And7(int limit)`. You may assume `limit >= 0`. The function should be `const`-correct (no modification of input parameters), and it should use a loop that increments by 5 (simulating the multiples of 5) but break or continue appropriately when a condition fails.
*/

#include <cstddef>

// Count positive multiples of 5 that are less than or equal to limit and also divisible by 7.
// The loop increments by 5 and checks divisibility by 7, stopping when i exceeds limit.
int countMultiplesOf5And7(int limit) {
    int count = 0;
    for (int i = 5; i <= limit; i += 5) {
        if (i % 7 == 0) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

int main() {
    // No multiples up to 4.
    assert(countMultiplesOf5And7(0) == 0);
    assert(countMultiplesOf5And7(4) == 0);
    // First multiple is 35.
    assert(countMultiplesOf5And7(34) == 0);
    assert(countMultiplesOf5And7(35) == 1);
    // Next multiple is 70.
    assert(countMultiplesOf5And7(69) == 1);
    assert(countMultiplesOf5And7(70) == 2);
    // Check larger range.
    assert(countMultiplesOf5And7(100) == 2);
    assert(countMultiplesOf5And7(104) == 2);
    assert(countMultiplesOf5And7(105) == 3);
    // Large limit (just a check for correctness, not performance).
    assert(countMultiplesOf5And7(1000000) == 28571); // 1000000 / 35 = 28571 (floor)
    return 0;
}

// The simplest correct approach is to iterate over all multiples of 5 from 5 up to `limit`, checking each for divisibility by 7. Since 5 and 7 are coprime, a number is divisible by both 5 and 7 if and only if it is divisible by 35. We can iterate by 5 and check `i % 7 == 0`; if true, increment the counter. Continue until `i > limit`. This runs in O(limit/5) time, which is linear in the input magnitude, and O(1) space. For very large limits (e.g., up to 2^31-1), this could be slow; a more efficient approach is to count multiples of 35 directly: `count = limit / 35`. This uses integer division and runs in O(1) time and O(1) space. The problem statement asks for an iterative loop, so we must implement the loop version, but we can mention the direct formula in analysis. Edge cases: if `limit` is 0 or less than 5, the count is 0. Ensure no infinite loop by checking the loop condition correctly. For `limit` exactly a multiple of 35, the count includes that number. For example, `limit=35` gives count 1, `limit=34` gives 0.
