Given a string `s` consisting of only lowercase English letters and an integer `n` equal to its length, write a C++ function `countGoodSubstrings` that returns the total number of non-empty substrings whose characters form a block of identical letters immediately followed by another block of identical letters (i.e., substrings consisting of exactly one repeated character for the entire substring). More precisely, a substring is "good" if all its characters are the same. For example, for `"aab"`, the good substrings are `"a"`, `"a"`, `"aa"`, and `"b"` (total 4). The result must be returned modulo `998244353`. The function signature should be `long long countGoodSubstrings(const std::string& s)`. Note that each individual character counts as a good substring.
The solution observes that for any maximal run of identical characters of length `L`, the number of good substrings entirely contained within that run is exactly `L*(L+1)/2` (since every substring of a run of identical characters is also a run of identical characters). However, we can compute this more efficiently in a single pass: iterate through the string while maintaining the length of the current consecutive equal-character run. For each character, extend the current run length if it matches the previous character, otherwise reset the run length to 1. Each time we process a character, the number of new good substrings ending at that position is exactly the current run length (because each of those substrings consists of a suffix of the run ending at the current index, all identical). So increment the answer by the current run length modulo `MOD`. Initial answer is 0, and after processing all characters, we have the total count. Edge cases: empty string returns 0 (though input non-empty), single character returns 1. Complexity: O(n) time, O(1) extra space.
#include <string>

const long long MOD = 998244353;

// Returns the number of non-empty substrings consisting of a single repeated character,
// modulo MOD.
long long countGoodSubstrings(const std::string& s) {
    if (s.empty()) return 0;
    long long ans = 0;
    long long currentRun = 1; // first character always counts
    ans = 1;
    for (std::size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1]) {
            currentRun = (currentRun + 1) % MOD; // run length grows by 1
        } else {
            currentRun = 1;
        }
        ans = (ans + currentRun) % MOD;
    }
    return ans;
}
#include <cassert>
#include <string>

// Declare the function (or include the header)
long long countGoodSubstrings(const std::string& s);

int main() {
    assert(countGoodSubstrings("a") == 1);
    assert(countGoodSubstrings("ab") == 2); // "a","b"
    assert(countGoodSubstrings("aa") == 3); // "a","a","aa"
    assert(countGoodSubstrings("aab") == 4); // "a","a","aa","b"
    assert(countGoodSubstrings("aaa") == 6); // 1+2+3 = 6
    assert(countGoodSubstrings("abcd") == 4); // each single char
    assert(countGoodSubstrings("aaabbb") == 6 + 6); // "aaa" gives 6, "bbb" gives 6
    assert(countGoodSubstrings("abba") == 1 + 1 + 3 + 1); // a,b,bb,a = 6
    assert(countGoodSubstrings("aabbcc") == 3 + 3 + 3); // 9
    // Large case: 1000 'a's => 1000*1001/2 = 500500
    std::string big(1000, 'a');
    assert(countGoodSubstrings(big) == 500500);
    return 0;
}
