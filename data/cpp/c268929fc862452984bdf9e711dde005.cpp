// Write a C++ function that takes a non-empty string `myStr` containing only lowercase English letters and splits it into substrings by removing every occurrence of the characters `'a'`, `'b'`, and `'c'`, treating those characters as delimiters. The function must return a `std::vector<std::string>` containing the remaining substrings in their original relative order, but only those that are non-empty. If the resulting vector is empty (because the entire input consists solely of `'a'`, `'b'`, or `'c'` characters), return a vector containing a single string `"EMPTY"`. Examples: `"baconlettucetomato"` → `{"onlettu", "etomato"}`; `"abcd"` → `{"d"}`; `"abc"` → `{"EMPTY"}`; `"aabbcc"` → `{"EMPTY"}`; `"cabab"` → `{"EMPTY"}` because after removing delimiters only spaces remain; also handle inputs where delimiters appear consecutively, at the beginning, or at the end.
#include <cassert>
#include <string>
#include <vector>

// (The solution function from above is assumed to be included here.)

int main() {
    // Basic cases
    assert(splitByDelimiters("baconlettucetomato") == std::vector<std::string>({"onlettu", "etomato"}));
    assert(splitByDelimiters("abcd") == std::vector<std::string>({"d"}));
    assert(splitByDelimiters("abc") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("aabbcc") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("cabab") == std::vector<std::string>({"EMPTY"}));

    // Consecutive delimiters and delimiters at edges
    assert(splitByDelimiters("a") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("ab") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("x") == std::vector<std::string>({"x"}));
    assert(splitByDelimiters("axb") == std::vector<std::string>({"x"}));
    assert(splitByDelimiters("xa") == std::vector<std::string>({"x"}));
    assert(splitByDelimiters("ax") == std::vector<std::string>({"x"}));
    assert(splitByDelimiters("aab") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("abcxyz") == std::vector<std::string>({"xyz"}));
    assert(splitByDelimiters("xyzbac") == std::vector<std::string>({"xyz"}));
    assert(splitByDelimiters("xaybzc") == std::vector<std::string>({"x", "y", "z"}));

    // Longer strings with mixed content
    assert(splitByDelimiters("helloaworldb") == std::vector<std::string>({"hello", "world"}));
    assert(splitByDelimiters("abcabcabc") == std::vector<std::string>({"EMPTY"}));
    assert(splitByDelimiters("abcdef") == std::vector<std::string>({"def"}));
    assert(splitByDelimiters("z") == std::vector<std::string>({"z"}));
    assert(splitByDelimiters("zazbzc") == std::vector<std::string>({"z", "z", "z"}));

    return 0;
}
#include <string>
#include <vector>

// Splits a string by removing 'a', 'b', and 'c' characters and returns the
// non-empty remaining segments. Returns {"EMPTY"} if no segments remain.
std::vector<std::string> splitByDelimiters(const std::string& myStr) {
    std::vector<std::string> result;
    std::string currentToken;

    for (char ch : myStr) {
        if (ch == 'a' || ch == 'b' || ch == 'c') {
            if (!currentToken.empty()) {
                result.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken.push_back(ch);
        }
    }

    // Add the last token if the string does not end with a delimiter.
    if (!currentToken.empty()) {
        result.push_back(currentToken);
    }

    if (result.empty()) {
        result.push_back("EMPTY");
    }

    return result;
}
// The algorithm scans the input string from left to right, identifying contiguous runs of characters that are not `'a'`, `'b'`, or `'c'`. For each such run, extract the substring and add it to the result vector. A simple two-pointer approach works: maintain a start index `s`. While `s` is less than the length, find the end index `e` by advancing while the character at `e` is not a delimiter (i.e., not `'a'`, `'b'`, or `'c'`). If `e > s` (meaning a non-empty block exists), push `myStr.substr(s, e - s)`. Then set `s = e + 1` to skip the delimiter at position `e` or to advance past the end if `e == length`. However, careful handling is needed: the inner loop must not access `myStr[e]` when `e` equals the length, so check bounds. A cleaner implementation uses a single loop building the current token and resetting it whenever a delimiter is encountered. Edge cases include: input all delimiters (result empty → return `{"EMPTY"}`), delimiters at the start or end (ignore leading/trailing empty tokens), and consecutive delimiters (no token between them). Also ensure that if the last character is not a delimiter, the final accumulated token is added after the loop. Time complexity is O(n) where n is the length of the input, because each character is examined once. Space complexity is O(n) for the output vector (in the worst case when no delimiters exist, the vector holds the entire string).
