// You are given a string `t` (the original text) and a string `p` (the pattern to find as a subsequence). You are also given a permutation `v` of the indices `1` through `|t|`, which specifies an order of character deletions. In each step `i` from 0 to `|t|-1`, you may delete the character at position `v[i]` (using 1-based indexing in the original string). Your task is to write a C++ function `maxDeletions` that takes `t`, `p`, and `v` and returns the maximum number of deletions you can perform (in the given order) such that after those deletions, `p` still appears as a subsequence of the remaining string. If `p` is empty, always return `|t|`. The function should handle cases where `p` is longer than `t`, returning `0` in that scenario.

The problem reduces to finding the largest prefix length `k` of the deletion sequence such that after removing the first `k` characters (in the order given by `v`), `p` is still a subsequence of the remaining characters of `t`. Since the set of deleted characters for each prefix is nested (i.e., deleting the first `k` characters always includes all deletions of any smaller prefix), the property “p is still a subsequence after deleting the first `k` characters” is monotonic: if it holds for `k`, it holds for all smaller `k`; if it fails for `k`, it fails for all larger `k`. Therefore, we can binary search on `k` from 0 to `n`. For a candidate `mid`, we mark the first `mid` positions in `v` as deleted, then scan `t` left to right, matching characters to `p` while skipping deleted positions. If we can match all of `p`, then `mid` is feasible; otherwise not. The answer is the maximum feasible `mid`. Edge cases: `p` empty always returns `n`; if `p` is longer than `t`, even with 0 deletions it cannot be a subsequence, so answer is 0; also the binary search should handle `n=0`. Time complexity is `O(n log n)` due to repeated scans, and space complexity is `O(n)` for the deleted boolean array.

#include <string>
#include <vector>
#include <algorithm>

// Returns the maximum number of deletions (in the order given by v) such that
// p remains a subsequence of t after those deletions.
int maxDeletions(const std::string& t, const std::string& p, const std::vector<int>& v) {
    const int n = static_cast<int>(t.size());
    const int m = static_cast<int>(p.size());

    // Empty pattern always remains a subsequence, so we can delete everything.
    if (m == 0) return n;

    // If the pattern is longer than the text, even with 0 deletions it cannot be a subsequence.
    if (m > n) return 0;

    // Lambda to check if after deleting the first delCnt elements of v,
    // p is still a subsequence of the remaining t.
    auto canForm = [&](int delCnt) -> bool {
        std::vector<bool> removed(n, false);
        for (int i = 0; i < delCnt; ++i) {
            removed[v[i] - 1] = true; // v is 1-based
        }

        int pIdx = 0;
        for (int i = 0; i < n && pIdx < m; ++i) {
            if (removed[i]) continue;
            if (t[i] == p[pIdx]) {
                ++pIdx;
            }
        }
        return pIdx == m;
    };

    // Binary search the maximum feasible deletion count.
    int low = 0, high = n; // high is n because if p is empty we already handled, but for non-empty p, high can be n (though may not be feasible)
    while (low < high) {
        int mid = low + (high - low + 1) / 2; // upper mid to avoid infinite loop
        if (canForm(mid)) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

#include <cassert>
#include <string>
#include <vector>

// Declaration of the function to test.
int maxDeletions(const std::string& t, const std::string& p, const std::vector<int>& v);

int main() {
    // Basic example: t="ab", p="a", v=[1,2]. Delete first char -> "b" fails, so max is 0.
    assert(maxDeletions("ab", "a", {1, 2}) == 0);
    // Example where one deletion works: t="abc", p="ac", v=[2,1,3]. Delete index 2 (b) -> "ac" works, delete more fails.
    assert(maxDeletions("abc", "ac", {2, 1, 3}) == 1);
    // Empty pattern: can delete all.
    assert(maxDeletions("hello", "", {1,2,3,4,5}) == 5);
    // Pattern longer than text: no deletions possible.
    assert(maxDeletions("abc", "abcd", {1,2,3}) == 0);
    // No deletions needed: pattern equal to text.
    assert(maxDeletions("xyz", "xyz", {1,2,3}) == 3);
    // Example where multiple deletions possible: t="aXbYc", p="abc", v=[2,4,1,3,5]. Delete positions 2 and 4 -> "abc" remains, delete more fails.
    assert(maxDeletions("aXbYc", "abc", {2, 4, 1, 3, 5}) == 2);
    // Example with all deletions allowed: t="abc", p="a", v=[1,2,3] -> cannot delete first char, so 0.
    assert(maxDeletions("abc", "a", {1,2,3}) == 0);
    // Example with single-character old and pattern: t="a", p="a", v=[1] -> deleting it fails, so 0.
    assert(maxDeletions("a", "a", {1}) == 0);
    // Example where pattern appears multiple times, but deletions break subsequence: t="aaa", p="aa", v=[2,1,3] -> delete index 2 leaves "aa", delete index 1 leaves "a", so max 1.
    assert(maxDeletions("aaa", "aa", {2,1,3}) == 1);
    // Example with pattern empty and empty text: max deletions = 0.
    assert(maxDeletions("", "", {}) == 0);
    return 0;
}
