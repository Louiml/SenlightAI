/*
Write a C++ function named `tallyUniqueWords` that takes a single string parameter containing lowercase letters, digits, and single spaces, and returns an `int` representing the number of distinct words in the string. A word is any maximal sequence of non-space characters (i.e., letters and digits). The input may contain leading, trailing, or consecutive spaces, and the function should ignore all whitespace when determining words. Words are case‑sensitive, so `"Cat"` and `"cat"` would be different, but since the input only contains lowercase letters, case differences will not occur. Duplicate words should be counted only once. The function must work for an empty string (returning `0`) and for strings with only spaces (also returning `0`). For example, for input `"the quick brown fox jumps over the lazy dog"`, the function returns `8` (all words are distinct except `"the"` appears twice). The function should be efficient and use `const` correctly.
*/

#include <string>
#include <unordered_set>

// Returns the number of distinct words in a whitespace-separated string.
// Words are sequences of non-space characters (lowercase letters/digits).
int tallyUniqueWords(const std::string& text) {
    std::unordered_set<std::string> distinctWords;
    std::string currentWord;
    
    for (char ch : text) {
        if (ch == ' ') {
            if (!currentWord.empty()) {
                distinctWords.insert(currentWord);
                currentWord.clear();
            }
        } else {
            currentWord += ch;
        }
    }
    
    // Handle the last word if the string does not end with a space.
    if (!currentWord.empty()) {
        distinctWords.insert(currentWord);
    }
    
    return static_cast<int>(distinctWords.size());
}

#include <cassert>
#include <string>

int main() {
    // Basic distinct words.
    assert(tallyUniqueWords("the quick brown fox jumps over the lazy dog") == 8);
    // Duplicates counted once.
    assert(tallyUniqueWords("one two two three three three") == 3);
    // Empty string.
    assert(tallyUniqueWords("") == 0);
    // Only spaces.
    assert(tallyUniqueWords("     ") == 0);
    // Leading/trailing/consecutive spaces.
    assert(tallyUniqueWords("  hello   world  hello  ") == 2);
    // Single word.
    assert(tallyUniqueWords("single") == 1);
    // Digits are allowed.
    assert(tallyUniqueWords("abc123 abc123 xyz") == 2);
    // Mixed letters and digits.
    assert(tallyUniqueWords("a1 b2 a1 c3") == 3);
    // No spaces, multiple identical.
    assert(tallyUniqueWords("aaaa") == 1);
    // Many spaces between words.
    assert(tallyUniqueWords("alpha   beta    alpha") == 2);
    return 0;
}

// The solution uses a `std::unordered_set<std::string>` to store each distinct word encountered. We read the input string character by character, identifying word boundaries by spaces. When a non‑space character is found, we accumulate it into a temporary string; when we reach a space or the end of the string and the temporary string is non‑empty, we insert it into the set. After processing the entire string, the size of the set gives the number of distinct words. Edge cases: empty input and input with only spaces are handled naturally because the temporary string remains empty and the set stays empty — returning 0. Consecutive spaces are ignored because we only flush the temporary word when we encounter a space (and the word is non‑empty), so multiple spaces just skip the flush. The algorithm runs in O(n) time where n is the length of the string (each character is processed once; each insertion into the unordered_set is O(1) average, but worst‑case O(word length) due to hashing). Space complexity is O(d * m) where d is the number of distinct words and m is the average word length, since we store each distinct word in the set plus the temporary buffer.
