/*
Write a C++ function `mergeStringsUnderLimit` that takes four parameters: two strings `a` and `b` containing only lowercase English letters, and two integers `k` representing the maximum number of consecutive characters that can be taken from one string without switching to the other. The function must construct and return the lexicographically smallest possible string by repeatedly choosing the smallest available character from the fronts of `a` and `b`, but with the constraint that you cannot take more than `k` consecutive characters from the same string; when you have already taken `k` consecutive characters from one string, you must take from the other. If both strings are exhausted, stop. If one string is exhausted early, you may only take from the other (subject to the same consecutive‑limit constraint, but if only one string remains, you must take from it even if that would exceed `k` consecutive picks, because no alternative exists). The input strings may be empty. Return the merged string.
*/
#include <string>
#include <algorithm>
#include <cassert>

// Returns the lexicographically smallest merged string by taking characters from
// the front of two sorted strings, with the constraint that no more than `k`
// consecutive characters can be taken from the same string unless forced.
std::string mergeStringsUnderLimit(std::string a, std::string b, int k) {
    // Sort both strings to enable greedy smallest-character selection.
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    std::string result;
    result.reserve(a.size() + b.size());

    std::size_t i = 0, j = 0;
    int cntA = 0, cntB = 0;

    while (i < a.size() && j < b.size()) {
        // Determine if we should take from a.
        bool takeA = false;
        if (cntB == k) {
            // Forced to take from a because we've exhausted b's allowance.
            takeA = true;
        } else if (cntA < k && a[i] < b[j]) {
            // a's character is smaller and we still have allowance on a.
            takeA = true;
        } else if (cntA == k) {
            // Forced to take from b.
            takeA = false;
        } else {
            // cntB < k and either a[i] >= b[j] or cntA == k not satisfied.
            // But if cntA < k and a[i] < b[j] would have been caught earlier.
            // Here we must take from b because either a[i] > b[j] or a[i]==b[j]
            // and we can choose either; tie-breaking: we can pick a, but to keep
            // deterministic and lexicographically smallest, we actually can pick
            // either when equal; but to avoid unnecessary counter build-up, we
            // pick b when counters are equal and characters equal? Actually the
            // original logic picks a only if strictly smaller. Since we have
            // sorted strings, if equal, either works but to match spec's
            // "smallest available" we can pick either; but to be safe, pick b.
            takeA = false;
        }

        if (takeA) {
            result.push_back(a[i]);
            ++i;
            ++cntA;
            cntB = 0;
        } else {
            result.push_back(b[j]);
            ++j;
            ++cntB;
            cntA = 0;
        }
    }

    // Append remaining characters from whichever string still has elements.
    while (i < a.size()) {
        result.push_back(a[i++]);
    }
    while (j < b.size()) {
        result.push_back(b[j++]);
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above (mergeStringsUnderLimit).
int main() {
    // Basic case: alternating because of limit 1.
    assert(mergeStringsUnderLimit("a", "b", 1) == "ab");
    assert(mergeStringsUnderLimit("b", "a", 1) == "ab");

    // Sorted inputs: a="ab", b="cd", k=1 => smallest: a(0) then b(0) then a(1) then b(1)
    assert(mergeStringsUnderLimit("ba", "dc", 1) == "abcd");

    // k=2, both can take two at a time.
    assert(mergeStringsUnderLimit("aa", "bb", 2) == "aabb");

    // When one string is empty, take all from the other.
    assert(mergeStringsUnderLimit("", "xyz", 2) == "xyz");
    assert(mergeStringsUnderLimit("abc", "", 1) == "abc");

    // Force switching when limit reached.
    // a="a", b="b", k=1: take a, then forced b.
    assert(mergeStringsUnderLimit("a", "b", 1) == "ab");

    // When a character is larger, we take from b even if a has allowance.
    // a="z", b="a", k=2 => take b then a.
    assert(mergeStringsUnderLimit("z", "a", 2) == "az");

    // Equal characters: order doesn't affect lexicographic result, but we pick b.
    // a="c", b="c", k=1 => either works; expected "cc".
    assert(mergeStringsUnderLimit("c", "c", 1) == "cc");

    // Mixed lengths with limit 2.
    // a="aac", b="bb", k=2 => take two a's, then forced b, then b, then c.
    assert(mergeStringsUnderLimit("caa", "bb", 2) == "aabbc");

    // Large k (like 100) behaves like free merge.
    assert(mergeStringsUnderLimit("ba", "cd", 100) == "abcd");
    assert(mergeStringsUnderLimit("aaab", "cc", 100) == "aaabcc");

    return 0;
}
// The core idea is to sort both strings character-wise first, because choosing the smallest available character at each step yields the lexicographically smallest result only if we can freely pick; however, the consecutive‑limit constraint forces occasional forced picks. Since the strings are sorted, the smallest remaining character of each string is at its front (index `i` for `a`, `j` for `b`). We maintain two counters: `cntA` and `cntB` for how many consecutive characters we’ve taken from `a` and `b` so far. At each step, if we can choose `a` (i.e., `cntA < k` and either `cntB == k` or `a[i] < b[j]`), we take from `a` and reset `cntB` to 0 while incrementing `cntA`. Otherwise, we take from `b` (when `cntB < k` or we are forced because `cntA == k`), resetting `cntA` to 0 and incrementing `cntB`. Important edge cases: if one string is exhausted, we simply take from the other regardless of the counter (but if the counter would otherwise block, we must allow it because no choice exists). Also, when both counters are zero, the comparison of the front characters decides. Complexity: sorting each string takes O(n log n + m log m), and the merging loop runs at most O(n+m) times. Space: O(n+m) for the result string and O(1) extra aside from that.
