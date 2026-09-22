// Given an integer `n` and a string `str` of length `n` consisting only of lowercase English letters, write a C++ function `transformToAllZ` that returns a string of length `n` containing only the character `'z'` — but if the input string already consists entirely of `'z'` characters, the function must return `"-1"` instead. The function should handle any `n ≥ 1`, and the input string is guaranteed to contain exactly `n` lowercase letters.

// The solution checks whether every character in the input string is `'z'`. This can be done by iterating through the string and comparing each character; if any character is not `'z'`, set a flag to false and break. If the flag remains true after the loop, the entire string is all `'z'`s, so we return `"-1"`. Otherwise, we construct a new string of length `n` consisting of `'z'`s. Since the output does not depend on which non-`'z'` characters exist (only that at least one exists), we can simply generate `n` copies of `'z'`. Edge cases: `n=1` and the string is `"a"` → output `"z"`; `n=1` and the string is `"z"` → output `"-1"`. The algorithm runs in `O(n)` time because we scan the input string once and construct the output of length `n`; auxiliary space is `O(1)` aside from the returned string of size `n`.

#include <string>

// Return a string of n 'z's, or "-1" if the input string already consists solely of 'z's.
std::string transformToAllZ(int n, const std::string& str) {
    bool allZ = true;
    for (char ch : str) {
        if (ch != 'z') {
            allZ = false;
            break;
        }
    }
    if (allZ) {
        return "-1";
    }
    return std::string(n, 'z');
}

#include <cassert>
#include <string>

// Declaration of the tested function
std::string transformToAllZ(int n, const std::string& str);

int main() {
    assert(transformToAllZ(1, "a") == "z");
    assert(transformToAllZ(1, "z") == "-1");
    assert(transformToAllZ(3, "abc") == "zzz");
    assert(transformToAllZ(3, "zzz") == "-1");
    assert(transformToAllZ(5, "zazzz") == "zzzzz");
    assert(transformToAllZ(2, "zz") == "-1");
    assert(transformToAllZ(4, "python") == "zzzz"); // only first 4 chars matter? 
    // Note: The original snippet reads exactly n and str of length n, so test with exact lengths:
    assert(transformToAllZ(4, "pyth") == "zzzz");
    assert(transformToAllZ(6, "zzzzzz") == "-1");
    assert(transformToAllZ(2, "ab") == "zz");
    return 0;
}
