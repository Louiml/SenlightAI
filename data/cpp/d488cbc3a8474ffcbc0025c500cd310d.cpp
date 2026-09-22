/*
Write a C++ function that accepts a non-empty string containing a sequence of lowercase or uppercase English letters (no spaces or other characters) and returns an integer representing the cumulative "typing cost" for that string on a QWERTY keyboard. The cost is computed as follows: typing the first character costs 2 units. For each subsequent character, the cost is 4 units if the current character and the previous character are typed with the same hand (both left-hand keys: `qwertasdfgzxcvb` and their uppercase counterparts, or both right-hand keys: `yuiophjklnm` and their uppercase counterparts), otherwise the cost is 2 units. The input string is guaranteed to be non-empty and contain only alphabetic characters. The function should return the total cost as an int.
*/
#include<string>
#include<cstddef>

// Returns the total typing cost for a non-empty alphabetic string on a QWERTY keyboard.
// Cost: 2 for the first character, then 4 if consecutive chars use the same hand, else 2.
int typingCost(const std::string& s) {
    // Lookup table: true means right-hand key, false means left-hand key.
    bool hand[256] = {false};

    // Left-hand keys (lowercase and uppercase) -> false
    const std::string left = "qwertasdfgzxcvbQWERTASDFGZXCVB";
    // Right-hand keys (lowercase and uppercase) -> true
    const std::string right = "yuiophjklnmYUIOPHJKLNM";

    for (char c : left) {
        hand[static_cast<unsigned char>(c)] = false;
    }
    for (char c : right) {
        hand[static_cast<unsigned char>(c)] = true;
    }

    int total = 2; // cost for the first character
    for (std::size_t i = 1; i < s.size(); ++i) {
        bool current = hand[static_cast<unsigned char>(s[i])];
        bool previous = hand[static_cast<unsigned char>(s[i - 1])];
        total += (current == previous) ? 4 : 2;
    }
    return total;
}
#include<cassert>
#include<string>

// Free function declared above (typingCost) must be included before this.

int main() {
    // Single character
    assert(typingCost("a") == 2);
    assert(typingCost("Z") == 2);

    // Two characters same hand (both left: q and w)
    assert(typingCost("qw") == 6); // 2 + 4

    // Two characters different hands (left a, right a? Actually a is left, ; not allowed; use y as right)
    assert(typingCost("ay") == 4); // 2 + 2

    // Mixed case: 'Q' left, 'P' right, 'P' right
    assert(typingCost("QPP") == 8); // 2 + 2 + 4

    // Longer mixed string: 'h' right, 'e' left, 'l' right, 'l' right, 'o' right
    // h->e diff (2), e->l diff (2), l->l same (4), l->o same (4) => 2+2+2+4+4 = 14
    assert(typingCost("hello") == 14);

    // All same hand: "asdf" all left
    // 2 + 4 + 4 + 4 = 14
    assert(typingCost("asdf") == 14);

    // Alternating hands: "qypq" (left, right, left, right) -> 2+2+2+2 = 8
    assert(typingCost("qypq") == 8);

    // Uppercase and lowercase mix: "Aa" (both left) -> 6, "Uu" (both right) -> 6
    assert(typingCost("Aa") == 6);
    assert(typingCost("Uu") == 6);

    // Long same hand: "zzzz" all left? z is left, so 2 + 4*3 = 14
    assert(typingCost("zzzz") == 14);
}
// The solution requires a static lookup table that maps each possible alphabetic character (A-Z, a-z) to a boolean indicating whether it is typed with the right hand (true) or left hand (false). Build this table once using two strings: all left-hand keys (lowercase and uppercase) are false, all right-hand keys are true. Then iterate through the input string starting from the second character (index 1), comparing the hand flag of the current character with that of the previous character. If they match, add 4; otherwise, add 2. Initialize the total with 2 (cost for the first character). Edge cases: the string may be a single character (returns 2), may contain mixed case, and may have repeated characters. The algorithm runs in O(n) time where n is the string length, and uses O(1) auxiliary space (the fixed-size lookup table for 256 ASCII values, though only 52 entries are actually used).
