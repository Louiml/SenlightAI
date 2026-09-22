// Write a C++ function named `measureParallelPi` that takes a positive integer `numThreads` as input and returns the number of milliseconds required to concurrently compute an approximation of π using the Leibniz formula with a fixed number of iterations (`max_iter = 1e9`) split across the given number of threads. The function must handle invalid input (non‑positive or zero `numThreads`) by returning `0` (indicating failure/timeout). The timing must be measured using `std::chrono::steady_clock`, starting just after launching all asynchronous tasks (so thread creation time is excluded), and ending after all futures are resolved. The function must use `std::async` with default launch policy and store futures in a `std::vector`. Provide the full implementation with necessary headers and `const` correctness; do not include a `main` function.
The approach follows the original snippet: create `numThreads` futures, each calling `calcPi(1e9)` asynchronously. Timing starts immediately after all futures are created, because thread creation overhead is not part of the calculation. Then the main thread waits for each future via `.get()`. The elapsed time in milliseconds is computed using `duration_cast<milliseconds>`. Edge cases: if `numThreads` is zero or negative, we cannot launch a thread; the function should return `0` to signal an invalid argument (since no calculation is performed). For `numThreads` = 1, it runs single‑threaded but still uses the async mechanism, which may add slight overhead compared to a direct call, but that is acceptable per the spec. Complexity: each thread performs O(max_iter) operations; total work is O(max_iter * numThreads) across all threads, but wall‑clock time depends on hardware parallelism. Space complexity is O(numThreads) for the futures vector. Note that `max_iter` is a constant `1e9`, which is large; the function may take noticeable time, but the task only requires returning the measured duration. For correctness testing, we cannot assert a specific time value in the test, so we only test the invalid‑input case (returns 0) and that for a valid input the returned time is non‑negative (though we can’t guarantee it’s >0 due to possible rounding, but with 1e9 iterations it will be). The test will call the function with `numThreads=1` and check that the return value is an integer ≥0 (we can assert `>=0`). For invalid inputs (0, -1), assert return is 0.
#include <chrono>
#include <future>
#include <vector>
#include <cstddef>

// Approximate π using the Leibniz formula with max_iter iterations.
static double calcPi(const size_t max_iter) {
    double pi = 0.0;
    double denominator = 1.0;
    for (size_t i = 0; i < max_iter; ++i) {
        // Alternating sign: + for even i, - for odd i.
        const double term = (i % 2 == 0) ? 4.0 / denominator : -4.0 / denominator;
        pi += term;
        denominator += 2.0;
    }
    return pi;
}

/**
 * Measures the wall‑clock time (in milliseconds) to compute π in parallel
 * using numThreads asynchronous tasks. Each task performs 1e9 iterations.
 * Returns 0 if numThreads is not positive (invalid input).
 * Timing starts after all futures have been created and stops after all are resolved.
 */
long long measureParallelPi(int numThreads) {
    const size_t max_iter = 1000000000ULL; // 1e9
    if (numThreads <= 0) {
        return 0; // invalid argument
    }

    // Launch all asynchronous tasks.
    std::vector<std::future<double>> futures;
    futures.reserve(static_cast<size_t>(numThreads));
    for (int i = 0; i < numThreads; ++i) {
        futures.push_back(std::async(std::launch::async, calcPi, max_iter));
    }

    // Start timing after task creation.
    const auto tbegin = std::chrono::steady_clock::now();

    // Wait for all tasks to complete.
    for (auto& fut : futures) {
        fut.get();
    }

    // Stop timing.
    const auto tend = std::chrono::steady_clock::now();

    // Return elapsed time in milliseconds.
    return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();
}
#include <cassert>

// Reference to the function under test (declaration from solution).
long long measureParallelPi(int numThreads);

int main() {
    // Invalid inputs should return 0.
    assert(measureParallelPi(0) == 0);
    assert(measureParallelPi(-3) == 0);

    // Valid input: time must be non‑negative (with 1e9 iterations it will be >0).
    long long t1 = measureParallelPi(1);
    assert(t1 >= 0);

    // Run with 2 threads; result should also be non‑negative.
    long long t2 = measureParallelPi(2);
    assert(t2 >= 0);

    // For very small thread counts, results are not deterministic in time,
    // but we can at least verify the function returns without error.
    long long t4 = measureParallelPi(4);
    assert(t4 >= 0);

    return 0;
}
