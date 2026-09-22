// You are given an array `a` of length `n` (where `n` is divisible by 4 and `n ≥ 4`), containing each integer from `1` to `n` exactly once (a permutation). Write a C++ function `std::vector<std::vector<int>> applyPermutationOperations(int n, const std::vector<int>& a)` that simulates the following three operations, performed sequentially on a working copy of the array:  
//
// 1. **Operation 1:** Mark all positions `p` that satisfy `p > (3n/4)` (1-indexed) OR `a[p] > (3n/4)`. Then, collect all marked positions, sort their values in ascending order, and write these sorted values back into the array at those marked positions (in the same order of marked positions from smallest to largest).  
// 2. **Operation 2:** Now mark all positions `p` that satisfy `(2n/4) < p ≤ (3n/4)` OR `(2n/4) < a[p] ≤ (3n/4)` (based on the current array after operation 1). Apply the same sort-and-write-back rule.  
// 3. **Operation 3:** Finally, mark all positions `p` with `p ≤ (n/2)`. Apply the same sort-and-write-back rule.  
//
// Return the final array after all three operations as a vector of integers. The operations must be performed exactly as described, using 1-indexed positions, and the array values are always a permutation of `1..n`.  
//
// **Constraints:** `n` is a multiple of 4, `4 ≤ n ≤ 100000`. The input array is guaranteed to be a permutation. Your function must run in `O(n log n)` time or better and use `O(n)` extra space.
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is assumed to be defined above.
int main() {
    // Test 1: n=4, arbitrary permutation
    std::vector<int> a1 = {4, 3, 2, 1};
    std::vector<int> r1 = applyPermutationOperations(4, a1);
    assert((r1 == std::vector<int>{1, 2, 3, 4}));

    // Test 2: n=4, already sorted
    std::vector<int> a2 = {1, 2, 3, 4};
    std::vector<int> r2 = applyPermutationOperations(4, a2);
    assert((r2 == std::vector<int>{1, 2, 3, 4}));

    // Test 3: n=8, reversed
    std::vector<int> a3 = {8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> r3 = applyPermutationOperations(8, a3);
    assert((r3 == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

    // Test 4: n=8, specific permutation
    std::vector<int> a4 = {5, 6, 7, 8, 1, 2, 3, 4};
    std::vector<int> r4 = applyPermutationOperations(8, a4);
    assert((r4 == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

    // Test 5: n=12, random permutation
    std::vector<int> a5 = {12, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11};
    std::vector<int> r5 = applyPermutationOperations(12, a5);
    assert((r5 == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}));

    // Test 6: n=4, single swap
    std::vector<int> a6 = {2, 1, 3, 4};
    std::vector<int> r6 = applyPermutationOperations(4, a6);
    assert((r6 == std::vector<int>{1, 2, 3, 4}));

    // Test 7: n=8, worst-case arrangement
    std::vector<int> a7 = {1, 8, 2, 7, 3, 6, 4, 5};
    std::vector<int> r7 = applyPermutationOperations(8, a7);
    assert((r7 == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

    // Test 8: n=4, identity but check pass boundaries
    std::vector<int> a8 = {1, 2, 3, 4};
    std::vector<int> r8 = applyPermutationOperations(4, a8);
    assert((r8 == std::vector<int>{1, 2, 3, 4}));

    // Test 9: n=8, all-mixed
    std::vector<int> a9 = {3, 1, 4, 2, 7, 5, 8, 6};
    std::vector<int> r9 = applyPermutationOperations(8, a9);
    assert((r9 == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8}));

    // Test 10: n=4, reversed again for robustness
    std::vector<int> a10 = {4, 1, 3, 2};
    std::vector<int> r10 = applyPermutationOperations(4, a10);
    assert((r10 == std::vector<int>{1, 2, 3, 4}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Apply the three marker-and-sort operations to a permutation of 1..n.
// The input is a permutation of 1..n; n is divisible by 4.
std::vector<int> applyPermutationOperations(int n, const std::vector<int>& a) {
    std::vector<int> arr = a; // working copy
    std::vector<int> pos(n + 1); // pos[value] = current index (1-based)
    for (int i = 0; i < n; ++i) {
        pos[arr[i]] = i + 1;
    }

    auto processPass = [&](int lowIndex, int highIndex, int lowValue, int highValue, bool useValueCondition) {
        std::vector<bool> marked(n + 1, false);
        // Mark positions
        for (int p = 1; p <= n; ++p) {
            if (p >= lowIndex && p <= highIndex) {
                marked[p] = true;
            } else if (useValueCondition && arr[p-1] >= lowValue && arr[p-1] <= highValue) {
                marked[p] = true;
            }
        }

        // Collect marked positions and their values
        std::vector<int> positions;
        std::vector<int> values;
        for (int p = 1; p <= n; ++p) {
            if (marked[p]) {
                positions.push_back(p);
                values.push_back(arr[p-1]);
            }
        }

        // Sort the extracted values
        std::sort(values.begin(), values.end());

        // Write back in order of increasing position
        for (size_t i = 0; i < positions.size(); ++i) {
            int p = positions[i];
            int newVal = values[i];
            arr[p-1] = newVal;
            pos[newVal] = p;
        }
    };

    // Pass 1: index > 3n/4 OR value > 3n/4
    processPass(3*n/4 + 1, n, 3*n/4 + 1, n, true);
    // Pass 2: index in (n/2, 3n/4] OR value in (n/2, 3n/4]
    processPass(n/2 + 1, 3*n/4, n/2 + 1, 3*n/4, true);
    // Pass 3: index ≤ n/2
    processPass(1, n/2, 0, 0, false);

    return arr;
}
// The key is to simulate the three passes exactly as specified. For each pass, we need to determine which positions are marked. A position `p` (1-indexed) is marked if either its index falls into the given index range for that pass, or the value currently at position `p` falls into the given value range for that pass. Note that the ranges for index and value are identical in each pass:  
// - Pass 1: index > 3n/4 OR value > 3n/4  
// - Pass 2: index in (n/2, 3n/4] OR value in (n/2, 3n/4]  
// - Pass 3: index ≤ n/2 (no value condition)  
//
// After collecting all marked positions, we extract the values at those positions, sort them ascending, and write them back into the marked positions in the same order (smallest position first). This modifies the array. We then repeat for the next pass.
//
// Important edge cases:  
// - A position may be marked in multiple passes; we need to use a boolean visited array reset each pass.  
// - Since values are a permutation, the value ranges are contiguous intervals.  
// - The sorting step per pass can be done by extracting and sorting; total time is O(n log n) because each pass processes at most n elements.  
// - The problem mimics a sorting network; at the end, the array might not be fully sorted if n is not a power of 2, but we don’t need to sort, just apply the operations.
//
// Time complexity: O(n log n) overall (three passes, each sorts a subset of size ≤ n). Space: O(n) for the working array and a boolean visited array.
