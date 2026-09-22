// Write a C++ function named `maxPartitions` that takes a non-empty vector of integers containing a permutation of the numbers from 0 to n-1 (where n is the vector's size), and returns the maximum number of contiguous segments (partitions) into which the array can be split such that, after concatenating the segments in order, each segment's elements form a consecutive set of integers starting from the smallest element in that segment and increasing by 1, and the segments cover the full array. Essentially, split the array at the fewest possible boundaries while ensuring that in each segment, the set of values equals the set of indices from the segment's start to its end (i.e., the maximum value seen so far equals the current index when scanning left to right). Return that count. The vector is guaranteed to be a permutation, so no duplicates or missing numbers occur. Your function should be efficient and handle edge cases like a single-element array (which trivially yields 1 partition).
// The main observation is: for a permutation of 0..n-1, a valid partition point occurs exactly at index `i` (0-based) when the maximum value encountered from index 0 to i equals `i`. At that moment, the set of values in that prefix is exactly {0,1,...,i}, because the values are distinct and all ≤ i (since the global max is i) and there are i+1 numbers in that prefix. So we can slice there. Therefore, the algorithm is: iterate through the array, maintain a running maximum of elements seen so far; whenever that maximum equals the current index, increment the partition count. This counts the maximum number of partitions, because each valid boundary is exactly where the prefix condition holds, and splitting at all such boundaries yields non-overlapping valid segments that together cover the whole array. Edge cases: array size 1 → max=0, index=0 → count=1. If the array is already sorted ascending (0,1,2,...), every index satisfies, producing n partitions. If the array is reversed (n-1, n-2, ..., 0), only the final index satisfies (max=n-1 equals last index), producing 1 partition. Time complexity is O(n) single pass, space O(1) auxiliary (excluding input storage).
#include <vector>
#include <algorithm>

// Given a permutation of 0..n-1, return the maximum number of partitions
// such that each partition's values form a contiguous range equal to its indices.
int maxPartitions(const std::vector<int>& arr) {
    int partitions = 0;
    int currentMax = -1;
    const int n = static_cast<int>(arr.size());

    for (int i = 0; i < n; ++i) {
        currentMax = std::max(currentMax, arr[i]);
        if (currentMax == i) {
            ++partitions;
        }
    }

    return partitions;
}
#include <cassert>
#include <vector>

// Function under test (declared here for the test; normally placed in header/source)
int maxPartitions(const std::vector<int>& arr);

int main() {
    // Single element
    assert(maxPartitions({0}) == 1);

    // Already sorted ascending: each index is a partition
    assert(maxPartitions({0, 1, 2, 3}) == 4);

    // Descending: only the last index qualifies
    assert(maxPartitions({3, 2, 1, 0}) == 1);

    // Mixed case: [1,0,2] -> partitions: [1,0] and [2] => 2
    assert(maxPartitions({1, 0, 2}) == 2);

    // Mixed case: [0,2,1] -> only [0] and then [2,1] => actually max=0 at i=0 (partition 1), then max=2 at i=1? no, at i=1 max=2 !=1, at i=2 max=2==2 => partition => total 2
    assert(maxPartitions({0, 2, 1}) == 2);

    // Case where one big partition: [2,0,1] -> max=2 at i=2 only => 1
    assert(maxPartitions({2, 0, 1}) == 1);

    // Case with multiple: [0,3,1,2] -> i=0 gives partition (max=0), then max=3 at i=3 gives another => 2
    assert(maxPartitions({0, 3, 1, 2}) == 2);

    // Larger test: [2,3,0,1,4] -> i=3 gives max=3 equals index 3 -> partition, then i=4 gives partition => 2
    assert(maxPartitions({2, 3, 0, 1, 4}) == 2);

    // Random valid permutation: [3,1,2,0,4] -> max=3 never equals index until i=3? at i=3 max=4? Actually max at i=3 is 3, equals 3 -> partition; then i=4 max=4 -> partition => 2
    assert(maxPartitions({3, 1, 2, 0, 4}) == 2);

    return 0;
}
