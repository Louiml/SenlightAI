// Write a C++ function `findMajorityElements` that takes a vector of integers and returns a vector containing all elements that appear more than `n/3` times (where `n` is the total number of elements). The returned vector should contain each qualifying element exactly once, in the order they first appear in the original input. The input vector may contain negative numbers, duplicates, and can be empty. If no element appears more than `n/3` times, return an empty vector. Your implementation must work for `n = 0` (return empty) and `n = 1` (the single element qualifies since `1/3 = 0` and `1 > 0`). You may modify the input vector if needed, but the function must be `const`-correct with respect to parameters it doesn't modify. Use only standard C++ libraries.
The classic approach for this problem is to sort the array first, which allows counting consecutive equal elements in a single pass. After sorting, we iterate through the array with a sliding window: for each distinct value, count how many times it appears consecutively, and if that count exceeds `n/3`, add it to the result. Sorting ensures that equal values are adjacent, so we can count efficiently. Edge cases include an empty array (immediately return empty), an array with one element (that element qualifies because `1 > 0` when `n/3 = 0`), and arrays where all elements are the same (the single element qualifies if `n > n/3`, which is always true for positive n). The algorithm has a time complexity of `O(n log n)` due to sorting, and `O(1)` auxiliary space if we sort in-place (or `O(n)` if we make a copy to preserve the original vector). The comparison `count > n/3` is integer arithmetic, so for `n = 1`, `n/3 = 0` and `1 > 0` is true. For `n = 2`, `n/3 = 0` and any element appearing twice qualifies, but an element appearing once does not.
#include <vector>
#include <algorithm>

// Return all elements that appear more than n/3 times in the input vector.
// Uses sorting to group equal elements, then counts consecutive occurrences.
std::vector<int> findMajorityElements(std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<int> result;
    if (n == 0) return result;

    std::sort(nums.begin(), nums.end());
    const int threshold = n / 3;
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n && nums[j] == nums[i]) {
            ++j;
        }
        const int count = j - i;
        if (count > threshold) {
            result.push_back(nums[i]);
        }
        i = j;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Solution function declared above (included for self-contained test)
std::vector<int> findMajorityElements(std::vector<int>& nums);

int main() {
    {
        std::vector<int> nums = {3, 2, 3};
        std::vector<int> expected = {3};
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {1};
        std::vector<int> expected = {1};
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {1, 2};
        std::vector<int> expected = {1, 2};  // both appear once, threshold 0, so both qualify
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {1, 2, 3};
        std::vector<int> expected;  // threshold 1, none appears more than once
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {};
        std::vector<int> expected;
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {2, 2, 1, 1, 1, 2, 2};
        std::vector<int> expected = {2};
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {-1, -1, -1, 0, 0, 2};
        std::vector<int> expected = {-1};
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {1, 1, 1, 1};
        std::vector<int> expected = {1};
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected);
    }
    {
        std::vector<int> nums = {1, 1, 2, 2, 3, 3};
        std::vector<int> expected = {1, 2, 3};  // each appears twice, threshold 2, 2 > 2 false? Actually n=6, threshold=2, each count=2, 2 > 2 is false, so expected empty
        // Correction: threshold = 6/3 = 2, count 2 is not > 2, so empty
        std::vector<int> expected_correct;
        std::vector<int> actual = findMajorityElements(nums);
        assert(actual == expected_correct);
    }
    return 0;
}
