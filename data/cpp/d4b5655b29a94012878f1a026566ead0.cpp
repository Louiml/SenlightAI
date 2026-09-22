// Write a C++ function that, given a non-empty vector of integers `nums` and a positive integer window size `k` (where `k <= nums.size()`), returns a vector of doubles containing the median of each contiguous subarray (sliding window) of length `k` as the window moves from the leftmost position to the rightmost position. For an even-sized window, the median is defined as the average of the two middle values after sorting the window; for an odd-sized window, it is the middle element. The function must handle negative numbers, duplicate values, and windows of size 1. The implementation must be efficient enough to support `nums.size()` up to 10^5 and `k` up to 10^5, so a naive re‑sorting of each window is not acceptable.

The core challenge is to maintain the median of a dynamic multiset as elements are added and removed. A common approach uses two heaps: a max‑heap (`low`) storing the smaller half of the current window and a min‑heap (`high`) storing the larger half. The invariant ensures that `low.size()` is either equal to `high.size()` or exactly one larger. When adding a new element, insert it into the appropriate heap based on comparison with `low.top()`, then re‑balance if sizes violate the invariant. When removing an element that scrolls out of the window, we must efficiently delete an arbitrary value from a heap. Since standard heaps do not support arbitrary deletion, we use a lazy deletion technique: maintain `std::unordered_map<int,int>` counts of elements scheduled for removal (or build a temporary heap as in the original snippet, but that is O(k) per removal). For a clean O(log k) removal, we can use a `std::multiset` instead of two heaps, since `multiset` supports insertion, deletion, and iteration to find the median in O(log k). The algorithm then processes each window as follows: add the new element, remove the outgoing element, and compute the median by advancing an iterator to the middle. Time complexity is O(n log k) overall, with O(k) space. Edge cases include `k == 1` (median is the element itself), duplicate values (handled naturally by `multiset`), and negative numbers (comparison works as usual).

#include <vector>
#include <set>
#include <iterator>

// Returns the medians of all sliding windows of size k in nums.
std::vector<double> slidingWindowMedians(const std::vector<int>& nums, int k) {
    std::vector<double> result;
    std::multiset<int> window;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // Add current element to window
        window.insert(nums[i]);

        // Remove element that left the window
        if (i >= k) {
            window.erase(window.find(nums[i - k]));
        }

        // When window has size k, compute median
        if (i >= k - 1) {
            auto it = window.begin();
            std::advance(it, k / 2);
            if (k % 2 == 1) {
                result.push_back(*it);
            } else {
                auto it2 = it;
                --it2;
                result.push_back((static_cast<double>(*it2) + *it) / 2.0);
            }
        }
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above (included via header or copied).
// Here we just provide the test harness.
int main() {
    // Test case 1: classical example with k=4
    {
        std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
        std::vector<double> medians = slidingWindowMedians(nums, 4);
        std::vector<double> expected = {0.0, 1.0, 2.0, 3.0, 4.5, 6.0};
        assert(medians.size() == expected.size());
        for (size_t i = 0; i < medians.size(); ++i) {
            assert(std::fabs(medians[i] - expected[i]) < 1e-9);
        }
    }

    // Test case 2: k=1, each element is its own median
    {
        std::vector<int> nums = {-5, 0, 3, -2, 8};
        std::vector<double> medians = slidingWindowMedians(nums, 1);
        std::vector<double> expected = {-5.0, 0.0, 3.0, -2.0, 8.0};
        assert(medians == expected);
    }

    // Test case 3: k equals the full array size (one window)
    {
        std::vector<int> nums = {2, 4, 6, 8};
        std::vector<double> medians = slidingWindowMedians(nums, 4);
        assert(medians.size() == 1);
        assert(std::fabs(medians[0] - 5.0) < 1e-9); // (4+6)/2 = 5
    }

    // Test case 4: duplicates and negative numbers
    {
        std::vector<int> nums = {-1, -1, 5, 5, 3};
        std::vector<double> medians = slidingWindowMedians(nums, 3);
        // Windows: [-1,-1,5] -> -1; [-1,5,5] -> 5; [5,5,3] -> 5
        std::vector<double> expected = {-1.0, 5.0, 5.0};
        assert(medians == expected);
    }

    // Test case 5: large values to ensure double conversion works correctly
    {
        std::vector<int> nums = {1000000, -1000000, 0};
        std::vector<double> medians = slidingWindowMedians(nums, 2);
        // Windows: [1000000,-1000000] -> 0.0; [-1000000,0] -> -500000.0
        std::vector<double> expected = {0.0, -500000.0};
        assert(medians.size() == expected.size());
        for (size_t i = 0; i < medians.size(); ++i) {
            assert(std::fabs(medians[i] - expected[i]) < 1e-9);
        }
    }

    return 0;
}
