// Given a string `s` containing only lowercase English letters and the character `'?'`, along with an array `cost` of 26 non-negative integers (one per letter), replace every `'?'` with a lowercase letter such that the total adjacent difference cost is minimized. The adjacent difference cost is the sum over all adjacent pairs `(s[i], s[i+1])` of `abs(cost[s[i]-'a'] - cost[s[i+1]-'a'])`. Process the string from left to right, and when replacing each `'?'`, choose the letter that minimizes the immediate local cost considering only the already-fixed left neighbor and the original right neighbor (if any), breaking ties by choosing the lexicographically smallest letter. After all replacements, return a pair consisting of the total cost after full replacement and the final string. Note that the right neighbor might itself be a `'?'` (not yet replaced), and in that case its cost contribution is ignored during the local decision (since it will be determined later). The final total cost is computed after all `?` are replaced.
The main idea is a greedy left-to-right replacement: when encountering `'?'`, the future choices depend only on the left and right neighbors that are already known (original letters). For each possible letter `c` (from `'a'` to `'z'`), compute the cost contribution to the adjacent pairs that are currently fixed: if `i>0` and `s[i-1]` is not `'?'`, add `abs(cost[prev]-cost[c])`; if `i<n-1` and `s[i+1]` is not `'?'`, add `abs(cost[c]-cost[next])`. If both neighbors are `'?'`, the local cost is 0 for all candidates, so we pick `'a'` due to lexicographic tie-breaking. Choose the candidate with the smallest local cost; if there is a tie, pick the lexicographically smallest character. Then replace the `'?'` with that chosen character and continue. After processing all characters, compute the final total cost by summing `abs(cost[s[i]-'a'] - cost[s[i+1]-'a'])` for all `i` from 0 to `n-2`. Edge cases include the first and last characters (only one neighbor), a string consisting solely of `'?'` (every replacement has local cost 0, so all become `'a'`), and equal costs among characters which leads to lexicographic tie-breaking. The algorithm runs in O(26·n) time (since for each `'?'` we examine 26 candidates) and O(1) extra space (excluding input and output storage). The final cost is computed in O(n).
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <climits>

// Replace every '?' with the best letter to minimize local cost, then return total cost and final string.
std::pair<long long, std::string> replaceQuestionMarks(std::string s, const std::vector<int>& cost) {
    int n = static_cast<int>(s.size());

    for (int i = 0; i < n; ++i) {
        if (s[i] == '?') {
            int bestCost = INT_MAX;
            char bestChar = 'a';

            for (int j = 0; j < 26; ++j) {
                char c = static_cast<char>('a' + j);
                int currentCost = 0;

                if (i > 0 && s[i - 1] != '?') {
                    currentCost += std::abs(cost[s[i - 1] - 'a'] - cost[c - 'a']);
                }
                if (i < n - 1 && s[i + 1] != '?') {
                    currentCost += std::abs(cost[c - 'a'] - cost[s[i + 1] - 'a']);
                }

                if (currentCost < bestCost || (currentCost == bestCost && c < bestChar)) {
                    bestCost = currentCost;
                    bestChar = c;
                }
            }
            s[i] = bestChar;
        }
    }

    long long totalCost = 0;
    for (int i = 0; i < n - 1; ++i) {
        totalCost += std::abs(cost[s[i] - 'a'] - cost[s[i + 1] - 'a']);
    }

    return {totalCost, s};
}
#include <cassert>
#include <string>
#include <vector>
#include <utility>

std::pair<long long, std::string> replaceQuestionMarks(std::string s, const std::vector<int>& cost);

int main() {
    // Example from the code snippet: s="abc??def?gh", costs = {4,9,5,9,6,1,0,3,7,2,5,9,6,1,3,2,3,2,9,1,1,0,1,8,8,4}
    std::vector<int> cost1 = {4,9,5,9,6,1,0,3,7,2,5,9,6,1,3,2,3,2,9,1,1,0,1,8,8,4};
    auto result1 = replaceQuestionMarks("abc??def?gh", cost1);
    assert(result1.first == 25);
    assert(result1.second == "abcbbdeffgh");

    // All '?' string, all costs zero -> every replacement becomes 'a' and total cost 0.
    std::vector<int> cost2(26, 0);
    auto result2 = replaceQuestionMarks("???", cost2);
    assert(result2.first == 0);
    assert(result2.second == "aaa");

    // Single character no '?' -> cost 0, string unchanged.
    std::vector<int> cost3(26, 5);
    auto result3 = replaceQuestionMarks("z", cost3);
    assert(result3.first == 0);
    assert(result3.second == "z");

    // Two '?' with fixed endpoints: costs differ, choose lower local cost.
    std::vector<int> cost4 = {10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    auto result4 = replaceQuestionMarks("a?b", cost4);
    // For a?b, if ?='a': |10-10|+|10-0|=10; if ?='b': |10-0|+|0-0|=10; tie -> lexicographically 'a'
    assert(result4.first == 10);
    assert(result4.second == "aab");

    // Cost pattern where lexicographic tie-breaking matters.
    std::vector<int> cost5 = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
    auto result5 = replaceQuestionMarks("?b", cost5);
    // Either 'a' or 'b' gives cost 0; tie -> 'a' because lexicographically smaller.
    assert(result5.first == 0);
    assert(result5.second == "ab");

    // String with only one '?' at the start, next is 'c' with distinct cost.
    std::vector<int> cost6 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    auto result6 = replaceQuestionMarks("?c", cost6);
    // For ?='a': |1-3|=2; ?='b': |2-3|=1; ?='c': |3-3|=0 -> choose 'c', cost 0.
    assert(result6.first == 0);
    assert(result6.second == "cc");

    return 0;
}
