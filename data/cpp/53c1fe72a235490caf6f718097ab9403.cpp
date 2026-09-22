Write a C++ function named `backspaceCompare` that takes two strings, `s` and `t`, where the `#` character represents a backspace (deleting the previous character if one exists). The function should return `true` if the two strings are equal after processing all backspaces, and `false` otherwise. The input strings may contain lowercase letters, digits, and `#` characters, and may be empty. A backspace at the beginning of the string (or after all characters have been deleted) has no effect. Your implementation must handle strings of any length up to 10^5 efficiently. Do not modify the input strings; use `const` references for parameters. The function must be declared as `bool backspaceCompare(const std::string& s, const std::string& t)`.
#include <cassert>
#include <string>

// Function declaration (for clarity; in practice, include the solution file)
bool backspaceCompare(const std::string& s, const std::string& t);

int main() {
    assert(backspaceCompare("ab#c", "ad#c") == true);
    assert(backspaceCompare("ab##", "c#d#") == true);
    assert(backspaceCompare("a##c", "#a#c") == true);
    assert(backspaceCompare("a#c", "b") == false);
    assert(backspaceCompare("", "") == true);
    assert(backspaceCompare("#", "") == true);
    assert(backspaceCompare("a###", "") == true);
    assert(backspaceCompare("abc#d", "ab#cd") == false);
    assert(backspaceCompare("xy#z", "xzz#") == true);
    assert(backspaceCompare("bxj##tw", "bxo#j##tw") == true);
    
    return 0;
}
#include <string>

// Compare two strings after processing backspace ('#') characters.
// Returns true if the resulting strings are identical, false otherwise.
bool backspaceCompare(const std::string& s, const std::string& t) {
    std::string resultS;
    resultS.reserve(s.size()); // optional: avoids reallocations
    
    for (char c : s) {
        if (c == '#') {
            if (!resultS.empty()) {
                resultS.pop_back();
            }
        } else {
            resultS.push_back(c);
        }
    }
    
    std::string resultT;
    resultT.reserve(t.size()); // optional: avoids reallocations
    
    for (char c : t) {
        if (c == '#') {
            if (!resultT.empty()) {
                resultT.pop_back();
            }
        } else {
            resultT.push_back(c);
        }
    }
    
    return resultS == resultT;
}
// The solution processes each string independently by simulating a stack: iterate through each character from left to right. If the character is not `#`, push it onto a result string (acting as a stack). If it is `#`, pop the last character from the result string if the string is not empty; otherwise, do nothing. After processing both strings completely, compare the two resulting strings for equality. This approach correctly handles multiple consecutive backspaces, backspaces at the start, and cases where backspaces delete previously added characters. Edge cases include empty strings, strings with only backspaces (resulting in empty strings), and strings where backspaces remove all characters. Time complexity is O(n + m), where n and m are the lengths of the input strings, because each character is processed exactly once. Space complexity is O(n + m) in the worst case for the two temporary result strings, though this can be reduced to O(1) with two-pointer iteration from the end (but the given approach is simpler and still meets typical constraints). The solution uses `std::string` as a dynamic array for push_back and pop_back, which are amortized O(1).
