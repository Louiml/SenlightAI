// Write a C++ function named `determineDiceGameWinner` that simulates a two-player dice game over `n` rounds, where each round both players roll a fair six-sided die. The function must take an integer `n` (number of rounds), generate `n` random rolls for Player 1 and `n` random rolls for Player 2 using a provided random number generator (passed as `std::mt19937&`), count the net score by incrementing for each round Player 1 wins (roll > opponent), decrementing for Player 2 wins, and ignoring ties. The function must return a string: `"Player 1 wins"` if the net score is positive, `"Player 2 wins"` if negative, and `"Draw"` if zero. The function must also store the rolls into two output arrays (provided as `std::vector<int>&` parameters) for later inspection. Ensure the function is deterministic given the same RNG state, handles `n = 0` gracefully (returning `"Draw"`), and does not print anything.
// The solution simulates the dice rolls using a uniform distribution over `[1, 6]` from the provided RNG. For each round `i`, generate `roll1` and `roll2`, store them in the corresponding vectors, then update `score` by `+1` if `roll1 > roll2`, `-1` if `roll2 > roll1`, and `0` on ties. After the loop, compare `score` to zero to decide the result. Edge cases: `n = 0` means both vectors remain empty, score stays zero, so return `"Draw"`. The function must initialize the vectors to size `n` before storing; if vectors are passed with different sizes, resize them to `n` to ensure safety. Time complexity is `O(n)` because we iterate exactly `n` times, each step constant work. Space complexity is `O(n)` due to storing the two vectors, which is required by the task. The RNG is passed by reference so that repeated calls produce different sequences if the caller reuses the same RNG; using `std::uniform_int_distribution` ensures correct uniform distribution modulo bias is avoided.
#include <string>
#include <vector>
#include <random>

// Simulates n rounds of a two-player dice game.
// Returns "Player 1 wins", "Player 2 wins", or "Draw".
// Rolls are stored in player1Rolls and player2Rolls (both resized to n).
std::string determineDiceGameWinner(int n, std::mt19937& rng,
                                    std::vector<int>& player1Rolls,
                                    std::vector<int>& player2Rolls) {
    player1Rolls.resize(n);
    player2Rolls.resize(n);
    std::uniform_int_distribution<int> dist(1, 6);
    int score = 0;
    for (int i = 0; i < n; ++i) {
        int roll1 = dist(rng);
        int roll2 = dist(rng);
        player1Rolls[i] = roll1;
        player2Rolls[i] = roll2;
        if (roll1 > roll2) {
            ++score;
        } else if (roll2 > roll1) {
            --score;
        }
    }
    if (score > 0) return "Player 1 wins";
    if (score < 0) return "Player 2 wins";
    return "Draw";
}
#include <cassert>
#include <vector>
#include <random>
#include <string>

// Function declaration from solution
std::string determineDiceGameWinner(int n, std::mt19937& rng,
                                    std::vector<int>& player1Rolls,
                                    std::vector<int>& player2Rolls);

int main() {
    // Test 1: n = 0 -> Draw, empty vectors
    std::mt19937 rng1(12345);
    std::vector<int> p1, p2;
    assert(determineDiceGameWinner(0, rng1, p1, p2) == "Draw");
    assert(p1.empty() && p2.empty());

    // Test 2: Deterministic with same RNG state -> same outcome and rolls
    std::mt19937 rng2a(99), rng2b(99);
    std::vector<int> a1, a2, b1, b2;
    std::string res1 = determineDiceGameWinner(5, rng2a, a1, a2);
    std::string res2 = determineDiceGameWinner(5, rng2b, b1, b2);
    assert(res1 == res2);
    assert(a1 == b1);
    assert(a2 == b2);

    // Test 3: Verify rolls are in [1,6] and vectors size equals n
    std::mt19937 rng3(7);
    std::vector<int> c1, c2;
    determineDiceGameWinner(10, rng3, c1, c2);
    assert(c1.size() == 10 && c2.size() == 10);
    for (int i = 0; i < 10; ++i) {
        assert(c1[i] >= 1 && c1[i] <= 6);
        assert(c2[i] >= 1 && c2[i] <= 6);
    }

    // Test 4: Edge case n=1, force win for player1 using fixed RNG? Not feasible, but check result is one of three strings
    std::mt19937 rng4(42);
    std::vector<int> d1, d2;
    std::string res = determineDiceGameWinner(1, rng4, d1, d2);
    assert(res == "Player 1 wins" || res == "Player 2 wins" || res == "Draw");

    // Test 5: Large n to ensure no crash, checks only sizes
    std::mt19937 rng5(2024);
    std::vector<int> e1, e2;
    determineDiceGameWinner(10000, rng5, e1, e2);
    assert(e1.size() == 10000 && e2.size() == 10000);
}
