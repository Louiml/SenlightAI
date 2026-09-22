/*
Write a C++ function `minimumPartyCost` that takes three vectors of integers: `friends` (eating capacity of each friend), `capacity` (filling capacity of each dish type), and `cost` (cost of each dish type). The function must return the minimum total cost required to satisfy all friends, where each friend must eat dishes whose total filling capacity exactly equals their eating capacity. Each dish can be used any number of times, but each dish type can only be eaten by one friend per serving (i.e., the same dish type can be taken multiple times by the same friend, but not shared). It is guaranteed that there is at least one dish with filling capacity 1, so a solution always exists. The constraints: number of friends ≥ 1, each friend's capacity ≤ 1000, number of dish types ≥ 1, dish capacity and cost are positive integers, and dish counts ≤ 1000. Return the total minimum cost as an integer.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum total cost to satisfy all friends.
// friends: eating capacity of each friend.
// capacity: filling capacity of each dish type.
// cost: cost of each dish type.
int minimumPartyCost(const std::vector<int>& friends, const std::vector<int>& capacity, const std::vector<int>& cost) {
    // Determine the maximum eating capacity among all friends.
    int maxCapacity = 0;
    for (int c : friends) {
        maxCapacity = std::max(maxCapacity, c);
    }

    // dp[i] = minimum cost to achieve exactly total capacity i.
    std::vector<int> dp(maxCapacity + 1, INT_MAX);
    dp[0] = 0;

    // Compute minimum cost for all capacities up to maxCapacity.
    for (int i = 1; i <= maxCapacity; ++i) {
        for (size_t j = 0; j < capacity.size(); ++j) {
            if (capacity[j] <= i && dp[i - capacity[j]] != INT_MAX) {
                dp[i] = std::min(dp[i], dp[i - capacity[j]] + cost[j]);
            }
        }
    }

    // Sum minimum costs for all friends.
    int total = 0;
    for (int c : friends) {
        total += dp[c];
    }
    return total;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we provide test cases.
int main() {
    // Example from the problem statement.
    std::vector<int> friends1 = {2, 4, 6};
    std::vector<int> capacity1 = {2, 1, 3};
    std::vector<int> cost1 = {2, 5, 3};
    assert(minimumPartyCost(friends1, capacity1, cost1) == 14);

    // Single friend with capacity 1, only dish capacity 1 cost 7.
    assert(minimumPartyCost({1}, {1}, {7}) == 7);

    // Multiple friends with same capacity, use cheapest dish for capacity.
    // Capacity 2: use two of dish capacity 1 cost 3 => total 6 per friend.
    // For 3 friends all capacity 2 => 18.
    assert(minimumPartyCost({2,2,2}, {1,2}, {3,5}) == 18);

    // Friend capacity 5 with dishes: capacity 3 cost 2, capacity 2 cost 4 => use 3+2 = 5 cost 6.
    assert(minimumPartyCost({5}, {3,2}, {2,4}) == 6);

    // Friend capacity 10 with dish capacity 5 cost 1 => use two dishes cost 2.
    assert(minimumPartyCost({10}, {5}, {1}) == 2);

    // Mixed capacities: 3 and 4, with cheap dish capacity 1 cost 10, dish capacity 2 cost 15, dish capacity 3 cost 20.
    // For capacity 3: use 3 => 20 (or 1+1+1 => 30). For capacity 4: use 3+1 => 30 (2+2 => 30). total 50.
    assert(minimumPartyCost({3,4}, {1,2,3}, {10,15,20}) == 50);

    // Large capacity with guaranteed dish 1.
    assert(minimumPartyCost({1000}, {1, 500}, {2, 100}) == 2000); // 1000 * 2 = 2000

    // Friend capacity exactly matches a dish with high cost, but cheaper combination exists.
    // capacity 6: dish 6 cost 100, dish 2 cost 3 => three of dish 2 cost 9.
    assert(minimumPartyCost({6}, {6,2}, {100,3}) == 9);

    // Multiple friends with capacities 7 and 11, use dish 1 cost 1.
    assert(minimumPartyCost({7,11}, {1}, {1}) == 18);
}

// The problem can be solved by observing that the minimum cost to satisfy each friend depends only on that friend's capacity, and the total cost is the sum of the minimum costs for each individual friend. Therefore, we first compute the maximum eating capacity among all friends. Then we solve an unbounded knapsack (coin change) problem: for every capacity value from 1 up to the maximum, we compute the minimum cost to reach exactly that capacity using any number of each dish type. This is done with dynamic programming: initialize `dp[0] = 0` and all others to a large value (e.g., `INT_MAX`). For each capacity `i` from 1 to maxCapacity, we iterate over all dish types `j`. If `capacity[j] <= i`, we update `dp[i] = min(dp[i], dp[i - capacity[j]] + cost[j])`. This works because we can fill capacity `i` by taking a dish of capacity `capacity[j]` and then optimally filling the remaining `i - capacity[j]`. Since each dish can be taken unlimited times, this forward DP correctly computes the minimum cost for all capacities. Edge cases: a friend capacity of 1 must be handled, and the guaranteed existence of a capacity-1 dish ensures finite solutions. Time complexity is O(maxCapacity * number_of_dishes), and space complexity is O(maxCapacity). Because maxCapacity ≤ 1000 and number of dishes ≤ 1000, this is at most 1,000,000 operations, which is efficient.
