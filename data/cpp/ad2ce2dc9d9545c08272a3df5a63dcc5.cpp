/*
Write a standalone C++ function named `computeMastermindFeedback` that, given two strings of equal length representing a secret code and a guess in a Mastermind-like game using digits `0-9` and lowercase letters `a-f` (so 16 possible symbols), returns a `std::pair<int, int>` containing the number of exact matches (same symbol in the same position) and the number of misplaced matches (symbol appears in both strings but in different positions). Each symbol in the secret may be counted at most once for either exact or misplaced, and the guess string is assumed to be valid (length ≥ 1, containing only allowed symbols, with no duplicate symbols in either string). The function must be const-correct, use no global variables, and handle cases where the two strings have different lengths by returning `{-1, -1}`.
*/

#include <string>
#include <utility>
#include <vector>

// Returns {exact, misplaced} matches between secret and guess.
// If lengths differ, returns {-1, -1}.
std::pair<int, int> computeMastermindFeedback(const std::string& secret, const std::string& guess) {
    if (secret.length() != guess.length()) {
        return {-1, -1};
    }

    int exact = 0;
    int misplaced = 0;
    int n = static_cast<int>(secret.length());

    std::vector<bool> usedInSecret(n, false);

    // Count exact matches
    for (int i = 0; i < n; ++i) {
        if (secret[i] == guess[i]) {
            ++exact;
            usedInSecret[i] = true;
        }
    }

    // Count misplaced matches
    for (int i = 0; i < n; ++i) {
        if (!usedInSecret[i] && secret[i] != guess[i]) {
            // Secret position i is not used and not exact; check guess[i] against rest of secret
            for (int j = 0; j < n; ++j) {
                if (!usedInSecret[j] && guess[i] == secret[j]) {
                    ++misplaced;
                    usedInSecret[j] = true;
                    break;
                }
            }
        }
    }

    return {exact, misplaced};
}

#include <cassert>
#include <string>
#include <utility>

// Function declaration (assuming defined elsewhere)
std::pair<int, int> computeMastermindFeedback(const std::string& secret, const std::string& guess);

int main() {
    // Basic exact matches
    assert(computeMastermindFeedback("1234", "1234") == std::make_pair(4, 0));
    // No matches
    assert(computeMastermindFeedback("1234", "5678") == std::make_pair(0, 0));
    // All misplaced
    assert(computeMastermindFeedback("1234", "4321") == std::make_pair(0, 4));
    // Mixed exact and misplaced
    assert(computeMastermindFeedback("12ab", "1ba2") == std::make_pair(1, 3));
    // Single character
    assert(computeMastermindFeedback("a", "a") == std::make_pair(1, 0));
    assert(computeMastermindFeedback("a", "b") == std::make_pair(0, 0));
    // Length mismatch
    assert(computeMastermindFeedback("12", "123") == std::make_pair(-1, -1));
    // Hex letters and digits
    assert(computeMastermindFeedback("abcdef", "fedcba") == std::make_pair(0, 6));
    // Reuse of a symbol in secret is prevented (though uniqueness is assumed)
    assert(computeMastermindFeedback("1123", "3412") == std::make_pair(0, 3)); // '1' in secret is used once
    return 0;
}

// The solution iterates through both strings simultaneously to count exact matches (where `secret[i] == guess[i]`). For each exact match, mark that position in the secret as "used" so it won’t be considered again for misplaced counting. Then, for each position in the guess that was not an exact match, search through the secret for a symbol that matches the guess symbol but has not been used yet; if found, increment the misplaced count and mark that secret position as used. This ensures each symbol in the secret is used at most once. Edge cases: if the strings have different lengths, return `{-1, -1}`. If there are duplicate symbols in the input, the function still works correctly because it marks used positions; however, the problem statement guarantees uniqueness, so this is extra robustness. Time complexity is O(n^2) in the worst case due to the nested loops for misplaced counting, where n is the string length; space complexity is O(n) for the visited array (or O(1) if using a bitset of fixed size 16 since the alphabet is fixed).
