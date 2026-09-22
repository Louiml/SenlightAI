// You are given an array of `n` positive integers, each representing the number of candies in a box, and an integer `k` representing the number of children. You need to write a function `long long maxCandiesPerChild(const std::vector<long long>& boxes, int k)` that returns the maximum number of candies that can be distributed **exactly equally** to each of the `k` children, where each child receives the same integer amount, and all candies used must come from the boxes. Each box can be used to provide candies to multiple children, but each time you take `candiesPerChild` from a box, that amount must be a full portion; leftover candies in a box after taking as many full portions as possible are discarded (cannot be combined with leftovers from other boxes). The goal is to maximize the number of candies per child such that at least `k` full portions can be obtained. Return the maximum possible value. If no positive integer works, return 0. The input boxes may be unsorted and contain duplicates.
// This problem is a classic "maximum feasible value" problem, solvable with binary search on the answer. Let `candiesPerChild` be the candidate value. For each box, the number of children that can be served from that box is `boxes[i] / candiesPerChild` (integer division). Sum these counts across all boxes; if the total is at least `k`, the candidate is feasible. Since the function is monotonic (if `x` is feasible, then any smaller value is also feasible), we can binary search the answer. The search range is from `1` to `max(boxes)` (since no child can receive more than the largest single box). Initially, we set low = 1, high = maxBoxes, and while low <= high, we check mid. If feasible, we record mid as a candidate answer and move low = mid+1 to try larger values; otherwise, we move high = mid-1. Edge cases: if `k` is larger than the total number of candies, the answer is 0 (because no positive integer can yield enough portions). Also, if `boxes` is empty or `k` is zero, return 0. The time complexity is O(n log M) where M is the maximum box value, due to sorting not needed (we only need max), and each feasibility check is O(n). Space complexity is O(1) besides the input vector.
#include <vector>
#include <algorithm>

// Returns the maximum number of candies each of k children can receive,
// where each portion is taken as a whole from a single box.
long long maxCandiesPerChild(const std::vector<long long>& boxes, int k) {
    if (boxes.empty() || k <= 0) {
        return 0;
    }
    long long maxBox = *std::max_element(boxes.begin(), boxes.end());
    long long low = 1;
    long long high = maxBox;
    long long answer = 0;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long count = 0;
        for (long long box : boxes) {
            count += box / mid;
            if (count >= k) {
                break; // early exit
            }
        }
        if (count >= k) {
            answer = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxCandiesPerChild({10, 20, 30}, 3) == 20);
    assert(maxCandiesPerChild({5, 5, 5}, 3) == 5);
    assert(maxCandiesPerChild({5, 5, 5}, 6) == 2);
    // Large k, impossible
    assert(maxCandiesPerChild({1, 1}, 5) == 0);
    // Empty or zero k
    assert(maxCandiesPerChild({}, 3) == 0);
    assert(maxCandiesPerChild({10, 20}, 0) == 0);
    // Single box, multiple children
    assert(maxCandiesPerChild({100}, 10) == 10);
    // Duplicates and unsorted
    assert(maxCandiesPerChild({7, 3, 7, 7}, 3) == 7);
    // Edge: all ones
    assert(maxCandiesPerChild({1, 1, 1, 1}, 4) == 1);
    // Larger numbers
    assert(maxCandiesPerChild({1000000000LL, 1000000000LL}, 2) == 1000000000LL);
    return 0;
}
