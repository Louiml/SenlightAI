/*
Write a C++ function named `medianSlidingWindow` that takes a `std::vector<int>` of integers and a positive integer `k` (the window size), and returns a `std::vector<double>` containing the medians of all sliding windows of size `k`. For a window with even length, the median is the average of the two middle elements; for odd length, it is the middle element. The input vector may contain negative numbers and duplicates, and `k` will not exceed the length of the vector. The function must handle removing elements from the left as the window slides and must correctly compute medians without fully sorting each window.
*/
#include <vector>
#include <set>
#include <functional>

// Returns the medians of all sliding windows of size k over nums.
std::vector<double> medianSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<double> medians;
    std::multiset<int> minHeap;                     // larger half
    std::multiset<int, std::greater<int>> maxHeap;  // smaller half

    for (size_t i = 0; i < nums.size(); ++i) {
        // Insert new element into the appropriate heap
        if (minHeap.empty() || *minHeap.begin() <= nums[i]) {
            minHeap.insert(nums[i]);
        } else {
            maxHeap.insert(nums[i]);
        }

        // Remove element that slides out of window if window is full
        if (i >= static_cast<size_t>(k)) {
            int toRemove = nums[i - k];
            auto itMax = maxHeap.find(toRemove);
            if (itMax != maxHeap.end()) {
                maxHeap.erase(itMax);
            } else {
                auto itMin = minHeap.find(toRemove);
                minHeap.erase(itMin);
            }
        }

        // Rebalance so maxHeap has same or at most one more element than minHeap
        if (maxHeap.size() > minHeap.size()) {
            while (maxHeap.size() > minHeap.size() + 1) {
                minHeap.insert(*maxHeap.begin());
                maxHeap.erase(maxHeap.begin());
            }
        } else {
            while (minHeap.size() > maxHeap.size()) {
                maxHeap.insert(*minHeap.begin());
                minHeap.erase(minHeap.begin());
            }
        }

        // Output median once the first full window is reached
        if (i >= static_cast<size_t>(k - 1)) {
            if (maxHeap.size() == minHeap.size()) {
                medians.push_back((static_cast<double>(*maxHeap.begin()) + *minHeap.begin()) / 2.0);
            } else {
                medians.push_back(static_cast<double>(*maxHeap.begin()));
            }
        }
    }

    return medians;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic odd window
    std::vector<double> res1 = medianSlidingWindow({1, 2, 3, 4, 5}, 3);
    assert(res1.size() == 3);
    assert(res1[0] == 2.0 && res1[1] == 3.0 && res1[2] == 4.0);

    // Basic even window
    std::vector<double> res2 = medianSlidingWindow({1, 2, 3, 4}, 2);
    assert(res2.size() == 3);
    assert(res2[0] == 1.5 && res2[1] == 2.5 && res2[2] == 3.5);

    // Duplicates and negatives
    std::vector<double> res3 = medianSlidingWindow({-1, 5, 5, 3, -2}, 3);
    assert(res3.size() == 3);
    assert(std::abs(res3[0] - 5.0) < 1e-9);
    assert(std::abs(res3[1] - 5.0) < 1e-9);
    assert(std::abs(res3[2] - 3.0) < 1e-9);

    // Single element window
    std::vector<double> res4 = medianSlidingWindow({7, 10, 1}, 1);
    assert(res4.size() == 3);
    assert(res4[0] == 7.0 && res4[1] == 10.0 && res4[2] == 1.0);

    // Window equals entire array (even)
    std::vector<double> res5 = medianSlidingWindow({1, 3, 3, 6, 7}, 5);
    assert(res5.size() == 1);
    assert(std::abs(res5[0] - 3.0) < 1e-9);

    // Window equals entire array (odd)
    std::vector<double> res6 = medianSlidingWindow({1, 3, 3, 6}, 4);
    assert(res6.size() == 1);
    assert(std::abs(res6[0] - 3.0) < 1e-9);

    // Ascending values with even window
    std::vector<double> res7 = medianSlidingWindow({1, 2, 3, 4}, 3);
    assert(res7.size() == 2);
    assert(res7[0] == 2.0 && res7[1] == 3.0);

    // Descending values
    std::vector<double> res8 = medianSlidingWindow({5, 4, 3, 2, 1}, 2);
    assert(res8.size() == 4);
    assert(res8[0] == 4.5 && res8[1] == 3.5 && res8[2] == 2.5 && res8[3] == 1.5);

    // Mixed large values
    std::vector<double> res9 = medianSlidingWindow({2147483647, 2147483647, -2147483648, -2147483648}, 2);
    assert(res9.size() == 3);
    assert(res9[0] == 2147483647.0);
    assert(res9[1] == 0.0);
    assert(res9[2] == -2147483648.0);

    return 0;
}
// The problem is solved by maintaining two heaps (implemented as `std::multiset`): a max-heap (`q_max`) containing the smaller half of the current window, and a min-heap (`q_min`) containing the larger half. The invariant is that `q_max` has either the same number of elements as `q_min` or at most one more. For each new element `nums[i]`, we insert it into the appropriate heap based on its comparison with the smallest element of `q_min`. Then we remove the element that leaves the window (`nums[i-k]`) if the window size has been reached, carefully choosing which heap it belongs to using `find`. After each insertion and deletion, we rebalance the heaps so their sizes differ by at most one. Once the window is fully populated (`i >= k-1`), we compute the median: if sizes are equal, average the tops of both heaps; otherwise, the median is the top of the larger heap. Key edge cases: duplicate values must be handled correctly when removing (using `find` to erase only one occurrence), the initial window build-up before any deletions, and even/odd `k` behavior. Time complexity is O(n log k) due to `multiset` operations, and space complexity is O(k).
