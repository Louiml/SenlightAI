/*
Write a C++ function named `wordSubsets` that takes two vectors of lowercase English words, `A` and `B`, and returns a vector of all words from `A` that are "universal" for `B`. A word `a` is universal if, for every word `b` in `B`, the frequency of each letter in `a` is at least the maximum frequency of that letter across all words in `B`. In other words, `a` must contain enough occurrences of each letter to cover the combined demands of all `b` words simultaneously. The input vectors may be empty, and individual words can be empty strings. The output order should match the order of words in `A`.
*/

#include <vector>
#include <string>
#include <algorithm>

// Return all words in A that contain at least the maximum letter counts required by B.
std::vector<std::string> wordSubsets(const std::vector<std::string>& A, const std::vector<std::string>& B) {
    // Peak requirements for each letter across all words in B.
    std::vector<int> maxB(26, 0);
    for (const std::string& b : B) {
        std::vector<int> count(26, 0);
        for (char c : b) {
            ++count[c - 'a'];
        }
        for (int i = 0; i < 26; ++i) {
            maxB[i] = std::max(maxB[i], count[i]);
        }
    }

    std::vector<std::string> result;
    for (const std::string& a : A) {
        std::vector<int> countA(26, 0);
        for (char c : a) {
            ++countA[c - 'a'];
        }
        bool universal = true;
        for (int i = 0; i < 26; ++i) {
            if (countA[i] < maxB[i]) {
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

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case
    std::vector<std::string> A1 = {"amazon", "apple", "facebook", "google", "leetcode"};
    std::vector<std::string> B1 = {"lo", "eo"};
    assert(wordSubsets(A1, B1) == std::vector<std::string>({"google", "leetcode"}));

    // B empty → all words in A are universal
    std::vector<std::string> A2 = {"a", "b", "c"};
    std::vector<std::string> B2 = {};
    assert(wordSubsets(A2, B2) == std::vector<std::string>({"a", "b", "c"}));

    // A empty → result empty
    std::vector<std::string> A3 = {};
    std::vector<std::string> B3 = {"x", "y"};
    assert(wordSubsets(A3, B3) == std::vector<std::string>());

    // Single character requirement
    std::vector<std::string> A4 = {"a", "aa", "ab", "b"};
    std::vector<std::string> B4 = {"a"};
    assert(wordSubsets(A4, B4) == std::vector<std::string>({"a", "aa", "ab"}));

    // Multiple letters with frequency needs (e.g., need two 'a's from any B word)
    std::vector<std::string> A5 = {"aaa", "aa", "a", "baa"};
    std::vector<std::string> B5 = {"aa", "a"};  // maxB requires 'a' twice, 'b' none
    assert(wordSubsets(A5, B5) == std::vector<std::string>({"aaa", "baa"}));

    // Empty words in A: empty string is universal only if B has no letters
    std::vector<std::string> A6 = {"", "a"};
    std::vector<std::string> B6 = {};
    assert(wordSubsets(A6, B6) == std::vector<std::string>({"", "a"}));

    // Empty words in B: all A universal
    std::vector<std::string> A7 = {"hello", ""};
    std::vector<std::string> B7 = {"", ""};
    assert(wordSubsets(A7, B7) == std::vector<std::string>({"hello", ""}));

    // Case sensitivity: only lowercase letters allowed
    std::vector<std::string> A8 = {"apple", "aple"};
    std::vector<std::string> B8 = {"pp"};
    assert(wordSubsets(A8, B8) == std::vector<std::string>({"apple"}));

    // Combined maximum from multiple B words
    std::vector<std::string> A9 = {"aabb", "ab", "aab"};
    std::vector<std::string> B9 = {"ab", "aa"};  // needs 'a':2, 'b':1
    assert(wordSubsets(A9, B9) == std::vector<std::string>({"aabb", "aab"}));

    return 0;
}

// The key insight is to avoid checking each `a` against every `b` individually, which would be inefficient. Instead, pre‑compute a single frequency vector `maxB` of size 26, where `maxB[i]` stores the maximum count of character `i` needed by any word in `B`. This is done by iterating over all words in `B`, counting their letter frequencies, and taking the element‑wise maximum. Then, for each word `a` in `A`, compute its frequency vector and verify that for every letter index, its count is greater than or equal to `maxB[i]`. If so, `a` is universal. This works because the universal condition is per‑letter and independent across words — we only need to know the peak requirement for each letter. Edge cases: if `B` is empty, `maxB` is all zeros, so every word in `A` (including empty strings) is universal; if `A` is empty, the result is empty. The time complexity is O(|A|·L_A + |B|·L_B), where L_X is the average word length in vector X, plus O(26·(|A|+|B|)) for frequency comparisons. Space complexity is O(26) for the frequency arrays (excluding the output vector).
