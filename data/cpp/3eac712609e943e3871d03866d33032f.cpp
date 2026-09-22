// Write a C++ function `wordSubsets` that takes two vectors of lowercase strings, `words1` and `words2`, and returns a vector containing all strings from `words1` that are "universal" for `words2`. A string `a` is universal if, for every string `b` in `words2`, `a` contains at least as many occurrences of each letter as `b` does (i.e., the letter-frequency vector of `a` is component-wise ≥ the maximum letter-frequency vector over all `b` in `words2`). The function should treat the inputs as read-only, preserve the order of matching strings as they appear in `words1`, and assume all strings contain only lowercase English letters ('a'–'z'). Example: if `words1 = {"amazon","apple","facebook","google","leetcode"}` and `words2 = {"e","o"}`, then the result is `{"facebook","google","leetcode"}` because `amazon` lacks `e` and `apple` lacks `o`, while the others have at least one `e` and one `o`.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic example from problem statement.
    std::vector<std::string> words1 = {"amazon","apple","facebook","google","leetcode"};
    std::vector<std::string> words2 = {"e","o"};
    assert(wordSubsets(words1, words2) == std::vector<std::string>({"facebook","google","leetcode"}));

    // Empty words2: every word is universal.
    assert(wordSubsets({"a","b","c"}, {}) == std::vector<std::string>({"a","b","c"}));

    // Empty words1: result is empty.
    assert(wordSubsets({}, {"a"}) == std::vector<std::string>({}));

    // Requirement with multiple letters and duplicates in words2.
    std::vector<std::string> w1 = {"aa","ab","ba","bb"};
    std::vector<std::string> w2 = {"a","b","aa"};
    assert(wordSubsets(w1, w2) == std::vector<std::string>({"aa"}));

    // Single character requirement.
    assert(wordSubsets({"abc","ac","a"}, {"b"}) == std::vector<std::string>({"abc"}));

    // Identical strings repeated.
    assert(wordSubsets({"x","x"}, {"x"}) == std::vector<std::string>({"x","x"}));

    // Mixed case with empty strings in words2 (empty string contributes no requirement).
    assert(wordSubsets({"", "a"}, {"", "a"}) == std::vector<std::string>({"a"}));

    // Larger alphabet coverage.
    std::vector<std::string> A = {"abcdefghijklmnopqrstuvwxyz"};
    std::vector<std::string> B = {"z","y","x"};
    assert(wordSubsets(A, B) == std::vector<std::string>({"abcdefghijklmnopqrstuvwxyz"}));

    // Requirement from multiple words2 where one has many of a letter.
    assert(wordSubsets({"aaa"}, {"a","aa","aaa"}) == std::vector<std::string>({"aaa"}));
    assert(wordSubsets({"aa"}, {"a","aa","aaa"}) == std::vector<std::string>({}));

    // No word satisfies the requirement.
    assert(wordSubsets({"dog","cat"}, {"zzz"}) == std::vector<std::string>({}));
}
#include <vector>
#include <string>
#include <algorithm>

// Helper function to compute character frequency vector for a single string.
std::vector<int> charCounts(const std::string& s) {
    std::vector<int> count(26, 0);
    for (char c : s) {
        ++count[c - 'a'];
    }
    return count;
}

// Returns all strings in words1 that contain at least as many of each letter
// as the maximum requirement derived from any string in words2.
std::vector<std::string> wordSubsets(
    const std::vector<std::string>& words1,
    const std::vector<std::string>& words2
) {
    // Build the maximum requirement vector from words2.
    std::vector<int> req(26, 0);
    for (const std::string& b : words2) {
        std::vector<int> cur = charCounts(b);
        for (int i = 0; i < 26; ++i) {
            req[i] = std::max(req[i], cur[i]);
        }
    }

    // Check each word in words1 against the requirement.
    std::vector<std::string> result;
    for (const std::string& a : words1) {
        std::vector<int> cur = charCounts(a);
        bool universal = true;
        for (int i = 0; i < 26; ++i) {
            if (cur[i] < req[i]) {
                universal = false;
                break;
            }
        }
        if (universal) {
            result.push_back(a);
        }
    }
    return result;
}
// The key insight is to avoid checking each word in `words1` against every word in `words2` individually, which would be expensive if `words2` is large. Instead, we first compute a single "maximum requirement" vector of size 26, `req`, where `req[i]` is the maximum count of character `i` (where `i = c - 'a'`) needed by any string in `words2`. For each string `b` in `words2`, we compute its character frequency vector and update `req[i] = max(req[i], cur[i])` for all 26 letters. After processing all of `words2`, `req` tells us the minimum number of each letter that any universal word must contain. Then, for each string `a` in `words1`, we compute its frequency vector and check if it satisfies `cur[i] >= req[i]` for every `i`. If it does, we add `a` to the result. Edge cases: if `words2` is empty, `req` is all zeros, and every string in `words1` is universal; if `words1` is empty, the result is empty; strings may be empty (then they contribute zero to `req` and are universal only if `req` is all zeros). Time complexity is O(L1 + L2 + 26*(|words1| + |words2|)), where L1 and L2 are the total characters across all strings in each vector. Since 26 is constant, this simplifies to O(L1 + L2) in practice, and space is O(26) for the requirement vector plus O(1) for each frequency vector.
