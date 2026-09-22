/*
Write a C++ function `bool canGuessWord(const std::string& secret, const std::vector<char>& guesses)` that returns `true` if, given the secret word and a list of already-guessed letters, every letter in `secret` appears at least once in `guesses`. The function should be case-insensitive (treat uppercase and lowercase letters as equivalent), and the `guesses` vector may contain duplicates, non‑letter characters, and characters that are not in the secret word. The secret is assumed to be non‑empty and contain only alphabetic characters. The comparison must ignore case for both the secret letters and the guessed letters.
*/
#include <string>
#include <vector>
#include <array>
#include <cctype>

// Returns true if every letter in 'secret' appears (case-insensitively) in 'guesses'.
bool canGuessWord(const std::string& secret, const std::vector<char>& guesses) {
    // Track which letters (A-Z) have been guessed.
    std::array<bool, 26> guessed{};
    for (char c : guesses) {
        if (std::isalpha(c)) {
            guessed[std::toupper(c) - 'A'] = true;
        }
    }
    // Check each letter of the secret.
    for (char c : secret) {
        if (!guessed[std::toupper(c) - 'A']) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>
#include <string>

// (canGuessWord is defined above—include here in actual test)

int main() {
    // Basic case
    assert(canGuessWord("HELLO", {'H','E','L','O'}) == true);
    // Missing a letter
    assert(canGuessWord("WORLD", {'W','O','R','L'}) == false);
    // Case insensitivity
    assert(canGuessWord("Abc", {'a','B','c'}) == true);
    assert(canGuessWord("abc", {'A','B','C'}) == true);
    // Duplicates and non-letters ignored
    assert(canGuessWord("TEST", {'T','T','E','S','1','!'}) == true);
    // Empty guesses
    assert(canGuessWord("A", {}) == false);
    // Single letter secret
    assert(canGuessWord("Z", {'z'}) == true);
    // Secret with multiple same letters
    assert(canGuessWord("BANANA", {'B','A','N'}) == true);
    return 0;
}
// The core algorithm repeatedly scans the secret word for each guessed letter, but only needs a boolean result. A direct approach: normalize each guessed character to uppercase (or lowercase), record it in a `std::set<char>`, then iterate through the secret string and check whether every letter is present in that set. For a secret of length \(L\) and a guesses vector of size \(G\), building the set takes \(O(G \log G)\) time (or \(O(G)\) if using a boolean array for A–Z), and the final check takes \(O(L)\) time. Total time \(O(G + L)\), space \(O(1)\) if using a fixed 26‑element boolean array, or \(O(G)\) worst‑case for a `set`. Edge cases: duplicate guesses (set handles), non‑alphabetic characters (ignore or store; they won’t match secret letters), case differences (normalize both sides), and an empty guesses vector (should return `false` unless the secret is empty, but secret is non‑empty). The solution uses a tailored `std::array<bool, 26>` for constant‑time checks.
