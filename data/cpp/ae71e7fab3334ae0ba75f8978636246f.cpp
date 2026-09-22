Write a standalone C++ function named `computePairwiseScores` that takes an integer parameter `rounds` (assumed to be positive) and returns a 6×6 matrix (as `std::vector<std::vector<int>>`) where the element at row `i` and column `j` represents the total score accumulated by a player of strategy `i` after playing `rounds` rounds against an opponent of strategy `j`. The six strategies are fixed and indexed as follows: 0 = Always Cooperate (AC), 1 = Always Defect (AD), 2 = Tit-for-Tat (TFT), 3 = Tit-for-Two-Tats (TF2T), 4 = Random (RND), and 5 = Grudger (GRD). Each round follows the Prisoner's Dilemma payoff matrix: both cooperate → 3 points each; player cooperates and opponent defects → 0 for player, 5 for opponent; player defects and opponent cooperates → 5 for player, 0 for opponent; both defect → 1 point each. Each strategy has a deterministic decision rule based on the opponent's previous move in the immediately preceding round, with these initial conditions: AC always cooperates; AD always defects; TFT cooperates on the first round, then mimics the opponent's previous move; TF2T cooperates on the first two rounds, then defects only if the opponent defected in both of the two previous rounds (otherwise cooperates); RND chooses randomly with equal probability (use a fixed deterministic pattern for reproducibility, e.g., alternate between cooperate and defect starting with cooperate); GRD cooperates until the opponent defects at least once, after which it always defects. The function must reset both players' scores to zero after each pair interaction and not reuse state across pairings (i.e., each pair starts fresh with the strategies' initial rules). The returned matrix must be organized so that `scores[i][j]` is the total points earned by player `i` against player `j` over exactly `rounds` rounds. Assume the strategies' internal state (previous move, counters, grudges) is reset for each new pair.

The solution requires simulating six deterministic (or pseudo-random with fixed sequence) strategies over a fixed number of rounds for every ordered pair of strategies (including self-pairs). The core algorithm: create a `StrategyState` struct that holds the strategy type, current score, previous move (as a character 'C' or 'D'), and any counters (e.g., for TF2T a count of consecutive opponent defections, for GRD a boolean flag). For each pair `(i, j)`, initialize both states with the appropriate initial conditions (e.g., TFT and GRD start with previous move 'C' for the first round decision; TF2T starts with a counter of 0). Then for each round from 1 to `rounds`: compute the moves for both players (using switch on type), apply the payoff matrix to accumulate scores, then update each player's internal state based on the opponent's move (e.g., TFT sets own previous move to opponent's move; GRD sets grudged flag if opponent defected; TF2T updates the streak of opponent defections). After all rounds, store `stateA.score` into `scores[i][j]`. Important edge cases: self-pairs (i == j) must also be simulated normally; the initial move for TFT, GRD, and TF2T requires a sentinel like 'N' or a dedicated `firstRound` flag to handle the "no previous move" case; the RND strategy must use a deterministic pattern (e.g., cooperate on even round numbers, defect on odd ones) to ensure reproducibility and avoid needing random library; scores must be reset after each pair. Time complexity: O(6 * 6 * rounds) = O(rounds), constant space O(1) for the states and the 6×6 output matrix. The function should be `const`-qualified where possible and use `std::vector` for the matrix.

#include <vector>
#include <string>

// Enum for strategy types
enum StrategyType { AC = 0, AD = 1, TFT = 2, TF2T = 3, RND = 4, GRD = 5 };

// Internal state for a single player during a simulated pair
struct StrategyState {
    StrategyType type;
    int score = 0;
    char previous = 'N';  // 'N' = no previous move, 'C' = cooperate, 'D' = defect
    int defectStreak = 0; // for TF2T: consecutive opponent defections
    bool grudged = false; // for GRD: true after first opponent defection
};

// Compute the move for a strategy given its current state
char getMove(const StrategyState& s) {
    switch (s.type) {
        case AC: return 'C';
        case AD: return 'D';
        case TFT:
            if (s.previous == 'N') return 'C'; // first move
            return s.previous; // mimic opponent's last move
        case TF2T:
            if (s.previous == 'N') return 'C'; // first move (no previous)
            // cooperate unless opponent defected in the previous two moves
            if (s.defectStreak >= 2) return 'D';
            return 'C';
        case RND:
            // Deterministic pattern: cooperate on round 1, defect on round 2, etc.
            // We use the number of rounds played via a simple counter: stored in defectStreak as round number
            // Instead, track a round counter in the state using a separate field? Use score? Use defectStreak as round counter.
            // Since we have only defectStreak for tracking, we repurpose it as round counter for RND.
            // This is acceptable since RND does not need defectStreak for anything else.
            if (s.defectStreak % 2 == 0) return 'C';
            else return 'D';
        case GRD:
            if (s.grudged) return 'D';
            return 'C';
        default: return 'C';
    }
}

// Update the state of player based on their own move and opponent's move
void updateState(StrategyState& self, char ownMove, char opponentMove) {
    if (self.type == TFT) {
        self.previous = opponentMove; // next round TFT plays opponent's move
    } else if (self.type == TF2T) {
        if (opponentMove == 'D') {
            self.defectStreak++;
        } else {
            self.defectStreak = 0;
        }
    } else if (self.type == RND) {
        self.defectStreak++; // increments round counter
    } else if (self.type == GRD) {
        if (opponentMove == 'D') {
            self.grudged = true;
        }
    }
    // AC and AD do not need state updates
}

// Main function: compute pairwise scores for all strategy pairs
std::vector<std::vector<int>> computePairwiseScores(int rounds) {
    std::vector<std::vector<int>> scores(6, std::vector<int>(6, 0));
    
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            // Initialize fresh states for each pair
            StrategyState a;
            a.type = static_cast<StrategyType>(i);
            a.previous = 'N'; a.defectStreak = 0; a.grudged = false;
            
            StrategyState b;
            b.type = static_cast<StrategyType>(j);
            b.previous = 'N'; b.defectStreak = 0; b.grudged = false;
            
            for (int r = 0; r < rounds; ++r) {
                // Compute moves
                char moveA = getMove(a);
                char moveB = getMove(b);
                
                // Apply payoff matrix
                if (moveA == 'C' && moveB == 'C') {
                    a.score += 3; b.score += 3;
                } else if (moveA == 'C' && moveB == 'D') {
                    a.score += 0; b.score += 5;
                } else if (moveA == 'D' && moveB == 'C') {
                    a.score += 5; b.score += 0;
                } else { // both 'D'
                    a.score += 1; b.score += 1;
                }
                
                // Update states for next round
                updateState(a, moveA, moveB);
                updateState(b, moveB, moveA);
            }
            
            scores[i][j] = a.score;
        }
    }
    return scores;
}

