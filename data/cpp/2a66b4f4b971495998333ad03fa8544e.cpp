// Write a C++ function that takes four integers representing the health values of two players' cards in a two-round game: `a1`, `a2` are Suneet's card values, and `b1`, `b2` are Slavic's card values. In each round, both players simultaneously reveal one of their cards (each card used exactly once), and the player with the higher value wins that round; ties count as a win for neither player. A player "wins the game" if they win strictly more rounds than the opponent. Suneet can choose the order in which he plays his two cards, but Slavic's order is fixed as `b1` then `b2`. However, each of the four possible assignments of Suneet's cards to the two rounds is equally likely (since Slavic's order is unknown to Suneet in a symmetric scenario), so we must count how many of these four possible pairings result in Suneet winning the game overall. Return that count as an integer between 0 and 4 inclusive. The input integers are between 1 and 10^9, and you should not assume any ordering among them.
// The problem reduces to considering all permutations of Suneet's two cards across the two rounds, while Slavic's cards are always played in the order `b1` then `b2`. There are exactly two distinct orderings for Suneet's cards: (a1,a2) and (a2,a1). But since we are told "each of the four possible assignments" is considered, we interpret this as the four assignments where each of Suneet's cards is paired with each of Slavic's cards in a round, but with the constraint that each round uses one card from each player and each card is used exactly once. That yields exactly two valid orderings: (a1 vs b1, a2 vs b2) and (a2 vs b1, a1 vs b2). However, the original code also considers (a1 vs b2, a2 vs b1) and (a2 vs b2, a1 vs b1). Those are actually the same two pairings but with the order of rounds swapped, which does not change the game outcome because the total number of round wins is independent of round order. So really there are only two distinct assignments, but the problem statement as given counts four "ways to flip the cards" by also swapping which of Suneet's cards is considered first in the round order. To match the original logic, we count all 4 permutations where we choose an ordering for Suneet's cards (2 ways) and an ordering for Slavic's cards (2 ways), but since Slavic's order is fixed as (b1,b2) in the problem statement, only two are valid. However, the provided code counts 4 distinct conditions that are effectively duplicates. For a self-contained task, we should count the two genuinely different pairings: (a1,a2) against (b1,b2) and (a2,a1) against (b1,b2). For each pairing, determine if Suneet wins more rounds than Slavic (strictly greater wins). Ties do not count for either. Sum the number of pairings where Suneet wins. Complexity: O(1) time, O(1) space. Edge cases: equal values produce a tie and do not contribute to either player's win count; if both rounds are ties, Suneet wins 0 rounds and does not win the game.
#include <algorithm>

// Count how many of the two possible orderings of Suneet's cards
// (a1,a2) vs (b1,b2) and (a2,a1) vs (b1,b2) result in Suneet
// winning more rounds than Slavic. Each pairing uses all cards exactly once.
// Returns an integer between 0 and 2 inclusive.
int countWinningPairings(long long a1, long long a2, long long b1, long long b2) {
    auto winsGame = [](long long s1, long long s2, long long o1, long long o2) {
        int suneetWins = 0;
        int slavicWins = 0;
        // Round 1: s1 vs o1
        if (s1 > o1) ++suneetWins;
        else if (s1 < o1) ++slavicWins;
        // Round 2: s2 vs o2
        if (s2 > o2) ++suneetWins;
        else if (s2 < o2) ++slavicWins;
        return suneetWins > slavicWins;
    };

    int count = 0;
    // Pairing 1: (a1,a2) against (b1,b2)
    if (winsGame(a1, a2, b1, b2)) ++count;
    // Pairing 2: (a2,a1) against (b1,b2)
    if (winsGame(a2, a1, b1, b2)) ++count;
    return count;
}
#include <cassert>

int main() {
    // No winning pairings: both cards strictly less
    assert(countWinningPairings(1, 2, 5, 6) == 0);
    // One pairing wins: (a1,a2) vs (b1,b2) -> 5>4 and 3<6 gives 1 win vs 1 tie? Actually 5>4 win, 3<6 loss -> 1 win, 1 loss -> not a win. (a2,a1) -> 3<4 loss, 5>6 loss -> 0 wins. So 0.
    // Better test: a1=5,a2=4,b1=3,b2=2 -> both pairings give 2 wins -> 2
    assert(countWinningPairings(5, 4, 3, 2) == 2);
    // a1=5,a2=1,b1=3,b2=2 -> (5,1) vs (3,2): round1 5>3 win, round2 1<2 loss -> 1 win 1 loss not win. (1,5) vs (3,2): round1 1<3 loss, round2 5>2 win -> 1 win 1 loss not win. So 0.
    assert(countWinningPairings(5, 1, 3, 2) == 0);
    // a1=5,a2=5,b1=3,b2=3 -> both rounds Suneet wins -> both pairings -> 2
    assert(countWinningPairings(5, 5, 3, 3) == 2);
    // a1=10,a2=1,b1=1,b2=10 -> (10,1) vs (1,10): round1 win, round2 loss -> 1-1 not win. (1,10) vs (1,10): round1 tie, round2 tie -> 0-0 not win. So 0.
    assert(countWinningPairings(10, 1, 1, 10) == 0);
    // a1=6,a2=7,b1=5,b2=4 -> (6,7) vs (5,4): both wins -> win. (7,6) vs (5,4): both wins -> win. So 2.
    assert(countWinningPairings(6, 7, 5, 4) == 2);
    // Large values, just one pairing wins: a1=1,a2=100,b1=50,b2=1 -> (1,100) vs (50,1): round1 loss, round2 win -> 1-1 not win. (100,1) vs (50,1): round1 win, round2 tie -> 1-0 win. So 1.
    assert(countWinningPairings(1, 100, 50, 1) == 1);
    // All equal -> no one wins any round -> 0
    assert(countWinningPairings(3, 3, 3, 3) == 0);
    // a1=2,a2=2,b1=1,b2=3 -> (2,2) vs (1,3): round1 win, round2 loss -> 1-1 not win. (2,2) vs (1,3): same as above because switching equal cards gives identical pairings. Actually both pairings are identical since a1==a2, so both produce the same result -> count 0.
    assert(countWinningPairings(2, 2, 1, 3) == 0);
    return 0;
}
