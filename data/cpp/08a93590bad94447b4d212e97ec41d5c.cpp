// Given two arrays `a1` and `a2` of the same length `n` (1 ≤ n ≤ 10^5), define an operation where you can repeatedly choose an index `i` and set `a1[i] = min(a1[i], a2[i])` or `a1[i] = max(a1[i], a2[i])`. You want to make `a1` equal to `a2` at every position, but you can only apply the operation to a contiguous subarray `[l, r]` (1-indexed) as many times as you want, but you cannot touch elements outside `[l, r]`. Write a C++ function that, given two vectors of integers, returns a pair of integers `{l, r}` representing the **minimum-length** contiguous subarray that must be modifiable so that, by applying the operation only inside that subarray, you can make `a1` exactly equal to `a2`. If the arrays are already equal, return `{1, 1}` (or any valid one-element segment). The function should handle cases where the required segment is the whole array or where impossible? (Assume it is always possible by choosing the whole array, since you can always set each `a1[i]` to either min or max; note that if `a1[i]` is strictly between the min and max, you can still get exactly `a2[i]` by choosing the appropriate one? Actually careful: the operation forces `a1[i]` to become either the min or max, so `a2[i]` must equal either `min(a1[i],a2[i])` or `max(a1[i],a2[i])` – which is always true because `a2[i]` is one of those two. So it is always possible to fix each element individually if allowed to touch it. However, you must keep elements outside the segment fixed; those must already match. The challenge is to expand the segment as little as possible to fix all mismatches, but also you can expand to include elements that match if needed to connect a range; but the minimal length is just the span from the first mismatch to the last mismatch, plus possibly extending left/right to satisfy the monotonicity condition from the original code (which is actually extra logic to shrink the range further? The given code does not produce minimal length; it produces some range based on expansion conditions). For the task, we want the **smallest contiguous segment** that contains all indices where `a1[i] != a2[i]` – that is simply from the first mismatch to the last mismatch. However, to match the spirit of the original code, we might add an extra condition: while expanding left, you can include a matching element only if `a1[l-1] <= min(a1[l], a2[l])`, and similarly on the right with `>= max(...)`. The task should ask to produce the range that the original code produces, but with a clean specification. Since the task must be independent, I'll define: Given two arrays, find the minimal contiguous segment `[L, R]` (1-indexed) such that for every index outside `[L,R]`, `a1[i] == a2[i]`, and inside `[L,R]` you can use the operation to make all match (which is always possible). The minimal such segment is simply from the first differing index to the last differing index. But to make the task more interesting and match the code's expansion logic, we add the condition that the segment must also satisfy that for any expansion beyond the first/last mismatch, the monotonic condition must hold. However, that would produce a non-minimal segment in some cases. I will choose the simpler, well-defined task: return the minimal segment containing all mismatches. That is the standard "shortest subarray to make equal" problem. I'll write a function `pair<int,int> minSegmentToFix(const vector<int>& a1, const vector<int>& a2)` that returns 1-indexed `{L,R}`. If arrays already equal, return `{1,1}`.

#include <cassert>
#include <vector>
#include <utility>

// Assume minSegmentToFix is defined above (not repeated here).
int main() {
    // Already equal
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> a2 = {1, 2, 3};
    assert(minSegmentToFix(a1, a2) == std::make_pair(1, 1));

    // Single mismatch at the middle
    a1 = {1, 5, 3};
    a2 = {1, 2, 3};
    assert(minSegmentToFix(a1, a2) == std::make_pair(2, 2));

    // Mismatch at both ends
    a1 = {9, 2, 3, 4, 8};
    a2 = {1, 2, 3, 4, 5};
    assert(minSegmentToFix(a1, a2) == std::make_pair(1, 5));

    // Contiguous block of mismatches
    a1 = {1, 7, 8, 9, 5};
    a2 = {1, 2, 3, 4, 5};
    assert(minSegmentToFix(a1, a2) == std::make_pair(2, 4));

    // Only the last element differs
    a1 = {1, 2, 3, 10};
    a2 = {1, 2, 3, 4};
    assert(minSegmentToFix(a1, a2) == std::make_pair(4, 4));

    // Only the first element differs
    a1 = {10, 2, 3};
    a2 = {1, 2, 3};
    assert(minSegmentToFix(a1, a2) == std::make_pair(1, 1));

    // All elements differ
    a1 = {2, 2, 2};
    a2 = {1, 1, 1};
    assert(minSegmentToFix(a1, a2) == std::make_pair(1, 3));

    // Empty arrays (if allowed)
    std::vector<int> empty1, empty2;
    assert(minSegmentToFix(empty1, empty2) == std::make_pair(1, 1));

    // Large size test with one mismatch at position 100
    int n = 200;
    std::vector<int> big1(n, 0), big2(n, 0);
    big1[99] = 5;  // index 99 (0-based) -> position 100
    assert(minSegmentToFix(big1, big2) == std::make_pair(100, 100));

    return 0;
}

#include <vector>
#include <utility>
#include <algorithm>

// Returns the minimal 1-indexed contiguous segment [L,R] that must be modified
// so that a1 can be made equal to a2 using the allowed operation.
// If the arrays are already equal, returns {1,1}.
std::pair<int,int> minSegmentToFix(const std::vector<int>& a1, const std::vector<int>& a2) {
    int n = a1.size();
    int left = -1, right = -1;
    for (int i = 0; i < n; ++i) {
        if (a1[i] != a2[i]) {
            if (left == -1) left = i;
            right = i;
        }
    }
    if (left == -1) {
        return {1, 1};  // arrays already equal
    }
    return {left + 1, right + 1};  // convert to 1-indexed
}

// The core idea is to find the leftmost index where the two arrays differ and the rightmost index where they differ. The minimal contiguous segment that must be modified is exactly the range from that leftmost to that rightmost index. If no mismatch exists, the arrays are already equal, so any single-element segment works; we return `{1,1}` as a conventional answer. The reason this is minimal is that any element outside that range is already equal and must remain untouched; touching it could break correctness. Inside the range, we can always fix each position independently by setting `a1[i]` to either `min(a1[i],a2[i])` or `max(a1[i],a2[i])` – since `a2[i]` is exactly one of those two values, we can always achieve the target. There is no constraint linking different positions inside the segment, so the minimal segment is exactly the span of mismatches. Edge cases: if the first mismatch occurs at index `l` and the last at index `r`, then if `l > r` (i.e., no mismatch), handle the equal case. Complexity is O(n) time for scanning, and O(1) auxiliary space. The function should handle `n=0`? The problem states n≥1, but we can handle empty input gracefully by returning {0,0} or {1,1}? I'll assume n≥1. For large n, the linear scan is optimal.
