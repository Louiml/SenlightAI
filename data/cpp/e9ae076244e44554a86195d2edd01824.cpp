/*
Write a standalone C++ function that takes a binary string consisting only of '0' and '1' characters and returns the number of separate groups of consecutive '1's. A group is defined as one or more adjacent '1' characters not separated by any '0'. The function should handle an empty string (returning 0) as well as strings with leading/trailing zeros or all zeros. The input string must not be modified, and the solution should be efficient for strings up to 10^6 characters.
*/

#include <string>

// Count the number of separate groups of consecutive '1's in a binary string.
// A group is one or more adjacent '1' characters not separated by '0'.
// Empty string returns 0.
long long countGroupsOfOnes(const std::string& s) {
    long long groupCount = 0;
    bool inGroup = false;
    
    for (char ch : s) {
        if (ch == '1' && !inGroup) {
            ++groupCount;
            inGroup = true;
        } else if (ch == '0') {
            inGroup = false;
        }
    }
    return groupCount;
}

#include <cassert>
#include <string>

// free function declaration (include the solution above)
long long countGroupsOfOnes(const std::string& s);

int main() {
    assert(countGroupsOfOnes("") == 0);
    assert(countGroupsOfOnes("000") == 0);
    assert(countGroupsOfOnes("111") == 1);
    assert(countGroupsOfOnes("101") == 2);
    assert(countGroupsOfOnes("1100111001") == 3);
    assert(countGroupsOfOnes("0100100010") == 2);
    assert(countGroupsOfOnes("111000111") == 2);
    assert(countGroupsOfOnes("1") == 1);
    assert(countGroupsOfOnes("0") == 0);
    assert(countGroupsOfOnes("1010101") == 4);
    return 0;
}

// The solution iterates through the string once, character by character. A boolean flag `inGroup` tracks whether we are currently inside a group of consecutive '1's. When we encounter a '1' and `inGroup` is false, we increment a counter and set the flag to true to mark the start of a new group. When we encounter a '0', we set the flag to false, indicating the group ended. This way, each group is counted exactly once when its first '1' is seen. Edge cases: an empty string returns 0 (loop simply doesn't run), a string of all zeros returns 0 (flag never set true), and a string of all ones returns 1 (single group). Time complexity is O(n) where n is the length of the string; space complexity is O(1) — only a counter and a boolean are used, no extra data structures.
