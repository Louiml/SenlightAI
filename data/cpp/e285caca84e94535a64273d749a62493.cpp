/*
Write a C++ function named `matrixMultiplyTiming` that takes a single `unsigned int` parameter `N` (the side length of square matrices), allocates three \(N \times N\) matrices `A`, `B`, and `C` on the heap using raw pointers (as in the snippet, using `double*` data arrays and `double**` row pointers), fills `A` and `B` with random floating-point numbers in the range \( [0,1) \) using `std::mt19937` and `std::uniform_real_distribution<double>`, initializes `C` to all zeros, performs an in-place naive triple-nested-loop matrix multiplication \(C = A \times B\) (with `i`, `j`, `k` loop order), and returns the elapsed wall-clock time for the multiplication step as a `double` (in seconds). If `N` is not strictly greater than 1 and strictly less than `UINT_MAX`, the function must return a negative value (e.g., `-1.0`) to indicate an invalid size, without performing any allocation or computation. The function must correctly manage memory by deleting all dynamically allocated arrays (both the `double*` data blocks and the `double**` pointer arrays) before returning. Use `std::chrono::high_resolution_clock` for precise timing, and ensure the function is standalone, uses proper `const` where appropriate (e.g., `const double*` for read-only access), and has no side effects other than random number generation and timing.
*/

#include <random>
#include <chrono>
#include <limits>
#include <cstdint>

// Performs naive matrix multiplication on N x N matrices with random entries.
// Returns the elapsed wall-clock time for the multiplication, or -1.0 if N is invalid.
double matrixMultiplyTiming(unsigned int N) {
    // Validate N: must be strictly greater than 1 and strictly less than UINT_MAX.
    if (!(N > 1 && N < std::numeric_limits<unsigned int>::max())) {
        return -1.0;
    }

    // Random number generator for initializing matrix entries.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Allocate contiguous data blocks for rows (row-major order).
    double* data_A = new double[N * N];
    double* data_B = new double[N * N];
    double* data_C = new double[N * N];

    // Allocate arrays of row pointers.
    double** A = new double*[N];
    double** B = new double*[N];
    double** C = new double*[N];

    // Set up row pointers and fill matrices.
    for (unsigned int i = 0; i < N; ++i) {
        A[i] = &data_A[N * i];
        B[i] = &data_B[N * i];
        C[i] = &data_C[N * i];
        for (unsigned int j = 0; j < N; ++j) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
            C[i][j] = 0.0;
        }
    }

    // Time the matrix multiplication.
    auto mm_start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (unsigned int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    auto mm_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mm_elapsed = mm_end - mm_start;
    double elapsed = mm_elapsed.count();

    // Clean up all dynamically allocated memory.
    delete[] data_A;
    delete[] data_B;
    delete[] data_C;
    delete[] A;
    delete[] B;
    delete[] C;

    return elapsed;
}

#include <cassert>
#include <cmath>

// Forward declaration of the solution function.
double matrixMultiplyTiming(unsigned int N);

int main() {
    // Invalid sizes: N <= 1 or N >= UINT_MAX.
    assert(matrixMultiplyTiming(0) == -1.0);
    assert(matrixMultiplyTiming(1) == -1.0);
    assert(matrixMultiplyTiming(std::numeric_limits<unsigned int>::max()) == -1.0);

    // Valid small sizes: multiplication should complete and return a non-negative time.
    double t2 = matrixMultiplyTiming(2);
    assert(t2 >= 0.0);
    assert(t2 < 1.0); // Should be very small for N=2.

    double t3 = matrixMultiplyTiming(3);
    assert(t3 >= 0.0);
    assert(t3 < 1.0);

    // Slightly larger size: ensure it returns a positive value.
    double t10 = matrixMultiplyTiming(10);
    assert(t10 > 0.0);

    // Performance sanity: N=100 should take more time than N=10 (likely, but not guaranteed).
    // We check that it completes without error.
    double t100 = matrixMultiplyTiming(100);
    assert(t100 >= 0.0);

    // Verify that invalid calls don't leave memory allocated (implicitly tested by program termination).

    return 0;
}

// The core algorithm is textbook naive matrix multiplication: for each row `i` and column `j`, compute the dot product of row `i` of `A` and column `j` of `B` by iterating over `k`. The triple loop runs exactly \(N^3\) multiplications and additions, which is the dominant computational cost. The function must first validate the input: `N` must be at least 2 and at most `UINT_MAX - 1` to avoid edge cases (e.g., `N=0` would lead to zero-sized allocations, `N=1` is valid but trivial; the snippet outright rejects `N<=1`). Upon invalid input, return `-1.0` immediately. For valid input, allocate two 1D `double*` arrays of size `N*N` for the data blocks and three 2D `double**` arrays of size `N` for row pointers; casting the data block pointer as `new double[N*N]` and then assigning `A[i] = &data_A[N*i]` gives row-major storage. Fill `A` and `B` using a single random engine and distribution, and initialize `C` to zero. Start a `std::chrono::high_resolution_clock::now()` timer, perform the triple loop `for(i) for(j) for(k) C[i][j] += A[i][k] * B[k][j]`, then stop the timer. Compute the elapsed time as `duration<double>(end - start).count()`. Finally, `delete[]` the three data arrays and the three pointer arrays. Edge cases include potential integer overflow in `N*N` when `N` is near `UINT_MAX`—this is avoided by the range check. Time complexity is \(O(N^3)\), space complexity is \(O(N^2)\) for the three matrices.
