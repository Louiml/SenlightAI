// Write a C++ function `int countValidRotations(const std::string& brackets)` that takes a string containing only the characters `(`, `)`, `{`, `}`, `[`, and `]`. The function should rotate the string to the left by each possible starting index (i.e., consider all cyclic rotations where the string is split at position `i` and recombined as `s.substr(i) + s.substr(0, i)`). For each rotation, check if the resulting string is a valid bracket sequence, meaning every opening bracket has a corresponding closing bracket of the same type in the correct order, and no closing bracket appears without a matching opener. Return the total number of valid rotations. If the string has odd length, return 0 immediately, since no rotation can be valid for an odd number of brackets. You may assume the input string is non-empty and contains only the six bracket characters.
The algorithm follows the logic in the provided snippet. First, if the string length is odd, return 0 because any valid bracket sequence must have even length. Then, for each index `i` from 0 to `n-1`, create the rotated string `r = s.substr(i) + s.substr(0, i)`. Only consider rotations where the first character of `r` is an opening bracket (`(`, `{`, `[`), because a valid sequence cannot start with a closing bracket. For such a candidate, use a stack to simulate matching: push the expected closing character for each opening bracket, and when a closing bracket appears, check it matches the top of the stack and pop; otherwise, mark the rotation invalid. After processing all characters, the rotation is valid if the stack is empty. Count all such valid rotations. Edge cases: odd length immediately returns 0; strings that never start with an opening bracket in any rotation (e.g., all closing brackets) yield 0; empty stack when encountering a closing bracket indicates invalid. Time complexity is O(n^2) per rotation check, summed over n rotations, so O(n^2) total; space complexity is O(n) for the stack and the rotated string.
#include <string>
#include <stack>
#include <map>

// Count how many cyclic rotations of the bracket string are valid bracket sequences.
int countValidRotations(const std::string& brackets) {
    const std::size_t n = brackets.size();
    if (n % 2 != 0) {
        return 0;
    }

    // Map opening brackets to their expected closing counterpart.
    const std::map<char, char> matching = {
        {'(', ')'},
        {'{', '}'},
        {'[', ']'}
    };

    int validCount = 0;

    for (std::size_t start = 0; start < n; ++start) {
        // Only consider rotations that begin with an opening bracket.
        if (brackets[start] != '(' && brackets[start] != '{' && brackets[start] != '[') {
            continue;
        }

        // Build the rotated string.
        const std::string rotated = brackets.substr(start) + brackets.substr(0, start);

        std::stack<char> expectedClosers;
        bool isValid = true;

        for (char ch : rotated) {
            if (ch == '(' || ch == '{' || ch == '[') {
                expectedClosers.push(matching.at(ch));
            } else {
                // Closing bracket: must match the most recent opener.
                if (expectedClosers.empty() || ch != expectedClosers.top()) {
                    isValid = false;
                    break;
                }
                expectedClosers.pop();
            }
        }

        if (isValid && expectedClosers.empty()) {
            ++validCount;
        }
    }

    return validCount;
}
#include <cassert>

int main() {
    // Basic cases from common bracket problems.
    assert(countValidRotations("[](){}") == 1);       // Only zero rotation is valid.
    assert(countValidRotations("()") == 1);            // Only rotation 0 valid.
    assert(countValidRotations(")(") == 0);            // No rotation starts with opener.
    assert(countValidRotations("{}[]()") == 1);        // Only rotation 0 valid.

    // Rotations that become valid at different starts.
    assert(countValidRotations("()[]") == 2);          // Rotations 0 and 2 are valid.
    assert(countValidRotations("([)]") == 0);          // No rotation is valid due to mismatched types.
    assert(countValidRotations("{[()]}") == 1);        // Only original is valid.

    // Odd-length strings always return 0.
    assert(countValidRotations("(") == 0);
    assert(countValidRotations("()(") == 0);
    assert(countValidRotations("[") == 0);

    // Multiple valid rotations from balanced but different patterns.
    assert(countValidRotations("()()") == 2);          // Rotations 0 and 2 valid.
    assert(countValidRotations("{{}}") == 1);          // Only original valid.

    // Empty stack at end and valid rotations.
    assert(countValidRotations("[]") == 1);
    assert(countValidRotations("][") == 0);

    return 0;
}
