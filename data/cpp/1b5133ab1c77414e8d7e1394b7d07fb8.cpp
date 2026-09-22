Given a non-empty string `s` consisting only of lowercase and uppercase English letters (no spaces or other characters), write a C++ function `adjustCase(const std::string& input)` that returns a new string where all letters are converted to uppercase if the number of uppercase letters is strictly greater than the number of lowercase letters; otherwise, all letters are converted to lowercase. The original input string must not be modified. If the counts are equal, convert all letters to lowercase. The function must handle arbitrary lengths up to a few thousand characters and should work correctly for single-character strings and strings already all in one case.
// The solution counts the number of uppercase and lowercase characters in the input string. Uppercase letters are ASCII values 65–90 (`'A'`–`'Z'`), and lowercase are 97–122 (`'a'`–`'z'`). After counting, compare the counts: if `upper > lower`, transform every lowercase letter to uppercase by subtracting 32 from its ASCII value; otherwise (including when `lower >= upper`), transform every uppercase letter to lowercase by adding 32. Since the transformation is applied based on ASCII arithmetic, no external libraries or locale-specific functions are needed. Edge cases include strings with only one character, strings with equal counts, and strings already in the target case (where the loop makes no changes). Time complexity is O(n) for the counting pass plus O(n) for the conversion pass, totaling O(n) where n is the string length. Space complexity is O(n) for the returned string (since we create a copy to avoid modifying the input), but auxiliary space is O(1) beyond the output string.
#include <string>

// Return a modified copy of the input where all letters are converted to uppercase
// if uppercase letters are more numerous than lowercase, otherwise to lowercase.
std::string adjustCase(const std::string& input) {
    int upperCount = 0;
    int lowerCount = 0;

    // Count uppercase and lowercase letters
    for (char c : input) {
        if (c >= 'A' && c <= 'Z') {
            ++upperCount;
        } else {
            ++lowerCount;
        }
    }

    std::string result = input;
    if (upperCount > lowerCount) {
        // Convert all lowercase to uppercase
        for (char& c : result) {
            if (c >= 'a' && c <= 'z') {
                c = c - 32;
            }
        }
    } else {
        // Convert all uppercase to lowercase (including when counts are equal)
        for (char& c : result) {
            if (c >= 'A' && c <= 'Z') {
                c = c + 32;
            }
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Assume adjustCase is declared above (or include the header)

int main() {
    // Basic mixed case where uppercase dominates
    assert(adjustCase("aBcDeF") == "ABCDEF");
    // Lowercase dominates
    assert(adjustCase("AbCdEf") == "abcdef");
    // Equal counts → lowercase
    assert(adjustCase("aB") == "ab");
    // All uppercase input → stays uppercase
    assert(adjustCase("HELLO") == "HELLO");
    // All lowercase input → stays lowercase
    assert(adjustCase("world") == "world");
    // Single uppercase letter
    assert(adjustCase("Z") == "Z");
    // Single lowercase letter
    assert(adjustCase("z") == "z");
    // Equal counts with many letters
    assert(adjustCase("aAbBcC") == "aabbcc");
    // Large string: 1000 'a' and 999 'A' → lowercase wins
    std::string big(1000, 'a');
    big += std::string(999, 'A');
    std::string expected(1999, 'a');
    assert(adjustCase(big) == expected);
    // Original input not modified
    std::string original = "MiXeD";
    std::string result = adjustCase(original);
    assert(original == "MiXeD");
    assert(result == "MIXED");
}
