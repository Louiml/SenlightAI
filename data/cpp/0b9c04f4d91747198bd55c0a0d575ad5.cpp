/*
Write a C++ function that takes an integer `N` and a vector of strings (each string consisting only of lowercase English letters), and returns the first string (in lexicographical order) that appears more than once in the input. If no string appears more than once, return an empty string. The function must handle up to 10^5 strings, each of length up to 10. The order is determined by the standard lexicographical comparison (`std::string`’s `<` operator), and duplicates are counted across all positions. For example, given `N=5` and `{"apple", "banana", "apple", "cherry", "banana"}`, the output is `"apple"` because it appears twice and is lexicographically smaller than `"banana"`.
*/
#include <string>
#include <vector>
#include <map>

// Return the lexicographically smallest string that appears more than once.
// Return an empty string if no duplicates exist.
std::string firstDuplicateLexicographic(const std::vector<std::string>& strings) {
    std::map<std::string, int> counts;
    for (const auto& s : strings) {
        ++counts[s];  // Insert or increment; map keeps ordering.
    }
    for (const auto& entry : counts) {
        if (entry.second > 1) {
            return entry.first;
        }
    }
    return "";
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration from solution (would be included in a header in practice)
std::string firstDuplicateLexicographic(const std::vector<std::string>& strings);

int main() {
    assert(firstDuplicateLexicographic({"apple", "banana", "apple", "cherry", "banana"}) == "apple");
    assert(firstDuplicateLexicographic({"zebra", "alpha", "zebra"}) == "zebra");
    assert(firstDuplicateLexicographic({"a", "b", "c"}) == "");
    assert(firstDuplicateLexicographic({"same", "same", "same"}) == "same");
    assert(firstDuplicateLexicographic({}) == "");
    assert(firstDuplicateLexicographic({"b", "a", "a", "b"}) == "a");
    assert(firstDuplicateLexicographic({"x", "y", "x", "y", "y"}) == "x");
    assert(firstDuplicateLexicographic({"hello", "world", "hello"}) == "hello");
    assert(firstDuplicateLexicographic({"abc", "abcd", "abc"}) == "abc");
    assert(firstDuplicateLexicographic({"one", "two", "three", "one", "two", "three"}) == "one");
    return 0;
}
// The solution uses a `std::map<std::string, int>` to count occurrences of each string while preserving lexicographical order automatically, since `std::map` is ordered by key. Iterate through each string in the input vector, increment its count in the map. After building the frequency map, iterate through the map from `begin()` to `end()` — this traversal is already in lexicographical order. For each entry, if the count is greater than 1, return that string immediately; since the map is traversed in ascending order, the first such entry is the answer. If no entry has count >1, return an empty string. Edge cases: an empty vector returns empty string; if only one string appears multiple times, that is returned; if all strings are unique, return empty. Time complexity: O(N log U) where U is the number of unique strings, because each insertion and lookup in the map is O(log U). Space complexity: O(U) for the map storage. The approach is straightforward and robust.
