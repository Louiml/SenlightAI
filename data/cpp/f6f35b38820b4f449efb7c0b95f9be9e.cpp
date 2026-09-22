Given two non-empty vectors `nums1` and `nums2` containing non-negative integers, and an integer `k` where `0 < k <= nums1.size() + nums2.size()`, write a C++ function that returns a vector of length `k` representing the maximum possible number that can be formed by selecting digits from both vectors while preserving the relative order of digits within each original vector. The result must be lexicographically largest among all possible selections of exactly `k` digits. Example: given `nums1 = [3, 4, 6, 5]`, `nums2 = [9, 1, 2, 5, 8, 3]`, and `k = 5`, the function should return `[9, 8, 6, 5, 3]`. The selection can take any number of digits from each array (including zero or all), but the total taken must be exactly `k`. Digits within each array must appear in the same relative order as in the original. The solution must handle duplicates and varying array sizes, ensuring the result is unique even when multiple selections produce the same numeric sequence (all equally valid, return any). The function signature is `std::vector<int> maxNumber(const std::vector<int>& nums1, const std::vector<int>& nums2, int k)`.

The core challenge is combining two subproblems: (1) for a given array and a chosen length `len`, compute the maximum subsequence of that exact length (while preserving order), and (2) merge two such subsequences (from `nums1` and `nums2`) to form the largest possible combined sequence of length `k`. The first subproblem is solved by a greedy stack-based approach: iterate through the array, and before pushing a digit, while the stack is non-empty, the next digit is greater than the top, and remaining digits plus stack size allow reaching the target length, pop. This ensures the largest lexicographic subsequence of the required length. For the merge step, we use a two-pointer comparison that always picks the larger digit from the front of either subsequence; if the digits are equal, we compare the remaining suffixes lexicographically to decide which to take, ensuring the optimal merge. The overall algorithm iterates over all possible split lengths `i` from `max(0, k - nums2.size())` to `min(k, nums1.size())`. For each split, compute the maximum subsequence of length `i` from `nums1` and `k-i` from `nums2`, merge them, and keep the lexicographically largest result. Edge cases include when one array is empty, when `k` equals the total size, and when there are many duplicate digits—the merge must handle ties correctly by comparing beyond the equal digits. Time complexity is O((nums1.size()+nums2.size()) * k) in the worst case because for each split we do O(k) work for subsequence extraction and O(k) for merge, and there are at most `k` splits. Space complexity is O(k) for storing the temporary result.

#include <vector>
#include <algorithm>
#include <iterator>

// Helper: find the maximum subsequence of exactly 'len' digits from 'nums'
// while preserving their order.
std::vector<int> maxSubsequence(const std::vector<int>& nums, int len) {
    std::vector<int> stack;
    int canDrop = static_cast<int>(nums.size()) - len;
    for (int digit : nums) {
        while (!stack.empty() && stack.back() < digit && canDrop > 0) {
            stack.pop_back();
            --canDrop;
        }
        stack.push_back(digit);
    }
    // Trim to required length (in case we didn't drop enough)
    stack.resize(len);
    return stack;
}

// Helper: merge two subsequences into the lexicographically largest combined
// sequence.
std::vector<int> merge(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    result.reserve(a.size() + b.size());
    int i = 0, j = 0;
    while (i < static_cast<int>(a.size()) || j < static_cast<int>(b.size())) {
        // Compare the remaining parts lexicographically to decide pick
        if (std::lexicographical_compare(
                a.begin() + i, a.end(),
                b.begin() + j, b.end())) {
            result.push_back(b[j++]);
        } else {
            result.push_back(a[i++]);
        }
    }
    return result;
}

// Return the maximum number of length k formed by merging digits from
// nums1 and nums2 while preserving each array's order.
std::vector<int> maxNumber(const std::vector<int>& nums1, const std::vector<int>& nums2, int k) {
    std::vector<int> best;
    const int n1 = static_cast<int>(nums1.size());
    const int n2 = static_cast<int>(nums2.size());
    const int start = std::max(0, k - n2);
    const int end = std::min(k, n1);
    for (int i = start; i <= end; ++i) {
        std::vector<int> sub1 = maxSubsequence(nums1, i);
        std::vector<int> sub2 = maxSubsequence(nums2, k - i);
        std::vector<int> candidate = merge(sub1, sub2);
        if (std::lexicographical_compare(best.begin(), best.end(),
                                         candidate.begin(), candidate.end())) {
            best = std::move(candidate);
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// Function under test is declared above; here we write tests.
int main() {
    // Basic example
    std::vector<int> r1 = maxNumber({3, 4, 6, 5}, {9, 1, 2, 5, 8, 3}, 5);
    assert(r1 == std::vector<int>({9, 8, 6, 5, 3}));

    // All from one array
    std::vector<int> r2 = maxNumber({6, 7}, {6, 0, 4}, 5);
    assert(r2 == std::vector<int>({6, 7, 6, 0, 4}));

    // Single array case (second empty)
    std::vector<int> r3 = maxNumber({3, 9}, {}, 2);
    assert(r3 == std::vector<int>({3, 9}));

    // Duplicates and equal merge decisions
    std::vector<int> r4 = maxNumber({2, 5, 2, 1, 2}, {3, 5, 1}, 5);
    assert(r4 == std::vector<int>({3, 5, 2, 2, 1}));

    // k = total size
    std::vector<int> r5 = maxNumber({1, 2}, {3, 4}, 4);
    assert(r5 == std::vector<int>({3, 4, 1, 2}));

    // k = 1 with large arrays
    std::vector<int> r6 = maxNumber({0, 0, 0, 0, 0}, {9, 8, 7}, 1);
    assert(r6 == std::vector<int>({9}));

    // Equal arrays interleaving
    std::vector<int> r7 = maxNumber({5, 5, 5}, {5, 5}, 5);
    assert(r7 == std::vector<int>({5, 5, 5, 5, 5}));

    // More complex case from known reasoning
    std::vector<int> r8 = maxNumber({2, 1, 7, 8}, {3, 6, 4, 2}, 4);
    assert(r8 == std::vector<int>({7, 8, 4, 2}));

    // Edge: all zeros
    std::vector<int> r9 = maxNumber({0, 0}, {0, 0, 0}, 3);
    assert(r9 == std::vector<int>({0, 0, 0}));
}
