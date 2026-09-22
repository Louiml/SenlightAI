// Write a C++ function `std::vector<int> reconstructFromPairwiseMinima(int n, const std::vector<int>& pairwiseMinima)` that, given the number of elements `n` (2 ≤ n ≤ 1000) and the multiset of all pairwise minima `min(a[i], a[j])` for 1 ≤ i < j ≤ n (exactly n*(n-1)/2 integers, not necessarily sorted), reconstructs any valid original array `a` of length `n` whose pairwise minima match the input. The input is guaranteed to have at least one valid solution. The function must return the reconstructed array in any order that satisfies the condition. For example, if `n=3` and the minima are `[1,1,2]`, possible arrays are `[1,2,2]`, `[1,1,2]`, etc. Output the array with elements in non-decreasing order. If multiple valid arrays exist, return the lexicographically smallest one in non-decreasing order.

The key observation is that the smallest value in the entire multiset of pairwise minima must be the smallest element in the original array, because the minimum of any pair involving that smallest element equals that smallest value. More generally, after sorting the given minima, the smallest value must appear exactly `n-1` times (since the global minimum pairs with every other element). Remove those `n-1` copies, and the next smallest remaining value becomes the second smallest original element, and it will appear exactly `n-2` times among the remaining minima (pairs with elements 3..n). Continue this process: for k from 1 to n-1, the k-th smallest original element `x` appears `n-k` times in the remaining minima. Thus, we can sort the input minima, then repeatedly take the current smallest value, append it to the answer, and erase exactly `n - current_position` copies of that value from the multiset. After processing n-1 values, the last original element is the remaining minima (which will be a single value). This works because the input is guaranteed consistent. Edge case: n=2, then we have exactly one minima, which is the smaller of the two original elements; the larger is that same value (since only one minima) or any value >= it, but to be lexicographically smallest we set both equal to that minima. Complexity: sorting takes O(n^2 log n), but since n ≤ 1000, n^2 ≤ 1e6, which is fine. Alternatively, use a multiset with O(n^2 log n) time. Space: O(n^2) for the sorted copy. The algorithm is O(n^2 log n) time and O(n^2) space.

#include <vector>
#include <algorithm>

// Reconstruct the original array from the multiset of pairwise minima.
// Return the array in non-decreasing order, lexicographically smallest.
std::vector<int> reconstructFromPairwiseMinima(int n, const std::vector<int>& pairwiseMinima) {
    std::vector<int> sorted = pairwiseMinima;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> result;
    result.reserve(n);
    
    size_t pos = 0;  // current position in sorted list
    for (int k = 1; k <= n - 1; ++k) {
        int currentMin = sorted[pos];
        result.push_back(currentMin);
        // Remove exactly (n - k) copies of currentMin.
        // All these copies are consecutive starting at pos.
        pos += (n - k);
    }
    
    // The remaining single element is the largest original element.
    result.push_back(sorted[pos]);
    
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// (The solution function is assumed to be defined above.)

int main() {
    // Test 1: n=2, single minima
    {
        std::vector<int> in = {5};
        std::vector<int> out = reconstructFromPairwiseMinima(2, in);
        assert(out == std::vector<int>({5, 5}));
    }
    // Test 2: n=3, example [1,1,2]
    {
        std::vector<int> in = {1, 1, 2};
        std::vector<int> out = reconstructFromPairwiseMinima(3, in);
        assert(out == std::vector<int>({1, 1, 2})); // lexicographically smallest non-decreasing
    }
    // Test 3: n=3, with larger values
    {
        std::vector<int> in = {2, 2, 3};
        std::vector<int> out = reconstructFromPairwiseMinima(3, in);
        assert(out == std::vector<int>({2, 2, 3}));
    }
    // Test 4: n=4, all equal
    {
        std::vector<int> in = {7, 7, 7, 7, 7, 7};
        std::vector<int> out = reconstructFromPairwiseMinima(4, in);
        assert(out == std::vector<int>({7, 7, 7, 7}));
    }
    // Test 5: n=4, distinct
    {
        std::vector<int> in = {1, 1, 1, 2, 2, 3};
        std::vector<int> out = reconstructFromPairwiseMinima(4, in);
        assert(out == std::vector<int>({1, 1, 2, 3}));
    }
    // Test 6: n=5, mixed duplicates
    {
        std::vector<int> in = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
        std::vector<int> out = reconstructFromPairwiseMinima(5, in);
        assert(out == std::vector<int>({1, 1, 2, 3, 4}));
    }
    // Test 7: n=6, large values
    {
        std::vector<int> in = {10, 10, 10, 10, 10, 20, 20, 20, 20, 30, 30, 30, 40, 40, 50};
        std::vector<int> out = reconstructFromPairwiseMinima(6, in);
        assert(out == std::vector<int>({10, 10, 20, 30, 40, 50}));
    }
    // Test 8: n=2 with larger one
    {
        std::vector<int> in = {100};
        std::vector<int> out = reconstructFromPairwiseMinima(2, in);
        assert(out == std::vector<int>({100, 100}));
    }
    // Test 9: n=3 with unsorted input
    {
        std::vector<int> in = {3, 1, 1};
        std::vector<int> out = reconstructFromPairwiseMinima(3, in);
        assert(out == std::vector<int>({1, 1, 3}));
    }
    // Test 10: n=4 with zeros
    {
        std::vector<int> in = {0, 0, 0, 1, 1, 2};
        std::vector<int> out = reconstructFromPairwiseMinima(4, in);
        assert(out == std::vector<int>({0, 0, 1, 2}));
    }
    return 0;
}
