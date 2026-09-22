/*
Write a C++ function that takes two parallel vectors of integers, `homeColors` and `awayColors`, each of length `n` (where `1 ≤ n ≤ 1000`), and returns the total number of pairs `(i, j)` such that the home team's color in game `i` equals the away team's color in game `j`. Each game is uniquely identified by its index: for game `i`, `homeColors[i]` is the host's uniform color and `awayColors[i]` is the visiting team's uniform color. The function must compute the count of all ordered index pairs `(i, j)` where the home color at index `i` matches the away color at index `j`. Note that the same game can contribute to the count if its own home and away colors match (i.e., when `i == j`), and different games can also match. Input values are integers between 1 and 100 (inclusive). The function should handle duplicates, and the vectors may contain repeated colors. The result can be as large as `n * n` (up to 1,000,000), so use a 64‑bit integer for the return type. Implement the function as a free function named `countHomeAwayMatches` that takes the two vectors by `const` reference and returns a `long long`.
*/
#include <vector>

// Counts all ordered pairs (i, j) where homeColors[i] equals awayColors[j].
long long countHomeAwayMatches(const std::vector<int>& homeColors,
                               const std::vector<int>& awayColors) {
    long long matches = 0;
    const int n = static_cast<int>(homeColors.size());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (homeColors[i] == awayColors[j]) {
                ++matches;
            }
        }
    }
    return matches;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with a single game where home and away colors match.
    std::vector<int> h1 = {1};
    std::vector<int> a1 = {1};
    assert(countHomeAwayMatches(h1, a1) == 1);

    // Two games, only one cross match.
    std::vector<int> h2 = {1, 2};
    std::vector<int> a2 = {2, 3};
    assert(countHomeAwayMatches(h2, a2) == 1);

    // Every home color matches every away color (all colors equal).
    std::vector<int> h3 = {5, 5, 5};
    std::vector<int> a3 = {5, 5};
    // But vectors must be same length; adjust.
    std::vector<int> a3b = {5, 5, 5};
    assert(countHomeAwayMatches(h3, a3b) == 9);

    // No matches at all.
    std::vector<int> h4 = {1, 2, 3};
    std::vector<int> a4 = {4, 5, 6};
    assert(countHomeAwayMatches(h4, a4) == 0);

    // Mixed duplicates, self‑matches and cross‑matches.
    std::vector<int> h5 = {1, 2, 1};
    std::vector<int> a5 = {1, 1, 3};
    // (0,0), (0,1), (2,0), (2,1) = 4
    assert(countHomeAwayMatches(h5, a5) == 4);

    // Larger n=1000 all same color, result is 1,000,000.
    std::vector<int> h6(1000, 7);
    std::vector<int> a6(1000, 7);
    assert(countHomeAwayMatches(h6, a6) == 1000000LL);

    return 0;
}
// The problem is a straightforward nested‑loop counting problem. For every home color at index `i`, we scan every away color at index `j` and increment a counter if they are equal. Because the constraints are small (n ≤ 1000), an O(n²) algorithm is acceptable. There are no special edge cases beyond ensuring the sum fits in a 64‑bit integer (max 1,000,000, well within `long long`). We also consider that the same index `i` and `j` can refer to the same game, which is allowed, and duplicates are handled naturally by the comparison. The time complexity is O(n²) and the space complexity is O(1) beyond the input vectors themselves.
