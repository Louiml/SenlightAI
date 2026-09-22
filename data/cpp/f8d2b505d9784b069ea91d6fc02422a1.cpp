// Write a C++ function named `FindFirstRepeatedLetter` that takes a single non-empty string as input (which may contain uppercase letters, lowercase letters, digits, punctuation, and spaces) and returns a `char` representing the first letter (case-insensitive) that appears at least twice in the string, converting the result to uppercase. If no letter repeats, return `'\0'` (the null character). The search should consider letters only (ignore all non-alphabetic characters), and the first occurrence of a letter (in the original string order) that has a duplicate anywhere later in the string determines the result. For example, for `"Hello World"`, the first repeated letter is `'L'` (since `'l'` appears at index 2 and again at index 9, and no earlier letter repeats before that), so the function returns `'L'`. For `"ab cd ef"` with no repeat, it returns `'\0'`.
#include <cassert>
#include <string>

int main() {
    // Basic repeated letter
    assert(FindFirstRepeatedLetter("Hello World") == 'L');
    // Case-insensitive
    assert(FindFirstRepeatedLetter("aA") == 'A');
    assert(FindFirstRepeatedLetter("bBaA") == 'B');
    // No letters
    assert(FindFirstRepeatedLetter("123 !@#") == '\0');
    // Single letter
    assert(FindFirstRepeatedLetter("x") == '\0');
    // Repeats after punctuation and digits
    assert(FindFirstRepeatedLetter("a1 b2 c3 a4") == 'A');
    // First repeat is not necessarily the first letter
    assert(FindFirstRepeatedLetter("xy zx") == 'X');
    // All unique letters
    assert(FindFirstRepeatedLetter("abcdef") == '\0');
    // Empty string (though spec says non-empty, just ensure it doesn't crash)
    assert(FindFirstRepeatedLetter("") == '\0');
}
#include <string>
#include <array>
#include <cctype>

// Returns the first repeated letter (case-insensitive) in the input string,
// converted to uppercase. Returns '\0' if no letter repeats.
char FindFirstRepeatedLetter(const std::string& input) {
    std::array<bool, 26> seen = {};
    for (char c : input) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            int index = upper - 'A';
            if (seen[index]) {
                return upper;
            }
            seen[index] = true;
        }
    }
    return '\0';
}
// The solution requires scanning the input string from left to right while tracking which letters have been seen so far. Since the function is case‑insensitive, we normalize each character to its uppercase form (or lowercase) before checking. A common approach is to use a `std::array<bool, 26>` (or a `std::unordered_set<char>`) to record whether a letter has been encountered. For each character in the string, if it is alphabetic (using `std::isalpha` or manual comparison), we convert it to uppercase (e.g., by subtracting `'a' - 'A'` if lowercase). If that letter is already marked as seen, then it is the first repeated letter (because we process in order) — we return its uppercase form immediately. Otherwise, we mark it as seen and continue. If the loop finishes without finding a repeat, we return `'\0'`. Edge cases include: strings with no letters, strings with only one letter, repeated letters with different cases (e.g., `"aA"` should return `'A'`), and non‑alphabetic characters interspersed that should be ignored. The time complexity is O(n) where n is the length of the input, and space complexity is O(1) because we only use a fixed‑size array (or a small set).
