/*
Write a C++ function `commonPrefixPattern` that takes a vector of strings of equal length (all strings will have the same number of characters) and a non‑negative integer `n` (which is the number of strings), and returns a single string of the same length. For each character position `i`, if all strings at that position have exactly the same character, put that character in the result; otherwise, put a `'?'` in the result. The input strings contain only lowercase English letters (so no `'?'` appears in the input), and the vector will always contain at least one string. The function must not modify the input vector.
*/
#include <string>
#include <vector>

// Given a vector of equal-length strings, return a string where each position
// is the common character if all strings agree, or '?' if they differ.
std::string commonPrefixPattern(const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return "";
    }
    
    const std::size_t length = strings[0].size();
    std::string result(length, '?');
    
    for (std::size_t i = 0; i < length; ++i) {
        const char firstChar = strings[0][i];
        bool allSame = true;
        for (std::size_t j = 1; j < strings.size(); ++j) {
            if (strings[j][i] != firstChar) {
                allSame = false;
                break;
            }
        }
        if (allSame) {
            result[i] = firstChar;
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Normal case with varying characters
    std::vector<std::string> test1 = {"abc", "abc", "abc"};
    assert(commonPrefixPattern(test1) == "abc");
    
    // Case with differences at some positions
    std::vector<std::string> test2 = {"abc", "adc", "aec"};
    assert(commonPrefixPattern(test2) == "a?c");
    
    // Case with all positions differing
    std::vector<std::string> test3 = {"ab", "cd", "ef"};
    assert(commonPrefixPattern(test3) == "??");
    
    // Single string: output identical to input
    std::vector<std::string> test4 = {"hello"};
    assert(commonPrefixPattern(test4) == "hello");
    
    // Mixed: first two same, third differs
    std::vector<std::string> test5 = {"xyz", "xyz", "xyw"};
    assert(commonPrefixPattern(test5) == "xy?");
    
    // Case where strings are empty (length 0) but at least one string exists
    std::vector<std::string> test6 = {""};
    assert(commonPrefixPattern(test6) == "");
    
    // Two strings with one difference
    std::vector<std::string> test7 = {"cat", "cot"};
    assert(commonPrefixPattern(test7) == "c?t");
    
    // Many strings, all same
    std::vector<std::string> test8 = {"a", "a", "a", "a"};
    assert(commonPrefixPattern(test8) == "a");
    
    // Many strings, all different at each position
    std::vector<std::string> test9 = {"ab", "cd", "ef", "gh"};
    assert(commonPrefixPattern(test9) == "??");
    
    // Edge: single character
    std::vector<std::string> test10 = {"z", "z", "y"};
    assert(commonPrefixPattern(test10) == "?");
    
    return 0;
}
// The solution is straightforward: iterate over the character positions from `0` to `length-1`, and for each position, compare the character from the first string with the corresponding character in every other string. If all match, set the result character to that character; otherwise, set it to `'?'`. Edge cases include `n == 1`, where every position is trivially all same, so the result is identical to the single input string. The length of the result is always equal to the length of the first string (and all strings are guaranteed equal length). Time complexity is O(n * L) where L is the string length, because we compare each of the n strings for each of the L positions. Space complexity is O(L) for the result string (plus O(1) auxiliary for loop variables and the temporary `char`).
