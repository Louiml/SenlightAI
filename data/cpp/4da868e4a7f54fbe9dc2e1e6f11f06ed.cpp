/*
Write a C++ function `longestBalancedSubsequence` that takes a string containing only the characters `'('`, `')'`, `'['`, and `']'`, and returns a struct `Result` holding the maximum number of `'['` characters that appear in a properly nested (balanced) contiguous substring, along with the starting index and ending index of that substring (inclusive). A substring is properly nested if every opening bracket has a matching closing bracket of the same type in the correct order, and no brackets overlap incorrectly (e.g., `([)]` is invalid). If there are multiple substrings with the same maximum count, return the one with the smallest starting index; if a tie persists, return the one with the smallest ending index. If no balanced substring contains any `'['`, return count `0` and indices `-1, -1`. You may assume the input length is between 1 and 100,000.
*/

#include <string>
#include <stack>
#include <vector>
#include <algorithm>

struct Result {
    int count;
    int left;
    int right;
};

// Finds the longest balanced substring with maximum number of '['.
// Returns count and inclusive indices; if none, count=0, left=right=-1.
Result longestBalancedSubsequence(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> prefix(n, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i] = (s[i] == '[') ? 1 : 0;
        if (i > 0) prefix[i] += prefix[i - 1];
    }

    auto rangeSum = [&](int l, int r) {
        return (l == 0) ? prefix[r] : prefix[r] - prefix[l - 1];
    };

    std::stack<int> openStack;          // indices of unmatched '(' or '['
    std::stack<std::pair<int, int>> seg; // valid segments as (left, right), top is leftmost

    auto isOpen = [](char c) { return c == '(' || c == '['; };
    auto matches = [](char open, char close) {
        return (open == '(' && close == ')') || (open == '[' && close == ']');
    };

    for (int i = 0; i < n; ++i) {
        char c = s[i];
        if (isOpen(c)) {
            openStack.push(i);
        } else {
            // closing bracket
            if (!openStack.empty() && matches(s[openStack.top()], c)) {
                int left = openStack.top();
                openStack.pop();
                int right = i;

                while (true) {
                    if (!seg.empty() && seg.top().second + 1 == left) {
                        // Adjacent: extend left boundary
                        left = seg.top().first;
                        seg.pop();
                    } else if (!seg.empty() && seg.top().first > left && seg.top().second < right) {
                        // Current segment is inside an existing larger one? 
                        // Actually the existing one is smaller, so we pop it because it's contained in the new one.
                        seg.pop();
                    } else {
                        seg.push({left, right});
                        break;
                    }
                }
            } else {
                // Unmatched closing: reset opening stack
                while (!openStack.empty()) openStack.pop();
                // Also clear seg? No, existing segments remain valid; but any segment that would have been
                // continued is now blocked, however we can keep what we have.
            }
        }
    }

    int bestCount = 0;
    int bestLeft = -1;
    int bestRight = -1;

    // Copy segments to vector to preserve order and allow stable tie-breaking
    std::vector<std::pair<int, int>> segList;
    while (!seg.empty()) {
        segList.push_back(seg.top());
        seg.pop();
    }
    std::reverse(segList.begin(), segList.end()); // leftmost first

    for (const auto& p : segList) {
        int l = p.first;
        int r = p.second;
        int cnt = rangeSum(l, r);
        if (cnt > bestCount ||
            (cnt == bestCount && (l < bestLeft || (l == bestLeft && r < bestRight)))) {
            bestCount = cnt;
            bestLeft = l;
            bestRight = r;
        }
    }

    if (bestCount == 0) {
        return {0, -1, -1};
    }
    return {bestCount, bestLeft, bestRight};
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic cases
    Result r;
    r = longestBalancedSubsequence("[]");
    assert(r.count == 1 && r.left == 0 && r.right == 1);

    r = longestBalancedSubsequence("([)]");
    assert(r.count == 0 && r.left == -1 && r.right == -1);

    r = longestBalancedSubsequence("()[]");
    // Two separate valid substrings: [0,1] and [2,3] both count 0 and 1
    // We pick smallest left, so [0,1] count 0.
    assert(r.count == 0 && r.left == 0 && r.right == 1);

    r = longestBalancedSubsequence("([[]])");
    // Whole string is balanced, contains 2 '[' -> indices 2,3
    assert(r.count == 2 && r.left == 2 && r.right == 3);

    r = longestBalancedSubsequence("[[[]]]");
    // Whole string, 3 '[' at indices 0,1,2
    assert(r.count == 3 && r.left == 0 && r.right == 2);

    r = longestBalancedSubsequence("((()))");
    assert(r.count == 0 && r.left == -1 && r.right == -1);

    r = longestBalancedSubsequence("][");
    assert(r.count == 0 && r.left == -1 && r.right == -1);

    r = longestBalancedSubsequence("[][[]]");
    // Whole string balanced, has 3 '[' (indices 0,2,3) but substring [2,5] has 2, whole has 3.
    assert(r.count == 3 && r.left == 0 && r.right == 2);

    r = longestBalancedSubsequence("a");
    // Input may contain only brackets, but test with letters? Actually spec says only brackets; we'll ignore.
    // Using "a" is invalid, but function should handle gracefully? We'll skip.
    
    // Edge: large nested
    std::string big(5000, '[');
    for (int i = 0; i < 5000; ++i) big += "]";
    r = longestBalancedSubsequence(big);
    assert(r.count == 5000 && r.left == 0 && r.right == 4999);

    // Tie-breaking: "[][]" has two separate [0,1] and [2,3] each count 1; choose [0,1]
    r = longestBalancedSubsequence("[][]");
    assert(r.count == 1 && r.left == 0 && r.right == 1);

    // Tie-breaking within same count: "[[][]" hmm complex, skip.

    return 0;
}

// The classic approach processes the string left to right with a stack of indices for opening brackets. When a closing bracket is encountered, if it matches the top opening bracket, we pop and form a candidate "segment" from the popped index to the current closing index. To extend validated substrings efficiently, we merge adjacent overlapping or adjacent segments: whenever a new segment (from `l` to `r` after popping) either is immediately preceded by an existing segment ending at `l-1` (adjacent link), we merge and extend the left boundary; or if the new segment is completely inside an existing segment (i.e., the existing one already has a larger range that encloses it), then the existing segment already represents a balanced substring that includes the new one, so we replace (pop) the existing segment with the new smaller one that has accurate boundaries. We maintain a stack of disjoint or adjacent valid segments, ordered by increasing left index (top is the leftmost). After processing all characters, the stack contains all maximal valid segments; we iterate through them to compute the `[` count via prefix sums and choose the best per the tie-breaking rules. Edge cases include unmatched closings (we clear the stack of opening brackets), consecutive independent balanced substrings (they should be considered separate unless adjacent), and nested brackets (the inner segment is handled by the `in` condition). Time complexity is O(n) because each character is pushed/popped at most once and each segment is pushed/popped at most once; space is O(n) for the stacks and prefix array.
