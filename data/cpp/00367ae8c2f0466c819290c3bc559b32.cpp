/*
Write a C++ function `int redundantBraces(const std::string& expr)` that takes a fully parenthesized arithmetic expression string consisting of digits, the binary operators `+`, `-`, `*`, `/`, and parentheses `(` and `)`. The expression is guaranteed to be syntactically valid, meaning parentheses are balanced and operators appear between operands (each operand is a single digit). The function must return `1` if the expression contains any redundant (unnecessary) pair of parentheses, and `0` otherwise. A pair of parentheses is considered redundant if removing it does not change the mathematical meaning of the expression; specifically, a pair is redundant if the subexpression inside it contains no operator at all (e.g., `(3)`) or contains only one operand (i.e., just a digit), or if the entire expression inside a pair is already enclosed by another redundant outer pair in a trivial way. The simplest detection: scan left to right; whenever you encounter a closing parenthesis, inspect the characters between the matching opening parenthesis and this closing one. If among those characters there is no operator or there is no operand (i.e., at most one operand and zero operators, or zero operands), then the parenthesis pair is redundant. Also, after processing all closing parentheses, if any unmatched opening parenthesis remains, treat it as redundant. Return `1` if any redundant pair is found, else `0`. The input string length will be between 1 and 1000. You must not assume the input is properly formatted beyond the guarantees; your solution must work for edge cases like `(1)`, `()`, `(1+2)`, `((1+2))` (the outer pair is redundant), and `(1+(2*3))` (no redundancy).
*/

#include <string>
#include <stack>

// Returns 1 if the expression contains redundant parentheses, 0 otherwise.
// The expression is assumed to be syntactically valid (balanced parentheses,
// digits as operands, operators + - * / between operands).
int redundantBraces(const std::string& expr) {
    std::stack<char> st;
    for (char ch : expr) {
        if (ch != ')') {
            st.push(ch);
        } else {
            bool hasOperator = false;
            bool hasOperand = false;
            while (!st.empty() && st.top() != '(') {
                char c = st.top();
                st.pop();
                if (c == '+' || c == '-' || c == '*' || c == '/') {
                    hasOperator = true;
                } else {
                    // Any non-operator character inside is considered an operand (digit)
                    hasOperand = true;
                }
            }
            // After the loop, the top should be '(' if expression is valid
            if (!st.empty() && st.top() == '(') {
                st.pop();
            }
            if (!hasOperator || !hasOperand) {
                return 1; // redundant pair found
            }
        }
    }
    // Any unmatched '(' left in stack is redundant
    while (!st.empty()) {
        if (st.top() == '(') {
            return 1;
        }
        st.pop();
    }
    return 0;
}

#include <cassert>
#include <string>

// Forward declaration of the solution function
int redundantBraces(const std::string& expr);

int main() {
    // Basic non-redundant expressions
    assert(redundantBraces("(1+2)") == 0);
    assert(redundantBraces("(1+(2*3))") == 0);
    assert(redundantBraces("(1+2)*(3-4)") == 0);

    // Redundant single operand
    assert(redundantBraces("(1)") == 1);
    assert(redundantBraces("((2))") == 1);
    assert(redundantBraces("(1+(2))") == 1); // inner (2) redundant

    // Empty parentheses are redundant
    assert(redundantBraces("()") == 1);

    // Outer redundant pair around a full expression
    assert(redundantBraces("((1+2))") == 1);

    // Expression with no parentheses at all is fine
    assert(redundantBraces("1+2*3") == 0);

    // Complex mixed case with one redundant pair
    assert(redundantBraces("(1+2)+(3)") == 1); // (3) redundant
    assert(redundantBraces("(1+2)+(3+4)") == 0);

    // Single digit with no parentheses is fine
    assert(redundantBraces("5") == 0);

    // Redundant pair around a single digit inside larger expression
    assert(redundantBraces("(1+(2*3))") == 0);
    assert(redundantBraces("((1+2)*3)") == 1); // outer pair wraps whole product

    return 0;
}

