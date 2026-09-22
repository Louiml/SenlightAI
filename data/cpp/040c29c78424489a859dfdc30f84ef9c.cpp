// Write a C++ function named `concatenateSubstrings` that takes two vectors: a vector of strings `my_strings` and a vector of vector<int> `parts`. Each element of `parts` contains exactly two integers [start, end], representing an inclusive substring range. The function must concatenate the substring of each string at the corresponding index (from `my_strings[i]` starting at position `parts[i][0]` and ending at `parts[i][1]`, inclusive) into a single result string, in order. You may assume that all indices are valid (i.e., 0 ≤ start ≤ end < length of the string), and that the vectors are non-empty and of equal size. The function should return the final concatenated string.
// The solution iterates over each string and its corresponding part range. For each index `i`, extract the substring using `std::string::substr(pos, len)` where the starting position is `parts[i][0]` and the length is `parts[i][1] - parts[i][0] + 1`. Append the extracted substring to an accumulator string. No special edge cases exist because the problem guarantees valid indices and non-empty vectors; however, the code should still handle empty strings if provided (substr would return an empty string, which is safe to append). The time complexity is O(total length of the output string), and the auxiliary space complexity is O(1) besides the output string itself, since we only use a loop counter and a temporary substring.
#include <string>
#include <vector>

// Concatenates substrings of each string in my_strings based on inclusive ranges in parts.
// parts[i] = {start, end} → extracts my_strings[i].substr(start, end - start + 1).
std::string concatenateSubstrings(const std::vector<std::string>& my_strings,
                                  const std::vector<std::vector<int>>& parts) {
    std::string result;
    const std::size_t size = my_strings.size();
    for (std::size_t i = 0; i < size; ++i) {
        int start = parts[i][0];
        int length = parts[i][1] - parts[i][0] + 1;
        result += my_strings[i].substr(start, length);
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Declaration (already defined in solution)
std::string concatenateSubstrings(const std::vector<std::string>&,
                                  const std::vector<std::vector<int>>&);

int main() {
    // Basic case with multiple strings
    std::vector<std::string> s1 = {"hello", "world"};
    std::vector<std::vector<int>> p1 = {{1, 3}, {0, 1}};
    assert(concatenateSubstrings(s1, p1) == "ellwo");

    // Full string extraction
    std::vector<std::string> s2 = {"abc"};
    std::vector<std::vector<int>> p2 = {{0, 2}};
    assert(concatenateSubstrings(s2, p2) == "abc");

    // Single-character ranges
    std::vector<std::string> s3 = {"testing", "code"};
    std::vector<std::vector<int>> p3 = {{4, 4}, {1, 1}};
    assert(concatenateSubstrings(s3, p3) == "io");

    // Empty string (valid index 0)
    std::vector<std::string> s4 = {"", "x"};
    std::vector<std::vector<int>> p4 = {{0, 0}, {0, 0}};
    assert(concatenateSubstrings(s4, p4) == "x");

    // Multiple characters from each
    std::vector<std::string> s5 = {"programming", "challenge"};
    std::vector<std::vector<int>> p5 = {{0, 5}, {2, 7}};
    assert(concatenateSubstrings(s5, p5) == "prograallen");

    // All strings same length, varied parts
    std::vector<std::string> s6 = {"aaaa", "bbbb", "cccc"};
    std::vector<std::vector<int>> p6 = {{1, 2}, {0, 3}, {2, 3}};
    assert(concatenateSubstrings(s6, p6) == "aabbbbcc");

    // Large range spanning entire string
    std::vector<std::string> s7 = {"abcdef"};
    std::vector<std::vector<int>> p7 = {{0, 5}};
    assert(concatenateSubstrings(s7, p7) == "abcdef");

    // Multiple strings, ranges at end
    std::vector<std::string> s8 = {"short", "longer"};
    std::vector<std::vector<int>> p8 = {{3, 4}, {3, 5}};
    assert(concatenateSubstrings(s8, p8) == "ttger");

    // Consistent output with repeated characters
    std::vector<std::string> s9 = {"zz", "yy", "xx"};
    std::vector<std::vector<int>> p9 = {{0, 1}, {1, 1}, {0, 0}};
    assert(concatenateSubstrings(s9, p9) == "zzyx");

    return 0;
}
