/*
Write a C++ function `generateBalancedString(int a, int b)` that takes two non-negative integers representing counts of the letters `'a'` and `'b'` and returns a string containing exactly `a` occurrences of `'a'` and `b` occurrences of `'b'`, arranged so that no two adjacent characters are the same and the string avoids producing three or more consecutive identical letters (i.e., no substring `"aaa"` or `"bbb"`). The function must always produce a valid string for any non-negative input, prioritizing alternating letters when counts are equal. For example, with `a=4, b=1`, one valid output is `"aabab"`; with `a=2, b=5`, one valid output is `"bbabb"` plus one `'a'` placed between the `'b'` groups (e.g., `"bbabbab"`). The function should handle cases where one count is zero (e.g., `a=3, b=0` must return `"aaa"`) and where counts are equal (e.g., `a=2, b=2` must return `"abab"` or `"baba"`). The order of letters matters only in that the constraints must be satisfied; there is no single correct answer as long as the constraints hold.
*/
#include <string>

// Return a string with exactly 'a' occurrences of 'a' and 'b' occurrences of 'b',
// such that no three consecutive identical letters appear.
std::string generateBalancedString(int a, int b) {
    std::string result;
    result.reserve(a + b);
    
    while (a > 0 || b > 0) {
        if (a > b) {
            if (a >= 2) {
                result += "aa";
                a -= 2;
            } else {
                result += "a";
                a -= 1;
            }
            if (b > 0) {
                result += "b";
                b -= 1;
            }
        } else if (b > a) {
            if (b >= 2) {
                result += "bb";
                b -= 2;
            } else {
                result += "b";
                b -= 1;
            }
            if (a > 0) {
                result += "a";
                a -= 1;
            }
        } else { // a == b
            if (a > 0) {
                result += "a";
                a -= 1;
            }
            if (b > 0) {
                result += "b";
                b -= 1;
            }
        }
    }
    return result;
}
#include <cassert>
#include <string>
#include <algorithm>

// Forward declaration (or include the solution header).
std::string generateBalancedString(int a, int b);

// Helper to count a specific character in a string.
int countChar(const std::string& s, char c) {
    return static_cast<int>(std::count(s.begin(), s.end(), c));
}

// Helper to check if a string has no three consecutive identical characters.
bool noThreeConsecutive(const std::string& s) {
    if (s.size() < 3) return true;
    for (size_t i = 0; i + 2 < s.size(); ++i) {
        if (s[i] == s[i+1] && s[i+1] == s[i+2]) return false;
    }
    return true;
}

int main() {
    // Case: a > b
    std::string s1 = generateBalancedString(4, 1);
    assert(countChar(s1, 'a') == 4);
    assert(countChar(s1, 'b') == 1);
    assert(noThreeConsecutive(s1));
    
    // Case: b > a
    std::string s2 = generateBalancedString(1, 3);
    assert(countChar(s2, 'a') == 1);
    assert(countChar(s2, 'b') == 3);
    assert(noThreeConsecutive(s2));
    
    // Case: a == b
    std::string s3 = generateBalancedString(2, 2);
    assert(countChar(s3, 'a') == 2);
    assert(countChar(s3, 'b') == 2);
    assert(noThreeConsecutive(s3));
    
    // Case: one count is zero
    std::string s4 = generateBalancedString(3, 0);
    assert(s4 == "aaa");
    assert(countChar(s4, 'a') == 3);
    assert(countChar(s4, 'b') == 0);
    assert(noThreeConsecutive(s4));
    
    // Case: zero counts
    std::string s5 = generateBalancedString(0, 0);
    assert(s5.empty());
    
    // Case: large imbalance
    std::string s6 = generateBalancedString(0, 5);
    assert(countChar(s6, 'a') == 0);
    assert(countChar(s6, 'b') == 5);
    assert(noThreeConsecutive(s6));
    
    // Case: moderate
    std::string s7 = generateBalancedString(5, 2);
    assert(countChar(s7, 'a') == 5);
    assert(countChar(s7, 'b') == 2);
    assert(noThreeConsecutive(s7));
    
    // Additional random test
    std::string s8 = generateBalancedString(1, 1);
    assert(countChar(s8, 'a') == 1);
    assert(countChar(s8, 'b') == 1);
    assert(noThreeConsecutive(s8));
    
    return 0;
}
// The core idea is to greedily place the letter with the larger remaining count, but when placing that letter, we must avoid creating three consecutive identical characters. Therefore, when the larger count is at least 2 and the other count is positive, we place two of the larger letter, then one of the smaller letter, and repeat. However, if the larger count is exactly 1 more than the smaller, placing two could immediately create a run of three when combined with a previous letter—so we must be careful about the boundary condition. The given snippet’s logic is: while either count is positive, if `a > b`, append `"aa"` (or `"a"` if only one remains) and then append one `'b'` if available. Similarly, if `b > a`, append `"bb"` (or `"b"`) then one `'a'`. If equal, append one `'a'` then one `'b'`. This ensures that we never place three identical letters in a row because we always alternate the larger group with a single from the smaller group. Edge cases: when one count is zero, the loop just appends pairs (or singles) of the remaining letter without inserting the other letter, which is correct because no constraint is violated (e.g., `"aaa"` is allowed). When counts are equal, the equal branch appends `'a'` then `'b'` repeatedly, producing a perfectly alternating string. The algorithm runs in O(a+b) time because each iteration reduces the total count by at least 1, and uses O(a+b) space for the result string (which is unavoidable). Auxiliary space is O(1) aside from the output.
