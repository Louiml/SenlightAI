// Write a C++ function that takes a vector of strings as input and returns a vector of strings containing only the unique elements from the input, preserving the order in which they first appear. The input may contain duplicate strings, empty strings, and strings with spaces. The function must handle an empty input vector by returning an empty vector. The returned vector should not contain any duplicates, and the relative order of the first occurrence of each string must be maintained.

#include <cassert>
#include <vector>
#include <string>

// Function declaration from the solution
std::vector<std::string> uniqueInOrder(const std::vector<std::string>& input);

int main() {
    // Basic duplicates
    assert(uniqueInOrder({"aman", "aman", "sdnf", "sdnf"}) == std::vector<std::string>({"aman", "sdnf"}));
    // Empty input
    assert(uniqueInOrder({}) == std::vector<std::string>({}));
    // All duplicates
    assert(uniqueInOrder({"x", "x", "x"}) == std::vector<std::string>({"x"}));
    // Mixed with empty strings
    assert(uniqueInOrder({"", "a", "", "b", "a"}) == std::vector<std::string>({"", "a", "b"}));
    // Strings with spaces
    assert(uniqueInOrder({"hello world", "hello", "hello world", "world"}) == std::vector<std::string>({"hello world", "hello", "world"}));
    // No duplicates
    assert(uniqueInOrder({"one", "two", "three"}) == std::vector<std::string>({"one", "two", "three"}));
    // Long sequence with repeated items not adjacent
    assert(uniqueInOrder({"a", "b", "a", "c", "b", "d"}) == std::vector<std::string>({"a", "b", "c", "d"}));
    // Case sensitive
    assert(uniqueInOrder({"A", "a", "A"}) == std::vector<std::string>({"A", "a"}));
    // Single element
    assert(uniqueInOrder({"only"}) == std::vector<std::string>({"only"}));
    // Duplicates with spaces and empty string
    auto test = uniqueInOrder({" ", " ", "a", " ", "a"});
    assert(test == std::vector<std::string>({" ", "a"}));

    return 0;
}

#include <vector>
#include <string>
#include <set>

// Return a vector containing only the first occurrence of each string in the input.
std::vector<std::string> uniqueInOrder(const std::vector<std::string>& input) {
    std::set<std::string> seen;
    std::vector<std::string> result;
    for (const auto& s : input) {
        if (seen.find(s) == seen.end()) {
            seen.insert(s);
            result.push_back(s);
        }
    }
    return result;
}

// The solution uses a `std::set` to track which strings have already been seen, combined with a vector to store the unique results in order. Iterate through each string in the input vector. For each string, check if it already exists in the set. If not, insert it into the set and append it to the result vector. This guarantees that only the first occurrence is kept, and the order of first appearances is preserved. Edge cases include an empty input (returns empty), all duplicates (returns one element), and empty strings (an empty string is treated like any other string). Time complexity is \(O(n \log m)\) where \(n\) is the total number of strings and \(m\) is the number of unique strings, due to set operations. Space complexity is \(O(m)\) for the set and the result vector, excluding the input.
