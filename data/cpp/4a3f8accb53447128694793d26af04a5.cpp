Write a C++ function named `findBlackJackWinOdds` that simulates a simplified Blackjack game to estimate the win probability for the player over a given number of rounds. The function should take four integer parameters: `packsInDeck` (number of 52-card decks shuffled together), `amountOfPlayers` (number of players at the table, including the dealer), and `finderIterations` (number of simulated rounds). It should return a `double` representing the proportion of rounds where the player (the first non-dealer) wins (i.e., their hand total exceeds the dealer's without exceeding 21, or the dealer busts while the player does not). Use a simple strategy: the player stands on 17 or higher, hits otherwise; the dealer stands on 17 or higher, hits otherwise. Cards are drawn without replacement from a shoe containing all cards from the specified decks. Aces count as 11 unless that would bust, then 1; face cards count as 10. If the player busts, the dealer wins; if the dealer busts and the player did not, the player wins; if both have the same total, it's a tie (neither wins). The function must handle edge cases like `packsInDeck` <= 0, `amountOfPlayers` <= 1, or `finderIterations` <= 0 by returning 0.0. Assume the shoe is reshuffled once it runs out of cards mid‑simulation.
// The solution simulates repeated rounds of Blackjack with a simplified hit/stand strategy. For each iteration, we need to deal cards to each player (including the dealer). Since the number of players can be arbitrary, we track each player's hand total separately. The main steps per round: initialize all hands (each player gets two cards), then for each non-dealer player, follow the hit-until-17-or-higher strategy; for the dealer, follow the same strategy. After all players act, compare the player's total (the first non-dealer) to the dealer's total, counting wins. We must handle card drawing from a shoe: we maintain a pool of card values (with counts per rank) and draw without replacement. When the shoe runs out (less than, say, 2*amountOfPlayers cards left), we reshuffle by replenishing the deck. Aces are handled specially: when adding an ace, we add 11 and adjust to 1 if total > 21. The main algorithm is straightforward simulation with O(iterations * players * (average cards per hand)) time. Space is O(1) for the shoe representation (an array of 10 card values with counts) plus O(players) for hand totals. Edge cases: invalid inputs return 0.0; ties are not counted as wins. Time complexity is O(I * P * H) where H is bounded by 12 (since a hand cannot exceed 21 with more than 5 cards typically, but worst case with many aces could be more, but bounded by 21 cards if all ones). Space is O(P) for storing hand totals.
#include <vector>
#include <random>
#include <algorithm>

constexpr int CARD_VALUES = 10; // 2-10, J, Q, K, A
constexpr int CARDS_PER_DECK = 52;

// Helper: add a card value to a hand total, adjusting for aces.
void addCardToHand(int cardValue, int& handTotal, int& aceCount) {
    if (cardValue == 11) { // ace
        aceCount++;
        handTotal += 11;
    } else {
        handTotal += cardValue;
    }
    // Reduce total for aces while busting
    while (handTotal > 21 && aceCount > 0) {
        handTotal -= 10; // change an ace from 11 to 1
        aceCount--;
    }
}

// Simulate Blackjack win probability for the first non-dealer player.
double findBlackJackWinOdds(int packsInDeck, int amountOfPlayers, int finderIterations) {
    if (packsInDeck <= 0 || amountOfPlayers <= 1 || finderIterations <= 0) {
        return 0.0;
    }

    // Deck representation: counts of each card value (0-8 are 2-10, 9 is J/Q/K, 10 is A)
    // We'll store as: index 0..8 for 2..10, index 9 for face (10), index 10 for ace (11)
    // Actually use 10 values: positions 0..8 are 2..10, position 9 is J/Q/K (value 10), position 10 is A (value 11)
    // Simplify: create a vector of card values in the shoe.
    std::vector<int> shoe;
    auto reshuffle = [&]() {
        shoe.clear();
        for (int deck = 0; deck < packsInDeck; ++deck) {
            for (int i = 2; i <= 10; ++i) {
                shoe.insert(shoe.end(), 4, i); // 4 cards of each value 2-10
            }
            // Face cards (J,Q,K) value 10: 12 cards per deck
            shoe.insert(shoe.end(), 12, 10);
            // Aces value 11: 4 per deck
            shoe.insert(shoe.end(), 4, 11);
        }
        std::shuffle(shoe.begin(), shoe.end(), std::mt19937{std::random_device{}()});
    };

    reshuffle();
    int cardsLeft = static_cast<int>(shoe.size());
    int cardsDealt = 0;

    auto drawCard = [&]() -> int {
        // Reshuffle if needed
        if (cardsDealt >= cardsLeft) {
            reshuffle();
            cardsDealt = 0;
        }
        return shoe[cardsDealt++];
    };

    int wins = 0;
    for (int iter = 0; iter < finderIterations; ++iter) {
        // Initialize hands for all players (dealer is last index)
        std::vector<int> handTotals(amountOfPlayers, 0);
        std::vector<int> aceCounts(amountOfPlayers, 0);
        
        // Deal two cards to each player
        for (int p = 0; p < amountOfPlayers; ++p) {
            for (int c = 0; c < 2; ++c) {
                int card = drawCard();
                addCardToHand(card, handTotals[p], aceCounts[p]);
            }
        }

        // Player strategy (players 0..amountOfPlayers-2 are non-dealers)
        // We only care about player 0 (the first non-dealer)
        int playerTotal = handTotals[0];
        int playerAces = aceCounts[0];
        while (playerTotal < 17) {
            int card = drawCard();
            addCardToHand(card, playerTotal, playerAces);
        }

        // Dealer strategy (last player)
        int dealerTotal = handTotals[amountOfPlayers - 1];
        int dealerAces = aceCounts[amountOfPlayers - 1];
        while (dealerTotal < 17) {
            int card = drawCard();
            addCardToHand(card, dealerTotal, dealerAces);
        }

        // Determine outcome
        bool playerBust = playerTotal > 21;
        bool dealerBust = dealerTotal > 21;
        if (!playerBust && (dealerBust || playerTotal > dealerTotal)) {
            ++wins;
        }
        // Ties and losses not counted
    }

    return static_cast<double>(wins) / finderIterations;
}
#include <cassert>
#include <cmath>

int main() {
    // Invalid inputs return 0.0
    assert(findBlackJackWinOdds(0, 2, 100) == 0.0);
    assert(findBlackJackWinOdds(1, 1, 100) == 0.0);
    assert(findBlackJackWinOdds(1, 2, 0) == 0.0);

    // With a single deck and 2 players (player+dealer), win probability should be between 0.3 and 0.5 for reasonable strategy
    double p1 = findBlackJackWinOdds(1, 2, 10000);
    assert(p1 > 0.30 && p1 < 0.50);

    // With many decks, probability should be similar (law of large numbers)
    double p4 = findBlackJackWinOdds(4, 2, 10000);
    assert(p4 > 0.30 && p4 < 0.50);

    // More players (4 players + dealer) should not affect the player's win probability much
    double pMany = findBlackJackWinOdds(1, 5, 10000);
    assert(pMany > 0.30 && pMany < 0.50);

    // Deterministic test: with 1 iteration, result is either 0 or 1
    double pSingle = findBlackJackWinOdds(1, 2, 1);
    assert(pSingle == 0.0 || pSingle == 1.0);

    // Consistency: running twice with same seed? We use random_device, so cannot force exact, but verify plausible
    // Just ensure no crash for large iterations
    double pLarge = findBlackJackWinOdds(6, 6, 50000);
    assert(pLarge > 0.30 && pLarge < 0.50);

    return 0;
}
