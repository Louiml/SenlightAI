/*
Write a C++ function named `partitionAroundFirst` that takes a non-empty `std::vector<int>` and two valid indices `low` and `high` (with `low <= high` and both in range), and performs a **Hoare-style partition** using the element at `low` as the pivot. The function must modify the vector in place so that all elements strictly less than the pivot appear before all elements strictly greater than the pivot, though the pivot itself may end up anywhere between them (elements equal to the pivot can be on either side). The function must return the index `j` such that after partitioning, every element in the subrange `[low, j]` is less than or equal to the pivot, and every element in `[j+1, high]` is greater than or equal to the pivot. The algorithm must use two indices starting just outside the subrange and move them inward with do-while loops, swapping out-of-order elements, and terminate when the indices cross. The function must not allocate extra containers and must operate in linear time. You may assume that the subrange contains at least two distinct values, but the code should still handle duplicates correctly.
*/

#include <vector>
#include <utility>

// Hoare partition using nums[low] as pivot.
// Returns index j such that all elements in [low, j] are <= pivot,
// and all elements in [j+1, high] are >= pivot.
// Modifies the subrange in place.
int partitionAroundFirst(std::vector<int>& nums, int low, int high) {
    int pivot = nums[low];
    int left = low - 1;
    int right = high + 1;

    while (true) {
        do {
            ++left;
        } while (nums[left] < pivot);

        do {
            --right;
        } while (nums[right] > pivot);

        if (left >= right) {
            return right;
        }
        std::swap(nums[left], nums[right]);
    }
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec1{5,3,8,4,2,7,1,10};
    int j1 = partitionAroundFirst(vec1, 0, vec1.size()-1);
    assert(j1 >= 0 && j1 < (int)vec1.size());
    for (int i = 0; i <= j1; ++i) assert(vec1[i] <= 5);
    for (int i = j1+1; i < (int)vec1.size(); ++i) assert(vec1[i] >= 5);

    std::vector<int> vec2{3,1,2};
    int j2 = partitionAroundFirst(vec2, 0, 2);
    assert(j2 >= 0 && j2 < 3);
    for (int i = 0; i <= j2; ++i) assert(vec2[i] <= 3);
    for (int i = j2+1; i < 3; ++i) assert(vec2[i] >= 3);

    std::vector<int> vec3{10,20,30};
    int j3 = partitionAroundFirst(vec3, 0, 2);
    assert(j3 >= 0 && j3 < 3);
    for (int i = 0; i <= j3; ++i) assert(vec3[i] <= 10);
    for (int i = j3+1; i < 3; ++i) assert(vec3[i] >= 10);

    std::vector<int> vec4{7,7,1,7};
    int j4 = partitionAroundFirst(vec4, 0, 3);
    assert(j4 >= 0 && j4 < 4);
    for (int i = 0; i <= j4; ++i) assert(vec4[i] <= 7);
    for (int i = j4+1; i < 4; ++i) assert(vec4[i] >= 7);

    std::vector<int> vec5{2,1};
    int j5 = partitionAroundFirst(vec5, 0, 1);
    assert(j5 >= 0 && j5 < 2);
    for (int i = 0; i <= j5; ++i) assert(vec5[i] <= 2);
    for (int i = j5+1; i < 2; ++i) assert(vec5[i] >= 2);

    // Helper to check that the subrange is a permutation of the original.
    std::vector<int> orig{5,3,8,4,2,7,1,10};
    std::vector<int> sorted_orig = orig;
    std::sort(sorted_orig.begin(), sorted_orig.end());
    std::vector<int> sorted_vec = vec1;
    std::sort(sorted_vec.begin(), sorted_vec.end());
    assert(sorted_orig == sorted_vec);

    return 0;
}

// The solution follows the classic Hoare partition scheme. Initialize two indices: `left = low - 1` and `right = high + 1`. In an infinite loop, increment `left` until `nums[left] >= pivot`, then decrement `right` until `nums[right] <= pivot`. If `left >= right`, return `right`; otherwise swap the elements at `left` and `right` and continue. The key invariant is that after each iteration, all elements before `left` are ≤ pivot and all elements after `right` are ≥ pivot. The returned `right` satisfies the condition that `[low, right]` contains only elements ≤ pivot (though because duplicates and equals, some may be equal) and `[right+1, high]` contains only elements ≥ pivot. Edge cases: when all elements are equal, the do-while loops will move `left` to `high` and `right` to `low`, then `left >= right` causes return of `right` which may be `low-1` in some implementations, but because we start `right = high+1` and decrement, the condition works; however, the problem guarantees at least two distinct values, so this edge case is secondary. The pivot value is `nums[low]` and is never moved except through swaps; because Hoare partition does not place the pivot in its final sorted position, it is not necessarily at index `j`. Time complexity is O(n) where n = high - low + 1, and space complexity is O(1) beyond the input vector.
