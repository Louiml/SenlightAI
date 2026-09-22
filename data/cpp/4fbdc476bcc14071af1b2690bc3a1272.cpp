/*
Write a C++ function named `isInCodeforces` that takes a single character as input and returns a boolean value indicating whether that character appears in the string `"codeforces."` (including the period). The function should be case-sensitive, meaning that `'C'` should return `false` while `'c'` should return `true`. You do not need to handle any input/output operations — just implement the logic of checking membership. The function must work correctly for any printable ASCII character, including punctuation and digits.
*/

#include <string>

// Return true if the given character appears in "codeforces." (case-sensitive).
bool isInCodeforces(char c) {
    const std::string target = "codeforces.";
    for (char ch : target) {
        if (ch == c) {
            return true;
        }
    }
    return false;
}

#include <cassert>

int main() {
    assert(isInCodeforces('c') == true);
    assert(isInCodeforces('o') == true);
    assert(isInCodeforces('.') == true);
    assert(isInCodeforces('C') == false);
    assert(isInCodeforces('z') == false);
    assert(isInCodeforces('1') == false);
    assert(isInCodeforces(' ') == false);
    assert(isInCodeforces('@') == false);
    assert(isInCodeforces('f') == true);
    assert(isInCodeforces('e') == true);
    return 0;
}

// The solution is straightforward: define a constant string containing the exact target characters `"codeforces."`. To check membership, iterate through each character of the string and compare it with the given input character. If a match is found, return `true` immediately; if the loop completes without a match, return `false`. Edge cases include: the period character `'.'` is part of the string and should return `true`; uppercase letters are not present, so they return `false`; and characters such as digits, spaces, or other symbols return `false` as well since they are not in the string. The time complexity is \(O(1)\) because the string length is fixed (11 characters), and the space complexity is \(O(1)\) as only a constant-size string is used.
