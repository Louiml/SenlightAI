Write a C++ function that, given a vector of banned integers, a positive integer `n`, and a positive integer `maxSum`, returns the maximum number of distinct integers from the range `[1, n]` that can be selected such that no selected integer is in the banned list and the sum of all selected integers does not exceed `maxSum`. You must select integers greedily in increasing order, and once adding the next allowed integer would make the sum exceed `maxSum`, stop and return the count so far. The function should take the vector by const reference, and handle edge cases where all numbers are banned, `n` is large, or `maxSum` is small.

The core idea is to iterate through integers from `1` to `n` in increasing order. For each integer, check if it is banned using a hash set built from the input vector for O(1) average lookup. If not banned, tentatively add it to a running sum. If the sum would exceed `maxSum`, return the current count immediately because adding any larger integer would only increase the sum further (since integers are positive and increasing). If not exceeded, increment the count. After the loop, return the final count. Edge cases: if `maxSum` is less than the smallest allowed number, the loop will return 0; if all numbers are banned, count remains 0; if `maxSum` is very large, all allowed numbers are counted. Time complexity is O(n + m) where m is the size of banned (for building the set), and space complexity is O(m) for the hash set. Using a hash set avoids the O(n*m) overhead of a linear search per integer.

#include <vector>
#include <unordered_set>

// Returns the maximum number of distinct integers from [1, n] that are not banned
// and whose sum does not exceed maxSum, selecting integers in increasing order.
int maxCountNonBanned(const std::vector<int>& banned, int n, int maxSum) {
    std::unordered_set<int> bannedSet(banned.begin(), banned.end());
    long long currentSum = 0;
    int count = 0;

    for (int i = 1; i <= n; ++i) {
        if (bannedSet.find(i) == bannedSet.end()) {
            currentSum += i;
            if (currentSum > static_cast<long long>(maxSum)) {
                return count;
            }
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function (as defined above).
int maxCountNonBanned(const std::vector<int>& banned, int n, int maxSum);

int main() {
    // Basic example from the snippet: banned = {1,4,6}, n=6, maxSum=10
    // Allowed: 2,3,5 -> sum 2+3+5=10, count=3
    assert(maxCountNonBanned({1,4,6}, 6, 10) == 3);

    // If sum immediately exceeds: banned = {1}, n=5, maxSum=1 -> allowed 2? sum=2>1, return 0
    assert(maxCountNonBanned({1}, 5, 1) == 0);

    // All numbers banned: banned = {1,2,3}, n=3, maxSum=100 -> count=0
    assert(maxCountNonBanned({1,2,3}, 3, 100) == 0);

    // No bans and maxSum large enough: n=5, maxSum=15 -> sum 1+2+3+4+5=15, count=5
    assert(maxCountNonBanned({}, 5, 15) == 5);

    // No bans but maxSum limits: n=10, maxSum=6 -> 1+2+3=6, count=3
    assert(maxCountNonBanned({}, 10, 6) == 3);

    // Banned numbers skipped, but sum still limited: banned={2,4}, n=6, maxSum=10
    // Allowed: 1,3,5 -> sum=9, then adding 6 would be 15>10, count=3
    assert(maxCountNonBanned({2,4}, 6, 10) == 3);

    // Edge with maxSum exactly matching after adding all allowed: banned={5}, n=5, maxSum=10
    // Allowed: 1,2,3,4 -> sum=10, count=4
    assert(maxCountNonBanned({5}, 5, 10) == 4);

    // Small maxSum with banned low numbers: banned={1}, n=3, maxSum=2
    // Allowed: 2 (sum=2), then 3 would exceed, count=1
    assert(maxCountNonBanned({1}, 3, 2) == 1);

    return 0;
}
