// Write a C++ function named `splitByLengthParity` that accepts a `const std::vector<std::string>&` input list and returns a `std::pair<std::set<std::string>, std::set<std::string>>` where the first set contains all strings from the input whose length is even, and the second set contains all strings whose length is odd. The sets must be automatically sorted in ascending lexicographic order. The function should handle empty input vectors gracefully (returning two empty sets) and should treat strings of length zero as even-length (since 0 is even). Preserve the original strings exactly, including any spaces or special characters, without modifying or trimming them. The function must be `const`-correct (i.e., it should not modify the input vector) and must not use any global variables. Example: for input `{"helo","salom","qalesan","tort"}`, the first set (even lengths) should be `{"helo","qalesan","tort"}` (lengths 4,7,4 — wait, "qalesan" has 7 chars, which is odd, so correct sets are: even set: `{"helo","tort"}`; odd set: `{"qalesan","salom"}` since "salom" has 5 chars). Ensure the function is self-contained with only necessary headers (`<vector>`, `<set>`, `<string>`, `<utility>`).

#include <cassert>
#include <vector>
#include <set>
#include <string>
#include <utility>

// function declaration (assumed to be included from solution)
std::pair<std::set<std::string>, std::set<std::string>> splitByLengthParity(const std::vector<std::string>& input);

int main() {
    // Test 1: Example from the snippet.
    std::vector<std::string> test1 = {"helo", "salom", "qalesan", "tort"};
    auto result1 = splitByLengthParity(test1);
    std::set<std::string> expectedEven1 = {"helo", "tort"};
    std::set<std::string> expectedOdd1 = {"qalesan", "salom"};
    assert(result1.first == expectedEven1);
    assert(result1.second == expectedOdd1);

    // Test 2: Empty input vector.
    std::vector<std::string> test2 = {};
    auto result2 = splitByLengthParity(test2);
    assert(result2.first.empty());
    assert(result2.second.empty());

    // Test 3: All strings have even length.
    std::vector<std::string> test3 = {"abcd", "xy", "123456"};
    auto result3 = splitByLengthParity(test3);
    assert(result3.first == std::set<std::string>({"abcd", "xy", "123456"}));
    assert(result3.second.empty());

    // Test 4: All strings have odd length.
    std::vector<std::string> test4 = {"a", "cat", "elephant"};
    auto result4 = splitByLengthParity(test4);
    assert(result4.first.empty());
    assert(result4.second == std::set<std::string>({"a", "cat", "elephant"}));

    // Test 5: Duplicate strings (both even and odd duplicates).
    std::vector<std::string> test5 = {"hello", "hello", "hi", "hi", "world"};
    auto result5 = splitByLengthParity(test5);
    assert(result5.first == std::set<std::string>({"hi"})); // "hi" length 2 even
    assert(result5.second == std::set<std::string>({"hello", "world"})); // length 5 and 5

    // Test 6: String with spaces and special characters.
    std::vector<std::string> test6 = {"a b", "c", "  "};
    auto result6 = splitByLengthParity(test6);
    assert(result6.first == std::set<std::string>({"  ", "a b"})); // lengths 2 and 3?  "a b" length 3 => odd, "  " length 2 => even
    // Correction: "a b" has length 3 (odd) -> goes to odd set; "  " length 2 (even) -> even set. Let's fix assertion.
    assert(result6.first == std::set<std::string>({"  "}));
    assert(result6.second == std::set<std::string>({"a b", "c"})); // "c" length 1 odd

    // Test 7: Empty string inside vector.
    std::vector<std::string> test7 = {"", "abc", ""};
    auto result7 = splitByLengthParity(test7);
    assert(result7.first == std::set<std::string>({""})); // length 0 even
    assert(result7.second == std::set<std::string>({"abc"})); // length 3 odd

    // Test 8: Sorted order check (already guaranteed by set, but verify content).
    std::vector<std::string> test8 = {"z", "a", "bb"};
    auto result8 = splitByLengthParity(test8);
    assert(result8.first == std::set<std::string>({"bb"})); // length 2 even
    assert(result8.second == std::set<std::string>({"a", "z"})); // sorted lexicographically

    return 0;
}

#include <vector>
#include <set>
#include <string>
#include <utility>

// Splits a vector of strings into two sets based on the parity of string lengths.
// Returns a pair: first set = even-length strings, second set = odd-length strings.
std::pair<std::set<std::string>, std::set<std::string>> splitByLengthParity(const std::vector<std::string>& input) {
    std::set<std::string> evenSet;
    std::set<std::string> oddSet;

    for (const auto& str : input) {
        if (str.length() % 2 == 0) {
            evenSet.insert(str);
        } else {
            oddSet.insert(str);
        }
    }

    return {evenSet, oddSet};
}

// The solution iterates over every string in the input vector. For each string, it checks the parity of its length using the modulo operator (`s.length() % 2 == 0`). If even, the string is inserted into the first set; if odd, into the second set. Using `std::set` automatically discards duplicate strings and maintains sorted order based on the default less-than comparator for strings. Edge cases: empty input vector — the loops do not execute, both sets remain empty. A string of length zero (only possible if the vector contains an empty string) is considered even because 0 % 2 == 0, so it goes into the even set. Duplicate strings are stored only once per set due to set semantics. The algorithm runs in O(m * n log n) where m is the total number of characters across all strings (to compute length, which is O(1) per string in most implementations, but string length is O(1) in C++11 and later) and n is the number of strings; each insertion into a set is O(log n). So overall time complexity is O(n log n) for n strings, and space complexity is O(n) for the sets (worst-case if all strings are unique).
