Write a C++ function named `reverseIfLong` that takes a single `std::string` argument (guaranteed non‑empty, containing only lowercase English letters) and returns a string. If the input string has exactly 2 characters, the function must return the original string unchanged. For any other length (including 1, 3, or more), the function must return the string with its characters reversed (i.e., the last character first, then the second‑last, etc.). The function must be const‑correct and should not modify the input.
The solution is straightforward: check the length of the input string using `size()`. If `size() == 2`, return the original string as is. Otherwise, construct a reversed version by iterating from the last character backwards to the first and appending each character to a new result string. Edge cases:  
- An empty string is not allowed per the specification, but if given, `size() == 0` falls into the "otherwise" branch and returns an empty string (which is correct since reversing an empty string yields an empty string).  
- A 1‑character string returns itself reversed (same string).  
- Strings with length ≥ 3 reverse correctly.  
Time complexity is O(n) because we create a new string of length n and copy all characters. Space complexity is O(n) for the output string; no extra auxiliary data structures are used.
#include <string>

// Return s unchanged if its length is exactly 2; otherwise return s reversed.
std::string reverseIfLong(const std::string& s) {
    if (s.size() == 2) {
        return s;
    }

    std::string result;
    result.reserve(s.size());  // avoid reallocations
    for (std::size_t i = s.size(); i > 0; --i) {
        result.push_back(s[i - 1]);
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test (normally in a header).
std::string reverseIfLong(const std::string& s);

int main() {
    assert(reverseIfLong("ab") == "ab");
    assert(reverseIfLong("a") == "a");
    assert(reverseIfLong("abc") == "cba");
    assert(reverseIfLong("abcd") == "dcba");
    assert(reverseIfLong("hello") == "olleh");
    assert(reverseIfLong("xy") == "xy");
    assert(reverseIfLong("") == "");
    assert(reverseIfLong("racecar") == "racecar");
    assert(reverseIfLong("123") == "321");  // digits are allowed? Spec says lowercase letters but function works generally.
    assert(reverseIfLong("xyzzy") == "yzzxy");
    return 0;
}
