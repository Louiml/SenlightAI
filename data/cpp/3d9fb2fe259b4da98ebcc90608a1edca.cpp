/*
Write a C++ function that takes a single string as input and returns a struct (or a tuple-like result) containing three integers: the count of uppercase letters ('A'-'Z'), lowercase letters ('a'-'z'), and digits ('0'-'9') in that string. The string may contain any printable ASCII characters, including spaces, punctuation, and symbols, but only letters and digits are counted. The input string will be non-empty, and you must handle both uppercase and lowercase separately. Your solution must not use any external libraries beyond `<string>` and `<cstddef>` (or `<iostream>` if needed for testing); in particular, avoid using `std::isupper`, `std::islower`, `std::isdigit` from `<cctype>` — implement the character range checks manually. The function should be `const`-correct, taking the input by `const std::string&` and returning the counts in a `struct CharacterCounts` with fields `uppercase`, `lowercase`, and `digits`.
*/
#include <string>

struct CharacterCounts {
    int uppercase;
    int lowercase;
    int digits;
};

// Count uppercase letters, lowercase letters, and digits in the given string.
CharacterCounts countCharacterTypes(const std::string& input) {
    CharacterCounts counts = {0, 0, 0};
    
    for (char c : input) {
        if (c >= 'A' && c <= 'Z') {
            counts.uppercase++;
        } else if (c >= 'a' && c <= 'z') {
            counts.lowercase++;
        } else if (c >= '0' && c <= '9') {
            counts.digits++;
        }
    }
    
    return counts;
}
#include <cassert>

int main() {
    // Test 1: Simple mixed case and digits
    CharacterCounts r1 = countCharacterTypes("Hello World 123");
    assert(r1.uppercase == 2);  // 'H' and 'W'
    assert(r1.lowercase == 8);  // "ello", "orld"
    assert(r1.digits == 3);     // "123"

    // Test 2: Only uppercase
    CharacterCounts r2 = countCharacterTypes("ABCD");
    assert(r2.uppercase == 4);
    assert(r2.lowercase == 0);
    assert(r2.digits == 0);

    // Test 3: Only lowercase
    CharacterCounts r3 = countCharacterTypes("xyz");
    assert(r3.uppercase == 0);
    assert(r3.lowercase == 3);
    assert(r3.digits == 0);

    // Test 4: Only digits
    CharacterCounts r4 = countCharacterTypes("9876543210");
    assert(r4.uppercase == 0);
    assert(r4.lowercase == 0);
    assert(r4.digits == 10);

    // Test 5: No alphanumeric characters (punctuation and spaces)
    CharacterCounts r5 = countCharacterTypes("!@#$%^&*() ");
    assert(r5.uppercase == 0);
    assert(r5.lowercase == 0);
    assert(r5.digits == 0);

    // Test 6: Edge boundary characters
    CharacterCounts r6 = countCharacterTypes("A0aZ9z");
    assert(r6.uppercase == 2);  // 'A' and 'Z'
    assert(r6.lowercase == 2);  // 'a' and 'z'
    assert(r6.digits == 2);     // '0' and '9'

    // Test 7: Empty string
    CharacterCounts r7 = countCharacterTypes("");
    assert(r7.uppercase == 0);
    assert(r7.lowercase == 0);
    assert(r7.digits == 0);
}
// The algorithm is straightforward: iterate over each character in the input string once, and for each character, test whether it falls in one of three ASCII ranges: 'A' to 'Z' (uppercase), 'a' to 'z' (lowercase), or '0' to '9' (digits). Since these ranges are contiguous in ASCII, comparisons like `c >= 'A' && c <= 'Z'` are sufficient and reliable. Increment the corresponding counter when a match is found; characters outside these ranges (spaces, punctuation, etc.) are ignored. Edge cases include an empty string (though the task says non-empty, handle it gracefully by returning all zeros), a string with only non-alphanumeric characters (all counters remain zero), and strings with mixed case and digits. Time complexity is O(n), where n is the string length, because each character is examined exactly once. Space complexity is O(1) — only three integer counters and the returned struct are used, independent of input size.
