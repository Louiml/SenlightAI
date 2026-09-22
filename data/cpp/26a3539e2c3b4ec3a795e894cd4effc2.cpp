// Write a C++ function that, given a vector of dominoes where each domino is represented as a vector of two integers (each between 0 and 9 inclusive), returns the number of pairs `(i, j)` with `i < j` such that the two dominoes are equivalent. Two dominoes are equivalent if they contain the same two numbers, regardless of order; for example `[1,2]` is equivalent to `[2,1]`. The input vector may be empty, may contain duplicate dominoes, and may contain pairs with equal values (e.g., `[3,3]`). The function must count every pair exactly once, including pairs formed from three or more identical dominoes (e.g., three `[1,2]` produce 3 pairs).

#include <cassert>
#include <vector>

// Forward declaration of the function under test.
int countEquivalentDominoPairs(const std::vector<std::vector<int>>& dominoes);

int main() {
    // Empty list: no pairs.
    assert(countEquivalentDominoPairs({}) == 0);

    // Single domino: no pairs.
    assert(countEquivalentDominoPairs({{1, 2}}) == 0);

    // Basic equivalent pair in same and reversed order.
    assert(countEquivalentDominoPairs({{1, 2}, {2, 1}}) == 1);

    // No equivalent pairs (all different values).
    assert(countEquivalentDominoPairs({{1, 2}, {3, 4}, {5, 6}}) == 0);

    // Three identical dominoes: C(3,2)=3 pairs.
    assert(countEquivalentDominoPairs({{1, 2}, {1, 2}, {1, 2}}) == 3);

    // Mixed: two [1,2] (1 pair) and three [3,4] (3 pairs) => total 4.
    assert(countEquivalentDominoPairs({{1, 2}, {3, 4}, {2, 1}, {4, 3}, {3, 4}}) == 4);

    // Domino with equal numbers counts normally.
    assert(countEquivalentDominoPairs({{3, 3}, {3, 3}}) == 1);
    assert(countEquivalentDominoPairs({{0, 9}, {9, 0}, {5, 5}, {9, 0}}) == 3);

    // Larger test with many duplicates: five [0,1] => C(5,2)=10 pairs.
    std::vector<std::vector<int>> big = {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
    assert(countEquivalentDominoPairs(big) == 10);

    return 0;
}

#include <vector>
#include <algorithm>

// Count the number of equivalent domino pairs in the input vector.
// Each domino is a vector of two integers in [0, 9].
// Two dominoes are equivalent if they contain the same two numbers in any order.
int countEquivalentDominoPairs(const std::vector<std::vector<int>>& dominoes) {
    // Frequency array for normalized keys: min*10 + max, range 0..99.
    std::vector<int> frequency(100, 0);
    int pairCount = 0;

    for (const auto& domino : dominoes) {
        int first = domino[0];
        int second = domino[1];
        int key = std::min(first, second) * 10 + std::max(first, second);

        // The current frequency of this key is the number of prior dominoes
        // that are equivalent to the current one, so those all form pairs.
        pairCount += frequency[key];
        frequency[key]++;
    }

    return pairCount;
}

// The key observation is that a domino `[a, b]` can be normalized by always ordering its two numbers as `(min, max)` so that equivalent dominoes map to the same canonical key. Since each number is between 0 and 9, the normalized pair can be encoded as a two-digit integer: `min * 10 + max`. This gives a compact representation in the range 0–99. We iterate through the list once, maintaining a frequency array of size 100. For each domino, before incrementing its frequency, we add the current frequency of that key to the answer because that represents the number of previously seen dominoes that are equivalent to the current one. This counts each pair exactly once as we encounter the second (or later) element of the pair. For a list of `n` dominoes, the time complexity is `O(n)` and the auxiliary space is `O(1)` (the frequency array is fixed at 100). Edge cases include: an empty list returns 0, a single domino returns 0, dominoes with equal numbers (e.g., `[3,3]`) are treated normally—their normalized key is `3*10+3 = 33`, and identical pairs will be counted correctly. The approach avoids sorting each domino which would be unnecessary and slower.