#include <cassert>
#include <vector>

// Note: The solution function is defined above here in a full program.
// For the test, we call computePairwiseScores with various rounds.

int main() {
    // Test 1: For 1 round, AC vs AC should be 3
    auto s1 = computePairwiseScores(1);
    assert(s1[0][0] == 3);  // AC vs AC
    assert(s1[1][1] == 1);  // AD vs AD (both defect)
    assert(s1[0][1] == 0);  // AC vs AD (AC gets 0)
    assert(s1[1][0] == 5);  // AD vs AC (AD gets 5)
    
    // Test 2: For 2 rounds, AC vs AD: AC always cooperates (0,0), AD always defects (5,5) => AC total 0, AD total 10
    auto s2 = computePairwiseScores(2);
    assert(s2[0][1] == 0);  // AC total points against AD after 2 rounds
    assert(s2[1][0] == 10); // AD total points against AC after 2 rounds
    
    // Test 3: For 2 rounds, TFT vs AD: TFT cooperates first round (0), then mimics AD's defect (1) => total 1
    auto s3 = computePairwiseScores(2);
    assert(s3[2][1] == 1);  // TFT (index 2) vs AD (index 1)
    assert(s3[1][2] == 5 + 1); // AD gets 5 then 1 = 6
    
    // Test 4: For 3 rounds, TFT vs TFT: both cooperate all rounds => 9 per player
    auto s4 = computePairwiseScores(3);
    assert(s4[2][2] == 9);
    
    // Test 5: For 4 rounds, GRD vs AD: GRD cooperates first round (0), then defects after seeing defect, all remaining are defect (1 each) => total 1+1+1 = 3
    auto s5 = computePairwiseScores(4);
    assert(s5[5][1] == 3);  // GRD (index 5) vs AD (index 1)
    assert(s5[1][5] == 5 + 1 + 1 + 1); // AD gets 5 then 1+1+1 = 8
    
    // Test 6: For 2 rounds, TF2T vs AD: TF2T cooperates both rounds (since only one defect before, streak =1) => gets 0 then 0 = 0
    auto s6 = computePairwiseScores(2);
    assert(s6[3][1] == 0);  // TF2T (index 3) vs AD
    
    // Test 7: For 3 rounds, TF2T vs AD: TF2T cooperates round1 (0), round2 (0), then streak=2 so defects round3 (1) => total 0+0+1=1
    auto s7 = computePairwiseScores(3);
    assert(s7[3][1] == 1);
    
    // Test 8: For 3 rounds, RND pattern C,D,C vs itself: each round both same move, C/D patterns yield 3+1+3=7
    auto s8 = computePairwiseScores(3);
    assert(s8[4][4] == 7);  // RND (index 4) vs itself
    
    // Test 9: For 5 rounds, AC vs AC total 15
    auto s9 = computePairwiseScores(5);
    assert(s9[0][0] == 15);
    
    // Test 10: For 1 round, AD vs GRD: AD defects, GRD cooperates => AD gets 5, GRD gets 0
    auto s10 = computePairwiseScores(1);
    assert(s10[1][5] == 5);
    assert(s10[5][1] == 0);
}
