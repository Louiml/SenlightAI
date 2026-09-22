/*
Write a C++ function that computes the total score for a single player in a dart-throwing game. The function takes three pairs of double-precision coordinates `(x, y)` representing three dart hits on a circular dartboard centered at the origin. Each hit earns points based on its squared distance `d = x² + y²` from the center, according to the following bands: if `d <= 9.0`, the hit is worth 100 points; if `9.0 < d <= 36.0`, worth 80 points; if `36.0 < d <= 81.0`, worth 60 points; if `81.0 < d <= 144.0`, worth 40 points; if `144.0 < d <= 225.0`, worth 20 points; if `d > 225.0`, the hit scores 0 points. The function should accept the coordinates in a convenient structure (for example, three separate pairs, or two arrays of size 3) and return the integer total score for those three throws. The function must be `const`-correct, use only standard headers, and not rely on any global state.
*/

#include <vector>
#include <utility>

// Compute the total score for three dart throws given as (x, y) coordinates.
// Each pair is a hit on a circular board centered at the origin.
int computeDartScore(const std::vector<std::pair<double, double>>& hits) {
    int total = 0;
    for (const auto& hit : hits) {
        double d = hit.first * hit.first + hit.second * hit.second;
        if (d <= 9.0) {
            total += 100;
        } else if (d <= 36.0) {
            total += 80;
        } else if (d <= 81.0) {
            total += 60;
        } else if (d <= 144.0) {
            total += 40;
        } else if (d <= 225.0) {
            total += 20;
        }
        // else: score 0 for this hit
    }
    return total;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (as in solution)
int computeDartScore(const std::vector<std::pair<double, double>>& hits);

int main() {
    // All three hits in the bullseye (d = 0) -> 100+100+100 = 300
    assert(computeDartScore({{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}) == 300);
    // Hits at distance exactly 3 (d=9) -> 100; at sqrt(36)=6; at sqrt(81)=9 -> 60
    assert(computeDartScore({{3.0, 0.0}, {0.0, 6.0}, {9.0, 0.0}}) == 100 + 80 + 60);
    // Boundary just above 225 (d=225.0001) -> 0 for that hit
    assert(computeDartScore({{15.0, 0.0}, {0.0, 15.0}, {15.0, 0.01}}) == 20 + 20 + 0);
    // Negative coordinates and mixed bands
    assert(computeDartScore({{-3.0, 0.0}, {0.0, -6.0}, {9.0, 9.0}}) == 100 + 80 + 20);
    // All misses far away
    assert(computeDartScore({{100.0, 0.0}, {0.0, 100.0}, {200.0, 0.0}}) == 0);
    // Exact distance sqrt(144)=12 -> 40; sqrt(225)=15 -> 20; combination
    assert(computeDartScore({{12.0, 0.0}, {0.0, 15.0}, {0.0, 0.0}}) == 40 + 20 + 100);
    // Two hits same coordinate
    assert(computeDartScore({{2.0, 2.0}, {2.0, 2.0}, {0.0, 0.0}}) == 100 + 100 + 100);
    // Score with one hit exactly at d=9 (100), one at d=36 (80), one at d=81 (60)
    assert(computeDartScore({{3.0, 0.0}, {6.0, 0.0}, {9.0, 0.0}}) == 240);
    // Verify with original snippet's example: player 1 gets 100+80+60=240, player 2 gets 40+20+0=60
    assert(computeDartScore({{1.0, 2.0}, {4.0, 4.0}, {9.0, 0.0}}) == 240);
    assert(computeDartScore({{10.0, 0.0}, {15.0, 0.0}, {20.0, 0.0}}) == 20 + 20 + 0);
    return 0;
}

// The solution directly applies the scoring rules to each of the three dart hits. For each hit, compute the squared distance from the origin (avoiding a square-root, which is unnecessary and less efficient). Compare the squared distance against the squared radii thresholds (9, 36, 81, 144, 225) in descending order, so the first matching condition yields the highest applicable score; if none match, the score is 0. Sum the three hit scores and return the total. Edge cases: coordinates exactly on a boundary (e.g., `d == 9.0`) must be counted in the inner band (100 points) because the condition uses `<=`; coordinates negative or large are handled by the squared distance being non-negative, and the `else if` chain naturally handles values above 225. Time complexity is O(1) since there are exactly three hits and a fixed number of comparisons per hit; space complexity is O(1) because only a few local variables are used.
