// Write a C++ function named `removeDots` that takes a non-empty string `s` containing only lowercase English letters, digits, and dots (`.`) as input, and returns a new string that is identical to `s` except that all dot characters have been removed. For example, if `s = "a.b.c"`, the function should return `"abc"`. If `s` contains no dots, the returned string should equal the original. The function must preserve the original order of all non-dot characters. Ensure your solution uses `const` correctness for the input parameter.

#include <cassert>
#include <string>

// Function under test
std::string removeDots(const std::string& s);

int main() {
    assert(removeDots("a.b.c") == "abc");
    assert(removeDots("hello") == "hello");
    assert(removeDots("...") == "");
    assert(removeDots("1.2.3") == "123");
    assert(removeDots(".a.b.") == "ab");
    assert(removeDots("") == "");
    assert(removeDots("x.y.z") == "xyz");
    assert(removeDots("no dots here") == "no dots here");
    assert(removeDots("...a...") == "a");
    assert(removeDots("a") == "a");
    return 0;
}

#include <string>

// Removes all '.' characters from the input string.
std::string removeDots(const std::string& s) {
    std::string result;
    result.reserve(s.size()); // optional optimization to avoid reallocations
    for (char ch : s) {
        if (ch != '.') {
            result.push_back(ch);
        }
    }
    return result;
}

// The task is straightforward string filtering. Iterate through each character of the input string using a range-based `for` loop or index-based loop. For each character, check if it is not equal to `'.'`; if so, append it to a result string. Since the input is read-only, pass it as `const std::string&` to avoid copying and ensure const-correctness. The algorithm processes each character exactly once, so the time complexity is O(n) where n is the length of the input string. The space complexity is O(n) for the output string, which is necessary to store the filtered result. Edge cases include empty input (though task states non-empty), input with only dots (returns empty string), and input with no dots (returns original content). No special handling is needed for digits or letters since the condition is simply "not a dot".
