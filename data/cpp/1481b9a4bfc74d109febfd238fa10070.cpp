// Write a C++ function that takes a vector of `float` values and an integer `iterations` (assumed to be at least 1), computes the sum of all elements in the vector using a simple sequential loop, performs that reduction `iterations` times, and returns the final sum as a `float`. The function must be const-correct (the input vector is not modified), handle an empty vector by returning `0.0f`, and not rely on any external libraries beyond the standard C++ headers. The task is to implement the core computational logic (the reduction) without any GPU, timing, or output formatting concerns.

// The solution is straightforward: initialize an accumulator (e.g., `float result = 0.0f;`) and iterate over the vector, adding each element to the accumulator. This single pass yields the sum in `O(n)` time, where `n` is the size of the input vector. We then repeat this process `iterations` times, so the total time is `O(iterations * n)`, and the space complexity is `O(1)` beyond the input (only a few scalar variables). Edge cases: if the vector is empty, the sum is `0.0f` regardless of `iterations`. If `iterations` is zero (though the task says at least 1), we could also return `0.0f`, but we will still handle it gracefully by returning `0.0f` if either the vector is empty or `iterations <= 0`. Floating-point accumulation is straightforward; no special handling is needed for `NaN` or infinities, but typical expected behavior is that they propagate naturally. We use a simple loop rather than `std::accumulate` to make the process explicit and avoid any potential compiler optimizations that could be unclear to a student.

#include <vector>

// Compute the sum of all elements in the input vector, repeated `iterations` times.
// Returns 0.0f if the vector is empty or if `iterations` is non-positive.
// The input vector is not modified (const reference).
float computeRepeatedSum(const std::vector<float>& data, int iterations) {
    if (data.empty() || iterations <= 0) {
        return 0.0f;
    }

    // Perform the reduction `iterations` times.
    // The sum is recomputed from scratch each time.
    float sum = 0.0f;
    for (int i = 0; i < iterations; ++i) {
        sum = 0.0f; // Reset accumulator for each iteration
        for (const float& value : data) {
            sum += value;
        }
    }
    return sum;
}

#include <cassert>
#include <vector>

// Include the solution function here (or via header)
float computeRepeatedSum(const std::vector<float>& data, int iterations);

int main() {
    std::vector<float> v1 = {1.0f, 2.0f, 3.0f, 4.0f};
    assert(computeRepeatedSum(v1, 1) == 10.0f);
    assert(computeRepeatedSum(v1, 3) == 10.0f); // Sum is same regardless of iterations
    assert(computeRepeatedSum(v1, 5) == 10.0f);

    std::vector<float> v2 = {-1.5f, 2.5f, 0.5f};
    assert(computeRepeatedSum(v2, 2) == 1.5f); // -1.5 + 2.5 + 0.5 = 1.5

    std::vector<float> v3 = {}; // empty
    assert(computeRepeatedSum(v3, 1) == 0.0f);
    assert(computeRepeatedSum(v3, 10) == 0.0f);

    std::vector<float> v4 = {0.0f, 0.0f, 0.0f};
    assert(computeRepeatedSum(v4, 7) == 0.0f);

    std::vector<float> v5 = {5.0f}; // single element
    assert(computeRepeatedSum(v5, 4) == 5.0f);

    // Edge case: iterations = 0 (though not expected, we handle it)
    assert(computeRepeatedSum(v1, 0) == 0.0f);

    // Negative iterations also handled
    assert(computeRepeatedSum(v1, -2) == 0.0f);

    return 0;
}
