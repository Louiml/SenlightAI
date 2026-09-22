// Write a C++ function named `makeFancyString` that takes a string `s` (which may be empty or contain only lowercase English letters) and returns a "fancy" version of it by removing any character that would cause three or more consecutive identical characters to appear in the result. In other words, the returned string must not contain any substring of length 3 with all equal characters, but the relative order of the remaining characters must be preserved, and you should remove as few characters as possible (only the ones that would create a triple). For example, given `"aaabaaaa"`, the output should be `"aabaa"` because the first `aaa` becomes `aa`, and the later `aaaa` becomes `aa`. The function must handle edge cases like very short strings (length < 3) and strings with no duplicates. Use `std::string` and avoid using any extra data structures beyond the result string.
The core idea is to iterate through the input string while building the output string, ensuring that we never append a character that would make the last three characters of the result identical. Since we only need to avoid three consecutive equal characters, we can maintain the output string `ans` and, for each character `c` in the input, check whether the last two characters of `ans` are both equal to `c`. If they are, we skip `c`; otherwise we append it. This works because if the last two are `c` and the current is `c`, then we would form `ccc`; if they are not both `c`, appending is safe. The initial state must handle strings shorter than 2 by directly copying the first two characters (or fewer if the string is shorter). Edge cases include empty strings (return empty), strings of length 1 or 2 (return unchanged), and strings with many consecutive duplicates that still allow some characters to remain (e.g., `"aaaa"` becomes `"aa"`). The algorithm runs in \(O(n)\) time because each character is considered once, and it uses \(O(n)\) auxiliary space in the worst case for the result string (which is the same size as the input when no removals occur). The space complexity is \(O(n)\) because the output string can be as large as the input.
#include <string>

// Remove characters that would cause three consecutive identical letters.
// Preserves order and removes as few characters as possible.
std::string makeFancyString(const std::string& s) {
    if (s.length() < 3) {
        return s;
    }
    std::string ans;
    ans.reserve(s.length());
    ans += s[0];  // always keep the first character
    ans += s[1];  // always keep the second character
    for (std::size_t i = 2; i < s.length(); ++i) {
        if (s[i] == ans[ans.length() - 1] && s[i] == ans[ans.length() - 2]) {
            continue;  // would form a triple, skip
        }
        ans += s[i];
    }
    return ans;
}
#include <cassert>
#include <string>

// The solution function is declared above; include it in the same file.
int main() {
    assert(makeFancyString("") == "");
    assert(makeFancyString("a") == "a");
    assert(makeFancyString("ab") == "ab");
    assert(makeFancyString("aa") == "aa");
    assert(makeFancyString("aaa") == "aa");
    assert(makeFancyString("aaabaaaa") == "aabaa");
    assert(makeFancyString("leetcode") == "leetcode");
    assert(makeFancyString("aabbccdd") == "aabbccdd");
    assert(makeFancyString("aaaaa") == "aa");
    assert(makeFancyString("abcccd") == "abccd");
    return 0;
}
