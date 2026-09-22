// You are given `n` students, where each student has a friend count `K[i]` (the number of gifts they must receive, meaning they need a gift costing at least `K[i]` dollars). There is a shop with `m` distinct gift prices, given in non-decreasing order as `cost[1]` through `cost[m]` (where `cost[1]` is the cheapest and `cost[m]` the most expensive). You must buy exactly one gift for each student, and for student `i` you must buy a gift that costs at least `K[i]` dollars (you may buy any gift from the shop, even multiple copies of the same price). Your goal is to minimize the total cost of all `n` gifts. Write a C++ function `long long minimumGiftCost(const std::vector<long long>& friends, const std::vector<long long>& prices)` that takes the friend counts (unsorted) and the non-decreasing price list, and returns the minimum possible total cost. The prices list is guaranteed to be sorted in non-decreasing order, and you may assume that `prices.back()` is always large enough to satisfy the largest friend count. For example, if friends = {3, 1, 2} and prices = {1, 2, 3, 4}, the minimum total cost is 1+2+3 = 6 (buy the cheapest gift for the friend with K=1, then the next for K=2, and the most expensive for K=3). If friends were all equal to 1 and prices = {1, 5}, the minimum is 3 (buy the $1 gift for all three).

The optimal strategy is to assign the most expensive (largest K) friends to the cheapest possible gifts, and the least demanding friends to the more expensive gifts, but only if that reduces the total compared to the naive assignment where each friend gets a gift costing exactly `cost[K[i]]`. The key observation is: sort the friend counts in non-decreasing order. Initially, the total cost is the sum of `cost[K[i]]` for each friend (this uses the most expensive option, which is always valid). Then, we consider replacing some of the largest friend counts with the cheapest available prices from the start of the price list, one by one, from the largest friend down to the smallest. For each replacement, we remove the cost of the gift that friend would have gotten (i.e., `cost[K[large]]`) and add the cost of the next cheap gift (starting from `prices[0]`, then `prices[1]`, etc.). We keep track of the minimum total cost over all possible numbers of replacements. This works because the friend counts are sorted, and the price list is non-decreasing, so replacing a larger friend with a cheaper price than their original required price will only be beneficial if the cheap price is less than the required price for that friend. We never need to consider assigning a cheap gift to a small-K friend while leaving a large-K friend with an expensive gift, because swapping them would not reduce cost (since both gifts are individually valid for either friend when the cheap gift is >= the small friend's K, and the expensive gift is >= the large friend's K, but swapping would increase cost). The algorithm is O(n log n) for sorting friend counts, plus O(n + m) for the loop, and uses O(1) extra space beyond the input vectors. Edge cases: if a friend has K equal to 1, the cheapest gift must be at least 1 (given by constraints). If there are more friends than the number of distinct prices, we reuse prices (e.g., multiple copies of the cheapest price). It is always safe to replace any friend with any price as long as the price is at least that friend's K; but the greedy strategy of replacing from largest K downwards with the cheapest prices ensures that all replaced friends get valid gifts (since those cheap prices are at least the smallest K, and we only replace friends whose original K is larger than the cheap price used).

#include <vector>
#include <algorithm>
#include <numeric>

// Returns the minimum total cost to buy one gift for each friend,
// where friend i needs a gift of price at least friends[i],
// and available gift prices are given in non-decreasing order.
long long minimumGiftCost(std::vector<long long> friends, const std::vector<long long>& prices) {
    // Sort friend counts in ascending order.
    std::sort(friends.begin(), friends.end());

    int n = static_cast<int>(friends.size());
    int m = static_cast<int>(prices.size());

    // Initial cost: for each friend, pay the price corresponding to their K.
    long long current = 0;
    for (long long f : friends) {
        current += prices[f - 1]; // prices are 1-indexed in the problem, so use f-1
    }

    long long answer = current;

    // Try replacing the largest friends with cheap gifts from the start of the price list.
    int cheapIndex = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (cheapIndex < m) {
            // Remove the original cost for this friend and add the cheap gift cost.
            current -= prices[friends[i] - 1];
            current += prices[cheapIndex];
            ++cheapIndex;
            answer = std::min(answer, current);
        }
    }

    return answer;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
long long minimumGiftCost(std::vector<long long> friends, const std::vector<long long>& prices);

int main() {
    // Example from the task
    assert(minimumGiftCost({3, 1, 2}, {1, 2, 3, 4}) == 6);
    // All friends need the cheapest gift
    assert(minimumGiftCost({1, 1, 1}, {1, 5}) == 3);
    // Single friend
    assert(minimumGiftCost({4}, {1, 2, 3, 4}) == 4);
    // Already sorted and no benefit from cheap replacements
    assert(minimumGiftCost({2, 3, 5}, {1, 2, 3, 4, 5}) == 2 + 3 + 5);
    // Replace all possible with cheapest gifts
    assert(minimumGiftCost({5, 5, 5}, {1, 2, 3, 4, 5}) == 3); // buy three $1 gifts
    // Mixed case where partial replacement is optimal
    assert(minimumGiftCost({2, 4, 4}, {2, 4, 10}) == 8); // buy price 2 for K=2, and price 4 twice for K=4 friends
    // Large K values but many cheap prices available
    assert(minimumGiftCost({10, 10, 10}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 3);
    // Unsorted input friends
    assert(minimumGiftCost({4, 1, 3, 2}, {1, 2, 3, 4}) == 1 + 2 + 3 + 4);
    // Duplicate prices allowed? The problem says distinct, but test with duplicates
    assert(minimumGiftCost({3, 1}, {1, 1, 3}) == 2); // two $1 gifts work
    return 0;
}
