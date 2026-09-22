// Write a C++ function `minimumMismatches` that takes a string `s` containing only lowercase English letters and returns the minimum number of positions that must be changed so that the string becomes sorted in non-decreasing order (i.e., each character is less than or equal to the next character). The function should output the count of positions where the original character differs from the character in the sorted version of the same string. The input string may contain duplicate characters, and the function should handle empty strings by returning 0. The function must be pure and not modify the input string.

#include <cassert>

int main() {
    // Test with already sorted string
    assert(minimumMismatches("abc") == 0);
    // Test with reversed string
    assert(minimumMismatches("cba") == 2);
    // Test with duplicates
    assert(minimumMismatches("aabb") == 0);
    // Test with single character
    assert(minimumMismatches("z") == 0);
    // Test with empty string
    assert(minimumMismatches("") == 0);
    // Test with mixed sorted and unsorted
    assert(minimumMismatches("bdac") == 3);
    // Test with all same characters
    assert(minimumMismatches("cccc") == 0);
    // Test with long string containing duplicates
    assert(minimumMismatches("edcba") == 4);
    // Test with case where only middle is wrong
    assert(minimumMismatches("abdc") == 2);
    // Test with two characters swapped
    assert(minimumMismatches("ba") == 2);
    return 0;
}

#include <string>
#include <algorithm>

// Returns the minimum number of positions that must be changed
// so that the string becomes sorted in non-decreasing order.
int minimumMismatches(const std::string& s) {
    std::string sorted = s;                    // copy the original
    std::sort(sorted.begin(), sorted.end());   // sort the copy
    int mismatches = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (sorted[i] != s[i]) {
            ++mismatches;
        }
    }
    return mismatches;
}

// The solution is straightforward: sort a copy of the input string, then compare it position-by-position with the original. The number of mismatches is exactly the number of positions where the original character is not equal to the sorted character. This works because the minimum number of changes to make a string sorted is achieved by replacing each character with the corresponding character from the fully sorted string; any other sorted string would require at least as many changes. Edge cases: empty string returns 0; duplicate characters require no special handling since sorting preserves equality where possible. Time complexity is O(n log n) due to sorting, with O(n) auxiliary space for the copy. If the input is already sorted, the answer is 0.
