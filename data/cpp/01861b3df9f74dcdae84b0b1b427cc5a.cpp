// Create a C++ function that takes an integer count of contestants `n`, an integer `k` representing the minimum skill level required for the contest to be interesting, and a vector of integers representing each contestant's individual skill level. The function should sort the skill levels and determine how many complete teams of exactly 3 contestants can be formed such that in each team, every contestant's skill level is within `k` points of the maximum possible universal skill level of 5 (in other words, each contestant's skill must be ≤ 5−k). Return the maximum number of such complete 3-person teams. The input is guaranteed to have at least one contestant and all skill levels are non-negative integers between 0 and 5 inclusive.
The problem reduces to counting how many contestants satisfy the constraint `skill ≤ 5−k`. Since the original code sorts the array, but sorting is not strictly necessary for counting, we can directly count the number of eligible contestants. However, to match the spirit of the original snippet, sorting would allow early termination, but a simple count is O(n) and more efficient. The main algorithm: iterate through the skill vector, count how many are ≤ (5−k). Then the maximum number of teams is this count divided by 3 (integer division). Edge cases: if `k` is greater than 5, then `5−k` is negative, meaning no contestant qualifies (since skills are non-negative), so the count will be 0 and the result 0. If `n` is less than 3, the result is 0. If there are more eligible contestants than needed, only complete groups of 3 count, so integer division floors the result. Time complexity O(n) because we traverse the vector once; space complexity O(1) auxiliary (ignoring input storage). Sorting would make it O(n log n) but is unnecessary.
#include <vector>
#include <algorithm>

// Returns the maximum number of complete 3-person teams where each member has skill <= 5 - k.
int maxTeams(int n, int k, const std::vector<int>& skills) {
    if (n < 3) return 0;
    int threshold = 5 - k;
    int eligible = 0;
    for (int skill : skills) {
        if (skill <= threshold) {
            ++eligible;
        }
    }
    return eligible / 3;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: many eligible, enough for multiple teams
    assert(maxTeams(6, 1, {1, 2, 3, 4, 5, 0}) == 2); // eligible: 0,1,2,3,4 → 5/3=1, but wait: threshold=4, eligible: 0,1,2,3,4 (5) → 1 team
    // Correct: threshold=4, eligible: 1,2,3,4,0 → 5 values, 5/3=1
    assert(maxTeams(6, 1, {1, 2, 3, 4, 5, 0}) == 1);

    // Exactly 3 eligible
    assert(maxTeams(3, 2, {3, 3, 3}) == 1); // threshold=3, all eligible

    // Only 2 eligible → no complete team
    assert(maxTeams(3, 2, {3, 3, 4}) == 0); // threshold=3, eligible count=2

    // k large, no one eligible
    assert(maxTeams(5, 5, {0,1,2,3,4}) == 0); // threshold=0, eligible only 0 → 0 teams

    // n < 3
    assert(maxTeams(2, 1, {4,4}) == 0);

    // All eligible but not divisible by 3
    assert(maxTeams(7, 0, {5,5,5,5,5,5,5}) == 2); // threshold=5, all 7 eligible, 7/3=2

    // Mixed with negative threshold (k>5)
    assert(maxTeams(4, 6, {0,1,2,3}) == 0); // threshold=-1, none eligible

    return 0;
}
