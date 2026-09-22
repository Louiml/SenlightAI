// Write a C++ function `std::vector<int> minDistancesToOnes(const std::string& boxes)` that takes a string `boxes` consisting only of characters `'0'` and `'1'` (length `n`, where `1 ≤ n ≤ 10^5`). For each index `i` (0-based), the function must compute the sum of distances from `i` to every index `j` where `boxes[j] == '1'`, where the distance is `|i - j|`. Return a vector of these sums, one for each position. If there are no `'1'` characters anywhere, the result must be a vector of zeros. The solution must work efficiently for large inputs, and the function should not modify the input string.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Single zero
    assert(minDistancesToOnes("0") == std::vector<int>{0});
    // Single one
    assert(minDistancesToOnes("1") == std::vector<int>{0});
    // No ones anywhere
    assert(minDistancesToOnes("0000") == std::vector<int>({0,0,0,0}));
    // One one in the middle
    assert(minDistancesToOnes("00100") == std::vector<int>({2,1,0,1,2}));
    // One one at the far left
    assert(minDistancesToOnes("1000") == std::vector<int>({0,1,2,3}));
    // One one at the far right
    assert(minDistancesToOnes("0001") == std::vector<int>({3,2,1,0}));
    // Two ones
    assert(minDistancesToOnes("1001") == std::vector<int>({0,1,1,0}));
    // All ones
    assert(minDistancesToOnes("111") == std::vector<int>({0+2, 1+1, 2+0}) == std::vector<int>({2,2,2}));
    // Alternating
    assert(minDistancesToOnes("10101") == std::vector<int>({0+0+2, 1+0+1, 0+0+0, 1+0+1, 2+0+0}) == std::vector<int>({2,2,0,2,2}));
    // Longer random check (manually computed for "0101")
    assert(minDistancesToOnes("0101") == std::vector<int>({1+0+1, 0+0+0, 1+0+0, 0+1+0}) == std::vector<int>({2,0,1,1}));
    return 0;
}
#include <vector>
#include <string>

// For each index i, compute the sum of distances |i - j| to every index j where boxes[j]=='1'.
// Uses two passes: left-to-right and right-to-left.
std::vector<int> minDistancesToOnes(const std::string& boxes) {
    const int n = static_cast<int>(boxes.size());
    std::vector<int> ans(n, 0);

    // Forward pass: distances to '1's on the left (including at i if it's '1')
    int left_ones = 0;
    int left_sum = 0;
    for (int i = 0; i < n; ++i) {
        left_sum += left_ones;  // every previous '1' becomes 1 farther
        ans[i] += left_sum;
        if (boxes[i] == '1') {
            ++left_ones;        // this '1' contributes 0 distance to itself
        }
    }

    // Backward pass: distances to '1's on the right
    int right_ones = 0;
    int right_sum = 0;
    for (int i = n - 1; i >= 0; --i) {
        right_sum += right_ones; // every '1' to the right becomes 1 closer
        ans[i] += right_sum;
        if (boxes[i] == '1') {
            ++right_ones;
        }
    }

    return ans;
}
// The naive approach of checking all pairs `(i, j)` has O(n²) time, which is too slow for n up to 10^5. Instead, we use two passes.  
// - **Forward pass (left to right):** For each `i`, we accumulate the contribution from all `'1'` characters to the left (or at `i`). Maintain a counter `left_ones` (number of `'1'` seen so far) and a running sum `left_sum` of distances from the current position to those `'1'`s. When moving from `i-1` to `i`, every previous `'1'` becomes 1 unit farther, so `left_sum += left_ones`. If `boxes[i]=='1'`, then this position itself contributes 0, and we increment `left_ones`. Store this `left_sum` into `ans[i]`.  
// - **Backward pass (right to left):** Similarly, maintain `right_ones` and `right_sum` for `'1'`s to the right. When moving from `i+1` to `i`, every right `'1'` becomes 1 unit closer, so `right_sum += right_ones`. If `boxes[i]=='1'`, increment `right_ones`. Add this `right_sum` to `ans[i]`.  
// - **Edge cases:** Empty string? The problem guarantees at least length 1. No `'1'`s? Both passes will keep sums at 0, so all zeros. All `'1'`s? The sums will be triangular numbers: for index `i`, sum left distances + sum right distances = `i*(i+1)/2 + (n-1-i)*(n-1-i+1)/2`. Our algorithm handles it naturally.  
// - **Complexity:** Each pass is O(n), total O(n) time and O(n) auxiliary space for the result vector (no extra data structures beyond a few counters).
