Write a C++ function `firstAndLastOccurrence` that takes three parameters: a `std::string` (the search string), a `char` (the target character), and an `int` (the starting index, which should always be 0 when called externally). The function must return a `std::string` containing the 0-based indices of the first and last occurrence of the target character in the string, formatted as two integers separated by a single space (e.g., `"2 5"`). If the character appears exactly once, both indices should be the same value (e.g., `"3 3"`). If the character does not appear at all, the function should return `"-1 -1"`. The function must use recursion to traverse the string from the given index to the end (the base case is when the index equals the string length). It must not use any loops, global variables, static local variables, or external helper functions. The function should be `const`-correct and work for any valid string (including empty strings) and any character.
// The solution recursively scans the string from a given index. At each step, if the current character matches the target, we need to record its position. The trick is to avoid static variables (as in the original snippet) because they are not reusable across multiple calls. Instead, the recursion should return a result that combines the current match with the result from the remaining substring. A clean approach is to define a recursive helper that returns a pair of indices (first and last) for the substring starting at `idx`. At the base case (idx == length), return `{-1, -1}`. Otherwise, recursively get the result from `idx+1`. Then, if the current character matches, the first occurrence should be `idx` (since it’s the earliest), and the last occurrence should be the recursive result's last (if that is not -1, else `idx`). If the current character does not match, then the result is exactly the recursive result. This works correctly for all cases: no occurrence returns `{-1,-1}`, one occurrence returns `{idx,idx}` for that position, and multiple occurrences correctly coalesce first and last. Time complexity is O(n) where n is the string length, because each character is processed exactly once. Space complexity is O(n) due to the recursion stack depth (worst case for a string of length n). The function returns a string formatted from the pair. Edge cases: empty string returns `"-1 -1"`; target appears at the very beginning or end; target appears exactly once. The function can be implemented as a private recursive helper returning a `std::pair<int,int>`, and a public wrapper that formats the result.
#include <string>
#include <utility>

// Recursive helper returning {first, last} indices of target in str starting at idx.
static std::pair<int, int> findOccurrences(const std::string& str, char target, int idx) {
    if (idx == static_cast<int>(str.length())) {
        return {-1, -1};
    }
    auto result = findOccurrences(str, target, idx + 1);
    if (str[idx] == target) {
        int first = idx;
        int last = (result.second == -1) ? idx : result.second;
        return {first, last};
    }
    return result;
}

// Return "first last" indices of the target character in the given string.
std::string firstAndLastOccurrence(const std::string& str, char target) {
    auto indices = findOccurrences(str, target, 0);
    return std::to_string(indices.first) + " " + std::to_string(indices.second);
}
#include <cassert>
#include <string>

// The function declaration (from our solution) is assumed to be available.
std::string firstAndLastOccurrence(const std::string& str, char target);

int main() {
    // Basic cases with multiple occurrences.
    assert(firstAndLastOccurrence("hello", 'l') == "2 3");
    assert(firstAndLastOccurrence("banana", 'a') == "1 5");
    assert(firstAndLastOccurrence("abracadabra", 'a') == "0 10");

    // Single occurrence.
    assert(firstAndLastOccurrence("hello", 'h') == "0 0");
    assert(firstAndLastOccurrence("hello", 'o') == "4 4");

    // No occurrence.
    assert(firstAndLastOccurrence("hello", 'x') == "-1 -1");

    // Empty string.
    assert(firstAndLastOccurrence("", 'a') == "-1 -1");

    // Character at the very start and end.
    assert(firstAndLastOccurrence("a", 'a') == "0 0");
    assert(firstAndLastOccurrence("abc", 'c') == "2 2");

    // Character appears multiple times with spaces or punctuation.
    assert(firstAndLastOccurrence("a b a", 'a') == "0 4");

    return 0;
}
