Write a C++ function `minimumPlaysToFinish` that takes two vectors of non-negative integers, `one` and `two` (both of length N), representing the star requirements for a game with N levels. Each level has two stars: the first star requires `one[i]` stars, and the second star requires `two[i]` stars. A player starts with 0 stars and can play a level at most twice: the first time they earn 1 star (if they have at least `one[i]` stars), and the second time they earn 2 stars (if they have at least `two[i]` stars, regardless of whether they already earned the first star). The player must earn all 2N stars to finish. The function should return the minimum number of plays needed to reach 2N stars, or -1 if impossible. Levels can be played in any order, and partial progress (earning only the first star) is allowed if the requirements are met.
// This is a greedy problem. The optimal strategy is to always take the highest-value available move: first, always play any level's second star if possible (since it gives 2 stars and may require a low threshold). If no second-star move is available, then play a first star, but among all available first-star moves (where `one[i] <= currentStars` and the level hasn't been played at all), choose the one with the largest `two[i]` requirement. This is because playing the first star on a level with a high second-star requirement is "safer" – it preserves levels with low second-star requirements for later. If no second-star and no first-star move exists, the game is impossible. The algorithm simulates this greedy loop. Edge cases: N=0 (trivially finish in 0 plays), levels where `one[i] > two[i]` (possible but still valid; the first-star requirement can be higher than the second, but usually not), and duplicate requirements. Time complexity is O(N^2) in the worst case because each play scans the vector; since at most 2N plays, this is O(N^2). Space complexity is O(N) for the `got` vector tracking how many stars each level has earned (0, 1, or 2). The greedy proof: choosing the highest second-star threshold among first-star moves is optimal because it delays using levels with lower thresholds, making them available for later second-star completions; any other choice can only reduce future options.
#include <vector>
#include <algorithm>

// Returns minimum plays to earn all 2N stars, or -1 if impossible.
int minimumPlaysToFinish(const std::vector<int>& one, const std::vector<int>& two) {
    int n = one.size();
    if (n == 0) return 0;

    std::vector<int> got(n, 0); // 0 = not played, 1 = first star earned, 2 = second star earned
    int stars = 0;
    int plays = 0;

    while (stars < 2 * n) {
        // First, try any level for which we can earn the second star now.
        int best = -1;
        for (int i = 0; i < n; ++i) {
            if (got[i] == 0 && two[i] <= stars) {
                best = i;
                break;
            }
            if (got[i] == 1 && two[i] <= stars) {
                best = i;
                break;
            }
        }
        if (best != -1) {
            int extra = (got[best] == 0) ? 2 : 1;
            got[best] = 2;
            stars += extra;
            plays++;
            continue;
        }

        // No second-star move available, find best first-star move.
        int maxTwo = -1;
        best = -1;
        for (int i = 0; i < n; ++i) {
            if (got[i] == 0 && one[i] <= stars && two[i] > maxTwo) {
                maxTwo = two[i];
                best = i;
            }
        }
        if (best != -1) {
            got[best] = 1;
            stars += 1;
            plays++;
            continue;
        }

        // No move possible.
        return -1;
    }

    return plays;
}
#include <cassert>
#include <vector>
#include <iostream>

int minimumPlaysToFinish(const std::vector<int>& one, const std::vector<int>& two);

int main() {
    // Simple cases
    assert(minimumPlaysToFinish({0, 0}, {1, 2}) == 3); // Play both first stars, then second for level 0, then second for level 1
    assert(minimumPlaysToFinish({1, 1}, {2, 2}) == 4); // Play first on each, then second on each
    assert(minimumPlaysToFinish({0}, {0}) == 1); // One level gives 2 stars immediately
    assert(minimumPlaysToFinish({10}, {10}) == -1); // Cannot even start
    assert(minimumPlaysToFinish({}, {}) == 0); // No levels

    // Greedy selection test
    // Level 0: first requires 0, second requires 5
    // Level 1: first requires 2, second requires 2
    // Best: play level 0 first (first star), then level 1 first, then level 1 second, then level 0 second
    // Plays: 4
    assert(minimumPlaysToFinish({0, 2}, {5, 2}) == 4);

    // Impossible where greedily picking high two first star leads to dead end
    // Level 0: first 0, second 3; Level 1: first 1, second 1
    // Correct: play level0 first, level1 first, level1 second, level0 second -> 4
    assert(minimumPlaysToFinish({0, 1}, {3, 1}) == 4);

    // Another test
    assert(minimumPlaysToFinish({1, 2, 1}, {3, 3, 2}) == 5); // Possible in 5 plays

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
