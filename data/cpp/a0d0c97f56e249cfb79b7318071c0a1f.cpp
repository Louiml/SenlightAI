/*
Write a C++ function `bool canFormTarget(const std::vector<std::vector<int>>& triplets, const std::vector<int>& target)` that determines whether the target triplet can be formed by repeatedly applying an operation that merges any two triplets by taking the maximum of each corresponding position. The input `triplets` is a vector of triples (each of size 3), and `target` is a triple of size 3. The operation can be applied any number of times, including zero, to combine triplets. The function should return `true` if there exists a sequence of operations that yields exactly the `target` triple, and `false` otherwise. You may assume all numbers are non-negative integers, and the input is non-empty.
*/
#include <vector>
#include <algorithm>

// Determine if the target triple can be formed by merging triplets via element-wise maximum.
bool canFormTarget(const std::vector<std::vector<int>>& triplets,
                   const std::vector<int>& target) {
    // Maximum values found so far for each position among valid triplets.
    int max1 = 0, max2 = 0, max3 = 0;

    for (const auto& trip : triplets) {
        // Skip any triplet that has a value exceeding the target in any position.
        if (trip[0] > target[0] || trip[1] > target[1] || trip[2] > target[2]) {
            continue;
        }
        // Update running maxima for each position.
        max1 = std::max(max1, trip[0]);
        max2 = std::max(max2, trip[1]);
        max3 = std::max(max3, trip[2]);
    }

    // The target is reachable if all three maxima match exactly.
    return (max1 == target[0] && max2 == target[1] && max3 == target[2]);
}
#include <cassert>
#include <vector>

int main() {
    // Test case 1: Standard example from the problem.
    std::vector<std::vector<int>> t1 = {{2,5,3},{1,8,4},{1,7,5}};
    std::vector<int> target1 = {2,7,5};
    assert(canFormTarget(t1, target1) == true);

    // Test case 2: No valid triplet can contribute to some position.
    std::vector<std::vector<int>> t2 = {{3,4,5},{4,5,6}};
    std::vector<int> target2 = {3,2,5};
    assert(canFormTarget(t2, target2) == false);

    // Test case 3: Need to combine several triplets.
    std::vector<std::vector<int>> t3 = {{2,5,3},{2,3,4},{1,2,5},{5,2,3}};
    std::vector<int> target3 = {5,5,5};
    assert(canFormTarget(t3, target3) == true);

    // Test case 4: All triplets have some value exceeding target.
    std::vector<std::vector<int>> t4 = {{2,6,1},{5,7,3},{4,8,3},{3,2,2},{1,4,3}};
    std::vector<int> target4 = {4,7,3};
    assert(canFormTarget(t4, target4) == false);

    // Test case 5: One triplet exactly equals the target.
    std::vector<std::vector<int>> t5 = {{4,6,1},{5,7,3},{4,8,3},{3,7,2},{1,4,3}};
    std::vector<int> target5 = {4,7,3};
    assert(canFormTarget(t5, target5) == true);

    // Test case 6: Target all zeros, valid zero triplets exist.
    std::vector<std::vector<int>> t6 = {{0,0,1},{0,0,0},{1,0,0}};
    std::vector<int> target6 = {0,0,0};
    assert(canFormTarget(t6, target6) == true);

    // Test case 7: Single triplet, but it is invalid.
    std::vector<std::vector<int>> t7 = {{5,5,5}};
    std::vector<int> target7 = {1,2,3};
    assert(canFormTarget(t7, target7) == false);

    return 0;
}
// The key insight is that since we can merge as many triplets as we want, the final result after any sequence of merges is simply the element‑wise maximum of the selected triplets. Therefore, to form the exact target triplet, we must select a subset of original triplets whose maximum at each index equals the target's value at that index. A crucial observation: any triplet that has a value greater than the target's corresponding value at any index cannot be used at all, because merging it would immediately make that index exceed the target, and no operation can lower a value. Thus, we first filter out all such "invalid" triplets. For the remaining valid triplets (each ≤ target at every index), we track the running maximum for each of the three positions. At the end, if all three running maxima equal the target values, then we can achieve the target by merging all valid triplets (or the necessary ones); otherwise, it is impossible. Edge cases: if all triplets are invalid, maxima remain 0 (or a sentinel), and the function returns false unless the target is all zeros (in which case valid triplets include zeros, and maxima become 0, returning true). Another edge case: a single triplet equal to the target is trivially valid. The algorithm runs in O(n) time where n is the number of triplets, and uses O(1) extra space beyond the input.
