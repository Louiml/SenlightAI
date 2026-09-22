// Write a C++ function `string lowercaseFrequencyReport(const string& input)` that takes a non-empty string containing only lowercase English letters (a–z) and returns a string that lists each distinct character that appears an odd number of times, followed by a colon and the count of that character, in the order the characters first appear in the input string, separated by semicolons. If no character appears an odd number of times, the function returns the string `"none"`. For example, for input `"abacaba"`, the characters `a` appears 4 times, `b` appears 2 times, `c` appears 1 time, so the answer is `"c:1"`. For `"aabbc"`, `c:1` is returned. For `"abcabc"`, all counts are even, so returns `"none"`. The function must be case‑sensitive but input is guaranteed lowercase; you may assume input length is at least 1.

#include <cassert>
#include <string>

int main() {
    assert(lowercaseFrequencyReport("a") == "a:1");
    assert(lowercaseFrequencyReport("abacaba") == "c:1");
    assert(lowercaseFrequencyReport("aabbc") == "c:1");
    assert(lowercaseFrequencyReport("abcabc") == "none");
    assert(lowercaseFrequencyReport("zz") == "none");
    assert(lowercaseFrequencyReport("abcdd") == "a:1;b:1;c:1");
    assert(lowercaseFrequencyReport("aaabbbccc") == "c:3");
    assert(lowercaseFrequencyReport("ab") == "a:1;b:1");
    assert(lowercaseFrequencyReport("xyzxyzx") == "x:3");
    assert(lowercaseFrequencyReport("aabbccddee") == "none");
}

#include <string>
#include <unordered_map>
#include <set>

// Returns a semicolon-separated list of characters that appear an odd number of times,
// with their counts, in order of first appearance. If none, returns "none".
std::string lowercaseFrequencyReport(const std::string& input) {
    std::unordered_map<char, int> freq;
    for (char c : input) {
        ++freq[c];
    }
    
    std::string result;
    std::set<char> reported;
    
    for (char c : input) {
        if (freq[c] % 2 == 1 && reported.find(c) == reported.end()) {
            if (!result.empty()) {
                result += ';';
            }
            result += c;
            result += ':';
            result += std::to_string(freq[c]);
            reported.insert(c);
        }
    }
    
    return result.empty() ? "none" : result;
}

// The solution uses an `unordered_map<char, int>` to count frequencies of each character by iterating through the string. After counting, we iterate over the original string again, and for each character, if its count is odd and we haven't already reported it (tracked with a `set` or by checking if we've already added it), we append the character, a colon, and the count to the result. We use a `set` to ensure each odd-frequency character is only reported once, and we preserve first‑appearance order by scanning the original string. Edge cases: all characters have even frequencies → return `"none"`; a single character string → return `"x:1"`; characters with count 1 → always odd, so reported. Time complexity is O(n) for counting plus O(n) for the second scan (ignoring the set operations which are O(1) average), so overall O(n). Space complexity is O(k) where k is the number of distinct characters (at most 26).
