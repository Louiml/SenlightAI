Write a C++ function named `sumPositiveValues` that takes a constant reference to a vector of floats and returns the sum of all strictly positive values in the vector. The function must be robust to an empty vector (return 0.0f) and handle negative values, zeros, and floating-point values naturally. Additionally, the function should work with `const` correctness (the input vector must not be modified) and should not use any global variables or macros. The task is to implement only this function, not a full program with `main`; the test harness will call it directly.

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Empty vector returns 0
    assert(sumPositiveValues({}) == 0.0f);

    // Mixed positive, negative, and zero
    assert(sumPositiveValues({1.5f, -2.0f, 3.0f, 0.0f, -0.5f}) == 4.5f);

    // All negatives
    assert(sumPositiveValues({-1.0f, -2.0f, -3.5f}) == 0.0f);

    // All positives
    assert(sumPositiveValues({2.0f, 4.0f, 6.0f}) == 12.0f);

    // Single positive value
    assert(sumPositiveValues({7.25f}) == 7.25f);

    // Single zero
    assert(sumPositiveValues({0.0f}) == 0.0f);

    // Large values
    assert(sumPositiveValues({1000000.0f, 2000000.0f, -500.0f}) == 3000000.0f);

    // Fractional values
    assert(sumPositiveValues({0.1f, 0.2f, 0.3f}) > 0.599f && sumPositiveValues({0.1f, 0.2f, 0.3f}) < 0.601f);

    // Positive and negative cancellation check
    assert(sumPositiveValues({5.0f, -5.0f, 5.0f}) == 10.0f);

    // Const correctness test (vector passed as temporary works)
    const std::vector<float> vec = {1.0f, 2.0f};
    assert(sumPositiveValues(vec) == 3.0f);
}

#include <vector>

// Returns the sum of all strictly positive values in the input vector.
// Returns 0.0f if the vector is empty or contains no positive values.
float sumPositiveValues(const std::vector<float>& values) {
    float sum = 0.0f;
    for (float value : values) {
        if (value > 0.0f) {
            sum += value;
        }
    }
    return sum;
}

// The solution iterates over each element of the input vector using a range-based for loop. For each value, it checks if the value is greater than zero (`value > 0.0f`). If true, that value is added to an accumulator initialized to `0.0f`. Edge cases include an empty vector (loop does not execute, sum returns 0.0f) and vectors containing only zeros or negatives (the condition is never true, sum remains 0.0f). Floating-point precision is handled naturally by using float addition; no special rounding is required. Time complexity is \(O(n)\) where \(n\) is the number of elements in the vector, and space complexity is \(O(1)\) since only a single accumulator and the loop variable are used.
