// Write a C++ function that takes a non-negative integer `age` (given as an `int`) and returns a `std::string` where each decimal digit of the number is replaced by the corresponding lowercase English letter, starting with 'a' for digit 0, 'b' for 1, ..., 'j' for 9. For example, the number `23` should produce `"cd"`, and `10` should produce `"ba"`. The input is guaranteed to be between 0 and 1000 inclusive, inclusive of 0, which maps to `"a"`. The function should be named descriptively (e.g., `ageToCode`) and take a `const int` or `int` parameter.

#include <cassert>
#include <string>

// The function declaration (included here for self-contained test).
std::string ageToCode(const int age);

int main() {
    assert(ageToCode(0) == "a");
    assert(ageToCode(1) == "b");
    assert(ageToCode(9) == "j");
    assert(ageToCode(10) == "ba");
    assert(ageToCode(23) == "cd");
    assert(ageToCode(99) == "jj");
    assert(ageToCode(100) == "baa");
    assert(ageToCode(1000) == "baaa");
    assert(ageToCode(7) == "h");
    assert(ageToCode(555) == "fff");
    return 0;
}

#include <string>

// Convert a non-negative integer's decimal digits to letters: 0->'a', 1->'b', ..., 9->'j'.
std::string ageToCode(const int age) {
    std::string digits = std::to_string(age);
    std::string result;
    result.reserve(digits.size());
    for (const char ch : digits) {
        const int digit = ch - '0';
        result += static_cast<char>('a' + digit);
    }
    return result;
}

// The solution converts the integer to its decimal string representation using `std::to_string(age)`. Then, for each character in that string, subtract the ASCII value of `'0'` to obtain the digit value (0–9). Add that digit value to the character `'a'` (ASCII 97) to get the corresponding letter ('a' + 0 = 'a', 'a' + 9 = 'j'). Append each letter to an empty result string. Edge cases: the input `0` converts to the string `"0"`, producing `"a"`. Numbers like `1000` convert to `"1000"` and produce `"baaa"`. No special handling for negative inputs is needed since the task specifies non-negative. Time complexity is O(d), where d is the number of digits (at most 4 for the range), and space complexity is O(d) for the converted string and result.
