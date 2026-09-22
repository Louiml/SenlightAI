Write a C++ function `sortDescending` that takes a vector of integers and returns a new vector containing the same integers sorted in descending order (largest to smallest). The function should not modify the original vector, must work correctly for vectors containing negative numbers, zeros, and duplicate values, and should handle an empty input vector by returning an empty vector. The function should have a descriptive name, use `const` reference for the input parameter, and return the sorted result by value.
The solution approach is straightforward: create a copy of the input vector (to ensure the original is not modified), then sort the copy in descending order using `std::sort` with `std::greater<int>()` as the comparator. Important edge cases include: an empty vector (which should return an empty vector without errors), vectors with all identical elements (sorting handles duplicates correctly), and negative numbers (since `std::greater<int>()` works on the actual integer values, negatives are naturally ordered). The time complexity is \(O(n \log n)\) due to the sort, and the auxiliary space complexity is \(O(n)\) because we make a copy of the input vector. If we were allowed to sort in place, space would be \(O(1)\), but the task requires non-modification of the original.
#include <vector>
#include <algorithm>
#include <functional>

// Returns a new vector containing the elements of input sorted in descending order.
// The original input vector is left unchanged.
std::vector<int> sortDescending(const std::vector<int>& input) {
    std::vector<int> result = input;  // copy to avoid modifying the original
    std::sort(result.begin(), result.end(), std::greater<int>());
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above (assume it's included).
// Test the sortDescending function with various cases.
int main() {
    std::vector<int> v1 = {3, 1, 2};
    assert(sortDescending(v1) == std::vector<int>({3, 2, 1}));
    assert(v1 == std::vector<int>({3, 1, 2}));  // original unchanged

    std::vector<int> v2 = {-5, -1, -10};
    assert(sortDescending(v2) == std::vector<int>({-1, -5, -10}));

    std::vector<int> v3 = {7};
    assert(sortDescending(v3) == std::vector<int>({7}));

    std::vector<int> v4 = {3, 3, 3};
    assert(sortDescending(v4) == std::vector<int>({3, 3, 3}));

    std::vector<int> v5 = {};
    assert(sortDescending(v5) == std::vector<int>({}));

    std::vector<int> v6 = {10, -2, 8, 0, 10};
    assert(sortDescending(v6) == std::vector<int>({10, 10, 8, 0, -2}));

    return 0;
}
