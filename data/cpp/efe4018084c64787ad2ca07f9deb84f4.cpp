/*
Write a standalone C++ function named `cumulativeMultiply` that takes a reference to a `std::vector<double>` and returns a new `std::vector<double>` where each element at index `i` is the product of all elements from the original vector from index `0` through index `i` inclusive (i.e., a prefix product, also known as cumulative product). If the input vector is empty, return an empty vector. For example, given input `{2, 3, 4}`, the output should be `{2, 6, 24}`. The function should handle floating-point values naturally without any special cases for overflow or underflow, and it must not modify the input vector.
*/
#include <vector>

// Returns a vector where output[i] = product of input[0] through input[i] inclusive.
std::vector<double> cumulativeMultiply(const std::vector<double>& input) {
    std::vector<double> result;
    result.reserve(input.size());

    double accumulator = 1.0;
    for (double value : input) {
        accumulator *= value;
        result.push_back(accumulator);
    }

    return result;
}
#include <cassert>
#include <vector>

std::vector<double> cumulativeMultiply(const std::vector<double>& input);

int main() {
    // Basic case
    std::vector<double> input1 = {2.0, 3.0, 4.0};
    std::vector<double> expected1 = {2.0, 6.0, 24.0};
    assert(cumulativeMultiply(input1) == expected1);

    // Single element
    std::vector<double> input2 = {5.0};
    std::vector<double> expected2 = {5.0};
    assert(cumulativeMultiply(input2) == expected2);

    // Empty input
    std::vector<double> input3 = {};
    std::vector<double> expected3 = {};
    assert(cumulativeMultiply(input3) == expected3);

    // Contains zero
    std::vector<double> input4 = {1.0, 0.0, 3.0};
    std::vector<double> expected4 = {1.0, 0.0, 0.0};
    assert(cumulativeMultiply(input4) == expected4);

    // Negative values
    std::vector<double> input5 = {-1.0, 2.0, -3.0};
    std::vector<double> expected5 = {-1.0, -2.0, 6.0};
    assert(cumulativeMultiply(input5) == expected5);

    // Floating-point values
    std::vector<double> input6 = {0.5, 2.0, 4.0};
    std::vector<double> expected6 = {0.5, 1.0, 4.0};
    assert(cumulativeMultiply(input6) == expected6);

    return 0;
}
// The solution involves iterating over the input vector in order, maintaining a running `double` accumulator initialized to `1.0`. For each element, multiply the accumulator by the current element and store the result in the output vector at the same index. This is a linear scan that processes each element exactly once. The problem is simple and has no special edge cases beyond the empty input, for which an empty output is returned. The time complexity is `O(n)` where `n` is the number of elements, and the space complexity is `O(n)` for the output vector (plus `O(1)` auxiliary space for the accumulator). Since the operation is straightforward, no overflow or underflow handling is required per the specification.
