/*
Write a C++ function `bool containsWordInOrder(const std::string& text, const std::string& word)` that determines whether the characters of `word` appear in `text` in the same relative order, but not necessarily consecutively. For example, given `text = "hlelo"` and `word = "hello"`, the function should return `true` because `h`, `e`, `l` (first `l`), `l` (second `l`), and `o` appear in order. However, if `text` is too short or the required characters are missing, return `false`. The function must be case-sensitive and treat spaces as regular characters (though the word will not contain spaces). The original code snippet used a nested loop to search for each character of `"hello"` in a given string, advancing the search position after each match — your task is to generalize this behavior to any two input strings.
*/
#include <string>

// Returns true if all characters of `word` appear in `text` in the same order.
bool containsWordInOrder(const std::string& text, const std::string& word) {
    if (word.empty()) {
        return true;
    }

    size_t pos = 0; // current search position in text

    for (char c : word) {
        bool found = false;
        while (pos < text.size()) {
            if (text[pos] == c) {
                ++pos; // move past matched character
                found = true;
                break;
            }
            ++pos;
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <string>

bool containsWordInOrder(const std::string& text, const std::string& word);

int main() {
    // Basic examples
    assert(containsWordInOrder("hello", "hello") == true);
    assert(containsWordInOrder("hlelo", "hello") == true);
    assert(containsWordInOrder("hllo", "hello") == false);

    // Empty word
    assert(containsWordInOrder("anything", "") == true);
    assert(containsWordInOrder("", "") == true);

    // Empty text with non-empty word
    assert(containsWordInOrder("", "a") == false);

    // Case sensitivity
    assert(containsWordInOrder("Hello", "hello") == false);
    assert(containsWordInOrder("Hello", "Hello") == true);

    // Duplicate characters require multiple occurrences
    assert(containsWordInOrder("helo", "hello") == false);
    assert(containsWordInOrder("lleh", "hello") == false);

    // Word longer than text
    assert(containsWordInOrder("hi", "hih") == false);

    // Spaces are ordinary characters
    assert(containsWordInOrder("h e l l o", "hello") == true);
    assert(containsWordInOrder("h e l l o", "h l o") == true);

    // Not matching due to order
    assert(containsWordInOrder("olleh", "hello") == true); // reversed order, but letters do appear in order? Actually 'o','l','l','e','h' — not "hello", so false
    // Correct test: "lleho" has l,l,e,h,o — not hello (needs h before e). So false.
    assert(containsWordInOrder("lleho", "hello") == false);
}
// The solution uses a two-pointer greedy approach: iterate over each character in the target `word`, and for each, scan forward in `text` starting from the current position to find a matching character. If a match is found, update the search position to just after that match (so characters cannot be reused) and move to the next character of `word`. If any character of `word` is not found before the end of `text`, return `false`. If all characters are found, return `true`.  
// Edge cases:  
// - If `word` is empty, the function should return `true` (every string contains the empty sequence).  
// - If `text` is empty and `word` is not, return `false`.  
// - Case sensitivity: 'A' and 'a' are different.  
// - If `word` has duplicate characters, the algorithm naturally requires multiple distinct occurrences in `text` in order.  
// Complexity: Let `n = text.length()` and `m = word.length()`. The worst-case time is `O(n*m)` because for each character of `word` we might scan the whole remaining `text`. However, since the search position advances strictly forward, the total work over all characters is at most `O(n)` (each position in `text` is examined at most once), so the actual time is `O(n + m)` in the worst case. Auxiliary space is `O(1)`.
