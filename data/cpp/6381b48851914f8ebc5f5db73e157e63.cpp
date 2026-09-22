/*
Create a C++ function `reverseString` that takes a non-const `std::string&` reference (as in the original snippet) and returns a new string containing the characters of the input string in reverse order. The function must preserve the original input string unchanged and must work correctly for empty strings, single-character strings, strings with whitespace, and strings containing any printable ASCII characters. The solution should not use any built-in reverse functions, and the reversal must be performed using explicit character-by-character concatenation.

##
*/
#include <string>

// Reverses the order of characters in the given string.
// The original string is not modified; a new reversed string is returned.
std::string reverseString(std::string& s) {
    const int n = static_cast<int>(s.size());
    std::string ans;
    ans.reserve(n);

    for (int i = 0; i < n; ++i) {
        ans += s[n - i - 1];
    }

    return ans;
}

##
#include <cassert>
#include <string>

// Assume reverseString is defined above

int main() {
    std::string s1 = "hello";
    assert(reverseString(s1) == "olleh");
    assert(s1 == "hello");  // original not modified

    std::string s2 = "";
    assert(reverseString(s2) == "");

    std::string s3 = "a";
    assert(reverseString(s3) == "a");

    std::string s4 = "12345";
    assert(reverseString(s4) == "54321");

    std::string s5 = "a b c";
    assert(reverseString(s5) == "c b a");

    std::string s6 = "racecar";
    assert(reverseString(s6) == "racecar");

    std::string s7 = "!@# $%^";
    assert(reverseString(s7) == "^%$ #@!");

    std::string s8 = "abcdefghijklmnopqrstuvwxyz";
    assert(reverseString(s8) == "zyxwvutsrqponmlkjihgfedcba");
}
// The algorithm iterates through the input string from index 0 to n-1, where n is the length of the string. For each index `i`, it appends the character at position `n - i - 1` (the mirrored position from the end) to a new `std::string ans`. This constructs the reversed string in forward order. For an empty string, the loop does not execute, and an empty string is returned. For a single character, the loop appends that character once, producing the same string. Time complexity is O(n) because each character is visited exactly once and each concatenation operation is amortized O(1). Space complexity is O(n) due to the new string containing n characters, with no additional significant auxiliary space beyond that.
//
// ##
