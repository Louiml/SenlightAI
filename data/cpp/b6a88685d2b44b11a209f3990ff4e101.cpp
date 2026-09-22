Write a C++ function named `longestRun` that takes a non-empty string `s` and returns the length of the longest consecutive sequence of identical characters in the string. The function should handle both uppercase and lowercase letters, digits, punctuation, and spaces (if present). For example, for input `"aaabbbbbcc"`, the longest run is `5` (the `b`s); for `"abc"`, the longest run is `1`; for `"!!!  !!"`, the longest run is `4` (there are two spaces, then one space, then two exclamation marks — the longest is 2? Actually the example: `"!!!  !!"` → runs: `!!!` (3), `  ` (2), `!!` (2) → longest is 3). If the string contains only one character, the longest run is the length of the string. The function must be efficient and not use any extra storage proportional to the input length beyond a few scalar variables.

#include <cassert>
#include <string>

// Declaration for testing (normally provided by the solution).
int longestRun(const std::string& s);

int main() {
    assert(longestRun("aaabbbbbcc") == 5);   // b's run of 5
    assert(longestRun("abc") == 1);          // no repeats
    assert(longestRun("!!!  !!") == 3);      // '!' run of 3
    assert(longestRun("a") == 1);            // single character
    assert(longestRun("zzzz") == 4);         // all same
    assert(longestRun("aAaA") == 1);         // case-sensitive: no equal neighbors
    assert(longestRun("112233344") == 3);    // '3's run of 3
    assert(longestRun("   ") == 3);          // three spaces
    assert(longestRun("abbbba") == 4);       // b's run
    assert(longestRun("!!??!!?") == 2);      // longest run is 2 (either '!' or '?' or '!' at start)
    return 0;
}

#include <string>

// Returns the length of the longest consecutive run of identical characters in s.
int longestRun(const std::string& s) {
    // Task guarantees non-empty input, but handle defensively.
    if (s.empty()) {
        return 0;
    }

    int currentRunLength = 1;   // length of the run we are currently in
    int longestRunLength = 1;   // maximum run length seen so far

    for (std::size_t i = 1; i < s.length(); ++i) {
        if (s[i] == s[i - 1]) {
            ++currentRunLength;
        } else {
            currentRunLength = 1;
        }

        if (currentRunLength > longestRunLength) {
            longestRunLength = currentRunLength;
        }
    }

    return longestRunLength;
}

// The algorithm performs a single left-to-right scan of the string. We maintain two variables: `currentRunLength` (the length of the current run of identical characters) and `longestRun` (the maximum run length seen so far). Start by setting both to 1, assuming the first character begins a run. For each subsequent character (starting from index 1), if it equals the previous character, increment `currentRunLength`; otherwise, reset `currentRunLength` to 1. After updating `currentRunLength`, compare it with `longestRun` and update if needed. Edge cases: an empty string is not allowed per the task, but if it were, we could return 0; a string of length 1 returns 1; a string where all characters are the same returns the full length. Since we only do one pass and use a constant number of variables, the time complexity is O(n) and the auxiliary space is O(1).
