/*
Write a C++ function named `calculateOXScores` that takes a vector of strings, where each string represents a quiz result containing only the characters 'O' and 'X' (uppercase). For each string, compute the score using the rule: consecutive 'O' characters award an increasing score starting at 1 (i.e., the first 'O' gives 1 point, the second consecutive 'O' gives 2 points, and so on). An 'X' resets the streak to 0. The function should return a vector of integers, where each integer is the total score for the corresponding input string. Assume the input vector is non-empty and each string has length at least 1. The function should not modify the input vector.
*/
#include <vector>
#include <string>

// Given a vector of 'O'/'X' strings, compute the cumulative score for each string.
// Consecutive 'O's award 1, 2, 3, ... points; 'X' resets the streak.
std::vector<int> calculateOXScores(const std::vector<std::string>& results) {
    std::vector<int> scores;
    scores.reserve(results.size());

    for (const std::string& result : results) {
        int streak = 0;
        int total = 0;
        for (char ch : result) {
            if (ch == 'O') {
                ++streak;
                total += streak;
            } else { // ch == 'X'
                streak = 0;
            }
        }
        scores.push_back(total);
    }
    return scores;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration (defined above)
std::vector<int> calculateOXScores(const std::vector<std::string>& results);

int main() {
    // Single string cases
    assert(calculateOXScores({"O"}) == std::vector<int>{1});
    assert(calculateOXScores({"X"}) == std::vector<int>{0});
    assert(calculateOXScores({"OOXOO"}) == std::vector<int>{6});
    assert(calculateOXScores({"XXX"}) == std::vector<int>{0});

    // Multiple strings
    assert(calculateOXScores({"OO", "OX", "XO", "XX"}) == std::vector<int>{3, 1, 1, 0});
    assert(calculateOXScores({"OOO", "OOXOOO", "XOXOX"}) == std::vector<int>{6, 9, 3});

    // Mixed and longer strings
    assert(calculateOXScores({"OOOO"}) == std::vector<int>{10});
    assert(calculateOXScores({"OXOXO"}) == std::vector<int>{3});
    assert(calculateOXScores({"O"}) != std::vector<int>{0});
    assert(calculateOXScores({"O", "X", "OO", "XX", "OOO"}) == std::vector<int>{1, 0, 3, 0, 6});

    return 0;
}
// The solution processes each string independently. For a given string, iterate through its characters from left to right, maintaining a running `streak` counter. When the character is 'O', increment `streak` by 1 and add the new `streak` value to the total score. When the character is 'X', reset `streak` to 0. The total score for a string is the sum of all increments. Edge cases: an all-'X' string yields a score of 0; a single 'O' yields 1; a string like "OOXOO" yields 1+2+0+1+2 = 6. The algorithm runs in O(total length of all strings) time because each character is processed once, and uses O(1) auxiliary space per string (plus O(n) for the output vector, where n is the number of strings). The input is taken by const reference to guarantee no modification, and the function returns a vector of ints.
