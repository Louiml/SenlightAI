/*
Write a C++ function `largestDivisibleSubset` that takes a vector of positive integers and returns a vector containing one valid largest subset where every pair of elements divides one another (i.e., for any two numbers `a` and `b` in the subset, either `a % b == 0` or `b % a == 0`). If multiple such subsets exist with the same maximum size, any one of them may be returned. The input vector may be unsorted, contain duplicates, and have any length from 1 to 10^5. The function should return the subset in non-decreasing order.
*/

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns a largest subset of arr where every pair of elements divides one another.
std::vector<int> largestDivisibleSubset(const std::vector<int>& arr) {
    if (arr.empty()) {
        return {};
    }

    int n = static_cast<int>(arr.size());
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    std::vector<int> dp(n, 1);
    std::vector<int> hash(n);
    for (int i = 0; i < n; ++i) {
        hash[i] = i;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (sorted[i] % sorted[j] == 0 && 1 + dp[j] > dp[i]) {
                dp[i] = 1 + dp[j];
                hash[i] = j;
            }
        }
    }

    int best_len = 0;
    int last_index = 0;
    for (int i = 0; i < n; ++i) {
        if (dp[i] > best_len) {
            best_len = dp[i];
            last_index = i;
        }
    }

    std::vector<int> result;
    int cur = last_index;
    while (true) {
        result.push_back(sorted[cur]);
        if (hash[cur] == cur) {
            break;
        }
        cur = hash[cur];
    }

    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic case with a clear chain
    std::vector<int> arr1 = {1, 2, 4, 8};
    auto res1 = largestDivisibleSubset(arr1);
    assert(res1.size() == 4);
    assert(std::is_sorted(res1.begin(), res1.end()));
    for (size_t i = 0; i < res1.size(); ++i) {
        for (size_t j = i + 1; j < res1.size(); ++j) {
            assert(res1[j] % res1[i] == 0);
        }
    }

    // Unsorted input
    std::vector<int> arr2 = {3, 9, 1, 27};
    auto res2 = largestDivisibleSubset(arr2);
    assert(res2.size() == 4 || res2.size() == 3); // e.g., {1,3,9,27} or {1,3,9}

    // Single element
    std::vector<int> arr3 = {7};
    auto res3 = largestDivisibleSubset(arr3);
    assert(res3.size() == 1 && res3[0] == 7);

    // Duplicates
    std::vector<int> arr4 = {2, 2, 2};
    auto res4 = largestDivisibleSubset(arr4);
    assert(res4.size() == 3);

    // No divisibility between distinct numbers
    std::vector<int> arr5 = {5, 7, 11};
    auto res5 = largestDivisibleSubset(arr5);
    assert(res5.size() == 1);

    // Mixed case
    std::vector<int> arr6 = {4, 8, 6, 12, 24};
    auto res6 = largestDivisibleSubset(arr6);
    assert(res6.size() >= 3);

    // Larger chain
    std::vector<int> arr7 = {2, 4, 6, 8, 16, 32};
    auto res7 = largestDivisibleSubset(arr7);
    assert(res7.size() == 5); // {2,4,8,16,32}

    // Empty input
    std::vector<int> arr8 = {};
    auto res8 = largestDivisibleSubset(arr8);
    assert(res8.empty());

    return 0;
}

// The problem is a classic Longest Increasing Subsequence (LIS) variant, but instead of comparing by value, we compare by divisibility. First, sort the input array in ascending order because if `a < b` and `a` divides `b`, then any element that divides `a` will also divide `b` if the divisibility chain is consistent; sorting ensures we only need to check `arr[i] % arr[j] == 0` for `j < i`. We maintain two arrays: `dp[i]` storing the length of the largest valid subset ending with `arr[i]`, and `hash[i]` storing the index of the previous element in that subset (or `i` itself if it's the start). Initialize `dp[i] = 1` and `hash[i] = i` for all `i`. For each `i`, iterate over all `j < i`; if `arr[i] % arr[j] == 0` and `1 + dp[j] > dp[i]`, then update `dp[i] = 1 + dp[j]` and `hash[i] = j`. After filling `dp`, find the index `lastIndex` with the maximum `dp` value. Then reconstruct the subset by tracing back through `hash` from `lastIndex` until reaching an index where `hash[idx] == idx`, collecting elements, and finally reverse the collected vector to get ascending order. Edge cases: duplicates in input are handled naturally because a number can divide itself; if the input has a single element, the function returns that element; if all numbers are pairwise non-divisible, the function returns a vector with a single smallest element (any element works). Time complexity is O(n^2) due to the double loop, and space complexity is O(n) for the `dp`, `hash`, and result vectors.
