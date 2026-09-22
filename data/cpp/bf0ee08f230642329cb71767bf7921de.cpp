Given a non-empty vector of non-empty integer vectors, write a C++ function that returns the intersection of all the inner vectors as a sorted vector of unique integers. The input vectors may contain duplicates, negative numbers, and appear in any order. The output must contain each integer that appears in every inner vector exactly once, sorted in ascending order. If no integer appears in all inner vectors, return an empty vector. The input vector itself must not be modified.

// The solution sorts each inner vector and reduces the problem to a sequence of two-array intersections. Start by sorting a copy of the first vector and using it as the initial intersection result. For each subsequent vector, sort it, then compute the intersection of the current result with this sorted vector using a standard two-pointer merge (or `std::set_intersection`), storing the common elements in a temporary vector. That temporary vector becomes the new result. Because each intersection step discards any element not present in the next vector, and because duplicates in any input vector are removed by the intersection process (only one copy is kept per common element), the final result is automatically unique and sorted. Edge cases include a single inner vector (the answer is its sorted unique elements), all vectors identical, and no common elements. If at any point the current intersection becomes empty, the final answer is empty and the loop can stop early. Time complexity is O(m·n log n) where m is the number of vectors and n is the average size of each vector (due to repeated sorting), and O(n) auxiliary space for temporary arrays and the result.

#include <vector>
#include <algorithm>

// Returns the intersection of all inner vectors as sorted unique values.
std::vector<int> intersectionOfAll(const std::vector<std::vector<int>>& nums) {
    if (nums.empty()) return {};
    if (nums.size() == 1) {
        std::vector<int> single = nums[0];
        std::sort(single.begin(), single.end());
        single.erase(std::unique(single.begin(), single.end()), single.end());
        return single;
    }

    std::vector<int> result = nums[0];
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    for (size_t i = 1; i < nums.size() && !result.empty(); ++i) {
        std::vector<int> next = nums[i];
        std::sort(next.begin(), next.end());
        std::vector<int> common;
        std::set_intersection(result.begin(), result.end(),
                              next.begin(), next.end(),
                              std::back_inserter(common));
        result.swap(common);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Example from typical LeetCode problem
    std::vector<std::vector<int>> nums1 = {{3,1,2,1}, {1,2,3,4}, {2,3,1}};
    std::vector<int> res1 = intersectionOfAll(nums1);
    assert((res1 == std::vector<int>{1,2,3}));

    // Single vector with duplicates
    std::vector<std::vector<int>> nums2 = {{7,7,7}};
    assert((intersectionOfAll(nums2) == std::vector<int>{7}));

    // No common element
    std::vector<std::vector<int>> nums3 = {{1,2}, {3,4}};
    assert(intersectionOfAll(nums3).empty());

    // Single element in each vector but common
    std::vector<std::vector<int>> nums4 = {{5}, {5}, {5}};
    assert((intersectionOfAll(nums4) == std::vector<int>{5}));

    // Negative numbers and duplicates
    std::vector<std::vector<int>> nums5 = {{-2, -2, 0, 4}, {-2, 4, 4}, {4, -2, 0}};
    assert((intersectionOfAll(nums5) == std::vector<int>{-2, 4}));

    // One vector with many duplicates and unique values
    std::vector<std::vector<int>> nums6 = {{3,1,3,1}};
    assert((intersectionOfAll(nums6) == std::vector<int>{1,3}));

    // Verify original input unchanged
    std::vector<std::vector<int>> nums7 = {{2,1}, {1,2}};
    std::vector<std::vector<int>> copy7 = {{2,1}, {1,2}};
    intersectionOfAll(nums7);
    assert(nums7 == copy7);

    // Large value range
    std::vector<std::vector<int>> nums8 = {{100, -100}, {-100, 0, 100}};
    assert((intersectionOfAll(nums8) == std::vector<int>{-100, 100}));

    // All identical but with duplicates
    std::vector<std::vector<int>> nums9 = {{1,2,2,3}, {2,3,3,1}, {3,1,1,2}};
    assert((intersectionOfAll(nums9) == std::vector<int>{1,2,3}));
}
