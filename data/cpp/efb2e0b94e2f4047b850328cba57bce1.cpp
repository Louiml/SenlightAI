Write a C++ function named `countJewelsInStones` that takes two strings, `jewels` and `stones`, and returns an integer representing the number of stones that are also jewels. Each character in `jewels` is a distinct jewel type, and each character in `stones` is a stone type. The function should count how many stones (characters in `stones`) appear among the jewel types (characters in `jewels`). The comparison is case-sensitive; for example, `'a'` is different from `'A'`. Both strings may be empty, may contain any printable ASCII characters, and may be of any length. The function must not modify the input strings.
#include <cassert>

int main() {
    // Basic cases
    assert(countJewelsInStones("aA", "aAAbbbb") == 3);
    assert(countJewelsInStones("z", "ZZ") == 0);

    // Empty strings
    assert(countJewelsInStones("", "") == 0);
    assert(countJewelsInStones("abc", "") == 0);
    assert(countJewelsInStones("", "xyz") == 0);

    // Duplicate jewels (should still count once per stone)
    assert(countJewelsInStones("aa", "aAa") == 2);  // 'a' and 'A' are distinct, but 'a' appears twice in jewels, still counts both lowercase 'a's

    // Case sensitivity and special characters
    assert(countJewelsInStones("ABC", "abcABC") == 3);
    assert(countJewelsInStones("!#", "a#!b!") == 3);

    // All stones are jewels
    assert(countJewelsInStones("abcd", "dcba") == 4);

    // No overlap
    assert(countJewelsInStones("xyz", "abc") == 0);
}
#include <string>
#include <array>

// Returns the number of characters in stones that also appear in jewels.
// Comparison is case-sensitive.
int countJewelsInStones(const std::string& jewels, const std::string& stones) {
    std::array<int, 256> lookup{};
    for (char c : jewels) {
        ++lookup[static_cast<unsigned char>(c)];
    }
    int total = 0;
    for (char c : stones) {
        if (lookup[static_cast<unsigned char>(c)] > 0) {
            ++total;
        }
    }
    return total;
}
// The solution uses a direct lookup table indexed by the ASCII value of each character. Since the problem only involves printable ASCII characters (typically 0–127, but using 256 covers extended ASCII safely), an array of 256 integers is allocated and initialized to zero. First, iterate over every character in `jewels`, and for each character, increment the count at its ASCII index. This marks which characters are jewels. Then iterate over every character in `stones`; if the value at that character’s index is non‑zero, it means that character is a jewel, so increment a running total. The edge cases are: empty `jewels` (no character is marked, so total remains zero), empty `stones` (loop does nothing, returns zero), and duplicate characters in `jewels` (incrementing more than once does not change the logical result because the lookup only tests for non‑zero). The time complexity is \(O(|jewels| + |stones|)\) because both strings are traversed exactly once. The space complexity is \(O(1)\) because the lookup table has a fixed size of 256 integers, regardless of input length.
