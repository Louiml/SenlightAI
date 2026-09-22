/*
You are given an array of positive integers representing the time required to paint each board, and an integer `k` representing the number of painters available. Each painter can paint a contiguous set of boards, and all painters work in parallel at the same rate. The goal is to minimize the maximum total time any single painter spends, which is equivalent to partitioning the array into at most `k` contiguous subarrays such that the largest subarray sum is as small as possible. Write a C++ function `long long minimizeLargestPartitionSum(const std::vector<int>& boards, int painters)` that returns the minimized maximum subarray sum. If `painters` is greater than or equal to the number of boards, the answer is simply the maximum board time. If `painters` is less than 1, return -1. The function must handle arrays up to length 10^5 and board times up to 10^9, so use 64-bit integers for sums and intermediate results.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimized maximum contiguous subarray sum when partitioning into at most `painters` groups.
// If painters <= 0, returns -1. If array empty, returns 0.
long long minimizeLargestPartitionSum(const std::vector<int>& boards, int painters) {
    const int n = static_cast<int>(boards.size());
    if (painters <= 0) return -1;
    if (n == 0) return 0;

    long long maxVal = 0;
    long long totalSum = 0;
    for (int val : boards) {
        if (val < 0) return -1; // boards must be non-negative
        maxVal = std::max(maxVal, static_cast<long long>(val));
        totalSum += val;
    }
    if (painters >= n) return maxVal;

    // Check if we can partition into at most `painters` groups with each sum <= limit.
    auto feasible = [&](long long limit) -> bool {
        long long groupSum = 0;
        int groups = 1;
        for (int val : boards) {
            if (groupSum + val > limit) {
                groups++;
                groupSum = val;
                if (groups > painters) return false;
            } else {
                groupSum += val;
            }
        }
        return true;
    };

    long long low = maxVal;
    long long high = totalSum;
    long long answer = totalSum;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (feasible(mid)) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// The function is declared above; include its definition before this test block.
// To keep the test self-contained, the function is assumed to be available.
int main() {
    // Basic examples
    assert(minimizeLargestPartitionSum({1, 2, 3, 4, 5, 6, 7, 8, 9}, 3) == 17); // partition: [1,2,3,4,5]=15, [6,7]=13, [8,9]=17
    assert(minimizeLargestPartitionSum({10, 20, 30, 40, 50}, 2) == 90); // [10,20,30,40]=100? Actually best: [10,20,30]=60 and [40,50]=90 -> max 90
    // Actually for {10,20,30,40,50}, best partition is [10,20,30,40]=100 and [50]=50 -> max 100, or [10,20,30]=60 and [40,50]=90 -> max 90. So 90 is correct.
    assert(minimizeLargestPartitionSum({1, 1, 1, 1}, 2) == 2); // [1,1] and [1,1]
    assert(minimizeLargestPartitionSum({5}, 1) == 5);
    assert(minimizeLargestPartitionSum({5}, 10) == 5); // painters >= n

    // Edge cases
    assert(minimizeLargestPartitionSum({}, 3) == 0);
    assert(minimizeLargestPartitionSum({1, 2, 3}, 0) == -1);
    assert(minimizeLargestPartitionSum({1, 2, 3}, -1) == -1);
    assert(minimizeLargestPartitionSum({100, 1, 100}, 2) == 101); // [100] and [1,100] -> max 101
    
    // Larger test with many painters
    std::vector<int> boards(100000, 1000000);
    assert(minimizeLargestPartitionSum(boards, 100000) == 1000000);
    assert(minimizeLargestPartitionSum(boards, 1) == 100000LL * 1000000);

    // All same values
    assert(minimizeLargestPartitionSum({7, 7, 7, 7}, 3) == 14); // [7,7]=14, [7], [7] -> max 14
    return 0;
}

// The problem is a classic binary search on the answer. Let `low` be the maximum single board time (since any partition must have at least one board per subarray, so the largest sum cannot be smaller than the maximum element) and `high` be the sum of all board times (when a single painter does all work). We binary search the candidate maximum sum `mid`. For each candidate, we greedily count the minimum number of contiguous groups needed so that no group sum exceeds `mid`: iterate through the array, accumulating boards into the current group; if adding the next board would exceed `mid`, start a new group and increment the group count. If the required number of groups is ≤ `painters`, then `mid` is feasible and we try smaller values; otherwise increase `low`. The binary search narrows down to the smallest feasible `mid`. This greedy count works because for a fixed maximum sum, using as few groups as possible (by packing as many boards as possible into each group) can only help meet the painter limit; if even that minimum exceeds the limit, no valid partition exists. Edge cases: if `painters <= 0`, return -1; if `painters >= n`, return max element; if the array is empty (n=0) but painters >0, return 0 (since no work). Time complexity is O(n log(sum)) because each feasibility check is O(n) and binary search runs over a range of size up to sum (log2(sum) iterations). Space complexity is O(1) auxiliary.
