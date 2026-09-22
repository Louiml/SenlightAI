Write a C++ function `countHandSwitches(const std::string& sequence)` that takes a string consisting only of the characters `'O'`, `'X'`, and `'F'`, representing a sequence of keys pressed by a user typing with one finger on a keyboard where switching from typing `'O'` to typing `'X'` (or vice versa) requires moving the hand, while `'F'` is a neutral character that does not require any hand movement and does not affect the current hand state. The function must return the total number of times the user switches hands, i.e., the number of transitions between `'O'` and `'X'` in the sequence, ignoring `'F'` characters. If the sequence begins with `'F'`, the initial hand state is considered undefined and no switch occurs until the first `'O'` or `'X'` is encountered; after that, only actual alternations between `'O'` and `'X'` count. For example, for `"OFOXFO"`, the switches occur at positions where `O` → `X` and `X` → `O`, giving 2 switches. Ensure the function handles empty strings (returning 0) and works for very long strings efficiently.
The solution iterates through the string once, maintaining the last non-`'F'` character seen (`last_hand`), initialized to a character that is neither `'O'` nor `'X'` (e.g., `'\0'`). For each character `c` in the sequence:  
- If `c == 'F'`, ignore it (it does not change the hand state).  
- Otherwise (`c` is `'O'` or `'X'`), compare it with `last_hand`. If `last_hand` is not the same as `c` and `last_hand` is a valid hand (`'O'` or `'X'`), then increment the switch counter. Then update `last_hand = c`.  
This correctly ignores leading `'F'`s because `last_hand` is invalid until the first actual hand character appears, and no switch is counted at that first appearance. The method handles all cases, including no switches, single hand character, and alternating sequences. Time complexity is \(O(n)\) where \(n\) is the length of the string, and auxiliary space is \(O(1)\) (only a few variables). Edge cases: empty string returns 0; a string with only `'F'` returns 0; a string with only one `'O'` or `'X'` returns 0; alternating patterns count each transition exactly once.
#include <string>

// Count the number of hand switches between 'O' and 'X' in the given sequence.
// 'F' characters are neutral and do not affect the hand state.
// The function ignores leading 'F's and returns 0 if the string is empty or contains no alternation.
int countHandSwitches(const std::string& sequence) {
    char last_hand = '\0';  // No hand set yet
    int switch_count = 0;

    for (char c : sequence) {
        if (c == 'F') {
            continue;  // Neutral, does not change hand
        }
        // c is either 'O' or 'X'
        if (last_hand != '\0' && last_hand != c) {
            ++switch_count;  // Hand changed from the previous non-'F' character
        }
        last_hand = c;  // Update current hand to this character
    }

    return switch_count;
}
#include <cassert>
#include <string>

// The solution function is already defined above.
int countHandSwitches(const std::string& sequence);

int main() {
    // Basic cases
    assert(countHandSwitches("O") == 0);
    assert(countHandSwitches("X") == 0);
    assert(countHandSwitches("F") == 0);
    assert(countHandSwitches("") == 0);

    // Leading F's and single switch
    assert(countHandSwitches("FFFOF") == 0);
    assert(countHandSwitches("FFFXF") == 0);
    assert(countHandSwitches("FOX") == 1);
    assert(countHandSwitches("XFO") == 1);

    // Multiple switches
    assert(countHandSwitches("OXOX") == 3);
    assert(countHandSwitches("OFOXFO") == 2);
    assert(countHandSwitches("XFOXFOXF") == 3);

    // Consecutive same letters and F's interspersed
    assert(countHandSwitches("OOXXOO") == 2);
    assert(countHandSwitches("OFFOFFX") == 1);
    assert(countHandSwitches("FFXFOOFO") == 2);

    // No switches at all
    assert(countHandSwitches("FFFF") == 0);
    assert(countHandSwitches("OOOFFF") == 0);
    assert(countHandSwitches("XXXFFF") == 0);

    return 0;
}
