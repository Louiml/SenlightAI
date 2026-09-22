Write a C++ function that simulates the card game "War" between two players using their provided decks. The input consists of two lists of cards (each card is a string like "2H", "10S", "KD", etc.), where the rank is the first character (or first two characters for "10") and the suit is the last character. Ranks are ordered (low to high): 2, 3, 4, 5, 6, 7, 8, 9, 1 (for 10), J, Q, K, A. The game proceeds in rounds: each round, both players reveal their top card. If their ranks differ, the player with the higher rank takes both cards and places them at the bottom of their deck (the losing player's cards first, then the winner's). If the ranks are equal, a "war" occurs: each player places the next 3 cards face down (if a player runs out of cards mid-war, that player loses, and the game ends with "PAT"), then reveals another card; the higher rank wins all cards on the table (the face-down cards and the revealed cards, in that order) and adds them to the bottom of their deck. Continue until one player has no cards. Your function should take two `std::deque<std::string>` representing player 1's and player 2's decks (with front as top of deck) and return a `std::string` containing either `"1 X"` (if player 1 wins after X rounds), `"2 X"` (if player 2 wins after X rounds), or `"PAT"` (if a war cannot be completed because a player runs out of cards). The function must not modify the input decks, so operate on copies. You may assume card strings always have a valid rank character (first char, or first two for "10") and a suit, and that all inputs are well-formed.
The solution simulates the game exactly as described using two copies of the input deques. Key points: (1) A rank-comparison function maps each rank character (or '1' for the "10" rank) to its index in the ordered string "234567891JQKA", where lower index means lower rank. The comparison returns a negative value if card1's rank is higher than card2's, zero if equal, and positive otherwise, matching the original code's convention. (2) In each round, pop the front card from each deck into temporary "table" deques. If ranks differ, the winner appends both temporary deques (loser's first, then winner's) to the bottom of their own deck. If ranks are equal, we enter a war loop: attempt to pop 3 cards from each deck (if a deck is empty at any pop, return "PAT"), then reveal one more card from each. Continue the war loop as long as the revealed ranks are equal. After the war resolves, the winner collects all cards from the table in order (they were appended in the order they were popped, so the table deques already preserve the sequence). (3) The game ends when one deck is empty; if player 2's deck is empty, player 1 wins, and vice versa. The round counter is incremented each time a non-war outcome or a resolved war occurs. Edge cases include empty decks at game start (impossible given problem statement but safe to return "1 0" or "2 0"), running out of cards during a war (returns "PAT"), and repeating wars. Time complexity is O(r * c) where r is the number of rounds and c is the total number of cards, since each card is moved and compared a constant number of times; in the worst case, the game can take O(c^2) rounds if cards cycle, but with fixed ranks it terminates. Space complexity is O(c) for the temporary deques and copies.
#include <deque>
#include <string>
#include <algorithm>

// Compare two card ranks (using first character of card string; "10" uses '1').
// Returns a value < 0 if rank1 > rank2, 0 if equal, > 0 if rank1 < rank2.
int rankCompare(char rank1, char rank2) {
    const std::string order = "234567891JQKA";
    int idx1 = order.find(rank1);
    int idx2 = order.find(rank2);
    if (idx1 == idx2) return 0;
    else if (idx1 > idx2) return -1;
    else return 1;
}

