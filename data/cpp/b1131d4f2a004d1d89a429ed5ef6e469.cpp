// Write a C++ function `int countTwoOnOneWins(int a, int b, int c, int d)` that determines how many of the four possible pairings of two dice rolls (each die showing a positive integer value) result in the first player winning. The original game: player 1 rolls die A and die B; player 2 rolls die C and die D. In each round, one of player 1's dice is compared to one of player 2's dice (each die used exactly once in a round). The higher value wins that comparison. A comparison is a "win" for player 1 if their die value is strictly greater than the opponent's die value. The round result is a win for player 1 if player 1 wins more comparisons than player 2 (ties count for neither). Consider all four possible ways to pair the dice (the two dice of each player are distinct, e.g., for player 1 dice A and B, player 2 dice C and D, the possible pairings are: (A vs C, B vs D), (A vs D, B vs C), (B vs C, A vs D), (B vs D, A vs C)). Return the number of these four pairings (counting each pairing separately) for which player 1 wins the round. The function should handle all positive integer inputs.
// The problem is straightforward enumeration. For each of the four possible pairings, we need to evaluate two comparisons, count how many comparisons player 1 wins (`fst`) and how many player 2 wins (`sed`). If `fst > sed`, then that pairing is a win for player 1, so we increment the answer. There are exactly four distinct pairings, and we must consider each exactly once. The logic is symmetric: for pairing (x, y) comparing `a` with `c` and `b` with `d`, and also `a` with `d` and `b` with `c`, and also swapping the first player's dice order (which is equivalent to the other two pairings). Since the pairings are distinct, we implement four separate checks, resetting the counters between them. Edge cases: all values equal (no wins for either, so `fst` and `sed` are both 0, so no win), values that are equal in one comparison and greater in another, etc. The algorithm runs in constant time O(1) and uses O(1) auxiliary space, as there are a fixed number of comparisons and no data structures.
#include <algorithm>

// Count how many of the four possible pairings of dice A,B vs C,D
// result in player 1 winning the round (strictly more wins than losses).
int countTwoOnOneWins(int a, int b, int c, int d) {
    int wins = 0;

    // Pairing 1: (A vs C, B vs D)
    {
        int fst = 0, sed = 0;
        if (a > c) ++fst; else if (a < c) ++sed;
        if (b > d) ++fst; else if (b < d) ++sed;
        if (fst > sed) ++wins;
    }

    // Pairing 2: (A vs D, B vs C)
    {
        int fst = 0, sed = 0;
        if (a > d) ++fst; else if (a < d) ++sed;
        if (b > c) ++fst; else if (b < c) ++sed;
        if (fst > sed) ++wins;
    }

    // Pairing 3: (B vs C, A vs D) — same outcomes as pairing 2 but swapped rounds,
    // but still distinct pairing and counted separately.
    {
        int fst = 0, sed = 0;
        if (b > c) ++fst; else if (b < c) ++sed;
        if (a > d) ++fst; else if (a < d) ++sed;
        if (fst > sed) ++wins;
    }

    // Pairing 4: (B vs D, A vs C) — same as pairing 1 swapped, counted separately.
    {
        int fst = 0, sed = 0;
        if (b > d) ++fst; else if (b < d) ++sed;
        if (a > c) ++fst; else if (a < c) ++sed;
        if (fst > sed) ++wins;
    }

    return wins;
}
#include <cassert>

int countTwoOnOneWins(int a, int b, int c, int d);

int main() {
    // All equal: no wins
    assert(countTwoOnOneWins(1, 1, 1, 1) == 0);

    // A and B both strictly greater than C and D: all 4 pairings win
    assert(countTwoOnOneWins(5, 5, 1, 1) == 4);

    // A > C, but B < D; also A > D, B < C: each pairing has one win and one loss -> no wins
    assert(countTwoOnOneWins(10, 1, 5, 5) == 0);

    // A > C, B > D: two comparisons win, no losses -> all 4 pairings? Actually only pairing (A vs C, B vs D) and (B vs D, A vs C) give 2 wins, others give 1 win and 1 loss? Let's check: (A vs C) win, (B vs D) win => 2-0. (A vs D) win (10>5), (B vs C) loss (1<5) => 1-1. (B vs C) loss, (A vs D) win => 1-1. (B vs D) win, (A vs C) win => 2-0. So 2 wins.
    assert(countTwoOnOneWins(10, 2, 5, 1) == 2);

    // Mixed: A > C, B = D -> pairing (A vs C, B vs D) gives 1-0 (win), (A vs D) tie, (B vs C) loss => 0-1? Actually: (A vs D) 10>1 win, (B vs C) 2<5 loss => 1-1. (B vs C) loss, (A vs D) win => 1-1. (B vs D) tie, (A vs C) win => 1-0. So 2 wins? Let's compute: 1) (A vs C 10>5 win, B vs D 2=2 tie) => fst=1 sed=0 win. 2) (A vs D 10>1 win, B vs C 2<5 loss) => 1-1 no. 3) (B vs C 2<5 loss, A vs D 10>1 win) => 1-1 no. 4) (B vs D 2=2 tie, A vs C 10>5 win) => 1-0 win. So 2.
    assert(countTwoOnOneWins(10, 2, 5, 2) == 2);

    // One die dominates but other loses: A > C, A > D, B < C, B < D -> each pairing has one win one loss -> 0
    assert(countTwoOnOneWins(10, 1, 5, 5) == 0);

    // A > C, A > D, B > C, B < D -> pairing (A vs C, B vs D): win+loss =1-1; (A vs D, B vs C): win+win=2-0 => win; (B vs C, A vs D): win+win=2-0 => win; (B vs D, A vs C): loss+win=1-1 => no. So 2 wins.
    assert(countTwoOnOneWins(10, 4, 3, 5) == 2);

    return 0;
}
