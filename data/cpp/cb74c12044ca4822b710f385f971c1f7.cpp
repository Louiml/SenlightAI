Write a C++ function named `canAliceWin` that takes a single integer `n` representing the number of apples remaining in a game. In this game, two players, Alice and Bob, take turns removing apples; Alice always starts. On the first turn, Alice must remove 10 apples (if possible), on the next turn Bob removes 9, then Alice removes 8, Bob removes 7, and so on, with each turn requiring the removal of exactly one fewer apple than the previous turn (starting at 10 and decreasing by 1 each turn). A player who cannot remove the required number of apples on their turn loses the game. The function should return `true` if Alice can force a win given that exactly `n` apples are available at the start, and `false` otherwise. The input `n` is guaranteed to be a non-negative integer. The function must be a free function with signature `bool canAliceWin(int n)` and must be declared `const`-correct (i.e., it should not modify any state). Edge cases include `n` values where Alice cannot even make her first move (n < 10) and values where the game ends in mid-sequence.

#include <cassert>

int main() {
    // Alice cannot make her first move (needs 10)
    assert(canAliceWin(0) == false);
    assert(canAliceWin(9) == false);
    // Alice takes 10, Bob cannot take 9 -> Alice wins
    assert(canAliceWin(10) == true);
    assert(canAliceWin(18) == true);
    assert(canAliceWin(19) == false); // Alice 10, Bob 9, Alice cannot take 8
    assert(canAliceWin(26) == false);
    assert(canAliceWin(27) == true); // Alice 10, Bob 9, Alice 8, Bob cannot take 7
    assert(canAliceWin(33) == true);
    assert(canAliceWin(34) == false);
    // Full game: 10+9+8+7+6+5+4+3+2+1 = 55. If n=55, Alice takes 10, Bob 9, ... Bob takes 1 -> Alice's turn with no apples, loses.
    assert(canAliceWin(55) == false);
    assert(canAliceWin(54) == true); // After 10+9+8+7+6+5+4+3+2 = 54, Alice takes 1, Bob has 0 apples and cannot take 1? Wait check: n=54, moves: A10 (44), B9 (35), A8 (27), B7 (20), A6 (14), B5 (9), A4 (5), B3 (2), A2 (0), then Bob needs 1 but 0 left -> Bob loses, Alice wins. So 54 is true.
    assert(canAliceWin(53) == false); // After A10 B9 A8 B7 A6 B5 A4 B3 A2 (total 47? Let's compute: 10+9+8+7+6+5+4+3+2 = 54? Wait sum of 10 to 2 is 54? 10+9=19, +8=27, +7=34, +6=40, +5=45, +4=49, +3=52, +2=54. So 53: after A10(43), B9(34), A8(26), B7(19), A6(13), B5(8), A4(4), B3(1), A needs 2 but only 1 left -> Alice loses. So 53 false.
    assert(canAliceWin(52) == false); // As computed earlier.
    assert(canAliceWin(100) == false); // After 10 moves, apples remain, next required is 0, game ends with Alice's turn? Actually after 10 moves (sum 55), remaining 45, but required would be 0, so no move possible. The player whose turn it is (Bob because after 10 moves it's Alice? Let's see: move10 is Bob takes 1, then Alice's turn with n>0, required=0 -> Alice cannot move, loses. So false.
    assert(canAliceWin(49) == true); // n=49: A10(39), B9(30), A8(22), B7(15), A6(9), B5(4), A4(0), Bob needs 3 but 0 -> Bob loses, Alice wins.
}

#include <cstddef>

// Determine if Alice wins a game where players alternately remove 10,9,8,...,1 apples.
// Alice moves first. A player who cannot remove the required number of apples loses.
bool canAliceWin(int n) {
    int required = 10;       // First move removes 10 apples
    bool aliceTurn = true;   // Alice starts

    while (n >= required && required > 0) {
        n -= required;       // Current player removes the required apples
        --required;          // Next required removal decreases by 1
        aliceTurn = !aliceTurn; // Switch player
    }

    // If the loop ended because required <= 0, then no more moves are possible.
    // The player whose turn it is when the game ends (aliceTurn) loses.
    // If the loop ended because n < required, then the current player cannot move.
    return aliceTurn ? false : true;
}

