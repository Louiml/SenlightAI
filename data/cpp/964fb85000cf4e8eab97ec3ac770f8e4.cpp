// Write a C++ function named `calculateComboPoints` that takes a C-style string (a null-terminated array of characters) as input, where each character is either `'C'` or another arbitrary character. The function must compute and return an integer score based on the following rules: each `'C'` contributes points equal to the current combo multiplier, where the combo starts at 1 and increases by 1 for each consecutive `'C'` (i.e., a run of consecutive `'C'` characters gives combo values 1, 2, 3, ...). The combo resets to 1 whenever a non-`'C'` character is encountered (or when a run ends). The function must handle empty strings and strings with no `'C'` characters. Use `const char*` parameter type for correctness, and do not modify the input string.

#include <cassert>

int calculateComboPoints(const char* result); // forward declaration

int main() {
    // Empty string
    assert(calculateComboPoints("") == 0);
    // No 'C' characters
    assert(calculateComboPoints("XYZ") == 0);
    // Single 'C'
    assert(calculateComboPoints("C") == 1);
    // Two consecutive C's
    assert(calculateComboPoints("CC") == 1 + 2);
    // Three consecutive C's
    assert(calculateComboPoints("CCC") == 1 + 2 + 3);
    // Mixed string with non-C breaks
    assert(calculateComboPoints("CXC") == 1 + 1);
    // Runs separated by non-C characters
    assert(calculateComboPoints("CCXCC") == (1+2) + (1+2));
    // Longer pattern: "C C C X C C" -> 1+2+3 + 1+2 = 9
    assert(calculateComboPoints("CCCXCC") == 1 + 2 + 3 + 1 + 2);
    // Single non-C after C resets combo for next C
    assert(calculateComboPoints("C X C") == 1 + 1); // note space is a character
    return 0;
}

#include <cstddef>   // for size_t if needed, but not explicitly required

// Calculate combo points from a null-terminated string containing 'C' and other chars.
// Each 'C' gives points equal to the current combo. Consecutive 'C's increase combo by 1.
int calculateComboPoints(const char* result) {
    int points = 0;
    int combo = 1;
    for (std::size_t i = 0; result[i] != '\0'; ++i) {
        if (result[i] == 'C') {
            points += combo;
            // Prepare combo for the next character
            if (result[i + 1] == 'C') {
                ++combo;
            } else {
                combo = 1;
            }
        }
    }
    return points;
}

// The algorithm scans the input string character by character from left to right. A variable `combo` is initialized to 1 before the loop. For each character, if it is `'C'`, we add the current `combo` value to the running total `points`, and then we need to determine the next combo value: if the next character (index `i+1`) is also `'C'`, we increment `combo`; otherwise, we reset it to 1. This handles runs of consecutive `'C'`: during a run, the combo increases before processing the next `'C'`. Key edge cases: empty string (loop does not run, returns 0), string with no `'C'` (returns 0), single `'C'` (adds 1, combo resets), and a run like "CCC" (adds 1+2+3=6). The approach also works correctly for strings with mixed characters like "CXC" (1 + 1 = 2). Time complexity is O(n) where n is the length of the string, because we do a single pass and constant work per character. Space complexity is O(1) auxiliary, since we only use a few integer variables.
