Write a C++ function that takes a vector of integers and a non-negative integer `k`, and returns the k-th smallest distinct element in the vector (0-indexed). If `k` is out of range (i.e., >= number of distinct elements), return `-1`. The vector may contain duplicates, negative numbers, and unsorted input. The function should handle empty vectors gracefully. Do not modify the input vector.

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic distinct values
    assert(kthSmallestDistinct({5, 1, 2}, 1) == 2);
    assert(kthSmallestDistinct({5, 1, 2}, 0) == 1);
    assert(kthSmallestDistinct({5, 1, 2}, 2) == 5);

    // Duplicates ignored
    assert(kthSmallestDistinct({3, 3, 3, 1, 2}, 1) == 2);
    assert(kthSmallestDistinct({3, 3, 3, 1, 2}, 0) == 1);
    assert(kthSmallestDistinct({3, 3, 3, 1, 2}, 2) == 3);

    // Negative numbers and unsorted
    assert(kthSmallestDistinct({-5, -10, 0, -5, 7}, 0) == -10);
    assert(kthSmallestDistinct({-5, -10, 0, -5, 7}, 2) == 7);

    // Out-of-range k
    assert(kthSmallestDistinct({4, 2, 9}, 3) == -1);
    assert(kthSmallestDistinct({4, 2, 9}, 10) == -1);

    // Empty vector
    assert(kthSmallestDistinct({}, 0) == -1);

    // Single element
    assert(kthSmallestDistinct({42}, 0) == 42);
    assert(kthSmallestDistinct({42}, 1) == -1);

    // All duplicates single distinct value
    assert(kthSmallestDistinct({7, 7, 7}, 0) == 7);
    assert(kthSmallestDistinct({7, 7, 7}, 1) == -1);

    return 0;
}

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <vector>

using namespace __gnu_pbds;
using ordered_set = tree<int, null_type, std::less<int>, rb_tree_tag, tree_order_statistics_node_update>;

// Returns the k-th smallest distinct element (0-indexed), or -1 if k is out of range.
int kthSmallestDistinct(const std::vector<int>& nums, int k) {
    ordered_set distinct_values;
    for (int value : nums) {
        distinct_values.insert(value);
    }
    if (k < 0 || k >= static_cast<int>(distinct_values.size())) {
        return -1;
    }
    return *distinct_values.find_by_order(k);
}

// The task requires finding the k-th smallest distinct value. The most straightforward approach is to insert all unique values into an ordered set (or a balanced BST) that supports order statistics. An ordered set (e.g., GNU PBDS tree) or a `std::set` combined with `std::advance` can be used, but the PBDS ordered_set provides `find_by_order(k)` in O(log n) time after insertion, which is more efficient than `std::advance` (O(n) worst-case). Since we need distinct elements, we insert each element from the vector into the ordered set, naturally removing duplicates. Then, if `k` is less than the size of the set, return `*set.find_by_order(k)`, otherwise return -1. Edge cases: empty vector → return -1; k equal to set size or larger → return -1; negative k → treat as invalid (since k is non-negative, but we can add a check). Time complexity: O(n log n) for inserting n elements, O(log n) for the query. Space complexity: O(n) for the set.
