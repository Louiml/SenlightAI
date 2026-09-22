Given a length \( n \) (1 ≤ \( n \) ≤ 100,000) sequence of characters where each character is either an opening parenthesis `'('` or a closing parenthesis `')'`, write a C++ function that, when processing the characters from left to right, maintains a running balance (starting at 0, increasing by 1 for `'('`, decreasing by 1 for `')'`). The function should return the number of times the balance returns to exactly 0 after at least one character has been processed, but the counting must stop immediately if at any point the balance becomes negative (i.e., more closing than opening parentheses so far). If the balance goes negative, the function should return the count collected up to, but not including, that character. The function should not read any input; instead, it should accept the sequence as a `std::string` and return an `int`.

The solution processes each character sequentially while maintaining an integer `balance` that starts at 0. For each character: if it is `'('`, increment `balance`; if it is `')'`, decrement `balance`. After updating the balance, check: if `balance == 0`, increment the answer counter; if `balance < 0`, break out of the loop immediately and ignore the rest of the string. This is correct because the problem requires stopping at the first prefix that has more closing than opening parentheses. Important edge cases: an empty string (though constraints guarantee non-empty, handle it by returning 0), a string that never returns to zero (return 0), and a string where the first character is `')'` (balance becomes -1, so the count remains 0 and we stop). The algorithm is a single pass with O(1) auxiliary space, so time complexity is O(n) and space complexity is O(1). The function does not modify the input and should be `const`-correct by taking a `const std::string&` parameter.

#include <string>

// Count the number of times the running balance returns to zero,
// stopping early if the balance ever becomes negative.
int countBalanceReturns(const std::string& sequence) {
    int balance = 0;
    int returns = 0;

    for (char ch : sequence) {
        if (ch == '(') {
            ++balance;
        } else if (ch == ')') {
            --balance;
        }
        // Note: any other character is ignored, but per spec only '(' and ')' appear.

        if (balance == 0) {
            ++returns;
        } else if (balance < 0) {
            break;
        }
    }

    return returns;
}

#include <cassert>

int main() {
    // Test 1: Balanced parentheses, returns to zero twice: after ")(" and at end.
    assert(countBalanceReturns("()()") == 2);
    // Test 2: Nested parentheses, only returns once at the very end.
    assert(countBalanceReturns("(())") == 1);
    // Test 3: Single opening parenthesis, never returns to zero, answer 0.
    assert(countBalanceReturns("(") == 0);
    // Test 4: Single closing parenthesis, immediately negative, answer 0.
    assert(countBalanceReturns(")") == 0);
    // Test 5: Stops early after first negative: starts with "())", returns 0 after "()", then negative, so answer is 1.
    assert(countBalanceReturns("())") == 1);
    // Test 6: Longer sequence: "(()())" returns once at the end.
    assert(countBalanceReturns("(()())") == 1);
    // Test 7: "()(()" returns once after first "()", then balance never zeros again, answer 1.
    assert(countBalanceReturns("()(()") == 1);
    // Test 8: "((()))" returns once at end.
    assert(countBalanceReturns("((()))") == 1);
}
