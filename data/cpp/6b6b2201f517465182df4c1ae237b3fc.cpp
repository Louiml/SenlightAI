Write a C++ function `bool isSubsetWord(const std::vector<char>& availableLetters, const std::string& candidateWord)` that determines whether a candidate word can be formed entirely from a multiset of available letters. Each letter in `availableLetters` can be used at most once, and the comparison must be case-insensitive (i.e., 'A' and 'a' are considered the same letter). The function should return `true` if every character in `candidateWord` appears in `availableLetters` with sufficient frequency (considering case-insensitive matching), and `false` otherwise. The input vector may contain uppercase and lowercase letters, and the candidate word may be empty or contain non-alphabetic characters (which should cause a return of `false`). The function must be `const`-correct and must not modify its inputs.

// The solution uses a frequency counting approach. First, we build a map (e.g., `std::map<char, int>` or `std::unordered_map`) that counts the occurrences of each lowercase version of the letters in `availableLetters`. For each character in `candidateWord`, we check if it is alphabetic; if not, return `false` immediately. If alphabetic, we convert it to lowercase (using `std::tolower` with casting to `unsigned char` to avoid undefined behavior for negative `char` values). We then check if the frequency of that lowercase character in the map is greater than zero; if not, return `false`. If yes, decrement the frequency (simulating using one occurrence). If we successfully process all characters in the candidate word, return `true`. Edge cases include an empty candidate word (always returns `true` because no letters are needed), duplicate letters in both inputs, and mixed case (handled by lowercasing both sides). Time complexity is O(n + m) where n is the size of `availableLetters` and m is the length of `candidateWord`, since each character is processed once and map operations are O(1) on average (or O(log k) for `std::map`, with k up to 26 distinct letters). Space complexity is O(1) in practice because the map can hold at most 26 entries (letters a–z).

#include <vector>
#include <string>
#include <cctype>
#include <map>

// Check if candidateWord can be formed from the multiset of availableLetters,
// ignoring case. Returns true if every alphabetic character in candidateWord
// has a corresponding available letter (case-insensitive), and false otherwise.
// Non-alphabetic characters in candidateWord cause an immediate false return.
bool isSubsetWord(const std::vector<char>& availableLetters,
                  const std::string& candidateWord) {
    // Count occurrences of each lowercase letter in availableLetters.
    std::map<char, int> freq;
    for (char c : availableLetters) {
        // Cast to unsigned char to safely pass to std::tolower.
        char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (std::isalpha(static_cast<unsigned char>(lower))) {
            ++freq[lower];
        }
    }

    // Process each character in the candidate word.
    for (char c : candidateWord) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalpha(uc)) {
            return false; // Non-alphabetic character disallowed.
        }
        char lower = static_cast<char>(std::tolower(uc));
        auto it = freq.find(lower);
        if (it == freq.end() || it->second == 0) {
            return false; // Letter not available or already used up.
        }
        --(it->second); // Use one occurrence.
    }

    return true;
}

#include <cassert>

int main() {
    // Basic happy path.
    std::vector<char> letters = {'a', 'B', 'c', 'd', 'e'};
    assert(isSubsetWord(letters, "abc") == true);
    assert(isSubsetWord(letters, "cab") == true);
    assert(isSubsetWord(letters, "abdc") == true);

    // Case-insensitive matching.
    assert(isSubsetWord(letters, "ABC") == true);
    assert(isSubsetWord(letters, "aBc") == true);
    assert(isSubsetWord(letters, "A") == true);

    // Insufficient letters (duplicate usage).
    assert(isSubsetWord(letters, "aa") == false);
    assert(isSubsetWord(letters, "bb") == false);
    assert(isSubsetWord(letters, "cc") == false);

    // Missing letters.
    assert(isSubsetWord(letters, "z") == false);
    assert(isSubsetWord(letters, "abcx") == false);

    // Empty candidate word is always true.
    assert(isSubsetWord(letters, "") == true);

    // Non-alphabetic characters should cause failure.
    assert(isSubsetWord(letters, "a1") == false);
    assert(isSubsetWord(letters, "-") == false);

    // Empty available letters.
    std::vector<char> emptyLetters;
    assert(isSubsetWord(emptyLetters, "") == true);
    assert(isSubsetWord(emptyLetters, "a") == false);

    // Duplicates in available letters allow repeated use.
    std::vector<char> duplicates = {'a', 'a', 'b'};
    assert(isSubsetWord(duplicates, "aa") == true);
    assert(isSubsetWord(duplicates, "aaa") == false);
    assert(isSubsetWord(duplicates, "ab") == true);

    // Mixed case in available letters.
    std::vector<char> mixed = {'A', 'b', 'C'};
    assert(isSubsetWord(mixed, "abc") == true);
    assert(isSubsetWord(mixed, "ABC") == true);
    assert(isSubsetWord(mixed, "AB") == true);
    assert(isSubsetWord(mixed, "ABc") == true);
    assert(isSubsetWord(mixed, "ABCC") == false);
}
