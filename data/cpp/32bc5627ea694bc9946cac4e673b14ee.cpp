// Write a C++ function named `makeFancyString` that takes a non-empty string `s` containing lowercase English letters and returns a new string where no character appears more than twice consecutively. Specifically, whenever a character appears three or more times in a row in the input, keep only the first two occurrences of that run. All other characters (including the first two of any run) must appear unchanged and in the original order. For example, `"aaabaaaa"` should become `"aabaa"`. The function must handle single-character strings and strings with only two consecutive identical characters correctly. The input string will contain only lowercase letters and will have length at least 1.
// The algorithm maintains an output string `ans` and a counter `cnt` for the current consecutive-run length of the character at the end of `ans`. Initialize `ans` containing the first character of `s` and set `cnt = 1`. Iterate through the remaining characters of `s` from index 1 to `n-1`. At each step, compare the current character with the last character of `ans`. If they are equal, increment `cnt`; if `cnt` is still less than 3, append the character to `ans`; otherwise skip it. If they differ, reset `cnt` to 1 and append the character. This ensures each run is truncated to at most two characters. Edge cases: a one-character string returns itself; a string like `"aa"` is unchanged; a string like `"aaa"` becomes `"aa"`. The time complexity is O(n) for an input of length n, and the auxiliary space is O(n) because the output string may be of length up to n (in the worst case where no character repeats three times). The solution uses constant extra variables beyond the output string.
#include <string>

// Remove characters that appear three or more times consecutively,
// keeping at most two consecutive occurrences of any character.
std::string makeFancyString(const std::string& s) {
    // The input is guaranteed non-empty.
    std::string ans;
    ans.reserve(s.size());
    ans.push_back(s[0]);
    
    int cnt = 1;
    
    for (std::size_t i = 1; i < s.size(); ++i) {
        if (s[i] == ans.back()) {
            ++cnt;
            if (cnt < 3) {
                ans.push_back(s[i]);
            }
        } else {
            cnt = 1;
            ans.push_back(s[i]);
        }
    }
    
    return ans;
}
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    assert(makeFancyString("a") == "a");
    assert(makeFancyString("aa") == "aa");
    assert(makeFancyString("aaa") == "aa");
    assert(makeFancyString("aaabaaaa") == "aabaa");
    assert(makeFancyString("aabaa") == "aabaa");
    assert(makeFancyString("abbbcc") == "abbcc");
    assert(makeFancyString("zzzxxxyyy") == "zzxxyy");
    assert(makeFancyString("abc") == "abc");
    assert(makeFancyString("aaabbbccc") == "aabbcc");
    assert(makeFancyString("aaaa") == "aa");
    return 0;
}
