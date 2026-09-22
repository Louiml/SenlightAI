Write a C++ function named `dishHeight` that takes a non-empty string `diamonds` consisting only of uppercase characters `'D'` and `'U'`, where each character represents one dish placed on a stack from bottom to top. When the stack is built, the first dish has a height of 10 units, and each subsequent dish adds 10 units to the total height if its orientation differs from the previous dish, but adds only 5 units if it has the same orientation as the previous dish (i.e., both `'D'` or both `'U'`). The function should return the total height of the stack as an `int`. For example, a stack with sequence `"DUD"` has height 25 (10 + 10 + 5), and a stack with sequence `"DD"` has height 15.

// The algorithm iterates through the string once. Initialize the sum with the height of the first dish, which is always 10. Then for every index from 1 to `len-1`, compare the current character with the previous character using `s[i] == s[i-1]`; if equal, add 5, otherwise add 10. This directly applies the rule. Edge cases: The string is guaranteed non-empty, so we can safely access `s[0]`. If the string has length 1, the loop does not execute, and the result is 10. The time complexity is O(n) where n is the length of the string, and the space complexity is O(1) additional memory.

#include <string>

// Compute the total height of a stack of dishes described by a string of 'D'/'U'.
// The first dish is 10 units tall; each subsequent dish adds 10 if different from
// the previous, else 5 if same.
int dishHeight(const std::string& diamonds) {
    int total = 10;  // first dish always adds 10
    for (std::size_t i = 1; i < diamonds.size(); ++i) {
        if (diamonds[i] == diamonds[i - 1]) {
            total += 5;   // same orientation
        } else {
            total += 10;  // different orientation
        }
    }
    return total;
}

#include <cassert>

int main() {
    // Basic sequences
    assert(dishHeight("D") == 10);
    assert(dishHeight("U") == 10);
    assert(dishHeight("DD") == 15);
    assert(dishHeight("DU") == 20);
    assert(dishHeight("DUD") == 25);
    assert(dishHeight("UDU") == 25);
    // Longer and mixed
    assert(dishHeight("DDUU") == 30);  // 10 + 5 + 10 + 5
    assert(dishHeight("UDUD") == 40);  // 10 + 10 + 10 + 10
    assert(dishHeight("DDD") == 20);   // 10 + 5 + 5
    assert(dishHeight("UUU") == 20);   // 10 + 5 + 5
    return 0;
}
