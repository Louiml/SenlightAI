Write a C++ function that takes a `std::vector<int>` and returns the total number of occurrences of the maximum value plus the total number of occurrences of the minimum value in the vector. The vector will contain at least one element, and the integers can be negative, zero, or positive. The function must be `const`-correct and should not modify the input. For example, given `{1, 2, 3, 3, 2}`, the minimum is `1` (occurs once) and the maximum is `3` (occurs twice), so the result is `1 + 2 = 3`. Given `{5, -1, -1, 5, 5}`, the minimum is `-1` (occurs twice) and the maximum is `5` (occurs three times), so the result is `2 + 3 = 5`.

// The solution requires a single pass to find both the minimum and maximum values, and a second pass to count their occurrences. Alternatively, a single pass can track both the current min/max and their counts simultaneously by using a state machine: for each element, if it is strictly greater than the current max, reset the max count to 1 and update max; if equal to max, increment max count; similarly for min (strictly less resets count, equal increments). This avoids a second travers. Edge cases include a vector with all identical values (the same value is both min and max, and its count is counted once for min and once for max, so the result is `2 * (size)`), negative numbers, and single-element vectors (result = 2). Time complexity is `O(n)` with `O(1)` auxiliary space. The implementation must handle `INT_MIN` and `INT_MAX` initializations carefully; initializing max to `INT_MIN` and min to `INT_MAX` works for any input, but the counting logic must be correct for the first element.

#include <vector>
#include <climits>

// Returns the total number of occurrences of the minimum value plus the
// maximum value in the given vector. The vector must be non-empty.
int minMaxOccurrenceSum(const std::vector<int>& input) {
    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    int minCount = 0;
    int maxCount = 0;

    for (int value : input) {
        if (value < minVal) {
            minVal = value;
            minCount = 1;
        } else if (value == minVal) {
            ++minCount;
        }

        if (value > maxVal) {
            maxVal = value;
            maxCount = 1;
        } else if (value == maxVal) {
            ++maxCount;
        }
    }

    return minCount + maxCount;
}

#include <cassert>
#include <vector>

// Function declaration for testing (already defined above, but keep for clarity)
int minMaxOccurrenceSum(const std::vector<int>& input);

int main() {
    assert(minMaxOccurrenceSum({1, 2, 3, 3, 2}) == 3);
    assert(minMaxOccurrenceSum({5, -1, -1, 5, 5}) == 5);
    assert(minMaxOccurrenceSum({7}) == 2);
    assert(minMaxOccurrenceSum({4, 4, 4, 4}) == 8);
    assert(minMaxOccurrenceSum({-3, 0, -3, 0, -3}) == 5);
    assert(minMaxOccurrenceSum({100, -100, 100, -100, 0}) == 4);
    assert(minMaxOccurrenceSum({2, 2, 3, 3, 3}) == 5);
    assert(minMaxOccurrenceSum({-1, -1, -1, 2, 2}) == 5);
    return 0;
}
