Write a C++ function `minimumSwitches(const std::string& s)` that, given a string containing only uppercase letters and the character `'F'`, returns the minimum number of switches needed to make all non-`'F'` characters identical to the first non-`'F'` character encountered, by only changing characters. Specifically, you process the string left to right: keep a current target character (initially none). For each non-`'F'` character, if it differs from the current target, you must "switch" (increment a counter) and update the target to that character. `'F'` characters are ignored (they do not affect the switch count or the target). If the string contains no non-`'F'` characters, return 0. The function should be efficient for very long strings (up to 10^6 characters). Example: `"ABBFFCC"` → first non-'F' is 'A' (target='A'), then 'B' differs → switch (ans=1, target='B'), next 'B' same, then 'C' differs → switch (ans=2, target='C'), so output 2. For `"FF"` output 0.
The problem reduces to counting the number of times the sequence of non-`'F'` characters changes value. We iterate through the string once, maintaining a variable `cur` that stores the current target character (initialized to some sentinel like `'\0'`). For each position:
- If the character is `'F'`, skip it (continue).
- If `cur` is unset (sentinel), set `cur` to the current character (no switch counted).
- If the character equals `cur`, continue.
- Otherwise, we have a change: increment the answer counter and update `cur` to the current character.

Edge cases: empty string or all `'F'` characters → answer 0. Single non-`'F'` character → answer 0. Repeated consecutive same letters → no extra switch. The algorithm runs in O(n) time with O(1) auxiliary space, which is optimal since we must examine every character.
#include <string>

// Count the number of times the sequence of non-'F' characters changes.
// 'F' is treated as a wildcard that does not affect the target.
int minimumSwitches(const std::string& s) {
    int switches = 0;
    char cur = '\0'; // sentinel meaning no target set yet

    for (char c : s) {
        if (c == 'F') continue;
        if (cur == '\0') {
            cur = c; // first non-'F' establishes target
        } else if (c != cur) {
            ++switches;
            cur = c;
        }
    }

    return switches;
}
#include <cassert>
#include <string>

int minimumSwitches(const std::string& s);

int main() {
    assert(minimumSwitches("") == 0);
    assert(minimumSwitches("FFF") == 0);
    assert(minimumSwitches("A") == 0);
    assert(minimumSwitches("ABBFFCC") == 2);
    assert(minimumSwitches("ABC") == 2);
    assert(minimumSwitches("AAAA") == 0);
    assert(minimumSwitches("AFFFA") == 0);
    assert(minimumSwitches("ABBA") == 1);
    assert(minimumSwitches("FBFBF") == 0); // only 'B' appears, no change
    assert(minimumSwitches("FAFBFC") == 2); // A->B->C
    return 0;
}
