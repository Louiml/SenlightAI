/*
Write a standalone C++ function named `threadPoolVote` that simulates the voting mechanism used in Stockfish's `ThreadPool::get_best_thread()` for selecting the best search thread. Given a vector of thread results, where each result contains a move identifier (integer), a score (integer), and a completed depth (integer), the function should return the move identifier of the winning thread according to the following rules: (1) For each thread, compute a vote value equal to `(score - minScore + 14) * completedDepth`, where `minScore` is the smallest score among all threads; (2) If any thread's score is at least 10000 (a tablebase win threshold), select the thread with the highest score, breaking ties by preferring the thread with the smaller completed depth (to pick the shortest mate); (3) Otherwise, accumulate votes per move identifier (summing votes from all threads with that move); select the thread whose move has the highest accumulated votes, and if there is a tie, select the thread with the higher individual score. The function should accept a `const std::vector<ThreadResult>&` where `ThreadResult` is a struct with fields `move`, `score`, and `depth`, and return the integer move of the winning thread. You may assume the vector is non-empty and every thread has a valid move. The thresholds are: `VALUE_TB_WIN = 10000`, `VALUE_TB_WIN_IN_MAX_PLY = 10000`, `VALUE_TB_LOSS_IN_MAX_PLY = -10000`. Handle edge cases such as all scores being sub-threshold, ties in votes, and negative scores.
*/

#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>
#include <cmath>

// Thresholds matching Stockfish constants
constexpr int VALUE_TB_WIN = 10000;
constexpr int VALUE_TB_WIN_IN_MAX_PLY = 10000;
constexpr int VALUE_TB_LOSS_IN_MAX_PLY = -10000;

// Result of a single search thread
struct ThreadResult {
    int move;   // move identifier
    int score;  // evaluation score
    int depth;  // completed search depth
};

