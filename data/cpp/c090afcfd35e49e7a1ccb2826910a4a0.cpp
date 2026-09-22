// Write a C++ function that takes a vector of strings and a character, and returns a vector of integers containing the indices of all strings that contain the given character at least once. The function should accept the vector by const reference and the character by value, and the returned indices should appear in increasing order as they appear in the input vector. The input vector may be empty, and strings may be empty or contain any characters including the target character multiple times. The function must be named `findIndicesContaining` and be declared in the global namespace without any external dependencies beyond the C++ standard library.
#include <cassert>
#include <vector>
#include <string>

// Forward declaration of the solution function (already defined above in actual usage).
std::vector<int> findIndicesContaining(const std::vector<std::string>& words, char x);

int main() {
    // Basic case: multiple matches
    std::vector<std::string> words1 = {"apple", "banana", "cherry"};
    assert(findIndicesContaining(words1, 'a') == std::vector<int>({0, 1}));

    // No matches
    std::vector<std::string> words2 = {"dog", "cat", "bird"};
    assert(findIndicesContaining(words2, 'z') == std::vector<int>());

    // Empty vector
    std::vector<std::string> words3;
    assert(findIndicesContaining(words3, 'a') == std::vector<int>());

    // Empty strings and duplicate characters
    std::vector<std::string> words4 = {"", "a", "aa", "b"};
    assert(findIndicesContaining(words4, 'a') == std::vector<int>({1, 2}));

    // All strings contain the character
    std::vector<std::string> words5 = {"x", "xx", "xxx"};
    assert(findIndicesContaining(words5, 'x') == std::vector<int>({0, 1, 2}));

    // Single character and case sensitivity
    std::vector<std::string> words6 = {"A", "a", "A"};
    assert(findIndicesContaining(words6, 'A') == std::vector<int>({0, 2}));

    // Long strings, last string matches
    std::vector<std::string> words7 = {"hello", "world", "xyz"};
    assert(findIndicesContaining(words7, 'z') == std::vector<int>({2}));

    // Only first string matches
    std::vector<std::string> words8 = {"abc", "def", "ghi"};
    assert(findIndicesContaining(words8, 'a') == std::vector<int>({0}));

    // Character appears in middle string
    std::vector<std::string> words9 = {"", "a", "b", "c"};
    assert(findIndicesContaining(words9, 'b') == std::vector<int>({2}));

    // Multiple same indices not duplicated
    std::vector<std::string> words10 = {"aa", "bb", "aa"};
    assert(findIndicesContaining(words10, 'a') == std::vector<int>({0, 2}));

    return 0;
}
#include <vector>
#include <string>

// Return indices of all strings that contain the given character at least once.
std::vector<int> findIndicesContaining(const std::vector<std::string>& words, char x) {
    std::vector<int> result;
    for (int i = 0; i < static_cast<int>(words.size()); ++i) {
        for (char c : words[i]) {
            if (c == x) {
                result.push_back(i);
                break;
            }
        }
    }
    return result;
}
// The main algorithm is a straightforward linear scan: iterate through each string in the vector using its index, and for each string, iterate through its characters to check if any equals the given character. As soon as a match is found, push the current index into the result vector and break out of the inner character loop to avoid redundant checks. This ensures each string is only checked until its first match, though in the worst case (when the character is absent) every character is examined. Edge cases include an empty input vector (returns an empty result), an empty string (the inner loop simply does nothing, so no index is added), and strings where the target character appears multiple times (only one index is added per string because of the break). Time complexity is O(N * M) in the worst case, where N is the number of strings and M is the maximum length of a string; space complexity is O(1) auxiliary plus O(K) for the result, where K is the number of matching indices.
