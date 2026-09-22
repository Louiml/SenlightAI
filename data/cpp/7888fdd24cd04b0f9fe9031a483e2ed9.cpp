Write a C++ function named `gameWinner` that takes a positive integer `n` and a string `results` representing the outcomes of `n` games between two friends, Anton and Danik. Each character in the string is either `'A'` (Anton wins that game) or `'D'` (Danik wins that game). The function should return the string `"Anton"` if Anton wins more games than Danik, `"Danik"` if Danik wins more games, and `"Friendship"` if they win the same number of games. Assume the string length exactly equals `n`, all characters are uppercase `'A'` or `'D'`, and `n` is at least 1. No other characters will appear.
The solution is a straightforward counting problem. Iterate through each character of the input string once, incrementing two counters: one for Anton's wins (when character is `'A'`) and one for Danik's wins (when character is `'D'`). Since the problem guarantees only `'A'` and `'D'` appear, no other cases need handling. After counting, compare the two totals: if Anton's count is greater, return `"Anton"`; if Danik's count is greater, return `"Danik"`; otherwise, return `"Friendship"`. Edge cases: when `n` is 1, the function returns the winner's name directly; when the counts are equal (including all draws), it returns `"Friendship"`. The time complexity is O(n) because we traverse the string once, and the space complexity is O(1) because we use only two integer counters and a constant-size return string (no dynamic memory beyond the inputs).
#include <string>
#include <algorithm>

// Count wins for Anton ('A') and Danik ('D') in a string of game results.
// Return "Anton" if Anton has more wins, "Danik" if Danik has more, 
// and "Friendship" if equal.
std::string gameWinner(int n, const std::string& results) {
    int antonWins = 0;
    int danikWins = 0;

    for (int i = 0; i < n; ++i) {
        if (results[i] == 'A') {
            ++antonWins;
        } else { // guaranteed to be 'D'
            ++danikWins;
        }
    }

    if (antonWins > danikWins) {
        return "Anton";
    } else if (danikWins > antonWins) {
        return "Danik";
    } else {
        return "Friendship";
    }
}
#include <cassert>
#include <string>

// The gameWinner function is assumed to be defined above (not repeated here).
int main() {
    assert(gameWinner(1, "A") == "Anton");
    assert(gameWinner(1, "D") == "Danik");
    assert(gameWinner(2, "AD") == "Friendship");
    assert(gameWinner(3, "AAA") == "Anton");
    assert(gameWinner(3, "DDD") == "Danik");
    assert(gameWinner(5, "AADDA") == "Friendship");
    assert(gameWinner(6, "ADADAD") == "Friendship");
    assert(gameWinner(4, "AADD") == "Friendship");
    assert(gameWinner(7, "AAAAADD") == "Anton");
    assert(gameWinner(7, "DDDDDAA") == "Danik");
    return 0;
}
