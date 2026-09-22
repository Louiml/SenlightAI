// Write a C++ function `long long markAndScore(const std::vector<int>& nums)` that simulates the following process on an array of non-negative integers. Repeatedly select the smallest value in the current array; if there are ties, select the one with the smallest index. Add that value to a running total score, then mark that element and its immediate neighbors (if they exist) as "removed" so that they cannot be selected again in future iterations. Continue until all elements are removed. Return the final total score. The input array can be empty (return 0), may contain duplicate values, and indices are 0-based. The function must not modify the input vector; instead, it should work on an internal copy. The total score may exceed 32-bit range, so use `long long`.

The optimal approach is to use a min-heap (priority queue) that stores pairs of `(value, index)`, ordered first by value and then by index (which naturally happens with `std::pair` comparisons). Push all elements onto the heap. Then repeatedly pop the top element. If the element at that index has already been marked as removed (by using a sentinel like `-1` in a copied array), skip it. Otherwise, add its value to the answer, mark it and its left and right neighbors as removed (by setting their values to `-1` in the copy). This greedy strategy works because at every step we pick the globally smallest remaining element, and marking neighbors is deterministic. Edge cases include: empty input (return 0), a single element (score is that element), and elements with no left or right neighbor at boundaries. The main complexity is `O(n log n)` due to heap operations, and `O(n)` auxiliary space for the heap and the copied array.

#include <vector>
#include <queue>
#include <utility>

// Simulate the marking process and return the total score.
long long markAndScore(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    // Copy to allow marking without modifying the input.
    std::vector<int> arr = nums;
    long long ans = 0;

    // Min-heap: ordered by value, then by index.
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        pq.push({arr[i], i});
    }

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        int val = cur.first;
        int idx = cur.second;

        // Skip if already removed.
        if (arr[idx] != -1) {
            ans += val;
            arr[idx] = -1;
            // Mark left neighbor.
            if (idx - 1 >= 0) {
                arr[idx - 1] = -1;
            }
            // Mark right neighbor.
            if (idx + 1 < static_cast<int>(arr.size())) {
                arr[idx + 1] = -1;
            }
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

int main() {
    // Empty input.
    assert(markAndScore({}) == 0);

    // Single element.
    assert(markAndScore({5}) == 5);

    // Basic example from the snippet: [2,1,3,4,5,2].
    // Step 1: pick value 1 at idx 1 (score 1), mark indices 0,1,2.
    // Remaining: [ -1, -1, -1, 4, 5, 2 ]
    // Step 2: pick value 2 at idx 5 (score 2), mark indices 4,5.
    // Remaining: [ -1, -1, -1, 4, -1, -1 ]
    // Step 3: pick value 4 at idx 3 (score 4). All removed. Total = 7.
    assert(markAndScore({2,1,3,4,5,2}) == 7);

    // All equal values: [1,1,1].
    // Step 1: pick idx 0 (value 1), mark idx 0 and 1.
    // Remaining: [ -1, -1, 1 ]
    // Step 2: pick idx 2 (value 1), total = 2.
    assert(markAndScore({1,1,1}) == 2);

    // Already removed scenario due to neighbors: [10,1,100].
    // Step 1: pick idx 1 (value 1), mark indices 0,1,2. Total = 1.
    assert(markAndScore({10,1,100}) == 1);

    // Large values to check long long.
    assert(markAndScore({1000000000, 1000000000}) == 1000000000); // both marked in one step

    // Pattern with gaps: [3,4,5,6].
    // Step 1: pick idx 0 (value 3), mark idx 0 and 1 -> [ -1, -1, 5, 6 ]
    // Step 2: pick idx 2 (value 5), mark idx 2 and 3 -> total 8.
    assert(markAndScore({3,4,5,6}) == 8);

    // Strictly increasing, all selected: [1,2,3].
    // Step 1: pick idx 0 (1), mark 0 and 1 -> [ -1, -1, 3 ]
    // Step 2: pick idx 2 (3), total 4.
    assert(markAndScore({1,2,3}) == 4);

    // Duplicate values with tie-breaking by index: [2,5,2].
    // Step 1: pick idx 0 (2) (since idx 2 also has 2 but idx 0 is smaller), mark idx 0 and 1.
    // Remaining: [ -1, -1, 2 ]
    // Step 2: pick idx 2 (2), total 4.
    assert(markAndScore({2,5,2}) == 4);
    return 0;
}
