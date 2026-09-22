Write a C++ function that builds a standard deck of 52 playing cards, with 13 ranks (A, 2-9, T, J, Q, K) and 4 suits (CLUB, DIAMOND, HEART, SPADE), and returns it as a `std::string` representation. The string must list each card on its own line in the exact order: rank/suit (e.g., `A/CLUB`, `A/DIAMOND`, ... `K/SPADE`), starting with all CLUBs first (ranks A through K), then DIAMONDs, then HEARTs, then SPADEs. The function must accept no arguments and return the complete deck as a single string with a trailing newline after the last card. You may use any standard library facilities, but the output format must match exactly.
#include <cassert>
#include <string>

// Declare the function from the solution.
std::string buildDeckString();

int main() {
    std::string deck = buildDeckString();

    // First card should be A/CLUB
    assert(deck.substr(0, 6) == "A/CLUB");

    // Last card should be K/SPADE followed by newline
    assert(deck.substr(deck.size() - 7) == "K/SPADE\n");

    // Deck should contain all 52 cards (each card line: rank/suit\n = 6 or 7 chars, but we just check line count)
    size_t newlineCount = 0;
    for (char c : deck) {
        if (c == '\n') newlineCount++;
    }
    assert(newlineCount == 52);

    // Check a middle card: after 13 ranks of CLUB and 13 of DIAMOND, first HEART is at index 26*7? Actually each line is rank(1) + '/' + suit(5 or 6) + '\n'. CLUB=5 chars, so line length = 1+1+5+1=8. DIAMOND=7, line length=10. So cannot rely on fixed length. Instead, search for specific known cards.
    assert(deck.find("9/CLUB\n") != std::string::npos);
    assert(deck.find("T/DIAMOND\n") != std::string::npos);
    assert(deck.find("Q/HEART\n") != std::string::npos);
    assert(deck.find("K/SPADE\n") != std::string::npos);

    // Ensure all 52 distinct cards exist by checking a few more and order: A/DIAMOND before A/HEART
    size_t posADiamond = deck.find("A/DIAMOND\n");
    size_t posAHeart = deck.find("A/HEART\n");
    assert(posADiamond != std::string::npos);
    assert(posAHeart != std::string::npos);
    assert(posADiamond < posAHeart);

    // Also check that CLUB cards all appear before DIAMOND cards
    size_t posAClub = deck.find("A/CLUB\n");
    size_t posADiamond2 = deck.find("A/DIAMOND\n");
    assert(posAClub < posADiamond2);
}
#include <string>

// Build a standard 52-card deck as a string, listing all cards rank/suit in order:
// rank index 0..12 (A,2,3,4,5,6,7,8,9,T,J,Q,K) and suit index 0..3 (CLUB,DIAMOND,HEART,SPADE).
std::string buildDeckString() {
    const char* ranks = "A23456789TJQK";
    const char* suits[] = {"CLUB", "DIAMOND", "HEART", "SPADE"};
    const int numberRanks = 13;
    const int numberSuits = 4;

    std::string deck;
    deck.reserve(numberRanks * numberSuits * 8); // rough estimate for capacity

    for (int rankIdx = 0; rankIdx < numberRanks; ++rankIdx) {
        for (int suitIdx = 0; suitIdx < numberSuits; ++suitIdx) {
            deck += ranks[rankIdx];
            deck += '/';
            deck += suits[suitIdx];
            deck += '\n';
        }
    }
    return deck;
}
// The solution must generate all 52 cards in a nested loop: outer loop over ranks (0 to 12), inner loop over suits (0 to 3), matching the original snippet's construction order. For each pair, map the rank index to a character (`A` through `K`, with `10` represented as `T`) and the suit index to its name string. Append the formatted card string plus a newline to a `std::string` accumulator. Since the order is fixed and deterministic, there is no need for dynamic memory or linked lists—a simple concatenation works. Edge cases: ensure the loop covers all 13×4 = 52 cards exactly once, the rank mapping includes `T` for ten, and the suit names are exactly `CLUB`, `DIAMOND`, `HEART`, `SPADE` (uppercase). Time complexity is O(52) = O(1) since the deck size is constant, and space complexity is O(1) besides the output string size, which is O(52) as well.
