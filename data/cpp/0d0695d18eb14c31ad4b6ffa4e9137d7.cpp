// Write a C++ function `playingWithDice` that takes two integers `a` and `b`, each representing the numbers chosen by two players on a standard six-sided die (values 1 through 6). The function must return a `std::array<int,3>` (or `std::vector<int>` of size 3) containing three counts: the number of outcomes (1 through 6) where rolling that value makes the first player closer to `a` than to `b` (strictly), the number of outcomes where the distances are equal (a tie), and the number of outcomes where the second player is strictly closer. Use the absolute difference as the distance metric. The input integers are guaranteed to be within 1..6, but your function should handle any integers safely (e.g., by treating values outside 1..6 as if they were within 1..6? No—assume valid input). The function must not print anything; it must only return the three counts.
// The problem is a direct brute‑force enumeration of all possible die outcomes, which are exactly six fixed values: 1, 2, 3, 4, 5, 6. For each outcome `x`, compute `da = abs(x - a)` and `db = abs(x - b)`. If `da < db`, increment the "first player wins" counter; if `da > db`, increment the "second player wins" counter; otherwise (equal), increment the "tie" counter. Since the number of outcomes is constant (6), the time complexity is O(1) and the space complexity is O(1). Edge cases include when `a == b` (then all outcomes are ties, because the distances are identical) and when the two values differ (some outcomes favor one player, some the other, and possibly none or some ties if the distance difference is even). The solution is deterministic and does not require any sorting or specialized data structures.
#include <array>
#include <cstdlib>

// Return {first_wins, ties, second_wins} for all die outcomes 1..6.
std::array<int,3> playingWithDice(int a, int b) {
    std::array<int,3> counts = {0, 0, 0};
    for (int outcome = 1; outcome <= 6; ++outcome) {
        const int distA = std::abs(outcome - a);
        const int distB = std::abs(outcome - b);
        if (distA < distB) {
            ++counts[0];
        } else if (distA > distB) {
            ++counts[2];
        } else {
            ++counts[1];
        }
    }
    return counts;
}
#include <cassert>

int main() {
    // Basic distinct numbers
    auto res1 = playingWithDice(1, 6);
    assert(res1[0] == 5 && res1[1] == 0 && res1[2] == 1);

    // Same numbers: all outcomes are ties
    auto res2 = playingWithDice(3, 3);
    assert(res2[0] == 0 && res2[1] == 6 && res2[2] == 0);

    // Distances differ by an odd amount: no ties
    auto res3 = playingWithDice(2, 5);
    assert(res3[0] == 4 && res3[1] == 0 && res3[2] == 2);

    // Distances differ by an even amount: exactly one tie
    auto res4 = playingWithDice(1, 3);
    assert(res4[0] == 2 && res4[1] == 1 && res4[2] == 3);

    // Adjacent values: no ties, first wins more for lower a
    auto res5 = playingWithDice(4, 5);
    assert(res5[0] == 3 && res5[1] == 0 && res5[2] == 3);

    // Boundary values: 1 vs 2
    auto res6 = playingWithDice(1, 2);
    assert(res6[0] == 1 && res6[1] == 0 && res6[2] == 5);
}
