// Write a C++ function `mostFrequentNumber` that takes an integer `n` and a `std::vector<int>` of length `n`, where each element is an integer between 1 and 5 inclusive. The function should return the smallest integer (from 1 to 5) that appears most frequently in the vector. If multiple numbers have the same maximum frequency, return the smallest one. You may assume the vector is non-empty and contains only values 1 through 5. Your solution must avoid using any external libraries beyond the standard headers.

The solution uses a fixed-size frequency array `count[6]` (indices 0–5, but index 0 is unused, indices 1–5 map to the possible values). First, iterate through the input vector and increment `count[value]` for each element. Then, scan indices 1 to 5 to find the maximum frequency. Since we iterate from 1 upward and only update the maximum when a strictly greater frequency is found, the first number to reach the maximum will be the smallest value with that frequency. This naturally handles ties by returning the smallest index. Edge cases include vectors with all identical values (the function returns that value) and vectors with uniform distribution (returns 1). Time complexity is O(n) for the counting pass plus O(1) for the fixed-size scan, so total O(n). Space complexity is O(1) because we only use a fixed array of 6 integers regardless of input size.

#include <vector>

// Return the smallest value (1-5) that appears most frequently in the input.
// The input is guaranteed to be non-empty and contain only integers from 1 to 5.
int mostFrequentNumber(const std::vector<int>& values) {
    // Frequency array: indices 1..5 correspond to values 1..5; index 0 is unused.
    int count[6] = {0};
    for (int v : values) {
        ++count[v];
    }

    int maxFrequency = 0;
    int result = 1;
    // Scan from 1 to 5; since we use strict '>', the first maximal is kept,
    // which ensures the smallest value with that frequency is returned.
    for (int i = 1; i <= 5; ++i) {
        if (count[i] > maxFrequency) {
            maxFrequency = count[i];
            result = i;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Function declaration (provided for clarity; in practice, include the solution header)
int mostFrequentNumber(const std::vector<int>& values);

int main() {
    // Single element
    assert(mostFrequentNumber({3}) == 3);

    // All distinct values, equal frequency → smallest is 1
    assert(mostFrequentNumber({1,2,3,4,5}) == 1);

    // Clear majority
    assert(mostFrequentNumber({2,2,3,3,3,5,5}) == 3);

    // Tie between 2 and 5, both appear twice → return 2
    assert(mostFrequentNumber({2,5,2,5,1}) == 2);

    // All same value
    assert(mostFrequentNumber({4,4,4,4}) == 4);

    // Mixed with tie among 1,3,4 each appearing once
    assert(mostFrequentNumber({1,3,4,2}) == 1);

    // Larger vector, maximum is 5 with 3 occurrences
    assert(mostFrequentNumber({5,1,5,2,5,3,4}) == 5);

    // Tie among 2 and 4, each twice, but 1 appears once
    assert(mostFrequentNumber({2,4,2,4,1}) == 2);

    return 0;
}
