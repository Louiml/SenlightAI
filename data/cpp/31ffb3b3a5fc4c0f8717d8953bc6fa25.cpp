Write a C++ function `vector<string> wordSubsets(vector<string>& words, vector<string>& required)` that takes two vectors of lowercase English words and returns all words from `words` that are "universal" with respect to `required`. A word `w` is universal if, for every word `b` in `required`, `w` contains each letter at least as many times as it appears in `b`. In other words, the count of every letter in `w` must be at least the maximum count of that letter across all words in `required`. The input vectors may be empty or contain words with repeated letters, and the output should preserve the original order of words in `words`. For example, if `words = {"amazon","apple","facebook","google","leetcode"}` and `required = {"e","oo"}`, the result is `{"facebook","google","leetcode"}` because "amazon" lacks the double `o` and "apple" lacks the double `o` and also lacks two `e`s (actually "apple" has one `e`, and required needs two `e` total from `"ee"`? – here use max counts: `e:1` from "e", `o:2` from "oo", so "apple" has `e:1` (ok) but lacks `o:2`, so not universal). Ensure the function handles an empty `required` (all words are universal) and an empty `words` (returns empty vector).
The solution uses a frequency counting approach with two main phases. First, build a single frequency vector `base` of size 26 initialized to zero. For each word in `required`, compute its own frequency vector by iterating over characters and incrementing the corresponding index (`c - 'a'`). Then update `base` by taking the element-wise maximum with the current word's frequencies; this captures the maximum requirement per letter across all required words. Second, iterate through each word in `words`. For each, compute its frequency vector similarly. Then check if every letter count in the word's frequency is at least the corresponding value in `base`. If so, add the word to the result. Edge cases: an empty `required` means `base` is all zeros, so every word passes. An empty `words` naturally yields an empty result. Words with repeated letters are handled correctly because counts are compared exactly. Time complexity is O((A+B) * L) where A and B are the sizes of the input vectors and L is the average word length, because each word is processed once for counting. Space complexity is O(1) auxiliary (besides output) because we only use fixed-size vectors of 26 integers. The output vector itself uses O(A) space for the result.
#include <vector>
#include <string>
#include <algorithm>

// Return all words in 'words' that contain at least the maximum
// letter frequencies required by any word in 'required'.
std::vector<std::string> wordSubsets(std::vector<std::string>& words,
                                     const std::vector<std::string>& required) {
    // base[letter] = maximum required count of that letter across all required words
    std::vector<int> base(26, 0);
    for (const auto& word : required) {
        std::vector<int> curr(26, 0);
        for (char ch : word) {
            curr[ch - 'a']++;
        }
        for (int i = 0; i < 26; i++) {
            base[i] = std::max(base[i], curr[i]);
        }
    }

    std::vector<std::string> result;
    for (const auto& word : words) {
        std::vector<int> freq(26, 0);
        for (char ch : word) {
            freq[ch - 'a']++;
        }

        bool universal = true;
        for (int i = 0; i < 26; i++) {
            if (freq[i] < base[i]) {
                universal = false;
                break;
            }
        }
        if (universal) {
            result.push_back(word);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Assume the solution function is declared above.

int main() {
    std::vector<std::string> words1 = {"amazon", "apple", "facebook", "google", "leetcode"};
    std::vector<std::string> req1 = {"e", "oo"};
    std::vector<std::string> expected1 = {"facebook", "google", "leetcode"};
    assert(wordSubsets(words1, req1) == expected1);

    std::vector<std::string> words2 = {"a", "b", "c"};
    std::vector<std::string> req2 = {};
    assert(wordSubsets(words2, req2) == words2);

    std::vector<std::string> words3 = {};
    std::vector<std::string> req3 = {"abc"};
    assert(wordSubsets(words3, req3).empty());

    std::vector<std::string> words4 = {"abc", "aabb", "ab", "aaab"};
    std::vector<std::string> req4 = {"aab"};
    std::vector<std::string> expected4 = {"aabb", "aaab"};
    assert(wordSubsets(words4, req4) == expected4);

    std::vector<std::string> words5 = {"aaaa", "aaa", "aa", "a"};
    std::vector<std::string> req5 = {"aaa", "aa"};
    std::vector<std::string> expected5 = {"aaaa", "aaa"};
    assert(wordSubsets(words5, req5) == expected5);

    std::vector<std::string> words6 = {"abc", "def", "ghi"};
    std::vector<std::string> req6 = {"z"};
    assert(wordSubsets(words6, req6).empty());

    std::vector<std::string> words7 = {"apple", "banana", "cherry"};
    std::vector<std::string> req7 = {"a", "p"};
    std::vector<std::string> expected7 = {"apple"};
    assert(wordSubsets(words7, req7) == expected7);

    return 0;
}
