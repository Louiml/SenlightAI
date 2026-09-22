/*
Write a C++ function `std::vector<int> bracketNumbers(const std::string& str)` that takes a string containing parentheses and other characters, and returns a vector of integers that assigns matching numbers to each pair of parentheses. Specifically, the first `(` encountered gets number 1, the second gets 2, and so on; each `)` is assigned the same number as its matching opening parenthesis. The output vector must contain one integer for every parenthesis in the original string, in the order they appear, while all non-parenthesis characters are ignored. The input string is guaranteed to be well-formed (balanced parentheses), and it may contain letters, digits, spaces, and other symbols.
*/

#include <string>
#include <vector>
#include <stack>

// Returns a vector of integers where each parenthesis in the input is
// replaced by its matching bracket number. Non-parenthesis characters are ignored.
std::vector<int> bracketNumbers(const std::string& str) {
    std::vector<int> result;
    std::stack<int> openStack;
    int nextNumber = 1;
    
    for (char ch : str) {
        if (ch == '(') {
            openStack.push(nextNumber);
            result.push_back(nextNumber);
            ++nextNumber;
        } else if (ch == ')') {
            result.push_back(openStack.top());
            openStack.pop();
        }
    }
    
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared here (in actual usage it would be included from above)
std::vector<int> bracketNumbers(const std::string& str);

int main() {
    // Simple case: one pair
    assert(bracketNumbers("()") == std::vector<int>({1, 1}));
    
    // Multiple independent pairs
    assert(bracketNumbers("()()()") == std::vector<int>({1, 1, 2, 2, 3, 3}));
    
    // Nested pairs
    assert(bracketNumbers("(())") == std::vector<int>({1, 2, 2, 1}));
    
    // Mixed nesting and siblings
    assert(bracketNumbers("(()())") == std::vector<int>({1, 2, 2, 1, 3, 3}));
    
    // With non-parenthesis characters
    assert(bracketNumbers("a(b)c(d)e") == std::vector<int>({1, 1, 2, 2}));
    
    // Deep nesting
    assert(bracketNumbers("((()))") == std::vector<int>({1, 2, 3, 3, 2, 1}));
    
    // Empty string
    assert(bracketNumbers("") == std::vector<int>());
    
    // Complex example with letters, digits, spaces
    assert(bracketNumbers("(x (y) z (w v))") == std::vector<int>({1, 2, 2, 1, 3, 3, 1}));
    
    // No parentheses
    assert(bracketNumbers("hello") == std::vector<int>());
    
    return 0;
}

// The algorithm uses a stack to keep track of the currently open parentheses and their assigned numbers. Traverse the string character by character from left to right:
// - For each `(`, push the next available number (starting from 1, incrementing each time) onto the stack, and append that same number to the result vector.
// - For each `)`, the top of the stack contains the matching number for the most recently opened parenthesis; append that number to the result vector and pop it from the stack.
// - Non-parenthesis characters are ignored entirely.
//
// The key insight is that the stack maintains a LIFO order that exactly mirrors the pairing of parentheses: the last opened parenthesis is the first to be closed. Since the string is guaranteed well-formed, the stack will never be empty when encountering a `)`. The numbers are assigned sequentially in the order opening parentheses appear, which is the natural numbering scheme.
//
// Edge cases: The input may be empty (returns empty vector). The input may contain nested parentheses like `(()())`, which produces numbers `1 2 2 1 3 3`. The input may contain unmatched characters, but they are simply skipped. The time complexity is O(n) where n is the length of the string, and the auxiliary space is O(k) where k is the maximum nesting depth (for the stack) plus O(m) for the result vector where m is the number of parentheses.
