Write a C++ function named `printArithmeticProgression` that takes three integers `start`, `end`, and `step` as parameters. The function should print all integers from `start` up to and including `end`, incrementing by `step` each time. If `step` is positive, the progression should increase as long as the current value is less than or equal to `end`. If `step` is negative, the progression should decrease as long as the current value is greater than or equal to `end`. If `step` is zero, the function should print nothing. The function should return nothing (void) and produce output to `std::cout` with each number followed by a single space (and no trailing space after the last number). Handle edge cases where `start` may already be beyond `end` given the direction of `step` (e.g., start=5, end=3, step=1) — in such cases, print nothing. The function must not assume any ordering between `start` and `end`.

// The core algorithm is a simple loop that iterates from `start` toward `end` by `step`. Before looping, check if `step` is zero — if so, return immediately. For positive `step`, the loop continues while `current <= end`; for negative `step`, while `current >= end`. In each iteration, print the current value followed by a space. To handle the trailing-space requirement, print the first number without a preceding space, then print a space before each subsequent number, or alternatively collect numbers in a vector and print with spaces between. The edge cases include: (1) `step=0` — no output; (2) when `start` is already beyond `end` for the given direction — loop condition fails immediately, printing nothing; (3) large ranges — ensure no integer overflow by using `long long` for the loop variable if necessary, though typical constraints are small. Time complexity is O(⌈(|end-start|)/|step|⌉ + 1) with a constant space overhead O(1) for the loop variable, plus O(1) for output buffering. The solution uses a `for` loop with an appropriate condition based on the sign of `step`.

#include <iostream>

// Print an arithmetic progression from `start` to `end` inclusive,
// incrementing by `step` each time. Prints nothing if `step` is zero
// or if the direction of `step` does not move from `start` toward `end`.
void printArithmeticProgression(int start, int end, int step) {
    if (step == 0) return;

    if (step > 0) {
        for (int i = start; i <= end; i += step) {
            if (i != start) std::cout << ' ';
            std::cout << i;
        }
    } else {
        for (int i = start; i >= end; i += step) {
            if (i != start) std::cout << ' ';
            std::cout << i;
        }
    }
}

#include <cassert>
#include <sstream>

// Helper to capture output of the function.
std::string capture(int start, int end, int step) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printArithmeticProgression(start, end, step);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    assert(capture(1, 10, 2) == "1 3 5 7 9");
    assert(capture(10, 1, -3) == "10 7 4 1");
    assert(capture(5, 5, 1) == "5");
    assert(capture(5, 3, 1) == "");
    assert(capture(3, 5, -1) == "");
    assert(capture(0, 10, 0) == "");
    assert(capture(-5, 5, 3) == "-5 -2 1 4");
    assert(capture(20, -10, -5) == "20 15 10 5 0 -5 -10");
    return 0;
}
