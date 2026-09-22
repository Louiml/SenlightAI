// Write a C++ function that takes a non-empty string consisting only of lowercase letters (a-z) and returns a new string where a single space is inserted immediately before the character at 0-based index 4, but only if that index exists (i.e., if the string length is at least 5). If the string length is less than 5, return the original string unchanged. For example, for the input "hello", the output is "hell o". For input "cat", the output is "cat". The function must not modify the input, and must preserve all original characters and their order except for the insertion.
// The solution iterates through the characters of the input string and builds the output string. For each position i from 0 to length-1, we check if i equals 4 (the fifth character) and if the string length is at least 5 (which is guaranteed if i==4 exists). If that condition holds, we append a space to the result first, then append the current character. If not, we just append the character. The edge case is when the string length is less than 5, in which case the condition i==4 is never satisfied, so the function simply returns the original string. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) for the output string (which is required to return a new string). No special error handling is needed beyond checking the length implicitly via the index.
#include <string>

// Insert a space before the character at index 4 if it exists.
std::string insertSpaceAtIndex4(const std::string& input) {
    std::string result;
    const std::size_t length = input.length();
    
    for (std::size_t i = 0; i < length; ++i) {
        if (i == 4) {
            result.push_back(' ');
        }
        result.push_back(input[i]);
    }
    return result;
}
#include <cassert>
#include <string>

int main() {
    assert(insertSpaceAtIndex4("hello") == "hell o");
    assert(insertSpaceAtIndex4("world") == "worl d");
    assert(insertSpaceAtIndex4("cat") == "cat");
    assert(insertSpaceAtIndex4("two") == "two");
    assert(insertSpaceAtIndex4("abcde") == "abcd e");
    assert(insertSpaceAtIndex4("abcdefg") == "abcd efg");
    assert(insertSpaceAtIndex4("a") == "a");
    assert(insertSpaceAtIndex4("") == "");
    return 0;
}
