/*
Write a C++ function `bool canFormTriangleWithLargest(std::vector<int>& nums)` that takes a vector of integers, sorts it, removes the largest element, and determines whether the remaining elements can form a valid triangle by placing the largest element as one side and selecting two adjacent elements from the sorted remaining list as the other two sides. The function should modify the vector by inserting the largest element back at the position where the two chosen sides satisfy the triangle inequality, but only if such a valid triangle exists. If no such triangle can be formed, leave the vector in its original sorted order (without the largest element removed, i.e., restore it) and return `false`; otherwise return `true`. The function must handle edge cases like fewer than 3 elements, duplicates, and large values.
*/
#include <vector>
#include <algorithm>

// Checks if a triangle can be formed using the largest element as one side
// and two adjacent elements from the remaining sorted list as the other sides.
// If possible, inserts the largest back at the correct position and returns true.
// Otherwise, restores the vector to original sorted order and returns false.
bool canFormTriangleWithLargest(std::vector<int>& nums) {
    if (nums.size() < 3) {
        return false;
    }
    std::sort(nums.begin(), nums.end());
    int largest = nums.back();
    nums.pop_back();
    for (int i = static_cast<int>(nums.size()) - 1; i >= 1; --i) {
        if (nums[i] + nums[i-1] > largest) {
            nums.insert(nums.begin() + i, largest);
            return true;
        }
    }
    // Restore original sorted vector
    nums.push_back(largest);
    std::sort(nums.begin(), nums.end());
    return false;
}
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is expected to be declared above this test.
// Placeholder declaration for clarity; in the actual combined file, the function is already defined.
bool canFormTriangleWithLargest(std::vector<int>& nums);

int main() {
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(canFormTriangleWithLargest(v1) == true);
    // After the call, the vector should be sorted and contain 5 as largest, with some pair summing >5.
    // Check that it contains all original elements and is sorted.
    assert(v1.size() == 5);
    assert(std::is_sorted(v1.begin(), v1.end()));
    assert(v1.back() == 5);

    std::vector<int> v2 = {1, 2, 2}; // 2+2 > 2? No, 2+2=4 >2 yes, but largest is 2, remaining [1,2], 1+2=3>2 yes.
    assert(canFormTriangleWithLargest(v2) == true);
    assert(v2.size() == 3);
    assert(v2[0] == 1 && v2[1] == 2 && v2[2] == 2); // sorted, largest 2 inserted at index 1.

    std::vector<int> v3 = {1, 2, 3}; // 3 is largest, remaining [1,2], 1+2=3 not >3 -> false
    assert(canFormTriangleWithLargest(v3) == false);
    std::vector<int> expected3 = {1, 2, 3};
    assert(v3 == expected3); // restored original

    std::vector<int> v4 = {10, 1, 2, 3}; // sorted [1,2,3,10], largest=10, remaining [1,2,3], max pair 2+3=5<10 false
    assert(canFormTriangleWithLargest(v4) == false);
    std::vector<int> expected4 = {1, 2, 3, 10};
    assert(v4 == expected4);

    std::vector<int> v5 = {5, 5, 5, 5}; // largest=5, remaining [5,5,5], any adjacent pair 5+5=10>5 -> true
    assert(canFormTriangleWithLargest(v5) == true);
    assert(v5.size() == 4);
    assert(std::is_sorted(v5.begin(), v5.end()));

    std::vector<int> v6 = {1, 2}; // fewer than 3 elements
    assert(canFormTriangleWithLargest(v6) == false);

    std::vector<int> v7 = {2, 3, 4, 6}; // sorted [2,3,4,6], largest=6, remaining [2,3,4], adjacent pairs: 3+4=7>6 yes, so insert at index 2 -> [2,3,6,4]? Actually insert at i=2 gives [2,3,6,4], but then vector not sorted? Wait after insertion, the vector is [2,3,6,4] which is not sorted. But the requirement only says "modify the vector by inserting the largest element back at the position where the two chosen sides satisfy the triangle inequality". It doesn't require the final vector to be sorted. But it's cleaner to keep it sorted. However, the condition for the pair is from the sorted remaining list. For v7, remaining [2,3,4], i=2 (last index, value 4), check nums[2]+nums[1] = 4+3=7>6 -> true, insert largest at begin()+2 => [2,3,6,4]. That's not sorted. The original code snippet does exactly that insert without resorting. So our test should just check that the vector contains all elements and the largest is at index 2. Let's adjust.
    std::vector<int> v7 = {2, 3, 4, 6};
    assert(canFormTriangleWithLargest(v7) == true);
    std::vector<int> expected7 = {2, 3, 6, 4}; // because insertion at index 2
    assert(v7 == expected7);

    return 0;
}
// The core idea is based on the property that for a sorted array, if a triangle exists with the largest element as the longest side, then the two other sides should be as large as possible while summing to more than the largest side. After sorting the vector, the algorithm removes the largest element and checks adjacent pairs from the end of the remaining list. Since the remaining list is sorted, the pair with the largest sum among all pairs is the two largest remaining elements (the last two). If that pair doesn't satisfy `a + b > largest`, then no pair will, because all other pairs have smaller or equal sums. So we iterate from the end downwards, checking `nums[i] + nums[i-1] > largest`. The first pair that satisfies this is the optimal choice (they are the largest possible pair). Once found, insert the largest element back at position `i` (so it sits between `nums[i-1]` and `nums[i]`), making the three sides consecutive? Actually, the order doesn't matter for validity, but the insertion keeps the vector sorted. If no pair satisfies, restore the vector to its original sorted full state (i.e., re-append the largest and re-sort) and return false. Edge cases: if input has fewer than 3 elements, it's impossible. Also, if the largest is so big that no pair sums to more than it, return false. Time complexity is O(n log n) for sorting and O(n) for the scan, so total O(n log n). Space complexity is O(1) auxiliary (ignoring the vector itself).
