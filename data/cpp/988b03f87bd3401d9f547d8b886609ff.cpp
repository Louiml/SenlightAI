// Write a C++ function `int minimumSortCost(const std::vector<int>& arr)` that takes a non-empty array of integers and returns the minimum total cost to sort the entire array, where in one operation you may select any contiguous sub-array and sort its elements (in-place). The cost of one operation is the square of the length of that sub-array. You may perform any number of operations, and the goal is to sort the whole array in non-decreasing order. The function must compute the minimum possible total cost. For example, for `arr = {4,3,2,1}`, the optimal is to sort the whole array of length 4, cost `4*4=16`. For `arr = {2,3,1,6,4,5}`, the optimal cost is 18.

#include <cassert>
#include <vector>

int minimumSortCost(const std::vector<int>& arr); // declaration from solution

int main() {
    // Example from problem statement: [4,3,2,1]
    assert(minimumSortCost({4,3,2,1}) == 16);

    // Example from problem statement: [2,3,1,6,4,5]
    assert(minimumSortCost({2,3,1,6,4,5}) == 18);

    // Already sorted array -> each element is its own block -> cost 0
    assert(minimumSortCost({1,2,3,4,5}) == 0);

    // Single element -> cost 0
    assert(minimumSortCost({7}) == 0);

    // Two elements reversed -> sort whole length 2 -> cost 4
    assert(minimumSortCost({5,1}) == 4);

    // Three elements with one inversion in middle: [1,3,2] -> sort [2..3] length 2 -> cost 4
    assert(minimumSortCost({1,3,2}) == 4);

    // Duplicate values: [2,2,1,1] -> sorted is [1,1,2,2], the whole array is a block -> cost 16
    assert(minimumSortCost({2,2,1,1}) == 16);

    // [3,1,2] -> need to sort whole array (length 3) -> cost 9 (because all three are out of place)
    assert(minimumSortCost({3,1,2}) == 9);

    // [1,2,2,3] already sorted with duplicates -> cost 0
    assert(minimumSortCost({1,2,2,3}) == 0);

    // [2,1,3,5,4] -> blocks: [2,1] and [5,4] each length 2 -> cost 4+4=8
    assert(minimumSortCost({2,1,3,5,4}) == 8);

    // Long repeated pattern: [3,2,1,3,2,1] -> sorted [1,1,2,2,3,3] -> whole array is one block? Actually no: each pair of identical values? Let's compute: unsorted block is entire array because no prefix multiset matches. So cost 36.
    assert(minimumSortCost({3,2,1,3,2,1}) == 36);

    return 0;
}

#include <vector>
#include <algorithm>
#include <unordered_map>

// Returns the minimum total cost to sort the array using sub-array sorting operations.
int minimumSortCost(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n <= 1) return 0;

    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    std::unordered_map<int, int> freqArr, freqSorted;
    int totalCost = 0;
    int blockStart = 0;

    for (int i = 0; i < n; ++i) {
        freqArr[arr[i]]++;
        freqSorted[sorted[i]]++;

        if (freqArr == freqSorted) {
            // Current block from blockStart to i is a closed permutation segment.
            int blockLength = i - blockStart + 1;
            totalCost += blockLength * blockLength;
            freqArr.clear();
            freqSorted.clear();
            blockStart = i + 1;
        }
    }

    return totalCost;
}

// The key insight is that sorting a sub-array is only necessary where the original array is not already in its sorted order. Place the sorted version `b` of the array next to the original. Compare the two sequences from left to right. The positions where `a[i] != b[i]` mark elements that are not in their final sorted position. However, we can sort larger chunks at once; the optimal strategy is to partition the array into segments that are "permutation matches" between the original and sorted arrays. Specifically, scan from left to right keeping a frequency map for both `a` and `b` for the current candidate segment. As soon as the two frequency maps become equal, that segment contains exactly the same multiset of elements as its sorted counterpart, so we can sort it as a block (cost = length²). If we split at every matching point, we get a partition into minimal independent blocks; sorting each block independently is optimal because sorting a block that spans multiple independent blocks would cost more (square of larger length) but not save anything. The algorithm iterates over the array, building the maps and resetting them when they match, accumulating the square of the length of each block. Edge cases: already sorted array yields blocks of length 1 → cost 0; single element → cost 0; duplicate values are handled correctly by frequency comparison. Time complexity is O(n) average because we do constant work per element and map operations are O(1) average; worst-case O(n²) due to unordered_map but practically linear. Space complexity O(n) for the sorted copy and the maps.
