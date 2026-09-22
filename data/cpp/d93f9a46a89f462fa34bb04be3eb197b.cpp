/*
Write a C++ function that takes a vector of positive integers representing side lengths and returns the largest possible perimeter of a triangle that can be formed using three of those side lengths. If no triangle can be formed (i.e., no triple satisfies the triangle inequality: sum of any two sides strictly greater than the third), the function must return 0. The vector must contain at least 3 elements, but the values are not necessarily distinct and can be in any order.
*/

#include <vector>
#include <algorithm>

// Returns the largest perimeter of a triangle that can be formed from
// any three side lengths in nums, or 0 if no triangle can be formed.
int largestPerimeter(std::vector<int>& nums) {
    // Sort sides in ascending order.
    std::sort(nums.begin(), nums.end());

    // The largest possible perimeter will use the three largest sides,
    // but we must check consecutive triples because only they are candidates.
    // Iterate from the end backward for early exit, but any direction works.
    for (int i = static_cast<int>(nums.size()) - 1; i >= 2; --i) {
        // For a valid triangle with sides nums[i-2], nums[i-1], nums[i],
        // the only necessary check is smallest + middle > largest.
        if (nums[i - 2] + nums[i - 1] > nums[i]) {
            return nums[i - 2] + nums[i - 1] + nums[i];
        }
    }
    // No valid triple found.
    return 0;
}

#include <cassert>
#include <vector>

int largestPerimeter(std::vector<int>& nums);

int main() {
    std::vector<int> v1 = {2, 1, 2};        // sides 1,2,2 -> valid, perimeter 5
    assert(largestPerimeter(v1) == 5);

    std::vector<int> v2 = {1, 2, 1};        // 1,1,2 fails (1+1 not >2) -> 0
    assert(largestPerimeter(v2) == 0);

    std::vector<int> v3 = {3, 6, 2, 3};     // sorted 2,3,3,6; 3+3>6? no; 2+3>3? yes → perimeter 8
    assert(largestPerimeter(v3) == 8);

    std::vector<int> v4 = {1, 2, 3, 4, 5};  // sorted; 3+4>5 → 12
    assert(largestPerimeter(v4) == 12);

    std::vector<int> v5 = {5, 5, 5};        // equilateral → 15
    assert(largestPerimeter(v5) == 15);

    std::vector<int> v6 = {2, 2, 3, 4};     // sorted 2,2,3,4; 2+3>4? yes → 9
    assert(largestPerimeter(v6) == 9);

    std::vector<int> v7 = {1, 1, 2, 2, 3};  // sorted; 2+2>3? yes → 7
    assert(largestPerimeter(v7) == 7);

    std::vector<int> v8 = {10, 1, 1, 1};    // sorted 1,1,1,10; 1+1 not >10 → 0
    assert(largestPerimeter(v8) == 0);

    std::vector<int> v9 = {3, 4, 5};        // classic 12
    assert(largestPerimeter(v9) == 12);

    std::vector<int> v10 = {2, 2, 2, 2};    // any triple → 6
    assert(largestPerimeter(v10) == 6);
}

// The triangle inequality for three sides a, b, c (after sorting a ≤ b ≤ c) only needs checking a + b > c, because if that holds, the other two inequalities (a + c > b and b + c > a) are automatically satisfied since c is the largest. Therefore, sort the array in ascending order. Then, iterate from the largest possible triple indices: for each i from 0 to n-3, check if nums[i] + nums[i+1] > nums[i+2]. Since the array is sorted, for any i, the triple (i, i+1, i+2) has the smallest sum among all triples that include i+2 as the largest side, so if this condition fails, no larger triple with that largest side can work (because increasing the two smaller sides would only help, but we are limited by the consecutive elements). However, the simplest correct approach is to iterate all i and keep the maximum valid perimeter. Sorting ensures we can compare consecutive elements only; the maximum perimeter will come from a triple of consecutive elements after sorting, because if we have a valid triple (i, j, k) with i < j < k, then (k-2, k-1, k) will also be valid and have a larger or equal perimeter. Thus we only need to check consecutive triples from the end backward, but checking all consecutive triples is also fine. Edge case: if no consecutive triple satisfies the inequality, return 0 (since any non-consecutive triple would have smaller or equal sides, also failing). Time complexity is O(n log n) from sorting, and O(n) for iterating, so O(n log n). Space complexity O(1) auxiliary (ignoring sort's internal stack).
