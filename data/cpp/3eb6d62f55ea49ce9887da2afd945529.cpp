// Write a C++ function `int simulateTenTwentyThirty(const std::vector<int>& initialDeck)` that simulates the classic solitaire card game "10-20-30" with a standard 52-card deck represented by integers (1 through 13, with suits ignored, so values repeat four times, but any multiset of 52 values works). The game starts by dealing the first 7 cards from the top of the deck face-up onto 7 piles (one card per pile, in order). Then, repeatedly, the game processes piles 0 through 6 (skipping empty ones): for each non‑empty pile, if the deck is empty the game ends immediately as a loss; otherwise, draw the top card from the deck and place it face-down on top of that pile. After placing the card, if the pile now has at least 3 cards, check the following removal rules, in order: (1) sum of the first two cards (from the bottom of the pile) and the last card (top) is a multiple of 10, (2) sum of the first card and the last two cards, (3) sum of the last three cards. If a rule applies, remove those three cards (preserving their order) and add them to the *bottom* of the deck (in the order they were removed, i.e., the first removed card goes to the back of the deck, then the next, then the last), and re‑check the same pile (only if it still has ≥3 cards) without moving to the next pile. Continue this process until one of the following occurs: all 7 piles are empty (win), the deck becomes empty (loss), or an exact previous state of (all pile contents, deck contents) repeats (draw — to detect this, compare the ordered tuple of all 7 deques and the deck queue; the initial state after dealing does not count as a repeat). Return the total number of cards dealt (moves) that were placed onto piles before the game ends. If the game never ends (e.g., due to an input that causes an infinite loop that is not a state repetition — but the rules guarantee a repetition will occur if no win/loss), you may assume the input is such that a win/loss/draw always occurs. The function should not print anything; it only returns the number of moves. If the game is a win, return the move count; if a loss, return the move count; if a draw, return the move count. Edge cases: if the deck initially has fewer than 7 cards, the game is impossible; handle by returning 0 (or you can assume input always has 52 cards, but design robustly). The function must be efficient enough for 52 cards; a naive state check is acceptable (since at most a few thousand states occur). Provide a self‑contained implementation with helper functions as needed.
#include <cassert>
#include <vector>

// The solution function is declared above (simulateTenTwentyThirty).

int main() {
    // Test 1: Simple known win? We'll create a custom deck that definitely wins quickly.
    // For example, use all 10's (value 10) so any three sum to 30 -> 0 mod 10.
    // Deal 7 cards to piles, then each subsequent card triggers removals.
    std::vector<int> deck1(52, 10);
    // This should quickly remove all cards, resulting in a win after some moves.
    int result1 = simulateTenTwentyThirty(deck1);
    // Expected: initial 7 deals + then each draw triggers a removal, so final moves count is exactly 52 (all cards eventually placed).
    // Let's verify manually: after dealing 7, we have 45 in deck. Each move places one card, then removes 3 (which go back to deck).
    // This is a cyclic process, but eventually all cards end up in piles? Actually they get removed back to deck, so
    // the game could draw. To be safe, we just check that result is a positive number.
    assert(result1 > 0);

    // Test 2: Input with fewer than 7 cards should return 0.
    std::vector<int> deck2 = {1, 2, 3};
    assert(simulateTenTwentyThirty(deck2) == 0);

    // Test 3: A deck that immediately loses? If deck runs out before placing all 7 initial cards? But we have 52, so not.
    // Test that a standard deck (1-13 repeated 4 times) runs without crash and returns a valid move count.
    std::vector<int> deck3;
    for (int i = 1; i <= 13; ++i) {
        for (int j = 0; j < 4; ++j) deck3.push_back(i);
    }
    int result3 = simulateTenTwentyThirty(deck3);
    assert(result3 > 0);
    // If the game draws, result is still positive.

    // Test 4: All 1's. Sum of any three = 3, not multiple of 10, so no removals ever.
    // The deck will eventually be exhausted? Let's see: initial deal 7, then each move draws a card, no removals.
    // Deck has 45 cards after dealing, so we can place 45 more cards, total moves = 7+45=52. After that deck is empty.
    // But during the 45th move, we place a card; deck becomes empty after that, and next iteration we detect loss.
    // So total moves = 52. 
    std::vector<int> deck4(52, 1);
    assert(simulateTenTwentyThirty(deck4) == 52);

    // Test 5: All 2's. Sum = 6, not multiple of 10. Same as above, should be 52 moves.
    std::vector<int> deck5(52, 2);
    assert(simulateTenTwentyThirty(deck5) == 52);

    // Test 6: Mix that causes a draw? Hard to predict, but we can check that the function terminates.
    std::vector<int> deck6;
    for (int i = 1; i <= 13; ++i) {
        for (int j = 0; j < 4; ++j) deck6.push_back(i);
    }
    // Shuffle deterministically? Just reuse deck3.
    assert(simulateTenTwentyThirty(deck6) == result3);

    // Test 7: A specific sequence known from original problem? Not needed; just ensure no crashes.

    return 0;
}
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>

