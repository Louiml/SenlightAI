// Write a C++ function `int minimumPossibleStrength(int n, const std::vector<int>& strengths, const std::vector<int>& endurance)` that solves the following problem: There are `n` athletes, each with a strength value `s[i]` and an endurance value `e[i]`. Athlete 0 wants to compete in a multi-stage competition. The winner must have strictly greater strength OR strictly greater endurance than every other athlete (i.e., no other athlete can have both `s[j] >= s[0]` and `e[j] >= e[0]` for `j > 0`). If athlete 0 cannot guarantee winning under these conditions, the function should return -1. Otherwise, it should return the minimum strength that athlete 0 could have and still be able to win, given that athlete 0's strength can be reduced (but not increased) while keeping their endurance fixed. In other words, given the original `s[0]`, if no one beats or ties athlete 0 on both metrics, return `s[0]`; but if there is any other athlete with `e[j] >= e[0]` and `s[j] >= s[0]`, then athlete 0 cannot win even by lowering their strength (since lowering only reduces their own strength further making it worse), so return -1. The function must be pure and not use global variables.
// The core logic is straightforward: to determine if athlete 0 can win, we check every other athlete (index 1 to n-1). If for any athlete j, both `s[j] >= s[0]` and `e[j] >= e[0]` hold, then athlete 0 cannot be the unique winner, because this other athlete is at least as strong and at least as enduring. In that case, return -1. Otherwise, athlete 0 is already strictly better in at least one metric compared to every other athlete, so they can win; since we cannot increase strength, the only possible answer is the original `s[0]` (we could theoretically lower it, but lowering would only make it harder to beat others, so the best is to keep it). Edge cases: n can be 1 (then no other athletes, return s[0]); if the array sizes are mismatched, we assume they are valid; and we must be careful with equality — if another athlete has equal strength and equal endurance, that also disqualifies athlete 0. Time complexity is O(n) because we scan each other athlete once. Space complexity is O(1) extra (excluding input vectors).
#include <vector>
#include <algorithm>

// Returns the minimum strength athlete 0 can have to win, or -1 if impossible.
// The function assumes strengths and endurance vectors have the same size n >= 1.
int minimumPossibleStrength(int n, const std::vector<int>& strengths, const std::vector<int>& endurance) {
    // Iterate over all other athletes.
    for (int i = 1; i < n; ++i) {
        // If any other athlete has strength >= s[0] and endurance >= e[0],
        // athlete 0 cannot be the unique winner.
        if (strengths[i] >= strengths[0] && endurance[i] >= endurance[0]) {
            return -1;
        }
    }
    // Otherwise athlete 0 can win with their original strength.
    return strengths[0];
}
#include <cassert>
#include <vector>
#include "solution.h" // Assume the function is declared here, or paste above.

int main() {
    // Test case 1: n=1, only athlete 0.
    {
        std::vector<int> s = {5};
        std::vector<int> e = {7};
        assert(minimumPossibleStrength(1, s, e) == 5);
    }
    // Test case 2: All others worse in both metrics.
    {
        std::vector<int> s = {10, 8, 9};
        std::vector<int> e = {10, 9, 8};
        assert(minimumPossibleStrength(3, s, e) == 10);
    }
    // Test case 3: Another athlete strictly better in both.
    {
        std::vector<int> s = {10, 11, 9};
        std::vector<int> e = {10, 11, 8};
        assert(minimumPossibleStrength(3, s, e) == -1);
    }
    // Test case 4: Another athlete with equal strength and endurance.
    {
        std::vector<int> s = {5, 5, 4};
        std::vector<int> e = {5, 5, 6};
        assert(minimumPossibleStrength(3, s, e) == -1);
    }
    // Test case 5: Another athlete has greater strength but lower endurance.
    {
        std::vector<int> s = {5, 8, 3};
        std::vector<int> e = {9, 5, 10};
        assert(minimumPossibleStrength(3, s, e) == 5);
    }
    // Test case 6: Another athlete has greater endurance but lower strength.
    {
        std::vector<int> s = {5, 2, 6};
        std::vector<int> e = {9, 9, 3};
        assert(minimumPossibleStrength(3, s, e) == 5);
    }
    // Test case 7: Mixed scenario with both conditions.
    {
        std::vector<int> s = {7, 7, 6, 8};
        std::vector<int> e = {7, 7, 8, 6};
        // Athlete 1 exactly ties both -> disqualify
        assert(minimumPossibleStrength(4, s, e) == -1);
    }
    // Test case 8: Only one competitor and it barely loses.
    {
        std::vector<int> s = {4, 3};
        std::vector<int> e = {4, 5};
        assert(minimumPossibleStrength(2, s, e) == 4);
    }
    // Test case 9: Negative values can appear, but logic still works.
    {
        std::vector<int> s = {-2, -1, -3};
        std::vector<int> e = {-2, -1, -3};
        // Athlete 1 has s=-1 >= -2 and e=-1 >= -2 -> -1
        assert(minimumPossibleStrength(3, s, e) == -1);
    }
    // Test case 10: All others are worse, but values are large.
    {
        std::vector<int> s = {1000000000, 999999999, 999999998};
        std::vector<int> e = {1000000000, 999999999, 999999998};
        assert(minimumPossibleStrength(3, s, e) == 1000000000);
    }
    return 0;
}
