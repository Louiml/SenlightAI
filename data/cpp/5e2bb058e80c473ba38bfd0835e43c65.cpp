/*
Write a C++ function that takes a string `s` containing only lowercase English letters and returns the smallest lexicographically possible string that can be obtained by removing duplicate characters while preserving the relative order of the first occurrences of each character. In other words, the result must contain each distinct character exactly once, and among all such possible strings (formed by deleting some characters from `s`), the returned string must be the lexicographically smallest one. For example, given `"bcabc"`, the possible results include `"bca"`, `"bac"`, `"cab"`, `"abc"`, etc., and the smallest lexicographically is `"abc"`. The input string will have length between 1 and 10,000, inclusive. If the input contains only one distinct character, the output is just that single character.
*/
#include <string>
#include <vector>

// Returns the lexicographically smallest string that contains each distinct
// character from s exactly once, preserving relative order of first occurrences.
std::string smallestDistinctString(const std::string& s) {
    std::vector<int> remaining(26, 0);
    std::vector<bool> inResult(26, false);

    // Count occurrences of each character.
    for (char c : s) {
        ++remaining[c - 'a'];
    }

    std::string result;
    result.reserve(26); // at most 26 distinct characters

    for (char c : s) {
        int index = c - 'a';
        --remaining[index];

        if (inResult[index]) {
            continue; // already placed, skip
        }

        // Remove larger characters from the result if they still appear later.
        while (!result.empty() && result.back() > c && remaining[result.back() - 'a'] > 0) {
            inResult[result.back() - 'a'] = false;
            result.pop_back();
        }

        result.push_back(c);
        inResult[index] = true;
    }

    return result;
}
#include <cassert>
#include <string>

// Function declaration from the solution.
std::string smallestDistinctString(const std::string& s);

int main() {
    // Basic examples
    assert(smallestDistinctString("bcabc") == "abc");
    assert(smallestDistinctString("cbacdcbc") == "acdb");
    
    // All distinct characters: result is same as input
    assert(smallestDistinctString("zyx") == "zyx");
    assert(smallestDistinctString("abcdefghijklmnopqrstuvwxyz") == "abcdefghijklmnopqrstuvwxyz");
    
    // All same character
    assert(smallestDistinctString("aaaaaa") == "a");
    
    // Single character
    assert(smallestDistinctString("z") == "z");
    
    // Already sorted with duplicates
    assert(smallestDistinctString("aabbcc") == "abc");
    
    // Reverse order with duplicates
    assert(smallestDistinctString("ccbbaa") == "cba"); // lexicographically smallest is "abc"? Actually "cba" is larger than "abc", but "abc" is impossible because order must preserve first occurrences: first 'c' before 'b' before 'a' -> we can only delete, not reorder. So "cba" is correct since we cannot get "abc" without reordering.
    // Wait: let's check: input "ccbbaa": first occurrences are c (index 0), b (index 2), a (index 4). We can delete all extra c's and b's -> "cba". That's the only possible order without rearranging. So correct.
    
    // Mixed with duplicates that require greedy removal
    assert(smallestDistinctString("abacb") == "abc"); 
    // Explanation: freq a:2, b:2, c:1. Process a -> res="a"; b -> res="ab"; a: dec freq a to 1, inResult? yes skip; c -> res="abc"; b: inResult? yes skip. Result "abc".
    
    // Long string with many duplicates
    assert(smallestDistinctString("edcbaedcba") == "edcba");
    
    return 0;
}
// The solution uses a greedy stack-based approach combined with frequency counting and a visited flag. First, count the total occurrences of each character in the input string in a frequency array `freq`. Then iterate through the string from left to right. For each character, decrement its remaining frequency (since we've now processed one occurrence). If the character is not already placed in the result (tracked by a `vis` boolean array), we consider adding it. Before adding, we need to ensure the result stays lexicographically smallest. While the result is non-empty, the last character in the result is greater than the current character, and that last character still has remaining occurrences later in the string (so it can be re-added later), we pop it from the result and mark it as not visited. This greedily removes larger characters that can be replaced by a smaller one appearing later. After the popping loop, we append the current character and mark it as visited. This guarantees that each character appears exactly once, and the final order is the smallest possible lexicographic string.
//
// Edge cases: The input may contain all distinct characters (then result is the original string), all identical characters (result is a single char), or characters that appear many times; the algorithm handles duplicates naturally. The stack/string operations maintain order. Time complexity is O(n) because each character is pushed and popped at most once. Space complexity is O(1) for the fixed-size arrays plus O(n) for the result string, which is unavoidable since the result length is at most 26 (number of distinct lowercase letters) — but in practice we consider the output string O(1) relative to character set size, and the input processing is O(n). Overall complexity: O(n) time and O(1) extra space excluding input/output.