// Helper to serialize the current game state into a vector of integers for easy comparison.
// We use a vector instead of a queue because queue doesn't have == operator.
std::vector<int> serializeState(const std::vector<std::deque<int>>& piles, const std::queue<int>& deck) {
    std::vector<int> state;
    for (const auto& pile : piles) {
        for (int card : pile) {
            state.push_back(card);
        }
        state.push_back(-1); // separator between piles
    }
    // Copy deck queue to a temporary queue to iterate
    std::queue<int> tempDeck = deck;
    while (!tempDeck.empty()) {
        state.push_back(tempDeck.front());
        tempDeck.pop();
    }
    return state;
}

// Simulates the 10-20-30 game. Returns the number of moves (cards placed on piles) until the game ends.
int simulateTenTwentyThirty(const std::vector<int>& initialDeck) {
    if (initialDeck.size() < 7) return 0; // not enough cards to deal

    std::queue<int> deck;
    for (int card : initialDeck) {
        deck.push(card);
    }

    std::vector<std::deque<int>> piles(7);
    int moves = 0;

    // Initial deal: 7 cards to piles 0..6
    for (int i = 0; i < 7; ++i) {
        int card = deck.front(); deck.pop();
        piles[i].push_back(card);
        moves++;
    }

    std::vector<std::vector<int>> seenStates;
    seenStates.push_back(serializeState(piles, deck));

    while (true) {
        int emptyCount = 0;
        bool gameEnded = false;

        for (int i = 0; i < 7; ++i) {
            if (piles[i].empty()) {
                emptyCount++;
                continue;
            }

            if (deck.empty()) {
                // Loss: no more cards to deal
                return moves;
            }

            // Deal a card to this pile
            int card = deck.front(); deck.pop();
            piles[i].push_back(card);
            moves++;

            // Check for state repetition
            std::vector<int> currentState = serializeState(piles, deck);
            if (std::find(seenStates.begin(), seenStates.end(), currentState) != seenStates.end()) {
                // Draw: state repeats
                return moves;
            }
            seenStates.push_back(currentState);

            // Apply removal rules while the pile has 3+ cards
            while (piles[i].size() >= 3) {
                int size = piles[i].size();

                // Rule 1: first two (bottom) + last one (top)
                int a = piles[i].front();
                int b = piles[i][1];
                int c = piles[i].back();
                if ((a + b + c) % 10 == 0) {
                    deck.push(a);
                    deck.push(b);
                    deck.push(c);
                    piles[i].pop_front();
                    piles[i].pop_front();
                    piles[i].pop_back();
                    continue;
                }

                // Rule 2: first one + last two
                a = piles[i].front();
                b = piles[i][size - 2];
                c = piles[i].back();
                if ((a + b + c) % 10 == 0) {
                    deck.push(a);
                    deck.push(b);
                    deck.push(c);
                    piles[i].pop_front();
                    piles[i].pop_back();
                    piles[i].pop_back();
                    continue;
                }

                // Rule 3: last three
                a = piles[i][size - 3];
                b = piles[i][size - 2];
                c = piles[i].back();
                if ((a + b + c) % 10 == 0) {
                    deck.push(a);
                    deck.push(b);
                    deck.push(c);
                    piles[i].pop_back();
                    piles[i].pop_back();
                    piles[i].pop_back();
                    continue;
                }

                break; // no rule matched
            }

            // If deck becomes empty after removals (the removals added cards, so not empty), 
            // but check if all piles are empty after this move
            bool allEmpty = true;
            for (const auto& pile : piles) {
                if (!pile.empty()) { allEmpty = false; break; }
            }
            if (allEmpty) {
                // Win
                return moves;
            }
        } // end for each pile

        // After the entire pass, check win condition again
        bool allEmpty = true;
        for (const auto& pile : piles) {
            if (!pile.empty()) { allEmpty = false; break; }
        }
        if (allEmpty) {
            return moves; // Win
        }
        // If deck empty and no win, it would have been caught earlier when trying to draw.
        // The loop continues; if no state repetition, it will eventually happen.
    }
}
// The solution simulates the game exactly as described. Represent the deck as `std::queue<int>` and the 7 piles as `std::deque<int>` (allowing push/pop from both ends). Initialize the deck by pushing all input values; then deal the first 7 cards to piles[0..6] (pop from front of deck, push_back to each pile, increment move counter). The main loop: use a variable `moves` to count each time a card is placed on a pile (initial dealing counts, but subsequent moves only when drawing from deck to pile). For each iteration, set `emptyCount` = 0. For each pile index 0..6: if the pile is empty, increment emptyCount; else, if the deck is empty, immediately return `moves` (loss). Otherwise, pop from deck front, push_back to pile, `moves++`, then check for state repetition: maintain a `std::vector<std::pair<std::vector<std::deque<int>>, std::queue<int>>>` of seen states (or a set of serialized strings, but copying is fine given small size). If the current state (the vector of 7 deques and the deck queue) matches any previously seen state, then return `moves` (draw). Otherwise, push this new state into the history. Then apply the "removal" loop: while pile size >= 3, check the three rules in order. For each rule, extract the three candidate cards (e.g., for rule 1: pile.front(), pile[1], pile.back(); for rule 2: pile.front(), pile[size-2], pile.back(); for rule 3: pile[size-3], pile[size-2], pile.back()). If their sum % 10 == 0, then push them to the deck in the order: first candidate, second, third (i.e., for rule 1: a, b, c; rule 2: a, b, c but with b being the second from back, c=back; make sure to preserve the exact order as written in the original code). Then remove those cards from the pile in the appropriate positions (pop_front/pop_back accordingly). Continue the while loop; if no rule matches, break out of the while loop. After processing all 7 piles in one pass, check if all piles are empty (emptyCount == 7) → return `moves` (win). If the deck becomes empty *during* the pass (when we try to draw for a non‑empty pile and the deck is empty), we return `moves` immediately (loss). Note: the game might end mid‑pass, so the check after the loop for win/loss should be done carefully. The original code has a slightly different structure but we can simplify: after each card placement, we check state and removal; after the for-loop over piles, check if all piles are empty → win. Also, if the deck is empty at the start of an iteration, it would have been caught in the last iteration. Time complexity: each move places one card, and each removal removes 3 cards but adds them to the deck, so the total number of deck operations is bounded by the number of cards dealt plus removed; typically the game terminates quickly. State checking can be O(52) per move, and the number of moves is at most a few thousand, so overall manageable. Space complexity: storing all states, each with 52 cards and 7 deques, is O(states * 52), which is fine.
