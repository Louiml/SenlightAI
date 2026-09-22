Write a C++ function that, given two strings `jewels` and `stones`, returns the number of characters in `stones` that are also present in `jewels`. Each character in `jewels` is distinct, but `stones` may contain repeated characters, and each occurrence in `stones` counts separately. The comparison is case-sensitive, and the function should handle empty strings efficiently.
// The simplest and most direct approach is to iterate through every character in `jewels` and, for each, count how many times it appears in `stones`. This uses a nested loop: for each jewel character, scan all stone characters and increment a counter whenever a match is found. Since the problem guarantees that characters in `jewels` are unique, there is no risk of double-counting a character across multiple jewels. Edge cases include empty `jewels` (return 0 immediately), empty `stones` (return 0), and cases where no jewels match any stones. The time complexity is \(O(m \times n)\), where \(m\) is the length of `jewels` and \(n` is the length of `stones`. The space complexity is \(O(1)\), as only a single integer counter is needed.
#include <string>

// Count occurrences in stones of any character present in jewels.
int countJewelsInStones(const std::string& jewels, const std::string& stones) {
    int result = 0;
    for (char jewel : jewels) {
        for (char stone : stones) {
            if (jewel == stone) {
                ++result;
            }
        }
    }
    return result;
}
#include <cassert>

int main() {
    // Basic mixed case
    assert(countJewelsInStones("aA", "aAAbbbb") == 3);
    // All stones are jewels
    assert(countJewelsInStones("abc", "abcabc") == 6);
    // No matches
    assert(countJewelsInStones("z", "ZZ") == 0);
    // Empty jewels
    assert(countJewelsInStones("", "abc") == 0);
    // Empty stones
    assert(countJewelsInStones("a", "") == 0);
    // Both empty
    assert(countJewelsInStones("", "") == 0);
    // Single repeated stone
    assert(countJewelsInStones("x", "xxxxx") == 5);
    // Single jewel, multiple different stones
    assert(countJewelsInStones("k", "KkKk") == 2);
    // Case sensitivity
    assert(countJewelsInStones("A", "aA") == 1);
    // Duplicate stones with multiple jewels
    assert(countJewelsInStones("ab", "ababab") == 6);
    return 0;
}
