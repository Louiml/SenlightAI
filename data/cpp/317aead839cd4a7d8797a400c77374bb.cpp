/*
Write a C++ function named `minimum_increase_steps` that takes a vector of 64-bit unsigned integers and returns the minimum number of unit increments needed to make the entire sequence non-decreasing. A sequence is non-decreasing if each element is greater than or equal to the previous one. You can only increase elements (never decrease them), and each unit increase of any element counts as one step. The function should return the total number of such increments. Use `uint64_t` for the input values and the result to handle large numbers, and ensure the vector can be empty (in which case return 0). Do not modify the input vector.
*/
#include <cstdint>
#include <vector>

// Returns the minimum number of unit increments to make the sequence non-decreasing.
uint64_t minimum_increase_steps(const std::vector<uint64_t>& values) {
    if (values.empty()) {
        return 0;
    }

    uint64_t steps = 0;
    uint64_t prev = values[0];

    for (size_t i = 1; i < values.size(); ++i) {
        const uint64_t current = values[i];
        if (current < prev) {
            steps += (prev - current);
        } else {
            prev = current;
        }
    }

    return steps;
}
#include <cassert>
#include <cstdint>
#include <vector>

// Include the solution function here (or link it).

int main() {
    // Empty vector
    assert(minimum_increase_steps({}) == 0);

    // Single element
    assert(minimum_increase_steps({5}) == 0);

    // Already non-decreasing
    assert(minimum_increase_steps({1, 2, 3, 4}) == 0);

    // Simple decrease
    assert(minimum_increase_steps({3, 1}) == 2);  // 1 -> 3

    // Multiple decreases
    assert(minimum_increase_steps({1, 2, 1, 2}) == 1); // 1 -> 2

    // Large drop
    assert(minimum_increase_steps({10, 1, 1, 1}) == 27); // 1+1+1 -> 10 each => 9+9+9

    // All zeros
    assert(minimum_increase_steps({0, 0, 0}) == 0);

    // Mixed with equal values
    assert(minimum_increase_steps({2, 2, 1, 2}) == 1); // 1 -> 2

    // Monotonic decreasing
    assert(minimum_increase_steps({5, 4, 3, 2, 1}) == 10); // 1+2+3+4

    // Large values (64-bit)
    assert(minimum_increase_steps({1000000000000000000ULL, 1}) == 999999999999999999ULL);

    return 0;
}
// The goal is to transform the array into a non-decreasing sequence using only increments. The optimal strategy is to process the array from left to right, maintaining the current required minimum value (which is simply the maximum value seen so far, since we can only increase). For each element, if it is smaller than the previous element, we must raise it to match the previous element — the difference between the previous element and the current element is the number of steps needed. If the current element is already >= previous, we simply update the "previous" to this larger value. This greedy approach works because increasing any element beyond what is necessary would only increase the total steps, and using the largest seen so far as the baseline ensures we don't do extra work. Edge cases: empty vector (return 0), single element (return 0), and cases where the sequence is already non-decreasing (return 0). Also, since values are `uint64_t`, differences are safe as long as we compare correctly (current < previous). The algorithm runs in O(n) time and O(1) extra space.
