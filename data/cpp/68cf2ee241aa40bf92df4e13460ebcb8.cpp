// Write a C++ free function `isReversedEqual` that takes two `std::string` parameters `s` and `t` (which may contain any printable ASCII characters, including spaces, and may be empty). The function should return `true` if the string `t` is exactly equal to the reverse of string `s`, and `false` otherwise. The comparison must be case-sensitive and account for all characters. The function should not modify the original input strings, and should not rely on any global state. Ensure the function is efficient for potentially long strings (up to 10^5 characters) and handles edge cases such as empty strings, single-character strings, and strings with repeated characters.
// The solution is straightforward: create a reversed copy of `s` and compare it directly with `t`. Because strings are immutable in the sense that we should not modify them, we can make a copy and use `std::reverse` on the copy, then compare with `t`. Alternatively, we can compare character by character from the end of `s` to the beginning with the beginning of `t`, which avoids creating a copy and is more memory-efficient. The latter approach: iterate from index 0 of `t` and from index `s.length()-1` of `s`, comparing characters; if any mismatch occurs, return `false`. Also, if lengths differ, return `false` immediately. Edge cases: empty strings—both empty? Then reverse of empty is empty, so result `true` if both empty, else `false`. For single characters, just compare the single characters. Time complexity is O(n) where n is the length of the strings (ignoring the length difference check which is O(1)). Space complexity is O(1) auxiliary if using the in-place comparison method, or O(n) if copying the string. We'll choose the O(1) space method.
#include <string>

// Returns true if t equals the reverse of s, false otherwise.
bool isReversedEqual(const std::string& s, const std::string& t) {
    if (s.length() != t.length()) {
        return false;
    }
    for (std::size_t i = 0; i < s.length(); ++i) {
        // Compare i-th character of t with (length-1-i)-th of s
        if (t[i] != s[s.length() - 1 - i]) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <string>

// Declaration of the function (as if from header)
bool isReversedEqual(const std::string& s, const std::string& t);

int main() {
    // Basic cases
    assert(isReversedEqual("hello", "olleh") == true);
    assert(isReversedEqual("hello", "hello") == false);
    assert(isReversedEqual("abc", "cba") == true);
    assert(isReversedEqual("abc", "abd") == false);
    
    // Empty strings
    assert(isReversedEqual("", "") == true);
    assert(isReversedEqual("", "a") == false);
    assert(isReversedEqual("a", "") == false);
    
    // Single character
    assert(isReversedEqual("a", "a") == true);
    assert(isReversedEqual("a", "b") == false);
    
    // With spaces and special characters
    assert(isReversedEqual("a b", "b a") == true);
    assert(isReversedEqual("  ", "  ") == true); // spaces reverse same
    assert(isReversedEqual("x!y", "y!x") == true);
    
    // Repeated characters
    assert(isReversedEqual("aaaa", "aaaa") == true);
    assert(isReversedEqual("aabb", "bbaa") == true);
    assert(isReversedEqual("aabb", "aabb") == false);
    
    // Length mismatch
    assert(isReversedEqual("abc", "ab") == false);
    assert(isReversedEqual("ab", "abc") == false);
    
    // Case sensitivity
    assert(isReversedEqual("AbC", "CbA") == true);
    assert(isReversedEqual("AbC", "cBa") == false);
    
    return 0;
}
