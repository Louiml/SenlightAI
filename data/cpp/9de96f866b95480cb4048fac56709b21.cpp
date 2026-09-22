/*
Write a C++ function `int countSpecialLetters(const std::string& word)` that returns the number of distinct letters that appear in the string `word` both in lowercase and uppercase. For example, if `word = "aAAbbBc"`, the letter 'a' appears as 'a' and 'A', and 'b' appears as 'b' and 'B', so the answer is 2 (for 'a' and 'b'). The input string consists only of English letters (uppercase and lowercase), and may be empty. Characters that appear multiple times in one case still count only once. The function must be case-sensitive when distinguishing letters but case-insensitive when matching lowercase to uppercase pairs.
*/
#include <string>
#include <vector>

// Returns the number of distinct letters that appear both lowercase and uppercase in the input string.
// Input consists only of English letters; may be empty.
int countSpecialLetters(const std::string& word) {
    std::vector<int> lowercaseCount(26, 0);
    std::vector<int> uppercaseCount(26, 0);
    
    for (char ch : word) {
        if (ch >= 'a' && ch <= 'z') {
            lowercaseCount[ch - 'a']++;
        } else if (ch >= 'A' && ch <= 'Z') {
            uppercaseCount[ch - 'A']++;
        }
    }
    
    int specialCount = 0;
    for (int i = 0; i < 26; ++i) {
        if (lowercaseCount[i] > 0 && uppercaseCount[i] > 0) {
            specialCount++;
        }
    }
    return specialCount;
}
#include <cassert>

int main() {
    // Basic case with both cases for some letters
    assert(countSpecialLetters("aAAbbBc") == 2);
    // Empty string
    assert(countSpecialLetters("") == 0);
    // Only lowercase
    assert(countSpecialLetters("abc") == 0);
    // Only uppercase
    assert(countSpecialLetters("ABC") == 0);
    // Single letter pair
    assert(countSpecialLetters("zZ") == 1);
    // Duplicate letters in both cases, but only one pair per letter
    assert(countSpecialLetters("aAaA") == 1);
    // Mixed with repeated pairs, some letters missing one case
    assert(countSpecialLetters("aA bB cC") == 3); // spaces ignored? Actually input must be letters only, but test with letters only
    // Correct letters-only test: "aAbBcC"
    assert(countSpecialLetters("aAbBcC") == 3);
    // Letters where only one case exists
    assert(countSpecialLetters("aBcD") == 0);
    // Long string with all letters in both cases
    std::string allLetters;
    for (char c = 'a'; c <= 'z'; c++) {
        allLetters += c;
        allLetters += (char)(c - 32);
    }
    assert(countSpecialLetters(allLetters) == 26);
    return 0;
}
// The solution uses two frequency arrays of size 26, one for lowercase letters (`lfreq[26]`) and one for uppercase letters (`ufreq[26]`). Iterate through each character of the input string: if it is an uppercase letter, increment the corresponding index in `ufreq` (using `ch - 'A'`); if it is lowercase, increment in `lfreq` (using `ch - 'a'`). After processing all characters, iterate through indices 0 to 25 and count how many indices have both `ufreq[i] > 0` and `lfreq[i] > 0`. This ensures each distinct letter is counted only once even if it appears multiple times in either case. Edge cases include an empty string (returns 0), a string with only one case (returns 0), and duplicate characters in the same case (still counted once). Time complexity is O(n) for the single pass plus O(26) for the final check, so O(n) overall. Space complexity is O(1) because the frequency arrays have fixed size.