// Simulate the game of War. Returns "1 X", "2 X", or "PAT".
std::string playWar(std::deque<std::string> deck1, std::deque<std::string> deck2) {
    std::deque<std::string> table1, table2;
    int rounds = 0;

    while (!deck1.empty() && !deck2.empty()) {
        table1.push_back(deck1.front());
        deck1.pop_front();
        table2.push_back(deck2.front());
        deck2.pop_front();

        // War loop: while the revealed ranks match, draw 3 face-down + 1 face-up.
        while (rankCompare(table1.back()[0], table2.back()[0]) == 0) {
            // Draw 3 cards each; if any draw fails because a deck is empty, return PAT.
            for (int i = 0; i < 3; ++i) {
                if (deck1.empty() || deck2.empty()) return "PAT";
                table1.push_back(deck1.front());
                deck1.pop_front();
                table2.push_back(deck2.front());
                deck2.pop_front();
            }
            // Reveal one more card each; if a deck is empty now, return PAT.
            if (deck1.empty() || deck2.empty()) return "PAT";
            table1.push_back(deck1.front());
            deck1.pop_front();
            table2.push_back(deck2.front());
            deck2.pop_front();
        }

        // Determine winner: rankCompare returns negative if table1.back() > table2.back() (player 1 wins).
        if (rankCompare(table1.back()[0], table2.back()[0]) < 0) {
            // Player 1 takes all table cards in order (player 2's first, then player 1's).
            deck1.insert(deck1.end(), table2.begin(), table2.end());
            deck1.insert(deck1.end(), table1.begin(), table1.end());
        } else {
            deck2.insert(deck2.end(), table1.begin(), table1.end());
            deck2.insert(deck2.end(), table2.begin(), table2.end());
        }

        table1.clear();
        table2.clear();
        ++rounds;
    }

    if (deck1.empty() && deck2.empty()) {
        // Should not happen; but treat as player 2 wins (last round gone wrong).
        return "PAT";
    } else if (deck2.empty()) {
        return "1 " + std::to_string(rounds);
    } else {
        return "2 " + std::to_string(rounds);
    }
}
#include <cassert>
#include <deque>
#include <string>

// Declaration of the function under test (from solution).
std::string playWar(std::deque<std::string> deck1, std::deque<std::string> deck2);

int main() {
    // Example from typical problem: Player 1 has 3 cards, Player 2 has 3 cards.
    // 10H > 9H, so player 1 wins round 1, takes both cards, etc.
    {
        std::deque<std::string> d1 = {"10H", "2S", "QD"};
        std::deque<std::string> d2 = {"9H", "3C", "KD"};
        assert(playWar(d1, d2) == "1 3");
    }

    // Immediate win: one card each, player 2 wins.
    {
        std::deque<std::string> d1 = {"2H"};
        std::deque<std::string> d2 = {"AS"};
        assert(playWar(d1, d2) == "2 1");
    }

    // PAT: war occurs, but player 2 runs out of cards during the face-down phase.
    {
        std::deque<std::string> d1 = {"10H", "2S", "3D", "4C", "5H"};
        std::deque<std::string> d2 = {"10D", "2C"}; // only 2 cards -> during first war, cannot draw 3
        assert(playWar(d1, d2) == "PAT");
    }

    // PAT: war occurs, both players can draw 3 and reveal, but player 2 runs out before reveal.
    {
        std::deque<std::string> d1 = {"KH", "3S", "4D", "5C", "6H"};
        std::deque<std::string> d2 = {"KD", "2S", "3C"}; // 3 cards exactly -> after drawing 3, deck2 empty before reveal
        assert(playWar(d1, d2) == "PAT");
    }

    // Same ranks but different suits: must trigger war and eventually resolve.
    {
        std::deque<std::string> d1 = {"7H", "2S", "3D", "4C", "5H"};
        std::deque<std::string> d2 = {"7S", "AS", "KD", "QC", "JH"};
        // Round 1: 7 vs 7 -> war, draw 3 face-down each (cards: 2,3,4 vs A,K,Q), reveal 5 vs J -> J higher, player 2 takes all.
        assert(playWar(d1, d2) == "2 1");
    }

    // Player 2 wins after several rounds.
    {
        std::deque<std::string> d1 = {"2H", "3S", "4D"};
        std::deque<std::string> d2 = {"5C", "6H", "7D"};
        // Round1: 2 vs 5 -> p2 wins, round2: 3 vs 6 -> p2 wins, round3: 4 vs 7 -> p2 wins.
        assert(playWar(d1, d2) == "2 3");
    }

    // Player 1 wins after several rounds.
    {
        std::deque<std::string> d1 = {"10C", "9S", "8D"};
        std::deque<std::string> d2 = {"2H", "3C", "4D"};
        // Round1: 10 vs 2 -> p1 wins, round2: 9 vs 3 -> p1 wins, round3: 8 vs 4 -> p1 wins.
        assert(playWar(d1, d2) == "1 3");
    }

    return 0;
}
