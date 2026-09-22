/*
Write a C++ function that takes a non-empty string `s` and a single character `c`, and returns a `std::string` containing the 0-based indices of the first and last occurrences of `c` in `s`. If `c` occurs exactly once, return only that single index as a string. If `c` does not occur at all, return an empty string. Indices in the output must be separated by a single space when there are two, and must be in the order: first occurrence then last occurrence. The input string may contain any printable characters, and the character `c` is a single character (not a space). The function must be case-sensitive.
*/

#include <string>

// Return a string with the first and last indices of character c in s.
// If c appears once, return that index. If not found, return empty string.
std::string firstAndLast(const std::string& s, char c) {
    int first = -1;
    int last = -1;
    
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == c) {
            if (first == -1) {
                first = static_cast<int>(i);
            }
            last = static_cast<int>(i);
        }
    }
    
    if (first == -1) {
        return "";
    }
    if (first == last) {
        return std::to_string(first);
    }
    return std::to_string(first) + " " + std::to_string(last);
}

#include <cassert>
#include <string>

// The function is declared above; test it here.
int main() {
    assert(firstAndLast("hello", 'l') == "2 3");
    assert(firstAndLast("hello", 'h') == "0");
    assert(firstAndLast("hello", 'o') == "4");
    assert(firstAndLast("hello", 'z') == "");
    assert(firstAndLast("aaaa", 'a') == "0 3");
    assert(firstAndLast("abcabc", 'b') == "1 4");
    assert(firstAndLast("x", 'x') == "0");
    assert(firstAndLast("ab", 'a') == "0");
    assert(firstAndLast("ab", 'b') == "1");
    assert(firstAndLast("", 'a') == "");
    return 0;
}

// The solution involves scanning the string from left to right to locate all positions where the character matches. Instead of storing all positions, we can track the first and last occurrence directly: the first occurrence is the smallest index where the match happens, and the last is the largest. Initialize both to -1. During a single pass, if a match is found and the first is still -1, set both first and last to the current index; otherwise, update last to the current index. After the pass, if first remains -1, return an empty string. If first equals last (meaning only one occurrence), return the string conversion of that single index. Otherwise, return the first index, a space, and the last index. Edge cases include an empty string (though problem states non-empty, but handle gracefully), a character appearing only at the very beginning or end, and repeated character runs. Time complexity is O(n) where n is the length of the string, and auxiliary space is O(1) excluding the returned string.
