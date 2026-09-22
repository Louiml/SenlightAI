/*
Write a C++ function `std::string removeMatchedParentheses(int maxPairs, const std::string& sequence)` that takes a positive integer `maxPairs` and a string `sequence` consisting only of `'('` and `')'` characters, guaranteed to be a valid bracket sequence (i.e., balanced and properly nested). The function must return a new string that contains exactly the first `maxPairs` complete pairs of parentheses from `sequence`, where a pair is defined as a `'('` and its matching `')'` (the matching is based on the standard bracket-matching algorithm, not simply the first `maxPairs` opening and closing characters). The returned string must preserve the original relative order of the selected characters; if the total number of pairs in `sequence` is less than `maxPairs`, return the entire original string. The input will contain at least one pair.
*/

#include <string>
#include <vector>
#include <stack>
#include <algorithm>

// Given a valid bracket sequence and a number of pairs to keep,
// return a string consisting of the first maxPairs complete pairs.
std::string removeMatchedParentheses(int maxPairs, const std::string& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> match(n, -1);
    std::stack<int> openStack;

    // Build matching for each bracket.
    for (int i = 0; i < n; ++i) {
        if (sequence[i] == '(') {
            openStack.push(i);
        } else { // sequence[i] == ')'
            int openIndex = openStack.top();
            openStack.pop();
            match[i] = openIndex;
            match[openIndex] = i;
        }
    }

    std::vector<bool> selected(n, false);
    int pairsKept = 0;

    // Select the first maxPairs complete pairs from left to right.
    for (int i = 0; i < n && pairsKept < maxPairs; ++i) {
        if (sequence[i] == '(') {
            selected[i] = true;
            selected[match[i]] = true;
            ++pairsKept;
        }
    }

    // Build the resulting string.
    std::string result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        if (selected[i]) {
            result.push_back(sequence[i]);
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (since it's not in main).
std::string removeMatchedParentheses(int maxPairs, const std::string& sequence);

int main() {
    // Basic case: keep first two pairs of three.
    assert(removeMatchedParentheses(2, "(()())") == "(())");
    // Keep all pairs when maxPairs >= total pairs.
    assert(removeMatchedParentheses(5, "()") == "()");
    // Nested structure: first pair is the outer pair.
    assert(removeMatchedParentheses(1, "((()))") == "((()))");
    // Multiple outer-level pairs.
    assert(removeMatchedParentheses(1, "()()") == "()");
    // Keep both pairs, but order preserved.
    assert(removeMatchedParentheses(2, "()(())") == "()(())");
    // Large input: keep first 3 of 4 pairs.
    assert(removeMatchedParentheses(3, "(()())(())") == "(()())(())");
    // Only one pair.
    assert(removeMatchedParentheses(1, "(())") == "(())");
    // Keep zero pairs.
    assert(removeMatchedParentheses(0, "()") == "");
    // String starts with deeply nested.
    assert(removeMatchedParentheses(2, "((())(()))") == "((())(()))");
    // All pairs kept if maxPairs is huge.
    assert(removeMatchedParentheses(100, "()()()") == "()()()");

    return 0;
}

// The solution first builds a matching array `match` such that for every index `i` in `sequence`, `match[i]` stores the index of its corresponding partner bracket (for `'('` it is the position of its matching `')'`, and vice versa). This is computed using a stack: iterate through the string; when a `'('` is encountered, push its index; when a `')'` is encountered, it must match the top of the stack (since the sequence is valid), so set `match[i]` to the top index and `match[top]` to `i`, then pop. After this pass, we traverse the string from left to right, and whenever we see a `'('` and we have not yet selected `maxPairs` pairs, we mark both this index and its matching `')'` index as selected, and increment a pair counter by 1. Finally, we scan the string again and append to the result every character whose index is marked selected. This approach correctly selects whole pairs, because each `'('` and its matching `')'` are always marked together. Edge cases: if `maxPairs` is larger than the number of pairs in the sequence, we simply mark all pairs by iterating until the end. Time complexity is O(n), where n is the length of the sequence, for the matching pass and two linear scans; space complexity is O(n) for the `match` array and the output string.
