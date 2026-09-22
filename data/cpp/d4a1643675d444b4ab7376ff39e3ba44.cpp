Write a C++ function that takes a vector of strings and returns a new vector containing the unique strings sorted first by length in ascending order, and for strings of equal length, sorted lexicographically in ascending order (case-sensitive). Duplicate strings should appear only once in the output. The input vector may be empty, may contain strings of varying lengths, and may contain duplicates. The function should not modify the original vector.

// The solution uses the standard `std::sort` algorithm with a custom comparator that compares strings primarily by their `.size()` and secondarily by their lexicographical order using the default `<` operator, which is case-sensitive and follows ASCII ordering. After sorting, we iterate through the sorted vector and copy each string to a result vector only if it differs from the previous element (or if it's the first element), thereby removing duplicates. Important edge cases include an empty input (return an empty vector), a single string (return it as-is), multiple identical strings (collapse to one), and strings of equal length that differ in case (e.g., "abc" and "ABC" are distinct). The time complexity is O(N log N) for sorting, with O(N) for duplication removal and output building, and O(N) auxiliary space for the result vector. Sorting is in-place on the copy, so we take the input by value to avoid modifying the original.

#include <vector>
#include <string>
#include <algorithm>

// Sort strings by length (ascending), then lexicographically (case-sensitive).
// Remove duplicates. Returns a new vector; input is unchanged.
std::vector<std::string> sortAndUnique(std::vector<std::string> strings) {
    if (strings.empty()) {
        return {};
    }

    std::sort(strings.begin(), strings.end(),
        [](const std::string& a, const std::string& b) {
            if (a.size() != b.size()) {
                return a.size() < b.size();
            }
            return a < b;
        });

    std::vector<std::string> result;
    result.reserve(strings.size());
    result.push_back(strings[0]);
    for (size_t i = 1; i < strings.size(); ++i) {
        if (strings[i] != strings[i - 1]) {
            result.push_back(strings[i]);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above.

int main() {
    // Basic sorting and duplicate removal
    std::vector<std::string> input1 = {"banana", "apple", "fig", "apple", "date"};
    std::vector<std::string> expected1 = {"fig", "date", "apple", "banana"};
    assert(sortAndUnique(input1) == expected1);

    // All duplicates collapse to one
    std::vector<std::string> input2 = {"aa", "aa", "aa"};
    std::vector<std::string> expected2 = {"aa"};
    assert(sortAndUnique(input2) == expected2);

    // Empty input
    std::vector<std::string> input3;
    assert(sortAndUnique(input3).empty());

    // Single string
    std::vector<std::string> input4 = {"hello"};
    std::vector<std::string> expected4 = {"hello"};
    assert(sortAndUnique(input4) == expected4);

    // Equal length, case-sensitive lexicographic order
    std::vector<std::string> input5 = {"abc", "ABc", "abC", "Abc"};
    std::vector<std::string> expected5 = {"ABc", "Abc", "abC", "abc"};
    assert(sortAndUnique(input5) == expected5);

    // Mixed lengths and duplicates
    std::vector<std::string> input6 = {"z", "aa", "b", "cc", "z", "b", "aaa"};
    std::vector<std::string> expected6 = {"b", "z", "aa", "cc", "aaa"};
    assert(sortAndUnique(input6) == expected6);

    // Original vector unchanged
    std::vector<std::string> original = {"b", "a", "a", "c"};
    std::vector<std::string> copy = original;
    sortAndUnique(original);
    assert(original == copy);

    return 0;
}
