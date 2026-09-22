Write a C++ function named `allLettersPresent` that takes a constant reference to a string of exactly three lowercase English letters and returns a boolean value indicating whether the string contains all three letters 'a', 'b', and 'c' at least once each. The input is guaranteed to be of length 3 and consist only of lowercase letters. Your function should return `true` if all three characters 'a', 'b', and 'c' appear in the string (regardless of order), and `false` otherwise.

The problem reduces to checking whether the set of characters in the input string covers the set {'a', 'b', 'c'}. Since the string length is exactly 3, the only way all three letters appear is if the string is a permutation of "abc" (e.g., "abc", "acb", "bac", "bca", "cab", "cba"). Thus, a simple approach is to use a boolean array of size 26 (or three separate flags) to mark which letters are seen, then check that the flags for 'a', 'b', and 'c' are all set. Alternatively, since the string is short, we can directly compare the sorted string to "abc" — if the sorted version equals "abc", then all three are present. This approach handles any duplicate letters (like "aab" → sorted "aab" ≠ "abc") correctly. Edge cases include strings with repeated letters, such as "aaa" (false) and "abb" (false). Time complexity is O(1) because the string length is fixed at 3, and space complexity is O(1) regardless of the method.

#include <string>
#include <algorithm>

// Checks if the input string of length 3 contains all letters 'a', 'b', and 'c'.
bool allLettersPresent(const std::string& s) {
    // Since length is exactly 3, all three are present iff sorted string equals "abc".
    std::string sorted = s;
    std::sort(sorted.begin(), sorted.end());
    return sorted == "abc";
}

#include <cassert>
#include <string>

// Function declaration/definition from solution would be above.
bool allLettersPresent(const std::string& s);

int main() {
    assert(allLettersPresent("abc") == true);
    assert(allLettersPresent("acb") == true);
    assert(allLettersPresent("bac") == true);
    assert(allLettersPresent("bca") == true);
    assert(allLettersPresent("cab") == true);
    assert(allLettersPresent("cba") == true);
    assert(allLettersPresent("aaa") == false);
    assert(allLettersPresent("abb") == false);
    assert(allLettersPresent("aab") == false);
    assert(allLettersPresent("xyz") == false);
    return 0;
}
