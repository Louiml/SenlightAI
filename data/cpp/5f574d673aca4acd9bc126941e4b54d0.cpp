Write a C++ function that takes a vector of integers and determines the minimum total number of adjustments required so that after sorting the vector in ascending order, each element at index i (0-based) becomes equal to i. In one adjustment, you can increase or decrease a single element by 1. The function should return the minimum total number of adjustments needed. The input vector may contain any integers (including negative values and duplicates), and the vector length n can be any positive integer. If the vector is already such that after sorting each element equals its index, the answer is 0.
// The key observation is that we want the sorted array to become exactly [0, 1, 2, ..., n-1]. Since we can only change element values by ±1 per operation, the minimum number of operations needed for each position is the absolute difference between the current sorted value at that position and the desired index. Therefore, the algorithm is: sort the vector in ascending order, then iterate over each index i, compute the absolute difference between v[i] and i, and sum these differences. Sorting ensures we match the largest available numbers to the highest indices, which is optimal because the cost function is sum of absolute differences, and the rearrangement inequality tells us that matching sorted input to sorted targets minimizes this sum. Edge cases include negative numbers (the absolute difference handles them), duplicates (each duplicate still must be moved to its own target index), and empty vectors (though the problem states n is positive, returning 0 for empty is safe). Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (ignoring the input vector itself).
#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns the minimum total number of ±1 adjustments needed so that
// after sorting, each element at index i equals i.
long long minimumAdjustments(std::vector<int> values) {
    // Sort a copy of the input to avoid modifying the caller's data.
    std::sort(values.begin(), values.end());

    long long totalCost = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        totalCost += std::abs(values[i] - static_cast<int>(i));
    }
    return totalCost;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // Already perfect: [0, 1, 2]
    assert(minimumAdjustments({0, 1, 2}) == 0);

    // Sorted becomes [1, 2, 3]; costs: |1-0|=1, |2-1|=1, |3-2|=1 => total 3
    assert(minimumAdjustments({3, 2, 1}) == 3);

    // Negative values: sorted [-2, 0, 1]; costs: |-2-0|=2, |0-1|=1, |1-2|=1 => total 4
    assert(minimumAdjustments({1, -2, 0}) == 4);

    // Duplicates: sorted [1, 1]; costs: |1-0|=1, |1-1|=0 => total 1
    assert(minimumAdjustments({1, 1}) == 1);

    // Single element: [5] -> |5-0|=5
    assert(minimumAdjustments({5}) == 5);

    // Large numbers: sorted [100, 200]; costs: |100-0|=100, |200-1|=199 => total 299
    assert(minimumAdjustments({200, 100}) == 299);

    // Empty vector: cost 0 (though problem says positive n, still safe)
    assert(minimumAdjustments({}) == 0);

    // Mixed positive and negative with a perfect match after sort
    // sorted [-1, 0, 1] -> costs: |-1-0|=1, |0-1|=1, |1-2|=1 => total 3
    assert(minimumAdjustments({0, -1, 1}) == 3);

    return 0;
}
