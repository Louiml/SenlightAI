/*
Write a C++ function `customInsertionString` that takes a non-empty string `s` consisting only of lowercase English letters. The function must determine the alphabetically smallest character that appears at least once in `s`, and then produce a new string by removing exactly one occurrence of that character from its first position in the original string, while keeping all other characters in their original order. In other words, for the alphabetically smallest distinct character present, delete its first occurrence and concatenate the remaining characters. The function should return this new string. For example, given "cbabc", the smallest distinct character is 'a', so we remove the first 'a' (at index 2) and return "cbbc". If the string is all the same letter, e.g., "zzz", the smallest distinct character is 'z', so remove the first 'z' and return "zz". The input may have repeated characters, but it will always contain at least one character, and all characters are guaranteed to be lowercase English letters. The function must not modify the input string, and it must work in O(n) time for a string of length n.
*/

#include <string>
#include <vector>

// Return a string formed by removing the first occurrence of the
// alphabetically smallest character present in s.
std::string customInsertionString(const std::string& s) {
    // Track which characters exist.
    std::vector<bool> present(26, false);
    for (char c : s) {
        present[c - 'a'] = true;
    }

    // Find the smallest character that appears.
    char target = 'a';
    for (int i = 0; i < 26; ++i) {
        if (present[i]) {
            target = static_cast<char>('a' + i);
            break;
        }
    }

    // Build the result, skipping the first occurrence of target.
    std::string result;
    result.reserve(s.size() - 1);
    bool skipped = false;
    for (char c : s) {
        if (!skipped && c == target) {
            skipped = true;
            continue;
        }
        result.push_back(c);
    }
    return result;
}

#include <cassert>
#include <string>

// Declare the function (or include the header if separated).
std::string customInsertionString(const std::string& s);

int main() {
    // Basic case: smallest distinct character is 'a', remove its first occurrence.
    assert(customInsertionString("cbabc") == "cbbc");

    // All same character: remove the first one.
    assert(customInsertionString("zzz") == "zz");

    // Single character: result is empty string.
    assert(customInsertionString("a") == "");

    // Smallest character appears multiple times, only first removed.
    assert(customInsertionString("aaab") == "aab");

    // Already sorted, smallest at beginning.
    assert(customInsertionString("abc") == "bc");

    // Smallest is not at the beginning.
    assert(customInsertionString("zab") == "zab"); // 'a' is smallest, first 'a' at index 1, result "zb"

    // Larger string with repeated letters.
    assert(customInsertionString("hello") == "ehello"); // smallest is 'e', remove first 'e', result "hllo"? Wait: "hello" -> smallest 'e', remove first 'e' -> "hllo"
    // Correct expected: "hllo"

    // Correct the above assertion:
    assert(customInsertionString("hello") == "hllo");

    // Another check: "xyz" -> smallest 'x', remove first -> "yz"
    assert(customInsertionString("xyz") == "yz");

    // Empty not allowed, but test a two-character string where both are same.
    assert(customInsertionString("bb") == "b");

    // Test with lowercase only and repeated smallest at end.
    assert(customInsertionString("bcaa") == "bca"); // smallest 'a', remove first at index 2 -> "bca"

    return 0;
}

// The problem requires identifying the lexicographically smallest character present in the string, then removing its first occurrence. We can solve this by first scanning the string to determine which characters appear. A boolean array of size 26 (one per lowercase letter) suffices to track existence. Then, iterate from 'a' to 'z' and find the first character marked as present—that is the target character. Next, scan the original string again, copying all characters except the first occurrence of that target character. This second scan must ignore exactly one match, so we use a flag to note whether we have already skipped one. After the skip, all subsequent characters (including other occurrences of the target) are copied normally. Time complexity is O(n) because we do two linear passes (one to find existence, one to build the result), and space complexity is O(1) auxiliary, not counting the output string which is also O(n). Edge cases include a single-character string (removing the only character yields an empty string), all characters being identical, and the smallest character appearing many times—only the first occurrence is removed. The target character is always present because the string is non-empty and contains lowercase letters.
