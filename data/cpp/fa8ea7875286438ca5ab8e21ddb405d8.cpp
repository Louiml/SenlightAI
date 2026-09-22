Write a C++ function that takes two strings, `text` and `pattern`, and returns a vector of integers containing all 0-based starting indices where the `pattern` occurs as a contiguous substring in `text`. The function should perform a simple naive pattern search with no overlapping matches skipped (i.e., if pattern matches at index `i`, the search continues from `i+1`, so overlapping occurrences like "ana" in "banana" would both be reported). If no matches are found, the vector should be empty. The function must be case-sensitive and handle empty patterns (return empty vector) and cases where the pattern is longer than the text (also return empty vector). You may assume both inputs are valid strings (can be empty). The function signature should be: `std::vector<int> findPatternIndices(const std::string& text, const std::string& pattern)`. Do not use advanced string search algorithms like KMP or the standard library `find` — implement the naive loop explicitly.
// The solution uses a nested-loop naive pattern matching algorithm. The outer loop iterates over every possible starting position `i` in `text` from 0 to `lt - lp` (inclusive), but we can simply iterate from 0 to `lt - 1` and check if `i + lp <= lt`. For each start, the inner loop compares each character of the pattern against the corresponding character in `text`. If all characters match, we record the index `i`. The search continues to `i+1` after a match, so overlapping patterns are reported. Edge cases: empty pattern → return empty (since there is no meaningful match; also pattern length zero would cause infinite loop in inner loop if not handled). Pattern longer than text → no possible start, loop condition naturally fails if we set upper bound as `lt - lp`. Also handle empty text and non-empty pattern → no matches. Time complexity is O(lt * lp) worst-case (e.g., pattern "aaaa" in text "aaaaa"), average O(lt) for random strings. Space complexity O(1) extra besides the output vector, which is O(number of matches).
#include <vector>
#include <string>

/**
 * @brief Find all 0-based starting indices where pattern occurs in text (naive search).
 * 
 * @param text The string to search within.
 * @param pattern The substring to look for.
 * @return std::vector<int> Vector of starting indices (in increasing order) where pattern matches.
 */
std::vector<int> findPatternIndices(const std::string& text, const std::string& pattern) {
    std::vector<int> indices;
    
    // Edge cases: empty pattern or pattern longer than text -> no matches
    if (pattern.empty() || pattern.size() > text.size()) {
        return indices;
    }
    
    const int textLength = static_cast<int>(text.size());
    const int patternLength = static_cast<int>(pattern.size());
    
    // Only need to check starting positions up to textLength - patternLength
    for (int i = 0; i <= textLength - patternLength; ++i) {
        bool match = true;
        for (int j = 0; j < patternLength; ++j) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            indices.push_back(i);
        }
    }
    
    return indices;
}
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Function prototype (declaration matches the solution)
std::vector<int> findPatternIndices(const std::string& text, const std::string& pattern);

int main() {
    // Test 1: Basic match
    assert(findPatternIndices("sanjay's nick-name is sanju", "an") == std::vector<int>({1, 23}));
    
    // Test 2: Overlapping matches
    assert(findPatternIndices("banana", "ana") == std::vector<int>({1, 3}));
    
    // Test 3: Multiple non-overlapping matches
    assert(findPatternIndices("aaaa", "aa") == std::vector<int>({0, 1, 2}));
    
    // Test 4: No match
    assert(findPatternIndices("hello world", "xyz") == std::vector<int>({}));
    
    // Test 5: Empty pattern
    assert(findPatternIndices("abc", "") == std::vector<int>({}));
    
    // Test 6: Empty text
    assert(findPatternIndices("", "a") == std::vector<int>({}));
    
    // Test 7: Pattern longer than text
    assert(findPatternIndices("abc", "abcd") == std::vector<int>({}));
    
    // Test 8: Single character pattern
    assert(findPatternIndices("abca", "a") == std::vector<int>({0, 3}));
    
    // Test 9: Case sensitivity
    assert(findPatternIndices("abcABC", "a") == std::vector<int>({0}));
    
    // Test 10: Pattern exactly equal to text
    assert(findPatternIndices("abc", "abc") == std::vector<int>({0}));
    
    std::cout << "All tests passed!\n";
    return 0;
}
