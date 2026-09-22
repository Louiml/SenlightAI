Write a C++ function `int commonCharCount(const std::string& first, const std::string& second)` that takes two lowercase strings (containing only letters `'a'` to `'z'`) and returns the number of characters that can be paired when forming a common multiset intersection — i.e., for each letter from `'a'` to `'z'`, count the minimum of its frequency in the first string and its frequency in the second string, and sum those minima. However, if any letter that appears in the second string does not appear in the first string at all, the function must return `-1` (because the second string cannot be fully matched from the first). The strings may be empty, and the function must handle that gracefully (empty strings yield a count of `0`).

// The solution uses frequency counting via two integer arrays of size 26 (one for each lowercase letter). First, populate `countA` from `first` and `countB` from `second` by iterating over each character and incrementing `count[ch - 'a']`. Then iterate over all 26 letters. For each letter `i`, if `countB[i] > 0` and `countA[i] == 0`, immediately return `-1` because the second string contains a character not present in the first. Otherwise, add `min(countA[i], countB[i])` to the result. Edge cases: empty strings (both arrays all zeros → sum = 0), strings with repeated letters, and cases where the second string has a letter absent from the first (returns -1). Time complexity is O(n + m + 26) = O(n + m), where n and m are string lengths; space complexity is O(1) because we use fixed-size arrays of 26 integers.

#include <string>
#include <vector>
#include <algorithm>

// Returns the number of common characters (counting multiplicity) between two
// lowercase-only strings. Returns -1 if the second string contains any letter
// that does not appear in the first string.
int commonCharCount(const std::string& first, const std::string& second) {
    std::vector<int> countFirst(26, 0);
    std::vector<int> countSecond(26, 0);

    for (char c : first) {
        ++countFirst[c - 'a'];
    }
    for (char c : second) {
        ++countSecond[c - 'a'];
    }

    int result = 0;
    for (int i = 0; i < 26; ++i) {
        if (countSecond[i] > 0 && countFirst[i] == 0) {
            return -1;
        }
        result += std::min(countFirst[i], countSecond[i]);
    }
    return result;
}

#include <cassert>
#include <string>

int commonCharCount(const std::string& first, const std::string& second);

int main() {
    // Basic cases
    assert(commonCharCount("hello", "world") == 3); // h, l, o appear in both (l appears twice in hello but only once in world → min=1)
    assert(commonCharCount("abc", "abc") == 3);
    assert(commonCharCount("aaa", "aa") == 2);
    assert(commonCharCount("", "") == 0);
    assert(commonCharCount("", "a") == -1);

    // Missing character case
    assert(commonCharCount("abc", "ad") == -1); // 'd' not in first
    assert(commonCharCount("ab", "bca") == -1); // 'c' not in first

    // Duplicates and different frequencies
    assert(commonCharCount("aab", "ab") == 2); // a and b
    assert(commonCharCount("aab", "aaab") == 3); // two a's and one b (min of frequencies)
    assert(commonCharCount("zzz", "zz") == 2);

    // No common characters but all letters in second exist in first? Not possible unless second is empty.
    assert(commonCharCount("xyz", "xy") == 2);

    // Large repeated
    assert(commonCharCount("aabbcc", "abcc") == 4); // a:1, b:1, c:2

    // All letters
    std::string all1 = "abcdefghijklmnopqrstuvwxyz";
    std::string all2 = "abcdefghijklmnopqrstuvwxyz";
    assert(commonCharCount(all1, all2) == 26);
}
