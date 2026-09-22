// Write a C++ function `string upperToLowerConversion(string input)` that takes a string of exactly five **uppercase** letters separated by the `|` character (e.g., `"A|B|C|D|E"`) and returns a string containing the corresponding five **lowercase** letters concatenated together without separators, followed by an exclamation mark `!`. The input is guaranteed to be exactly in the format of five uppercase letters with single `|` separators between them, and no leading/trailing whitespace. Assume only `A`–`Z` appear. The function should handle the conversion by adding 32 to each character's ASCII value (for uppercase to lowercase) and return the final result such as `"abcde!"`. Do not use `std::tolower` or any library conversion functions — implement the arithmetic conversion explicitly.

// The main algorithm is straightforward: we need to parse the input string to extract the five uppercase letters that appear at positions 0, 2, 4, 6, 8 (since every odd index is a `|` separator). For each such character, we add 32 to its ASCII value (since `'A'` is 65 and `'a'` is 97, the difference is 32) to convert it to lowercase. We then build a result string by appending each converted character, and finally append `'!'` at the end. Edge cases: the input length is always exactly 9 characters (`letter|letter|letter|letter|letter`), so no need to validate format; we must ensure we only process indices 0, 2, 4, 6, 8 and skip the separators. Complexity: O(1) time because the input size is fixed at 9 characters, and O(1) auxiliary space for the output string (which has fixed length 6). The function is `const`-correct by taking the input as a `const std::string&` and returning a new string.

#include <string>

// Given a string of five uppercase letters separated by '|', return the five
// lowercase letters concatenated followed by an exclamation mark.
// Example: "A|B|C|D|E" -> "abcde!"
std::string upperToLowerConversion(const std::string& input) {
    std::string result;
    result.reserve(6);  // 5 letters + '!'

    // The uppercase letters are at even indices (0,2,4,6,8) in the input.
    for (int i = 0; i < 9; i += 2) {
        char lower = static_cast<char>(static_cast<int>(input[i]) + 32);
        result.push_back(lower);
    }
    result.push_back('!');
    return result;
}

#include <cassert>

int main() {
    assert(upperToLowerConversion("A|B|C|D|E") == "abcde!");
    assert(upperToLowerConversion("Z|Y|X|W|V") == "zyxwv!");
    assert(upperToLowerConversion("A|A|A|A|A") == "aaaaa!");
    assert(upperToLowerConversion("M|N|O|P|Q") == "mnopq!");
    assert(upperToLowerConversion("H|E|L|L|O") == "hello!");
    assert(upperToLowerConversion("Q|W|E|R|T") == "qwert!");
    return 0;
}
