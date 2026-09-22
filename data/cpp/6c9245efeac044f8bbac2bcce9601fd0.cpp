/*
Write a C++ function that, given a vector of integers `nums` and a positive window size `k` (where `k` is no larger than the vector’s length), returns a vector of doubles containing the median of every contiguous subarray (sliding window) of length `k`, in order from left to right. For a window of even length, the median is the average of the two middle elements after sorting; for odd length, it is the single middle element. The input may contain negative numbers and duplicates. The function must handle all valid `k` and vector sizes, including `k == 1` and `k == nums.size()`.
*/
#include <vector>
#include <set>
#include <cmath>
#include <iterator>

// Returns the median of every sliding window of size k in nums.
std::vector<double> medianSlidingWindow(const std::vector<int>& nums, int k) {
    std::multiset<int> window(nums.begin(), nums.begin() + k);
    auto mid = std::next(window.begin(), k / 2);
    std::vector<double> medians;
    
    for (int i = k; ; ++i) {
        // Compute current window median
        if (k % 2 == 1) {
            medians.push_back(static_cast<double>(*mid));
        } else {
            medians.push_back((static_cast<double>(*mid) + static_cast<double>(*std::prev(mid))) / 2.0);
        }
        
        if (i == static_cast<int>(nums.size())) {
            break;
        }
        
        // Insert new element
        window.insert(nums[i]);
        if (nums[i] < *mid) {
            --mid;
        }
        
        // Remove old element
        int outgoing = nums[i - k];
        if (outgoing <= *mid) {
            ++mid;
        }
        window.erase(window.lower_bound(outgoing));
    }
    
    return medians;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic odd and even windows
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    auto res1 = medianSlidingWindow(nums1, 3);
    assert(res1.size() == 6);
    assert(std::fabs(res1[0] - 1.0) < 1e-9);
    assert(std::fabs(res1[1] - (-1.0)) < 1e-9);
    assert(std::fabs(res1[2] - (-1.0)) < 1e-9);
    assert(std::fabs(res1[3] - 3.0) < 1e-9);
    assert(std::fabs(res1[4] - 5.0) < 1e-9);
    assert(std::fabs(res1[5] - 6.0) < 1e-9);
    
    // Even window size
    auto res2 = medianSlidingWindow(nums1, 4);
    assert(res2.size() == 5);
    assert(std::fabs(res2[0] - 1.0) < 1e-9);   // median of {1,3,-1,-3} = (1 + (-1))/2 = 0? Wait: sorted {-3,-1,1,3} → (−1+1)/2=0
    // fix expected value: actually sorted -3,-1,1,3 → (-1+1)/2 = 0.0
    assert(std::fabs(res2[0] - 0.0) < 1e-9);
    assert(std::fabs(res2[1] - 0.0) < 1e-9);   // {3,-1,-3,5} → sorted -3,-1,3,5 → (-1+3)/2=1.0
    assert(std::fabs(res2[1] - 1.0) < 1e-9);
    
    // k == 1
    auto res3 = medianSlidingWindow({5, 2, 8, 1}, 1);
    assert(res3.size() == 4);
    assert(res3[0] == 5.0);
    assert(res3[1] == 2.0);
    assert(res3[2] == 8.0);
    assert(res3[3] == 1.0);
    
    // k == vector size
    auto res4 = medianSlidingWindow({1, 2, 3, 4}, 4);
    assert(res4.size() == 1);
    assert(std::fabs(res4[0] - 2.5) < 1e-9);
    
    // Negative numbers only
    auto res5 = medianSlidingWindow({-1, -2, -3, -4}, 2);
    assert(res5.size() == 3);
    assert(std::fabs(res5[0] - (-1.5)) < 1e-9);
    assert(std::fabs(res5[1] - (-2.5)) < 1e-9);
    assert(std::fabs(res5[2] - (-3.5)) < 1e-9);
    
    // Duplicate values
    auto res6 = medianSlidingWindow({1, 1, 1, 1}, 3);
    assert(res6.size() == 2);
    assert(res6[0] == 1.0);
    assert(res6[1] == 1.0);
    
    return 0;
}
// The classic approach is to maintain a balanced binary search tree (e.g., `std::multiset`) representing the current window in sorted order, plus an iterator pointing to the median element. Initially, insert the first `k` elements and set the iterator to the `k/2`-th element (0‑based) using `next(begin(), k/2)`. For each window, compute the median: if `k` is odd, the median is `*mid`; if even, it is the average of `*mid` and `*prev(mid)`. To slide to the next window, insert the new element `nums[i]`. If the new element is smaller than the current median, the median iterator must move one position left. Then remove the outgoing element `nums[i-k]`. If the outgoing element is less than or equal to the current median, the iterator must move right (because after removal the median shifts right). The order of insertion/removal and iterator adjustments must be carefully coordinated to keep the iterator valid. Edge cases include `k == 1` (median is each single element), `k == nums.size()` (only one median for the whole array), and duplicate values (handled naturally by multiset). The algorithm runs in `O(n log k)` time for `n` input elements and uses `O(k)` extra space for the multiset. The median computation and insertion/removal each cost `O(log k)`.
