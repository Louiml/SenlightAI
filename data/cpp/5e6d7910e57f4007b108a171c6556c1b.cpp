/*
Write a C++ function `containsNearbyAlmostDuplicate` that takes a vector of integers `nums`, an integer `k`, and an integer `t`, and returns a boolean indicating whether there exist two indices `i` and `j` such that `i != j`, the absolute difference between `nums[i]` and `nums[j]` is at most `t`, and the absolute difference between `i` and `j` is at most `k`. The function must handle large value ranges (use `long long` for difference calculations to avoid overflow) and must be efficient for inputs up to 10^5 elements. The algorithm must use merge sort to sort the array while preserving original indices, then check nearby values in sorted order. Edge cases include empty arrays, single elements, `t` or `k` being zero, negative numbers, and duplicates.
*/
#include <vector>
#include <cstdlib>
#include <cmath>

struct Node {
    int val;
    int pos;
};

// Merge two sorted subarrays of nodes, sorting by val.
void merge(std::vector<Node>& nodes, int left, int mid, int right) {
    std::vector<Node> leftPart;
    for (int i = left; i <= mid; ++i) {
        leftPart.push_back(nodes[i]);
    }
    int i = 0;
    int j = mid + 1;
    int k = left;
    while (i < static_cast<int>(leftPart.size())) {
        if (j == right + 1 || leftPart[i].val <= nodes[j].val) {
            nodes[k++] = leftPart[i++];
        } else {
            nodes[k++] = nodes[j++];
        }
    }
}

// Recursive merge sort on nodes sorted by val.
void mergeSort(std::vector<Node>& nodes, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(nodes, left, mid);
    mergeSort(nodes, mid + 1, right);
    merge(nodes, left, mid, right);
}

// Returns true if there exist two indices with |nums[i] - nums[j]| <= t and |i - j| <= k.
bool containsNearbyAlmostDuplicate(const std::vector<int>& nums, int k, int t) {
    int n = static_cast<int>(nums.size());
    if (n < 2) {
        return false;
    }
    std::vector<Node> nodes;
    nodes.reserve(n);
    for (int i = 0; i < n; ++i) {
        Node node = {nums[i], i};
        nodes.push_back(node);
    }
    mergeSort(nodes, 0, n - 1);
    for (int i = 1; i < n; ++i) {
        for (int j = i - 1; j >= 0; --j) {
            long long diff = static_cast<long long>(nodes[i].val) - static_cast<long long>(nodes[j].val);
            if (diff <= static_cast<long long>(t)) {
                if (std::abs(nodes[i].pos - nodes[j].pos) <= k) {
                    return true;
                }
            } else {
                break;
            }
        }
    }
    return false;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(containsNearbyAlmostDuplicate({1,2,3,1}, 3, 0) == true);
    assert(containsNearbyAlmostDuplicate({1,0,1,1}, 1, 2) == true);
    assert(containsNearbyAlmostDuplicate({1,5,9,1,5,9}, 2, 3) == false);
    // Empty and single element
    assert(containsNearbyAlmostDuplicate({}, 1, 1) == false);
    assert(containsNearbyAlmostDuplicate({5}, 1, 1) == false);
    // Negative and large values
    assert(containsNearbyAlmostDuplicate({-3, 3}, 1, 0) == false);
    assert(containsNearbyAlmostDuplicate({-3, -1}, 1, 2) == true);
    // Duplicates
    assert(containsNearbyAlmostDuplicate({1, 1}, 0, 0) == true);
    assert(containsNearbyAlmostDuplicate({1, 100, 1}, 0, 0) == false);
    // Overflow check: values near INT_MAX
    assert(containsNearbyAlmostDuplicate({2147483647, -2147483648}, 1, 4294967295LL) == true);
    // t zero but positions far
    assert(containsNearbyAlmostDuplicate({1, 2, 1}, 1, 0) == true);
    assert(containsNearbyAlmostDuplicate({1, 2, 3, 4}, 1, 0) == false);
    return 0;
}
// The solution sorts the array values while tracking their original positions using a custom `node` struct containing `val` and `pos`. Merge sort is implemented to sort these nodes by value ascending, while preserving each value’s original index. After sorting, the array is ordered by value, so for any sorted position `i`, the values that are close in magnitude are located near it. We iterate from `i = 1` to `n-1`, moving backwards with `j` while `nodvec[i].val - nodvec[j].val <= t` (since the array is sorted, once the difference exceeds `t`, we break). For each candidate `j`, we check if `abs(nodvec[i].pos - nodvec[j].pos) <= k`; if so, return true immediately. The algorithm avoids checking pairs with large value differences, making it efficient. In the worst case, if `t` is very large, the inner loop may check many pairs, but for typical constraints with small `t`, it runs near linearly. The time complexity is O(n log n) for the merge sort, plus O(n * m) for the check phase, where m is the average number of values within difference t; in the worst case m = n, giving O(n^2). Space complexity is O(n) for the merge sort auxiliary array.
