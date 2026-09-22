Write a standalone C++ function `evaluatePokerHand` that takes a vector of card values (integers from 2 to 14, where 11=Jack, 12=Queen, 13=King, 14=Ace) and a vector of card suits (integers 1 to 4, where 1=Diamonds, 2=Clubs, 3=Spades, 4=Hearts). The function should return an integer `HandStrength` enum value (0=HIGH_CARD, 1=ONE_PAIR, 2=TWO_PAIR, 3=THREE_OF_A_KIND, 4=STRAIGHT, 5=FLUSH, 6=FULL_HOUSE, 7=FOUR_OF_A_KIND, 8=STRAIGHT_FLUSH, 9=ROYAL_FLUSH). The input vectors may be empty or contain between 2 and 7 cards total (combining the player’s 2 hole cards and 0–5 community cards). The function must correctly evaluate the best possible 5-card poker hand from the given cards, including handling duplicate cards, unsorted input, and edge cases like a straight with Ace as both high (10-J-Q-K-A) and low (A-2-3-4-5). Implement helper functions for checking pairs, flushes, straights, and full houses.
The solution requires evaluating the strength of the best 5-card poker hand from a set of 2–7 cards. The key steps:  
1. Sort the values in ascending order to simplify straight detection and counting duplicates.  
2. Count occurrences of each value (using a frequency array of size 15) to detect pairs, three-of-a-kind, and four-of-a-kind.  
3. Count occurrences of each suit to detect a flush (any suit with ≥5 cards).  
4. For straights, check all possible consecutive sequences of length 5 in the sorted unique values. Special handling for the Ace-low straight (A-2-3-4-5) by treating Ace as value 1 when checking that sequence.  
5. Combine these checks hierarchically: check royal flush (straight flush with top card Ace), straight flush, four-of-a-kind, full house (three of a kind and a pair), flush, straight, three of a kind, two pair, one pair, then high card.  
6. Edge cases: If fewer than 5 cards are provided, the best hand is still the highest rank possible from those cards (e.g., a pair or high card). Duplicate cards (e.g., two identical suits+values) are not expected but should not break logic; the frequency counting still works. Time complexity is O(n log n) for sorting (n ≤ 7), and O(1) for all checks. Space complexity is O(1) since we use fixed-size arrays.
#include <vector>
#include <algorithm>
#include <array>

enum HandStrength {
    HIGH_CARD = 0, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND,
    STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND,
    STRAIGHT_FLUSH, ROYAL_FLUSH
};

// Count occurrences of each value (2..14) in cards
std::array<int, 15> countValues(const std::vector<int>& values) {
    std::array<int, 15> counts{};
    for (int v : values) {
        if (v >= 2 && v <= 14) ++counts[v];
    }
    return counts;
}

// Check if there are exactly 'threshold' cards of the same value
bool hasRepeated(const std::array<int, 15>& counts, int threshold) {
    for (int c : counts) if (c >= threshold) return true;
    return false;
}

// Check if there are at least two pairs
bool hasTwoPairs(const std::array<int, 15>& counts) {
    int pairs = 0;
    for (int c : counts) if (c >= 2) ++pairs;
    return pairs >= 2;
}

// Check for a flush: any suit with >=5 cards
bool isFlush(const std::vector<int>& suits) {
    std::array<int, 5> suitCounts{};
    for (int s : suits) if (s >= 1 && s <= 4) ++suitCounts[s];
    for (int c : suitCounts) if (c >= 5) return true;
    return false;
}

// Check for a straight. Aces can be low (1-2-3-4-5) or high (10-J-Q-K-A)
bool isStraight(const std::vector<int>& sortedValues) {
    // Get unique sorted values
    std::vector<int> unique;
    for (int v : sortedValues) {
        if (unique.empty() || unique.back() != v) unique.push_back(v);
    }
    if (unique.size() < 5) return false;
    
    // Try consecutive sequences of length 5
    for (size_t i = 0; i + 4 < unique.size(); ++i) {
        bool consecutive = true;
        for (size_t j = i; j < i + 4; ++j) {
            if (unique[j+1] != unique[j] + 1) { consecutive = false; break; }
        }
        if (consecutive) return true;
    }
    
    // Special case: Ace-low straight (A, 2, 3, 4, 5)
    // Check if we have Ace, 2, 3, 4, 5
    bool hasAce = std::find(sortedValues.begin(), sortedValues.end(), 14) != sortedValues.end();
    if (hasAce) {
        bool has2 = std::find(sortedValues.begin(), sortedValues.end(), 2) != sortedValues.end();
        bool has3 = std::find(sortedValues.begin(), sortedValues.end(), 3) != sortedValues.end();
        bool has4 = std::find(sortedValues.begin(), sortedValues.end(), 4) != sortedValues.end();
        bool has5 = std::find(sortedValues.begin(), sortedValues.end(), 5) != sortedValues.end();
        if (has2 && has3 && has4 && has5) return true;
    }
    return false;
}

// Check for a full house: three of a kind and a pair
bool isFullHouse(const std::array<int, 15>& counts) {
    bool hasThree = false, hasPair = false;
    for (int c : counts) {
        if (c >= 3) hasThree = true;
        else if (c >= 2) hasPair = true;
    }
    // Also handle case where three of a kind and a pair come from same count (e.g., 5 of a kind not possible here, but 3+2 with different values)
    return hasThree && hasPair;
}

