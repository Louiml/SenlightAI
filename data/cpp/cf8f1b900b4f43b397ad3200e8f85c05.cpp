/*
Write a C++ function `insertDigit` that takes three parameters: an integer `n` (the length of the string), an integer `d` (a digit between 0 and 9), and a string `input` (which represents a non-negative integer without leading zeros, except possibly the string "0"). The function must insert the digit `d` into the string at the leftmost position such that the resulting string, when interpreted as an integer, is as large as possible. In other words, find the first character in `input` (from left to right) whose numeric value is less than `d`; if such a character exists, insert `d` immediately before it; otherwise, append `d` at the end. Return the new string. The input string length `n` will match `input.size()`, and `n` will be at least 1. The digit `d` is guaranteed to be a single digit (0-9). Your function should be efficient and handle edge cases like `input = "5"`, `d = 5`, `d = 0`, or all digits greater than `d`.
*/

#include <string>

// Insert digit d into the string s to maximize the resulting integer value.
// The first character from left that is strictly less than d gets changed position.
// If none exists, append d at the end.
std::string maximizeNumber(int n, int d, const std::string& s) {
    // n is given but we can use s.size() for safety.
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (d > (s[i] - '0')) {
            // Found the first digit less than d, insert before it.
            std::string result = s;
            result.insert(i, 1, static_cast<char>('0' + d));
            return result;
        }
    }
    // All digits are >= d, append at the end.
    return s + static_cast<char>('0' + d);
}

#include <cassert>
#include <string>

// Declare the function (typically in a header, but here for completeness)
std::string maximizeNumber(int n, int d, const std::string& s);

int main() {
    // Basic case: insert 5 into "1234" -> first digit less than 5 is '1' (at index 0)
    assert(maximizeNumber(4, 5, "1234") == "51234");
    // Insert 3 into "9876" -> no digit less than 3, append at end
    assert(maximizeNumber(4, 3, "9876") == "98763");
    // Insert 4 into "4321" -> first digit less than 4 is '3' (index 2)
    assert(maximizeNumber(4, 4, "4321") == "43421");
    // Insert 0 into "123" -> no digit less than 0, append
    assert(maximizeNumber(3, 0, "123") == "1230");
    // Single digit string: "5", d=5 -> no digit strictly less, append
    assert(maximizeNumber(1, 5, "5") == "55");
    // Single digit string: "5", d=7 -> '5' < '7', insert before
    assert(maximizeNumber(1, 7, "5") == "75");
    // Equal digits: "2222", d=2 -> no digit strictly less, append
    assert(maximizeNumber(4, 2, "2222") == "22222");
    // d is larger than all: "999", d=9 -> no digit strictly less, append
    assert(maximizeNumber(3, 9, "999") == "9999");
    // Leading zeros not allowed in input, but test with "0" and d=1
    assert(maximizeNumber(1, 1, "0") == "10");
    // Longer example: "12345", d=3 -> first digit less than 3 is '1' (index 0)
    assert(maximizeNumber(5, 3, "12345") == "312345");
    return 0;
}

// The main algorithm is straightforward: iterate through the characters of the string from left to right, comparing each character's integer value (obtained by subtracting `'0'`) with the digit `d`. The first position where the digit value is strictly less than `d` is the optimal insertion point because placing `d` there yields a number with a higher digit in that position than any later insertion, and any earlier position would have a digit ≥ `d`, so placing `d` there would not increase the value. If no such position exists (i.e., all digits are ≥ `d`), then inserting at the end yields the largest result. Edge cases include: `d` being smaller than all digits (append at end), `d` equal to some digits (skip equal digits, continue until a smaller one is found), and when the string is of length 1. The time complexity is O(n) where n is the string length, and space complexity is O(n) for the resulting string (but only O(1) auxiliary space since we use `string::insert` which may allocate internally). The function should be `const`-correct by taking the input string as `const std::string&` and returning a new `std::string`.
