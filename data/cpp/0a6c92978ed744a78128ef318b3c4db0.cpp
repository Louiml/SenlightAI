// Write a C++ function `bool hasMatchingCard(const std::vector<std::string>& hand, const std::string& target)` that takes a vector of exactly 5 two-character card strings (e.g., `"Ah"`, `"Kd"`, `"Qs"`, `"Jc"`, `"10"` is invalid because it has more than 2 chars) and a target string `target` consisting of exactly 2 characters (first char = rank, second char = suit). The function returns `true` if there exists at least one card in the hand that either matches the target's rank (first character) OR matches the target's suit (second character). Otherwise, return `false`. The input hand will always contain exactly 5 valid two-character strings, and the target will always be valid (exactly 2 characters). Note: The card strings may have uppercase or lowercase letters, but characters are compared case‑sensitively (so `'A'` != `'a'`). The original snippet reads from `stdin`, but your function must be pure and return a `bool`.
The solution is straightforward: iterate over all 5 cards in the hand. For each card, compare its first character (index 0) to the target's first character (rank) OR its second character (index 1) to the target's second character (suit). If any card satisfies the condition, return `true` immediately. If the loop finishes with no match, return `false`. Edge cases: The hand always has exactly 5 cards and each card is guaranteed to have length 2, so no need to check size. The condition is an inclusive OR — a card matches if it shares either the rank or the suit. Duplicate cards are irrelevant; the function only needs to know if at least one match exists. Time complexity is O(1) because the hand size is fixed at 5. Space complexity is O(1) since only a few local variables are used (no extra data structures). The function should be `const`-correct: take parameters by `const std::vector<std::string>&` and `const std::string&` to avoid unnecessary copies.
#include <string>
#include <vector>

// Returns true if any card in the hand matches the target's rank or suit.
bool hasMatchingCard(const std::vector<std::string>& hand, const std::string& target) {
    for (const std::string& card : hand) {
        if (card[0] == target[0] || card[1] == target[1]) {
            return true;
        }
    }
    return false;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here or include the header.
bool hasMatchingCard(const std::vector<std::string>& hand, const std::string& target) {
    for (const std::string& card : hand) {
        if (card[0] == target[0] || card[1] == target[1]) {
            return true;
        }
    }
    return false;
}

int main() {
    // Exact match on rank
    assert(hasMatchingCard({"Ah", "Kd", "Qs", "Jc", "10"}, "As") == true);
    // Exact match on suit
    assert(hasMatchingCard({"Ah", "Kd", "Qs", "Jc", "10"}, "2h") == true);
    // No match
    assert(hasMatchingCard({"Ah", "Kd", "Qs", "Jc", "10"}, "3c") == false);
    // Case sensitivity: lowercase 'a' does not match uppercase 'A'
    assert(hasMatchingCard({"Ah", "Kd", "Qs", "Jc", "10"}, "as") == false);
    // All cards share the target's suit
    assert(hasMatchingCard({"2h", "3h", "4h", "5h", "6h"}, "Kh") == true);
    // All cards share the target's rank
    assert(hasMatchingCard({"7s", "7d", "7c", "7h", "7x"}, "7d") == true);
    // Only the last card matches
    assert(hasMatchingCard({"2s", "3d", "4c", "5h", "6s"}, "6d") == true);
    // Only the first card matches
    assert(hasMatchingCard({"2s", "3d", "4c", "5h", "6s"}, "2h") == true);
    // Target suit appears in multiple cards
    assert(hasMatchingCard({"Ah", "Kd", "Qs", "Jc", "10"}, "9d") == true);
    // Empty target is not expected, but function still handles: compare char by char
    // Not testing this because target is guaranteed valid per spec.
    return 0;
}
