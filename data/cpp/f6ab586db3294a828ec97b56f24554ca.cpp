/*
Write a C++ function `int numTeams(const std::vector<int>& rating)` that counts the number of teams of size 3 that can be formed from the given list of soldier ratings. A team is valid if for three indices `i < j < k`, the ratings are either strictly increasing (`rating[i] < rating[j] < rating[k]`) or strictly decreasing (`rating[i] > rating[j] > rating[k]`). The function must not modify the input vector and should handle arrays of length less than 3 by returning 0. For example, given `{2, 5, 3, 4, 1}`, valid teams are `(2,3,4)`, `(2,5,4)`, `(2,5,3)`, `(5,4,1)`, and `(3,4,1)` for increasing and decreasing patterns, so the answer is 5.
*/
#include <vector>

// Count the number of valid teams of size 3 with strictly increasing or decreasing ratings.
int numTeams(const std::vector<int>& rating) {
    const int n = static_cast<int>(rating.size());
    if (n < 3) {
        return 0;
    }

    int result = 0;
    for (int j = 1; j < n - 1; ++j) {
        int leftMin = 0;
        int leftMax = 0;
        int rightMin = 0;
        int rightMax = 0;

        for (int i = j - 1; i >= 0; --i) {
            if (rating[i] < rating[j]) {
                ++leftMin;
            } else if (rating[i] > rating[j]) {
                ++leftMax;
            }
        }

        for (int k = j + 1; k < n; ++k) {
            if (rating[k] < rating[j]) {
                ++rightMin;
            } else if (rating[k] > rating[j]) {
                ++rightMax;
            }
        }

        result += leftMin * rightMax + leftMax * rightMin;
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it here or link accordingly.

int main() {
    // Example from problem statement
    assert(numTeams({2, 5, 3, 4, 1}) == 5);

    // Empty or small arrays
    assert(numTeams({}) == 0);
    assert(numTeams({1}) == 0);
    assert(numTeams({1, 2}) == 0);

    // Fully increasing
    assert(numTeams({1, 2, 3, 4}) == 4); // (1,2,3), (1,2,4), (1,3,4), (2,3,4)

    // Fully decreasing
    assert(numTeams({4, 3, 2, 1}) == 4); // (4,3,2), (4,3,1), (4,2,1), (3,2,1)

    // Duplicates (no valid teams due to strict inequality)
    assert(numTeams({2, 2, 2, 2}) == 0);

    // Mixed case with more elements
    assert(numTeams({1, 3, 2, 4, 5}) == 6); // All triplets are valid in this permutation

    // Alternating pattern
    assert(numTeams({5, 1, 4, 2, 3}) == 3); // (5,1,2), (5,1,3), (5,4,2) and similar

    return 0;
}
// The solution fixes the middle element `j` (index) of the triplet to avoid counting each combination multiple times. For each `j` from 1 to `n-2`, we count:
// - `leftMin`: number of elements before `j` with rating less than `rating[j]`
// - `leftMax`: number of elements before `j` with rating greater than `rating[j]`
// - `rightMin`: number of elements after `j` with rating less than `rating[j]`
// - `rightMax`: number of elements after `j` with rating greater than `rating[j]`
//
// Then, any valid increasing team with `j` as middle uses one smaller element from the left and one larger element from the right: `leftMin * rightMax`. Similarly, decreasing teams use `leftMax * rightMin`. Summing these products for all `j` gives the total. Edge cases: if `n < 3`, return 0 immediately. Duplicate ratings are handled naturally since strict comparisons are used. Time complexity is O(n²) because for each `j` we scan the entire left and right subarrays. Space complexity is O(1) auxiliary, plus O(n) for the input vector.
