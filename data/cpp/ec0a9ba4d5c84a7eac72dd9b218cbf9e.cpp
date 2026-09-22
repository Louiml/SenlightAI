Write a C++ function that takes a vector of 7 card indices (each in the canonical range 0–51, where rank = index / 4 and suit = index % 4, with ascending ranks 0=deuce, 12=ace) and returns a unique, order-preserving numeric hand strength value (higher is better) for a standard 7-card poker hand. The strength value must be consistent with the conventional poker hand ranking: royal flush > straight flush > four of a kind > full house > flush > straight > three of a kind > two pair > one pair > high card. For simplicity, do not use a lookup table — implement a deterministic algorithm that evaluates the hand by analyzing ranks and suits directly. The function signature must be: `unsigned evaluateSevenCardHand(const std::array<unsigned,7>& cards)`.
The solution involves categorizing the hand based on counts of ranks and suit patterns. First, extract suits and ranks from each card index (rank = idx / 4, suit = idx % 4). Build frequency arrays: one for ranks (size 13) and one for suits (size 4). Handle the special case of an ace-low straight (Ace,2,3,4,5) by treating Ace as rank 13 (or re‑mapping to 0 when needed). To compute the hand strength as an ordered numeric value, use a lexicographic scoring scheme: assign a base category score (e.g., 8 for straight flush, 7 for quads, ... 0 for high card) multiplied by a large constant (e.g., 2^20), then add tie‑breaker information packed into lower bits. For tie‑breakers, consider the ranks that matter: for quads, include quad rank then kicker; for full house, triplet then pair; for flush and high card, sort ranks descending and pack them; for straight, only the high rank matters (with Ace‑low handled specially). For pairs and two pairs, include pair ranks and kickers in descending order. The ranking must be strictly monotonic: a better hand always yields a larger unsigned value. Use a helper that converts a sorted vector of ranks (descending) into a packed integer using a base‑13 representation (since ranks 0–12). Time complexity is O(1) because the hand always has 7 cards; space is O(1) for fixed‑size arrays.
#include <array>
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns a unique hand strength value for a 7-card poker hand.
// Higher value = better hand. Cards are given as indices 0..51 where
// rank = index / 4 (0=2, ..., 12=Ace) and suit = index % 4.
unsigned evaluateSevenCardHand(const std::array<unsigned,7>& cards) {
    // Count ranks and suits.
    unsigned rankCount[13] = {0};
    unsigned suitCount[4] = {0};
    for (unsigned c : cards) {
        unsigned r = c / 4;
        unsigned s = c % 4;
        rankCount[r]++;
        suitCount[s]++;
    }

    // Gather ranks that appear at least once, plus how many times.
    std::vector<unsigned> presentRanks;
    std::vector<unsigned> quadRanks, tripleRanks, pairRanks, singleRanks;
    for (unsigned r = 0; r < 13; ++r) {
        if (rankCount[r] > 0) {
            presentRanks.push_back(r);
            if (rankCount[r] == 4) quadRanks.push_back(r);
            else if (rankCount[r] == 3) tripleRanks.push_back(r);
            else if (rankCount[r] == 2) pairRanks.push_back(r);
            else singleRanks.push_back(r);
        }
    }

    // Check for flush: any suit with at least 5 cards.
    bool isFlush = false;
    for (unsigned s = 0; s < 4; ++s) {
        if (suitCount[s] >= 5) { isFlush = true; break; }
    }

    // Check for straight: at least 5 consecutive ranks.
    // Handle Ace-low (A,2,3,4,5) by treating Ace as rank 13 temporarily.
    std::array<bool,14> have = {false};
    for (unsigned r : presentRanks) have[r] = true;
    if (have[12]) have[13] = true; // ace also acts as rank 13

    int straightHigh = -1;
    for (int high = 12; high >= 0; --high) {
        // Check if ranks high-4..high are all present.
        bool ok = true;
        for (int r = high; r >= high-4 && r >= 0; --r) {
            if (!have[r]) { ok = false; break; }
        }
        // Special case for Ace-low: ranks 12, 0,1,2,3
        if (ok) {
            straightHigh = high;
            break;
        }
        // Also check Ace-low directly.
        if (high == 3) {
            if (have[12] && have[0] && have[1] && have[2] && have[3]) {
                straightHigh = 3; // high card = 3 (which is 5-spot)
                break;
            }
        }
    }
    bool isStraight = (straightHigh >= 0);

    // Determine category and primary tie-breakers.
    // Categories: 8=straight flush, 7=four of a kind, 6=full house,
    //             5=flush, 4=straight, 3=three of a kind,
    //             2=two pair, 1=one pair, 0=high card.
    unsigned category;
    std::vector<unsigned> tieRanks;

    if (isFlush && isStraight) {
        category = 8;
        tieRanks = {static_cast<unsigned>(straightHigh)};
    } else if (!quadRanks.empty()) {
        category = 7;
        // Quads rank first, then best kicker.
        unsigned quad = quadRanks[0];
        unsigned bestKicker = 0;
        for (unsigned r : presentRanks) if (r != quad && r > bestKicker) bestKicker = r;
        tieRanks = {quad, bestKicker};
    } else if (!tripleRanks.empty() && (rankCount[tripleRanks[0]] == 3 && pairRanks.size() > 0)) {
        category = 6;
        // Full house: highest triplet, then highest pair.
        std::sort(tripleRanks.rbegin(), tripleRanks.rend());
        std::sort(pairRanks.rbegin(), pairRanks.rend());
        tieRanks = {tripleRanks[0], pairRanks[0]};
    } else if (isFlush) {
        category = 5;
        // Flush: sort all ranks descending.
        std::sort(presentRanks.rbegin(), presentRanks.rend());
        tieRanks = presentRanks;
    } else if (isStraight) {
        category = 4;
        tieRanks = {static_cast<unsigned>(straightHigh)};
    } else if (!tripleRanks.empty()) {
        category = 3;
        // Three of a kind: triplet then kickers descending.
        std::sort(tripleRanks.rbegin(), tripleRanks.rend());
        std::sort(singleRanks.rbegin(), singleRanks.rend());
        tieRanks.push_back(tripleRanks[0]);
        tieRanks.insert(tieRanks.end(), singleRanks.begin(), singleRanks.end());
    } else if (pairRanks.size() >= 2) {
        category = 2;
        // Two pair: pairs descending, then kicker.
        std::sort(pairRanks.rbegin(), pairRanks.rend());
        tieRanks = {pairRanks[0], pairRanks[1]};
        for (unsigned r : presentRanks) {
            if (r != pairRanks[0] && r != pairRanks[1]) {
                tieRanks.push_back(r);
                break;
            }
        }
    } else if (!pairRanks.empty()) {
        category = 1;
        // One pair: pair, then three kickers descending.
        unsigned p = pairRanks[0];
        std::vector<unsigned> kickers;
        for (unsigned r : presentRanks) if (r != p) kickers.push_back(r);
        std::sort(kickers.rbegin(), kickers.rend());
        tieRanks.push_back(p);
        tieRanks.insert(tieRanks.end(), kickers.begin(), kickers.end());
    } else {
        category = 0;
        std::sort(presentRanks.rbegin(), presentRanks.rend());
        tieRanks = presentRanks;
    }

    // Pack into a single unsigned value: category in the high bits,
    // then tie-breaker ranks in base-13 (max 5 ranks).
    unsigned result = category << 20;
    unsigned multiplier = 1;
    for (size_t i = 0; i < tieRanks.size() && i < 5; ++i) {
        result += tieRanks[i] * multiplier;
        multiplier *= 13;
    }
    return result;
}
#include <cassert>
#include <array>

