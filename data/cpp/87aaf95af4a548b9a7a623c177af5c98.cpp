// Write a C++ function `std::pair<int, int> fastMinMax(const std::vector<int>& nums)` that, given a non-empty vector of integers, returns a pair containing the minimum and maximum values in that order. The function must operate in a single pass, handle negative numbers, duplicates, and vectors of size 1 correctly. You are not allowed to use `std::minmax_element` or any other standard library algorithm that directly computes both min and max; you must implement the logic manually using a loop and simple comparisons. The function should be `const`-correct (i.e., it should not modify the input vector) and should work for any `int` values.

// The solution is straightforward: initialize both `minVal` and `maxVal` to the first element of the vector. Then iterate over the remaining elements (from index 1 to size-1). For each element, compare it against `minVal` and update if smaller, and compare against `maxVal` and update if larger. This ensures a single pass over the data. Edge cases: if the vector has size 1, the loop does not run, and the first element is returned as both min and max. Duplicates are handled naturally since comparisons with `<` or `>` do not change the min/max when values are equal. Negative numbers are just regular integers, so no special handling is needed. Time complexity is O(n) where n is the size of the vector, and space complexity is O(1) auxiliary (only two integers stored). The function returns a `std::pair<int, int>` with `.first` as min and `.second` as max.

#include <vector>
#include <utility>

std::pair<int, int> fastMinMax(const std::vector<int>& nums) {
    int minVal = nums[0];
    int maxVal = nums[0];
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] < minVal) minVal = nums[i];
        if (nums[i] > maxVal) maxVal = nums[i];
    }
    return {minVal, maxVal};
}

#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is defined above here (fastMinMax)

int main() {
    assert(fastMinMax({1, 2, 3, 4}) == std::make_pair(1, 4));
    assert(fastMinMax({-5, -1, -10}) == std::make_pair(-10, -1));
    assert(fastMinMax({7}) == std::make_pair(7, 7));
    assert(fastMinMax({3, 3, 3}) == std::make_pair(3, 3));
    assert(fastMinMax({10, -2, 8, 0}) == std::make_pair(-2, 10));
    assert(fastMinMax({0, 0, 0}) == std::make_pair(0, 0));
    assert(fastMinMax({-100, 100, -50, 50}) == std::make_pair(-100, 100));
    assert(fastMinMax({42}) == std::make_pair(42, 42));
    return 0;
}
