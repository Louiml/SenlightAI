/*
Write a C++ function `bool isFBMatch(const std::string& s)` that determines whether the given string `s` appears as a contiguous substring in the infinite periodic string formed by repeating the pattern "FBFFBFFB" (i.e., the sequence F, B, F, F, B, F, F, B, F, F, B, ...). The function should return `true` if `s` is a substring of this infinite pattern, and `false` otherwise. The input string consists only of uppercase 'F' and 'B' characters, and its length is between 1 and 50 inclusive. You may assume the infinite pattern extends far enough that any testable substring of length up to 50 will appear if it matches the pattern. Ensure your solution handles edge cases like single-character strings and strings that span pattern boundaries.
*/

#include <string>

// Check if the given string appears as a contiguous substring in the infinite
// periodic string "FBFFBFFB" (period 8). Returns true if found, false otherwise.
bool isFBMatch(const std::string& s) {
    const std::string period = "FBFFBFFB";
    // Build a long enough string to cover any substring of length up to 50,
    // plus some margin to handle crossing boundaries.
    std::string pattern;
    pattern.reserve(80); // 10 repetitions of the period
    for (int i = 0; i < 10; ++i) {
        pattern += period;
    }
    // Use find to search for s as a substring.
    return pattern.find(s) != std::string::npos;
}

#include <cassert>
#include <string>

// Function declaration (as would be given in the solution)
bool isFBMatch(const std::string& s);

int main() {
    // Basic single-character cases
    assert(isFBMatch("F") == true);
    assert(isFBMatch("B") == true);
    
    // Full period and substrings within it
    assert(isFBMatch("FBFFBFFB") == true);
    assert(isFBMatch("BFFB") == true);
    assert(isFBMatch("FFB") == true);
    
    // Cross-boundary substrings (period repeats)
    assert(isFBMatch("BFFBFFBF") == true); // spans across repetition
    assert(isFBMatch("BFB") == true);      // occurs at positions 6-8 in "FBFFBFFB"
    
    // Longer sequences that are valid (from repeated pattern)
    assert(isFBMatch("FBFFBFFBFBFFBFFB") == true); // length 17, two full periods
    assert(isFBMatch("FFBFFBFFBFFB") == true);     // length 13, crosses boundaries
    
    // Invalid patterns that should not appear
    assert(isFBMatch("BBB") == false);
    assert(isFBMatch("FFF") == false);
    assert(isFBMatch("BFBF") == false);
    assert(isFBMatch("FBB") == false);
    assert(isFBMatch("BBFF") == false);
    
    // Edge case: long string that is not valid (e.g., all B's length 50)
    std::string long_invalid(50, 'B');
    assert(isFBMatch(long_invalid) == false);
    
    return 0;
}

// The infinite pattern is periodic with period 8: "FBFFBFFB". Since the input length is at most 50, we can construct a sufficiently long prefix of this infinite string—say, the pattern repeated enough times to cover at least 50 characters plus the length of the input (to allow for substrings that cross boundaries). A safe approach is to repeat the pattern, say, 10 times to get a string of length 80, which is more than enough for any input up to length 50. Then use the standard `std::string::find` method to check if `s` occurs as a contiguous substring. If found, return `true`; otherwise, return `false`. Time complexity is O(L * M) in the worst case for naive substring search, where L is the length of the constructed string (80) and M is the input length (≤50), so effectively constant time. Space complexity is O(L) for the constructed pattern string. Edge cases include single-character inputs (e.g., "F" or "B" both appear), strings of length 1 that are not in the pattern (but both 'F' and 'B' appear, so all single-character strings are valid), and longer strings that cross the pattern boundary—handled by the repeated pattern.
