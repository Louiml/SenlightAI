/*
Write a C++ function named `parallel_acc` that takes an integer `num_threads` (greater than zero) and an initial integer `seed_value`, then uses OpenMP to repeatedly perform the same two operations (`add 1` and `add 3`) on a shared integer, in a parallel region with `num_threads` threads. The function must return the final value of that shared integer. The operations should be applied exactly once per thread (i.e., each thread executes `shared += 1; shared += 3;`), so the total added per thread is 4. The function must correctly handle the race condition using a synchronization mechanism (e.g., a critical section or atomic update) so the final result is deterministic and equals `seed_value + 4 * num_threads`. Do not include a `main` function in your solution; only provide the free function with appropriate headers and comments.
*/

#include <omp.h>

// Adds 1 and 3 to a shared integer once per thread using OpenMP.
// Returns seed_value + 4 * num_threads.
int parallel_acc(int num_threads, int seed_value) {
    int shared_value = seed_value;

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp critical
        {
            shared_value += 1;
            shared_value += 3;
        }
    }

    return shared_value;
}

#include <cassert>

int main() {
    // Single thread: just adds 4.
    assert(parallel_acc(1, 0) == 4);
    assert(parallel_acc(1, 100) == 104);

    // Multiple threads: deterministic 4 per thread.
    assert(parallel_acc(2, 0) == 8);
    assert(parallel_acc(4, 1) == 17);
    assert(parallel_acc(10, -5) == 35);

    // Larger thread count and seed values.
    assert(parallel_acc(16, 42) == 42 + 64);
    assert(parallel_acc(100, 7) == 407);

    // Check with an unusual but valid thread count.
    assert(parallel_acc(3, 0) == 12);
    assert(parallel_acc(8, 1000) == 1032);
}

// The core challenge is that in the original snippet, multiple threads increment the same shared variable without synchronization, causing a data race and nondeterministic results. To fix this, we must protect the updates to the shared integer. The simplest and most efficient approach is to use an OpenMP `critical` section around the two updates, or use `#pragma omp atomic` for each addition. Since the operations `+1` and `+3` are separate, we can wrap both in a single critical section to ensure they are applied together, though order does not matter for correctness of the final sum. Alternatively, we could combine them into a single `+= 4` with atomic update. The algorithm: allocate an integer on the heap (or stack) initialized to `seed_value`, enter a parallel region with `num_threads` threads, each thread performs the two additions inside a critical block, then after the region ends, return the final value. Edge cases: `num_threads` can be 1 (works fine), and large thread counts still yield deterministic results. Complexity: time is `O(num_threads)` since each thread does constant work; space is `O(1)`.
