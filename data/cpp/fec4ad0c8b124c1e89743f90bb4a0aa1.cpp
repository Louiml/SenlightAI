You are given a sequence of positive integers. Write a C++ function that processes these integers one by one using a multiset to simulate a system of piles: for each new integer `x`, find the smallest existing pile-top value that is strictly greater than `x`. If such a pile top exists, replace that pile top with `x` (effectively removing the old top and inserting `x`). If no such pile top exists (i.e., `x` is greater than or equal to all current pile tops), start a new pile with `x`. After processing all integers, return the total number of piles. The input is an array of distinct or duplicate positive integers, and the function must handle duplicates correctly. The output is a single integer representing the final pile count.

#include <cassert>
#include <vector>

int countPiles(const std::vector<int>& nums);

int main() {
    // Single element
    assert(countPiles({5}) == 1);

    // Empty input
    assert(countPiles({}) == 0);

    // Already increasing: each element must go on a new pile? Actually:
    // 1 -> pile {1}, 2 -> no top >2? Actually top=1, upper_bound(2)=end, new pile. 
    // So strictly increasing gives pile count = n.
    assert(countPiles({1, 2, 3}) == 3);

    // All equal: each new element cannot be placed (upper_bound skips equal), so new piles.
    assert(countPiles({7, 7, 7}) == 3);

    // Typical case: sequence like 5, 2, 4, 1, 3
    // 5 -> pile [5]
    // 2 -> upper_bound(2) finds 5, replace -> pile [2]
    // 4 -> upper_bound(4) no top >4? top=2, none, so new pile -> [2] [4]
    // 1 -> upper_bound(1) finds 2, replace -> [1] [4]
    // 3 -> upper_bound(3) finds 4, replace -> [1] [3]
    // Final piles = 2
    assert(countPiles({5, 2, 4, 1, 3}) == 2);

    // Duplicates mixed
    // 3, 3, 1, 2, 3
    // 3 -> [3]
    // 3 -> upper_bound(3)=end, new pile -> [3][3]
    // 1 -> upper_bound(1) finds 3 (first), replace -> [1][3]
    // 2 -> upper_bound(2) finds 3 (second), replace -> [1][2]
    // 3 -> upper_bound(3)=end, new pile -> [1][2][3] => 3 piles
    assert(countPiles({3, 3, 1, 2, 3}) == 3);

    // Large numbers
    assert(countPiles({1000000, 1, 999999, 2}) == 2);

    // A classic non-decreasing covering example: {2, 2, 2} all equal -> 3 piles
    assert(countPiles({2, 2, 2}) == 3);

    return 0;
}

#include <vector>
#include <set>

// Given a vector of positive integers, simulate pile placement using a multiset.
// Returns the number of piles after processing all elements.
int countPiles(const std::vector<int>& nums) {
    std::multiset<int> pileTops;  // stores the top value of each pile

    for (int x : nums) {
        auto it = pileTops.upper_bound(x);  // smallest top > x
        if (it == pileTops.end()) {
            pileTops.insert(x);  // no suitable pile, start new one
        } else {
            pileTops.erase(it);  // replace old top
            pileTops.insert(x);  // new top
        }
    }
    return static_cast<int>(pileTops.size());
}

// This problem is equivalent to finding the minimum number of strictly increasing subsequences required to cover the sequence (a variant of patience sorting for non-decreasing subsequences). The multiset stores the current top value of each pile. For each incoming `x`, we use `upper_bound(x)` to locate the smallest pile top that is strictly greater than `x`. If such a pile exists, we replace that top with `x` (since `x` can be placed on that pile and becomes the new top, keeping the pile valid). If no such pile exists (i.e., `x` is ≥ all pile tops), we start a new pile, so we insert `x` as a new top. This greedy strategy is optimal because placing `x` on the smallest possible larger top minimizes the growth of pile counts. Duplicates are handled naturally: if `x` equals some pile top, `upper_bound` skips that equal value, so `x` cannot be placed on that pile and must either go onto a larger top or start a new pile. Edge cases: an empty input (return 0), a single element (return 1), and all equal elements (each new element starts a new pile because no top is strictly greater). Time complexity is O(n log n) due to each multiset insertion/erasure/log lookup; space is O(n) for the multiset.
