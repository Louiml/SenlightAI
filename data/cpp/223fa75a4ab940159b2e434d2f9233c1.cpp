Given a binary string `s` consisting only of characters `'0'` and `'1'`, write a C++ function `int minAlternatingOperations(const std::string& s)` that returns the minimum number of character changes needed to make the string alternating (i.e., no two adjacent characters are the same). You may only flip a character (change `'0'` to `'1'` or vice versa) one at a time. The input string is guaranteed to be non-empty. The function must handle strings of arbitrary length and should not modify the input string.

// The core observation is that a valid alternating binary string can only have one of two distinct patterns:  
// - Pattern A: starts with `'0'`, then `'1'`, then `'0'`, ... (i.e., at every index `i`, the expected character is `'0' + (i % 2)`).  
// - Pattern B: starts with `'1'`, then `'0'`, then `'1'`, ... (i.e., at every index `i`, the expected character is `'0' + ((i+1) % 2)`).
//
// For each pattern, we compare the actual character at position `i` with the expected character. If they differ, that means we would need one flip to make that position match the pattern. Summing these mismatches across all positions gives the total flips required to transform the string into that particular pattern. The answer is simply the minimum of the two totals. This works because any alternating string must match exactly one of these two patterns, and the number of flips is exactly the Hamming distance to that pattern. Edge cases include single-character strings (always already alternating, so answer is 0) and strings that are already alternating (both patterns yield different totals, but one will be 0). Time complexity is O(n) where n is the length of the string, and space complexity is O(1) since we only use a few integer variables.

#include <string>
#include <algorithm>

// Return the minimum number of character flips to make the string alternating.
int minAlternatingOperations(const std::string& s) {
    const int n = static_cast<int>(s.size());
    
    int flipsPatternA = 0; // Pattern A: starts with '0'
    int flipsPatternB = 0; // Pattern B: starts with '1'
    
    for (int i = 0; i < n; ++i) {
        // Expected char for Pattern A at index i: '0' + (i % 2)
        char expectedA = static_cast<char>('0' + (i % 2));
        if (s[i] != expectedA) {
            ++flipsPatternA;
        }
        
        // Expected char for Pattern B at index i: '0' + ((i+1) % 2)
        char expectedB = static_cast<char>('0' + ((i + 1) % 2));
        if (s[i] != expectedB) {
            ++flipsPatternB;
        }
    }
    
    return std::min(flipsPatternA, flipsPatternB);
}

#include <cassert>
#include <string>

// Declaration of the function being tested (provided above).
int minAlternatingOperations(const std::string& s);

int main() {
    // Single character is already alternating.
    assert(minAlternatingOperations("0") == 0);
    assert(minAlternatingOperations("1") == 0);
    
    // Already alternating strings.
    assert(minAlternatingOperations("010101") == 0);
    assert(minAlternatingOperations("1010") == 0);
    
    // Need one flip.
    assert(minAlternatingOperations("000") == 1); // flip middle to get 010
    assert(minAlternatingOperations("111") == 1); // flip middle to get 101
    
    // More complex cases.
    assert(minAlternatingOperations("0100") == 1); // change last to 1 -> 0101
    assert(minAlternatingOperations("1001") == 2); // either 1010 or 0101 requires 2 flips
    assert(minAlternatingOperations("1111") == 2); // change positions 1 and 3 -> 1010
    
    // Long case with mixed flips.
    assert(minAlternatingOperations("000111") == 3); // Always need at least 3 flips
    
    return 0;
}
