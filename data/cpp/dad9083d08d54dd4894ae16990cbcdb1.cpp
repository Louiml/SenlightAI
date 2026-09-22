// Write a C++ function `int countMissingInRange(const std::vector<int>& sortedValues, int low, int high)` that, given a sorted vector of distinct integers and a closed interval `[low, high]`, returns the number of integers in that interval that do **not** appear in the vector. The vector may be empty, and the interval may be large (values up to `1e9`). You cannot assume the vector contains all or any numbers in the interval. Use binary search to achieve efficiency. For example, if the vector is `{1,2,3,5,6,7,8,9,10}` and the interval is `[4,4]`, the function returns `1` because 4 is missing.
#include <cassert>
#include <vector>

// Include the solution function here (or link it)

int main() {
    // Example from the snippet: vector {1,2,3,5,6,7,8,9,10}, interval [4,4]
    std::vector<int> a = {1,2,3,5,6,7,8,9,10};
    assert(countMissingInRange(a, 4, 4) == 1);
    // Entire vector covers interval, no missing
    assert(countMissingInRange(a, 5, 10) == 0);
    // Interval below all values
    assert(countMissingInRange(a, -5, 0) == 6); // -5,-4,-3,-2,-1,0 all missing
    // Interval above all values
    assert(countMissingInRange(a, 11, 15) == 5);
    // Empty vector
    std::vector<int> empty;
    assert(countMissingInRange(empty, 1, 5) == 5);
    // Interval with all values present
    std::vector<int> b = {2,4,6};
    assert(countMissingInRange(b, 2, 6) == 2); // missing 3 and 5
    // Single element interval
    assert(countMissingInRange(b, 3, 3) == 1);
    // Interval exactly matches one element
    assert(countMissingInRange(b, 4, 4) == 0);
    // Large interval with sparse values
    std::vector<int> c = {1000000000};
    assert(countMissingInRange(c, 1, 1000000000) == 999999999);
    // low > high handle gracefully
    assert(countMissingInRange(a, 7, 3) == 0);
}
#include <vector>
#include <algorithm>

// Given a sorted vector of distinct integers and a closed interval [low, high],
// return the count of integers in the interval that do not appear in the vector.
int countMissingInRange(const std::vector<int>& sortedValues, int low, int high) {
    if (low > high) {
        return 0;
    }
    // First element >= low
    auto left = std::lower_bound(sortedValues.begin(), sortedValues.end(), low);
    // First element > high
    auto right = std::upper_bound(sortedValues.begin(), sortedValues.end(), high);
    long long intervalSize = static_cast<long long>(high) - low + 1;
    long long presentCount = std::distance(left, right);
    return static_cast<int>(intervalSize - presentCount);
}
// The key observation is that if the vector is sorted, we can quickly find how many elements fall within `[low, high]` using two binary searches. The number of missing integers is simply the total count of integers in the interval, i.e., `high - low + 1`, minus the count of distinct vector elements that lie in the interval. Since the vector is sorted and distinct, we can use `std::lower_bound` to find the first position where an element is `>= low`, and `std::upper_bound` to find the first position where an element is `> high`. The difference of these two positions gives the count of elements in the interval. Edge cases: empty vector → count is zero → missing = interval size; interval entirely outside vector range → binary searches give the same position → missing = interval size; low > high (though we can assume low ≤ high, but handle gracefully by returning 0). Time complexity: O(log n) where n is vector size, because we perform two binary searches. Space complexity: O(1) auxiliary.
