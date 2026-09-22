Write a C++ function `bool isMeowSequence(const std::string& s)` that determines whether a given string follows the pattern of a "meow" sequence: it must consist only of the letters `m`, `e`, `o`, `w` (in any case), appearing in that exact groups order — all `m`'s first, then all `e`'s, then all `o`'s, then all `w`'s — with each group non‑empty. The function should return `true` if the string matches the pattern and `false` otherwise. Whitespace, digits, or any other characters make it invalid. For example, `"meow"`, `"MMMeeeoowWW"`, and `"mEoW"` are valid, while `"meo"` (missing w), `"emow"` (wrong order), `"mmeeow"` (mixed m and e), and `"meowx"` (extra character) are invalid. The function must handle empty strings and strings of length 1 correctly.

The problem is a variation of checking a sequence of grouped identical characters with a strict order. We can solve it in a single pass: iterate through the string and maintain a state variable representing which group we are currently in (0 = before m, 1 = m group, 2 = e group, 3 = o group, 4 = w group). For each character (converted to lowercase for uniformity), we check:
- If the character is the expected letter for the current state’s group, we allow it (state stays).
- If the character is the next letter in the sequence (e.g., current state = m group, char = 'e'), we move to the next state.
- If the character is any earlier letter (e.g., 'm' after we already passed to 'e' group) or an invalid character, we immediately return false.
At the end, we must have already seen at least one character for each of the four groups (state must be exactly 4, meaning we reached the w group and finished). Also, the string length must be at least 4 because each group has at least one character; but this is naturally checked by state transitions (can't reach final state with fewer than 4 transitions). However, we must ensure that a string like "m" gets false because state would be 1 at end, not 4. Edge cases: empty string, single character, case variations, characters like spaces, digits, or symbols. Time complexity is O(n), space O(1). We do not need any extra data structures.

#include <string>
#include <cctype>

// Checks if the given string follows the "meow" pattern.
// Valid pattern: all 'm'/'M' first, then all 'e'/'E', then all 'o'/'O', then all 'w'/'W'.
// Each group must be non-empty, and no other characters allowed.
bool isMeowSequence(const std::string& s) {
    // state 0 = start, 1 = inside m group, 2 = inside e group, 3 = inside o group, 4 = inside w group
    int state = 0;
    
    for (char ch : s) {
        char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        
        // Check if character is one of the allowed letters
        if (lower != 'm' && lower != 'e' && lower != 'o' && lower != 'w') {
            return false; // invalid character
        }
        
        if (state == 0) {
            // Expecting first 'm'
            if (lower != 'm') return false;
            state = 1;
        } else if (state == 1) {
            // In m group: can stay on m, or move to e
            if (lower == 'm') continue;
            if (lower == 'e') { state = 2; continue; }
            return false; // 'o' or 'w' or other
        } else if (state == 2) {
            // In e group: can stay on e, or move to o
            if (lower == 'e') continue;
            if (lower == 'o') { state = 3; continue; }
            return false;
        } else if (state == 3) {
            // In o group: can stay on o, or move to w
            if (lower == 'o') continue;
            if (lower == 'w') { state = 4; continue; }
            return false;
        } else { // state == 4
            // In w group: only w allowed
            if (lower == 'w') continue;
            return false;
        }
    }
    
    // All four groups must have at least one character, meaning state must be exactly 4
    return state == 4;
}

#include <cassert>
#include <string>

// The function isMeowSequence is assumed to be declared above.

int main() {
    // Basic valid cases
    assert(isMeowSequence("meow") == true);
    assert(isMeowSequence("MMMeeeoowWW") == true);
    assert(isMeowSequence("mEoW") == true);
    assert(isMeowSequence("mmeoww") == true);
    assert(isMeowSequence("mmmmeeeeeoooooowwwww") == true);
    
    // Invalid: missing groups or wrong order
    assert(isMeowSequence("meo") == false);          // missing w
    assert(isMeowSequence("emow") == false);         // starts with e
    assert(isMeowSequence("mmeoow") == false);       // m and e mixed
    assert(isMeowSequence("mewo") == false);         // e then w before o
    assert(isMeowSequence("mw") == false);           // missing e and o
    
    // Invalid: extra characters or empty
    assert(isMeowSequence("meowx") == false);        // extra x
    assert(isMeowSequence("meow1") == false);        // digit
    assert(isMeowSequence(" meow") == false);        // space
    assert(isMeowSequence("") == false);             // empty
    
    // Invalid: only one group
    assert(isMeowSequence("m") == false);
    assert(isMeowSequence("w") == false);
    
    // Valid: case variations
    assert(isMeowSequence("MEOW") == true);
    assert(isMeowSequence("MmEeOoWw") == true);
    
    return 0;
}
