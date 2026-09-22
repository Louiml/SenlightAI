Write a C++ function named `spellCheck` that takes two vectors of strings: a wordlist of correctly spelled words and a list of query strings. For each query, return the correct spelling from the wordlist according to the following matching rules, or an empty string if no match exists. The matching rules are: (1) If the query exactly matches a word in the wordlist (case-sensitive), return that exact word. (2) If the query matches a word in the wordlist when both are converted to lowercase (case-insensitive match), return the first word from the wordlist that satisfies this case-insensitive match. (3) If the query matches a word in the wordlist when counting only letters (ignoring vowel case variations, i.e., treat 'a','e','i','o','u' as equivalent regardless of case), and not already handled by rule 2, return the first word from the wordlist satisfying this vowel-error match. (4) If no match is found, return an empty string. The wordlist is assumed to contain unique words, but the queries may contain any strings including non-letter characters and empty strings (empty queries should return empty string). The function should return a vector of strings containing the answer for each query in order.

#include <cassert>
#include <vector>
#include <string>

// Assuming the spellCheck function is defined above in the same translation unit.

int main() {
    std::vector<std::string> wordlist = {"KiTe", "kite", "hare", "Hare"};
    std::vector<std::string> queries = {"kite", "Kite", "KiTe", "Hare", "HARE", "Hear", "hear", "keti", "keet", "keto"};
    std::vector<std::string> expected = {"kite", "KiTe", "KiTe", "Hare", "hare", "", "", "KiTe", "", "KiTe"};
    assert(spellCheck(wordlist, queries) == expected);

    // Additional edge cases
    assert(spellCheck({}, {"a"}) == std::vector<std::string>{""});
    assert(spellCheck({"Apple"}, {"apple", "Apple", "APPLE", "applE", "aPPlE", "x"}) == 
           std::vector<std::string>{"Apple", "Apple", "Apple", "Apple", "Apple", ""});
    assert(spellCheck({"ab"}, {"", "a", "b", "ab"}) == std::vector<std::string>{"", "", "", "ab"});
    assert(spellCheck({"BoB"}, {"bob", "BAB", "bub", "bEb"}) == std::vector<std::string>{"BoB", "BoB", "BoB", "BoB"});
    assert(spellCheck({"run", "bun"}, {"ran","rUn","bAn"}) == std::vector<std::string>{"run", "run", "bun"});
    return 0;
}

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>

// Returns the correctly spelled word for each query based on three matching rules.
std::vector<std::string> spellCheck(const std::vector<std::string>& wordlist, const std::vector<std::string>& queries) {
    // Preprocess wordlist
    std::unordered_set<std::string> exact_set;
    std::unordered_map<std::string, std::string> lower_map;   // lowercase -> original word (first occurrence)
    std::unordered_map<std::string, std::string> vowel_map;   // lowercase with vowels replaced -> original word

    for (const auto& word : wordlist) {
        exact_set.insert(word);

        std::string lower = word;
        for (auto& ch : lower) ch = std::tolower(static_cast<unsigned char>(ch));
        if (lower_map.find(lower) == lower_map.end()) {
            lower_map[lower] = word;
        }

        std::string vowel_key = lower;
        for (auto& ch : vowel_key) {
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                ch = '`'; // placeholder
            }
        }
        if (vowel_map.find(vowel_key) == vowel_map.end()) {
            vowel_map[vowel_key] = word;
        }
    }

    std::vector<std::string> result;
    result.reserve(queries.size());

    for (const auto& query : queries) {
        // Rule 1: exact match
        if (exact_set.count(query)) {
            result.push_back(query);
            continue;
        }

        // Lowercase query
        std::string lower_q = query;
        for (auto& ch : lower_q) ch = std::tolower(static_cast<unsigned char>(ch));

        // Rule 2: case-insensitive match
        auto it_lower = lower_map.find(lower_q);
        if (it_lower != lower_map.end()) {
            result.push_back(it_lower->second);
            continue;
        }

        // Rule 3: vowel-error match
        std::string vowel_key_q = lower_q;
        for (auto& ch : vowel_key_q) {
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                ch = '`';
            }
        }
        auto it_vowel = vowel_map.find(vowel_key_q);
        if (it_vowel != vowel_map.end()) {
            result.push_back(it_vowel->second);
            continue;
        }

        // no match
        result.push_back("");
    }
    return result;
}

// The problem requires three levels of matching with specificity: exact match, case-insensitive match, and vowel-error match. We can preprocess the wordlist into three maps:
// - `exact`: a set (or bool map) of original words for O(1) exact lookup.
// - `lower`: a map from lowercase version of a word to the first original word that lowercases to that (preserving original casing).
// - `vowel`: a map from a "vowel-key" (where all vowels are replaced by a placeholder like '`') to the first original word with that key.
//
// For each query, first check exact match. If not, compute the lowercase version and check the lower map. If not, compute the vowel-key (by lowercasing then replacing any character in "aeiou" with '`') and check the vowel map. Using `unordered_map` gives average O(1) lookups, but since the wordlist size is small, we can also just use linear search, but for clarity and efficiency we’ll use maps. Time complexity: preprocessing O(N * L) where N is wordlist size and L is average word length; each query O(L). Space: O(N * L). Edge cases: empty query string – it will not match any word in the maps, so return empty string automatically. Also note that vowel-error matching should only apply if the lowercase version did not match (rule 2 has higher priority). Also, a word with no vowels still has a vowel-key; we must ensure that if the lowercase version differs only by vowels, the vowel-key matches. Use a placeholder character that is not a letter or digit, e.g., '`'.
