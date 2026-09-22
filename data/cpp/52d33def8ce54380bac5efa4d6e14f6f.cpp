// Write a C++ function `int scoreHand(int n, const std::string& cards)` that, given a positive integer `n` representing the number of cards in a player's hand and a string `cards` of length `n` containing only uppercase letters `A` through `Z` (each representing a card suit or value), computes a numerical score using the following rules: first, count how many times each distinct card character appears in the hand. For each distinct character, if its total frequency is greater than 1, add the frequency squared to a running total. Then, the final score is `52 - (running total) + (running total - 1)`. In other words, the score simplifies to `51` regardless of the input if you follow the arithmetic correctly, but your function must still perform the described counting and arithmetic. The function must handle typical edge cases: when `n` is 1 (no duplicates), when the string contains all identical letters, when there are many distinct duplicates, and when letters appear a large number of times (up to `n`). Time and space complexity should be linear in `n` (or constant if we consider the fixed 26-letter alphabet).
// The key insight is that the problem is mathematically invariant. Let `D` be the set of distinct characters with frequency > 1, and let `sumFreqSquares` be the sum of squares of frequencies for those characters. The score is `52 - sumFreqSquares + (sumFreqSquares - 1) = 51`. So regardless of the card distribution, the answer is always `51` as long as the input is valid (length `n`, all uppercase letters). However, a robust solution must still verify input constraints and perform the counting. The algorithm: initialize an array of size 26 (for letters A-Z) to zero. Iterate through `cards`, incrementing counts for each character. After counting, iterate through the 26 counts, and for each `count > 1`, add `count*count` to `sumFreqSquares`. Then compute `52 - sumFreqSquares + (sumFreqSquares - 1)` and return. Edge cases: `n=0` (if allowed, return 52 - 0 + (0-1)=51? Actually the problem says positive `n`, but we can handle `n=0` gracefully; the formula yields 51). `n=1` with no duplicates yields 52 - 0 + (0-1)=51. All same letters: if `n=5` and all 'A', sumFreqSquares = 25, score = 52 - 25 + 24 = 51. Time complexity O(n + 26) = O(n), space O(26) = O(1) auxiliary.
#include <string>
#include <array>
#include <cassert>

// Given a hand of n uppercase letters, compute the score as:
// 52 - (sum of squares of frequencies of repeated letters) + (sum - 1)
// The arithmetic simplifies to 51 for any valid input.
int scoreHand(int n, const std::string& cards) {
    // Defensive: if n doesn't match string length, adjust to actual length.
    if (n < 0) n = 0;
    if (static_cast<int>(cards.size()) < n) n = static_cast<int>(cards.size());
    
    std::array<int, 26> freq{};
    for (int i = 0; i < n; ++i) {
        char c = cards[i];
        // Ensure the character is an uppercase letter; if not, skip or assert.
        assert(c >= 'A' && c <= 'Z');
        ++freq[c - 'A'];
    }
    
    int sumSquares = 0;
    for (int count : freq) {
        if (count > 1) {
            sumSquares += count * count;
        }
    }
    
    // The given formula: 52 - sumSquares + (sumSquares - 1) = 51.
    return 52 - sumSquares + (sumSquares - 1);
}
#include <cassert>

// Global main for testing the solution function.
int main() {
    assert(scoreHand(1, "A") == 51);
    assert(scoreHand(5, "AAAAA") == 51);
    assert(scoreHand(4, "ABCD") == 51);
    assert(scoreHand(6, "AABBCC") == 51);
    assert(scoreHand(8, "ABCABCAB") == 51);
    assert(scoreHand(0, "") == 51);   // edge: empty hand
    assert(scoreHand(3, "ZZZ") == 51);
    assert(scoreHand(10, "MIXEDDUPES") == 51); // contains M,I,X,E,D,U,P,S (some repeats)
    assert(scoreHand(26, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 51); // no duplicates
    assert(scoreHand(52, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") == 51); // 52 A's
    return 0;
}
