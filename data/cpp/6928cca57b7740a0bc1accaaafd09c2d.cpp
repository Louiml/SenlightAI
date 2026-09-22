// Write a C++ function `int deceitfulWarScore(vector<double> naomi, vector<double> ken)` that takes two vectors of equal length containing the masses of blocks owned by Naomi and Ken in the game "Deceitful War" (a variant of the classic game War). The function must return the maximum number of points Naomi can score by playing optimally under the following rule: In each round, Naomi secretly chooses a block weight, Ken sees it and then chooses a block of his own. The round is won by the player whose block is heavier. However, Naomi may choose to *lie* about her block's weight (she can tell Ken any positive real number, not necessarily her block's actual weight) before Ken makes his choice. Ken's choice is based on the *told* weight, but the actual comparison during the round uses the *real* weights. Naomi knows all of Ken's block weights before the game starts. All blocks are distinct reals, and the vectors may be unsorted and contain any positive real numbers. Return the maximum number of rounds Naomi can win.

#include <cassert>
#include <vector>

// Function prototype (include the solution code above this test in a full program)
int deceitfulWarScore(std::vector<double> naomi, std::vector<double> ken);

int main() {
    // Basic cases
    assert(deceitfulWarScore({0.5, 0.1, 0.2}, {0.4, 0.3, 0.6}) == 1);
    assert(deceitfulWarScore({1.0, 2.0, 3.0}, {0.5, 1.5, 2.5}) == 3);
    assert(deceitfulWarScore({0.1, 0.2}, {0.3, 0.4}) == 0);
    assert(deceitfulWarScore({0.9, 0.8, 0.7}, {0.6, 0.5, 0.4}) == 3);
    // Empty vectors
    assert(deceitfulWarScore({}, {}) == 0);
    // Unsorted input
    assert(deceitfulWarScore({0.3, 0.1, 0.2}, {0.5, 0.4, 0.6}) == 0);
    // Larger mixed case
    assert(deceitfulWarScore({0.1, 0.5, 0.9, 1.0}, {0.2, 0.3, 0.4, 0.8}) == 3);
    // All distinct and equal count, but Naomi strictly dominates
    assert(deceitfulWarScore({10.0, 11.0, 12.0}, {1.0, 2.0, 3.0}) == 3);
    // Single element
    assert(deceitfulWarScore({5.0}, {3.0}) == 1);
    assert(deceitfulWarScore({3.0}, {5.0}) == 0);
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum number of rounds Naomi can win in Deceitful War.
// Assumes both vectors have the same size and all values are positive reals.
int deceitfulWarScore(std::vector<double> naomi, std::vector<double> ken) {
    if (naomi.empty()) return 0;

    std::sort(naomi.begin(), naomi.end());
    std::sort(ken.begin(), ken.end());

    int wins = 0;
    int i = 0;               // index for Naomi's lightest unused block
    int j = 0;               // index for Ken's lightest unused block
    int kenHeavy = static_cast<int>(ken.size()) - 1; // index for Ken's heaviest unused block

    while (i < static_cast<int>(naomi.size()) && j <= kenHeavy) {
        if (naomi[i] > ken[j]) {
            // Naomi can beat Ken's lightest block, take the win.
            ++wins;
            ++i;
            ++j;
        } else {
            // Naomi's lightest cannot beat Ken's lightest, so sacrifice it
            // against Ken's heaviest block by claiming it is just barely lighter.
            ++i;
            --kenHeavy;
        }
    }
    return wins;
}

// The key insight is that Naomi can exploit her ability to lie to force Ken to waste his heavy blocks on rounds she will lose anyway. The optimal strategy is to sort both vectors in ascending order. There are two symmetric phases conceptually, but the classic solution is to simulate Naomi's best play using a two-pointer approach on the sorted arrays. Think of it as Naomi trying to maximize wins:  
// - Sort both arrays ascending.  
// - Use two pointers: `i` for Naomi's lightest block and `j` for Ken's lightest block.  
// - If Naomi's lightest can beat Ken's lightest (i.e., `naomi[i] > ken[j]`), then she uses that block to win that round (score++, `i++`, `j++`).  
// - If Naomi's lightest cannot beat Ken's lightest, she sacrifices her lightest block against Ken's heaviest block (since she can claim it is just barely lighter than Ken's heaviest, forcing Ken to play his heaviest block, wasting it). So `i++` and `j--` (move Ken's pointer from the heaviest side).  
// This greedy works because sacrificing the smallest block against the largest opponent block is always optimal. The total time complexity is O(n log n) due to sorting, with O(n) for the two-pointer scan and O(1) auxiliary space if we sort in-place. Edge cases: empty vectors (return 0), all blocks equal (but all blocks are distinct, so no equal weights), and the case where Naomi's smallest is already larger than Ken's smallest — then she can win every round. The algorithm handles duplicates in the sense that distinct is guaranteed but the comparison `>` is strict.
