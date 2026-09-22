// Given an array of `n` integers and a window size `k`, write a C++ function named `slidingWindowMedians` that takes a vector of integers and an integer `k`, and returns a vector containing the medians of every contiguous subarray of length `k` (i.e., each sliding window). The median is defined as the middle element when the window is sorted; if the window length is even, take the lower median (the element at position `(k-1)/2` in 0-indexed sorted order). For example, for the window `[2,1,5]`, the sorted window is `[1,2,5]` and the median is `2`. For the window `[2,1,5,4]` with `k=4`, the sorted window is `[1,2,4,5]`, and the median is `2` (the element at index `(4-1)/2 = 1`). The input array size `n` is at least 1 and `k` is between 1 and `n`. Return the medians in order from window starting at index 0 to index `n-k`.
The problem asks for sliding window medians. A naive approach would recompute the median for each window by sorting, giving `O((n-k+1) * k log k)` time, which is too slow for large inputs. A better approach maintains two multisets: a max-heap (`mx`) for the lower half of the current window and a min-heap (`mn`) for the upper half, ensuring that the size of `mx` is either equal to or one greater than the size of `mn`. This invariant makes the median always be the maximum of `mx` (since we take the lower median, and when sizes are equal, the median is the largest element of the lower half). When sliding the window, we add the new element and remove the exiting element, then rebalance the two heaps to maintain the size invariant. Rebalancing involves moving the extreme element from the larger heap to the smaller heap if the size difference becomes 2. Edge cases include `k=1` (each element is a median), duplicate values (multisets handle duplicates naturally), and when removing an element that is not in the correct heap—we locate it based on comparison with the boundary between heaps. Time complexity is `O(n log k)` for the `n` insertions and removals, each `O(log k)`, and auxiliary space is `O(k)` for the two multisets. We must be careful to handle removal correctly: if the removed element is greater than the maximum of `mx`, it belongs to `mn`, otherwise to `mx`.
#include <vector>
#include <set>
#include <cstdint>

// Returns the medians of every contiguous subarray of length k.
// The median is the lower median when k is even: element at index (k-1)/2 in sorted order.
std::vector<int> slidingWindowMedians(const std::vector<int>& arr, int k) {
    const int n = static_cast<int>(arr.size());
    std::vector<int> result;
    if (k <= 0 || k > n) return result;
    if (n == 0) return result;

    std::multiset<int> lower;  // max-heap via rbegin
    std::multiset<int> upper;  // min-heap via begin

    auto add = [&](int value) {
        if (lower.empty() || value <= *lower.rbegin()) {
            lower.insert(value);
        } else {
            upper.insert(value);
        }
        // Rebalance: lower size is either equal to upper or one greater.
        if (lower.size() > upper.size() + 1) {
            upper.insert(*lower.rbegin());
            lower.erase(std::prev(lower.end()));
        } else if (upper.size() > lower.size()) {
            lower.insert(*upper.begin());
            upper.erase(upper.begin());
        }
    };

    auto removeValue = [&](int value) {
        if (!lower.empty() && value <= *lower.rbegin()) {
            auto it = lower.find(value);
            if (it != lower.end()) {
                lower.erase(it);
            }
        } else {
            auto it = upper.find(value);
            if (it != upper.end()) {
                upper.erase(it);
            }
        }
        // Rebalance after removal.
        if (lower.size() > upper.size() + 1) {
            upper.insert(*lower.rbegin());
            lower.erase(std::prev(lower.end()));
        } else if (upper.size() > lower.size()) {
            lower.insert(*upper.begin());
            upper.erase(upper.begin());
        }
    };

    auto median = [&]() -> int {
        // Since lower has size either equal or one more than upper,
        // the lower median is the maximum of lower.
        return *lower.rbegin();
    };

    for (int i = 0; i < n; ++i) {
        add(arr[i]);
        if (i >= k) {
            removeValue(arr[i - k]);
        }
        if (i >= k - 1) {
            result.push_back(median());
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// The function is declared above; include its definition in the same translation unit.

int main() {
    // Basic odd window
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<int> res1 = slidingWindowMedians(arr1, 3);
    assert(res1 == std::vector<int>({2, 3, 4}));

    // Even window lower median
    std::vector<int> arr2 = {2, 1, 5, 4};
    std::vector<int> res2 = slidingWindowMedians(arr2, 4);
    assert(res2 == std::vector<int>({2}));

    // k = 1
    std::vector<int> arr3 = {7, -3, 0, 12};
    std::vector<int> res3 = slidingWindowMedians(arr3, 1);
    assert(res3 == std::vector<int>({7, -3, 0, 12}));

    // Duplicates
    std::vector<int> arr4 = {3, 3, 3, 3};
    std::vector<int> res4 = slidingWindowMedians(arr4, 2);
    assert(res4 == std::vector<int>({3, 3, 3}));

    // Negative numbers and unsorted
    std::vector<int> arr5 = {-5, -1, -10, 2, 8};
    std::vector<int> res5 = slidingWindowMedians(arr5, 3);
    // windows: [-5,-1,-10] sorted [-10,-5,-1] median -5; [-1,-10,2] sorted [-10,-1,2] median -1; [-10,2,8] sorted [-10,2,8] median 2
    assert(res5 == std::vector<int>({-5, -1, 2}));

    // Window same as array size
    std::vector<int> arr6 = {10, 20, 30};
    std::vector<int> res6 = slidingWindowMedians(arr6, 3);
    assert(res6 == std::vector<int>({20}));

    // Decreasing array
    std::vector<int> arr7 = {9, 8, 7, 6};
    std::vector<int> res7 = slidingWindowMedians(arr7, 2);
    // windows: [9,8] median 8; [8,7] median 7; [7,6] median 6
    assert(res7 == std::vector<int>({8, 7, 6}));

    // Large k with many duplicates
    std::vector<int> arr8 = {1, 1, 1, 2, 2, 2};
    std::vector<int> res8 = slidingWindowMedians(arr8, 4);
    // windows: [1,1,1,2] sorted [1,1,1,2] median 1; [1,1,2,2] sorted [1,1,2,2] median 1; [1,2,2,2] median 2
    assert(res8 == std::vector<int>({1, 1, 2}));

    // Single element array with k=1
    std::vector<int> arr9 = {42};
    std::vector<int> res9 = slidingWindowMedians(arr9, 1);
    assert(res9 == std::vector<int>({42}));

    return 0;
}
