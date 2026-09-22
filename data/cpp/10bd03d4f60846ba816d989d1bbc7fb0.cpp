/*
Write a C++ function `makeFancyString` that takes a non-empty string `s` consisting of lowercase English letters and returns a "fancy" version of it where no three consecutive characters are identical. The function should preserve the original order of characters and only remove characters as needed to eliminate any run of three or more identical characters, making the resulting string as close to the original as possible (i.e., remove the minimum number of characters). For example, the input `"aaabaaaa"` should become `"aabaa"` (the first run of three `'a'` is reduced to two, and the later run of four `'a'` is reduced to two), and `"aab"` should remain unchanged because it contains no three identical consecutive characters.
*/

#include <string>

// Returns a version of s with no three consecutive identical characters.
std::string makeFancyString(const std::string& s) {
    std::string result;
    int runLength = 0; // tracks consecutive identical characters in result
    
    for (char c : s) {
        if (!result.empty() && result.back() == c) {
            ++runLength;
        } else {
            runLength = 1;
        }
        
        if (runLength <= 2) {
            result.push_back(c);
        }
    }
    
    return result;
}

#include <cassert>
#include <string>

// Assume makeFancyString is declared above.

int main() {
    assert(makeFancyString("aaabaaaa") == "aabaa");
    assert(makeFancyString("aab") == "aab");
    assert(makeFancyString("a") == "a");
    assert(makeFancyString("ab") == "ab");
    assert(makeFancyString("aaa") == "aa");
    assert(makeFancyString("bbbbbb") == "bb");
    assert(makeFancyString("aabbcc") == "aabbcc");
    assert(makeFancyString("abcccd") == "abccd");
    assert(makeFancyString("") == "");
    assert(makeFancyString("aabaa") == "aabaa");
    return 0;
}

// The algorithm scans the string from left to right, building the result iteratively. We can process the input character by character and maintain a count of how many consecutive identical characters have been appended to the result so far. For each new character, if it is the same as the last character in the result, we increment the current run length; otherwise, we reset it to 1. If at any point the run length would become 3, we skip appending that character (since that would create three identical consecutive characters), and we also skip incrementing the run length further. This ensures we never append more than two identical characters in a row. Edge cases: input length less than 3 (including empty or single character) needs no processing—though the problem guarantees non-empty, the function should handle lengths 1 and 2 gracefully. Also, the algorithm works correctly when the string has multiple separate runs of identical characters separated by different characters. Time complexity is O(n) where n is the length of the input, because we make a single pass. Space complexity is O(n) for the output string, but auxiliary space (excluding output) is O(1) because we only track a few integers.