// The algorithm uses a stack to process the expression character by character. Push every non-`)` character onto the stack, including digits, operators, and opening parentheses. When a `)` is encountered, pop elements from the stack until the matching `(` is found. While popping, track two boolean flags: `hasOperator` (set true if any of `+`, `-`, `*`, `/` is seen) and `hasOperand` (set true if any character that is not an operator, not a parenthesis — i.e., a digit — is seen). After popping all the way to the opening parenthesis (but not popping it yet), check: if either `hasOperator` is false or `hasOperand` is false, then the parenthesis pair is redundant, so return `1`. If both are true, then pop the opening parenthesis and reset the flags for the next pair. After processing all characters, any remaining opening parentheses in the stack indicate unmatched (hence redundant) parentheses, so return `1` if any remain; otherwise return `0`. Edge cases include `()` (empty inside: both flags false → redundant), `(1)` (no operator → redundant), `(1+2)` (both true → not redundant), `((1+2))` — inner pair fine, outer pair has both flags true because inside the outer pair is the entire inner expression including operator, so both true → not redundant by this simple flag check, but actually the outer pair is redundant! Wait: careful — the simple flag check as described incorrectly marks `((1+2))` as non-redundant because inside the outer parentheses there is an operator (from the inner expression). The correct definition of redundant is more subtle: a pair is redundant if removing it does not change the order of operations. For `((1+2))`, removing the outer pair yields `(1+2)`, which is equivalent, so outer is redundant. A better detection: a pair of parentheses is redundant if the subexpression between them contains exactly one operand and zero operators, OR if that subexpression is itself a single parenthesized expression (i.e., the entire content is exactly one opening parenthesis, some stuff, one closing parenthesis) — in other words, if the content is a single non-atomic expression without any operator outside an inner pair. The simplest correct method: when processing a `)`, pop until the matching `(`, but also count the number of operators found at the "top level" of that segment, ignoring operators that are inside nested parentheses. To do that, while popping, maintain a count of nested parentheses; only count an operator if the current nesting depth (inside this popped segment) is zero relative to the segment. Alternatively, a known trick: push a special marker for each pair. The cleaner algorithm: traverse the string, use a stack of characters. On `)`, pop and count how many characters are between the `(` and `)` that are not themselves inside another set of parentheses. But simpler and correct: maintain a stack of booleans? Actually the well-known solution for "redundant braces" (InterviewBit problem) uses exactly the two-flag approach, but that approach works because the input is guaranteed to have exactly one operator per valid pair; then `((1+2))` is actually considered NOT redundant by that problem? Let's re-read the snippet: the original code returns 1 if `!oper || !operand`. That means it will return 1 for `(1)` but 0 for `((1+2))`? Let's test mentally: For `((1+2))`, scanning: push `(`, push `(`, push `1`, push `+`, push `2`, then `)`: pop `2` (operand true), pop `+` (oper true), pop `1` (operand true), then top is `(` → both true → pop `(`, reset flags. Then next `)`: pop? stack now has only first `(` and inside? After popping the first pair, the stack has `(` from the outer. Then we encounter the second `)`: we pop from stack until we hit the outer `(`? But there's nothing between them except the outer `(`? Actually after the first `)` processing, we popped the inner `(` and the stack contains only the outer `(`. Then we hit second `)`: while top != '('? The top is '(' so the while loop body does not execute, so both flags remain false from reset? Wait in the snippet, after the inner pair, `oper=false, operand=false` are reset. Then the while loop for the outer `)` sees top == '(' immediately, so it doesn't execute, so both flags are false, thus condition `!oper || !operand` true → returns 1. Ah yes! Because for the outer pair, there is nothing inside except the inner parentheses, but the while loop only pops characters until it hits a `(`, and since the immediate top is `(`, it pops nothing, so flags remain false. So the original code correctly identifies `((1+2))` as redundant. However, what about `(1+(2*3))`? The outer pair contains `1`, `+`, `(`, `2`, `*`, `3`, `)`. When processing the outer `)`, it will pop until it sees the outer `(`. It will pop: `)`, `3`, `*`, `2`, `(` (the inner opening) — but wait, the inner `)` was processed earlier and popped, so the stack at outer processing contains: `(`, `1`, `+`, `(`, `2`, `*`, `3`? Actually after processing inner `)` for `2*3`, we popped those and popped the inner `(`, leaving stack: `(`, `1`, `+`. Then outer `)` comes: pop `+` (oper true), pop `1` (operand true), then top is `(` → both true → not redundant. Good. So the two-flag approach is correct for the intended problem. But note the case `(1+2)` is not redundant, and `(1)` is redundant. The tricky case `((1+2))` is handled because the outer pair's content after popping includes the inner `(`? In the original code, when processing outer `)`, the top of stack is the inner `(`? No, because the inner `(` was popped when processing inner `)`. So the stack after inner processing is just the outer `(` then `1`, `+`, `2`? Wait, I need to carefully simulate: Original code: push characters. For `((1+2))`:
// i=0: push '(' -> stack: (
// i=1: push '(' -> stack: ( (
// i=2: push '1' -> ( ( 1
// i=3: push '+' -> ( ( 1 +
// i=4: push '2' -> ( ( 1 + 2
// i=5: ')' -> while top != '(': pop 2 (operand true), pop + (oper true), pop 1 (operand true), now top is '(' -> exit loop. Both true? yes. Then pop that '(' (the inner one). Stack now: ( (outer). Reset flags false.
// i=6: ')' -> while top != '(': top is '(' (outer) so loop doesn't execute. Both flags false -> condition true -> return 1. So correct.
// Thus the algorithm from the snippet is valid for the given constraints. However, the problem statement should ensure that the input is well-formed and fully parenthesized? The snippet doesn't check for unmatched closing parentheses beyond the stack logic. For simplicity, we can assume the input is valid as stated. We'll adopt the same two-flag method. Complexity: O(n) time, O(n) space for stack. Edge cases: empty string? Not allowed per constraints (length >=1). But we handle gracefully. Also note that digits are single characters, so any non-operator non-parenthesis character is an operand. The solution function must be `const` correct, take `const std::string&`, and return int.
