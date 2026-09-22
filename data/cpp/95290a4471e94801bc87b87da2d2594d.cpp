// Write a C++ function named `classifyCharacter` that takes a single `char` parameter and returns a `std::string` describing the character's category. The function must classify the character into exactly one of four categories: "digit" if the character is between '0' and '9' inclusive, "lower case letter" if between 'a' and 'z' inclusive, "upper case letter" if between 'A' and 'Z' inclusive, and "non-alphanumeric character" for any other character. The returned string must be exactly one of these four phrases, with no extra formatting or punctuation. The function should be `const`-correct (i.e., the parameter is passed by value, so no `const` reference is needed, but the returned string is `const`-qualified where appropriate). The function must not use any external libraries beyond the standard C++ headers (e.g., `<string>`), and must not print anything to the console. Ensure the logic handles all possible `char` values, including digits, letters, punctuation, whitespace, and any extended ASCII characters if `char` is signed.

The solution uses simple character comparisons based on ASCII values. Since the input is a single character, we can directly compare it against the ranges for digits (`'0'` to `'9'`), lowercase letters (`'a'` to `'z'`), and uppercase letters (`'A'` to `'Z'`). Each comparison is a simple integer comparison because `char` is implicitly convertible to `int` in C++. The order of checks does not matter because the ranges are mutually exclusive. The edge cases include: the boundaries of each range (e.g., `'0'`, `'9'`, `'a'`, `'z'`, `'A'`, `'Z'`), characters like `'/'` (just below `'0'`), `':'` (just above `'9'`), and non-alphanumeric characters such as space, `'!'`, `'\n'`, or `'\0'`. The function returns a string, so we can return a string literal directly. Time complexity is O(1) because we perform a constant number of comparisons. Space complexity is O(1) for the computation, though the returned string allocates memory for the phrase, which is constant size.

#include <string>

// Classify a character into one of four categories and return the category name.
std::string classifyCharacter(const char c) {
    if (c >= '0' && c <= '9') {
        return "digit";
    }
    if (c >= 'a' && c <= 'z') {
        return "lower case letter";
    }
    if (c >= 'A' && c <= 'Z') {
        return "upper case letter";
    }
    return "non-alphanumeric character";
}

#include <cassert>
#include <string>

// The solution function declaration is assumed to be available.
std::string classifyCharacter(const char c);

int main() {
    // Digits
    assert(classifyCharacter('0') == "digit");
    assert(classifyCharacter('5') == "digit");
    assert(classifyCharacter('9') == "digit");

    // Lowercase letters
    assert(classifyCharacter('a') == "lower case letter");
    assert(classifyCharacter('m') == "lower case letter");
    assert(classifyCharacter('z') == "lower case letter");

    // Uppercase letters
    assert(classifyCharacter('A') == "upper case letter");
    assert(classifyCharacter('K') == "upper case letter");
    assert(classifyCharacter('Z') == "upper case letter");

    // Non-alphanumeric characters
    assert(classifyCharacter(' ') == "non-alphanumeric character");
    assert(classifyCharacter('!') == "non-alphanumeric character");
    assert(classifyCharacter('/') == "non-alphanumeric character");
    assert(classifyCharacter(':') == "non-alphanumeric character");
    assert(classifyCharacter('\n') == "non-alphanumeric character");
    assert(classifyCharacter('\0') == "non-alphanumeric character");

    return 0;
}
