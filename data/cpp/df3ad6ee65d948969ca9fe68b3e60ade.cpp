/*
Write a C++ function `maxRemainingStrength` that takes a vector of positive integers representing the strengths of `n` cards (`n >= 1`). The cards must be combined in the following way: choose any two cards with strengths `a` and `b`, remove them, and add a new card with strength `a + b - 1`. Repeat this process until only one card remains. The function should return the maximum possible strength of the final single card. The input vector may contain duplicate values, and the order of the original vector does not matter. You may reorder the cards arbitrarily before the merging process begins.
*/
#include <vector>
#include <algorithm>
#include <numeric>

// Returns the maximum possible strength of the final card after merging all cards.
// The merging rule is: combine two cards of strengths a and b to get a+b-1.
// The result is independent of merge order and equals sum - (n-1).
long long maxRemainingStrength(const std::vector<long long>& cards) {
    if (cards.empty()) return 0; // not expected per problem, but safe
    long long sum = 0;
    for (long long val : cards) {
        sum += val; // sum may exceed int, so use long long
    }
    // For n cards, final strength = sum - (n-1)
    return sum - static_cast<long long>(cards.size() - 1);
}
#include <cassert>
#include <vector>

// The function to test (provided above)
long long maxRemainingStrength(const std::vector<long long>& cards);

int main() {
    // Single card
    assert(maxRemainingStrength({7}) == 7);
    // Two cards: 5+3-1=7
    assert(maxRemainingStrength({5, 3}) == 7);
    // Example from snippet: sorted descending [5,4,3] => 5+4-1=8, then 8+3-1=10
    assert(maxRemainingStrength({3, 4, 5}) == 10);
    // All ones: 1+1-1=1, then 1+1-1=1 => final 1; sum=3, n=3 => 3-2=1
    assert(maxRemainingStrength({1, 1, 1}) == 1);
    // Duplicates and large numbers
    assert(maxRemainingStrength({10, 10, 1}) == 10+10+1-2 = 19);
    // Many elements
    assert(maxRemainingStrength({2, 2, 2, 2}) == 8-3 = 5);
    // Large values to check overflow handling (use long long)
    assert(maxRemainingStrength({1000000000LL, 1000000000LL, 1000000000LL}) == 3000000000LL - 2);
    return 0;
}
// The key observation is that when combining two cards with strengths `a` and `b`, the result is `a + b - 1`. If we combine `n` cards, the final strength is the sum of all initial strengths minus `(n-1)` (since each merge subtracts 1). This is independent of the merge order! To see why: each merge reduces the total sum by exactly 1, and there are exactly `n-1` merges. Therefore, the final strength is simply `(sum of all strengths) - (n-1)`. However, there is a crucial caveat: during the process, intermediate strengths must remain positive. Since all original strengths are positive integers, if at any point we combine a card of strength 1 with another of strength 1, we get `1+1-1=1`, which is fine. The only problem would be if we had a card of strength 0 or negative, but that never happens because all values are positive and `a+b-1 >= 1` for `a,b >= 1`. Therefore, any merge order is valid. The maximum final strength is always `sum - (n-1)`. For `n=1`, the answer is just `a[0]`. The solution sorts the array in descending order (though not strictly necessary) and then accumulates the result as `c = a[0] + a[1] - 1`, then for each subsequent card, `c = a[i] + c - 1`. This is equivalent to computing `sum - (n-1)`. Edge case: `n=1` returns the single value. Time complexity is `O(n log n)` due to sorting (or `O(n)` if we just sum, but the snippet sorts anyway). Space complexity is `O(1)` auxiliary.
