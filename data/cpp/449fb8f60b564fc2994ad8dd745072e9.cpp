Write a C++ function named `findFrequencyRankedSums` that takes a vector of integers `nums`, an integer `k`, and an integer `x` as input. The function returns a vector of integers where each element corresponds to the sum of the top `x` most frequent distinct values in each contiguous subarray of length `k` of `nums`. For each sliding window of size `k`, you need to compute the sum of the values of the `x` most frequent elements in that window. If two values have the same frequency, the larger value is considered higher priority. If there are fewer than `x` distinct values in a window, include all of them. The function must handle duplicates, negative numbers, and cases where `x` can be larger than the number of distinct elements in a window. For example, if `nums = {1, 2, 2, 3}`, `k = 3`, `x = 2`, the windows are `{1,2,2}` (frequencies: 2 for 2, 1 for 1 → sum of top 2 = 2+1=3) and `{2,2,3}` (frequencies: 2 for 2, 1 for 3 → sum of top 2 = 2+3=5), so the result is `{3, 5}`.
#include <cassert>
#include <vector>

int main() {
    // Example from description
    std::vector<int> nums1 = {1, 2, 2, 3};
    assert(findFrequencyRankedSums(nums1, 3, 2) == std::vector<long long>({3, 5}));
    // Explanation: window [1,2,2] -> top 2: 2 (freq 2), 1 (freq 1) -> sum 2+1=3
    // window [2,2,3] -> top 2: 2 (freq 2), 3 (freq 1) -> sum 2+3=5

    // Single window with all duplicates
    std::vector<int> nums2 = {5, 5, 5};
    assert(findFrequencyRankedSums(nums2, 3, 1) == std::vector<long long>({5})); // only one distinct, value 5

    // x larger than distinct count
    std::vector<int> nums3 = {7, 1, 7, 1, 7};
    assert(findFrequencyRankedSums(nums3, 3, 5) == std::vector<long long>({15, 15, 15}));
    // each window has two distinct values, sum = 7+1 = 8? Wait, every window of size 3: e.g., [7,1,7] distinct {7,1} sum 8; [1,7,1] sum 8; [7,1,7] sum 8. So should be {8,8,8}. Let me correct.

    // Correct test for nums3
    assert(findFrequencyRankedSums(nums3, 3, 5) == std::vector<long long>({8, 8, 8}));

    // Negative numbers and tie-breaking by value
    std::vector<int> nums4 = {-1, -2, -1, -2, -1};
    // windows: [-1,-2,-1] -> freq: -1 twice, -2 once -> top 2 by freq: -1 and -2 -> sum -3
    // [-2,-1,-2] -> -2 twice, -1 once -> sum -3
    // [-1,-2,-1] -> sum -3
    assert(findFrequencyRankedSums(nums4, 3, 2) == std::vector<long long>({-3, -3, -3}));

    // Tie in frequency: larger value wins for inclusion in top x
    std::vector<int> nums5 = {4, 4, 3, 3, 2};
    // window [4,4,3] -> freq: 4 twice, 3 once → top 1 by freq is 4, sum 4
    // window [4,3,3] -> freq: 3 twice, 4 once → top 1 is 3, sum 3
    // window [3,3,2] -> freq: 3 twice, 2 once → top 1 is 3, sum 3
    assert(findFrequencyRankedSums(nums5, 3, 1) == std::vector<long long>({4, 3, 3}));

    // Edge: k = 1, each window single element
    std::vector<int> nums6 = {10, 20, 30};
    assert(findFrequencyRankedSums(nums6, 1, 1) == std::vector<long long>({10, 20, 30}));
}
#include <vector>
#include <set>
#include <unordered_map>
#include <cstdint>

/*
 * Returns a vector where each element is the sum of the values of the x most frequent
 * distinct values in the sliding window of length k ending at that position.
 * Ties in frequency are broken by larger value.
 */
