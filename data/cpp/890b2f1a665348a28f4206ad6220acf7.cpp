// Write a C++ function `int countMatchingLetterCases(const std::string& input)` that analyzes a string containing only ASCII letters (a-z and A-Z) and returns the number of distinct letters that appear in both lowercase and uppercase forms somewhere in the input. For example, for the string `"DabdcABCC"`, the letters 'a' (appears as 'a' and 'A') and 'c' (appears as 'c' and 'C') qualify, while 'd' appears only as lowercase, 'b' only as lowercase, and 'B' appears only as uppercase but 'b' is lowercase — wait, actually 'b' appears as lowercase 'b' and uppercase 'B' in the example, so let's be precise: the input `"DabdcABCC"` has: lowercase 'a','b','d','c' and uppercase 'D','A','B','C','C'. The letters that appear in both cases are 'a' (a and A), 'b' (b and B), 'c' (c and C), and 'd'? No 'd' uppercase does not appear, so only 'a','b','c' count → result 3. Your function should ignore any non-alphabetic characters if they appear (though the task guarantees only letters), be case-sensitive in detection but case-insensitive when pairing, and return the count as an integer. The function must not modify the input string and must be efficient for strings of length up to 10^6.
// The algorithm uses two fixed-size integer arrays of length 26, one for lowercase frequencies and one for uppercase frequencies. Iterate through each character of the input string. If the character is between 'a' and 'z' inclusive, increment the corresponding index in the lowercase array using `ch - 'a'`. If it is between 'A' and 'Z', increment the uppercase array using `ch - 'A'`. After the pass, loop through indices 0..25 and count how many indices have a value ≥ 1 in both the lowercase and uppercase arrays. This directly counts distinct letters that appear in both forms. Edge cases: an empty string returns 0; a string with only one case type (e.g., all lowercase) returns 0; duplicate letters in the same case do not affect the count because we only check for presence (>= 1). Time complexity is O(n) for the single pass plus O(26) for the final check, so overall O(n). Space complexity is O(1) because the two arrays have fixed size 26 regardless of input length.
#include <string>
#include <vector>

// Count distinct letters that appear in both lowercase and uppercase in the given string.
// The input is assumed to contain only ASCII letters (a-z, A-Z).
// The function does not modify the input.
int countMatchingLetterCases(const std::string& input) {
    // Frequency arrays for lowercase and uppercase letters.
    // Index 0 corresponds to 'a'/'A', 25 to 'z'/'Z'.
    std::vector<int> lowercaseCount(26, 0);
    std::vector<int> uppercaseCount(26, 0);

    // Single pass: count occurrences per case.
    for (const char ch : input) {
        if (ch >= 'a' && ch <= 'z') {
            ++lowercaseCount[ch - 'a'];
        } else if (ch >= 'A' && ch <= 'Z') {
            ++uppercaseCount[ch - 'A'];
        }
        // Non-alphabetic characters are ignored per specification.
    }

    // Count letters present in both cases.
    int result = 0;
    for (int i = 0; i < 26; ++i) {
        if (lowercaseCount[i] >= 1 && uppercaseCount[i] >= 1) {
            ++result;
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test.
int countMatchingLetterCases(const std::string& input);

int main() {
    // Test 1: Example from the original snippet.
    assert(countMatchingLetterCases("DabdcABCC") == 3); // a, b, c appear in both cases
    // Test 2: All lowercase only.
    assert(countMatchingLetterCases("abcdef") == 0);
    // Test 3: All uppercase only.
    assert(countMatchingLetterCases("XYZ") == 0);
    // Test 4: Single letter both cases.
    assert(countMatchingLetterCases("Aa") == 1);
    // Test 5: Multiple duplicates in one case, but missing other case.
    assert(countMatchingLetterCases("aaaAAA") == 1); // only 'a' appears in both
    // Test 6: Empty string.
    assert(countMatchingLetterCases("") == 0);
    // Test 7: Mixed with repeated letters but only one case per letter.
    assert(countMatchingLetterCases("aAbBcC") == 3);
    // Test 8: No matches even with variety.
    assert(countMatchingLetterCases("aA") == 1); // actually 'a' matches, but to test zero: use "aB"
    assert(countMatchingLetterCases("aBcD") == 0); // no letter appears in both cases
    // Test 9: Large string pattern (just a quick sanity check).
    std::string large;
    for (int i = 0; i < 100000; ++i) {
        large += (i % 2 == 0) ? "a" : "A";
    }
    assert(countMatchingLetterCases(large) == 1); // only 'a' appears in both

    return 0;
}