// The game is deterministic: the sequence of required removals is fixed (10, 9, 8, 7, 6, 5, 4, 3, 2, 1) repeated? Actually, the pattern continues decreasing by 1 each turn, but the game ends the moment a player cannot take the required number. Since Alice always starts, the sequence of moves is: Alice takes 10 (if n≥10), Bob takes 9 (if remaining≥9), Alice takes 8, Bob takes 7, Alice takes 6, Bob takes 5, Alice takes 4, Bob takes 3, Alice takes 2, Bob takes 1. The game can end at any point when the remaining apples are less than the required number for the current player. Because the required numbers decrease each turn, we can simulate the game or directly compute the winner. Observing the pattern: if Alice cannot take 10 (n<10), she loses. If Alice takes 10, the remaining apples are n-10. Then Bob must take 9; if remaining <9, Bob loses (Alice wins). Otherwise Bob takes 9, remaining = n-19. Then Alice must take 8; if remaining <8, Alice loses (Bob wins). Continue this pattern. The total apples consumed after k full rounds (each round being Alice+Bob) is 10+9+8+7+... = sum of consecutive integers. The key is to find the maximum number of complete moves possible. Since the required removals strictly decrease, we can compute the winner by checking at each step. The given code snippet uses hard-coded thresholds: n<10 → false (lose), n<19 → true (win because after Alice's 10, Bob can't take 9), n<27 → false (after Alice 10, Bob 9, Alice can't take 8), n<34 → true, etc. The thresholds correspond to cumulative sums: after 1 move (10), after 2 moves (19), after 3 moves (27), after 4 moves (34), after 5 moves (40), after 6 moves (45), after 7 moves (49), after 8 moves (52), after 9 moves (54), after 10 moves (55). The pattern is that Alice wins when the total apples n is in ranges: [10,18], [27,33], [40,44], [49,51], [54,54]? Wait the given snippet only goes up to n<49 returns true? Actually snippet: n<49 return true; return true; So for n>=49 it always returns true. That matches: after 7 moves (49) it's Bob's turn? Let's compute: move1 Alice takes10 (n≥10), move2 Bob takes9 (n≥19), move3 Alice takes8 (n≥27), move4 Bob takes7 (n≥34), move5 Alice takes6 (n≥40), move6 Bob takes5 (n≥45), move7 Alice takes4 (n≥49), move8 Bob takes3 (n≥52). The snippet returns true for n<49 (i.e., after Alice's 4th move? Actually n<49 means after 7 moves? Let's check: for n<49, the last possible move is Alice's turn? The given snippet: n<10 false, n<19 true, n<27 false, n<34 true, n<40 false, n<45 true, n<49 false, return true. So for n=49, it returns true. That means at n=49, after 7 moves (Alice has taken 10+8+6+4 = 28? Wait let's simulate: Alice moves on odd turns: 1st (10), 3rd (8), 5th (6), 7th (4) => total Alice removed 28. Bob moves on even: 2nd (9), 4th (7), 6th (5) => total Bob removed 21. Total 49. After 7 moves, it's Bob's turn to take 3, but if n=49 exactly, remaining is 0, so Bob cannot take 3, Bob loses, Alice wins. So for n≥49, the pattern continues: if n<52 (Bob needs 3), Bob loses, Alice wins; if n<54 (Alice needs 2), Alice loses? Wait after Bob takes 3 (requires n≥52), remaining = n-52, then Alice needs 2, then Bob needs 1. The given snippet simplifies by returning true for n≥49 always, but that is incorrect for n=52? Let's check: n=52: Alice takes10 (rem42), Bob9 (33), Alice8 (25), Bob7 (18), Alice6 (12), Bob5 (7), Alice4 (3), Bob needs 3, takes 3 (rem0), then Alice needs 2 but no apples, Alice loses. So n=52 should return false, but snippet returns true. So the snippet is incorrect for n≥52. As a teaching assistant, we must create a task that is correct. So we should not rely on the incorrect snippet. We'll design the task to require implementing the correct game logic, which is to simulate the game. The correct approach: repeatedly subtract the required number (starting from 10 and decrementing) in order, alternating turns, and the first player unable to make a move loses. Since the sequence is fixed, we can simulate with a loop. The time complexity is O(number of moves), which is at most 10 moves because the required numbers go down to 1, after which if apples remain, the next required would be 0? Actually after taking 1, the next required would be 0, but you cannot take 0 apples – the game ends. So maximum number of moves is 10 (each turn subtracts a distinct positive integer from 10 down to 1). After that, if any apples remain, the player whose turn it is cannot move (since required would be 0, not allowed). So simulation is O(1) constant time (max 10 iterations). Space O(1). Edge cases: n=0, n=10, n=19, n=100 etc. The function should handle all non-negative integers.
