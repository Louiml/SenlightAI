Write a C++ function `int totalWinnings(const std::vector<std::string>& lines)` that processes a list of lines, each containing a poker hand of 5 cards (characters `2`-`9`, `T`, `J`, `Q`, `K`, `A`) followed by a space and an integer bid (e.g., `"32T3K 765"`). The function must classify each hand into one of seven types (from strongest to weakest: five of a kind, four of a kind, full house, three of a kind, two pair, one pair, high card), where card order for tie-breaking uses the ranking `2 < 3 < 4 < 5 < 6 < 7 < 8 < 9 < T < J < Q < K < A`. Sort all hands from weakest to strongest: first by hand type (weakest first), then by comparing cards left-to-right using the given card ranking when types are equal. Return the sum of each hand’s bid multiplied by its rank (1 for weakest, 2 for next, etc.). The input vector may be empty (return 0) and is guaranteed to contain well‑formed lines with exactly 5 cards and a valid positive integer bid. Do not read from any file; the function takes the lines directly as a parameter.
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test.
int totalWinnings(const std::vector<std::string>& lines);

int main() {
    // Example from the classic puzzle: 5 hands.
    std::vector<std::string> lines1 = {
        "32T3K 765",
        "T55J5 684",
        "KK677 28",
        "KTJJT 220",
        "QQQJA 483"
    };
    assert(totalWinnings(lines1) == 6440);

    // Single hand – rank 1 * bid.
    std::vector<std::string> lines2 = {"AAAAA 100"};
    assert(totalWinnings(lines2) == 100);

    // Two hands: one pair vs high card – one pair is stronger.
    std::vector<std::string> lines3 = {"23456 10", "22345 20"};
    // Sorted: high card first (rank1), one pair second (rank2) → 1*10 + 2*20 = 50.
    assert(totalWinnings(lines3) == 50);

    // Tie‑breaking by card values within same type.
    // Both one pair: pair of 2s vs pair of 3s.
    std::vector<std::string> lines4 = {"22345 50", "33456 1"};
    // Sorted: 22345 (rank1) then 33456 (rank2) → 1*50 + 2*1 = 52.
    assert(totalWinnings(lines4) == 52);

    // All five of a kind – sort by card value (A highest).
    std::vector<std::string> lines5 = {"22222 5", "33333 10"};
    // Sorted: 22222 (rank1), 33333 (rank2) → 1*5 + 2*10 = 25.
    assert(totalWinnings(lines5) == 25);

    // Empty input returns 0.
    std::vector<std::string> lines6;
    assert(totalWinnings(lines6) == 0);

    // Full house vs four of a kind.
    std::vector<std::string> lines7 = {"22233 7", "44445 3"};
    // Sorted: full house (rank1), four of a kind (rank2) → 1*7 + 2*3 = 13.
    assert(totalWinnings(lines7) == 13);

    // Same type, same first three cards, differing fourth.
    std::vector<std::string> lines8 = {"QQQ23 10", "QQQ24 20"};
    // Sorted: QQQ23 (rank1) because 3<4, then QQQ24 (rank2) → 1*10 + 2*20 = 50.
    assert(totalWinnings(lines8) == 50);

    return 0;
}
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

// Enum for hand types, ordered from weakest to strongest.
enum class HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

// Convert a 5‑card hand (string) to its HandType.
HandType classifyHand(const std::string& hand) {
    std::map<char, int> counts;
    for (char c : hand) {
        counts[c]++;
    }
    int distinct = static_cast<int>(counts.size());
    if (distinct == 1) return HandType::FiveOfAKind;
    if (distinct == 2) {
        // Either 4+1 or 3+2.
        for (const auto& pair : counts) {
            if (pair.second == 4) return HandType::FourOfAKind;
        }
        return HandType::FullHouse;
    }
    if (distinct == 3) {
        // Either 3+1+1 or 2+2+1.
        for (const auto& pair : counts) {
            if (pair.second == 3) return HandType::ThreeOfAKind;
        }
        return HandType::TwoPair;
    }
    if (distinct == 4) return HandType::OnePair;
    return HandType::HighCard;
}

// Map a card character to its numeric value (2 is lowest, A is highest).
int cardValue(char c) {
    if (std::isdigit(static_cast<unsigned char>(c))) return c - '0';
    switch (c) {
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 14;
        default: return 0; // Should not occur for valid input.
    }
}

// Main function: compute total winnings from a list of "hand bid" lines.
int totalWinnings(const std::vector<std::string>& lines) {
    struct Hand {
        HandType type;
        std::vector<int> cards;
        int bid;
    };

    std::vector<Hand> hands;
    for (const std::string& line : lines) {
        std::string handStr = line.substr(0, 5);
        std::string bidStr = line.substr(6);
        Hand h;
        h.type = classifyHand(handStr);
        for (char c : handStr) h.cards.push_back(cardValue(c));
        h.bid = std::stoi(bidStr);
        hands.push_back(h);
    }

    // Sort weakest to strongest: lower enum value first, then by card values.
    std::sort(hands.begin(), hands.end(), [](const Hand& a, const Hand& b) {
        if (a.type != b.type) {
            return static_cast<int>(a.type) < static_cast<int>(b.type);
        }
        for (size_t i = 0; i < a.cards.size(); ++i) {
            if (a.cards[i] != b.cards[i]) return a.cards[i] < b.cards[i];
        }
        return false; // Equal hands – order irrelevant.
    });

    int total = 0;
    for (size_t i = 0; i < hands.size(); ++i) {
        total += static_cast<int>(i + 1) * hands[i].bid;
    }
    return total;
}
// The solution begins by defining an enum for the seven hand types with values ordered from weakest (`highCard` = 0) to strongest (`fiveOfAKind` = 6), making sorting straightforward. For each hand, we compute a frequency map of card characters and classify as follows: if all 5 are identical → five of a kind; if there are 2 distinct characters, check if any count is 4 → four of a kind, otherwise full house (counts 3 and 2); if there are 3 distinct characters, check if any count is 3 → three of a kind, otherwise two pair (counts 2,2,1); if there are 4 distinct → one pair; else high card. For tie‑breaking, we map each card character to its numeric value (2–9 → 2–9, T→10, J→11, Q→12, K→13, A→14) and store these in a vector. We sort the hands using a custom comparator that first compares the hand type (using the enum’s numeric order, where lower is weaker) and then compares the card‑value vectors element‑by‑element. After sorting, we iterate with a 1‑based index, multiply the bid by the index, and sum. Time complexity is O(n log n) due to sorting, and O(n) space for storing the hand objects (each hand holds the original string, a vector of 5 ints, the type, and the bid). Edge cases include empty input, duplicate card values within a hand, and identical hands (though the problem guarantees well‑formed input, ties are naturally handled by the comparator returning false both ways, preserving original order).
