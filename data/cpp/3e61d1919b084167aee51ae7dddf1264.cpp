Write a C++ function named `analyzeVector` that takes a `const std::vector<int>&` as its sole parameter and returns a `std::vector<int>` containing exactly three elements: the minimum value, the maximum value, and the sum of all elements in the input vector, in that order. The function must handle empty vectors by returning `{0, 0, 0}` as a special case (since there is no meaningful minimum or maximum). For non-empty vectors, it must correctly process negative numbers, duplicates, and large values. The input vector must not be modified, and the function should be efficient for vectors of any size.
// The solution involves a single pass through the input vector, initializing the minimum and maximum from the first element and the sum to zero. For each subsequent element, update the minimum if smaller, the maximum if larger, and add the value to the sum. The empty-vector case is checked first before any iteration, returning a vector of three zeros. This approach ensures correctness for all edge cases: vectors with one element (where min equals max equals that element), negative numbers (since comparisons work on actual integer values), duplicates (no special handling needed), and large numbers (since `int` sum may overflow, but that is acceptable unless specified otherwise; using `long long` internally could be more robust, but the task implies `int` sum). Time complexity is O(n) where n is the size of the input vector, and space complexity is O(1) auxiliary, since we only use a few scalar variables and the output vector of fixed size.
#include <vector>
#include <algorithm> // for std::min, std::max

// Returns a vector containing {minimum, maximum, sum} of the input vector.
// For an empty input, returns {0, 0, 0}.
std::vector<int> analyzeVector(const std::vector<int>& data) {
    if (data.empty()) {
        return {0, 0, 0};
    }

    int minVal = data[0];
    int maxVal = data[0];
    int sum = 0;

    for (const int value : data) {
        minVal = std::min(minVal, value);
        maxVal = std::max(maxVal, value);
        sum += value;
    }

    return {minVal, maxVal, sum};
}
#include <cassert>
#include <vector>

int main() {
    // Basic test with multiple positive numbers
    std::vector<int> v1 = {5, 2, 9, 1, 7};
    std::vector<int> r1 = analyzeVector(v1);
    assert(r1.size() == 3);
    assert(r1[0] == 1 && r1[1] == 9 && r1[2] == 24);

    // Empty vector
    std::vector<int> v2;
    std::vector<int> r2 = analyzeVector(v2);
    assert(r2.size() == 3);
    assert(r2[0] == 0 && r2[1] == 0 && r2[2] == 0);

    // Single element
    std::vector<int> v3 = {42};
    std::vector<int> r3 = analyzeVector(v3);
    assert(r3[0] == 42 && r3[1] == 42 && r3[2] == 42);

    // Negative numbers and duplicates
    std::vector<int> v4 = {-3, -10, -1, -3, 0, 5};
    std::vector<int> r4 = analyzeVector(v4);
    assert(r4[0] == -10 && r4[1] == 5 && r4[2] == -12);

    // All equal values
    std::vector<int> v5 = {7, 7, 7, 7};
    std::vector<int> r5 = analyzeVector(v5);
    assert(r5[0] == 7 && r5[1] == 7 && r5[2] == 28);

    return 0;
}