// Simulate Stockfish's ThreadPool::get_best_thread() voting logic.
// Returns the move identifier of the winning thread.
// Assumes 'results' is non-empty.
int threadPoolVote(const std::vector<ThreadResult>& results) {
    // Find minimum score across all threads
    int minScore = results[0].score;
    for (const auto& r : results) {
        minScore = std::min(minScore, r.score);
    }

    std::map<int, int64_t> votes;   // move -> accumulated votes
    size_t bestIndex = 0;

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        // Accumulate vote for this thread's move
        votes[r.move] += (static_cast<int64_t>(r.score - minScore) + 14) * r.depth;

        // Determine current best thread
        const auto& best = results[bestIndex];

        // If best thread's score is a tablebase win/loss, pick highest score with smallest depth
        if (std::abs(best.score) >= VALUE_TB_WIN_IN_MAX_PLY) {
            // If current thread has higher score, or same score but smaller depth, update
            if (r.score > best.score ||
                (r.score == best.score && r.depth < best.depth)) {
                bestIndex = i;
            }
        } else {
            // If current thread reaches tablebase win, prefer it immediately
            if (r.score >= VALUE_TB_WIN_IN_MAX_PLY) {
                bestIndex = i;
            } else if (r.score > VALUE_TB_LOSS_IN_MAX_PLY) {
                // Compare votes for the moves of the two threads
                int64_t currentMoveVotes = votes[r.move];
                int64_t bestMoveVotes = votes[best.move];
                if (currentMoveVotes > bestMoveVotes ||
                    (currentMoveVotes == bestMoveVotes && r.score > best.score)) {
                    bestIndex = i;
                }
            }
            // If r.score <= VALUE_TB_LOSS_IN_MAX_PLY, it's a tablebase loss, skip (not preferred)
        }
    }

    return results[bestIndex].move;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with distinct moves and scores
    std::vector<ThreadResult> t1 = {{1, 50, 10}, {2, 100, 5}, {3, 80, 8}};
    assert(threadPoolVote(t1) == 2);

    // All negative scores, tie in votes by depth
    std::vector<ThreadResult> t2 = {{1, -5, 3}, {2, -1, 1}, {1, -10, 2}};
    // minScore = -10, votes: move1 = (5+14)*3 + (0+14)*2 = 57+28=85, move2 = (9+14)*1=23 => move1 wins
    assert(threadPoolVote(t2) == 1);

    // Tablebase win threshold: pick highest score even if votes lower
    std::vector<ThreadResult> t3 = {{1, 10001, 2}, {2, 10000, 10}};
    assert(threadPoolVote(t3) == 1);

    // Tie in scores at tablebase level: prefer smaller depth
    std::vector<ThreadResult> t4 = {{1, 15000, 8}, {2, 15000, 3}};
    assert(threadPoolVote(t4) == 2);

    // Tablebase loss: avoid selecting losing threads unless no other option
    std::vector<ThreadResult> t5 = {{1, -10001, 5}, {2, -500, 4}, {3, -600, 6}};
    // minScore=-10001, votes: move2 = (9501+14)*4=38060, move3=(9401+14)*6=56490, move1=0*5=0
    // best starts at 1, score -10001 is tablebase loss -> r.score > -10001? No.
    // r2: score -500 > -10001, votes 38060 > 0 => update best to 2
    // r3: score -600 > -10001, votes 56490 > 38060 => update best to 3
    assert(threadPoolVote(t5) == 3);

    // Single thread
    std::vector<ThreadResult> t6 = {{42, 100, 7}};
    assert(threadPoolVote(t6) == 42);

    // Tie in total votes between moves, higher individual score wins
    std::vector<ThreadResult> t7 = {{1, 10, 1}, {2, 20, 1}};
    // minScore=10, votes: move1=(0+14)*1=14, move2=(10+14)*1=24 => move2 wins by votes already
    assert(threadPoolVote(t7) == 2);

    // Depth = 0 for all threads
    std::vector<ThreadResult> t8 = {{5, 100, 0}, {6, -100, 0}};
    // All votes 0, selection by score: r0 is best initially, r1 score -100 > -10000 but votes tie 0, score lower => no change
    assert(threadPoolVote(t8) == 5);

    // Mixed tablebase and normal, tablebase win always preferred
    std::vector<ThreadResult> t9 = {{1, 10000, 1}, {2, 9999, 100}};
    // minScore=9999, votes: move1=(1+14)*1=15, move2=(0+14)*100=1400
    // best starts at 1 with score 10000 >= 10000, so tablebase win branch: r1 vs r2? r1 score == best score but smaller depth => update to 1
    // r2 score 9999 < best.score 10000 => no update
    assert(threadPoolVote(t9) == 1);

    // Negative depth is possible? Assume depth >= 0, but test edge with zero votes
    std::vector<ThreadResult> t10 = {{1, 0, 0}, {2, 0, 0}};
    // All votes 0, tie score, bestIndex remains 0
    assert(threadPoolVote(t10) == 1);

    return 0;
}

// The solution models the exact logic in the provided snippet. First, find the minimum score across all threads. Then iterate through all threads and compute an individual vote contribution for each thread as `(score - minScore + 14) * depth`. Maintain a `std::map<int, int64_t>` accumulating votes per move. Also track the `bestThread` index based on rules: (a) if any thread's score is ≥ 10000 or ≤ -10000 (indicating tablebase win/loss), then the selection prioritizes the highest score among such threads; if scores are equal, choose the one with smaller depth (to prefer shorter mate or longer defense); (b) otherwise, if a thread's score is ≥ 10000, immediately prefer it; else compare accumulated votes for the move of the candidate thread against the move of the current best thread. If the candidate's move has greater vote sum, or if the candidate's score is higher than the current best's score (when the current best's move has a tie), update bestThread. Important edge cases: all threads have negative scores; votes can overflow 32-bit so use `int64_t`; the winner may have a move with zero total votes if all threads have depth 0; if multiple threads have identical move and score, the first encountered is acceptable as long as behavior matches the described rules. Time complexity is O(n log n) due to `std::map` operations, where n is the number of threads; space complexity is O(n) for the map. If using an unordered_map, it would be O(n) average time, but std::map provides deterministic ordering.
