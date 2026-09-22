/*
Write a C++ function that simulates a betting game. The function takes an initial token balance `chouMa` and a number of rounds `num`, followed by `num` lines of four integers: `n1`, `b`, `t`, `n2`. For each round, if `t` is greater than the current balance, print `"Not enough tokens.  Total = <balance>.\n"` and continue. Otherwise, if the guess is correct — meaning (`n1 < n2` and `b == 1`) or (`n1 > n2` and `b == 0`) — increase the balance by `t` and print `"Win <t>!  Total = <balance>.\n"`. If the guess is wrong, decrease the balance by `t` and print `"Lose <t>.  Total = <balance>.\n"`; if the balance becomes zero, print `"Game Over."` and stop processing further rounds immediately (do not consume remaining input). The function should return the final balance after all rounds are processed or after a game-over, and should not output anything else. Assume input is valid and `num >= 0`, `chouMa >= 0`, and `t >= 0`.
*/
#include <vector>
#include <array>
#include <cstdio>

// Simulates a betting game. Returns final balance after processing all rounds
// or after a game-over (balance reaches zero). The `rounds` parameter contains
// `numRounds` entries, each with {n1, b, t, n2}.
int bettingGame(int initialBalance, int numRounds, const std::vector<std::array<int,4>>& rounds) {
    int balance = initialBalance;
    for (int i = 0; i < numRounds && balance > 0; ++i) {
        int n1 = rounds[i][0];
        int b  = rounds[i][1];
        int t  = rounds[i][2];
        int n2 = rounds[i][3];

        if (t <= balance) {
            // Correct guess: (n1 < n2 and b == 1) or (n1 > n2 and b == 0)
            if ((n1 < n2 && b == 1) || (n1 > n2 && b == 0)) {
                balance += t;
                std::printf("Win %d!  Total = %d.\n", t, balance);
            } else {
                balance -= t;
                std::printf("Lose %d.  Total = %d.\n", t, balance);
                if (balance == 0) {
                    std::printf("Game Over.");
                }
            }
        } else {
            std::printf("Not enough tokens.  Total = %d.\n", balance);
        }
    }
    return balance;
}
#include <cassert>
#include <vector>
#include <array>

int main() {
    // Case 1: Normal win and loss sequence, ending with positive balance.
    {
        std::vector<std::array<int,4>> rounds = {{1,1,10,2}, {5,0,5,3}, {7,1,2,7}};
        // Round1: win (1<2, b=1) -> balance 10+10=20
        // Round2: lose (5>3 but b=0 means predict n_down, actually 5>3 so b=0 would be correct? Wait condition: (n1<n2 && b==1) OR (n1>n2 && b==0). Here n1=5>n2=3 so b=0 is correct -> win. Let's recalc: b=0 means guess n1>n2, true, so win. balance 20+5=25
        // Round3: t=2, n1=7, n2=7 -> neither < nor >, so lose. balance 25-2=23
        int final = bettingGame(10, 3, rounds);
        assert(final == 23);
    }
    // Case 2: Game over when balance becomes zero.
    {
        std::vector<std::array<int,4>> rounds = {{1,1,10,2}, {5,0,10,3}};
        // Round1 win -> 20
        // Round2: n1=5>n2=3, b=0 correct -> win? Wait b=0 means predict n1>n2, so win, balance 30. Not game over. Let's design loss: {5,1,10,3} with b=1 means predict n1<n2, but 5>3, so lose. balance 10-10=0 -> game over.
        std::vector<std::array<int,4>> rounds2 = {{1,1,10,2}, {5,1,10,3}};
        int final = bettingGame(10, 2, rounds2);
        assert(final == 0);
    }
    // Case 3: More tokens than needed.
    {
        std::vector<std::array<int,4>> rounds = {{1,1,15,2}};
        int final = bettingGame(10, 1, rounds);
        assert(final == 10); // Not enough tokens, balance unchanged
    }
    // Case 4: Zero rounds.
    {
        std::vector<std::array<int,4>> rounds;
        int final = bettingGame(5, 0, rounds);
        assert(final == 5);
    }
    // Case 5: Multiple wrong guesses leading to game over.
    {
        std::vector<std::array<int,4>> rounds = {{1,1,5,2}, {2,1,5,3}};
        // Round1: win -> 15
        // Round2: n1=2>n2=3? No, n1=2 < n2=3, b=1 correct -> win, balance 20. Not game over.
        // Let's change: {1,1,5,2} win -> 15, then {2,0,15,3}? b=0 means n1>n2, but 2<3 so lose -> balance 0. 
        std::vector<std::array<int,4>> rounds3 = {{1,1,5,2}, {2,0,15,3}};
        int final = bettingGame(10, 2, rounds3);
        assert(final == 0);
    }
    return 0;
}
// The core simulation processes each round sequentially, updating the balance based on the guess outcome. The main algorithm is straightforward: for each round, read four integers, check the token condition first, then the guess correctness. Important edge cases include:
// - If `num == 0`, no rounds are processed and the initial balance is returned.
// - If `t` is exactly zero, a correct guess wins zero tokens (balance unchanged) and a wrong guess loses zero (balance unchanged), but the messages are still printed — however, if the balance is already zero at the start of a round, the `t <= chouMa` condition with `t=0` will be true, but if the guess is wrong, the balance remains zero and "Game Over." is printed only if balance becomes zero (it already is, so yes, after the loss message it prints "Game Over." and returns). This matches the original behavior.
// - Negative `n1`, `n2` are allowed; comparisons are plain integer comparisons.
// - The function must not read from standard input; instead, it receives all data via parameters (e.g., a vector or arrays). For simplicity, we can pass a 2D vector of rounds, or a flat vector of ints. A clean signature: `int bettingGame(int initialBalance, int numRounds, const std::vector<std::array<int,4>>& rounds)`.
// - Time complexity: O(numRounds) because each round is processed in constant time. Space complexity: O(numRounds) to store the rounds vector, or O(1) if streaming input, but since the task is a standalone function, we use the vector parameter.
