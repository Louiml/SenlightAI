// Write a C++ function `int minSwapsToMatch(const std::string& source, const std::string& target)` that, given two strings of equal length consisting only of lowercase English letters, returns the minimum number of adjacent swaps needed to transform `source` into `target`. Each character in `source` must be matched to a corresponding character in `target` (characters of the same letter are indistinguishable). If it is impossible to transform `source` into `target` because the multiset of characters differs, return `-1`. The function should work for strings up to length 50,000.
This is a classic problem of counting the number of inversions needed to reorder one sequence into another when characters are matched greedily. First, verify that both strings have the same character counts; if not, return `-1`. Then build a permutation `perm` of indices from 1 to N (where N is the length) by scanning `source` left to right. For each character `source[i]`, locate the next unused occurrence of that same character in `target` (by scanning forward from a pointer), and record the position (1-indexed) in `perm[i]`. If no unused occurrence exists, return `-1`. After constructing `perm`, the minimum number of adjacent swaps needed is exactly the number of inversions in `perm`. We compute this using a Fenwick tree (Binary Indexed Tree): for each position `i` from 1 to N, the number of elements greater than `perm[i]` that have already appeared before it is `(i-1) - query(perm[i])`, where `query(x)` returns the count of previously seen elements with index ≤ x. Accumulate this sum and update the tree with `perm[i]`. Edge cases: duplicate letters are handled by the greedy matching; empty strings return 0; if lengths differ, the problem statement guarantees equal length but robust code checks. Time complexity is O(N log N) due to the Fenwick tree operations, and space is O(N) for arrays.
#include <string>
#include <vector>

// Returns the minimum number of adjacent swaps to transform source into target.
// Returns -1 if impossible (different character counts).
int minSwapsToMatch(const std::string& source, const std::string& target) {
    int n = static_cast<int>(source.size());
    if (n != static_cast<int>(target.size())) return -1;
    if (n == 0) return 0;

    // Check character count equality
    std::vector<int> cntSource(26, 0), cntTarget(26, 0);
    for (char c : source) cntSource[c - 'a']++;
    for (char c : target) cntTarget[c - 'a']++;
    for (int i = 0; i < 26; ++i) {
        if (cntSource[i] != cntTarget[i]) return -1;
    }

    // Build permutation: perm[i] = position in target (1-indexed) for source[i]
    std::vector<int> nextPos(26, 0); // next occurrence pointer per letter
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        int letter = source[i] - 'a';
        int pos = nextPos[letter];
        // Find next occurrence of this letter in target starting from pos
        while (pos < n && target[pos] != source[i]) {
            ++pos;
        }
        if (pos == n) {
            // This should not happen if counts match, but guard anyway
            return -1;
        }
        nextPos[letter] = pos + 1; // mark used
        perm[i] = pos + 1; // 1-indexed
    }

    // Fenwick tree to count inversions
    std::vector<int> bit(n + 1, 0); // 1-indexed

    auto lsb = [](int x) { return x & -x; };

    auto update = [&](int idx) {
        while (idx <= n) {
            bit[idx] += 1;
            idx += lsb(idx);
        }
    };

    auto query = [&](int idx) {
        int sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= lsb(idx);
        }
        return sum;
    };

    int swaps = 0;
    for (int i = 0; i < n; ++i) {
        swaps += i - query(perm[i]); // i is number of previous elements
        update(perm[i]);
    }
    return swaps;
}
#include <cassert>
#include <string>

// The free function is declared above or included from header.
// Here we provide a main function for testing.

int main() {
    // Basic cases
    assert(minSwapsToMatch("ab", "ba") == 1);
    assert(minSwapsToMatch("abc", "abc") == 0);
    assert(minSwapsToMatch("aab", "aba") == 1);
    assert(minSwapsToMatch("aaa", "aaa") == 0);

    // Impossible due to different character counts
    assert(minSwapsToMatch("ab", "aa") == -1);
    assert(minSwapsToMatch("abc", "abd") == -1);
    assert(minSwapsToMatch("", "") == 0);
    assert(minSwapsToMatch("a", "b") == -1);

    // Larger case with duplicates
    assert(minSwapsToMatch("abcddcba", "abcddcba") == 0);
    assert(minSwapsToMatch("dcbaabcd", "abcddcba") == 12);
    assert(minSwapsToMatch("abcabc", "cbaabc") == 4);

    // All same letters: no swaps needed regardless of order
    assert(minSwapsToMatch("zzzz", "zzzz") == 0);

    // Exact reverse
    assert(minSwapsToMatch("abcd", "dcba") == 6); // 3+2+1 = 6

    return 0;
}
