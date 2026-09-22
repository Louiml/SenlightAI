/*
Write a C++ function `std::string bestRockPaperScissorsMove(const std::vector<std::string>& moves)` that, given a list of players' moves (each being "Rock", "Paper", or "Scissors"), returns a string in the format `"<Move> <Index>"` where `<Move>` is the single move (one of "Rock", "Paper", "Scissors") that, when played against all players sequentially in a circular manner starting from that player's index, yields the **maximum number of wins**. If multiple (move, starting index) combinations achieve the same maximum wins, choose the one with the **smallest starting index**; if still tied, choose the **smallest move** in the order Rock < Paper < Scissors. The scoring works as follows: starting at a chosen index `startIdx` (0‑based), you play a fixed move against the player at that index, then move to the next index (wrapping around after the last player), and continue until you have faced every player exactly once. A win gives +1 point, a draw gives 0 points, and a loss ends the game immediately (you stop and keep the score accumulated so far). The function must handle empty input? No, the input is guaranteed to be non-empty. Invalid strings in the input are treated as "Rock" (default). Return the best move string and the best starting index separated by a space, e.g., `"Paper 3"`.
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

// Convert string to move integer: 0=Rock, 1=Paper, 2=Scissors
int toMove(const std::string& s) {
    static const std::unordered_map<std::string, int> table = {
        {"Rock", 0}, {"Paper", 1}, {"Scissors", 2}
    };
    auto it = table.find(s);
    return (it != table.end()) ? it->second : 0; // default Rock
}

// Returns true if move `a` beats move `b`
bool beats(int a, int b) {
    return (a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1);
}

// Determine best (move, starting index) for maximum score
std::string bestRockPaperScissorsMove(const std::vector<std::string>& moves) {
    int n = static_cast<int>(moves.size());
    std::vector<int> nums(n);
    for (int i = 0; i < n; ++i) nums[i] = toMove(moves[i]);

    int bestScore = -1;
    int bestIdx = -1;
    int bestMove = -1;

    for (int start = 0; start < n; ++start) {
        for (int myMove = 0; myMove <= 2; ++myMove) {
            int score = 0;
            bool lost = false;
            for (int step = 0; step < n && !lost; ++step) {
                int idx = (start + step) % n;
                int opp = nums[idx];
                if (beats(myMove, opp)) {
                    ++score;
                } else if (myMove != opp) {
                    // loss
                    lost = true;
                }
                // draw: nothing
            }
            // Update best if better, or tie-break by smaller index then smaller move
            if (score > bestScore ||
                (score == bestScore && (start < bestIdx ||
                 (start == bestIdx && myMove < bestMove)))) {
                bestScore = score;
                bestIdx = start;
                bestMove = myMove;
            }
        }
    }

    static const std::vector<std::string> names = {"Rock", "Paper", "Scissors"};
    return names[bestMove] + " " + std::to_string(bestIdx);
}
#include <cassert>
#include <string>
#include <vector>

// Function declaration from solution (already included above in a real project)
std::string bestRockPaperScissorsMove(const std::vector<std::string>&);

int main() {
    // Basic case: one player
    assert(bestRockPaperScissorsMove({"Rock"}) == "Paper 0"); // Paper beats Rock, score 1

    // Two players: Rock, Paper
    // Start 0: Paper vs Rock(win), vs Paper(draw) => 1
    // Start 0: Rock vs Rock(draw), vs Paper(lose) => 0
    // Start 1: Paper vs Paper(draw), vs Rock(win) => 1; tie with start0, choose smaller index 0
    assert(bestRockPaperScissorsMove({"Rock", "Paper"}) == "Paper 0");

    // Three players all Rock: any move wins vs Rock yields score 3; choose Rock (smallest move), index 0
    assert(bestRockPaperScissorsMove({"Rock", "Rock", "Rock"}) == "Rock 0");

    // Mixed: Rock, Scissors, Paper
    // Try start 0 with Scissors: vs Rock(lose, stop) => 0
    // start 0 with Paper: vs Rock(win), vs Scissors(lose) => 1
    // start 1 with Rock: vs Scissors(win), vs Paper(lose) => 1
    // start 2 with Scissors: vs Paper(win), vs Rock(lose) => 1
    // Tie at score 1: smallest index 0, move Paper (1) vs Rock (0) =>? Wait we compare all: start0 Paper score1, start1 Rock score1, start2 Scissors score1. Tie: smallest index 0, among moves at index0: Paper is move1, Rock move0 (but Rock at index0 gives score0), so only Paper at index0 gives score1 => "Paper 0"
    assert(bestRockPaperScissorsMove({"Rock", "Scissors", "Paper"}) == "Paper 0");

    // Invalid string defaults to Rock
    // Two players: "Invalid", "Rock" => same as two Rocks => "Rock 0"
    assert(bestRockPaperScissorsMove({"Invalid", "Rock"}) == "Rock 0");

    // Edge: loss immediately at start
    // One player Paper: Rock vs Paper loses at first => score0; Paper vs Paper draw =>0; Scissors vs Paper win =>1 => "Scissors 0"
    assert(bestRockPaperScissorsMove({"Paper"}) == "Scissors 0");

    // Larger tie-breaking: 2 players: "Scissors", "Rock"
    // start0: Rock vs Scissors(win), vs Rock(draw) =>1
    // start0: Paper vs Scissors(lose) =>0
    // start0: Scissors vs Scissors(draw), vs Rock(win) =>1
    // start1: Rock vs Rock(draw), vs Scissors(win) =>1
    // start1: Paper vs Rock(win), vs Scissors(lose) =>1
    // start1: Scissors vs Rock(lose) =>0
    // Scores: (start0, Rock)=1, (start0, Scissors)=1, (start1, Rock)=1, (start1, Paper)=1
    // Choose smallest index: 0; then smallest move at index0: Rock (0) vs Scissors (2) => Rock
    assert(bestRockPaperScissorsMove({"Scissors", "Rock"}) == "Rock 0");

    return 0;
}
// The problem requires simulating a circular scan for each possible starting index and each possible move (Rock, Paper, Scissors). For each starting index `i` (0 to n-1) and each move `m`, we iterate through players starting at index `i` and moving forward (with wrap‑around) until either we have visited all n players or we lose. For each opponent, we compare moves: Rock beats Scissors, Scissors beats Paper, Paper beats Rock. If we win, we increment score and continue; if draw, we continue without changing score; if loss, we break and do not continue further. We track the best (score, then smallest starting index, then smallest move). Time complexity: O(n^2 * 3) = O(n^2) because for each starting index (n) and each move (3) we do up to n comparisons. Space complexity: O(1) aside from input storage and output string.
//
// Edge cases: a player may have an unknown move that defaults to Rock; a loss at the very first player yields score 0; if the entire scan results in no losses, score equals number of wins; ties are resolved by index then move order.
