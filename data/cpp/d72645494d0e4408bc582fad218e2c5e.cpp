Write a C++ function `int netBalance(const std::string& transactions)` that takes a string consisting only of the characters `'+'` and `'-'` (with no spaces, guaranteed non-empty). The function should interpret each character as a transaction: `'+'` increases a starting balance of 0 by 1, and `'-'` decreases it by 1. Return the final net balance after processing all characters. For example, `"++-+"` should return `2` (starting at 0, +1, +1, -1, +1 = 2). The input will never be empty. Your solution must be a standalone function (no `main`), using only standard headers, and you must not modify the input string.

#include <cassert>
#include <string>

int netBalance(const std::string& transactions); // declaration from solution

int main() {
    assert(netBalance("+") == 1);
    assert(netBalance("-") == -1);
    assert(netBalance("+++") == 3);
    assert(netBalance("---") == -3);
    assert(netBalance("+-+-") == 0);
    assert(netBalance("++--++") == 2);
    assert(netBalance("----++++") == 0);
    assert(netBalance("+--+--+") == -1);
    assert(netBalance("++++++++++") == 10);
    assert(netBalance("----------") == -10);
    return 0;
}

#include <string>

// Returns the net balance after processing all '+' and '-' characters.
// '+' adds 1 to the balance, '-' subtracts 1.
// The input is a non-empty string containing only '+' and '-'.
int netBalance(const std::string& transactions) {
    int balance = 0;
    for (char ch : transactions) {
        if (ch == '+') {
            ++balance;
        } else { // ch == '-'
            --balance;
        }
    }
    return balance;
}

// The algorithm is a straightforward linear scan: initialize `balance = 0`, then iterate over each character in the string. For each `'+'`, increment `balance`; for each `'-'`, decrement `balance`. Since the input is guaranteed to contain only these two characters, no other cases need handling. The edge case of an empty string is excluded by the task, but if it occurred, the function would return `0` naturally because the loop would not execute. Time complexity is O(n), where n is the length of the string, as we visit each character once. Space complexity is O(1) beyond the input itself (we only use a single integer). The function should be `const`-correct by taking the input by `const std::string&` and not modifying it.
