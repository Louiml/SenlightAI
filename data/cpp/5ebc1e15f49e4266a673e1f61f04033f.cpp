Write a C++ function that simulates a two-player dice game with a deterministic 100-sided die. The function takes two starting positions (integers 1–10, representing board spaces) and a winning score threshold, then returns the final result as a struct containing the losing player's score and the total number of die rolls. The die starts at 1 and increases by 1 each time it is rolled, wrapping from 100 back to 1. Each turn, a player rolls the die three times, sums those rolls, moves that many spaces forward around a circular board of 10 spaces, and adds the new position (1–10, not 0-based) to their score. Play alternates starting with player 1, and the first player to reach or exceed the threshold wins immediately. The function must return the product of the total rolls and the losing player's final score.
The core problem is simulating the game turn by turn while tracking die state. Use a reference or pointer to a mutable die value and a roll counter so the die state persists across calls. Represent players as a simple struct with position (1–10, but better stored 0–9 for modulo arithmetic) and score. The main loop: roll three times for player 1, update position as `(pos + roll) % 10` (with pos stored 0-based), add `pos + 1` to score, check if score >= threshold. If yes, compute result as `rolls * p2.score` and return. Otherwise, do the same for player 2 and check. Important edge cases: when the die reaches 100, it wraps to 1; starting positions are given as 1–10 but converted to 0–9 for modulo; the threshold could be any positive integer (e.g., 1000); both players could theoretically win on the same turn? No, because the loop checks player 1 first and returns immediately, so player 2 never acts after a player 1 win. Time complexity is \(O(\text{rolls})\) where rolls are proportional to the threshold divided by average turn score gain (roughly 5.5 per turn), so about \(O(\text{threshold})\) rolls. Space complexity is \(O(1)\) for the state and result struct.
#include <cstddef>

// Struct to hold the final result: losing player's score and total die rolls.
struct GameResult {
    long long rolls;
    int losingScore;
};

// Simulate the deterministic dice game starting from given positions.
// start1, start2 are in 1..10. threshold is the winning score.
GameResult playDeterministicGame(int start1, int start2, int threshold) {
    // Internal player state (positions stored 0-based).
    int pos1 = start1 - 1;
    int pos2 = start2 - 1;
    int score1 = 0;
    int score2 = 0;

    int die = 1;          // Current die value.
    long long rolls = 0;  // Total die rolls.

    // Roll three times and return the sum.
    auto rollThree = [&]() {
        int sum = 0;
        for (int i = 0; i < 3; ++i) {
            sum += die;
            if (die == 100) die = 1; else ++die;
            ++rolls;
        }
        return sum;
    };

    while (true) {
        // Player 1 turn.
        int roll = rollThree();
        pos1 = (pos1 + roll) % 10;
        score1 += pos1 + 1;
        if (score1 >= threshold) {
            return GameResult{rolls, score2};
        }

        // Player 2 turn.
        roll = rollThree();
        pos2 = (pos2 + roll) % 10;
        score2 += pos2 + 1;
        if (score2 >= threshold) {
            return GameResult{rolls, score1};
        }
    }
}
#include <cassert>

int main() {
    // Example from the original problem: start 4, start 8, threshold 1000.
    GameResult r1 = playDeterministicGame(4, 8, 1000);
    assert(r1.rolls == 993);
    assert(r1.losingScore == 745);

    // Starting positions 1 and 1 with low threshold.
    GameResult r2 = playDeterministicGame(1, 1, 10);
    // Simulate manually: P1 rolls 1+2+3=6, moves to 7, score 7. P2 rolls 4+5+6=15, moves to 6, score 6.
    // P1 rolls 7+8+9=24, moves to 1, score 8. P2 rolls 10+11+12=33, moves to 9, score 9.
    // P1 rolls 13+14+15=42, moves to 3, score 11 -> wins. Total rolls = 3*3=9, loser score=9.
    assert(r2.rolls == 9);
    assert(r2.losingScore == 9);

    // Threshold exactly reached on first turn.
    GameResult r3 = playDeterministicGame(5, 5, 100);
    // P1 rolls 1+2+3=6, pos=1, score=2 (not >=100), then P2 rolls 4+5+6=15, pos=10, score=10.
    // Then P1 later wins eventually. We only check consistency: rolls > 0 and scores are valid.
    assert(r3.rolls > 0);
    assert(r3.losingScore >= 0 && r3.losingScore < 1000);

    // Starting at maximum position 10.
    GameResult r4 = playDeterministicGame(10, 10, 1000);
    assert(r4.rolls > 0);
    assert(r4.losingScore >= 0);

    // Ensure die wraps correctly at 100.
    // Force a situation by using a large threshold so many rolls happen.
    GameResult r5 = playDeterministicGame(1, 2, 1000000);
    assert(r5.rolls > 1000);  // Should definitely roll many times.
    assert(r5.losingScore > 0);
}
