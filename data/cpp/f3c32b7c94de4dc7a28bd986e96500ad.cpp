Given a sequence of positive integers `arr[1..n]` (1-indexed) representing the lengths of adjacent segments, write a standalone C++ function `int longestSequenceAfterOneRemoval(const std::vector<int>& arr)` that returns the length of the longest contiguous strictly increasing subsequence (CIS) that can be obtained after removing at most one element from the original sequence. In other words, you may either keep the original sequence and take any contiguous increasing run, or you may delete exactly one element (if n > 1) and then take the longest contiguous increasing run in the remaining sequence. The function must handle all edge cases, including n = 1 (return 1), n = 2 (return 2), sequences that are already fully strictly increasing (return n), and sequences where no removal helps (still return the best possible). The input vector is 0-indexed in the function signature, but internally treat it as 1-indexed to match the original logic. The function must be robust, self-contained, and not rely on any global state. The expected time complexity is O(n) and space complexity is O(n) for precomputed DP arrays.
#include <cassert>
#include <vector>

// Intentionally re-declare the function from the solution
int longestSequenceAfterOneRemoval(const std::vector<int>& arr);

int main() {
    // n=1
    assert(longestSequenceAfterOneRemoval({5}) == 1);
    // Already increasing
    assert(longestSequenceAfterOneRemoval({1, 2, 3, 4}) == 4);
    // Fully decreasing
    assert(longestSequenceAfterOneRemoval({5, 4, 3, 2, 1}) == 1);
    // n=2 increasing
    assert(longestSequenceAfterOneRemoval({3, 7}) == 2);
    // n=2 non-increasing
    assert(longestSequenceAfterOneRemoval({7, 3}) == 1);
    // Single peak removal helps
    assert(longestSequenceAfterOneRemoval({1, 3, 2, 4}) == 3);
    // Remove endpoint helps
    assert(longestSequenceAfterOneRemoval({4, 1, 2, 3}) == 3);
    // Remove interior to merge two runs
    assert(longestSequenceAfterOneRemoval({1, 2, 5, 3, 4}) == 4); // remove 5 -> [1,2,3,4]
    // Long run with a single break
    assert(longestSequenceAfterOneRemoval({1, 2, 3, 10, 4, 5, 6}) == 6); // remove 10 -> [1,2,3,4,5,6]
    // Multiple options, choose max
    assert(longestSequenceAfterOneRemoval({2, 1, 3, 2, 4}) == 3); // remove 1 or 2? remove 1 -> [2,3,2,4] best 3; remove 2 (second) -> [2,1,3,4] best 3
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing contiguous subsequence
// that can be obtained after removing at most one element from the given sequence.
// The input vector is 0-indexed; internally we treat it as 1-indexed.
int longestSequenceAfterOneRemoval(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // 1-indexed copies for convenience
    std::vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = arr[i - 1];
    }

    // left[i] = length of longest strictly increasing contiguous segment ending at i
    std::vector<int> left(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        if (a[i] > a[i - 1]) {
            left[i] = left[i - 1] + 1;
        } else {
            left[i] = 1;
        }
    }

    // right[i] = length of longest strictly increasing contiguous segment starting at i
    std::vector<int> right(n + 1, 1);
    for (int i = n - 1; i >= 1; --i) {
        if (a[i] < a[i + 1]) {
            right[i] = right[i + 1] + 1;
        } else {
            right[i] = 1;
        }
    }

    int best = 1;
    // Best without deletion
    for (int i = 1; i <= n; ++i) {
        best = std::max(best, left[i]);
    }

    // Best after removing an interior element (2..n-1)
    for (int i = 2; i <= n - 1; ++i) {
        if (a[i - 1] < a[i + 1]) {
            best = std::max(best, left[i - 1] + right[i + 1]);
        }
    }

    // Remove the first element
    if (n >= 2) {
        best = std::max(best, right[2]);
    }
    // Remove the last element
    if (n >= 2) {
        best = std::max(best, left[n - 1]);
    }

    return best;
}
// The problem reduces to finding the longest contiguous strictly increasing subsequence (CIS) either in the original array or after deleting exactly one element. To solve efficiently, we precompute two DP arrays while scanning left to right:
//
// - `left[i]` (or `dp[i]` in the snippet): length of the longest strictly increasing contiguous segment ending at index i. This is computed as `left[i] = left[i-1] + 1` if `arr[i] > arr[i-1]`, else `left[i] = 1`. For clarity, we set `left[1] = 1` and use 1-indexing.
// - `right[i]`: symmetric, length of the longest strictly increasing contiguous segment starting at index i, computed right-to-left: `right[i] = right[i+1] + 1` if `arr[i] < arr[i+1]`, else `right[i] = 1`.
//
// The best answer without deletion is the maximum value in `left` (or equivalently `right`). For deletion, we consider each index i (1-based) from 1 to n as the element to remove. After removing element i, the new sequence may merge a prefix ending at i-1 and a suffix starting at i+1. Such a merge is valid only if `arr[i-1] < arr[i+1]`. In that case, the combined length is `left[i-1] + right[i+1]`. Additionally, we can always consider removing a boundary element (first or last), which simply gives the longest run in the remaining sequence, i.e., `left[n-1]` or `right[2]`. For n = 1, the answer is 1; for n = 2, the answer is always 2 because removing either element leaves a single element, and the original run may be 1 or 2 but at most one deletion yields a length-2 run if the two are increasing, else length 1; however the function must return 2 for n=2 because you can delete one element leaving one, but the best is 2 if the original is increasing, otherwise the longest after deletion is 1? Actually the snippet forces answer for n=2 to be 2, but careful: if n=2 and arr = [5,3], the original longest increasing run is 1, after deleting one you get [5] or [3] length 1, so answer should be 1, not 2. However the snippet says `if(n == 2)ans = 2;` which is wrong unless the problem guarantees all sequences have at least one increasing pair? The task must be self-consistent; so I will fix the spec: for n=2, the correct answer is 1 if the two are not increasing, else 2. But the snippet forces 2, so to be faithful I will state the problem exactly as the snippet's intent: you are allowed to remove **at most one** element, and you want the **maximum** contiguous increasing subsequence length after that removal. For n=2, if the two elements are not increasing, the best you can do is 1, not 2. Therefore I will correct the logic in my solution. However, to honor the task's origin, I will specify that the sequence contains **positive integers** and the answer is the maximum possible length of a contiguous strictly increasing subsequence after removing at most one element. For n=2, if a[0] < a[1], the longest is 2 (no removal or remove any one leaves length 1, so best is 2 by keeping both); if a[0] >= a[1], the longest is 1 (you can keep one element). So the snippet's forced `ans=2` for n=2 is a bug; I will not include that in the task spec. Instead, the correct algorithm handles all cases naturally.
//
// Edge cases:
// - n=1: return 1.
// - All increasing: answer = n (no removal needed, or removal gives n-1, so keep n).
// - All non-increasing: answer = 1 (remove one element, best is a single element).
// - A single "peak" element: e.g., [1,3,2,4] – removing 3 gives [1,2,4] length 3; removing 2 gives [1,3,4] length 3; answer 3.
// - Removing an endpoint: e.g., [4,1,2,3] – remove 4 gives [1,2,3] length 3.
//
// Algorithm:
// 1. Convert input vector to 1-indexed `a[1..n]`.
// 2. Compute `left[1..n]` and `right[1..n]` as described.
// 3. Compute `best = max(left[i])` for i=1..n (original best).
// 4. For each i from 2 to n-1, if `a[i-1] < a[i+1]`, then `best = max(best, left[i-1] + right[i+1])`.
// 5. Also consider removing first element: `best = max(best, right[2])` if n>=2; removing last element: `best = max(best, left[n-1])` if n>=2.
// 6. Return best.
//
// Time complexity: O(n) for the three linear passes (compute left, compute right, scan for deletion). Space complexity: O(n) for the two DP arrays.
