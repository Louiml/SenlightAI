Write a C++ function `bool canSegmentString(const std::string& s, const std::vector<std::string>& wordDict)` that determines whether the input string `s` can be segmented into a space-separated sequence of one or more dictionary words, where each word in the sequence must appear in `wordDict` (the dictionary may contain duplicate entries, and words can be reused). The string `s` is non-empty and contains only lowercase English letters; `wordDict` is a non-empty vector of non-empty lowercase strings. The function must return `true` if a valid segmentation exists, and `false` otherwise. For example, for `s = "leetcode"` and `wordDict = {"leet", "code"}`, return `true`; for `s = "catsandog"` with `wordDict = {"cats", "dog", "sand", "and", "cat"}`, return `false` because no combination of those words (each used any number of times) can exactly form the string without leftover characters or gaps. The function should handle edge cases such as a single word that exactly equals the entire string, overlapping dictionary words, and cases where no segmentation is possible.

The problem is a classic dynamic programming / reachability problem. Define a boolean array `reachable` of size `s.size() + 1` (or `s.size()` with careful indexing). Position `i` (0-indexed) means that the prefix `s[0..i-1]` (first `i` characters) can be segmented into valid words. We initialize `reachable[0] = true` because the empty prefix is trivially segmented. Then for each starting index `start` from 0 to `s.size()-1`, if `reachable[start]` is true, we attempt to match every word in the dictionary at position `start`. If a word matches exactly the substring `s.substr(start, word.size())` and the end index `end = start + word.size()` is within bounds, then the prefix up to `end` is reachable, so we set `reachable[end] = true`. If `end == s.size()`, we can immediately return `true` because the entire string is segmented. After processing all possible starts, if no reachable position reaches the end, return `false`. This approach is correct because it considers all possible ways to build the string from left to right, and by processing starts in increasing order, each reachable position is discovered when it is first reachable (no need for BFS/DFS, plain iteration suffices since once a position is marked reachable, we check it later when its turn comes as a start). Edge cases: empty dictionary is not allowed per problem, but if it were, the function should return `false` for non-empty `s`; a word that exactly matches the entire string will be found when `start = 0`; words longer than the remaining substring are skipped by the bounds check. Time complexity: `O(n * m * L)` where `n = s.size()`, `m = wordDict.size()`, `L` is the average length of words, because for each reachable start (up to `n`), we try each word (up to `m`) and do a substring comparison that costs `O(L)` on average (though with `substr` it's linear in word length). Space complexity: `O(n)` for the boolean array plus `O(L)` temporary for substring copies, so `O(n + L)`.

#include <string>
#include <vector>

// Determines if s can be segmented into a sequence of words from wordDict.
bool canSegmentString(const std::string& s, const std::vector<std::string>& wordDict) {
    const int n = static_cast<int>(s.size());
    std::vector<bool> reachable(n + 1, false);
    reachable[0] = true; // empty prefix is always segmentable

    for (int start = 0; start < n; ++start) {
        if (!reachable[start]) {
            continue;
        }
        for (const std::string& word : wordDict) {
            const int wordLen = static_cast<int>(word.size());
            const int end = start + wordLen;
            if (end <= n && s.compare(start, wordLen, word) == 0) {
                if (end == n) {
                    return true; // entire string segmented
                }
                reachable[end] = true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <string>
#include <vector>

// Function under test is declared in the solution section; include it here.
bool canSegmentString(const std::string& s, const std::vector<std::string>& wordDict);

int main() {
    // Basic positive case
    assert(canSegmentString("leetcode", {"leet", "code"}) == true);
    // Basic negative case with overlapping possibilities
    assert(canSegmentString("catsandog", {"cats", "dog", "sand", "and", "cat"}) == false);
    // Single word equals entire string
    assert(canSegmentString("hello", {"hello"}) == true);
    // Word longer than remaining string cannot match
    assert(canSegmentString("ab", {"abc", "a", "b"}) == true);
    // Empty prefix only, but non-empty string with no words
    assert(canSegmentString("x", {"y", "z"}) == false);
    // Reuse of words (e.g., "a" repeated)
    assert(canSegmentString("aaaa", {"a"}) == true);
    assert(canSegmentString("aaaa", {"aa"}) == true);
    // Multiple words with partial overlaps
    assert(canSegmentString("pineapplepenapple", {"apple", "pen", "applepen", "pine", "pineapple"}) == true);
    // Leading and trailing mismatches
    assert(canSegmentString("abc", {"ab", "bc"}) == false);
    // Dictionary contains duplicates – should still work
    assert(canSegmentString("goalspecial", {"go", "goal", "goals", "special"}) == true);

    return 0;
}
