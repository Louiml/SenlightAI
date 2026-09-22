// Write a C++ function named `isPermutation` that takes two constant string references and returns a boolean indicating whether one string is a permutation of the other (i.e., contains exactly the same characters with the same frequencies, but possibly in a different order). The input strings may contain any printable ASCII characters, including spaces, digits, punctuation, and uppercase/lowercase letters; the comparison must be case-sensitive and treat spaces as significant characters. The function must work for empty strings (two empty strings are permutations), handle strings of differing lengths immediately by returning false, and must not modify the input strings. You may use sorting or a frequency-counting approach, but must not rely on any external libraries beyond the C++ standard library.
// The solution can be approached in two main ways. The first is to create a copy of each string, sort both copies, and then compare them element-wise; if they are identical after sorting, they are permutations. This approach has time complexity \(O(n \log n)\) due to sorting, where \(n\) is the length of the strings, and uses \(O(n)\) auxiliary space for the copies. The second approach uses a fixed-size frequency array (e.g., of size 256 for all ASCII characters). We iterate over the first string and increment the count for each character, then iterate over the second string and decrement the count; if any count becomes negative or if at the end any count is nonzero, the strings are not permutations. This is \(O(n)\) time and \(O(1)\) auxiliary space (since the array is fixed at 256). Edge cases include: empty strings (should return true), strings of different lengths (immediate false), and strings with the same characters but different counts (e.g., "aab" vs "abb") which must be correctly identified as false. The sorting approach is simpler and robust, while the counting approach is more efficient; either is acceptable as long as the function is correct and const-correct.
#include <string>
#include <algorithm>
#include <vector>

// Returns true if str1 is a permutation of str2 (same multiset of characters).
bool isPermutation(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length()) {
        return false;
    }
    // Use a frequency count for all possible char values (0-255).
    std::vector<int> count(256, 0);
    for (char c : str1) {
        count[static_cast<unsigned char>(c)]++;
    }
    for (char c : str2) {
        int idx = static_cast<unsigned char>(c);
        count[idx]--;
        if (count[idx] < 0) {
            return false;
        }
    }
    // All counts should be zero now if they matched.
    return true;
}
#include <cassert>

int main() {
    // Basic true cases
    assert(isPermutation("abc", "cba") == true);
    assert(isPermutation("aabbcc", "abcabc") == true);
    assert(isPermutation("", "") == true);
    assert(isPermutation("hello", "olleh") == true);
    
    // Basic false cases
    assert(isPermutation("abc", "abd") == false);
    assert(isPermutation("abc", "ab") == false);  // different lengths
    assert(isPermutation("aab", "abb") == false); // same chars, different counts
    
    // Edge cases with spaces and case sensitivity
    assert(isPermutation("a b", "b a") == true);
    assert(isPermutation("Abc", "abc") == false); // case-sensitive
    
    // Larger test with punctuation
    std::string s1 = "C++ is fun!";
    std::string s2 = "fun! C++ is";
    assert(isPermutation(s1, s2) == true);
    
    // Null characters and special ASCII
    assert(isPermutation(std::string("\0\1", 2), std::string("\1\0", 2)) == true);
}
