Write a C++ function that takes a non-negative integer `num` and returns the number of steps to reduce it to zero using the rule: if the number is even, divide it by 2; if it is odd, subtract 1. Each operation counts as one step. The function must handle the edge case where `num` is zero, and must work efficiently for large inputs (up to the maximum value of a 32-bit signed integer). The function should be named `numberOfSteps` and should be declared as a free function (not a class method) with a descriptive signature.

#include <cassert>

int main() {
    // Basic cases
    assert(numberOfSteps(0) == 0);
    assert(numberOfSteps(1) == 1);      // 1 → 0
    assert(numberOfSteps(2) == 2);      // 2 → 1 → 0
    assert(numberOfSteps(3) == 3);      // 3 → 2 → 1 → 0
    assert(numberOfSteps(4) == 3);      // 4 → 2 → 1 → 0
    assert(numberOfSteps(5) == 4);      // 5 → 4 → 2 → 1 → 0
    assert(numberOfSteps(6) == 4);      // 6 → 3 → 2 → 1 → 0
    assert(numberOfSteps(7) == 5);      // 7 → 6 → 3 → 2 → 1 → 0
    assert(numberOfSteps(8) == 4);      // 8 → 4 → 2 → 1 → 0
    assert(numberOfSteps(14) == 6);     // 14 → 7 → 6 → 3 → 2 → 1 → 0
    assert(numberOfSteps(123) == 12);   // known result
    assert(numberOfSteps(2147483647) == 46); // max 32-bit int
}

#include <cstdint>

// Count steps to reduce a non-negative integer to zero.
// Rule: even → divide by 2; odd → subtract 1; each operation counts as one step.
int numberOfSteps(int num) {
    int steps = 0;
    while (num > 0) {
        if (num % 2 == 0) {
            num /= 2;
        } else {
            num -= 1;
        }
        ++steps;
    }
    return steps;
}

// The solution simulates the described process directly. Starting from `num`, a loop continues while `num > 0`. Inside each iteration, if `num % 2 == 0`, we divide by 2; otherwise, we subtract 1. Each iteration increments a step counter. The loop terminates when `num` becomes 0. The edge case `num == 0` is naturally handled because the loop condition fails immediately, returning 0 steps (since zero requires no operations). The algorithm runs in \(O(\log n)\) time in the worst case, because each even operation halves the number and each odd operation reduces it by one (which then becomes even, so at most two operations per bit of the number). Space complexity is \(O(1)\) as only a few integer variables are used.
