/*
Write a C++ function `removeCharacter` that takes two strings as input: `my_string` (the original text) and `letter` (a single-character string). The function must return a new string that contains every character from `my_string` except all occurrences of the single character specified by `letter`. For example, if `my_string` is `"banana"` and `letter` is `"a"`, the result should be `"bnn"`. The input can be empty, and `letter` is guaranteed to contain exactly one character (though you should still handle it robustly). Do not modify the original strings—return a newly constructed string. The function signature should be: `std::string removeCharacter(const std::string& my_string, const std::string& letter)`.
*/
#include <string>

// Removes all occurrences of the single character represented by `letter` from `my_string`.
// Returns a new string without modifying the input.
std::string removeCharacter(const std::string& my_string, const std::string& letter) {
    std::string result;
    // If letter is empty (unexpected per spec), return my_string as is.
    if (letter.empty()) {
        return my_string;
    }
    char target = letter[0];
    for (char c : my_string) {
        if (c != target) {
            result.push_back(c);
        }
    }
    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(removeCharacter("banana", "a") == "bnn");
    assert(removeCharacter("hello", "l") == "heo");
    assert(removeCharacter("abc", "x") == "abc");
    assert(removeCharacter("aaaa", "a") == "");
    assert(removeCharacter("", "a") == "");

    // Single character input
    assert(removeCharacter("x", "x") == "");
    assert(removeCharacter("x", "y") == "x");

    // Case sensitivity
    assert(removeCharacter("AaA", "A") == "a");
    assert(removeCharacter("AaA", "a") == "AA");

    // Letter may be given as a multi-character string, but only first char is used
    assert(removeCharacter("test", "te") == "st"); // 't' removed
    assert(removeCharacter("space", "sp") == "ace");

    // Whitespace handling
    assert(removeCharacter("a b c", " ") == "abc");
}
// The solution iterates through each character in `my_string` and compares it with the first character of `letter` (i.e., `letter[0]`). If the characters are equal, skip that character; otherwise, append it to the result string. Since the function takes `letter` as a string but only uses its first character, we can index it directly. Edge cases: (1) an empty `my_string` returns an empty string; (2) if `letter` is empty (though not expected per spec), `letter[0]` would be out of bounds, so we can guard by checking `!letter.empty()` or just assume it's non-empty per the problem statement; (3) if `my_string` contains no occurrence of the target character, the result equals the original string; (4) all characters removed results in an empty string. Time complexity is O(n) where n is the length of `my_string`, as we traverse each character once. Space complexity is O(n) for the resulting string (plus O(1) auxiliary space for the loop and the answer string building).
