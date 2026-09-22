/*
Write a C++ function named `groupAnagrams` that takes a vector of lowercase strings and returns a `vector<vector<string>>`, where each inner vector contains all strings from the input that are anagrams of each other (i.e., strings that contain exactly the same letters but in a different order). The order of the groups and the order of strings within each group do not matter. The function must be robust for an empty input vector, for input containing duplicate strings, and for single-character strings. Explicitly handle the constraint that all input strings are non-empty and consist only of lowercase English letters. Your implementation must use a hash map keyed by the sorted version of each string to group anagrams efficiently.
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Groups strings that are anagrams of each other.
// Each inner vector contains all strings from the input that are anagrams.
// The order of groups and strings within groups is unspecified.
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    for (const std::string& s : strs) {
        std::string key = s;
        std::sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    std::vector<std::vector<std::string>> result;
    result.reserve(groups.size());
    for (const auto& entry : groups) {
        result.push_back(entry.second);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// Helper to check if two ragged 2D vectors contain the same groups regardless of order.
static bool sameGroups(const std::vector<std::vector<std::string>>& a,
                       const std::vector<std::vector<std::string>>& b) {
    if (a.size() != b.size()) return false;
    std::vector<std::vector<std::string>> aCopy = a;
    std::vector<std::vector<std::string>> bCopy = b;
    for (auto& group : aCopy) std::sort(group.begin(), group.end());
    for (auto& group : bCopy) std::sort(group.begin(), group.end());
    std::sort(aCopy.begin(), aCopy.end());
    std::sort(bCopy.begin(), bCopy.end());
    return aCopy == bCopy;
}

int main() {
    // Example from the problem statement
    std::vector<std::string> input1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto result1 = groupAnagrams(input1);
    std::vector<std::vector<std::string>> expected1 = {{"eat", "tea", "ate"}, {"tan", "nat"}, {"bat"}};
    assert(sameGroups(result1, expected1));

    // Empty input
    std::vector<std::string> input2;
    auto result2 = groupAnagrams(input2);
    assert(result2.empty());

    // Single string
    std::vector<std::string> input3 = {"abc"};
    auto result3 = groupAnagrams(input3);
    assert(result3.size() == 1 && result3[0].size() == 1 && result3[0][0] == "abc");

    // Duplicates
    std::vector<std::string> input4 = {"a", "a", "b", "b", "c"};
    auto result4 = groupAnagrams(input4);
    std::vector<std::vector<std::string>> expected4 = {{"a", "a"}, {"b", "b"}, {"c"}};
    assert(sameGroups(result4, expected4));

    // All same letters, different order
    std::vector<std::string> input5 = {"ab", "ba", "ab"};
    auto result5 = groupAnagrams(input5);
    std::vector<std::vector<std::string>> expected5 = {{"ab", "ba", "ab"}};
    assert(sameGroups(result5, expected5));

    // Single character strings
    std::vector<std::string> input6 = {"x", "y", "x"};
    auto result6 = groupAnagrams(input6);
    std::vector<std::vector<std::string>> expected6 = {{"x", "x"}, {"y"}};
    assert(sameGroups(result6, expected6));

    return 0;
}
// The core idea is to transform each string into a canonical key by sorting its characters. Since anagrams share the same multiset of letters, sorting all strings in the same anagram group yields the same sorted key. For example, `"eat"`, `"tea"`, and `"ate"` all become `"aet"` after sorting. We then use an unordered map from that key to a vector of original strings. For each input string, sort it, look up (or insert) the key in the map, and append the original unsorted string to the corresponding vector. Finally, collect all vectors from the map into the result. Edge cases: an empty input yields an empty result; a string with a single character sorts to itself; duplicate strings will appear in the same group because they have identical sorted keys. Sorting each string takes \(O(k \log k)\) where \(k\) is the average string length, so the total time is \(O(n \cdot k \log k)\) for \(n\) strings. The auxiliary space is \(O(n \cdot k)\) for storing the map keys and values. No special handling for lowercase is needed beyond sorting, but the problem guarantee ensures no non-lowercase characters.
