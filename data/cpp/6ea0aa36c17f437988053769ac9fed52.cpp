// Write a C++ function `largeGroupPositions` that takes a string `s` consisting of lowercase English letters and returns a vector of vectors, where each inner vector contains exactly two integers: the starting index and ending index (inclusive) of every maximal consecutive group of identical characters whose length is at least 3. Groups must be reported in the order they appear in the string. If no such group exists, return an empty vector.

The algorithm scans the string with two pointers: `i` marks the start of the current run, and `j` is advanced to the first character that differs from `s[i]`. The length of the run is `j - i`. If this length is ≥ 3, we record `{i, j-1}`. Then we set `i = j` to begin processing the next run. Edge cases: a single character run, runs at the beginning/end, and all identical characters (the whole string is one group). The time complexity is O(n) because each character is visited exactly twice (once by `i`, once by `j`), and space complexity is O(n) in the worst case for the output, but O(1) auxiliary space besides the output.

#include <vector>
#include <string>

// Returns the start and end indices (inclusive) of every maximal run
// of identical characters in s whose length is at least 3.
std::vector<std::vector<int>> largeGroupPositions(const std::string& s) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(s.size());
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && s[j] == s[i]) {
            ++j;
        }
        if (j - i >= 3) {
            result.push_back({i, j - 1});
        }
        i = j;
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Function declaration (same as solution)
std::vector<std::vector<int>> largeGroupPositions(const std::string& s);

int main() {
    // Basic case
    assert(largeGroupPositions("abbxxxxzzy") == std::vector<std::vector<int>>({{3,6}}));
    // Multiple groups
    assert(largeGroupPositions("abc") == std::vector<std::vector<int>>());
    // Group at the start
    assert(largeGroupPositions("aaabbb") == std::vector<std::vector<int>>({{0,2},{3,5}}));
    // All same characters
    assert(largeGroupPositions("aaaa") == std::vector<std::vector<int>>({{0,3}}));
    // Exactly length 3 group
    assert(largeGroupPositions("abccc") == std::vector<std::vector<int>>({{2,4}}));
    // Length 2 is not large
    assert(largeGroupPositions("aabb") == std::vector<std::vector<int>>());
    // Group at the end
    assert(largeGroupPositions("xxxy") == std::vector<std::vector<int>>({{0,2}}));
    // Single character string
    assert(largeGroupPositions("z") == std::vector<std::vector<int>>());
    // Overlapping style multiple groups with a break
    assert(largeGroupPositions("aaabbbccc") == std::vector<std::vector<int>>({{0,2},{3,5},{6,8}}));
    // Empty string (should return empty)
    assert(largeGroupPositions("") == std::vector<std::vector<int>>());
    return 0;
}
