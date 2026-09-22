/*
Write a C++ function that, given a positive integer `n`, simulates the following elimination process: Starting with the numbers `1, 2, ..., n` in a list, remove every other number, first from left to right, then from right to left, alternating direction after each full pass. After each pass, the remaining list shrinks by approximately half. Continue this process until only one number remains. Return that last remaining number. For example, for `n = 9`, the process yields `6`. The function must handle large `n` efficiently without actually building the list. Edge cases include `n = 1` (returns `1`) and very large `n` up to at least `10^9`.
*/

#include <cstdint>

// Returns the last remaining number after repeatedly eliminating every other
// element, alternating direction starting from left to right.
// Precondition: n >= 1.
std::int64_t lastRemainingNumber(std::int64_t n) {
    bool left_to_right = true;
    std::int64_t remaining = n;
    std::int64_t step = 1;
    std::int64_t head = 1;

    while (remaining > 1) {
        // Move head forward if going left-to-right,
        // or if the remaining count is odd when going right-to-left.
        if (left_to_right || (remaining % 2 == 1)) {
            head += step;
        }

        // Halve the remaining count and double the gap between survivors.
        remaining /= 2;
        step *= 2;
        left_to_right = !left_to_right;
    }

    return head;
}

#include <cassert>
#include <cstdint>

// Declare the function under test (declared elsewhere in the solution).
std::int64_t lastRemainingNumber(std::int64_t n);

int main() {
    assert(lastRemainingNumber(1) == 1);
    assert(lastRemainingNumber(2) == 2);
    assert(lastRemainingNumber(3) == 2);
    assert(lastRemainingNumber(4) == 2);
    assert(lastRemainingNumber(5) == 2);
    assert(lastRemainingNumber(6) == 4);
    assert(lastRemainingNumber(7) == 4);
    assert(lastRemainingNumber(8) == 6);
    assert(lastRemainingNumber(9) == 6);
    assert(lastRemainingNumber(10) == 8);
    assert(lastRemainingNumber(100) == 54);
    assert(lastRemainingNumber(1000000000LL) == 542782278LL);
    return 0;
}

// The key observation is that we do not need to simulate the list. At each pass, we track the first element of the current remaining sequence (the "head") and the step size between consecutive remaining numbers. Initially, head = 1, step = 1, and remaining count = n. In each pass, we halve the remaining count (integer division) and double the step. We also toggle direction. The crucial update rule: if we are moving left-to-right, the head always advances by `step`. If moving right-to-left, the head advances only when the current remaining count is odd (because then the first element is removed, so the new head becomes the second element). This rule stems from whether the first element survives the elimination. The process continues until `remaining` becomes 1. Time complexity is O(log n) because we halve the count each iteration. Space complexity is O(1). Edge cases: `n = 1` returns 1 immediately; any odd remaining count when moving right-to-left shifts the head; the step size doubles correctly because after removing every other element, the gap between survivors doubles.
