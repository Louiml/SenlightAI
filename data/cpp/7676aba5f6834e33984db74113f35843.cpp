Write a C++ function `std::string winnerOfGame(const std::vector<int>& apples)` that simulates a two-player game on a circular array of apple piles. The players are Mike and Joe. Mike always moves first. In each turn, the current player removes exactly one apple from the pile with the smallest current number of apples (ties broken by the original index, with the smallest index winning the tie). After removing an apple, if that pile becomes empty, it is removed from the circle. The game ends when all piles are empty. The player who removes the last apple wins. Return `"Mike"` if Mike wins, or `"Joe"` if Joe wins. The input is a non-empty vector of positive integers (each `apples[i]` ≤ 10^9). The answer depends only on the initial configuration, not on the tie-breaking after removals—actually the game is deterministic given the initial array and the rule that you always pick the globally smallest current pile (ties by smallest original index). However, the intended solution simplifies to: if the number of piles is odd, Mike wins; if even, find the index of the unique minimum element in the original array (if ties, the first occurrence), and if that index is even (0-based), Mike wins, otherwise Joe. Your function must implement exactly this logic.
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test.
std::string winnerOfGame(const std::vector<int>& apples);

int main() {
    // Single pile: odd n -> Mike wins.
    assert(winnerOfGame({5}) == "Mike");

    // Two piles, min at index 0 -> Mike wins.
    assert(winnerOfGame({3, 5}) == "Mike");
    // Two piles, min at index 1 -> Joe wins.
    assert(winnerOfGame({5, 3}) == "Joe");

    // Three piles: odd n -> Mike wins.
    assert(winnerOfGame({2, 2, 2}) == "Mike");

    // Four piles, min at index 0 -> Mike wins.
    assert(winnerOfGame({1, 4, 3, 2}) == "Mike");
    // Four piles, min at index 2 -> even index? actually 2 is even, Mike.
    assert(winnerOfGame({4, 3, 1, 2}) == "Mike");
    // Four piles, min at index 1 (odd) -> Joe.
    assert(winnerOfGame({4, 1, 3, 2}) == "Joe");
    // Four piles, min at index 3 (odd) -> Joe.
    assert(winnerOfGame({4, 3, 2, 1}) == "Joe");

    // Even number of piles, all equal minima: first occurrence index 0 -> Mike.
    assert(winnerOfGame({7, 7}) == "Mike");
    // Even number, duplicates of min but first occurrence at odd index.
    assert(winnerOfGame({9, 2, 2, 8}) == "Joe");

    return 0;
}
#include <vector>
#include <string>
#include <limits>
#include <cstdint>

// Determine the winner of the deterministic apple-removal game.
// Returns "Mike" or "Joe" based on the parity of n and the index of the minimum.
std::string winnerOfGame(const std::vector<int>& apples) {
    const std::size_t n = apples.size();

    // If the number of piles is odd, Mike wins.
    if (n % 2 != 0) {
        return "Mike";
    }

    // For even n, find the index of the first occurrence of the minimum.
    int minValue = std::numeric_limits<int>::max();
    std::size_t minIndex = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (apples[i] < minValue) {
            minValue = apples[i];
            minIndex = i;
        }
    }

    // If the minimum is at an even index, Mike wins; otherwise Joe wins.
    return (minIndex % 2 == 0) ? "Mike" : "Joe";
}
// The key observation is that the game is equivalent to a classic Codeforces problem. Since each turn removes one apple from the current minimum pile, the process is deterministic. If the total number of piles `n` is odd, Mike makes the last move because the total number of apples removed is the sum of all elements, and since each move removes exactly one apple, the parity of the total number of moves determines the winner: if total apples is odd, Mike wins, if even, Joe wins. However, that's not the intended simplification. The actual known solution: For odd `n`, Mike wins. For even `n`, the first player (Mike) wins unless the minimum element is at an odd index (0-based) in the original array. Why? Because the game mimics a process where players alternate removing from the current global minimum; when `n` is even, the player who removes from the pile that becomes the minimum first will lose if that pile is at an odd index, because the other player can mirror. More precisely, the minimum value `m` will be removed `m` times (once per apple). Since the pile with the minimum value is the last to be completely depleted? Actually, the known result: If `n` is odd, Mike wins. If `n` is even, look at the position of the first occurrence of the minimum value. If that position is even (0-based), Mike wins; if odd, Joe wins. This is because when `n` is even, the game reduces to a pairing strategy: the minimum pile acts as a "key" and the winner is determined by the parity of its index. Edge cases: all piles equal, multiple minima, n=1 (odd, Mike wins). Time complexity O(n), space O(1) aside from input vector.
