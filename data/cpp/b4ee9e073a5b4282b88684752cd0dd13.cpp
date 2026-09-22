Write a C++ function `string transformString(const string& s)` that takes a string of lowercase English letters and returns a new string where each character is replaced by the next letter in the alphabet (i.e., 'a' becomes 'b', 'b' becomes 'c', ..., 'z' wraps around to 'a'). The function must preserve the original order and length of the string, handle empty strings gracefully (return an empty string), and operate on a `const std::string&` input without modifying it. The solution must be efficient for strings up to 10^6 characters in length.

// The problem is a straightforward character transformation. For each character `c` in the input string:
// - if `c` is 'z', map it to 'a'
// - otherwise, map it to `c + 1` (since characters are contiguous in ASCII for lowercase letters)
// This works because lowercase letters 'a'-'z' have ASCII codes 97-122. Incrementing a character by 1 moves to the next letter; for 'z' (122), adding 1 gives '{' (123), which is not a letter, so we special-case wrap-around. An empty string returns an empty string—no special logic needed since just iterating over an empty string produces no output. Time complexity is O(n) where n = s.length(), scanning each character once. Space complexity is O(n) for the result string, plus O(1) auxiliary. Edge cases: single character strings, all 'z's, a string already containing 'a' (maps to 'b'), and very long strings. The solution uses a `std::string` result and `reserve` to avoid reallocation, and uses a range-based for loop with `const char&` to avoid copying.

#include <string>
#include <cctype>

// Transform each lowercase letter to the next alphabet letter, wrapping z->a.
// Input is a const reference; output is a new string.
std::string transformString(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    for (const char c : s) {
        if (c == 'z') {
            result.push_back('a');
        } else {
            // Since input is guaranteed lowercase, safe to increment.
            result.push_back(c + 1);
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (include its definition above in a real project).
std::string transformString(const std::string& s);

int main() {
    // Basic cases
    assert(transformString("abc") == "bcd");
    assert(transformString("z") == "a");
    assert(transformString("zzz") == "aaa");
    assert(transformString("") == "");

    // Wrap-around and mixed characters
    assert(transformString("az") == "ba");
    assert(transformString("zyx") == "azy");

    // Longer string with all letters
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string shifted;
    for (char c : alphabet) {
        shifted += (c == 'z' ? 'a' : c + 1);
    }
    assert(transformString(alphabet) == shifted);

    // Single character each
    assert(transformString("a") == "b");
    assert(transformString("m") == "n");
    assert(transformString("y") == "z");

    // Repeated pattern
    assert(transformString("xyzxyz") == "yzyzyz");

    // Full wrap for each letter individually
    assert(transformString("zazb") == "abac");

    // Large string (100000 letters) - only check size and first/last char
    std::string large(100000, 'a');
    std::string result = transformString(large);
    assert(result.size() == 100000);
    assert(result.front() == 'b');
    assert(result.back() == 'b');

    // Mixed with non-z near end
    assert(transformString("v") == "w");
    assert(transformString("w") == "x");

    // Test all lowercase letters
    std::string input = "abcdefghijklmnopqrstuvwxyz";
    std::string expected = "bcdefghijklmnopqrstuvwxyza";
    assert(transformString(input) == expected);
}
