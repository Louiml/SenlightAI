/*
Given a string `pattern` consisting only of the characters `'I'` (increasing) and `'D'` (decreasing), write a C++ function `smallestNumber` that returns the lexicographically smallest number (as a string of digits 1–9, each used exactly once) satisfying the relative magnitude constraints. For each adjacent pair of digits in the output, if `pattern[i] == 'I'`, then `output[i] < output[i+1]`; if `pattern[i] == 'D'`, then `output[i] > output[i+1]`. The output must contain exactly `pattern.length() + 1` digits, each from 1 to 9, with no repetition. For example, for `pattern = "II"`, the smallest valid number is `"123"` (since 1<2<3); for `pattern = "DD"`, the smallest is `"321"` (since 3>2>1); for `pattern = "ID"`, the smallest is `"132"` (since 1<3 and 3>2). The function must handle empty `pattern` (returning `"1"`) and any length up to, say, 8 (so at most 9 digits). Implement the function efficiently.
*/
#include <string>
#include <stack>

// Returns the lexicographically smallest number (as a string of digits 1..9)
// satisfying the increase/decrease constraints given by the pattern.
std::string smallestNumber(const std::string& pattern) {
    std::string result;
    std::stack<char> pending;
    
    // Start with the smallest digit.
    pending.push('1');
    
    for (char c : pattern) {
        // The next digit to consider is one greater than the current top.
        char next_digit = pending.top() + 1;
        pending.push(next_digit);
        
        if (c == 'I') {
            // On 'I', output all pending digits (they form an increasing run).
            while (!pending.empty()) {
                result += pending.top();
                pending.pop();
            }
        }
        // On 'D', we keep digits on the stack; they will be reversed later.
    }
    
    // Flush remaining digits (handles trailing 'D's).
    while (!pending.empty()) {
        result += pending.top();
        pending.pop();
    }
    
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test (assumes it is defined above).
std::string smallestNumber(const std::string& pattern);

int main() {
    // Basic cases
    assert(smallestNumber("I") == "12");
    assert(smallestNumber("D") == "21");
    assert(smallestNumber("II") == "123");
    assert(smallestNumber("DD") == "321");
    assert(smallestNumber("ID") == "132");
    assert(smallestNumber("DI") == "213");
    
    // Mixed patterns
    assert(smallestNumber("IID") == "1243");
    assert(smallestNumber("DID") == "2143");
    assert(smallestNumber("IDID") == "13254");
    assert(smallestNumber("DDI") == "3214");
    
    // Empty and longer patterns
    assert(smallestNumber("") == "1");
    assert(smallestNumber("IIII") == "12345");
    assert(smallestNumber("DDDD") == "54321");
    
    return 0;
}
// The problem is a classic "construct smallest number from I/D pattern" puzzle. A straightforward greedy approach that assigns digits sequentially while respecting constraints often fails because a `'D'` forces a reversal that may lead to a non‑minimal result. The correct approach uses a stack to temporarily hold digits until a `'I'` (or the end) forces them to be output in reverse order, which naturally handles the decreasing runs. The algorithm works as follows:
// - Initialize an empty result string and a stack that will store digits to be output later.
// - Start with the smallest digit `'1'` and push it onto the stack.
// - For each character `c` in the pattern:
//   - Push the next digit (current maximum + 1) onto the stack.
//   - If `c == 'I'`, then pop all elements from the stack and append them to the result. This reverses the last increasing run (which includes the just‑pushed digit) and ensures that any preceding `'D'` run (which was pushed but not yet popped) is output in reverse order, producing the smallest possible arrangement for that segment.
// - After processing the pattern, pop any remaining stack elements and append them to the result. This handles a trailing run of `'D'` characters (or a pattern ending in `'D'`), whose digits must be output in decreasing order.
// - Edge cases: an empty pattern returns `"1"` (the stack holds only `'1'` and then is popped). A pattern with only `'I'` yields strictly increasing digits; a pattern with only `'D'` yields strictly decreasing digits. The algorithm guarantees each digit 1..(n+1) is used exactly once, and because we always push the next smallest available digit, the result is lexicographically minimal.
// - Time complexity: O(n) where n is the length of the pattern, because each digit is pushed and popped exactly once. Space complexity: O(n) for the stack and the result string (result is of length n+1).
