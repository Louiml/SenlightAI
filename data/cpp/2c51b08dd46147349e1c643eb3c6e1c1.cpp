Write a C++ function that takes a vector of integers and returns the number of distinct elements in the vector. The function should count only unique values, ignoring duplicates, and return the count as an integer. The input vector may contain negative numbers, zeros, and duplicates in any order. The function must not modify the input vector and should work efficiently for large vectors.

// The simplest approach is to insert all elements into a `std::set` (or `std::unordered_set`), which automatically removes duplicates. The size of the set gives the number of distinct values. Using `std::set` ensures sorted order but at a cost of `O(log n)` insertion per element, resulting in `O(n log n)` total time and `O(n)` auxiliary space in the worst case. If we use `std::unordered_set`, the average insertion time is `O(1)`, giving `O(n)` average time and `O(n)` space. Edge cases include an empty vector (return 0), a vector with all identical elements (return 1), and negative numbers (handled naturally by the set). The function must be `const`-correct by accepting the vector by const reference and not modifying it.

#include <vector>
#include <unordered_set>

// Count the number of distinct elements in the input vector.
// Returns 0 for an empty vector.
int countDistinct(const std::vector<int>& values) {
    std::unordered_set<int> unique_values(values.begin(), values.end());
    return static_cast<int>(unique_values.size());
}

#include <cassert>
#include <vector>

int countDistinct(const std::vector<int>& values);

int main() {
    assert(countDistinct({}) == 0);
    assert(countDistinct({7}) == 1);
    assert(countDistinct({1, 2, 3, 4, 5}) == 5);
    assert(countDistinct({5, 5, 5, 5}) == 1);
    assert(countDistinct({-1, -1, 0, 0, 2, -3}) == 4);
    assert(countDistinct({1, 2, 1, 2, 3, 1, 3, 2, 1}) == 3);
    assert(countDistinct({0, 0, -1, 1, -1}) == 3);
    assert(countDistinct({100, -100, 100, -100, 0}) == 3);
    assert(countDistinct({1, 2, 3, 3, 2, 1}) == 3);
    assert(countDistinct({5, 4, 3, 2, 1, 1, 2, 3, 4, 5}) == 5);
    return 0;
}
