Write a C++ function `long long minimumCoins(vector<long long> heroes, long long dragonDefense, long long dragonAttack)` that, given a list of hero strengths (each hero has a strength value; there are `n` heroes), and a dragon with defense `d` and attack `a`, returns the minimum total coins required to defeat the dragon. You may spend coins to increase any hero's strength by 1 coin per 1 strength point (increasing a hero's strength permanently). To win, you must choose exactly one hero to send against the dragon: that hero's final strength must be at least `d`, and the sum of the strengths of all other heroes (after any increases) must be at least `a`. You can increase multiple heroes, but each point of increase costs 1 coin. The function should handle up to 200,000 heroes and up to 200,000 queries (the same list of heroes is used for all queries, but each query has its own dragon parameters). Return the minimum coins for each query, but the function will be called once per query with the same hero list.

The optimal strategy is to select one hero to fight the dragon. For that selected hero, we pay `max(0, d - strength)` to bring them to at least `d`. For the remaining heroes, we need the sum of their strengths (after possibly increasing them) to be at least `a`. Since increasing any non-fighting hero costs 1 coin per point, it's always cheapest to raise the strongest remaining hero (or heroes) to cover the deficit, but actually the cost is simply `max(0, a - (sum_of_all_heroes - strength_of_selected))`, because we can distribute increases among any non-selected heroes arbitrarily; the total additional strength needed is the deficit, and each point costs 1. So for a selected hero with strength `s`, the cost is `max(0, d - s) + max(0, a - (totalSum - s))`.

We want to minimize this over all possible choices of the selected hero. To evaluate efficiently for each query, sort the hero strengths ascending and precompute the total sum. For a query `(d, a)`, we consider two candidate selections around the threshold `d`: the first hero with strength `>= d` (using binary search) and the hero immediately before that. Why only these two? Because the cost function `f(s) = max(0, d-s) + max(0, a - (totalSum - s))` is piecewise linear and convex in `s`. Let’s analyze: if `s < d`, the first term is `d-s`; if `s >= d`, first term is 0. Second term: if `totalSum - s >= a` i.e. `s <= totalSum - a`, second term is 0; else second term is `a - totalSum + s`. There are four regions based on comparisons with `d` and `totalSum - a`. The function is convex, and its minimum over the discrete sorted values occurs at one of the two breakpoints around where the derivative changes sign. In practice, the optimal hero is either the smallest hero with strength `>= d` or the largest hero with strength `< d` (if any). This is because increasing a hero's strength to meet `d` is costly if they are weak, and using a strong hero reduces the other-hero sum deficit. The convexity ensures the minimum is at one of the two neighbors of the point where `s` crosses `d`. Edge cases: if all heroes are weaker than `d`, then the smallest `>= d` does not exist, so we only consider the strongest hero (index n). Similarly, if all heroes are `>= d`, we only consider the smallest such hero (index 1 after sort). Also, if `n=0`? Problem states at least 1 hero. For each query, binary search `upper_bound` on sorted array to find first index `pos` (1-based) where `a[pos] >= d`. Then candidates are `pos` and `pos-1` (if valid). Compute cost using formula with `totalSum`. The total complexity per query is `O(log n)` for binary search and `O(1)` for cost computation. Preprocessing sort is `O(n log n)`. Space is `O(n)`.

#include <vector>
#include <algorithm>
#include <cstdint>

// Computes minimum coins to defeat a dragon with given defense and attack.
// heroes: list of strengths (size >= 1). The function does not modify heroes.
long long minimumCoins(const std::vector<long long>& heroes, long long dragonDefense, long long dragonAttack) {
    // sorted copy for binary search
    std::vector<long long> sorted = heroes;
    std::sort(sorted.begin(), sorted.end());
    int n = static_cast<int>(sorted.size());
    long long totalSum = 0;
    for (long long v : sorted) totalSum += v;

    // Find first hero with strength >= dragonDefense
    // lower_bound returns first >= value
    auto it = std::lower_bound(sorted.begin(), sorted.end(), dragonDefense);
    int pos = static_cast<int>(it - sorted.begin()); // 0-based index

    long long best = LLONG_MAX;

    // Candidate function to evaluate a specific index
    auto evaluate = [&](int idx) {
        if (idx < 0 || idx >= n) return;
        long long s = sorted[idx];
        long long cost1 = (s < dragonDefense) ? (dragonDefense - s) : 0;
        long long remainingSum = totalSum - s;
        long long cost2 = (remainingSum < dragonAttack) ? (dragonAttack - remainingSum) : 0;
        best = std::min(best, cost1 + cost2);
    };

    // Candidate: first hero with strength >= defense (if exists)
    if (pos < n) evaluate(pos);
    // Candidate: the hero just before that (if exists)
    if (pos - 1 >= 0) evaluate(pos - 1);

    // In case no candidate was evaluated (n>0 always, so at least one will be evaluated)
    return best;
}

