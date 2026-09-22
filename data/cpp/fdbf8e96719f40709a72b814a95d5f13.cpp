Write a C++ function that takes a non-empty string `word` and returns a vector of strings containing all non-empty substrings of `word`, ordered first by increasing substring length (from 1 to the full length), and within each length, ordered by their starting position from index 0 to `word.length() - length`. The substrings should be generated exactly as in the given snippet: for each length `i` from 1 to n, output all substrings starting at each valid index `j` (0 to n-i inclusive) using `word.substr(j, i)`. The function must preserve the original order and should handle strings with repeated characters and single-character inputs correctly.
#include <cassert>
#include <string>
#include <vector>

// Include the function definition here (for completeness in test) or separate header.
int main() {
    // Single character
    std::vector<std::string> r1 = allSubstringsOrdered("a");
    assert(r1.size() == 1 && r1[0] == "a");

    // Two characters, one repeated
    std::vector<std::string> r2 = allSubstringsOrdered("aa");
    assert(r2.size() == 3);
    assert(r2[0] == "a" && r2[1] == "a" && r2[2] == "aa");

    // Three distinct characters
    std::vector<std::string> r3 = allSubstringsOrdered("abc");
    assert(r3.size() == 6);
    assert(r3[0] == "a" && r3[1] == "b" && r3[2] == "c");
    assert(r3[3] == "ab" && r3[4] == "bc");
    assert(r3[5] == "abc");

    // String with spaces (spaces are valid characters)
    std::vector<std::string> r4 = allSubstringsOrdered("a b");
    assert(r4.size() == 6);
    assert(r4[0] == "a" && r4[1] == " " && r4[2] == "b");
    assert(r4[3] == "a " && r4[4] == " b");
    assert(r4[5] == "a b");

    // Longer string, ensure order and count: n*(n+1)/2 substrings for n=4 => 10
    std::vector<std::string> r5 = allSubstringsOrdered("abcd");
    assert(r5.size() == 10);
    assert(r5[0] == "a" && r5[1] == "b" && r5[2] == "c" && r5[3] == "d");
    assert(r5[4] == "ab" && r5[5] == "bc" && r5[6] == "cd");
    assert(r5[7] == "abc" && r5[8] == "bcd");
    assert(r5[9] == "abcd");

    // Check empty string is NOT allowed by task (non-empty input), but function would return empty vector
    // Uncomment to test if desired: assert(allSubstringsOrdered("").empty());

    return 0;
}
#include <string>
#include <vector>

// Return all substrings of word ordered by increasing length, then by start index.
std::vector<std::string> allSubstringsOrdered(const std::string& word) {
    std::vector<std::string> result;
    const size_t n = word.length();

    // For each possible substring length, from 1 to full length
    for (size_t len = 1; len <= n; ++len) {
        // For each valid starting position
        for (size_t start = 0; start <= n - len; ++start) {
            result.push_back(word.substr(start, len));
        }
    }

    return result;
}
// The solution mirrors the nested-loop logic of the provided snippet. The outer loop iterates over substring length `len` from 1 to `word.size()`. The inner loop iterates over starting positions `start` from 0 to `word.size() - len`, appending `word.substr(start, len)` to the result vector. The order is naturally determined: longer substrings appear later, and for the same length, earlier starting positions appear first. Edge cases include: when the string has length 1, only one substring (the entire string) is produced; when the string contains repeated characters, each distinct occurrence is output separately (e.g., "aa" yields "a", "a", "aa"). The algorithm runs in O(n³) time in the worst case because there are O(n²) substrings and each substring copy costs O(len) time on average; total output size is O(n³). Auxiliary space is O(n²) to store all substrings (excluding the input), since the total number of characters across all substrings is O(n³) but the vector of strings itself uses O(n²) pointers and string overhead, plus the actual character data.
