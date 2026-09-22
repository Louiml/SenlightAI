Write a C++ function `makeFancyString` that takes a string `s` as input and returns a new string where no three consecutive characters are identical. The function should remove characters from the original string as necessary to ensure that in the resulting string, every run of identical characters has length at most two. The relative order of the remaining characters must be preserved, and the function should handle empty strings, strings with only one or two characters, and strings with repeated characters at any position (beginning, middle, end). For example, `"aaabaaaa"` should become `"aabaa"`. The function should not modify the input string and should be efficient in both time and space.

// The solution uses a greedy single-pass approach. We iterate through each character in the input string, maintaining a result string. For each incoming character `c`, we check whether the result string already ends with two identical occurrences of `c` (i.e., the last two characters of the result are both equal to `c`). If so, adding `c` would create three consecutive identical characters, so we skip it. Otherwise, we append `c` to the result. This guarantees that the result never has more than two consecutive identical characters.  
// Edge cases:  
// - Empty string: loop doesn't execute, returns empty string.  
// - String length 1 or 2: every character is appended because `res.size()` is less than 2, so the condition is never true.  
// - All identical characters (e.g., `"aaaa"`): output becomes `"aa"`.  
// - Mixed patterns like `"aaabaaa"` → `"aabaa"`.  
// Time complexity: O(n), where n is the length of the input string. Space complexity: O(n) for the result string (the maximum possible size of the result is the same as the input). The auxiliary space excluding the output is O(1).

#include <string>

// Return a fancy string where no three consecutive characters are identical.
// The function preserves the relative order of the remaining characters.
std::string makeFancyString(const std::string& s) {
    std::string result;
    for (char c : s) {
        // If the last two characters in the result are both equal to c,
        // adding c would create three identical consecutive characters, so skip it.
        if (result.size() >= 2 && result.back() == c && result[result.size() - 2] == c) {
            continue;
        }
        result.push_back(c);
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    assert(makeFancyString("") == "");
    assert(makeFancyString("a") == "a");
    assert(makeFancyString("ab") == "ab");
    assert(makeFancyString("aa") == "aa");
    assert(makeFancyString("aaa") == "aa");
    assert(makeFancyString("aaabaaaa") == "aabaa");
    assert(makeFancyString("aabbaa") == "aabbaa");
    assert(makeFancyString("aaabbbccc") == "aabbcc");
    assert(makeFancyString("zzzxxxyyy") == "zzxxyy");
    assert(makeFancyString("abcddcba") == "abcddcba");
    return 0;
}
