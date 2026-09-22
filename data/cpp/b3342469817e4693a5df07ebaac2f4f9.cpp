Write a C++ function that takes a vector of integers representing kids' candy counts and an integer `extraCandies`. For each kid, determine whether giving them all the extra candies would make their total greater than or equal to every other kid's original candy count (i.e., they would have the greatest number of candies, possibly tied). Return a vector of booleans in the same order, where `true` means the kid would have the greatest candy count after receiving the extra candies, and `false` otherwise. Assume the input is non-empty, and note that ties are allowed (if two kids end up with the same maximum, both should be `true`).
// The algorithm is straightforward: first find the maximum candy count among all kids in the input vector. Then, for each kid, add `extraCandies` to their original count and compare it to the original maximum. If the sum is greater than or equal to the maximum, the kid qualifies as a potential greatest-candy holder. The key edge case is when the maximum appears multiple times—since we compare against the maximum, any kid who reaches that maximum (including ties) gets `true`. The brute-force nested loop in the original snippet is inefficient; by precomputing the maximum, we avoid an inner loop. Time complexity is O(n) for finding the maximum plus O(n) for the comparison loop, total O(n). Space complexity is O(n) for the result vector (and O(1) auxiliary space beyond the output).
#include <vector>
#include <algorithm>

// Given candy counts for kids and extra candies, return a vector of booleans
// indicating whether each kid would have the greatest candy count after receiving
// the extra candies (ties are allowed).
std::vector<bool> kidsWithCandies(const std::vector<int>& candies, int extraCandies) {
    std::vector<bool> result;
    result.reserve(candies.size());

    // Find the maximum original candy count among all kids.
    int max_candies = *std::max_element(candies.begin(), candies.end());

    for (int count : candies) {
        // Kid qualifies if their total after extra candies is at least the maximum.
        result.push_back(count + extraCandies >= max_candies);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Declare the function (included from the solution above)
std::vector<bool> kidsWithCandies(const std::vector<int>& candies, int extraCandies);

int main() {
    // Test case 1: Example from LeetCode
    std::vector<int> candies1 = {2, 3, 5, 1, 3};
    std::vector<bool> result1 = kidsWithCandies(candies1, 3);
    assert(result1 == std::vector<bool>({true, true, true, false, true}));

    // Test case 2: All kids can become greatest with extra candies
    std::vector<int> candies2 = {4, 2, 1, 1, 2};
    std::vector<bool> result2 = kidsWithCandies(candies2, 1);
    assert(result2 == std::vector<bool>({true, false, false, false, false}));

    // Test case 3: Single kid always true
    std::vector<int> candies3 = {12};
    assert(kidsWithCandies(candies3, 10) == std::vector<bool>({true}));

    // Test case 4: Ties for maximum
    std::vector<int> candies4 = {5, 5, 5};
    assert(kidsWithCandies(candies4, 0) == std::vector<bool>({true, true, true}));

    // Test case 5: Extra candies = 0
    std::vector<int> candies5 = {1, 2, 3};
    assert(kidsWithCandies(candies5, 0) == std::vector<bool>({false, false, true}));

    // Test case 6: Extra candies very large
    std::vector<int> candies6 = {0, 0, 0};
    assert(kidsWithCandies(candies6, 100) == std::vector<bool>({true, true, true}));

    // Test case 7: Negative counts not allowed, but ensure no crash with small values
    std::vector<int> candies7 = {3, 1, 2};
    assert(kidsWithCandies(candies7, 2) == std::vector<bool>({true, true, true}));

    // Test case 8: Duplicates with one kid just below maximum
    std::vector<int> candies8 = {5, 4, 7, 7};
    assert(kidsWithCandies(candies8, 3) == std::vector<bool>({true, true, true, true}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
