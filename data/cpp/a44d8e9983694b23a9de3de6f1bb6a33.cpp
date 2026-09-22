Write a C++ function named `countVowelStartingAndEndingWords` that takes a vector of strings (`words`) and two integer indices (`left` and `right`) and returns the number of strings in the inclusive range `[left, right]` that both start and end with a lowercase vowel (`a`, `e`, `i`, `o`, `u`). The function must handle empty strings gracefully (they do not qualify), work correctly when `left` > `right` (return 0), and treat only lowercase vowels as valid. The function should be declared as a free function with proper `const` correctness (take `words` by const reference, and use a helper `isVowel` that is either a private member or an internal static function). The input range is guaranteed to be within the bounds of the vector, but the order of `left` and `right` may not be sorted.

#include <cassert>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words1 = {"apple", "banana", "orange", "umbrella", "elephant"};
    assert(countVowelStartingAndEndingWords(words1, 0, 4) == 3); // apple, orange, umbrella

    std::vector<std::string> words2 = {"", "a", "b", "ae", "io", "xyz"};
    assert(countVowelStartingAndEndingWords(words2, 0, 5) == 3); // "a", "ae", "io"

    std::vector<std::string> words3 = {"cat", "dog", "egg"};
    assert(countVowelStartingAndEndingWords(words3, 2, 2) == 1); // only "egg"

    std::vector<std::string> words4 = {"start", "end", "middle"};
    assert(countVowelStartingAndEndingWords(words4, 1, 3) == 0); // left <= right but none match

    // left > right returns 0
    assert(countVowelStartingAndEndingWords(words1, 4, 0) == 0);

    // Single character "u" qualifies
    std::vector<std::string> words5 = {"u", "v", "w"};
    assert(countVowelStartingAndEndingWords(words5, 0, 2) == 1);

    // Empty range with left == right on an empty string
    std::vector<std::string> words6 = {""};
    assert(countVowelStartingAndEndingWords(words6, 0, 0) == 0);
}

#include <string>
#include <vector>

// Helper to check if a character is a lowercase vowel.
static bool isVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

// Count words in the inclusive index range [left, right] that start and end with a lowercase vowel.
int countVowelStartingAndEndingWords(const std::vector<std::string>& words, int left, int right) {
    int count = 0;
    // Ensure we only iterate if left <= right
    for (int i = left; i <= right; ++i) {
        const std::string& word = words[i];
        // Skip empty strings to avoid out-of-bounds access
        if (word.empty()) {
            continue;
        }
        if (isVowel(word.front()) && isVowel(word.back())) {
            ++count;
        }
    }
    return count;
}

// The solution iterates through the indices from `left` to `right` inclusive (note: the original snippet had an off‑by‑one error; the task corrects this by using `<=`). For each index `i`, we first check if the string is non‑empty (to avoid accessing `words[i][0]` on an empty string). Then we check whether the first character (`words[i][0]`) and the last character (`words[i][words[i].size() - 1]`) are both lowercase vowels. We use a helper function `isVowel` that returns `true` for exactly the five lowercase vowels. If both conditions hold, we increment a counter. If `left` > `right`, the loop does not execute and we return 0. Time complexity is O(k) where k = max(0, right - left + 1), and space complexity is O(1) beyond the input storage. Edge cases include empty strings (skip), single‑character strings (first and last are the same, so both checks must pass), and the case where `left` equals `right` (only one word considered).
