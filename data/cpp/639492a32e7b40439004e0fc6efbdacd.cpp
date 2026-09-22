// Write a C++ function named `transformCaseAlternating` that takes a single non-empty string by reference (without `const`) and modifies it in place so that every lowercase letter becomes uppercase and every uppercase letter becomes lowercase, while leaving all non-alphabetic characters (digits, spaces, punctuation) completely unchanged. The function must preserve the original string length and order of characters. After calling the function, the caller should be able to inspect the modified string directly. The task requires using only standard C++ libraries and must handle any input string that may contain mixed-case letters, numbers, symbols, and whitespace.

The solution iterates over each character of the input string using its index within a standard `for` loop. For each character, it checks whether it is a lowercase letter via `std::islower` (which requires casting to `unsigned char` to avoid undefined behavior for negative char values), and if so, converts it to uppercase using `std::toupper`. Similarly, if it is an uppercase letter via `std::isupper`, it converts to lowercase using `std::tolower`. Non-alphabetic characters are ignored because neither condition matches. Since the modification is done in place on a string passed by reference, no extra storage is needed, and the original ordering and length are naturally preserved. Edge cases include strings with no letters (function does nothing), strings with only lowercase or only uppercase letters, strings containing digits and punctuation that are left untouched, and the empty string (though the task specifies non-empty, the loop simply doesn't execute). The time complexity is O(n), where n is the string length, since each character is examined once. The space complexity is O(1) auxiliary, as no additional data structures are used beyond loop variables and the input string itself.

#include <string>
#include <cctype>

// Modifies the input string in place, toggling the case of every alphabetic character.
// Lowercase letters become uppercase, uppercase letters become lowercase, and all
// non-alphabetic characters remain unchanged.
void transformCaseAlternating(std::string& str) {
    for (std::size_t i = 0; i < str.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        if (std::islower(c)) {
            str[i] = static_cast<char>(std::toupper(c));
        } else if (std::isupper(c)) {
            str[i] = static_cast<char>(std::tolower(c));
        }
    }
}

#include <cassert>
#include <string>

int main() {
    std::string s1 = "Hello World!";
    transformCaseAlternating(s1);
    assert(s1 == "hELLO wORLD!");

    std::string s2 = "abc123";
    transformCaseAlternating(s2);
    assert(s2 == "ABC123");

    std::string s3 = "XYZ-xyz_9";
    transformCaseAlternating(s3);
    assert(s3 == "xyz-XYZ_9");

    std::string s4 = "MiXeD CaSe";
    transformCaseAlternating(s4);
    assert(s4 == "mIxEd cAsE");

    std::string s5 = "123 456 !@#";
    transformCaseAlternating(s5);
    assert(s5 == "123 456 !@#");

    std::string s6 = "A";
    transformCaseAlternating(s6);
    assert(s6 == "a");

    std::string s7 = "z";
    transformCaseAlternating(s7);
    assert(s7 == "Z");

    std::string s8 = "Hello, 2024!";
    transformCaseAlternating(s8);
    assert(s8 == "hELLO, 2024!");

    std::string s9 = "   ";
    transformCaseAlternating(s9);
    assert(s9 == "   ");

    std::string s10 = "ThE qUiCk BrOwN fOx";
    transformCaseAlternating(s10);
    assert(s10 == "tHe QuIcK bRoWn FoX");
    
    return 0;
}