// Function prototype from solution.
unsigned evaluateSevenCardHand(const std::array<unsigned,7>& cards);

int main() {
    // Helper to convert to canonical index.
    auto C = [](unsigned rank, unsigned suit) { return rank * 4 + suit; };

    // High card: 2,3,4,5,7,9,J no flush/straight
    std::array<unsigned,7> h1 = {C(0,0),C(1,1),C(2,0),C(3,1),C(5,2),C(7,3),C(9,0)};
    // One pair: 3,4,5,6,8,8,K
    std::array<unsigned,7> h2 = {C(1,1),C(2,0),C(3,2),C(4,1),C(6,3),C(6,0),C(11,0)};
    // Two pair: 2,2,Q,Q,A
    std::array<unsigned,7> h3 = {C(0,0),C(0,1),C(10,2),C(10,3),C(12,0),C(4,1),C(5,2)};
    // Three of a kind: 2,2,2,5,7,9,J
    std::array<unsigned,7> h4 = {C(0,0),C(0,1),C(0,2),C(3,0),C(5,1),C(7,2),C(9,3)};
    // Straight: 5,6,7,8,9 + other cards
    std::array<unsigned,7> h5 = {C(3,0),C(4,1),C(5,2),C(6,3),C(7,0),C(11,1),C(12,2)};
    // Flush: all hearts (suit 0) 2,4,7,9,K
    std::array<unsigned,7> h6 = {C(0,0),C(2,0),C(5,0),C(7,0),C(11,0),C(1,1),C(3,2)};
    // Full house: 5,5,5,K,K + extra
    std::array<unsigned,7> h7 = {C(3,0),C(3,1),C(3,2),C(11,0),C(11,1),C(0,3),C(1,2)};
    // Four of a kind: 8,8,8,8,2 + extra
    std::array<unsigned,7> h8 = {C(6,0),C(6,1),C(6,2),C(6,3),C(0,0),C(3,1),C(5,2)};
    // Straight flush: 7,8,9,10,J all spades (suit 1)
    std::array<unsigned,7> h9 = {C(5,1),C(6,1),C(7,1),C(8,1),C(9,1),C(0,0),C(1,2)};
    // Royal flush: 10,J,Q,K,A all spades
    std::array<unsigned,7> h10 = {C(8,1),C(9,1),C(10,1),C(11,1),C(12,1),C(0,2),C(1,3)};

    unsigned v1 = evaluateSevenCardHand(h1);
    unsigned v2 = evaluateSevenCardHand(h2);
    unsigned v3 = evaluateSevenCardHand(h3);
    unsigned v4 = evaluateSevenCardHand(h4);
    unsigned v5 = evaluateSevenCardHand(h5);
    unsigned v6 = evaluateSevenCardHand(h6);
    unsigned v7 = evaluateSevenCardHand(h7);
    unsigned v8 = evaluateSevenCardHand(h8);
    unsigned v9 = evaluateSevenCardHand(h9);
    unsigned v10 = evaluateSevenCardHand(h10);

    assert(v1 < v2 && v2 < v3 && v3 < v4 && v4 < v5 && v5 < v6 && v6 < v7 && v7 < v8 && v8 < v9 && v9 < v10);

    // Additional checks within same category (e.g., higher high card beats lower)
    std::array<unsigned,7> high2 = {C(0,0),C(1,1),C(2,0),C(3,1),C(5,2),C(7,3),C(10,0)}; // Q instead of J
    assert(evaluateSevenCardHand(high2) > v1);

    // Ace-low straight (A,2,3,4,5) should beat any high card but lose to a 6-high straight.
    std::array<unsigned,7> aceLow = {C(12,0),C(0,1),C(1,2),C(2,3),C(3,0),C(9,1),C(11,2)};
    std::array<unsigned,7> sixHigh = {C(4,0),C(1,1),C(2,2),C(3,3),C(0,0),C(8,1),C(10,2)};
    assert(evaluateSevenCardHand(aceLow) > v1);
    assert(evaluateSevenCardHand(sixHigh) < v5);
    assert(evaluateSevenCardHand(aceLow) < evaluateSevenCardHand(sixHigh));

    return 0;
}
