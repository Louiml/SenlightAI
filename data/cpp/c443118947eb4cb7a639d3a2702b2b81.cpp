Write a C++ function `vectorAddWithTiming` that takes two `std::vector<float>` inputs of equal non-zero size, performs element-wise addition into a new `std::vector<float>` using a `#pragma omp simd` loop (as shown in the snippet, but adapted to a function), and returns the resulting vector. The function must also record and return the execution time of the addition loop (in seconds as a `double`) via an output parameter. The function should handle empty vectors by returning an empty vector and setting time to 0.0. Ensure the function is `const`-correct: the input vectors are passed by const reference, and no mutation occurs except through the output parameter. Focus on correctness, clear code, and proper use of `std::chrono` for timing.

The main algorithm is straightforward: first check if the input vectors are empty or have mismatched sizes; if so, return an empty vector and set time to 0.0. Otherwise, allocate a result vector of the same size, record the start time, execute a `#pragma omp simd` loop over `i` from 0 to `n` (where `n = size`) computing `result[i] = x[i] + y[i]`, then record the finish time and compute the duration in seconds using `std::chrono::duration_cast<std::chrono::duration<double>>`. Edge cases include empty inputs (handled by early return), mismatched sizes (also treat as error case and return empty), and very large vectors where the loop might be heavy—but that's fine. The use of `#pragma omp simd` is optional for compilation since it is ignored by non-OpenMP compilers; thus the code remains portable. Time complexity is O(n) for the addition loop; space complexity is O(n) for the result vector (plus constant overhead). The timing uses `std::chrono::high_resolution_clock` which is the most precise available.

#include <vector>
#include <chrono>

// Performs element-wise addition of two float vectors, returns result,
// and writes the execution time of the addition loop (in seconds) to `timeSeconds`.
// If inputs are empty or sizes mismatch, returns empty vector and sets time to 0.0.
std::vector<float> vectorAddWithTiming(const std::vector<float>& x,
                                       const std::vector<float>& y,
                                       double& timeSeconds)
{
    // Handle empty or mismatched sizes
    if (x.empty() || y.empty() || x.size() != y.size()) {
        timeSeconds = 0.0;
        return {};
    }

    const std::size_t n = x.size();
    std::vector<float> result(n, 0.0f);

    auto start = std::chrono::high_resolution_clock::now();
#pragma omp simd
    for (std::size_t i = 0; i < n; ++i) {
        result[i] = x[i] + y[i];
    }
    auto finish = std::chrono::high_resolution_clock::now();

    timeSeconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    return result;
}

#include <cassert>
#include <vector>

// Declare the function from the solution (assumed to be in the same translation unit)
std::vector<float> vectorAddWithTiming(const std::vector<float>& x,
                                       const std::vector<float>& y,
                                       double& timeSeconds);

int main() {
    // Test 1: Normal addition
    std::vector<float> x1 = {1.0f, 2.0f, 3.0f};
    std::vector<float> y1 = {0.5f, 1.0f, 1.5f};
    double t1 = -1.0;
    auto r1 = vectorAddWithTiming(x1, y1, t1);
    assert(r1.size() == 3);
    assert(r1[0] == 1.5f);
    assert(r1[1] == 3.0f);
    assert(r1[2] == 4.5f);
    assert(t1 >= 0.0);

    // Test 2: Empty inputs
    std::vector<float> x2, y2;
    double t2 = -1.0;
    auto r2 = vectorAddWithTiming(x2, y2, t2);
    assert(r2.empty());
    assert(t2 == 0.0);

    // Test 3: Mismatched sizes
    std::vector<float> x3 = {1.0f};
    std::vector<float> y3 = {1.0f, 2.0f};
    double t3 = -1.0;
    auto r3 = vectorAddWithTiming(x3, y3, t3);
    assert(r3.empty());
    assert(t3 == 0.0);

    // Test 4: Single element
    std::vector<float> x4 = {10.0f};
    std::vector<float> y4 = {-2.0f};
    double t4 = -1.0;
    auto r4 = vectorAddWithTiming(x4, y4, t4);
    assert(r4.size() == 1);
    assert(r4[0] == 8.0f);
    assert(t4 >= 0.0);

    // Test 5: Larger vector (e.g., 1000 elements)
    std::vector<float> x5(1000, 1.0f);
    std::vector<float> y5(1000, 2.0f);
    double t5 = -1.0;
    auto r5 = vectorAddWithTiming(x5, y5, t5);
    assert(r5.size() == 1000);
    for (std::size_t i = 0; i < 1000; ++i) {
        assert(r5[i] == 3.0f);
    }
    assert(t5 >= 0.0);

    return 0;
}