#include <cassert>
#include <vector>
#include <climits>
#include <cstdint>

// include the solution function here (e.g., from the section)

int main() {
    // Basic tests
    {
        std::vector<long long> heroes = {3, 5, 2};
        // totalSum = 10
        // For dragon d=8, a=9:
        // Try hero 5: cost1=3, remaining=5, cost2=4, total=7
        // Try hero 3: cost1=5, remaining=7, cost2=2, total=7
        // Try hero 2: cost1=6, remaining=8, cost2=1, total=7
        // Minimum = 7
        assert(minimumCoins(heroes, 8, 9) == 7);
        // d=5, a=8: hero5: cost1=0, rem=5, cost2=3 => 3; hero3: cost1=2, rem=7, cost2=1 =>3; hero2: cost1=3, rem=8, cost2=0 =>3 => min=3
        assert(minimumCoins(heroes, 5, 8) == 3);
        // d=2, a=2: hero2: cost1=0, rem=8, cost2=0 =>0 => min=0
        assert(minimumCoins(heroes, 2, 2) == 0);
        // d=100, a=1: all cost1 huge, rem sums small => best? hero5: cost1=95, rem=5, cost2=0 =95; hero3: 97, hero2:98 => 95
        assert(minimumCoins(heroes, 100, 1) == 95);
        // d=1, a=100: hero5: cost1=0, rem=5, cost2=95 =>95; hero3:0, rem=7, cost2=93 =>93; hero2:0, rem=8, cost2=92=>92 => min=92
        assert(minimumCoins(heroes, 1, 100) == 92);
    }

    // Single hero
    {
        std::vector<long long> heroes = {10};
        // d=5, a=5: hero10: cost1=0, rem=0, cost2=5 =>5
        assert(minimumCoins(heroes, 5, 5) == 5);
        // d=15, a=0: cost1=5, rem=0, cost2=0 =>5
        assert(minimumCoins(heroes, 15, 0) == 5);
        // d=10, a=10: cost1=0, rem=0, cost2=10 =>10
        assert(minimumCoins(heroes, 10, 10) == 10);
    }

    // Many heroes, large values
    {
        std::vector<long long> heroes;
        for (int i = 1; i <= 1000; ++i) heroes.push_back(i);
        // totalSum = 500500
        // d=500, a=500000: best? choose hero500: cost1=0, rem=500000, cost2=0 =>0? Wait rem = 500500-500=500000, a=500000 => cost2=0 => total0
        assert(minimumCoins(heroes, 500, 500000) == 0);
        // d=1001, a=1: choose hero1000? cost1=1, rem=499500, cost2=0 =>1; actually hero1000 (strength 1000) is <1001 so cost1=1, total=1
        assert(minimumCoins(heroes, 1001, 1) == 1);
        // d=1001, a=500000: hero1000: cost1=1, rem=499500, cost2=500 =>501; hero1001: cost1=0, rem=499499, cost2=501=>501 => min=501
        assert(minimumCoins(heroes, 1001, 500000) == 501);
    }

    // Edge case: all heroes weaker than defense
    {
        std::vector<long long> heroes = {1, 2, 3};
        // totalSum=6
        // d=10, a=0: strongest hero3: cost1=7, rem=3, cost2=0 =>7
        assert(minimumCoins(heroes, 10, 0) == 7);
        // d=10, a=5: hero3: cost1=7, rem=3, cost2=2 =>9; hero2: cost1=8 rem=4 cost2=1 =>9; hero1: cost1=9 rem=5 cost2=0=>9 => min=9
        assert(minimumCoins(heroes, 10, 5) == 9);
    }

    // Edge case: all heroes stronger than defense
    {
        std::vector<long long> heroes = {10, 20, 30};
        // totalSum=60
        // d=5, a=50: hero10: cost1=0, rem=50, cost2=0 =>0
        assert(minimumCoins(heroes, 5, 50) == 0);
        // d=5, a=55: hero10: cost1=0, rem=50, cost2=5 =>5; hero20: rem=40 cost2=15=>15; hero30: rem=30 cost2=25=>25 => min=5
        assert(minimumCoins(heroes, 5, 55) == 5);
    }

    return 0;
}
