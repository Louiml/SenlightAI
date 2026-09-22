/*
Given a list of dominoes, where each domino is represented as a pair of integers between 1 and 9 inclusive (e.g., `[2,1]`), write a C++ function that returns the number of pairs `(i, j)` with `i < j` such that domino `i` is equivalent to domino `j`. Two dominoes are considered equivalent if they have the same two numbers, regardless of order (e.g., `[1,2]` is equivalent to `[2,1]`). The input is a `vector<vector<int>>`, where each inner vector has exactly two elements. The function should handle empty and single-element inputs, and must not modify the input.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the number of pairs (i, j) with i < j such that the two dominoes
// are equivalent (same two numbers, order ignored).
int countEquivalentDominoPairs(const std::vector<std::vector<int>>& dominoes) {
    std::unordered_map<int, int> seen;
    int totalPairs = 0;
    
    for (const auto& domino : dominoes) {
        int a = std::min(domino[0], domino[1]);
        int b = std::max(domino[0], domino[1]);
        int key = a * 10 + b;
        totalPairs += seen[key];
        ++seen[key];
    }
    return totalPairs;
}

#include <cassert>
#include <vector>

int countEquivalentDominoPairs(const std::vector<std::vector<int>>& dominoes);

int main() {
    // Basic case with ordered equivalent pairs
    assert(countEquivalentDominoPairs({{1, 2}, {2, 1}, {3, 4}, {5, 6}}) == 1);
    
    // Multiple equivalent pairs
    assert(countEquivalentDominoPairs({{1, 2}, {2, 1}, {1, 2}, {2, 1}}) == 6);
    
    // Single domino
    assert(countEquivalentDominoPairs({{3, 3}}) == 0);
    
    // Empty input
    assert(countEquivalentDominoPairs({}) == 0);
    
    // Identical numbers on a domino
    assert(countEquivalentDominoPairs({{1, 1}, {1, 1}, {1, 1}}) == 3);
    
    // Mixed pairs with no equivalences
    assert(countEquivalentDominoPairs({{1, 2}, {3, 4}, {5, 6}, {8, 9}}) == 0);
    
    // Large input with repeated pattern (e.g., all same pair)
    std::vector<std::vector<int>> large(10, {4, 4});
    assert(countEquivalentDominoPairs(large) == 45); // 10 choose 2 = 45
    
    // Dominoes with numbers in different order but not equivalent to others
    assert(countEquivalentDominoPairs({{1, 2}, {2, 3}, {3, 1}}) == 0);
    
    return 0;
}

// We need to count how many unordered pairs of indices are equivalent. Because order within a domino doesn't matter, we can normalize each domino by sorting its two values: let `a = min(x[0], x[1])` and `b = max(x[0], x[1])`. Since values are between 1 and 9, we can encode the pair as a single integer key `a * 10 + b` (which uniquely represents the unordered pair, e.g., `[1,2]` → 12, `[2,1]` → 12, but `[1,3]` → 13 ≠ 12). We iterate through the dominoes, keeping a hash map from key to the count of previously seen equivalent dominoes. For each current domino, the number of equivalent pairs that include this domino with any earlier one equals the current count stored for that key. We add that count to the total, then increment the map entry. This works because each new domino forms one pair with each previous equivalent one. Edge cases: an empty vector yields 0; a vector with a single domino yields 0 because no pairs exist; duplicated identical dominoes (e.g., `[[1,1],[1,1]]`) are handled correctly since the key for `[1,1]` is 11. Time complexity is O(n) where n is the number of dominoes, and space complexity is O(k) where k is the number of distinct normalized pairs (at most 45 possible keys since 1 ≤ a ≤ b ≤ 9, but at most n in the worst case).
