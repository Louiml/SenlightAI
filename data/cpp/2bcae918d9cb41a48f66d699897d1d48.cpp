// Write a C++ function that simulates a card-drawing game between two players. The game uses 9 cards with values, and each card has a corresponding "base" value for Player 2. The function takes two vectors of 9 integers (the card values for Player 1 and the base values for Player 2). Player 1's cards are shuffled in all possible permutations (each equally likely). For each permutation, pairs are formed by matching Player 1's card at position i with Player 2's base value at position i. For each pair, if Player 1's card is less than Player 2's base, Player 1 gains the sum of both values; if greater, Player 2 gains the sum. After all pairs, if Player 1's total is less than Player 2's total, that permutation is a win for Player 2. The function should return a pair of doubles: the probability that Player 2 wins and the probability that Player 1 wins (or ties, if applicable), each rounded to 5 decimal places (as exact probabilities, not just the ratio). Since the sum of the two probabilities should be exactly 1 (because ties are impossible when all values are distinct, but if ties occur, they are counted as Player 1 wins in the original code), define that a permutation is a win for Player 2 only if Player 2's total > Player 1's total; otherwise, it's a win for Player 1. The input vectors may contain duplicate values, but all values are positive integers. The function signature is `std::pair<double, double> cardGameProbability(const std::vector<int>& player1, const std::vector<int>& player2)`. The output should be the probability of Player 2 winning, and the probability of Player 1 winning, each as a double with 5-decimal-place precision (but the function returns raw doubles; the test code will compare with approximately 1e-6 tolerance).
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is declared above (not repeated here).

int main() {
    // Example 1: All equal on both sides -> only one permutation, all ties -> Player 1 wins.
    std::vector<int> p1_equal = {5,5,5,5,5,5,5,5,5};
    std::vector<int> p2_equal = {5,5,5,5,5,5,5,5,5};
    auto res1 = cardGameProbability(p1_equal, p2_equal);
    assert(std::fabs(res1.first - 0.0) < 1e-9);
    assert(std::fabs(res1.second - 1.0) < 1e-9);

    // Example 2: Player 1 always smaller than Player 2 -> Player 1 always wins.
    std::vector<int> p1_small = {1,2,3,4,5,6,7,8,9};
    std::vector<int> p2_big =   {9,9,9,9,9,9,9,9,9};
    auto res2 = cardGameProbability(p1_small, p2_big);
    assert(std::fabs(res2.first - 0.0) < 1e-9);
    assert(std::fabs(res2.second - 1.0) < 1e-9);

    // Example 3: Player 1 always greater than Player 2 -> Player 2 always wins.
    std::vector<int> p1_big = {9,9,9,9,9,9,9,9,9};
    std::vector<int> p2_small = {1,1,1,1,1,1,1,1,1};
    auto res3 = cardGameProbability(p1_big, p2_small);
    assert(std::fabs(res3.first - 1.0) < 1e-9);
    assert(std::fabs(res3.second - 0.0) < 1e-9);

    // Example 4: Simple case where order matters, using two unique values.
    // player1 = {1,2}, player2 = {2,1} but we need 9 elements, so repeat pattern.
    std::vector<int> p1_mix = {1,2,1,2,1,2,1,2,1};
    std::vector<int> p2_mix = {2,1,2,1,2,1,2,1,2};
    auto res4 = cardGameProbability(p1_mix, p2_mix);
    // The problem is symmetric here: each permutation is equally likely, but due to duplicates,
    // the number of unique permutations is less. We can test that the sum is 1.
    assert(std::fabs((res4.first + res4.second) - 1.0) < 1e-9);
    // Also verify that the probability is not trivially 0 or 1.
    assert(res4.first > 0.0 && res4.first < 1.0);

    // Example 5: Known small case: player1 = {1,2,3}, player2 = {3,2,1} (but we need 9, so repeat).
    // We'll just test correctness of a brute-force alternative for 3 cards by using only first 3? Not possible given the fixed size.
    // Instead, verify that for the given input, the sum of probabilities is exactly 1 and each is within [0,1].
    std::vector<int> p1 = {3,1,4,1,5,9,2,6,5};
    std::vector<int> p2 = {2,7,1,8,2,8,1,8,2};
    auto res5 = cardGameProbability(p1, p2);
    assert(res5.first >= 0.0 && res5.first <= 1.0);
    assert(res5.second >= 0.0 && res5.second <= 1.0);
    assert(std::fabs((res5.first + res5.second) - 1.0) < 1e-9);

    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

// Compute the probability that Player 2 wins and Player 1 wins given two vectors of 9 integers.
// Player 1's cards are permuted uniformly; Player 2's values are fixed in order.
std::pair<double, double> cardGameProbability(const std::vector<int>& player1, const std::vector<int>& player2) {
    // Ensure we have exactly 9 elements as per the problem statement.
    const std::size_t n = 9;
    std::vector<int> cards = player1;  // copy to sort and permute
    std::sort(cards.begin(), cards.end());

    long long num_wins_player2 = 0;
    long long total_permutations = 0;

    do {
        long long score1 = 0;
        long long score2 = 0;
        for (std::size_t i = 0; i < n; ++i) {
            if (cards[i] < player2[i]) {
                score1 += cards[i] + player2[i];
            } else if (cards[i] > player2[i]) {
                score2 += cards[i] + player2[i];
            }
            // In case of equality, neither score changes (ties are not counted as wins for player2).
        }
        if (score2 > score1) {
            ++num_wins_player2;
        }
        ++total_permutations;
    } while (std::next_permutation(cards.begin(), cards.end()));

    double prob2 = static_cast<double>(num_wins_player2) / static_cast<double>(total_permutations);
    double prob1 = 1.0 - prob2;
    return {prob2, prob1};
}
// The core idea is to enumerate all distinct permutations of Player 1's cards. Since `next_permutation` only generates unique permutations when the input is sorted (and duplicates are ignored), we first sort `player1` to ensure we don't overcount permutations with duplicate values. For each permutation, we compute Player 1's total score (sum of `a+b` when `a < b`) and Player 2's total (sum of `a+b` when `a > b`). If Player 2's total is greater than Player 1's total, we count that as a win for Player 2. The total number of permutations is `cnt` (the number of unique permutations), which is less than or equal to 9! = 362880. The probability for Player 2 is `num_wins / cnt`, and for Player 1 it's `1 - probability_2`. Edge cases: if all Player 1 values are identical, there is only one permutation; also, if ties occur (sums equal), the original code treats `x<y` as Player 2 win, otherwise it's a Player 1 win (even if tie). The algorithm is straightforward brute-force with O(9! * 9) time in the worst case, which is about 3.26 million operations, well within limits. Space complexity is O(9) for the vectors, plus the permutation generation overhead.
