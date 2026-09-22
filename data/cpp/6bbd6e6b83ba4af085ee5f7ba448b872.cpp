// Write a C++ function named `sortColorsInPlace` that takes a `std::vector<int>&` containing only the integers 0, 1, and 2 (representing colors red, white, and blue) and sorts the vector in ascending order entirely in place, without using any standard sorting library functions. The function must rearrange the elements so that all 0s come first, then all 1s, then all 2s. The input vector may be empty, may contain only one distinct value, or may have duplicates of any color. The function should not return anything (void) and must modify the vector directly. The solution should avoid extra space beyond constant auxiliary memory (e.g., no copying the vector or using extra arrays of size proportional to the input). Edge cases include an empty vector, vectors with all elements the same, and vectors already sorted.
The optimal approach is the Dutch National Flag algorithm, which uses three pointers: `low`, `mid`, and `high`. Initially, `low` and `mid` point to the start of the vector, and `high` points to the end. The algorithm processes elements with `mid` moving from left to right. While `mid <= high`, we inspect `nums[mid]`:
- If the value is 0, swap `nums[low]` and `nums[mid]`, then increment both `low` and `mid`.
- If the value is 1, simply increment `mid` (since 1s belong in the middle).
- If the value is 2, swap `nums[mid]` and `nums[high]`, then decrement `high` (do not increment `mid` because the swapped-in value from `high` has not been checked yet).
This ensures that all elements before `low` are 0, elements between `low` and `mid` are 1, and elements after `high` are 2. The algorithm runs in O(n) time and O(1) auxiliary space. It correctly handles edge cases: empty vector (loop never runs), all same values (swaps still happen but maintain order), and already sorted vectors (no unnecessary swaps if values match). Complexity: time O(n) where n is the size of the vector, space O(1).
#include <vector>
#include <utility>  // for std::swap

// Sorts a vector containing only 0, 1, 2 in ascending order in place
// using the Dutch National Flag algorithm.
void sortColorsInPlace(std::vector<int>& nums) {
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(nums.size()) - 1;
    
    while (mid <= high) {
        if (nums[mid] == 0) {
            std::swap(nums[low], nums[mid]);
            ++low;
            ++mid;
        } else if (nums[mid] == 1) {
            ++mid;
        } else {  // nums[mid] == 2
            std::swap(nums[mid], nums[high]);
            --high;
        }
    }
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {2, 0, 2, 1, 1, 0};
    sortColorsInPlace(v1);
    assert(v1 == std::vector<int>({0, 0, 1, 1, 2, 2}));

    std::vector<int> v2 = {2, 0, 1};
    sortColorsInPlace(v2);
    assert(v2 == std::vector<int>({0, 1, 2}));

    std::vector<int> v3 = {0};
    sortColorsInPlace(v3);
    assert(v3 == std::vector<int>({0}));

    std::vector<int> v4 = {};
    sortColorsInPlace(v4);
    assert(v4.empty());

    std::vector<int> v5 = {1, 1, 1};
    sortColorsInPlace(v5);
    assert(v5 == std::vector<int>({1, 1, 1}));

    std::vector<int> v6 = {2, 2, 0, 0, 1, 1};
    sortColorsInPlace(v6);
    assert(v6 == std::vector<int>({0, 0, 1, 1, 2, 2}));

    std::vector<int> v7 = {1, 0, 2, 0, 2, 1, 0, 1, 2};
    sortColorsInPlace(v7);
    assert(v7 == std::vector<int>({0, 0, 0, 1, 1, 1, 2, 2, 2}));

    std::vector<int> v8 = {0, 0, 0, 2, 2, 2};
    sortColorsInPlace(v8);
    assert(v8 == std::vector<int>({0, 0, 0, 2, 2, 2}));

    std::vector<int> v9 = {2, 1, 0};
    sortColorsInPlace(v9);
    assert(v9 == std::vector<int>({0, 1, 2}));

    std::vector<int> v10 = {0, 1, 2};
    sortColorsInPlace(v10);
    assert(v10 == std::vector<int>({0, 1, 2}));

    return 0;
}
