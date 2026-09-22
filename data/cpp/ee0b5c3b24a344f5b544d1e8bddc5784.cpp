// Write a C++ function `bool isValidBrackets(const std::string& input)` that determines whether a string consisting only of the characters `(`, `)`, `[`, `]`, `{`, and `}` forms a correctly nested and balanced parentheses sequence. The function should return `true` if every opening bracket has a corresponding closing bracket in the correct order and no unmatched or mismatched brackets remain; otherwise, return `false`. The input string may be empty (which is considered valid), may contain only one type of bracket, or may mix all three types. Handle edge cases such as a closing bracket appearing before any opening bracket, mismatched pairs like `(]`, and leftover unclosed brackets like `([`.
// The solution uses a stack to track unmatched opening brackets. Iterate through each character of the input string: if it is an opening bracket (`(`, `[`, `{`), push it onto the stack. If it is a closing bracket (`)`, `]`, `}`), check that the stack is not empty and that the top of the stack is the matching opening bracket (using a helper function that maps pairs: `(`↔`)`, `[`↔`]`, `{`↔`}`). If either condition fails, return `false` immediately. After processing all characters, the string is valid only if the stack is empty — meaning every opening bracket was properly closed. Key edge cases include: empty string (valid), a single unmatched opening bracket, a closing bracket with empty stack, and mismatched pairs. Time complexity is O(n), where n is the length of the string, and space complexity is O(n) in the worst case (e.g., all opening brackets), because the stack can hold up to n characters.
#include <string>
#include <stack>

// Helper to check if a closing bracket matches the top opening bracket.
bool isMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Determine if the provided string has balanced and correctly nested brackets.
bool isValidBrackets(const std::string& input) {
    std::stack<char> st;

    for (char ch : input) {
        if (ch == '(' || ch == '[' || ch == '{') {
            st.push(ch);
        } else {
            // ch is a closing bracket
            if (st.empty()) return false;           // no opening bracket available
            if (!isMatchingPair(st.top(), ch)) return false; // wrong type
            st.pop();
        }
    }

    return st.empty(); // all opening brackets must be closed
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(isValidBrackets("()") == true);
    assert(isValidBrackets("([])") == true);
    assert(isValidBrackets("({[]})") == true);
    assert(isValidBrackets("()[]{}") == true);

    // Edge case: empty string
    assert(isValidBrackets("") == true);

    // Invalid cases
    assert(isValidBrackets("(]") == false);
    assert(isValidBrackets("([)]") == false);
    assert(isValidBrackets("(") == false);
    assert(isValidBrackets(")") == false);
    assert(isValidBrackets("{[}") == false);
    assert(isValidBrackets("({[}])") == false);

    // Valid with mixed types
    assert(isValidBrackets("{[]}()") == true);
    assert(isValidBrackets("[{()}]") == true);

    // Invalid: extra closing bracket
    assert(isValidBrackets("()]") == false);
    assert(isValidBrackets("{}[])") == false);
}
