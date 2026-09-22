/*
Given an array of `n` integers, write a C++ function `int countDistinct(const std::vector<int>& a)` that returns the number of distinct integer values in the array. The function must handle empty arrays by returning 0, and must work correctly for arrays containing duplicates, negative numbers, and values up to the standard `int` range. The function should not modify the input vector.
*/

#include <vector>
#include <set>

// Count the number of distinct integer values in the given vector.
// Returns 0 for an empty vector.
int countDistinct(const std::vector<int>& a) {
    std::set<int> unique_values;
    for (int x : a) {
        unique_values.insert(x);
    }
    return static_cast<int>(unique_values.size());
}

#include <cassert>
#include <vector>

int countDistinct(const std::vector<int>& a);

int main() {
    assert(countDistinct({}) == 0);
    assert(countDistinct({1}) == 1);
    assert(countDistinct({1, 2, 3}) == 3);
    assert(countDistinct({1, 1, 1}) == 1);
    assert(countDistinct({-1, 0, 1, -1, 0}) == 3);
    assert(countDistinct({5, 5, 2, 2, 5, 3}) == 3);
    assert(countDistinct({1000000, -1000000, 0, 1000000}) == 3);
    assert(countDistinct({7, 7, 7, 8, 9, 9, 10}) == 4);
    assert(countDistinct({-5, -5, -4, -5, -3}) == 3);
    assert(countDistinct({0, 0, 0, 0, 0}) == 1);
    return 0;
}

// The simplest and most efficient approach is to insert all elements of the array into a `std::set<int>`. A set automatically discards duplicate values and stores only unique elements. The size of the set after insertion gives the count of distinct values. For an empty array, the set remains empty and its size is 0, which naturally satisfies the edge case. Time complexity is O(n log n) due to each insertion/ lookup in a balanced tree, and space complexity is O(k) where k is the number of distinct elements. Since `std::set` stores elements in sorted order, no additional sorting is needed. This solution is robust for all valid integer inputs, including negative and large positive values.
