// Write a C++ function that benchmarks a matrix-vector multiplication operation by measuring and returning the average execution time (in microseconds) for a given square matrix size. The function should accept an integer `n` representing the matrix dimension, allocate a dynamically sized row-major matrix and vector, fill them with deterministic values (e.g., `matrix[i][j] = (i * n + j) % 7`, `vector[j] = (j % 3) + 1`), perform the multiplication 1000 times, and return the average time per operation. Use `std::chrono::high_resolution_clock` for timing. The function must be named `benchmark_matvec` and must be const-correct where applicable (e.g., input matrix/vector as `const`). The result vector should be computed from the first multiplication to avoid compiler optimizations removing the loop.

#include <cassert>
#include <cmath>

int main() {
    // Test n=0 returns 0.
    assert(benchmark_matvec(0) == 0.0);

    // Test that small sizes return positive time.
    double t1 = benchmark_matvec(1);
    assert(t1 > 0.0);
    assert(std::isfinite(t1));

    double t2 = benchmark_matvec(4);
    assert(t2 > 0.0);
    assert(std::isfinite(t2));

    // Larger matrix should generally take longer than tiny one? Not guaranteed,
    // but at least should be finite and positive.
    double t3 = benchmark_matvec(16);
    assert(t3 > 0.0);
    assert(std::isfinite(t3));

    // Repeat call gives consistent positive result.
    double t4 = benchmark_matvec(4);
    assert(t4 > 0.0);
    assert(std::isfinite(t4));

    // Ensure different dimensions are handled without exception.
    benchmark_matvec(2);
    benchmark_matvec(8);
    benchmark_matvec(32);
    benchmark_matvec(64);

    // All passed.
    assert(true);

    return 0;
}

#include <chrono>
#include <vector>
#include <cstddef>

// Benchmark matrix-vector multiplication for a square matrix of size n x n.
// Returns average execution time per multiplication in microseconds.
double benchmark_matvec(std::size_t n) {
    if (n == 0) {
        return 0.0;
    }

    // Allocate row-major matrix and vectors.
    std::vector<double> matrix(n * n);
    std::vector<double> vec(n);
    std::vector<double> result(n, 0.0);

    // Fill with deterministic values.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            matrix[i * n + j] = static_cast<double>((i * n + j) % 7);
        }
        vec[i] = static_cast<double>((i % 3) + 1);
    }

    // Warm-up: perform one multiplication to ensure caches are populated.
    for (std::size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (std::size_t j = 0; j < n; ++j) {
            sum += matrix[i * n + j] * vec[j];
        }
        result[i] = sum;
    }

    constexpr int ITERATIONS = 1000;
    volatile double sink = 0.0; // Prevent optimization.

    auto start = std::chrono::high_resolution_clock::now();
    for (int iter = 0; iter < ITERATIONS; ++iter) {
        for (std::size_t i = 0; i < n; ++i) {
            double sum = 0.0;
            for (std::size_t j = 0; j < n; ++j) {
                sum += matrix[i * n + j] * vec[j];
            }
            result[i] = sum;
        }
        // Touch result to prevent loop removal.
        for (std::size_t i = 0; i < n; ++i) {
            sink += result[i];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_us = std::chrono::duration<double, std::micro>(end - start).count();
    return total_us / static_cast<double>(ITERATIONS);
}

// The solution follows a straightforward benchmarking approach. First, allocate a 1D `std::vector<double>` of size `n*n` to represent a row-major matrix, and another vector of size `n` for the input, plus a result vector of size `n`. Fill the matrix and vector with deterministic values to ensure reproducible timings. Then, perform 1000 iterations of matrix-vector multiplication using the classic triple-loop: for each row `i`, compute the dot product of row `i` with the vector and store it in `result[i]` (overwriting each time). To prevent the compiler from optimizing away the loop (since results are unused), store the final result vector and maybe accumulate its sum into an external variable (e.g., `volatile double sink`). Use `std::chrono::high_resolution_clock::now()` before and after the loop, then compute the average time per multiplication as `total_duration / 1000.0`. Edge cases: `n = 0` should return 0 (no work). Complexity: The multiplication is O(n^2) per iteration, so O(1000 * n^2) overall; memory usage is O(n^2) for the matrix.
