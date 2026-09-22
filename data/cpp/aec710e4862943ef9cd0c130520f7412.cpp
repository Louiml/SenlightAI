// Write a C++ function `int countLocalExtremes(const std::string& s)` that takes a non-empty string consisting of printable ASCII characters (no spaces) and returns the number of positions `i` in the string such that the character at `i` is strictly different from both its immediate neighbors (i.e., `s[i] != s[i-1]` and `s[i] != s[i+1]`). For the first character, only check the neighbor to the right; for the last character, only check the neighbor to the left. Special cases: if the string has length 1, the count is 0; if the string has length 2 and the two characters are different, the count is 1; if length 2 and identical, the count is 0. The function must handle all possible non-empty strings, including those with repeated adjacent characters.

// The solution iterates through the string once, checking each position against its relevant neighbor(s). For a string of length `n`, we handle three cases explicitly: if `n == 1` return 0; if `n == 2` return 1 if the two characters differ, else 0; otherwise, loop `i` from 0 to `n-1`, and for each index check: if `i == 0`, compare `s[0]` to `s[1]`; if `i == n-1`, compare `s[n-1]` to `s[n-2]`; otherwise compare `s[i]` to both `s[i-1]` and `s[i+1]`. Increment the counter whenever the required inequality holds. This matches the behavior of the original snippet. Edge cases include length 1 and length 2, which are handled separately. The time complexity is O(n) because we examine each character a constant number of times, and the space complexity is O(1) since we only use a few integer variables and modify nothing.

#include <string>

// Count positions where the character differs from all immediate neighbors.
// For endpoints, only the single existing neighbor is considered.
int countLocalExtremes(const std::string& s) {
    const size_t n = s.length();
    
    if (n == 1) {
        return 0;
    }
    if (n == 2) {
        return (s[0] != s[1]) ? 1 : 0;
    }
    
    int count = 0;
    for (size_t i = 0; i < n; ++i) {
        if (i == 0) {
            if (s[i] != s[i + 1]) {
                ++count;
            }
        } else if (i == n - 1) {
            if (s[i] != s[i - 1]) {
                ++count;
            }
        } else {
            if (s[i] != s[i - 1] && s[i] != s[i + 1]) {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
#include <string>

// Function declaration (from the solution above)
int countLocalExtremes(const std::string& s);

int main() {
    // Length 1
    assert(countLocalExtremes("a") == 0);
    // Length 2 different
    assert(countLocalExtremes("ab") == 1);
    // Length 2 identical
    assert(countLocalExtremes("aa") == 0);
    // General case with local extremes
    assert(countLocalExtremes("aba") == 2); // both 'b' and 'a' at ends? Actually 'a' at 0 differs from 'b' -> 1, 'b' differs from both -> 2, 'a' at 2 differs from 'b' -> 3? Wait: "aba": i=0 'a' vs 'b' -> count++; i=1 'b' vs 'a' and 'a' -> count++; i=2 'a' vs 'b' -> count++; total 3. Let's correct: countLocalExtremes("aba") == 3
    assert(countLocalExtremes("aba") == 3);
    // All same characters
    assert(countLocalExtremes("bbbb") == 0);
    // Alternating but with repeated at ends
    assert(countLocalExtremes("aab") == 1); // i=0: 'a' vs 'a' no; i=1: 'a' vs 'a' and 'b'? s[1]='a', s[0]='a', s[2]='b' -> not different from both -> no; i=2: 'b' vs 'a' -> yes -> total 1
    // Longer pattern
    assert(countLocalExtremes("abcba") == 3); // a-b diff, b-c diff? Actually i=0: 'a' vs 'b' -> 1; i=1: 'b' vs 'a' and 'c' -> both diff -> 2; i=2: 'c' vs 'b' and 'b' -> diff from both? s[2]='c', neighbors 'b' and 'b' -> both diff -> 3; i=3: 'b' vs 'c' and 'a' -> both diff -> 4; i=4: 'a' vs 'b' -> 5? Wait let's compute carefully: string "abcba": indices 0-4. i=0: s[0]='a' vs s[1]='b' -> diff -> count=1. i=1: s[1]='b' vs s[0]='a' and s[2]='c' -> diff from both -> count=2. i=2: s[2]='c' vs s[1]='b' and s[3]='b' -> diff from both? 'c' vs 'b' and 'b' -> yes -> count=3. i=3: s[3]='b' vs s[2]='c' and s[4]='a' -> diff from both -> count=4. i=4: s[4]='a' vs s[3]='b' -> diff -> count=5. So total 5. Correct: countLocalExtremes("abcba") == 5.
    assert(countLocalExtremes("abcba") == 5);
    // String with spaces? Not allowed, but test with e.g. "a b" is invalid; skip.
    // Edge: "abab" -> i=0 'a' vs 'b' ->1; i=1 'b' vs 'a','a' -> both diff? b vs a and a -> yes ->2; i=2 'a' vs 'b','b' -> both diff ->3; i=3 'b' vs 'a' ->4. So 4.
    assert(countLocalExtremes("abab") == 4);
    
    return 0;
}
