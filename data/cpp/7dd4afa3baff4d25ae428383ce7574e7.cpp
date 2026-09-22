/*
Write a C++ function `int minCardsToHold(int m, int n, const std::vector<int>& playedCards)` that simulates a game where a player holds a set of cards numbered 1 through `m*n`. The player reveals `n` specific cards (provided as `playedCards`, each between 1 and `m*n`, possibly unsorted and with duplicates? Assume distinct as per original). Another player plays cards from `m*n` down to 1. The goal: for each card the other player plays, if the first player’s held cards include that exact card number, the first player must win that round (so they need to have a "higher" unplayed card to beat it; if they have no higher unplayed card, the round is lost). The function should return the minimum number of rounds the first player loses, assuming optimal play (i.e., they can choose which of their held cards to use for each round). The original code processes multiple test cases; your function should handle a single test case. The input: `m` and `n` are positive integers, `playedCards` is a list of `n` distinct numbers between 1 and `m*n`. The other player plays all numbers from `m*n` down to 1, but stops once the first player's `n` held cards have all been used (i.e., after the `n`th held card is encountered). Actually the original code stops after encountering all `n` held cards. So the simulation: consider numbers from `m*n` down to 1. For each number, if it is NOT in `playedCards`, it's a "free" card that the first player can save as a higher card to beat later. If it IS in `playedCards`, the first player must respond with a free card (if any available) or lose. The function returns the count of losses.
*/

#include <vector>
#include <cstring>

// Returns the minimum number of losses for the player holding the given cards.
// m and n are positive integers; playedCards contains n distinct integers in [1, m*n].
int minCardsToHold(int m, int n, const std::vector<int>& playedCards) {
    // Create a boolean array to mark which numbers are held.
    std::vector<bool> held(m * n + 1, false);
    for (int card : playedCards) {
        held[card] = true;
    }
    
    int freeCards = 0;  // Count of unheld numbers seen so far that can beat later held cards.
    int losses = 0;
    int heldSeen = 0;
    
    // Scan from the highest number down to 1.
    for (int num = m * n; num >= 1 && heldSeen < n; --num) {
        if (!held[num]) {
            // This is a free card; save it for later.
            ++freeCards;
        } else {
            // This is a held card; must respond.
            ++heldSeen;
            if (freeCards > 0) {
                // Use one saved free card.
                --freeCards;
            } else {
                // No free card available, we lose this round.
                ++losses;
            }
        }
    }
    return losses;
}

#include <cassert>
#include <vector>

// Declaration of the function under test.
int minCardsToHold(int m, int n, const std::vector<int>& playedCards);

int main() {
    // Test 1: m=1,n=1, held card is 1, no free cards -> loss.
    assert(minCardsToHold(1, 1, {1}) == 1);
    
    // Test 2: m=2,n=1, held card is 4 (highest), no free card -> loss.
    assert(minCardsToHold(2, 1, {4}) == 1);
    
    // Test 3: m=2,n=1, held card is 1, there are free cards 4,3,2 -> win.
    assert(minCardsToHold(2, 1, {1}) == 0);
    
    // Test 4: m=2,n=2, held cards are 4 and 3 (both highest), no free -> two losses.
    assert(minCardsToHold(2, 2, {4, 3}) == 2);
    
    // Test 5: m=2,n=2, held cards are 4 and 1. Scan: 4 held, no free -> loss; 3 free, 2 free, 1 held -> use one free -> win. Losses=1.
    assert(minCardsToHold(2, 2, {4, 1}) == 1);
    
    // Test 6: m=3,n=2, held cards 9 and 1. Scan: 9 held -> loss; 8,7,6,5,4,3,2 free; 1 held -> use free -> win. Losses=1.
    assert(minCardsToHold(3, 2, {9, 1}) == 1);
    
    // Test 7: m=3,n=3, held cards 5,4,3. Scan: 9,8,7,6 free (4 free); 5 held -> use free (3 left); 4 held -> use free (2 left); 3 held -> use free (1 left). Losses=0.
    assert(minCardsToHold(3, 3, {5, 4, 3}) == 0);
    
    // Test 8: m=2,n=4 (all numbers held), losses=4.
    assert(minCardsToHold(2, 4, {1, 2, 3, 4}) == 4);
    
    // Test 9: m=2,n=1, held card 2, free cards 4,3 -> win.
    assert(minCardsToHold(2, 1, {2}) == 0);
    
    // Test 10: m=4,n=3, held cards 16,15,14 -> all highest, no free -> 3 losses.
    assert(minCardsToHold(4, 3, {16, 15, 14}) == 3);
    
    return 0;
}

// The problem is a greedy matching: scan from the highest number down to 1. Maintain a count of "available free cards" (numbers not in the held set) that can be used to beat a held card that appears later (lower numbers). When we see a number that is one of the held cards, we must use one available free card if we have any (decrement the counter); if not, we lose that round and increment the loss counter. Because we scan from high to low, any free card encountered before a held card is guaranteed to be larger than that held card, so it's always optimal to use a free card if available. We stop as soon as we have processed all `n` held cards. Edge cases: if the held cards include the highest number `m*n`, no free card exists before it, so that round is lost. If `m=1`, then held cards are all numbers from 1 to n, no free cards, so losses equal `n` (but the scan goes down from n to 1; all held, so losses = n). Complexity: iterate at most `m*n` numbers, but we can break early after processing all held cards, so worst-case O(m*n) time and O(1) extra space if we use a bool array. For large m*n, use a boolean array of size m*n+1.
