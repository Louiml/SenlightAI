// Write a C++ function `bool isAvailable(const std::string& query, const std::vector<std::string>& candidates)` that determines whether a given query string appears in a list of candidate strings after applying a normalization rule. The normalization rule is: convert all lowercase letters to uppercase, replace every `'O'` with `'0'` (digit zero), and replace every `'L'` or `'I'` with `'1'` (digit one). The function should return `false` if the normalized query is present in the normalized candidate list (meaning it should be rejected as unavailable), and `true` otherwise. The input strings may contain only uppercase/lowercase letters and digits. Handle empty strings gracefully—an empty string normalizes to itself, so if an empty query appears in the candidates, return `false`. Assume no punctuation or spaces. Provide the solution as a single free function that performs the normalization on both the query and each candidate, then returns whether the normalized query is **not** in the normalized set.
The core idea is to preprocess every input string by applying the character normalization rule: for each character, if it is lowercase, convert it to uppercase using `std::toupper`; then if the resulting character is `'O'`, change it to `'0'`; and if it is `'L'` or `'I'`, change it to `'1'`. This transformation is deterministic and idempotent. To check availability, normalize the query string first. Then build a `std::set<std::string>` containing the normalized versions of all candidate strings. Finally, check whether the normalized query is a member of that set; if yes, return `false` (available = no), else return `true`. Edge cases: empty strings normalize to empty strings, so an empty query will match an empty candidate; mixed-case letters are handled by uppercasing first, so `'o'` becomes `'0'`, `'l'` and `'i'` become `'1'`; digits remain unchanged. Time complexity is \(O(L \cdot \log N)\) for the set insertions and lookups where \(L\) is the average string length and \(N\) is the number of candidates, and \(O(L)\) per string for normalization. Space complexity is \(O(L \cdot N)\) for storing the normalized candidates in the set.
#include <string>
#include <vector>
#include <set>
#include <cctype>

// Normalize a single character according to the rule.
char normalizeChar(char c) {
    if (std::islower(static_cast<unsigned char>(c))) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    if (c == 'O') {
        c = '0';
    }
    if (c == 'L' || c == 'I') {
        c = '1';
    }
    return c;
}

// Normalize an entire string.
std::string normalizeString(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    for (char c : s) {
        result.push_back(normalizeChar(c));
    }
    return result;
}

// Returns true if the normalized query is NOT present in the normalized candidates.
bool isAvailable(const std::string& query, const std::vector<std::string>& candidates) {
    std::string normalizedQuery = normalizeString(query);
    std::set<std::string> normalizedCandidates;
    for (const std::string& candidate : candidates) {
        normalizedCandidates.insert(normalizeString(candidate));
    }
    return normalizedCandidates.find(normalizedQuery) == normalizedCandidates.end();
}
#include <cassert>
#include <string>
#include <vector>

// (Assume the solution function is declared above.)

int main() {
    // Basic case: exact match after normalization
    assert(isAvailable("hello", {"HELLO", "world"}) == false);
    assert(isAvailable("hello", {"world", "test"}) == true);

    // Case insensitivity: lowercase 'o', 'l', 'i' become digits
    assert(isAvailable("o", {"0"}) == false);
    assert(isAvailable("l", {"1"}) == false);
    assert(isAvailable("i", {"1"}) == false);
    assert(isAvailable("L", {"1"}) == false);
    assert(isAvailable("I", {"1"}) == false);

    // Mixed characters and digits
    assert(isAvailable("OIL", {"011", "012"}) == false);
    assert(isAvailable("OIL", {"010", "012"}) == true);
    assert(isAvailable("code", {"C0DE", "CODE"}) == false);

    // Empty string handling
    assert(isAvailable("", {"", "abc"}) == false);
    assert(isAvailable("", {"abc"}) == true);

    // Multiple candidates, duplicates
    assert(isAvailable("abc", {"ABC", "AbC", "aBc"}) == false);
    assert(isAvailable("abc", {"ABD", "xyz"}) == true);

    // Digits and letters unaffected in other positions
    assert(isAvailable("a1b2", {"A1B2"}) == false);
    assert(isAvailable("a1b2", {"A1B3"}) == true);

    // Test that 'o' becomes '0' but 'q' does not
    assert(isAvailable("q", {"Q", "0"}) == false);
    assert(isAvailable("q", {"0"}) == true);

    return 0;
}