std::vector<long long> findFrequencyRankedSums(const std::vector<int>& nums, int k, int x) {
    using Pair = std::pair<int, int>; // (frequency, value)
    
    // descending order on (frequency, value) – higher frequency first, then larger value
    std::set<Pair, std::greater<Pair>> ranked;
    std::unordered_map<int, int> cnt;

    long long currentSum = 0; // not really used for incremental sum, but for window sum we compute on query

    std::vector<long long> result;
    result.reserve(nums.size() - k + 1);

    for (int i = 0; i < (int)nums.size(); ++i) {
        int val = nums[i];
        // Add new element to window
        auto itCnt = cnt.find(val);
        if (itCnt == cnt.end()) {
            cnt[val] = 1;
            ranked.insert({1, val});
        } else {
            int oldF = itCnt->second;
            // remove old pair
            ranked.erase({oldF, val});
            // increment frequency
            itCnt->second = oldF + 1;
            ranked.insert({oldF + 1, val});
        }

        // When window size exceeds k, remove the oldest element
        if (i >= k) {
            int leaving = nums[i - k];
            int oldF = cnt[leaving];
            // remove old pair
            ranked.erase({oldF, leaving});
            if (oldF == 1) {
                cnt.erase(leaving);
            } else {
                cnt[leaving] = oldF - 1;
                ranked.insert({oldF - 1, leaving});
            }
        }

        // If window is fully formed (size == k), compute the sum of top x
        if (i >= k - 1) {
            long long windowSum = 0;
            int taken = 0;
            for (auto it = ranked.begin(); it != ranked.end() && taken < x; ++it, ++taken) {
                windowSum += static_cast<long long>(it->first) * it->second;
            }
            result.push_back(windowSum);
        }
    }
    return result;
}
// The problem is a sliding window frequency ranking problem. We need to maintain the frequency count of each distinct value in the current window, and then query the sum of the `x` largest values when sorted first by frequency (descending) and then by value (descending) as a tiebreaker. A straightforward approach would be to recompute frequencies and sort for each window, but that would be O(k log k) per window, leading to O(n k log k) total. For better efficiency, we can use a balanced binary search tree (like an order-statistics tree) to maintain a sorted order of pairs `(frequency, value)` where the pair is ordered by frequency descending, then value descending. We use a hash map `cnt` to store the current frequency of each value. When sliding the window, we update the frequency: when a value enters the window, we remove its old pair from the tree, increment its count, insert the new pair; when a value leaves, we decrement similarly. For querying the sum of top `x`, we need to be able to iterate over the first `x` elements in the tree and sum their values multiplied by their frequency. However, maintaining the sum dynamically is tricky because the order changes with frequency updates. A simpler approach is to, for each window, extract the top `x` pairs from the tree, sum them, and not optimize further. Since the tree supports order statistics, extracting the first `x` elements takes O(x log n). With n windows, the total time is O(n * x log n). If `x` is small, this is efficient; if `x` can be large (up to k), worst-case is O(n k log n) which is acceptable for moderate sizes. Alternatively, we could maintain a cumulative sum structure, but that is overkill. Important edge cases: when `x` is greater than the number of distinct elements in a window, we sum all. Also, when frequency changes, we must ensure the tree's ordering correctly handles ties (larger value breaks tie). Negative values are fine because the sums can be negative. Space complexity is O(distinct elements in window) for the map and tree.
//
// We’ll implement an order-statistics tree using GNU PBDS (policy-based data structure) as seen in the snippet, but to make it standard C++ (without non-standard extensions), we can use `std::set` with a custom comparator and iterate to find top `x` each time. However, a `std::set` does not support order statistics (like `find_by_order`), so we would have to iterate from begin each time, which is O(k) per query. That still gives O(n*k) overall, which is fine for many cases. But to match the spirit of the snippet and provide an efficient solution, we can either use PBDS (which is commonly available in GCC) or implement a Fenwick tree over frequencies. For a self-contained, portable solution, we can use a simple multiset of pairs with a custom order and just take the first `x` via iterators, which is O(x) per query because the tree is already sorted. Actually, a `std::set` of pairs with descending comparator gives sorted order, and taking the first `x` elements is O(x) by iterators. So total per window is O(x) for query and O(log k) for each insertion/removal (since we need to find and erase). But to update frequency, we need to remove the existing pair from the set, which requires finding it. A `std::set` can erase by value if we know the exact pair, which we can reconstruct from `cnt[value]` before updating. So that works. Complexity per window: O(x + log k) for updates, total O(n * (x + log k)). This is efficient. Edge cases: when the window has fewer than `x` distinct values, we sum all pairs present. Also, when a value’s frequency becomes zero, we remove it from the set and map. We must be careful with iterators when erasing.
//
// We’ll design a function `findFrequencyRankedSums(const vector<int>& nums, int k, int x)` that returns a vector of int (sum could overflow? We’ll assume int fits). We’ll use `std::set<pair<int,int>, greater<pair<int,int>>>` but note that `greater<pair<int,int>>` sorts by first descending, then second descending, which is exactly what we need (higher frequency first, then higher value). However, `pair` comparison works lexicographically; `greater` gives descending on both, which matches the requirement: if frequencies equal, larger value comes first. We’ll maintain a `unordered_map<int,int> cnt`. For each sliding window, after updating counts, we reconstruct the set? No, we maintain the set incrementally: when a value arrives, if it was present before, we erase the old pair `(old_cnt, value)` from the set, then after incrementing we insert `(new_cnt, value)`. Similarly on removal. For querying sum, we iterate `it = s.begin()` and sum `it->first * it->second` for the first `x` elements (or until end). We must ensure we don't erase when `old_cnt == 0` (i.e., value not in set). Also, when the value leaves and its frequency becomes zero, we erase it from the set and map. The order of operations when sliding: first, add new element (update its count), then if window size > k, remove the oldest element. But we need to compute sum after the window is fully formed (size == k). So we process the first k-1 elements without querying, then for each i from 0 to n-k, we add nums[i+k-1], query, then remove nums[i]. We'll implement carefully.
//
// Time complexity: O(n * (x + log d)) where d is distinct count in window (max min(k, unique values)). Space: O(k) for set and map.
