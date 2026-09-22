// Write a standalone C++ function named `threeSumUnique` that takes a `const std::vector<int>& nums` and returns a `std::vector<std::vector<int>>` containing all unique triplets (i, j, k) such that `nums[i] + nums[j] + nums[k] == 0`, where the indices are distinct (i ≠ j, i ≠ k, j ≠ k). Each triplet in the output must be sorted in non-decreasing order, and the overall result must not contain any duplicate triplets. The input vector may contain negative and positive integers, zeros, and duplicates; the function should handle empty or small vectors (fewer than 3 elements) by returning an empty result. Ensure the function is `const`-correct and uses only standard C++ libraries.

// The solution uses the classic two-pointer technique after sorting the input array. Sorting enables easy duplicate skipping and efficient two-pointer scanning. For each index `i` from 0 to n-3, we fix the first element `nums[i]` and then set two pointers: `l = i+1` (left) and `r = n-1` (right). We compute the sum `nums[i] + nums[l] + nums[r]`:
// - If the sum is less than zero, increment `l` to increase the sum.
// - If greater than zero, decrement `r` to decrease the sum.
// - If equal to zero, record the triplet, then skip all duplicates of `nums[l]` and `nums[r]` by advancing `l` and `r` past equal values, then continue scanning.
//
// After finishing the inner loop for a fixed `i`, we skip all subsequent duplicate values of `nums[i]` to avoid duplicate triplets at the outer level. Edge cases include vectors of size < 3 (return empty), all negative or all positive sums (no triplets), and handling of many duplicate values (the duplicate-skipping loops must guard against `l < r` to avoid out-of-bounds). Time complexity is O(n^2) in the worst case (due to outer loop of n iterations and inner while loop that scans at most n elements per outer iteration), and space complexity is O(1) extra auxiliary space, not counting the output storage, which is O(k) for k triplets.

#include <vector>
#include <algorithm>

// Return all unique triplets whose sum equals zero, sorted and without duplicates.
std::vector<std::vector<int>> threeSumUnique(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(nums.size());
    if (n < 3) return result;

    // Work on a copy to allow sorting without modifying the input.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicates for the first element.
        if (i > 0 && sorted[i] == sorted[i - 1]) continue;

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            int sum = sorted[i] + sorted[left] + sorted[right];
            if (sum < 0) {
                ++left;
            } else if (sum > 0) {
                --right;
            } else {
                result.push_back({sorted[i], sorted[left], sorted[right]});
                // Skip duplicates for the second element.
                while (left < right && sorted[left] == sorted[left + 1]) ++left;
                // Skip duplicates for the third element.
                while (left < right && sorted[right] == sorted[right - 1]) --right;
                ++left;
                --right;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume threeSumUnique is defined above.
int main() {
    // Basic case with one unique triplet.
    std::vector<std::vector<int>> r1 = threeSumUnique({-1, 0, 1, 2, -1, -4});
    assert(r1.size() == 2);
    assert(r1[0] == std::vector<int>({-1, -1, 2}));
    assert(r1[1] == std::vector<int>({-1, 0, 1}));

    // Empty input.
    assert(threeSumUnique({}).empty());

    // Fewer than three elements.
    assert(threeSumUnique({1, 2}).empty());

    // All zeros: only one unique triplet.
    auto r2 = threeSumUnique({0, 0, 0});
    assert(r2.size() == 1);
    assert(r2[0] == std::vector<int>({0, 0, 0}));

    // No valid triplet.
    assert(threeSumUnique({1, 2, 3, 4}).empty());

    // Duplicate values but only one unique triplet from duplicates.
    auto r3 = threeSumUnique({-2, 0, 1, 1, 2});
    assert(r3.size() == 2);
    // Order of triplets depends on sort, but the first fixed i=-2 produces (-2,0,2) and second i=-2 duplicate is skipped.
    // Check membership without relying on order.
    bool found1 = false, found2 = false;
    for (const auto& t : r3) {
        if (t == std::vector<int>({-2, 0, 2})) found1 = true;
        if (t == std::vector<int>({-2, 1, 1})) found2 = true;
    }
    assert(found1 && found2);

    // Mixed with negatives and positives.
    auto r4 = threeSumUnique({-4, -2, -2, -1, -1, 0, 1, 2, 3, 3});
    assert(r4.size() == 3);
    bool ok1 = false, ok2 = false, ok3 = false;
    for (const auto& t : r4) {
        if (t == std::vector<int>({-2, -1, 3})) ok1 = true;
        if (t == std::vector<int>({-2, 0, 2})) ok2 = true;
        if (t == std::vector<int>({-1, -1, 2})) ok3 = true;
    }
    assert(ok1 && ok2 && ok3);

    // Large range with only negative numbers.
    assert(threeSumUnique({-5, -4, -3, -2, -1}).empty());

    // Input remains unmodified.
    std::vector<int> original = {3, -1, 2, 0};
    std::vector<int> copy = original;
    threeSumUnique(original);
    assert(original == copy);
}
