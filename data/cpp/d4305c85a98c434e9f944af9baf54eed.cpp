/*
Write a C++ function `std::string reverseName(const char input[])` that takes a C-style string (null-terminated character array), reverses the characters in place (i.e., modifies the original array), and returns the reversed string as a `std::string`. The input is guaranteed to contain only lowercase or uppercase alphabetic characters and no spaces, and it will have at most 20 characters (excluding the null terminator). The function should handle empty strings (length 0) gracefully, and must not use any standard library reverse functions.
*/

#include <string>

// Reverses the input C-string in place and returns the reversed result as a std::string.
std::string reverseName(char input[]) {
    int length = 0;
    while (input[length] != '\0') {
        ++length;
    }

    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = input[start];
        input[start] = input[end];
        input[end] = temp;
        ++start;
        --end;
    }

    return std::string(input);
}

#include <cassert>
#include <cstring>

std::string reverseName(char input[]); // declaration from solution

int main() {
    // Test basic reversal
    char name1[21] = "hello";
    assert(reverseName(name1) == "olleh");
    assert(strcmp(name1, "olleh") == 0); // in-place modification

    // Test single character
    char name2[21] = "a";
    assert(reverseName(name2) == "a");
    assert(strcmp(name2, "a") == 0);

    // Test empty string (length 0)
    char name3[21] = "";
    assert(reverseName(name3) == "");
    assert(strcmp(name3, "") == 0);

    // Test longer string with mixed case
    char name4[21] = "AbCdEf";
    assert(reverseName(name4) == "fEdCbA");
    assert(strcmp(name4, "fEdCbA") == 0);

    // Test palindrome (reverse should be same)
    char name5[21] = "racecar";
    assert(reverseName(name5) == "racecar");
    assert(strcmp(name5, "racecar") == 0);

    // Test maximum length (20 chars)
    char name6[21] = "abcdefghijklmnopqrst"; // 20 chars
    assert(reverseName(name6) == "tsrqponmlkjihgfedcba");
    assert(strcmp(name6, "tsrqponmlkjihgfedcba") == 0);

    // Test string with repeated characters
    char name7[21] = "aaabbbbccc";
    assert(reverseName(name7) == "cccbbbbaaa");
    assert(strcmp(name7, "cccbbbbaaa") == 0);

    // Test even-length string
    char name8[21] = "abcd";
    assert(reverseName(name8) == "dcba");
    assert(strcmp(name8, "dcba") == 0);

    return 0;
}

// The solution follows the classic two-pointer reversal technique. First, compute the length of the input C-string by iterating until the null terminator `'\0'` is found (this is `O(n)`). Then, set a `start` pointer at index 0 and an `end` pointer at `length - 1`. While `start < end`, swap the characters at these positions and move `start` forward and `end` backward. This swaps characters in pairs until the middle is reached, producing a reversed string in `O(n)` time with `O(1)` auxiliary space. Edge cases: an empty string (length 0) requires no swaps; a single-character string also requires no swaps. Since the input is guaranteed alphabetic with no spaces, no whitespace or symbol handling is needed. The function modifies the original array in place (as per the requirement) and then returns a `std::string` built from the modified array. The time complexity is `O(n)` because each character is visited once for length counting and once for swapping. The space complexity is `O(1)` for the two pointers and temporary swap variable, plus `O(n)` for the returned string (but that is required output).
