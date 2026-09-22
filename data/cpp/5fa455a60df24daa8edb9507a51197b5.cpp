Write a C++ function named `sumOfOddNumbersInRange` that takes two integers `start` and `end` (inclusive) and returns the sum of all odd numbers between them. The function must handle cases where `start` is greater than `end` by returning 0, and it must correctly process negative integers and ranges that include zero. The task is standalone and does not require any input/output; the function will be called directly with test cases.
#include <cassert>

int main() {
    // Basic positive range
    assert(sumOfOddNumbersInRange(1, 10) == 25);  // 1+3+5+7+9 = 25
    // Single odd number
    assert(sumOfOddNumbersInRange(7, 7) == 7);
    // Single even number
    assert(sumOfOddNumbersInRange(8, 8) == 0);
    // Reversed range (start > end) returns 0
    assert(sumOfOddNumbersInRange(10, 1) == 0);
    // Negative range
    assert(sumOfOddNumbersInRange(-5, 5) == 0);  // -5 + -3 + -1 + 1 + 3 + 5 = 0
    // Range including zero
    assert(sumOfOddNumbersInRange(-2, 2) == 0);  // -1 + 1 = 0
    // Large positive range
    assert(sumOfOddNumbersInRange(100, 200) == 7500);  // sum of odds 101..199 (50 numbers, avg 150)
    // Zero start with positive end
    assert(sumOfOddNumbersInRange(0, 3) == 4);  // 1 + 3 = 4
    // Negative range with only odds
    assert(sumOfOddNumbersInRange(-7, -3) == -15);  // -7 + -5 + -3 = -15
    // All even numbers
    assert(sumOfOddNumbersInRange(2, 6) == 0);  // 2,4,6 none odd
    return 0;
}
#include <cstddef>

// Returns the sum of all odd integers in the inclusive range [start, end].
// If start > end, returns 0.
int sumOfOddNumbersInRange(int start, int end) {
    int sum = 0;
    // Iterate only if start <= end; otherwise, loop body never executes.
    for (int number = start; number <= end; ++number) {
        if (number % 2 != 0) {  // Odd numbers have remainder 1 or -1 when divided by 2.
            sum += number;
        }
    }
    return sum;
}
// The solution iterates from `start` to `end` inclusive, checking each number for oddness using the modulo operator (`number % 2 != 0`). For each odd number, it adds it to a running total. Edge cases: if `start > end`, the loop condition `start <= end` fails immediately, so the sum remains 0. Negative odd numbers are handled correctly because modulo with negative numbers in C++ yields a negative remainder (e.g., `-3 % 2 == -1`), which is not zero, so the condition `number % 2 != 0` correctly identifies them as odd. Zero is even, so it is excluded. Time complexity is O(n) where n = |end - start| + 1 (or 0 if reversed), and space complexity is O(1) since only a single integer accumulator is used.
