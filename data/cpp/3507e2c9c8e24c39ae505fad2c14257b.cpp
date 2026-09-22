Write a C++ function named `isEmailMatch` that takes two strings: a reference string and an input string. The function should return `true` if the input string, after removing all leading and trailing whitespace characters (spaces, tabs, newlines, carriage returns, form feeds, and vertical tabs), exactly matches the reference string (case-sensitive), and `false` otherwise. The whitespace trimming must only affect the beginning and end of the input string, not any internal whitespace. If the trimmed input becomes empty, the function should return `false` unless the reference string is also empty (in which case it returns `true`). The function must handle strings with only whitespace, strings with no whitespace, and strings with internal spaces that should be preserved.
The solution uses a manual trimming approach to remove all leading and trailing characters that are considered whitespace. We define a helper lambda (or inline logic) that checks whether a character belongs to the set `" \t\n\r\f\v"`. First, find the index of the first character that is not whitespace using a loop from the beginning. If no such character exists, the trimmed string is empty, and we compare the reference string to an empty string. Otherwise, find the index of the last non-whitespace character by scanning from the end. Then extract the substring between these indices and compare it directly to the reference using `==`. This avoids creating a temporary trimmed copy that could be expensive, but for simplicity we can also create a substring. The main edge cases are: all-whitespace input, empty reference string, internal whitespace preservation, and input longer than the reference. Time complexity is O(n) where n is the length of the input string, and space complexity is O(n) if we create a substring copy, or O(1) if we compare character-by-character without copying. For clarity and correctness, we create a substring copy. The function must be `const`-correct, taking both strings as `const std::string&`.
#include <string>

// Returns true if the input string, after stripping leading and trailing whitespace,
// exactly matches the reference string.
bool isEmailMatch(const std::string& reference, const std::string& input) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = 0;
    while (start < input.size() && whitespace.find(input[start]) != std::string::npos) {
        ++start;
    }
    
    // If the whole input is whitespace, then trimmed string is empty.
    if (start == input.size()) {
        return reference.empty();
    }
    
    size_t end = input.size() - 1;
    while (end > start && whitespace.find(input[end]) != std::string::npos) {
        --end;
    }
    
    // Compare the trimmed substring to the reference.
    return input.compare(start, end - start + 1, reference) == 0;
}
#include <cassert>
#include <string>

// Function declaration (as per solution) — assumed to be in same file.
bool isEmailMatch(const std::string& reference, const std::string& input);

int main() {
    // Normal match
    assert(isEmailMatch("user@example.com", "  user@example.com  ") == true);
    // Case-sensitive
    assert(isEmailMatch("User@Example.com", "user@example.com") == false);
    // Internal whitespace preserved
    assert(isEmailMatch("a b c", "   a b c   ") == true);
    assert(isEmailMatch("a b c", "a b  c") == false);
    // All whitespace input, non-empty reference
    assert(isEmailMatch("x", "   \t\n   ") == false);
    // All whitespace input, empty reference
    assert(isEmailMatch("", "   \t\n   ") == true);
    // Empty input and empty reference
    assert(isEmailMatch("", "") == true);
    // Input with newlines, tabs
    assert(isEmailMatch("hello", "\r\n hello \v\f") == true);
    // No trimming needed
    assert(isEmailMatch("exact", "exact") == true);
    // Input shorter than reference
    assert(isEmailMatch("longer", "long") == false);
    return 0;
}