// Check for a straight flush
bool isStraightFlush(const std::vector<int>& sortedValues, const std::vector<int>& suits) {
    // Need both straight and flush
    if (!isFlush(suits) || !isStraight(sortedValues)) return false;
    
    // For simplicity, we assume if we have both a straight and a flush, it's a straight flush.
    // In real poker we need to verify the straight uses the flush suit, but for this enum evaluation,
    // we check if there exists a suit with at least 5 cards that form a consecutive sequence.
    // For a compact solution, we check every suit group:
    for (int suit = 1; suit <= 4; ++suit) {
        std::vector<int> suitValues;
        for (size_t i = 0; i < suits.size(); ++i) {
            if (suits[i] == suit) suitValues.push_back(sortedValues[i]);
        }
        if (suitValues.size() >= 5) {
            std::sort(suitValues.begin(), suitValues.end());
            if (isStraight(suitValues)) return true;
        }
    }
    return false;
}

// Evaluate the best hand strength from given values and suits
int evaluatePokerHand(const std::vector<int>& values, const std::vector<int>& suits) {
    if (values.size() != suits.size()) return HIGH_CARD; // invalid input
    
    std::vector<int> sortedValues = values;
    std::sort(sortedValues.begin(), sortedValues.end());
    
    auto counts = countValues(sortedValues);
    
    // Determine if it's a royal flush (straight flush with high card Ace, i.e. 10-J-Q-K-A)
    if (isStraightFlush(sortedValues, suits)) {
        // Check for Ace-high straight flush (values 10,11,12,13,14)
        bool hasAce = std::find(sortedValues.begin(), sortedValues.end(), 14) != sortedValues.end();
        bool hasKing = std::find(sortedValues.begin(), sortedValues.end(), 13) != sortedValues.end();
        bool hasQueen = std::find(sortedValues.begin(), sortedValues.end(), 12) != sortedValues.end();
        bool hasJack = std::find(sortedValues.begin(), sortedValues.end(), 11) != sortedValues.end();
        bool hasTen = std::find(sortedValues.begin(), sortedValues.end(), 10) != sortedValues.end();
        if (hasAce && hasKing && hasQueen && hasJack && hasTen) return ROYAL_FLUSH;
        return STRAIGHT_FLUSH;
    }
    
    if (hasRepeated(counts, 4)) return FOUR_OF_A_KIND;
    if (isFullHouse(counts)) return FULL_HOUSE;
    if (isFlush(suits)) return FLUSH;
    if (isStraight(sortedValues)) return STRAIGHT;
    if (hasRepeated(counts, 3)) return THREE_OF_A_KIND;
    if (hasTwoPairs(counts)) return TWO_PAIR;
    if (hasRepeated(counts, 2)) return ONE_PAIR;
    return HIGH_CARD;
}
#include <cassert>

int main() {
    // Royal flush: 10, J, Q, K, A of hearts
    assert(evaluatePokerHand({10, 11, 12, 13, 14}, {4, 4, 4, 4, 4}) == ROYAL_FLUSH);
    
    // Straight flush: 5,6,7,8,9 of spades
    assert(evaluatePokerHand({5, 6, 7, 8, 9}, {3, 3, 3, 3, 3}) == STRAIGHT_FLUSH);
    
    // Four of a kind
    assert(evaluatePokerHand({7, 7, 7, 7, 2}, {1, 2, 3, 4, 1}) == FOUR_OF_A_KIND);
    
    // Full house
    assert(evaluatePokerHand({3, 3, 3, 9, 9}, {1, 2, 3, 1, 2}) == FULL_HOUSE);
    
    // Flush (five hearts, not straight)
    assert(evaluatePokerHand({2, 5, 8, 12, 14}, {4, 4, 4, 4, 4}) == FLUSH);
    
    // Straight with Ace low: A,2,3,4,5
    assert(evaluatePokerHand({14, 2, 3, 4, 5}, {1, 2, 3, 4, 1}) == STRAIGHT);
    
    // Three of a kind
    assert(evaluatePokerHand({6, 6, 6, 10, 11}, {1, 2, 3, 4, 1}) == THREE_OF_A_KIND);
    
    // Two pair
    assert(evaluatePokerHand({4, 4, 8, 8, 12}, {1, 2, 3, 4, 1}) == TWO_PAIR);
    
    // One pair
    assert(evaluatePokerHand({4, 4, 8, 9, 12}, {1, 2, 3, 4, 1}) == ONE_PAIR);
    
    // High card
    assert(evaluatePokerHand({2, 4, 7, 9, 12}, {1, 2, 3, 4, 1}) == HIGH_CARD);
    
    // Edge: only two cards (hole cards only) => best is pair or high card
    assert(evaluatePokerHand({13, 13}, {1, 2}) == ONE_PAIR);
    assert(evaluatePokerHand({13, 2}, {1, 2}) == HIGH_CARD);
    
    // Edge: 7 cards with straight flush using 5 of them
    assert(evaluatePokerHand({2, 3, 4, 5, 6, 7, 14}, {1, 1, 1, 1, 1, 2, 3}) == STRAIGHT_FLUSH);
    
    return 0;
}
