Write a C++ function named `countWordSegments` that takes a `const std::string&` as input and returns an `int` representing the number of distinct non‑empty segments (words) separated by commas or spaces. The input string may contain leading, trailing, or multiple consecutive delimiters (commas or spaces), and delimiters may appear in any combination (e.g., `" , "`). The function must correctly handle empty strings (returning 0), strings with only delimiters (returning 0), and strings with no delimiters (returning 1). The function must be declared `int countWordSegments(const std::string& s);` and must not use any standard library functions beyond basic string access (`operator[]`, `length()`) and character checking via `std::isspace` or custom delimiter logic. The solution must be self‑contained, with a clean implementation that avoids undefined behavior (e.g., reading past the end of the string) and uses `const` correctness throughout.

// The problem is a classic boundary‑scanning count of words separated by a set of delimiters (here, space and comma). The main algorithm iterates through the string once, using two inner skip loops: first, skip over consecutive delimiters until a non‑delimiter (start of a word) is found; if we reach the end of the string, stop. Otherwise increment the word counter, then skip over non‑delimiter characters until a delimiter or end‑of‑string is found. This essentially counts each maximal contiguous block of non‑delimiter characters.  
// Edge cases: (1) empty string – return 0; (2) string with only delimiters – the skip‑delimiters loop will reach the end without finding a word, so the loop terminates without incrementing; (3) leading/trailing delimiters – they are skipped before the first word and after the last word, so they don't affect the count; (4) multiple consecutive delimiters – they are skipped together, so no extra increments occur.  
// Time complexity is O(n) because each character is visited at most twice (once in a delimiter‑skip and once in a non‑delimiter‑skip). Space complexity is O(1) auxiliary, not counting the input string storage.

#include <string>

// Returns the number of non-empty segments separated by spaces or commas.
// Handles leading/trailing delimiters, consecutive delimiters, and empty input.
int countWordSegments(const std::string& s) {
    int count = 0;
    std::size_t i = 0;
    const std::size_t n = s.length();

    while (i < n) {
        // Skip delimiters (space or comma)
        while (i < n && (s[i] == ' ' || s[i] == ',')) {
            ++i;
        }
        if (i >= n) {
            break; // No more words
        }
        // Found a non-delimiter: start of a word
        ++count;
        // Skip over the word (non-delimiters)
        while (i < n && s[i] != ' ' && s[i] != ',') {
            ++i;
        }
    }

    return count;
}

#include <cassert>
#include <string>

// Function declaration (already defined in solution)
int countWordSegments(const std::string& s);

int main() {
    // Empty and delimiter-only strings
    assert(countWordSegments("") == 0);
    assert(countWordSegments("   , ,  ") == 0);

    // Simple single word, no delimiters
    assert(countWordSegments("Hello") == 1);

    // Leading/trailing delimiters
    assert(countWordSegments("   Hello   ") == 1);
    assert(countWordSegments(", Hello ,") == 1);

    // Multiple words with various delimiter combinations
    assert(countWordSegments("Hello, my name is John") == 5);
    assert(countWordSegments("a,b,c") == 3);
    assert(countWordSegments("a   b   c") == 3);
    assert(countWordSegments("  a , b , c  ") == 3);

    // Consecutive delimiters between words
    assert(countWordSegments("one  ,,  two") == 2);
    assert(countWordSegments(",,x,, y ,,") == 2);

    // Single character words
    assert(countWordSegments("a") == 1);
    assert(countWordSegments(" a b ") == 2);
}
