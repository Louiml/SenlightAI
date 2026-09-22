/*
Write a C++ function named `evaluatePokerHand` that takes a 5x2 integer matrix representing a poker hand (each row is a card: column 0 is the suit index 0-3, column 1 is the rank/face index 0-12, where 0 represents Ace, 1-9 represent 2-10, 10=Jack, 11=Queen, 12=King) and returns an integer rank from 0 to 8 according to standard poker hand ranking: 8 for straight flush, 7 for four of a kind, 6 for full house, 5 for flush (all same suit but not straight), 4 for straight (sequential ranks but not all same suit), 3 for three of a kind, 2 for two pairs, 1 for one pair, and 0 for high card. The hand is guaranteed to consist of distinct cards (no duplicates) and the input is a valid 5-card hand. Implement all helper functions internally (do not rely on external functions). The function should handle Ace as both low (before 2) and high (after King) when detecting straights, but for rank evaluation Ace is always considered the highest for tie-breaking purposes (though tie-breaking is not needed here, only classification). The function must not modify the input matrix and should use appropriate `const` correctness.
*/
#include <array>
#include <algorithm>
#include <vector>

int evaluatePokerHand(const int hand[5][2]) {
    // Count rank frequencies (ranks 0-12)
    int freq[13] = {0};
    for (int i = 0; i < 5; ++i) {
        freq[hand[i][1]]++;
    }

    // Count pairs, three of a kind, four of a kind
    int pairs = 0, three = 0, four = 0;
    for (int r = 0; r < 13; ++r) {
        if (freq[r] == 2) pairs++;
        else if (freq[r] == 3) three++;
        else if (freq[r] == 4) four++;
    }

    // Check flush
    bool flush = true;
    for (int i = 1; i < 5; ++i) {
        if (hand[i][0] != hand[0][0]) {
            flush = false;
            break;
        }
    }

    // Collect unique ranks for straight detection
    std::vector<int> ranks;
    for (int r = 0; r < 13; ++r) {
        if (freq[r] > 0) ranks.push_back(r);
    }
    std::sort(ranks.begin(), ranks.end());

    // Check straight: either ranks are {0,1,2,3,12} (Ace low) or five consecutive
    bool straight = false;
    if (ranks.size() == 5) {
        // Ace-low straight: 0,1,2,3,12
        if (ranks[0] == 0 && ranks[1] == 1 && ranks[2] == 2 && ranks[3] == 3 && ranks[4] == 12) {
            straight = true;
        } else {
            // Consecutive check
            straight = true;
            for (int i = 1; i < 5; ++i) {
                if (ranks[i] != ranks[i-1] + 1) {
                    straight = false;
                    break;
                }
            }
        }
    }

    // Classification
    if (straight && flush) return 8;
    if (four == 1) return 7;
    if (three == 1 && pairs == 1) return 6;
    if (flush) return 5;
    if (straight) return 4;
    if (three == 1) return 3;
    if (pairs == 2) return 2;
    if (pairs == 1) return 1;
    return 0;
}
#include <cassert>

int main() {
    // Straight flush: all same suit, 5-6-7-8-9 (ranks 4-8)
    int hand1[5][2] = {{0,4},{0,5},{0,6},{0,7},{0,8}};
    assert(evaluatePokerHand(hand1) == 8);

    // Four of a kind: four 7s (rank 6), one 2 (rank 1)
    int hand2[5][2] = {{0,6},{1,6},{2,6},{3,6},{0,1}};
    assert(evaluatePokerHand(hand2) == 7);

    // Full house: three 10s (rank 9), two 3s (rank 2)
    int hand3[5][2] = {{0,9},{1,9},{2,9},{0,2},{1,2}};
    assert(evaluatePokerHand(hand3) == 6);

    // Flush: all same suit, not consecutive
    int hand4[5][2] = {{1,0},{1,2},{1,5},{1,9},{1,12}};
    assert(evaluatePokerHand(hand4) == 5);

    // Straight: Ace low (0,1,2,3,12) not flush
    int hand5[5][2] = {{0,0},{1,1},{0,2},{1,3},{0,12}};
    assert(evaluatePokerHand(hand5) == 4);

    // Three of a kind: three 5s (rank 4), two distinct others
    int hand6[5][2] = {{0,4},{1,4},{2,4},{3,7},{0,11}};
    assert(evaluatePokerHand(hand6) == 3);

    // Two pairs: Kings (11) and 4s (3)
    int hand7[5][2] = {{0,11},{1,11},{2,3},{3,3},{0,7}};
    assert(evaluatePokerHand(hand7) == 2);

    // One pair: two Jacks (rank 10)
    int hand8[5][2] = {{0,10},{1,10},{2,2},{3,5},{0,8}};
    assert(evaluatePokerHand(hand8) == 1);

    // High card: nothing
    int hand9[5][2] = {{0,0},{1,4},{2,6},{3,9},{0,11}};
    assert(evaluatePokerHand(hand9) == 0);

    // Edge case: Ace-high straight (9,10,J,Q,K) all suits mixed
    int hand10[5][2] = {{0,8},{1,9},{2,10},{3,11},{0,12}};
    assert(evaluatePokerHand(hand10) == 4);

    return 0;
}
// The solution approach is to compute five boolean/flag indicators for the hand: `isFlush` (all suits equal), `isStraight` (five consecutive ranks, considering Ace as both 0 and 13), `counts` of each rank frequency, and the number of pairs, three-of-a-kinds, and four-of-a-kinds. First, convert ranks to a frequency array of size 13, and count frequencies. Then check flush by comparing all suits to the first suit. For straight, sort the ranks or use a set; note Ace can be 0 or 13, so check for pattern {0,1,2,3,12} as a valid straight (Ace low) and any set of five consecutive numbers in the sorted ranks. Then classify: if straight and flush → 8; if any frequency is 4 → 7; if frequency has 3 and 2 → 6; if flush → 5; if straight → 4; if frequency has 3 → 3; if exactly two pairs → 2; if exactly one pair → 1; else 0. Edge cases: duplicate cards not possible, but the frequency logic still works. Time complexity is O(1) since the hand size is fixed at 5; space complexity is O(1) (constant-size arrays).
