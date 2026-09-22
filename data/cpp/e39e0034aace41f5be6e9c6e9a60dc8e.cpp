Write a C++ function `int countDivisibleInParallel(int rows, int cols, int divisor)` that simulates a parallel initialization of a 2D integer matrix where each cell `A[i][j]` is assigned the product `i * j`, and then returns the number of cells in the matrix whose value is divisible by `divisor`. The function must internally divide the rows among `NUM_THREADS` (set to 5) using pthreads, where each thread processes a contiguous block of rows, computes the product `i * j`, checks divisibility, and accumulates a local count. The main function (outside the solution) will test the function for small matrices where results can be verified by a brute-force sequential loop. The matrix dimensions `rows` and `cols` are positive integers (with `rows` guaranteed to be divisible by `NUM_THREADS`), and `divisor` is a positive integer. The function must be thread-safe and handle the case where both `rows` and `cols` are large (e.g., up to 10,000) by avoiding excessive memory—do not actually store the full matrix; instead, compute the product on the fly within each thread. The function returns the total count of divisible values.

The solution uses pthreads to partition the `rows` into `NUM_THREADS` contiguous segments of equal size (`rows / NUM_THREADS`). Each thread receives a `ThreadData` struct containing its thread ID and the starting row index. In its worker routine, the thread iterates over its assigned rows and all columns, computes `value = i * j`, and if `value % divisor == 0`, increments a local counter (not shared to avoid data races). After the loop, each thread stores its local count in its `ThreadData` struct. The main function (inside the solution function) creates `NUM_THREADS` threads, waits for all via `pthread_join`, and then sums the local counts from all thread data structs to produce the final result. Edge cases: if `divisor` is 1, every value is divisible, so the count is `rows * cols`. If `divisor` is large, the number of divisible values may be zero. The division `rows % NUM_THREADS == 0` is guaranteed by the task, so no remainder handling is needed. Time complexity is \(O(rows \times cols / NUM_THREADS)\) per thread, leading to overall \(O(rows \times cols)\) work, but parallelized across threads; space complexity is \(O(NUM_THREADS)\) for thread data and the pthread array, constant additional memory since no matrix storage.

#include <pthread.h>
#include <vector>

namespace {
    struct ThreadData {
        int id;
        int startRow;
        int rowsPerThread;
        int cols;
        int divisor;
        long long localCount;
    };

    void* worker(void* arg) {
        ThreadData* data = static_cast<ThreadData*>(arg);
        long long count = 0;
        int endRow = data->startRow + data->rowsPerThread;
        for (int i = data->startRow; i < endRow; ++i) {
            for (int j = 0; j < data->cols; ++j) {
                int value = i * j; // product may be large but within int range for given constraints
                if (value % data->divisor == 0) {
                    ++count;
                }
            }
        }
        data->localCount = count;
        return nullptr;
    }
}

// Count how many cells (i,j) in a rows x cols matrix where i*j is divisible by divisor.
// Parallelizes row processing across 5 threads. Requires rows % 5 == 0.
int countDivisibleInParallel(int rows, int cols, int divisor) {
    const int NUM_THREADS = 5;
    int rowsPerThread = rows / NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    std::vector<ThreadData> data(NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; ++i) {
        data[i].id = i;
        data[i].startRow = i * rowsPerThread;
        data[i].rowsPerThread = rowsPerThread;
        data[i].cols = cols;
        data[i].divisor = divisor;
        data[i].localCount = 0;
        pthread_create(&threads[i], nullptr, worker, &data[i]);
    }

    long long total = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
        total += data[i].localCount;
    }
    return static_cast<int>(total);
}

#include <cassert>

// Brute-force sequential checker for small dimensions
int bruteForce(int rows, int cols, int divisor) {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if ((i * j) % divisor == 0) {
                ++count;
            }
        }
    }
    return count;
}

int main() {
    // rows must be divisible by 5
    // Test 1: 5x5, divisor=2
    assert(countDivisibleInParallel(5, 5, 2) == bruteForce(5, 5, 2));

    // Test 2: 10x3, divisor=1 (all divisible)
    assert(countDivisibleInParallel(10, 3, 1) == 30);

    // Test 3: 10x10, divisor=100 (only i=0 or j=0 gives 0, divisible)
    assert(countDivisibleInParallel(10, 10, 100) == bruteForce(10, 10, 100));

    // Test 4: 5x7, divisor=7
    assert(countDivisibleInParallel(5, 7, 7) == bruteForce(5, 7, 7));

    // Test 5: 15x2, divisor=5
    assert(countDivisibleInParallel(15, 2, 5) == bruteForce(15, 2, 5));

    // Test 6: 20x1, divisor=3
    assert(countDivisibleInParallel(20, 1, 3) == bruteForce(20, 1, 3));

    // Test 7: 5x4, divisor=6
    assert(countDivisibleInParallel(5, 4, 6) == bruteForce(5, 4, 6));

    // Test 8: 100x100, divisor=10
    assert(countDivisibleInParallel(100, 100, 10) == bruteForce(100, 100, 10));

    // Test 9: 25x25, divisor=1
    assert(countDivisibleInParallel(25, 25, 1) == 625);

    // Test 10: 5x5, divisor=25
    assert(countDivisibleInParallel(5, 5, 25) == bruteForce(5, 5, 25));

    return 0;
}
