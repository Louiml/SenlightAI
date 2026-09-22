// Write a C++ function `char getMaxOccuringChar(const std::string& str)` that takes a non-empty string consisting only of lowercase English letters (`'a'` to `'z'`) and returns the character that appears most frequently. If multiple characters have the same maximum frequency, return the one that is lexicographically smallest (i.e., the closest to `'a'`). The function must handle strings of any length up to large sizes and must not modify the input string. The solution should be efficient and should not use any additional libraries beyond standard C++ headers.

#include <cassert>

int main() {
    assert(getMaxOccuringChar("a") == 'a');
    assert(getMaxOccuringChar("ab") == 'a'); // tie: 'a' and 'b' both 1, pick 'a'
    assert(getMaxOccuringChar("abcabc") == 'a'); // all appear twice, pick lexicographically smallest
    assert(getMaxOccuringChar("zzzz") == 'z');
    assert(getMaxOccuringChar("aabbbcc") == 'b');
    assert(getMaxOccuringChar("bca") == 'a');
    assert(getMaxOccuringChar("zyxwvutsrqponmlkjihgfedcba") == 'a'); // all once, pick 'a'
    assert(getMaxOccuringChar("hello") == 'l'); // 'l' appears twice, others once
    return 0;
}

#include <string>
#include <vector>
#include <climits>

// Return the most frequent lowercase character in the input string.
// In case of a tie, return the lexicographically smallest character.
char getMaxOccuringChar(const std::string& str) {
    std::vector<int> freq(26, 0);
    
    for (char ch : str) {
        ++freq[ch - 'a'];
    }
    
    int maxCount = 0;
    char result = 'a';
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > maxCount) {
            maxCount = freq[i];
            result = static_cast<char>('a' + i);
        }
    }
    
    return result;
}

// The algorithm uses a frequency array of size 26, one slot per lowercase letter. Iterate through each character in the input string, convert it to an index by subtracting `'a'`, and increment the corresponding frequency counter. After building the frequency table, scan the array from index 0 upward (which corresponds to `'a'` to `'z'`) and track the index with the highest count. Because we scan from `'a'` to `'z'` and only update when the count is strictly greater than the current maximum, the first encountered maximum is preserved, ensuring lexicographically smallest tie-breaking. Edge cases include strings of length 1, strings with all identical characters, and strings where ties occur; all are handled naturally. Time complexity is \(O(n)\) for a string of length \(n\), and space complexity is \(O(1)\) because the frequency array has a fixed size of 26. The function is `const`-correct because it does not modify the input string.
