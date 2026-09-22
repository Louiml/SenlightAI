Write a C++ function named `countLetterOccurrences` that takes a `std::vector<char>` representing a sequence of characters (each guaranteed to be a lowercase English letter) and a `std::vector<char>` of query characters. The function must return a `std::map<char, int>` where each key is a character from the query vector, and its associated value is the number of times that character appears in the first vector. If a query character appears multiple times in the query vector, include it only once in the result (the first occurrence). The input vectors may be empty, and the result should be an empty map if either vector is empty. The function must not modify the input vectors and should handle up to 10^6 characters efficiently.

The solution uses a frequency map to count occurrences of each character in the first vector by iterating through it once, incrementing `countMap[character]`. Then, we iterate through the query vector, and for each unique query character (tracked using a `std::set` or checking if the key already exists in the result map), we copy its count from `countMap` to the result map. If a query character is not present in the first vector, its count is naturally 0 (since `std::map::operator[]` returns 0 for missing keys). Edge cases include empty input vectors, duplicate query characters (handled by not overwriting and skipping if the key is already in the result), and large inputs where `std::map` is efficient enough (O(log m) per operation). The time complexity is O(n + q log q) for `n` input characters and `q` query characters, where the `log q` arises from inserting into the result map. The space complexity is O(n + q) for the frequency and result maps. The `const` correctness is applied by taking both vectors as `const std::vector<char>&`.

#include <map>
#include <vector>

// Count occurrences of each query character in the given character vector.
// Returns a map where each key is a unique query character and the value
// is its frequency in the input vector.
std::map<char, int> countLetterOccurrences(const std::vector<char>& characters,
                                           const std::vector<char>& queries) {
    // Step 1: Build frequency map for all characters.
    std::map<char, int> frequencyMap;
    for (char c : characters) {
        ++frequencyMap[c];
    }

    // Step 2: Build result map for unique query characters.
    std::map<char, int> result;
    for (char q : queries) {
        // Insert only if not already present (handles duplicate queries).
        if (result.find(q) == result.end()) {
            result[q] = frequencyMap[q];  // operator[] returns 0 if absent.
        }
    }

    return result;
}

#include <cassert>
#include <map>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Basic case with distinct queries.
    std::vector<char> chars1 = {'a', 'b', 'c', 'a', 'b', 'a'};
    std::vector<char> queries1 = {'a', 'c', 'z'};
    std::map<char, int> expected1 = {{'a', 3}, {'c', 1}, {'z', 0}};
    assert(countLetterOccurrences(chars1, queries1) == expected1);

    // Test 2: Duplicate queries – only first occurrence kept.
    std::vector<char> chars2 = {'x', 'y', 'x'};
    std::vector<char> queries2 = {'x', 'x', 'y', 'x'};
    std::map<char, int> expected2 = {{'x', 2}, {'y', 1}};
    assert(countLetterOccurrences(chars2, queries2) == expected2);

    // Test 3: Empty input vector.
    std::vector<char> chars3 = {};
    std::vector<char> queries3 = {'p', 'q'};
    std::map<char, int> expected3 = {{'p', 0}, {'q', 0}};
    assert(countLetterOccurrences(chars3, queries3) == expected3);

    // Test 4: Empty query vector – result empty.
    std::vector<char> chars4 = {'a', 'b'};
    std::vector<char> queries4 = {};
    std::map<char, int> expected4 = {};
    assert(countLetterOccurrences(chars4, queries4) == expected4);

    // Test 5: Single character in both.
    std::vector<char> chars5 = {'m'};
    std::vector<char> queries5 = {'m', 'n'};
    std::map<char, int> expected5 = {{'m', 1}, {'n', 0}};
    assert(countLetterOccurrences(chars5, queries5) == expected5);

    // Test 6: All same characters and query includes all alphabet letters.
    std::vector<char> chars6 = {'a', 'a', 'a'};
    std::vector<char> queries6 = {'a', 'b', 'c'};
    std::map<char, int> expected6 = {{'a', 3}, {'b', 0}, {'c', 0}};
    assert(countLetterOccurrences(chars6, queries6) == expected6);

    return 0;
}
