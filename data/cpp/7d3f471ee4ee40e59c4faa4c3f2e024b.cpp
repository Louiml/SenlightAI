/*
Write a C++ function `findPalindromicSquares` that takes an integer base `b` (where 2 ≤ b ≤ 20) and returns a `std::vector<std::pair<std::string, std::string>>` containing all numbers `i` from 1 to 300 inclusive for which the square `i*i`, when expressed in base `b`, is a palindrome. For each such `i`, the vector must contain a pair where the first string is `i` converted to base `b` and the second string is `i*i` converted to base `b`. Digits above 9 must be represented by uppercase letters starting at `A` for 10, `B` for 11, etc. The pairs must be in increasing order of `i`. The function must be self-contained, not rely on global state, and handle all valid bases correctly.
*/
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// Converts a non-negative integer to a string in the given base (2-20).
// Digits above 9 are represented by uppercase letters A-J.
std::string toBase(unsigned int num, int base) {
    if (num == 0) return "0";
    std::string result;
    while (num > 0) {
        int rem = num % base;
        if (rem < 10) {
            result.push_back(static_cast<char>('0' + rem));
        } else {
            result.push_back(static_cast<char>('A' + rem - 10));
        }
        num /= base;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// Checks if a string is a palindrome.
bool isPalindrome(const std::string& s) {
    for (std::size_t left = 0, right = s.size() - 1; left < right; ++left, --right) {
        if (s[left] != s[right]) {
            return false;
        }
    }
    return true;
}

// Returns all pairs (i, i*i) in base b where i*i is a palindrome, for i from 1 to 300.
std::vector<std::pair<std::string, std::string>> findPalindromicSquares(int base) {
    std::vector<std::pair<std::string, std::string>> result;
    for (int i = 1; i <= 300; ++i) {
        int square = i * i;
        std::string squareBase = toBase(static_cast<unsigned int>(square), base);
        if (isPalindrome(squareBase)) {
            std::string iBase = toBase(static_cast<unsigned int>(i), base);
            result.emplace_back(iBase, squareBase);
        }
    }
    return result;
}
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

// declaration from solution
std::vector<std::pair<std::string, std::string>> findPalindromicSquares(int base);

int main() {
    // Base 10 example: i=1 (1*1=1, palindrome "1"), i=2 (4), i=3 (9),
    // i=11 (121), i=22 (484), i=26 (676), i=101? No, up to 300 only.
    auto res10 = findPalindromicSquares(10);
    assert(res10[0].first == "1" && res10[0].second == "1");
    assert(res10[1].first == "2" && res10[1].second == "4");
    assert(res10[2].first == "3" && res10[2].second == "9");
    // Check that 11 squared is 121 (palindrome)
    bool found121 = false;
    for (const auto& p : res10) {
        if (p.first == "11" && p.second == "121") found121 = true;
    }
    assert(found121);
    // Base 2: test i=1 (1), i=3 (1001), i=5 (11001? actually 25 decimal = 11001 binary, palindrome? 11001 reversed 10011, not), i=7 (49 = 110001, reversed 100011 not), i=9 (81 = 1010001, reversed 1000101 not) — but i=3 square=9 decimal=1001 binary palindrome.
    auto res2 = findPalindromicSquares(2);
    bool found3 = false;
    for (const auto& p : res2) {
        if (p.first == "11" && p.second == "1001") found3 = true;
    }
    assert(found3);
    // Base 20: i=1, square=1, i=2 square=4, i=3 square=9, i=10 square=100 decimal = "50" in base 20 (5*20+0), not palindrome. i=17 square=289 = 14*20+9 = "E9" not palindrome.
    auto res20 = findPalindromicSquares(20);
    assert(res20[0].first == "1" && res20[0].second == "1");
    // Check that no invalid characters appear (only digits and A-J)
    for (const auto& p : res20) {
        for (char c : p.second) {
            assert((c >= '0' && c <= '9') || (c >= 'A' && c <= 'J'));
        }
    }
    // Base 16: i=17 (decimal 17 = "11" hex), square=289 decimal = "121" hex, palindrome.
    auto res16 = findPalindromicSquares(16);
    bool found17 = false;
    for (const auto& p : res16) {
        if (p.first == "11" && p.second == "121") found17 = true;
    }
    assert(found17);
    // Test that base 10 contains no duplicates and is sorted.
    for (std::size_t idx = 1; idx < res10.size(); ++idx) {
        assert(res10[idx].first > res10[idx-1].first);
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The core algorithm is straightforward: iterate `i` from 1 to 300, compute `i*i`, convert `i` and `i*i` to the given base `b` using a conversion function, and check if the string representation of the square is a palindrome (reads the same forward and backward). The conversion function repeatedly divides the number by `b`, collecting remainders, and maps remainders 0-9 to characters '0'-'9' and 10-19 to 'A'-'J' (since max base is 20). The remainders are collected in reverse order and then reversed to produce the correct base representation. Edge cases include the number 0 (though not needed here since i starts at 1), and bases up to 20 which require letter digits. The palindrome check compares characters from both ends toward the middle. Time complexity is O(300 * log_b(300^2)) ≈ O(300 * log_b(90000)) which is effectively constant; space complexity is O(k) where k is the number of qualifying numbers, at most 300 pairs.
