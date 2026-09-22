Write a C++ function named `countStretchyWords` that accepts a source string `S` and a vector of query strings `words`. A query word is considered "stretchy" if it can be transformed into `S` by repeatedly replacing any group of identical consecutive characters with a longer group of the same character, under the rule that the resulting group must have length at least 3 (i.e., you may extend a group only if the final group length is ≥ 3, and you may not shrink groups). More precisely, after compressing both `S` and the query word into sequences of `(character, count)` pairs, the query word can be matched if for every group in `S`, the corresponding group in the query has the same character, the query's group count is less than or equal to `S`'s group count, and if the counts differ, `S`'s group count must be at least 3. Return the number of query words that satisfy this condition. For example, if `S = "heeellooo"` and `words = {"hello", "hi", "helo"}`, the function should return 1. Assume all strings consist only of lowercase English letters and have lengths between 0 and 100 (inclusive), and the `words` vector length is between 0 and 100.

The problem essentially asks to verify whether each query word can be expanded into the target string `S` by "stretching" some groups. The key observation is that both `S` and the query word can be compressed into runs of identical characters. For example, `"heeellooo"` becomes `[('h',1), ('e',3), ('l',2), ('o',3)]`, and `"hello"` becomes `[('h',1), ('e',1), ('l',2), ('o',1)]`. The matching condition is: the sequence of characters must match exactly, and for each corresponding pair, the query's count `q` must be ≤ `S`'s count `s`. Additionally, if `q != s`, then `s` must be at least 3 (because we cannot extend a group to a length of 1 or 2; all extensions must produce a group of length ≥ 3). If `q == s`, no extension is needed. If `q > s`, the query would have to shrink, which is not allowed. If `q < s` but `s < 3`, the extension would not reach the required minimum of 3, so it's invalid. Edge cases: empty `S` — only an empty query word matches; empty query word matches only if `S` is also empty; words with mismatched character sequences fail immediately. The algorithm compresses `S` once into a vector of pairs. For each word, it compresses on‑the‑fly while comparing with `S`'s groups, stopping early on mismatch. Time complexity: `O(|S| + total length of all words)` because each character is processed at most once per compression/scan. Space complexity: `O(|S|)` for the compressed representation of `S`, plus `O(1)` auxiliary for each word scan.

#include <vector>
#include <string>
#include <utility>

// Counts how many words in the list can be expanded to match S
// by stretching groups of length at least 3.
int countStretchyWords(const std::string& S, const std::vector<std::string>& words) {
    // Compress S into (character, run length) pairs.
    std::vector<std::pair<char, int>> sGroups;
    for (int i = 0; i < static_cast<int>(S.size()); ) {
        int j = i + 1;
        while (j < static_cast<int>(S.size()) && S[i] == S[j]) ++j;
        sGroups.emplace_back(S[i], j - i);
        i = j;
    }

    int validCount = 0;

    for (const std::string& w : words) {
        int si = 0;  // index into sGroups
        int wi = 0;  // index into w
        int wn = static_cast<int>(w.size());
        bool matches = true;

        while (wi < wn && si < static_cast<int>(sGroups.size())) {
            // Count the current run in w.
            int runStart = wi;
            while (wi + 1 < wn && w[wi] == w[wi + 1]) ++wi;
            int wRunLen = wi - runStart + 1;
            ++wi;  // move to next character (or end)

            char expectedChar = sGroups[si].first;
            int sRunLen = sGroups[si].second;

            // Characters must match, query length must not exceed S's length,
            // and if lengths differ, S's group must be at least 3.
            if (w[runStart] != expectedChar ||
                wRunLen > sRunLen ||
                (wRunLen < sRunLen && sRunLen < 3)) {
                matches = false;
                break;
            }
            ++si;
        }

        // A match is valid only if both the word and S groups are fully consumed.
        if (matches && wi == wn && si == static_cast<int>(sGroups.size())) {
            ++validCount;
        }
    }

    return validCount;
}

#include <cassert>
#include <vector>
#include <string>

// Function declaration (assumed from the solution above)
int countStretchyWords(const std::string& S, const std::vector<std::string>& words);

int main() {
    // Example from the problem statement
    assert(countStretchyWords("heeellooo", {"hello", "hi", "helo"}) == 1);

    // Empty source string: only empty word matches
    assert(countStretchyWords("", {}) == 0);
    assert(countStretchyWords("", {""}) == 1);
    assert(countStretchyWords("", {"a"}) == 0);

    // Group of length 2 cannot be stretched to length 3? Actually it can, but
    // from length 1 to 2 is not allowed. Check that "ll" in "hello" blocks "helo".
    assert(countStretchyWords("heeellooo", {"helo"}) == 0);

    // A group of length 2 in S cannot be matched by a group of length 1 in word
    // because extension would produce length 2, which is < 3, so invalid.
    assert(countStretchyWords("abb", {"ab"}) == 0);

    // If query group is longer than S group, it cannot be shrunk.
    assert(countStretchyWords("ab", {"aab"}) == 0);

    // Exact match always counts, even if groups are length 1 or 2.
    assert(countStretchyWords("abc", {"abc"}) == 1);
    assert(countStretchyWords("aabb", {"aabb"}) == 1);

    // A group of length 3 in S can be matched by a group of length 1, 2, or 3 in word.
    assert(countStretchyWords("aaab", {"ab"}) == 1);
    assert(countStretchyWords("aaab", {"aab"}) == 1);
    assert(countStretchyWords("aaab", {"aaab"}) == 1);

    // Multiple valid words.
    assert(countStretchyWords("zzzzyyy", {"zzy", "zzzyy"}) == 2);

    return 0;
}
