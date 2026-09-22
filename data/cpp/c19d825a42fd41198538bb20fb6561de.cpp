Write a standalone C++ function that, given a non-empty string consisting only of uppercase English letters, returns the length of the longest contiguous substring composed exclusively of the characters 'A', 'C', 'G', or 'T'. The function must handle strings of arbitrary length, including strings with no such characters (return 0) and strings entirely composed of them (return the full length). The input string is passed by const reference; the function must not modify it.
The solution uses a single left-to-right scan of the string. Maintain a running count (`current_run`) of consecutive ACGT characters. For each character, if it is one of the four allowed, increment the run; otherwise, update the answer with the maximum of the current max and the run, then reset the run to zero. After the loop, one final update accounts for a run that reaches the end of the string. Edge cases: empty string (though specified non-empty, safe to return 0), no valid characters (all resets, answer remains 0), and all valid characters (run grows to full length and final update captures it). Time complexity is O(n) where n is the string length; space complexity is O(1) because only two integers are used.
#include <string>
#include <algorithm>

// Returns the length of the longest contiguous substring consisting only of A, C, G, or T.
int longestACGTSubstring(const std::string& s) {
    int max_len = 0;
    int current_run = 0;
    
    for (char c : s) {
        if (c == 'A' || c == 'C' || c == 'G' || c == 'T') {
            ++current_run;
        } else {
            max_len = std::max(max_len, current_run);
            current_run = 0;
        }
    }
    return std::max(max_len, current_run);
}
#include <cassert>
#include <string>
#include "solution.h" // assume the solution is in this header, or paste the function above

int main() {
    // Basic cases
    assert(longestACGTSubstring("ACGT") == 4);
    assert(longestACGTSubstring("AAAA") == 4);
    
    // Mixed with non-ACGT characters
    assert(longestACGTSubstring("ACGTXACGT") == 4);
    assert(longestACGTSubstring("AACGTTA") == 5);  // "AACGT" length 5
    
    // No valid characters
    assert(longestACGTSubstring("XYZ") == 0);
    assert(longestACGTSubstring("B") == 0);
    
    // Run at the very end
    assert(longestACGTSubstring("XYZAC") == 2);
    
    // Run at the very beginning
    assert(longestACGTSubstring("ACXYZ") == 2);
    
    // Multiple runs, pick longest
    assert(longestACGTSubstring("ACGTXXCGT") == 4);
    assert(longestACGTSubstring("AAATTTCCCGGG") == 12); // entire string
    
    // Single character valid
    assert(longestACGTSubstring("G") == 1);
    
    // Single character invalid
    assert(longestACGTSubstring("N") == 0);
    
    return 0;
}
