Write a C++ function `std::vector<int> rabinKarpMatches(const std::string& text, const std::string& pattern, int q)` that implements the Rabin-Karp string matching algorithm and returns a vector of 0-based starting indices where `pattern` occurs in `text`. The function should handle empty pattern (return all positions from 0 to `text.length()` inclusive, since an empty string matches at every position), empty text (return empty vector unless pattern is also empty, then return {0}), and cases where `q` is a non‑positive integer (throw `std::invalid_argument`). The hash must be computed using the base `d = 10` and modulo `q`. The search must verify characters when hashes match to avoid false positives. The function must be `const`‑correct (take `const std::string&` parameters) and use `size_t` for indices. Do not include `main` in the solution.

// The solution uses the classic Rabin‑Karp rolling hash: first compute the hash of the pattern and the initial window of text of length `m` using the formula hash = (d * hash + char) % q. Precompute `h = d^(m-1) % q` to allow removal of the leftmost character when sliding the window. For each position `i` from 0 to `n-m`, compare hashes; if equal, verify each character to ensure no false match. After checking, compute the next window hash as `(d * (t - text[i]*h) + text[i+m]) % q`, and if negative add `q` to make it positive. Edge cases: if `pattern` is empty, every position (including `n`) is a match because an empty string is a substring of any string; if `text` is empty but pattern non‑empty, there are no matches; if `q <= 0`, the modulo operation is undefined/throws, so throw `invalid_argument`. Rolling hash update requires careful handling of negative intermediate results, especially with large characters; the modulo operation in C++ on negative numbers yields a negative result, so we add `q` once. Time complexity is O(n + m) average, O(n*m) worst case due to verification when hash collisions occur frequently (e.g., pattern and text all same characters, but q small). Space complexity is O(1) auxiliary beyond the output vector.

#include <stdexcept>
#include <string>
#include <vector>

// Rabin-Karp string matching: returns starting indices (0-based) of all occurrences of pattern in text.
// Uses base d=10 and modulus q. Throws std::invalid_argument if q <= 0.
// Handles empty pattern: matches at every position from 0 to text.length().
std::vector<int> rabinKarpMatches(const std::string& text, const std::string& pattern, int q) {
    if (q <= 0) {
        throw std::invalid_argument("q must be positive");
    }

    const size_t n = text.size();
    const size_t m = pattern.size();
    std::vector<int> matches;

    // Empty pattern matches at all positions including after the last character.
    if (m == 0) {
        matches.reserve(n + 1);
        for (size_t i = 0; i <= n; ++i) {
            matches.push_back(static_cast<int>(i));
        }
        return matches;
    }

    if (n < m) {
        return matches; // No possible match
    }

    const int d = 10;
    int p = 0; // hash of pattern
    int t = 0; // hash of current text window
    int h = 1; // d^(m-1) % q

    // Precompute h = d^(m-1) % q
    for (size_t i = 0; i < m - 1; ++i) {
        h = (h * d) % q;
    }

    // Compute initial hashes for pattern and first window of text
    for (size_t i = 0; i < m; ++i) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // Slide the window over text
    for (size_t i = 0; i <= n - m; ++i) {
        // Check hash equality and verify characters
        if (p == t) {
            size_t j = 0;
            while (j < m && text[i + j] == pattern[j]) {
                ++j;
            }
            if (j == m) {
                matches.push_back(static_cast<int>(i));
            }
        }

        // Move to next window (if not at the last possible position)
        if (i < n - m) {
            // Remove leftmost character, add new character
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) {
                t += q;
            }
        }
    }

    return matches;
}

#include <cassert>
#include <string>
#include <vector>

// Declare the solution function (would be in a header in practice)
std::vector<int> rabinKarpMatches(const std::string& text, const std::string& pattern, int q);

int main() {
    // Standard match at 0-based index 2 for pattern "CDD" in text "ABCCDDAEFG"
    assert(rabinKarpMatches("ABCCDDAEFG", "CDD", 13) == std::vector<int>{2});

    // Multiple occurrences
    assert(rabinKarpMatches("aaa", "a", 101) == std::vector<int>{0, 1, 2});

    // Overlapping pattern
    assert(rabinKarpMatches("ababa", "aba", 101) == std::vector<int>{0, 2});

    // Empty pattern matches at every position (0 to text.length())
    assert(rabinKarpMatches("abc", "", 13) == std::vector<int>{0, 1, 2, 3});

    // Empty text and non-empty pattern -> no matches
    assert(rabinKarpMatches("", "abc", 13) == std::vector<int>{});

    // Empty text and empty pattern -> only position 0
    assert(rabinKarpMatches("", "", 13) == std::vector<int>{0});

    // Pattern longer than text -> no matches
    assert(rabinKarpMatches("abc", "abcd", 13) == std::vector<int>{});

    // No match
    assert(rabinKarpMatches("abcdef", "xyz", 13) == std::vector<int>{});

    // Single character match
    assert(rabinKarpMatches("x", "x", 13) == std::vector<int>{0});

    // q=1 (all hashes zero) still works correctly
    assert(rabinKarpMatches("abc", "b", 1) == std::vector<int>{1});

    // Non-positive q throws exception
    bool threw = false;
    try {
        rabinKarpMatches("abc", "b", 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Large q with many characters
    assert(rabinKarpMatches("The quick brown fox", "quick", 997) == std::vector<int>{4});

    return 0;
}
