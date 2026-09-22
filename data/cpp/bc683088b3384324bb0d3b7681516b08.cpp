Write a C++ function that simulates the core computation of a distributed dot product using the MPI-style scatter-then-reduce pattern, but implemented entirely with standard C++ so it can run without an MPI library. The function should take two vectors of equal length and a positive integer `num_procs` representing the number of processes that would participate. It should compute the dot product of the two vectors by conceptually splitting the vectors into contiguous chunks of equal size across the processes (assuming the vector length is divisible by `num_procs`), having each process compute the partial dot product of its chunk, and then summing all partial results (simulating the root process receiving from others). The function must handle the case where `num_procs == 1` by simply computing the full dot product. The input vectors may contain any integers (including negative and zero). Return the final dot product as an integer.

// The algorithm mimics a simple parallel reduction. First, assert that the vector length is non-zero and divisible by `num_procs`. Then compute `chunk_size = vec_size / num_procs`. For each process index `p` from 0 to `num_procs-1`, compute the partial sum of products for indices `p*chunk_size` through `(p+1)*chunk_size - 1`. Accumulate these partial sums into a single `total` variable, which simulates the root process summing results from all processes (including itself). The key edge case is `num_procs == 1`, where the loop runs once for the entire vector, which works naturally. The case of empty vectors is explicitly disallowed. Negative values are handled by normal integer multiplication and addition. The time complexity is O(n) because we iterate over each element exactly once, regardless of the number of processes; the space complexity is O(1) extra (only a few integers), as no additional containers are created. The function should be `const`-correct, taking vectors by `const std::vector<int>&`.

#include <vector>
#include <cassert>

// Compute dot product of two equal-length vectors using a simulated MPI scatter-reduce pattern.
int simulatedParallelDotProduct(const std::vector<int>& a, const std::vector<int>& b, int num_procs) {
    const size_t n = a.size();
    assert(n == b.size());
    assert(n > 0);
    assert(n % num_procs == 0);
    assert(num_procs > 0);

    const size_t chunk_size = n / num_procs;
    int total = 0;

    for (int proc = 0; proc < num_procs; ++proc) {
        int partial = 0;
        for (size_t i = proc * chunk_size; i < (proc + 1) * chunk_size; ++i) {
            partial += a[i] * b[i];
        }
        total += partial; // Simulate sending to root and summing
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with 2 processes
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> b1 = {5, 6, 7, 8};
    assert(simulatedParallelDotProduct(a1, b1, 2) == 70); // 1*5+2*6+3*7+4*8 = 70

    // Single process (full dot product)
    assert(simulatedParallelDotProduct(a1, b1, 1) == 70);

    // Three processes, length 6
    std::vector<int> a2 = {1, -2, 3, 4, 5, -6};
    std::vector<int> b2 = {2, 3, -4, 5, 6, 1};
    assert(simulatedParallelDotProduct(a2, b2, 3) == (2 - 6 - 12 + 20 + 30 - 6) == 28);

    // Four processes, all zeros
    std::vector<int> a3 = {0, 0, 0, 0};
    std::vector<int> b3 = {1, 2, 3, 4};
    assert(simulatedParallelDotProduct(a3, b3, 4) == 0);

    // Two processes with negative values
    std::vector<int> a4 = {-1, -2, -3, -4};
    std::vector<int> b4 = {-1, -2, -3, -4};
    assert(simulatedParallelDotProduct(a4, b4, 2) == (1 + 4 + 9 + 16) == 30);

    return 0;
}
