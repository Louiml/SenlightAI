// You are given an array of `n` positive integers representing the skill levels of players, and a threshold value `k`. A team can be formed by selecting any subset of the players, and a team is considered "valid" if the product of the team size and the minimum skill level in that team is at least `k`. Your task is to write a C++ function `maxValidTeams` that takes the array of skill levels and the threshold `k`, and returns the maximum number of disjoint valid teams that can be formed from the given players. Each player can belong to at most one team, and you may leave some players unused. You need to maximize the count of valid teams. The function should handle multiple test cases efficiently.

#include <cassert>
#include <vector>

int maxValidTeams(std::vector<int> skills, int k);

int main() {
    // Basic case
    assert(maxValidTeams({1, 2, 3, 4, 5}, 6) == 2);
    // All players individually valid
    assert(maxValidTeams({10, 20, 30}, 1) == 3);
    // No valid team possible
    assert(maxValidTeams({1, 1, 1}, 100) == 0);
    // Large k requires one big team
    assert(maxValidTeams({1, 1, 1, 1, 100}, 200) == 1);
    // Single player forming a team
    assert(maxValidTeams({100}, 50) == 1);
    // Edge with equal skills
    assert(maxValidTeams({5, 5, 5}, 10) == 1);
    // Mixed skills, optimal grouping
    assert(maxValidTeams({2, 3, 4, 8, 9}, 10) == 2);
    // Already sorted input
    assert(maxValidTeams({1, 2, 3}, 3) == 1);
    // Unsorted input with multiple teams
    assert(maxValidTeams({4, 1, 3, 2, 5}, 6) == 2);
}

#include <vector>
#include <algorithm>

// Returns the maximum number of disjoint valid teams.
// A team of size s with minimum skill m is valid if s * m >= k.
int maxValidTeams(std::vector<int> skills, int k) {
    std::sort(skills.begin(), skills.end());
    int teams = 0;
    int teamSize = 0;
    // Process from highest skill downwards.
    for (int i = static_cast<int>(skills.size()) - 1; i >= 0; --i) {
        ++teamSize;
        if (static_cast<long long>(teamSize) * skills[i] >= k) {
            ++teams;
            teamSize = 0;
        }
    }
    return teams;
}

// The key insight is that to maximize the number of teams, we should sort the skill levels in ascending order and then greedily form teams starting from the highest skill levels. A team of size `s` with minimum skill `m` is valid if `s * m >= k`. If we process players from highest skill to lowest, we can accumulate players until the product condition is met, then finalize a team and reset the count. This works because using higher-skilled players first makes it easier to satisfy the condition with fewer players, leaving more players for other teams. Sorting takes `O(n log n)` time, and the greedy scan takes `O(n)` time, so overall `O(n log n)` per test case. Space complexity is `O(n)` for the vector copy (or we can sort in place). Edge cases: if `k=1`, every single player forms a team (since `1 * skill >= 1` always). If no team can be formed, return 0. The greedy approach is optimal because any valid team requires at least `ceil(k/min_skill)` players, and taking the highest skills first minimizes the number of players needed per team, maximizing the total count.
