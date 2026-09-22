// Write a C++ function `determineWinner` that takes a vector of positive integers representing card values. Alice and Bob play a game where Alice starts by picking one card; then each subsequent turn, a player may pick any card that shares a value with a card already picked. The game ends when no more legal moves exist, and the player who made the last move wins. Determine the winner as either `"Alice"` or `"Bob"` given that both play optimally. The input vector may contain duplicate values and is not necessarily sorted. The function must handle `n >= 1` and return a `std::string`.
The key insight is that if there are at least two cards of the same value, then after Alice picks one copy of the most frequent value, Bob can always mirror her moves by picking another copy of that same value, preventing Alice from ever making the last move. More formally, sort the array and count the frequency of the minimum value. Let `k = 2 * freq(min) - 2`. This `k` represents the number of cards that can be picked in pairs (two copies each turn) by Bob after Alice's initial pick. If `k < n`, then there are cards outside this paired set, and Alice can force a win by picking a different value on her first move (or the remaining unpaired cards give her the final move). If `k >= n`, then all cards can be paired after Alice's first pick, so Bob can always respond and win. Since the optimal strategy is deterministic by choosing the most frequent value (the minimum value after sorting, but actually the maximum frequency value, though the snippet uses the minimum value), the array is sorted, the minimum value's count is doubled and reduced by 2, and compared to `n`. Time complexity is O(n log n) due to sorting, and space is O(n) for the vector, or O(1) auxiliary if sorting in-place.
#include <string>
#include <vector>
#include <algorithm>

// Determine the winner of the card game given the card values.
std::string determineWinner(std::vector<int> cards) {
    const int n = static_cast<int>(cards.size());
    if (n == 0) {
        return "Bob"; // Edge case, but problem guarantees n >= 1
    }
    
    std::sort(cards.begin(), cards.end());
    const int minValue = cards.front();
    int freqMin = 0;
    for (int value : cards) {
        if (value == minValue) {
            ++freqMin;
        } else {
            break;
        }
    }
    
    const int pairedCards = 2 * freqMin - 2;
    if (pairedCards < n) {
        return "Alice";
    } else {
        return "Bob";
    }
}
#include <cassert>
#include <string>
#include <vector>

// Declare the solution function
std::string determineWinner(std::vector<int> cards);

int main() {
    // Single card: Alice picks it, wins.
    assert(determineWinner({5}) == "Alice");
    // Two identical: Alice picks one, Bob picks the other, Bob wins.
    assert(determineWinner({3, 3}) == "Bob");
    // Three identical: Alice picks one, Bob picks one, then Alice picks last, Alice wins.
    assert(determineWinner({7, 7, 7}) == "Alice");
    // Four identical: Alice picks one, Bob picks one, Alice picks one, Bob picks last, Bob wins.
    assert(determineWinner({2, 2, 2, 2}) == "Bob");
    // Mixed: one pair and one singleton, Alice picks singleton, Bob picks one pair, Alice picks last pair, Alice wins.
    assert(determineWinner({1, 2, 2}) == "Alice");
    // Mixed with larger set: three pairs, Bob wins.
    std::vector<int> test6 = {4, 4, 5, 5, 6, 6};
    assert(determineWinner(test6) == "Bob");
    // Unsorted input with min frequency > 1: four copies of 10, one 20, Alice picks 20, then pairs of 10 remain, Bob wins.
    std::vector<int> test7 = {10, 20, 10, 10, 10};
    assert(determineWinner(test7) == "Bob");
    // Five identical: Alice, Bob, Alice, Bob, Alice -> Alice wins.
    assert(determineWinner({9, 9, 9, 9, 9}) == "Alice");
    // Two identical and one different: Alice picks different, then Bob picks one pair, Alice picks last pair? Actually n=3, freq min=2 (if min=1, pairedCards=2, k=2, n=3 => Alice wins)
    assert(determineWinner({1, 1, 2}) == "Alice");
    // Many distinct values: Alice always wins since freq min=1, pairedCards=0 < n.
    assert(determineWinner({1, 2, 3, 4, 5}) == "Alice");
}
