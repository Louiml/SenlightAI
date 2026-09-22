/*
Write a C++ function named `sumPositiveOddValues` that takes a constant reference to a vector of integers and returns the sum of all elements that are both positive and odd. If the vector is empty or contains no positive odd integers, the function must return 0. The function must not modify the input vector, must use `const` correctness, and should be implemented without using any external libraries beyond the standard ones required for the vector type.
*/
#include <vector>

// Return the sum of all positive odd integers in the input vector.
int sumPositiveOddValues(const std::vector<int>& values) {
    int total = 0;
    for (int value : values) {
        if (value > 0 && value % 2 != 0) {
            total += value;
        }
    }
    return total;
}
#include <cassert>
#include <vector>

// Function declaration (from solution)
int sumPositiveOddValues(const std::vector<int>& values);

int main() {
    // Test with mixed values
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(sumPositiveOddValues(v1) == 9);  // 1+3+5

    // Test with negative and zero
    std::vector<int> v2 = {-1, 0, 3, -5, 7};
    assert(sumPositiveOddValues(v2) == 10); // 3+7

    // Test with no positive odd values
    std::vector<int> v3 = {2, 4, 6, -3, 0};
    assert(sumPositiveOddValues(v3) == 0);

    // Test with empty vector
    std::vector<int> v4;
    assert(sumPositiveOddValues(v4) == 0);

    // Test with all positive odd
    std::vector<int> v5 = {1, 3, 5};
    assert(sumPositiveOddValues(v5) == 9);

    // Test with a single positive odd
    std::vector<int> v6 = {7};
    assert(sumPositiveOddValues(v6) == 7);

    // Test with a single even positive
    std::vector<int> v7 = {8};
    assert(sumPositiveOddValues(v7) == 0);

    // Test with large values
    std::vector<int> v8 = {1001, -1, 2002, 3};
    assert(sumPositiveOddValues(v8) == 1004); // 1001+3

    return 0;
}
// The solution iterates through each element of the input vector using a range-based for loop. For each value `x`, the condition `x > 0` checks whether it is positive, and `x % 2 != 0` (or `x % 2 == 1` for positive values, but `!=0` is safer for negative odd numbers even though they are already filtered out) checks whether it is odd. When both conditions are true, the value is added to a running sum. The algorithm handles edge cases naturally: an empty vector simply yields no iterations and returns 0; negative or zero values are ignored by the positivity check; even positive numbers are ignored by the odd check. Time complexity is O(n) because each element is visited exactly once. Space complexity is O(1) beyond the input vector itself, as only a single integer accumulator is used.
