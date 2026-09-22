Write a C++ function `int minVowelJumps(const std::string& s)` that, given a non-empty string consisting only of uppercase English letters, returns the maximum number of consecutive non‑vowel characters between two vowels, or between the start/end of the string and the nearest vowel. Vowels are `'A'`, `'E'`, `'I'`, `'O'`, `'U'`, and `'Y'`. Consecutive vowels count as a jump of 1, and the count includes the vowel itself at the end of each run. For example, in `"BCDF"` (no vowels), the whole string plus one extra position (like a virtual end) yields 5. In `"AEIOU"`, each vowel is a jump of 1, so the maximum is 1. In `"BCDAE"`, the runs are: before `'A'` = 4 (positions 0‑3 including `'A'`), then after `'A'` to `'E'` = 2 (positions 4‑5), and after `'E'` to end = 2 (position 6 + 1). So the answer is 4. The function must be `const`‑correct and handle any length, including very long strings, with O(n) time and O(1) space.
// The main idea is to scan the string once, keeping a counter `jump` that starts at 0. For each character, we increment `jump` first, then check if the character is a vowel. If it is a vowel, we compare `jump` with the current maximum and reset `jump` to 0. At the end, we must handle the tail after the last vowel: the problem considers a virtual end marker, so the final segment length is `jump + 1` (because the last vowel or the end contributes a count). For example, if the string has no vowels, the whole length plus 1 is the answer. Edge cases: a string that starts with a vowel gives a jump of 1 on the first character (since we increment before checking). A string that ends with a vowel – the final segment after the last vowel is length 0, but the virtual end adds 1, so we compare `0 + 1 = 1`. Time complexity is O(n) where n is the string length, space is O(1). The function should be `const`‑correct by taking a `const std::string&` parameter.
#include <string>
#include <algorithm>

// Returns the maximum number of consecutive non-vowel characters (including a virtual end) 
// between vowels, or from the start/end to the nearest vowel. Vowels: A, E, I, O, U, Y.
int minVowelJumps(const std::string& s) {
    int jump = 0;
    int maxRun = 0;
    for (char c : s) {
        ++jump;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y') {
            maxRun = std::max(maxRun, jump);
            jump = 0;
        }
    }
    // Add the virtual end position after the last character.
    maxRun = std::max(maxRun, jump + 1);
    return maxRun;
}
#include <cassert>
#include <string>

// The solution function is declared above.

int main() {
    // Simple cases
    assert(minVowelJumps("BCDF") == 5);          // no vowels: 4 chars + virtual end = 5
    assert(minVowelJumps("AEIOU") == 1);         // each vowel alone => run of 1
    assert(minVowelJumps("BCDAE") == 4);         // run before 'A' is 4, others are 2 and 2

    // Edge cases
    assert(minVowelJumps("A") == 1);             // one vowel at start
    assert(minVowelJumps("B") == 2);             // one non-vowel, virtual end => 2
    assert(minVowelJumps("AB") == 2);            // before 'A' = 1, after 'A' to end = 1+1 = 2
    assert(minVowelJumps("BA") == 2);            // before 'A' = 2, after 'A' = 0+1 = 1 => max 2
    assert(minVowelJumps("Y") == 1);             // Y is vowel
    assert(minVowelJumps("BCYDF") == 3);         // run before Y = 3, after Y = 2+1 = 3 => max 3

    // Longer string with mixed vowels and consonants
    assert(minVowelJumps("BBBBYBBBBYBBBB") == 5); // between Y's: 4+1=5, ends: 4+1=5
    assert(minVowelJumps("U") == 1);              // single vowel
    assert(minVowelJumps("ZZZZ") == 5);           // all consonants
}
