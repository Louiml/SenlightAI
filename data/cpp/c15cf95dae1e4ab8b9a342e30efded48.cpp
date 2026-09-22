Write a C++ function that takes two strings, a main string and a substring, and returns the zero-based starting index of the first occurrence of the substring within the main string, or `-1` if the substring does not appear. The function must be case-sensitive, handle empty inputs gracefully (an empty substring should return 0, and an empty main string with a non-empty substring should return -1), and should not use any standard library search functions (e.g., `std::string::find`, `std::search`). You must implement the matching logic manually using a character-by-character comparison.

The solution uses a naive brute-force pattern matching algorithm. The main string is iterated from index `0` to `n - m`, where `n` is the length of the main string and `m` is the length of the substring. For each possible starting position `i`, we compare the substring characters with the main string characters from `i` onwards. If all `m` characters match, we return `i` immediately. If a mismatch occurs, we break out of the inner loop and try the next starting position. If no starting position yields a full match, we return `-1`.

Edge cases: 
- If the substring is empty (`m == 0`), return 0 because an empty string is considered found at any position (conventionally at index 0).
- If the main string is empty and the substring is non-empty, the loop condition `i <= (n - m)` becomes `0 <= -m` which is false, so we return `-1`.
- If the substring is longer than the main string, the loop also never executes and we return `-1`.
- Case sensitivity is natural because `!=` compares exact character values.

Time complexity is `O(n * m)` in the worst case (e.g., when the substring almost matches but fails at the last character for every position). Space complexity is `O(1)` auxiliary, not counting the input strings.

#include <string>

// Returns the starting index of the first occurrence of 'sub' in 'super',
// or -1 if 'sub' is not found. Uses manual character comparison.
int findFirstMatch(const std::string& super, const std::string& sub) {
    const int n = static_cast<int>(super.length());
    const int m = static_cast<int>(sub.length());

    // Empty substring is trivially found at index 0.
    if (m == 0) {
        return 0;
    }

    // If substring is longer than main string, impossible to match.
    if (m > n) {
        return -1;
    }

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        // Compare characters from the current starting position.
        while (j < m && super[i + j] == sub[j]) {
            ++j;
        }
        // If we compared all characters and they matched, return index.
        if (j == m) {
            return i;
        }
    }
    return -1;
}

#include <cassert>

// Declare the function from the solution (included here for compilation).
int findFirstMatch(const std::string& super, const std::string& sub);

int main() {
    // Basic case
    assert(findFirstMatch("hello world", "world") == 6);

    // Substring at the beginning
    assert(findFirstMatch("hello", "he") == 0);

    // Substring at the end
    assert(findFirstMatch("hello", "lo") == 3);

    // Substring not present
    assert(findFirstMatch("hello", "xyz") == -1);

    // Empty substring
    assert(findFirstMatch("anything", "") == 0);

    // Empty main string with non-empty substring
    assert(findFirstMatch("", "a") == -1);

    // Substring longer than main string
    assert(findFirstMatch("abc", "abcd") == -1);

    // Case sensitivity
    assert(findFirstMatch("Hello", "hello") == -1);

    // Overlapping pattern not relevant but ensure first occurrence
    assert(findFirstMatch("aaa", "aa") == 0);

    // Single character matches
    assert(findFirstMatch("abc", "b") == 1);

    return 0;
}
