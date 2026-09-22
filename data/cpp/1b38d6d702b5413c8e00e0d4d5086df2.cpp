// Write a C++ function that takes a non-empty string and returns the character that appears in the longest consecutive run (i.e., the character that has the maximum number of adjacent identical occurrences). If multiple characters have the same maximum run length, return the character whose run appears first in the string. The function should work for any ASCII string, including those with digits, symbols, and mixed-case letters. For example, in the string `"geeekk"`, the longest consecutive run is `"eee"` (length 3), so the result is `'e'`. In `"aaaabbcbbb"`, the runs are `"aaaa"` (length 4) and `"bbb"` (length 3), so the result is `'a'`. The function must be case-sensitive and should not modify the input.

The core algorithm is a single pass over the string, tracking the current run length and the best run found so far. Initialize `best_char` to the first character and `best_len` to 1 (or 0 if the string is empty, but the problem guarantees non-empty). Then iterate from index 1 to the end: if the current character equals the previous one, increment the current run length; otherwise, reset the current run length to 1 for the new character. After updating the current run length, compare it with `best_len`; if it is strictly greater, update `best_len` and `best_char` to the current character (which is the start of that run). This ensures that ties are broken by the earliest run, because we only replace when strictly greater. Edge cases: a single-character string returns that character; a string where all characters are identical returns that character; strings with no repeated characters (all runs length 1) return the first character. Time complexity is O(n) where n is the length of the string, with O(1) auxiliary space.

#include <string>

// Returns the character that appears in the longest consecutive run.
// If multiple runs have equal maximum length, returns the character of the first such run.
char longestConsecutiveRun(const std::string& str) {
    // The problem guarantees a non-empty string.
    char best_char = str[0];
    int best_len = 1;

    int current_len = 1;
    for (std::size_t i = 1; i < str.size(); ++i) {
        if (str[i] == str[i - 1]) {
            ++current_len;
        } else {
            current_len = 1;
        }

        if (current_len > best_len) {
            best_len = current_len;
            best_char = str[i];
        }
    }

    return best_char;
}

#include <cassert>
#include <string>

// declare the function (or include the header where it's defined)
char longestConsecutiveRun(const std::string& str);

int main() {
    assert(longestConsecutiveRun("geeekk") == 'e');           // "eee" length 3
    assert(longestConsecutiveRun("3786868") == '8');          // "88" length 2 vs "77" length 1? Actually "88" appears once, and "7" runs are length 1, but "8" at index 2-3 -> length 2
    assert(longestConsecutiveRun("110") == '1');              // "11" length 2 vs "0" length 1
    assert(longestConsecutiveRun("aaaabbcbbb") == 'a');       // "aaaa" length 4
    assert(longestConsecutiveRun("11") == '1');               // "11" length 2
    assert(longestConsecutiveRun("011101") == '1');           // "111" length 3 vs "0" length 1 at ends
    assert(longestConsecutiveRun("WoHNyJYLC") == 'W');        // all runs length 1, first char
    assert(longestConsecutiveRun("3141711779") == '1');       // "11" length 2? Actually "77" also length 2 but first "11" at index 5-6? Let's verify: "3141711779" -> runs: "3","1","4","1","7","1","11","7","9" -> longest run is "11" length 2, first such is '1'
    assert(longestConsecutiveRun("10111101101") == '1');      // "1111" length 4
    assert(longestConsecutiveRun("aabbabababcc") == 'a');     // "aa" length 2, "bb" length 2, "cc" length 2 -> first is 'a'
    assert(longestConsecutiveRun("x") == 'x');                // single char
    assert(longestConsecutiveRun("ABBA") == 'B');             // "BB" length 2

    return 0;
}
