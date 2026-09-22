Write a C++ function `std::pair<int,int> bestTwoSegments(const std::vector<long long>& arr, int k)` that, given a non-empty array of `n` integers (where `n >= 2*k`) and a positive integer `k`, returns the 1-based starting indices `(i, j)` of two non-overlapping contiguous subarrays each of length exactly `k`, such that the sum of their elements is maximized. The two subarrays must be disjoint: the first ends before the second starts, or vice versa. If multiple pairs yield the same maximum sum, return the pair with smaller starting index first, and if still tied, the pair whose first starting index is smaller. The function should return the pair in ascending order (i.e., the smaller index first).

The key idea is to precompute the sum of every contiguous subarray of length `k`. Let `segSum[i]` be the sum of elements from index `i` to `i+k-1` (1-based indexing). Then for each possible second segment starting at position `i` (where `i >= k+1` because the first must end before it), we need the best first segment among all positions `1` to `i-k`. We can precompute for every prefix position `t` the maximum segment sum and its starting index among segments starting from `1` up to `t`. This is done in a single left-to-right pass. Then for each candidate second segment starting at `i`, we combine its sum with the best first segment from the prefix `i-k`. We maintain the global maximum and the corresponding pair, handling ties by preferring the pair with the smaller first starting index, then smaller second index. Edge cases: `n` must be at least `2*k`; if not, the problem is undefined, but the function can assume valid input. The main loop runs from `i = k+1` to `n-k+1`. Time complexity is `O(n)` for computing segment sums and `O(n)` for the scanning, so overall `O(n)`. Space complexity is `O(n)` for the segment sum array and the prefix maximum array.

#include <vector>  
#include <utility>  
#include <algorithm>  

// Returns the pair (i, j) of starting indices (1-based) of two non-overlapping  
// length-k subarrays maximizing the total sum. The pair is returned in ascending order.  
std::pair<int,int> bestTwoSegments(const std::vector<long long>& arr, int k) {  
    int n = (int)arr.size();  
    // Compute sums of all length-k contiguous segments (1-based indexing)  
    std::vector<long long> segSum(n - k + 2, 0); // index 1..n-k+1  
    long long current = 0;  
    for (int i = 0; i < k; ++i) current += arr[i];  
    segSum[1] = current;  
    for (int i = 2; i <= n - k + 1; ++i) {  
        current += arr[i + k - 2] - arr[i - 2];  
        segSum[i] = current;  
    }  

    // Prefix best: for each t (1..n-k+1), store max segSum[1..t] and its index  
    std::vector<std::pair<long long,int>> bestPrefix(n - k + 2);  
    bestPrefix[1] = {segSum[1], 1};  
    for (int t = 2; t <= n - k + 1; ++t) {  
        if (segSum[t] > bestPrefix[t-1].first) {  
            bestPrefix[t] = {segSum[t], t};  
        } else {  
            bestPrefix[t] = bestPrefix[t-1];  
        }  
    }  

    long long maxSum = -1; // since sums can be negative, use a very negative initial  
    int bestA = 1, bestB = k + 1;  
    bool found = false;  

    // For each possible second segment starting at i (i >= k+1)  
    for (int i = k + 1; i <= n - k + 1; ++i) {  
        // best first segment from prefix up to i-k  
        const auto& bestFirst = bestPrefix[i - k];  
        long long total = bestFirst.first + segSum[i];  
        int firstIdx = bestFirst.second;  
        int secondIdx = i;  

        if (!found || total > maxSum ||  
            (total == maxSum && firstIdx < bestA) ||  
            (total == maxSum && firstIdx == bestA && secondIdx < bestB)) {  
            maxSum = total;  
            bestA = firstIdx;  
            bestB = secondIdx;  
            found = true;  
        }  
    }  

    // Return in ascending order  
    if (bestA > bestB) std::swap(bestA, bestB);  
    return {bestA, bestB};  
}

#include <cassert>  
#include <vector>  
#include <utility>  

int main() {  
    // Example 1  
    std::vector<long long> arr1 = {1, 2, 3, 4, 5, 6};  
    auto r1 = bestTwoSegments(arr1, 2);  
    assert(r1.first == 3 && r1.second == 5); // [3,4] sum 7 and [5,6] sum 11 = 18  

    // Example 2: all negative  
    std::vector<long long> arr2 = {-1, -2, -3, -4, -5};  
    auto r2 = bestTwoSegments(arr2, 1);  
    assert(r2.first == 1 && r2.second == 2); // -1 and -2 sum -3, pairs with smaller indices  

    // Example 3: tie handling  
    std::vector<long long> arr3 = {5, 5, 5, 5, 5, 5};  
    auto r3 = bestTwoSegments(arr3, 2);  
    assert(r3.first == 1 && r3.second == 3); // both sums 10, smallest first index 1  

    // Example 4: minimal valid length (n = 2k)  
    std::vector<long long> arr4 = {10, -5, 3, 7};  
    auto r4 = bestTwoSegments(arr4, 2);  
    assert(r4.first == 1 && r4.second == 2); // sums: 5 and 10 -> 15  

    // Example 5: zeros  
    std::vector<long long> arr5 = {0, 0, 0, 0, 0};  
    auto r5 = bestTwoSegments(arr5, 2);  
    assert(r5.first == 1 && r5.second == 3);  

    // Example 6: large values  
    std::vector<long long> arr6 = {100, 1, 1, 100};  
    auto r6 = bestTwoSegments(arr6, 1);  
    assert(r6.first == 1 && r6.second == 4); // 100+100=200  

    // Example 7: negative and positive mix  
    std::vector<long long> arr7 = {-2, 5, -1, 3, 7, -4};  
    auto r7 = bestTwoSegments(arr7, 2);  
    assert(r7.first == 2 && r7.second == 4); // [5,-1] sum 4 and [3,7] sum 10 = 14  

    // Example 8: single element segments  
    std::vector<long long> arr8 = {1, 2, 3, 4, 5};  
    auto r8 = bestTwoSegments(arr8, 1);  
    assert(r8.first == 4 && r8.second == 5); // 4+5=9  

    // Example 9: first segment must be before second  
    std::vector<long long> arr9 = {3, 1, 2, 1, 3};  
    auto r9 = bestTwoSegments(arr9, 1);  
    assert(r9.first == 1 && r9.second == 5); // 3+3=6  

    // Example 10: with k larger  
    std::vector<long long> arr10 = {1, 2, 3, 4, 5, 6, 7, 8};  
    auto r10 = bestTwoSegments(arr10, 3);  
    assert(r10.first == 1 && r10.second == 4); // [1,2,3]=6 and [4,5,6]=15 total 21? Wait check: actually best is [3,4,5] and [6,7,8]? Let me recompute: sums: 1-3=6, 2-4=9, 3-5=12, 4-6=15, 5-7=18, 6-8=21. Best pair max: 12+21=33 with first 3 and second 6? But we need non-overlap: first index 3 (covers 3,4,5) and second 6 (covers 6,7,8) -> 12+21=33. The pair (3,6). But also (1,5) would give 6+18=24. So correct is (3,6). So assert should be (3,6).  
    assert(r10.first == 3 && r10.second == 6);  

    return 0;  
}
