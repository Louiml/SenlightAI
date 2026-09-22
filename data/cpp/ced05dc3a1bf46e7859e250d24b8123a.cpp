// Write a C++ function that takes an integer `n` and a vector of `n` strings, then returns a vector of pairs (string, int) containing each unique string in lexicographical order along with its frequency (number of occurrences) in the original input. The strings may contain lowercase and uppercase letters, digits, and may be of length up to 100. The function must handle duplicates and return the result sorted lexicographically (using standard `std::string` comparison, which is case-sensitive, so `"abc"` and `"ABC"` are distinct). Ensure the function is efficient for `n` up to 100,000.

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Assume the solution function is included above.

int main() {
    // Test 1: empty input
    std::vector<std::string> input1;
    assert(uniqueStringFrequencies(input1).empty());

    // Test 2: all distinct strings, order checked
    std::vector<std::string> input2 = {"banana", "apple", "cherry"};
    std::vector<std::pair<std::string, int>> expected2 = {{"apple", 1}, {"banana", 1}, {"cherry", 1}};
    assert(uniqueStringFrequencies(input2) == expected2);

    // Test 3: duplicates with correct frequencies
    std::vector<std::string> input3 = {"abc", "def", "abc", "ghj", "jkl", "ghj", "ghj", "abc"};
    std::vector<std::pair<std::string, int>> expected3 = {{"abc", 3}, {"def", 1}, {"ghj", 3}, {"jkl", 1}};
    assert(uniqueStringFrequencies(input3) == expected3);

    // Test 4: case sensitivity
    std::vector<std::string> input4 = {"Hello", "hello", "HELLO", "Hello"};
    std::vector<std::pair<std::string, int>> expected4 = {{"HELLO", 1}, {"Hello", 2}, {"hello", 1}};
    assert(uniqueStringFrequencies(input4) == expected4);

    // Test 5: all identical
    std::vector<std::string> input5 = {"same", "same", "same"};
    std::vector<std::pair<std::string, int>> expected5 = {{"same", 3}};
    assert(uniqueStringFrequencies(input5) == expected5);

    // Test 6: numeric-like strings and mixed characters
    std::vector<std::string> input6 = {"123", "12", "123", "a1b", "a1b", "z"};
    std::vector<std::pair<std::string, int>> expected6 = {{"12", 1}, {"123", 2}, {"a1b", 2}, {"z", 1}};
    assert(uniqueStringFrequencies(input6) == expected6);

    // Test 7: long string and large n (example with 1000 copies of one string)
    std::vector<std::string> input7(1000, "repeated");
    std::vector<std::pair<std::string, int>> expected7 = {{"repeated", 1000}};
    assert(uniqueStringFrequencies(input7) == expected7);

    // Test 8: already sorted input (order should be preserved)
    std::vector<std::string> input8 = {"a", "b", "c"};
    std::vector<std::pair<std::string, int>> expected8 = {{"a", 1}, {"b", 1}, {"c", 1}};
    assert(uniqueStringFrequencies(input8) == expected8);

    return 0;
}

#include <vector>
#include <string>
#include <map>
#include <utility>

// Returns a vector of (string, frequency) pairs for unique strings,
// sorted lexicographically by string.
std::vector<std::pair<std::string, int>> uniqueStringFrequencies(
    const std::vector<std::string>& inputStrings) {
    std::map<std::string, int> freqMap;

    for (const std::string& s : inputStrings) {
        ++freqMap[s];
    }

    std::vector<std::pair<std::string, int>> result;
    result.reserve(freqMap.size());

    for (const auto& entry : freqMap) {
        result.emplace_back(entry.first, entry.second);
    }

    return result;
}

// The core idea is to use a `std::map<string, int>` (or `std::unordered_map` plus sorting, but the map is simpler and directly orders keys lexicographically). We iterate through each string in the input vector, incrementing the count in the map for that string. The map automatically maintains keys in sorted order. After processing all strings, we iterate through the map and build a vector of pairs, where each pair is `(string, count)`. Edge cases: empty input vector returns an empty vector; strings with different cases are considered distinct; strings may contain spaces? Since inputs are given as separate elements in a vector, spaces are not an issue, but if they were they'd be fine as part of the string. Time complexity is O(n log u) where u is the number of unique strings (worst-case O(n log n)), and space is O(u).
