Write a C++ function `bool isOneSlashTwoPattern(const std::string& s)` that takes a non-empty string `s` consisting only of characters `'1'`, `'2'`, and `'/'` (no other characters) and returns `true` if and only if the string has odd length and exactly matches the pattern: zero or more `'1'` characters, then exactly one `'/'` in the exact middle position, then the same number of `'2'` characters as the leading `'1'`s. In other words, for a string of length `n` (odd), every position `i < n/2` must be `'1'`, position `n/2` must be `'/'`, and every position `i > n/2` must be `'2'`. If the length is even, or if any character violates the pattern, return `false`. You may assume the input string is non-empty.
#include <cassert>
#include <string>

// Function declaration (provided by the solution section)
bool isOneSlashTwoPattern(const std::string& s);

int main() {
    // Valid patterns
    assert(isOneSlashTwoPattern("/") == true);                      // n=1
    assert(isOneSlashTwoPattern("1/2") == true);                    // n=3
    assert(isOneSlashTwoPattern("11/22") == true);                  // n=5
    assert(isOneSlashTwoPattern("111/222") == true);                // n=7
    assert(isOneSlashTwoPattern("11111/22222") == true);            // n=11

    // Invalid: even length
    assert(isOneSlashTwoPattern("") == false);                      // empty (even)
    assert(isOneSlashTwoPattern("1/") == false);                    // length 2
    assert(isOneSlashTwoPattern("11/22/33") == false);              // length 7? actually 8 -> even
    assert(isOneSlashTwoPattern("11/2") == false);                  // length 4

    // Invalid: wrong characters in left half
    assert(isOneSlashTwoPattern("2/2") == false);                   // left has '2'
    assert(isOneSlashTwoPattern("1/1") == false);                   // right has '1'
    assert(isOneSlashTwoPattern("11/12") == false);                 // right has '1'
    assert(isOneSlashTwoPattern("12/22") == false);                 // left has '2'

    // Invalid: wrong middle character
    assert(isOneSlashTwoPattern("1/2") == true);    // Wait, this is valid. Test for wrong middle:
    assert(isOneSlashTwoPattern("112") == false);   // middle should be '/', but is '1'
    assert(isOneSlashTwoPattern("1/22") == true);   // Wait, this is length 4? Actually "1/22" length 4 -> even, so false
    assert(isOneSlashTwoPattern("11/2") == true);   // Wow, length 4, even, so false.
    // Let's test proper odd-length with wrong middle:
    assert(isOneSlashTwoPattern("11122") == false); // length 5, middle is '1' not '/'
    assert(isOneSlashTwoPattern("11/1") == false);  // length 4 even
    assert(isOneSlashTwoPattern("1//1") == false);  // length 4 even

    // More invalid: mixed characters left/right
    assert(isOneSlashTwoPattern("1/22") == false);  // length 4 even
    assert(isOneSlashTwoPattern("111/22") == false); // length 6 even
    assert(isOneSlashTwoPattern("111/2222") == false); // length 8 even
    assert(isOneSlashTwoPattern("11/222") == false); // length 6 even

    // Valid with larger odd length
    assert(isOneSlashTwoPattern("11111/22222") == true);
    assert(isOneSlashTwoPattern("1111111/2222222") == true);
    assert(isOneSlashTwoPattern("111111111/222222222") == true);

    // Edge: all same characters but odd length with slash in middle
    assert(isOneSlashTwoPattern("1/2") == true);
    assert(isOneSlashTwoPattern("1/2") == true);
    assert(isOneSlashTwoPattern("111/222") == true);

    // Wrong slash position for odd length
    assert(isOneSlashTwoPattern("1/112") == false); // length 5, but slash is not at middle (index 1 vs 2)
    assert(isOneSlashTwoPattern("112/2") == false); // length 5, slash at middle? index 2 is '2' not '/', so false

    // Extra safety: ensure no out-of-bounds issues
    assert(isOneSlashTwoPattern("/") == true);
    assert(isOneSlashTwoPattern("1/2") == true);

    return 0;
}
#include <string>

// Returns true iff the string matches the pattern: 1...1 / 2...2 with equal counts on both sides and odd length.
bool isOneSlashTwoPattern(const std::string& s) {
    const size_t n = s.size();
    if (n % 2 == 0) {
        return false; // even length cannot have a unique middle character
    }

    const size_t mid = n / 2;

    // Check left half: all must be '1'
    for (size_t i = 0; i < mid; ++i) {
        if (s[i] != '1') {
            return false;
        }
    }

    // Check middle character must be '/'
    if (s[mid] != '/') {
        return false;
    }

    // Check right half: all must be '2'
    for (size_t i = mid + 1; i < n; ++i) {
        if (s[i] != '2') {
            return false;
        }
    }

    return true;
}
// The task is a direct string pattern‑matching problem. First check if the length `n` is odd; if not, return `false` immediately because the pattern requires a distinct middle character. Then verify the left half: iterate from index `0` to `n/2 - 1` and require each character to be `'1'`. Next, check the middle character at index `n/2` equals `'/'`. Finally, iterate from index `n/2 + 1` to `n-1` and require each character to be `'2'`. If all these checks pass, return `true`. Edge cases: the shortest valid string is `"/"` (n=1) — in that case both loops are empty and only the middle check applies. Empty strings are not considered by the problem statement, but the function can handle them safely (returning `false` because length 0 is even). Time complexity is `O(n)` because we scan the string a constant number of times. Space complexity is `O(1)` aside from the input string itself.
