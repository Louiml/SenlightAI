// Write a C++ function that takes a non-empty string as input and returns a new string where all characters of the original string are reversed, preserving any leading or trailing whitespace (i.e., whitespaces should stay at the same positions as in the original string, but the non-whitespace characters in between are reversed). For example, an input of `"  hello world  "` should produce `"  dlrow olleh  "`. The function should handle Unicode characters? For simplicity, assume ASCII input only. The function must be const-correct and should not modify the input string. Edge cases include strings with only whitespace, strings with a single character, and strings with multiple spaces between words.

The main algorithm is straightforward: copy the input string to a local variable, then use the standard `std::reverse` algorithm on the entire string (including whitespace). However, this would reverse whitespace too, which violates the requirement to keep whitespace at the same positions. Instead, we need to reverse only the non-whitespace characters in place. Approach: use a two-pointer technique where we traverse from both ends, and whenever both pointers point to non-whitespace characters, swap them. Move left pointer rightwards if it points to whitespace, move right pointer leftwards if it points to whitespace. Continue until pointers cross. This yields O(n) time and O(1) extra space (excluding the copy of the string which is O(n)). Edge cases: empty string (though non-empty given), all spaces (no swaps), single character (no change), leading/trailing spaces are untouched because the pointers skip them. Complexity: O(n) time, O(n) auxiliary space for the returned string (since we return by value).

#include <string>
#include <cctype>

// Reverse only the non-whitespace characters in the input string,
// preserving the positions of whitespace (spaces, tabs, etc.).
std::string reverseNonWhitespace(const std::string& s) {
    std::string result = s;
    size_t left = 0;
    size_t right = result.size() - 1;

    while (left < right) {
        // Move left pointer to next non-whitespace
        while (left < right && std::isspace(static_cast<unsigned char>(result[left]))) {
            ++left;
        }
        // Move right pointer to previous non-whitespace
        while (left < right && std::isspace(static_cast<unsigned char>(result[right]))) {
            --right;
        }
        // Swap the non-whitespace characters
        if (left < right) {
            std::swap(result[left], result[right]);
            ++left;
            --right;
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Function declaration (prototype) for testing
std::string reverseNonWhitespace(const std::string& s);

int main() {
    assert(reverseNonWhitespace("hello") == "olleh");
    assert(reverseNonWhitespace("  hello world  ") == "  dlrow olleh  ");
    assert(reverseNonWhitespace("a") == "a");
    assert(reverseNonWhitespace("   ") == "   ");
    assert(reverseNonWhitespace("abc def ghi") == "ihg fed cba");
    assert(reverseNonWhitespace("  multiple   spaces  here ") == "  eroh   secaps  elpitlum ");
    assert(reverseNonWhitespace("x y z") == "z y x");
    assert(reverseNonWhitespace(" ab") == " ba");
    assert(reverseNonWhitespace("a b") == "b a");
    assert(reverseNonWhitespace("t a b") == "b a t");
    return 0;
}
