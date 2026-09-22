Write a C++ function `std::vector<std::string> generateGeneralizedAbbreviations(const std::string& word)` that returns all unique generalized abbreviations of the input word. A generalized abbreviation replaces any number of non-overlapping, non-adjacent substrings with their lengths (as decimal numbers), while keeping the remaining characters in their original order. For example, for the word `"BAT"`, the output must include `"BAT"`, `"BA1"`, `"B1T"`, `"B2"`, `"1AT"`, `"1A1"`, `"2T"`, and `"3"`. Two abbreviations are considered distinct if their strings differ. The function must handle empty strings, single-character strings, repeated characters (e.g., `"AAA"`), and produce results in any order. The input will consist only of uppercase English letters (A-Z). Ensure the output contains no duplicates and each abbreviation is constructed exactly as described.

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Test empty string: only the empty abbreviation.
    std::vector<std::string> r0 = generateGeneralizedAbbreviations("");
    assert(r0.size() == 1 && r0[0] == "");

    // Single character: "A" and "1".
    std::vector<std::string> r1 = generateGeneralizedAbbreviations("A");
    std::sort(r1.begin(), r1.end());
    std::vector<std::string> expected1 = {"1", "A"};
    assert(r1 == expected1);

    // Two-character word: all valid abbreviations.
    std::vector<std::string> r2 = generateGeneralizedAbbreviations("AB");
    std::sort(r2.begin(), r2.end());
    std::vector<std::string> expected2 = {"1B", "2", "A1", "AB"};
    assert(r2 == expected2);

    // Example from problem statement: "BAT".
    std::vector<std::string> r3 = generateGeneralizedAbbreviations("BAT");
    std::sort(r3.begin(), r3.end());
    std::vector<std::string> expected3 = {"1AT", "1A1", "2T", "3", "B1T", "B2", "BA1", "BAT"};
    assert(r3 == expected3);

    // Repeated characters: "AAA" must produce unique abbreviations without duplicates.
    std::vector<std::string> r4 = generateGeneralizedAbbreviations("AAA");
    std::sort(r4.begin(), r4.end());
    std::vector<std::string> expected4 = {"1AA", "1A1", "2A", "3", "A1A", "A2", "AA1", "AAA"};
    assert(r4 == expected4);

    // "code" from the original comment: check count and no duplicates.
    std::vector<std::string> r5 = generateGeneralizedAbbreviations("code");
    assert(r5.size() == 8); // Fibonacci-like count due to non-adjacent rule? Actually check known result.
    // Verify uniqueness.
    std::sort(r5.begin(), r5.end());
    assert(std::adjacent_find(r5.begin(), r5.end()) == r5.end());

    // Verify each abbreviation is valid by expanding (custom check omitted for brevity).
    // Basic sanity: "code" includes "1ode", "c1de", "co1e", "cod1", "2de", "c2e", "co2", "3e", etc.
    // At least check known ones manually.
    assert(std::find(r5.begin(), r5.end(), "1ode") != r5.end());
    assert(std::find(r5.begin(), r5.end(), "c2e") != r5.end());
    assert(std::find(r5.begin(), r5.end(), "4") != r5.end());
    assert(std::find(r5.begin(), r5.end(), "code") != r5.end());
}

#include <string>
#include <vector>
#include <to_string>

// Recursive helper to build abbreviations.
void generateHelper(const std::string& word, int index, std::string current,
                    bool prevNum, std::vector<std::string>& result) {
    if (index == word.length()) {
        result.push_back(current);
        return;
    }

    // Option 1: keep the current character.
    generateHelper(word, index + 1, current + word[index], false, result);

    // Option 2: replace a substring with its length, only if the previous
    // abbreviation part was a character (not a number) to avoid adjacency.
    if (!prevNum) {
        for (int len = 1; index + len <= word.length(); ++len) {
            std::string num = std::to_string(len);
            generateHelper(word, index + len, current + num, true, result);
        }
    }
}

// Returns all unique generalized abbreviations of the input word.
std::vector<std::string> generateGeneralizedAbbreviations(const std::string& word) {
    std::vector<std::string> result;
    generateHelper(word, 0, "", false, result);
    return result;
}

// The solution uses recursive backtracking to explore all valid abbreviations. At each position `index` in the word, we have two choices: either output the current character and move to the next index, or replace a substring starting at `index` with its length (a number) and jump past that substring. To enforce the "non-adjacent" rule, we track a boolean `prevNum` that indicates whether the immediately previous character in the abbreviation was a number (i.e., a compressed substring). If `prevNum` is true, we cannot start a new number at the current position because that would create adjacent numbers from adjacent substrings. Thus, when `prevNum` is false, we try all possible lengths `len` from 1 up to `word.length() - index`, append `to_string(len)`, and recurse at `index + len` with `prevNum = true`. When we output a character, we set `prevNum = false`. The base case occurs when `index == word.length()`, where we add the accumulated abbreviation string to the result. This approach naturally avoids duplicates because each valid abbreviation corresponds to exactly one sequence of choices. The time complexity is \(O(n \cdot 2^n)\) because the recursion forms a binary tree with up to \(2^n\) leaves, and each leaf string can be up to length \(n\) (copying and appending takes linear time). The space complexity is \(O(n \cdot 2^n)\) for the result storage plus \(O(n)\) recursion stack depth, but excluding the output, it's \(O(n)\).
