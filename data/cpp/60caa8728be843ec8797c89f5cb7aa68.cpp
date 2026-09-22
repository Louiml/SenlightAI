// Write a standalone C++ function named `buildConcordance` that reads whitespace-separated words from a `std::istream` until end-of-file, cleans each word by stripping leading/trailing non-alphanumeric characters (keeping internal punctuation like hyphens), converting the retained portion to lowercase, and then counts how many times each cleaned word appears. The function should return a `std::map<std::string, int>` (or `std::unordered_map` if you prefer) where the key is the cleaned word and the value is its frequency. Words that become empty after cleaning (e.g., a string of only punctuation) must be ignored. The function must handle an empty input stream gracefully and must not modify the input stream’s state beyond reading. The signature should be: `std::map<std::string, int> buildConcordance(std::istream& in);`. Use only the standard library, and ensure the function is self-contained (no global state).

#include <cassert>
#include <sstream>
#include <string>
#include <map>

// The solution function is assumed to be available from the previous section.

int main() {
    // Test 1: Basic words with punctuation and case.
    std::istringstream in1("Hello, world! HELLO World.");
    auto result1 = buildConcordance(in1);
    std::map<std::string, int> expected1 = {{"hello", 2}, {"world", 2}};
    assert(result1 == expected1);

    // Test 2: Empty input.
    std::istringstream in2("");
    auto result2 = buildConcordance(in2);
    assert(result2.empty());

    // Test 3: All punctuation tokens should be ignored.
    std::istringstream in3("!!! ??? ... ,");
    auto result3 = buildConcordance(in3);
    assert(result3.empty());

    // Test 4: Internal hyphen kept, digits counted.
    std::istringstream in4("co-op 123 co-op 123 456");
    auto result4 = buildConcordance(in4);
    std::map<std::string, int> expected4 = {{"co-op", 2}, {"123", 2}, {"456", 1}};
    assert(result4 == expected4);

    // Test 5: Mixed case and leading/trailing punctuation.
    std::istringstream in5("!!Apple!! -banana- (Cherry) @apple");
    auto result5 = buildConcordance(in5);
    std::map<std::string, int> expected5 = {{"apple", 2}, {"banana", 1}, {"cherry", 1}};
    assert(result5 == expected5);

    // Test 6: Token with only one alphanumeric character.
    std::istringstream in6("a ! b !! c ?");
    auto result6 = buildConcordance(in6);
    std::map<std::string, int> expected6 = {{"a", 1}, {"b", 1}, {"c", 1}};
    assert(result6 == expected6);

    // Test 7: Unicode non-ASCII characters (treated as non-alphanumeric by default).
    std::istringstream in7("café café");
    auto result7 = buildConcordance(in7);
    // Since 'é' is not alphanumeric under the default C locale, the word becomes "caf".
    std::map<std::string, int> expected7 = {{"caf", 2}};
    assert(result7 == expected7);

    // Test 8: Newlines and tabs are whitespace.
    std::istringstream in8("one\ttwo\nthree one");
    auto result8 = buildConcordance(in8);
    std::map<std::string, int> expected8 = {{"one", 2}, {"two", 1}, {"three", 1}};
    assert(result8 == expected8);

    // Test 9: Long repeated word.
    std::istringstream in9("test test test");
    auto result9 = buildConcordance(in9);
    std::map<std::string, int> expected9 = {{"test", 3}};
    assert(result9 == expected9);

    // Test 10: Token with only internal punctuation after trimming.
    std::istringstream in10("a-b-c a-b-c");
    auto result10 = buildConcordance(in10);
    std::map<std::string, int> expected10 = {{"a-b-c", 2}};
    assert(result10 == expected10);

    return 0;
}

#include <map>
#include <string>
#include <istream>
#include <algorithm>
#include <cctype>

// Helper predicate to check if a character is alphanumeric.
struct IsAlnum {
    bool operator()(char ch) const {
        return std::isalnum(static_cast<unsigned char>(ch)) != 0;
    }
};

// Helper functor to convert a character to lowercase.
struct ToLowerCase {
    void operator()(char& ch) const {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
};

// Build a concordance from the given input stream.
// Reads whitespace-separated tokens until eof, cleans each token by
// trimming leading/trailing non-alphanumeric characters, converts
// the remaining part to lowercase, and counts occurrences.
std::map<std::string, int> buildConcordance(std::istream& in) {
    std::map<std::string, int> concordance; // word -> count
    std::string word;

    while (in >> word) {
        // Find first alphanumeric character.
        auto first = std::find_if(word.begin(), word.end(), IsAlnum());
        if (first == word.end()) {
            continue; // all punctuation, ignore
        }

        // Find last alphanumeric character (using reverse iterator).
        auto rlast = std::find_if(word.rbegin(), word.rend(), IsAlnum());
        // Convert reverse iterator to forward iterator; rlast.base() points
        // one past the last valid character, so subtract 1.
        auto last = rlast.base() - 1;

        // Convert the valid substring to lowercase in place.
        std::for_each(first, last + 1, ToLowerCase());

        // Build the cleaned word.
        std::string cleaned(first, last + 1);

        // Increment count; if absent, operator[] inserts with 0 then ++.
        concordance[cleaned]++;
    }

    return concordance;
}

// The core algorithm processes each whitespace-separated token from the input stream using the extraction operator `>>`. For each token, we need to determine the substring that consists only of alphanumeric characters, trimming any leading or trailing non-alphanumeric characters. This can be done by finding the first and last positions of an alphanumeric character using `std::find_if` with a predicate that checks `std::isalnum`. If none exists, the token is discarded. After identifying the valid substring, convert all its characters to lowercase using `std::tolower` (casting to `unsigned char` to avoid undefined behavior on negative values). Then increment the count in the map. Edge cases include: tokens that are entirely punctuation (yielding an empty string), tokens with mixed case (converted to lower), tokens with internal hyphens or apostrophes (kept because they are non-alphanumeric and not at the boundaries), and digits (treated as alphanumeric). Time complexity is O(total number of characters across all tokens) because each character is examined a constant number of times (once in `find_if` from the left, once from the right, and once in `for_each` for lowercasing). Space complexity is O(U) where U is the number of unique cleaned words, plus the storage for the input tokens. The function returns the map by value, which is fine for this scale.
