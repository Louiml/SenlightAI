Write a C++ function named `classifyCharacter` that takes a single `char` parameter and returns a `std::string` indicating whether the character is classified as a "vowel" or "consonant" according to this rule: if the ASCII code of the character is even, classify it as a consonant; if the ASCII code is odd, classify it as a vowel. The function must not accept any input or produce output; it must purely compute and return the classification string. Ensure the function is `const`-correct by taking the parameter by value (since `char` is a simple type) and returning a `std::string` by value. The function should work for any valid `char` value, including punctuation, digits, and uppercase letters, following the same parity rule. Note that the original snippet's logic is intentionally flawed (it classifies based on ASCII parity, not actual vowels/consonants), and your task is to reproduce that same logical behavior in a clean, standalone function.

// The solution must inspect the integer value of the character (implicitly converted to `int` when using the `%` operator) and check its parity. If `(static_cast<int>(ch) % 2 == 0)`, return `"consonant"`; otherwise, if `(static_cast<int>(ch) % 2 == 1)`, return `"vowel"`. Because the modulus operation on a non-negative integer yields 0 or 1, and all `char` values are non-negative when cast to `int` (assuming standard unsigned `char` or signed `char` with values in 0–127 typically), both branches are mutually exclusive. Important edge cases: the character with ASCII code 0 (null character) is even and returns "consonant"; the character with ASCII code 127 (DEL) is odd and returns "vowel". For signed `char` implementations, the cast to `int` can yield negative values for characters with high bit set, but the rule in the original snippet uses `char1 % 2` which for negative numbers in C++ yields negative results: e.g., -1 % 2 = -1, and neither `== 0` nor `== 1` is true, so no output would occur in the original. To be robust and match the intended behavior, we cast to `unsigned char` first to ensure non-negative values, then compute parity. This guarantees a definitive classification for every possible `char`. Time complexity is O(1), space complexity is O(1) (excluding the returned string).

#include <string>

// Classify a character as "vowel" if its ASCII code is odd, "consonant" if even.
// Uses unsigned char cast to guarantee non-negative values for parity checks.
std::string classifyCharacter(char ch) {
    unsigned char value = static_cast<unsigned char>(ch);
    if (value % 2 == 0) {
        return "consonant";
    }
    return "vowel";
}

#include <cassert>
#include <string>

// Declaration of the function under test (in a real scenario, this would be in a header)
std::string classifyCharacter(char ch);

int main() {
    // ASCII 'A' = 65 (odd) -> vowel
    assert(classifyCharacter('A') == "vowel");
    // ASCII 'a' = 97 (odd) -> vowel
    assert(classifyCharacter('a') == "vowel");
    // ASCII 'B' = 66 (even) -> consonant
    assert(classifyCharacter('B') == "consonant");
    // ASCII '0' = 48 (even) -> consonant
    assert(classifyCharacter('0') == "consonant");
    // ASCII '1' = 49 (odd) -> vowel
    assert(classifyCharacter('1') == "vowel");
    // ASCII '!' = 33 (odd) -> vowel
    assert(classifyCharacter('!') == "vowel");
    // ASCII ' ' (space) = 32 (even) -> consonant
    assert(classifyCharacter(' ') == "consonant");
    // ASCII 127 (DEL) if representable as char -> odd -> vowel
    // Note: char may be signed, but the cast to unsigned char ensures positive value.
    assert(classifyCharacter(static_cast<char>(127)) == "vowel");
    // ASCII 128 (if char is unsigned, else signed negative) -> even -> consonant
    // In either case, the cast to unsigned char yields 128, which is even.
    assert(classifyCharacter(static_cast<char>(128)) == "consonant");
    // ASCII 0 (null) -> even -> consonant
    assert(classifyCharacter(static_cast<char>(0)) == "consonant");
    return 0;
}
