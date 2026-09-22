Write a standalone C++ function that takes a vector of domino pairs (each pair is a vector of exactly two integers) and returns the number of pairs of dominoes that are equivalent, where two dominoes are equivalent if they can be rotated to match each other (i.e., `(a,b)` is equivalent to `(b,a)`). The input is guaranteed to be a non-empty vector, each inner vector has exactly two elements, and each element is a positive integer between 1 and 9. The function should count unordered pairs of indices `(i, j)` with `i < j` such that dominoes `i` and `j` are equivalent. For example, given `[[1,2],[2,1],[1,2],[3,4]]`, the equivalent pairs are (0,1), (0,2), and (1,2), so the result is 3. Implement the function with `const` correctness and no global state, returning an `int`.
The key observation is that two dominoes are equivalent if their canonical (ordered) form is identical. The canonical form can be obtained by sorting the two numbers in each pair so that the smaller comes first. For each domino, compute `(min(a,b), max(a,b))` as a key. Use a hash map (or map) to count how many times each canonical key appears. The number of unordered pairs among `k` identical keys is the combination `k choose 2`, i.e., `k * (k - 1) / 2`. Sum these for all distinct keys. The algorithm runs in O(n) time on average (with a hash map) and O(n) worst-case with a `std::map` (O(n log n)), and uses O(n) space for the counts. Edge cases: if all dominoes are distinct, the result is 0; if there are many identical pairs, the combination formula handles it correctly; no need to worry about invalid inputs as the problem guarantees valid data.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Count unordered pairs of equivalent dominoes (a,b) == (b,a).
int countEquivalentDominoPairs(const std::vector<std::vector<int>>& dominoes) {
    std::unordered_map<long long, int> frequency;
    for (const auto& d : dominoes) {
        int a = std::min(d[0], d[1]);
        int b = std::max(d[0], d[1]);
        // Encode pair as a single long long to avoid needing a custom hash.
        long long key = static_cast<long long>(a) * 10 + b;
        ++frequency[key];
    }
    int total = 0;
    for (const auto& [key, count] : frequency) {
        total += count * (count - 1) / 2;
    }
    return total;
}
#include <cassert>
#include <vector>
int main() {
    // Empty-like but non-empty minimal case.
    std::vector<std::vector<int>> d1 = {{1, 1}};
    assert(countEquivalentDominoPairs(d1) == 0);

    // Two equivalent (rotated) dominoes.
    std::vector<std::vector<int>> d2 = {{1, 2}, {2, 1}};
    assert(countEquivalentDominoPairs(d2) == 1);

    // Three identical pairs (all same orientation).
    std::vector<std::vector<int>> d3 = {{1, 2}, {1, 2}, {1, 2}};
    assert(countEquivalentDominoPairs(d3) == 3);

    // Mixed orientations and duplicates.
    std::vector<std::vector<int>> d4 = {{1, 2}, {2, 1}, {1, 2}, {3, 4}};
    assert(countEquivalentDominoPairs(d4) == 3);

    // Same numbers forming a pair with equal values.
    std::vector<std::vector<int>> d5 = {{2, 2}, {2, 2}, {2, 2}, {2, 2}};
    assert(countEquivalentDominoPairs(d5) == 6);

    // Multiple distinct keys with varying counts.
    std::vector<std::vector<int>> d6 = {{1, 2}, {2, 1}, {3, 4}, {4, 3}, {1, 2}, {5, 5}};
    // key (1,2): 3 occurrences → 3 pairs; key (3,4): 2 occurrences → 1 pair; key (5,5): 1 → 0. Total 4.
    assert(countEquivalentDominoPairs(d6) == 4);

    // All distinct no pairs.
    std::vector<std::vector<int>> d7 = {{1, 2}, {3, 4}, {5, 6}};
    assert(countEquivalentDominoPairs(d7) == 0);

    // Large max values within range.
    std::vector<std::vector<int>> d8 = {{9, 1}, {1, 9}, {9, 1}, {1, 9}};
    assert(countEquivalentDominoPairs(d8) == 6);
}
