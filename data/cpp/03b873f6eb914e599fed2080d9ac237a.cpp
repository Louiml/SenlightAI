// Write a C++ function named `countTokenFrequencies` that reads tokens (whitespace-separated words) from a given input string, counts the occurrences of each unique token, and returns a `std::map<std::string, long>` where the key is the token text and the value is its count. The function should preserve ordering by token text (lexicographical order). The input may contain repeated tokens, punctuation attached to words (e.g., "hello," should be treated as a distinct token from "hello"), and leading/trailing whitespace. Empty input should produce an empty map. The function must accept the input as a `const std::string&` and return the map by value.

#include <cassert>
#include <map>
#include <string>

// Assume the solution function is declared above or included.
#include "countTokenFrequencies.h" // Adjust include if needed

int main() {
    // Basic repeated tokens.
    std::map<std::string, long> result1 = countTokenFrequencies("apple banana apple cherry");
    assert(result1.size() == 3);
    assert(result1["apple"] == 2);
    assert(result1["banana"] == 1);
    assert(result1["cherry"] == 1);

    // Empty input.
    std::map<std::string, long> empty = countTokenFrequencies("");
    assert(empty.empty());

    // Leading/trailing whitespace and multiple spaces.
    std::map<std::string, long> result2 = countTokenFrequencies("   one   two two  one three   ");
    assert(result2.size() == 3);
    assert(result2["one"] == 2);
    assert(result2["two"] == 2);
    assert(result2["three"] == 1);

    // Punctuation attached to words counts as part of the token.
    std::map<std::string, long> result3 = countTokenFrequencies("hi, hi. hi");
    assert(result3.size() == 3);
    assert(result3["hi,"] == 1);
    assert(result3["hi."] == 1);
    assert(result3["hi"] == 1);

    // Single token repeated many times.
    std::map<std::string, long> result4 = countTokenFrequencies("go go go");
    assert(result4.size() == 1);
    assert(result4["go"] == 3);

    // Lexicographic order is guaranteed by map.
    std::map<std::string, long> result5 = countTokenFrequencies("zebra apple mango");
    assert(result5.begin()->first == "apple");
    assert(result5.rbegin()->first == "zebra");

    return 0;
}

#include <map>
#include <sstream>
#include <string>

// Count occurrences of whitespace-separated tokens in the input string.
// Returns a map from token text to its frequency, ordered lexicographically.
std::map<std::string, long> countTokenFrequencies(const std::string& input) {
    std::map<std::string, long> frequencies;
    std::istringstream stream(input);
    std::string token;
    while (stream >> token) {
        ++frequencies[token];
    }
    return frequencies;
}

// The solution uses `std::istringstream` to read whitespace-delimited tokens from the input string. For each token read, we increment the count in a `std::map<std::string, long>` using `operator[]`. The map automatically orders keys lexicographically, and `operator[]` inserts a default value (0) for new keys and returns a reference to the value, which we then increment. Edge cases: empty input results in an empty map (no loop iterations); tokens with punctuation are treated as whole strings (since extraction stops at whitespace, not punctuation, so "hello," is distinct from "hello"); repeated tokens correctly accumulate counts. Time complexity: \(O(n \log m)\) where \(n\) is the total number of tokens and \(m\) is the number of unique tokens (due to map insertion). Space complexity: \(O(m)\) for the map storage, plus temporary string storage per token.
