// Write a C++ function `double measureOpenMPMaxReduction(int threads_num, int count, const std::vector<int>& array)` that measures the average wall-clock time in seconds required to find the maximum element of the given array using an OpenMP parallel `for` loop with a `reduction(max: ...)` clause. The function must run 10 timing repetitions and return the arithmetic mean of the elapsed times. It must handle any positive thread count and any non-empty array; you may assume OpenMP is enabled and `count` is the vector's size. Edge cases include arrays with all negative values or large positive values, and thread counts exceeding available hardware — the function should still work correctly.

#include <cassert>
#include <vector>
#include <omp.h>

// Declaration of the tested function (included from solution)
double measureOpenMPMaxReduction(int threads_num, int count, const std::vector<int>& array);

int main() {
    // Test 1: single thread, small array with negative numbers
    std::vector<int> arr1 = {-5, -2, -9, -1};
    double t1 = measureOpenMPMaxReduction(1, arr1.size(), arr1);
    assert(t1 >= 0.0 && "Time should be non-negative");

    // Test 2: multiple threads, mixed values
    std::vector<int> arr2 = {3, 100, 42, 8, 7, 200};
    double t2 = measureOpenMPMaxReduction(2, arr2.size(), arr2);
    assert(t2 >= 0.0 && "Time should be non-negative");

    // Test 3: many threads (more than needed), single element
    std::vector<int> arr3 = {42};
    int max_threads = omp_get_max_threads();
    double t3 = measureOpenMPMaxReduction(max_threads, arr3.size(), arr3);
    assert(t3 >= 0.0 && "Time should be non-negative");

    // Test 4: all identical values
    std::vector<int> arr4(1000, 7);
    double t4 = measureOpenMPMaxReduction(4, arr4.size(), arr4);
    assert(t4 >= 0.0 && "Time should be non-negative");

    // Test 5: array with large positive int
    std::vector<int> arr5 = {1000000, 2, 3, 4};
    double t5 = measureOpenMPMaxReduction(1, arr5.size(), arr5);
    assert(t5 >= 0.0 && "Time should be non-negative");

    // Test 6: zero-length array should not be called per spec, but ensure function handles count=0 gracefully
    // The function would return time without entering loop (max remains min int) — but spec says non-empty, so skip.

    // Test 7: repeated calls give consistent positive times
    double u1 = measureOpenMPMaxReduction(4, arr2.size(), arr2);
    double u2 = measureOpenMPMaxReduction(4, arr2.size(), arr2);
    assert(u1 >= 0.0 && u2 >= 0.0);

    // Test 8: negative times not possible
    assert(u1 + 1.0 > u1);

    // Test 9: thread count = 0? Not allowed per spec (positive threads), but check if passed, OpenMP might error. Skip.

    // Test 10: time with one thread is not NaN
    assert(t1 == t1 && "t1 should not be NaN");

    return 0;
}

#include <vector>
#include <limits>
#include <omp.h>
#include <chrono>

// Returns the average wall-clock time (seconds) to find the max of array
// using an OpenMP parallel reduction over 'threads_num' threads.
double measureOpenMPMaxReduction(int threads_num, int count, const std::vector<int>& array) {
    const int repetitions = 10;
    double total_time = 0.0;

    for (int rep = 0; rep < repetitions; ++rep) {
        double start = omp_get_wtime();
        int max_val = std::numeric_limits<int>::min();

        #pragma omp parallel num_threads(threads_num) shared(array, count) reduction(max: max_val)
        {
            #pragma omp for
            for (int i = 0; i < count; ++i) {
                if (array[i] > max_val) {
                    max_val = array[i];
                }
            }
        }

        double end = omp_get_wtime();
        total_time += (end - start);
    }

    return total_time / repetitions;
}

// The solution uses OpenMP's `max` reduction to compute the array maximum in parallel. For each repetition, record `omp_get_wtime()` before and after the parallel region, compute the elapsed time, and accumulate it. After 10 repetitions, return the accumulated time divided by 10. The reduction ensures thread-safe updates to a shared maximum variable without explicit locks. Key edge cases: the initial value of `max` should be set to the smallest possible integer (`std::numeric_limits<int>::min()`) so any real element updates it; if the array has only one element, the parallel loop still works; empty arrays are not allowed per the specification. Time complexity is O(count) per repetition (with parallel speedup), and the 10 repetitions make it O(10*count). Space complexity is O(1) auxiliary, aside from the input vector.
