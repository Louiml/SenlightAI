// Write a C++ function named `parallelMovingAverage` that takes a reference to a constant vector of integers, a chunk size, and a number of threads, and returns a vector of doubles containing the moving average of each interior element (indices 1 through size-2) computed as the average of the element and its immediate left and right neighbors. The function must parallelize the computation using OpenMP with the specified number of threads and a static schedule of the given chunk size. The input vector is guaranteed to have at least 3 elements. The returned vector must have exactly size-2 elements, where the element at position `i-1` in the result corresponds to the average of `input[i-1]`, `input[i]`, and `input[i+1]` from the original input. The function must be correct regardless of thread count, chunk size, or input values, and must not modify the input. Use `default(none)` in the OpenMP pragma and explicitly declare all variables as shared or private as appropriate.
// The solution reads all elements from the input vector into an OpenMP parallel `for` loop, iterating over indices `1` to `size-2` (inclusive). For each index `i`, it computes `(input[i-1] + input[i] + input[i+1]) / 3.0` and stores the result at `output[i-1]` (since the output starts from the first interior index). The loop is parallelized with the `#pragma omp parallel for` directive, specifying the number of threads, `default(none)`, all necessary shared variables (`input`, `output`, `chunk_size`), and `schedule(static, chunk_size)`. Because each iteration writes to a distinct output index and reads only from the input, there are no race conditions. Edge cases: if the input size is exactly 3, the loop runs only once, producing a single-element output. The function must handle very large inputs efficiently, and the time complexity is `O(n)` where `n` is the number of interior elements (approximately `input.size()`), with `O(1)` extra space beyond the output vector. The parallelization does not change the semantics; it only speeds up execution. Correctness is ensured by checking that the output values match a sequential computation within a small floating-point tolerance (or exact match in the test, since integer arithmetic is exact when divided by 3.0 and stored as double).
#include <vector>
#include <cstddef>

// Compute moving averages of interior elements using OpenMP static scheduling.
std::vector<double> parallelMovingAverage(
    const std::vector<int>& input,
    int chunk_size,
    int num_threads
) {
    std::size_t n = input.size();
    std::vector<double> output(n - 2);
    if (n < 3) return output;

    #pragma omp parallel for num_threads(num_threads) default(none) shared(input, output, chunk_size) schedule(static, chunk_size)
    for (std::size_t i = 1; i < n - 1; ++i) {
        output[i - 1] = (input[i - 1] + input[i] + input[i + 1]) / 3.0;
    }
    return output;
}
#include <cassert>
#include <vector>

// The function is assumed to be declared above (or in the same translation unit).
// In a real test, include the solution header or copy the function.

int main() {
    // Basic case
    std::vector<int> a = {1, 2, 3, 4, 5};
    auto result = parallelMovingAverage(a, 2, 4);
    assert(result.size() == 3);
    assert(result[0] == 2.0);   // (1+2+3)/3
    assert(result[1] == 3.0);   // (2+3+4)/3
    assert(result[2] == 4.0);   // (3+4+5)/3

    // Minimal size (3 elements)
    std::vector<int> b = {10, 20, 30};
    auto result_b = parallelMovingAverage(b, 1, 8);
    assert(result_b.size() == 1);
    assert(result_b[0] == 20.0); // (10+20+30)/3

    // Negative numbers
    std::vector<int> c = {-5, 0, 5, 10};
    auto result_c = parallelMovingAverage(c, 1, 4);
    assert(result_c.size() == 2);
    assert(result_c[0] == 0.0);  // (-5+0+5)/3
    assert(result_c[1] == 5.0);  // (0+5+10)/3

    // Large vector with known pattern: each interior average equals the center value if it's constant
    std::vector<int> d(1000, 7);
    auto result_d = parallelMovingAverage(d, 100, 8);
    assert(result_d.size() == 998);
    for (size_t i = 0; i < result_d.size(); ++i) {
        assert(result_d[i] == 7.0);
    }

    // Different chunk sizes and thread counts
    std::vector<int> e = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    auto result_e = parallelMovingAverage(e, 3, 3);
    // Computed sequential expected values:
    // i=1: (1+1+1)/3=1
    // i=2: (1+1+2)/3=4/3
    // i=3: (1+2+2)/3=5/3
    // i=4: (2+2+2)/3=2
    // i=5: (2+2+3)/3=7/3
    // i=6: (2+3+3)/3=8/3
    // i=7: (3+3+3)/3=3
    assert(result_e.size() == 7);
    assert(result_e[0] == 1.0);
    assert(result_e[1] == 4.0 / 3.0);
    assert(result_e[2] == 5.0 / 3.0);
    assert(result_e[3] == 2.0);
    assert(result_e[4] == 7.0 / 3.0);
    assert(result_e[5] == 8.0 / 3.0);
    assert(result_e[6] == 3.0);

    // Test with chunk size larger than loop count
    std::vector<int> f = {1, 2, 3};
    auto result_f = parallelMovingAverage(f, 100, 2);
    assert(result_f.size() == 1);
    assert(result_f[0] == 2.0);

    return 0;
}
