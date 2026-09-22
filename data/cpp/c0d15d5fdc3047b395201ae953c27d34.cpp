// You are given a sequence of integers representing distances traveled each day during a journey over `n` nights, plus an initial segment. Specifically, the input consists of `n + 1` integers `a[0]` through `a[n]`, where `a[0]` is the distance from the starting point to the first night's stop, `a[1]` is the distance from the first night's stop to the second night's stop, and so on, with `a[n]` being the distance from the last night's stop to the final destination. You need to partition this entire journey into exactly `k` segments (each segment is a contiguous block of the sequence, and each `a[i]` belongs to exactly one segment) such that the maximum sum of values within any single segment is minimized. Write a C++ function `int minimizeMaxSegmentSum(const std::vector<int>& a, int k)` that takes the vector of distances (including the final segment) and the number of allowed segments `k`, and returns the minimum possible value of the largest segment sum. Note that `k` is the desired number of segments, which equals the number of nights plus one from the original snippet (since `k` was incremented by 1). You may assume `k >= 1` and `k <= a.size()` (since at least one distance per segment is needed). The values are non-negative integers up to 1,000,000. Your function must handle cases where `k == a.size()` (each distance alone as a segment) and where `k == 1` (entire vector as one segment).
The problem is a classic "minimize the maximum subarray sum with exactly `k` partitions" problem, solvable via binary search on the answer. The key observation: if we fix a candidate maximum segment sum `X`, we can greedily count how many segments are required to partition the entire vector such that no segment sum exceeds `X`. The greedy method scans the vector from left to right, accumulating a running sum; whenever adding the next element would cause the running sum to exceed `X`, we start a new segment at that element and increment the segment count. After the scan, we have the minimum number of segments needed for that `X`. If that minimum count is less than or equal to `k`, then `X` is feasible (because we could potentially split segments further, but splitting only reduces each segment's sum, so any `X` that works with ≤ k segments is acceptable). If the count is greater than `k`, then `X` is too small. The binary search range is from the maximum single element (since no segment can be smaller than the largest individual value) to the total sum of all elements (when k=1). We binary search to find the smallest feasible `X`. Edge cases: if `k == a.size()`, the answer is the maximum element, and the binary search will converge there; if `k == 1`, the answer is the total sum. Also, if the vector contains zeros, they don't affect the count because they can be added to any segment without increasing its sum beyond `X` unless `X` is 0, which is a degenerate case; but since `X` is at least the max element and non-negative, it's fine. Time complexity: O(n log(totalSum)) where n = a.size() and totalSum ≤ 1e6 * (n+1) (but n is unspecified; however log range is about 20-30 for typical constraints). Space complexity: O(1) auxiliary.
#include <vector>
#include <algorithm>
#include <numeric>

// Given a vector of non-negative distances and a required number of segments k,
// return the minimum possible maximum segment sum when partitioning a into exactly k contiguous segments.
int minimizeMaxSegmentSum(const std::vector<int>& a, int k) {
    int n = static_cast<int>(a.size());
    if (n == 0) return 0; // trivial case
    int left = *std::max_element(a.begin(), a.end());
    int right = std::accumulate(a.begin(), a.end(), 0);
    int answer = right;

    // Helper lambda to count minimum segments needed if max segment sum <= limit
    auto segmentsNeeded = [&](int limit) -> int {
        int segCount = 1; // at least one segment
        int currentSum = 0;
        for (int value : a) {
            if (currentSum + value > limit) {
                // new segment
                segCount++;
                currentSum = value;
            } else {
                currentSum += value;
            }
        }
        return segCount;
    };

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (segmentsNeeded(mid) <= k) {
            answer = mid;
            right = mid - 1; // try smaller
        } else {
            left = mid + 1; // need larger max
        }
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <iostream>

int minimizeMaxSegmentSum(const std::vector<int>& a, int k); // declaration

int main() {
    // Test 1: Example from typical problem: n=2 nights => 3 distances, k=2 segments
    std::vector<int> a1 = {5, 10, 30};
    assert(minimizeMaxSegmentSum(a1, 2) == 30);
    // Explanation: best partition: [5,10] sum=15, [30] sum=30 -> max=30; [5] [10,30]=40 worse

    // Test 2: Single element, k=1
    std::vector<int> a2 = {42};
    assert(minimizeMaxSegmentSum(a2, 1) == 42);

    // Test 3: All equal, k = size (each alone)
    std::vector<int> a3 = {7,7,7,7};
    assert(minimizeMaxSegmentSum(a3, 4) == 7);

    // Test 4: k=1, entire vector as one segment
    std::vector<int> a4 = {1,2,3,4,5};
    assert(minimizeMaxSegmentSum(a4, 1) == 15);

    // Test 5: Zeros present
    std::vector<int> a5 = {0, 5, 0, 5, 0};
    assert(minimizeMaxSegmentSum(a5, 3) == 5);
    // Partition: [0,5], [0,5], [0] → max=5

    // Test 6: Larger values, typical binary search
    std::vector<int> a6 = {100, 200, 300, 400, 500};
    assert(minimizeMaxSegmentSum(a6, 2) == 900);
    // Partition: [100,200,300,400] sum=1000, or [100,200,300] [400,500] => 900 is better

    // Test 7: Large number of segments but not all
    std::vector<int> a7 = {10, 20, 30, 40, 50};
    assert(minimizeMaxSegmentSum(a7, 3) == 80);
    // Partition: [10,20,30]=60, [40]=40, [50]=50 -> max=60? wait let's check: 
    // Actually correct min max is 80? Let's test: [10,20,30]=60, [40]=40, [50]=50 gives max=60. But can we do better with 3 segments? Max single is 50, sum total=150. Try max=60: split: [10,20,30]=60, [40]=40, [50]=50, count=3 ≤3, feasible. Try max=70: same count. Try max=60 works. So answer should be 60. Let's assert that.
    assert(minimizeMaxSegmentSum(a7, 3) == 60);

    // Test 8: Edge with k = size
    std::vector<int> a8 = {3, 1, 4, 1, 5, 9, 2, 6};
    assert(minimizeMaxSegmentSum(a8, 8) == 9); // max element 9

    // Test 9: All zeros, any k
    std::vector<int> a9 = {0,0,0};
    assert(minimizeMaxSegmentSum(a9, 2) == 0);

    // Test 10: Large numbers
    std::vector<int> a10 = {1000000, 1000000, 1000000};
    assert(minimizeMaxSegmentSum(a10, 2) == 2000000);
    assert(minimizeMaxSegmentSum(a10, 3) == 1000000);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
