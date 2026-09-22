// Given a permutation of the first `n` positive integers (where `n` is given as input) and a number of test cases, write a C++ function `std::vector<int> countGoodPrefixes(const std::vector<int>& perm)` that, for each length `k` from `1` to `n`, determines whether the subarray consisting of the first `k` elements of the permutation contains exactly the integers from `1` to `k` (i.e., it is a valid prefix that is a permutation of the first `k` numbers). Return a binary vector of length `n` where the `i`-th element (1-indexed) is `1` if the first `i` elements form such a contiguous block of `1..i`, and `0` otherwise. For example, for the permutation `[1,3,2]`, the first 1 element is `{1}` (valid → 1), the first 2 elements are `{1,3}` (missing 2 → 0), and the first 3 elements are `{1,3,2}` (valid → 1), so the output is `[1,0,1]`. The function must efficiently handle large `n` up to 10^5 and many test cases.
// The key observation is that for the first `k` elements to be a permutation of `1..k`, the maximum value among those first `k` elements must equal `k`, and also the minimum value must equal `1` if `k>0`. However, a simpler and more direct approach: if we track the positions of each value in the permutation (i.e., `pos[value]` gives the index where that value appears), then the condition that the first `k` elements contain exactly `1..k` is equivalent to: the set of positions of values `1` through `k` are all within the first `k` indices, and importantly, they must be exactly the set `{1,2,...,k}` (since there are `k` values and `k` positions). But that set being exactly the first `k` positions is equivalent to the maximum position among values `1..k` being exactly `k` and the minimum position being exactly `1` (because positions are a permutation of `1..n`). Actually, if max position = k and min position = 1, does it guarantee all positions from 1..k are present? Since there are exactly k values and they are distinct, if their positions all lie between 1 and k inclusive, then they must cover all positions 1..k exactly. So we just need to track the running minimum and maximum of the positions of values seen so far as we iterate i from 1 to n. For each i, after including value i, we update minPos = min(minPos, pos[i]), maxPos = max(maxPos, pos[i]). If maxPos - minPos == i - 1 (since positions are 1-indexed, the count of positions should be i), then the first i elements form the set {1..i}. This condition is sufficient and necessary because the positions are distinct. Edge case: for i=1, minPos = maxPos = pos[1], and maxPos-minPos=0, so condition holds if pos[1]==1, but actually if pos[1] is not 1, say perm = [2,1], then first 1 element is {2} which is not {1}, so the condition fails correctly because for i=1, minPos=maxPos=pos[1], and we require maxPos-minPos+1 == i? Let's check: for i=1, we need that the only position among values 1..1 is 1, so pos[1] must be 1. The condition maxPos-minPos == i-1 gives 0 == 0 always, which is wrong. So we need a stricter condition: we need that minPos == 1 and maxPos == i. Because if minPos>1, then position 1 is not covered. Similarly if maxPos > i, then some position beyond i is covered. So we check if (minPos == 1 && maxPos == i). That ensures the positions are exactly 1..i (since there are i positions and they are all between 1 and i, and distinct, so they must be all of them). Indeed, if minPos=1 and maxPos=i, then the set of positions is a subset of [1,i] with i elements, so it must be exactly {1..i}. This works for all i. So algorithm: precompute pos array from input perm. Initialize minPos = large, maxPos = 0. Loop i=1..n: update minPos = min(minPos, pos[i]), maxPos = max(maxPos, pos[i]). If (minPos == 1 && maxPos == i) then ans[i-1] = 1 else 0. This is O(n) per test case, with O(n) space. Complexity: O(n) time and O(n) space per test, which is optimal.
#include <vector>
#include <algorithm>

// Returns a binary vector where result[i-1] == 1 iff the first i elements
// of the permutation form exactly the set {1,2,...,i}.
std::vector<int> countGoodPrefixes(const std::vector<int>& perm) {
    int n = static_cast<int>(perm.size());
    std::vector<int> pos(n + 1);
    for (int idx = 0; idx < n; ++idx) {
        pos[perm[idx]] = idx + 1;  // 1-indexed positions
    }

    std::vector<int> ans(n, 0);
    int minPos = n + 1;
    int maxPos = 0;
    for (int value = 1; value <= n; ++value) {
        minPos = std::min(minPos, pos[value]);
        maxPos = std::max(maxPos, pos[value]);
        if (minPos == 1 && maxPos == value) {
            ans[value - 1] = 1;
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Function under test (declaration)
std::vector<int> countGoodPrefixes(const std::vector<int>& perm);

int main() {
    // Example from description: [1,3,2] -> [1,0,1]
    std::vector<int> result1 = countGoodPrefixes({1, 3, 2});
    assert((result1 == std::vector<int>{1, 0, 1}));

    // Already sorted permutation: every prefix is good
    std::vector<int> result2 = countGoodPrefixes({1, 2, 3, 4});
    assert((result2 == std::vector<int>{1, 1, 1, 1}));

    // Reverse order: only the full length is good
    std::vector<int> result3 = countGoodPrefixes({4, 3, 2, 1});
    assert((result3 == std::vector<int>{0, 0, 0, 1}));

    // Single element
    std::vector<int> result4 = countGoodPrefixes({1});
    assert((result4 == std::vector<int>{1}));

    // Another permutation: [2,1,4,3] -> prefixes: {2} no, {2,1} yes, {2,1,4} no, full yes
    std::vector<int> result5 = countGoodPrefixes({2, 1, 4, 3});
    assert((result5 == std::vector<int>{0, 1, 0, 1}));

    // Case where a value appears at position 1 but not the whole set
    // [3,1,2] -> prefixes: {3} no, {3,1} no, {3,1,2} yes
    std::vector<int> result6 = countGoodPrefixes({3, 1, 2});
    assert((result6 == std::vector<int>{0, 0, 1}));

    // Large n to test performance implicitly, just a simple check
    int n = 100000;
    std::vector<int> big(n);
    for (int i = 0; i < n; ++i) big[i] = i + 1;  // sorted
    std::vector<int> resultBig = countGoodPrefixes(big);
    for (int i = 0; i < n; ++i) assert(resultBig[i] == 1);

    return 0;
}
