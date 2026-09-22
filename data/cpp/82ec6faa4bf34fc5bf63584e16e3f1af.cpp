/*
Write a C++ function named `countOccurrences` that takes a vector of integers and a target integer as parameters, and returns the number of times the target appears in the vector. The function must handle vectors of any size (including empty) and work correctly with negative numbers and duplicates. Do not modify the input vector, and ensure the solution uses only standard library facilities.
*/
#include <vector>

// Count occurrences of a target value in a vector of integers.
// Returns the number of times target appears in values.
int countOccurrences(const std::vector<int>& values, int target) {
    int count = 0;
    for (int value : values) {
        if (value == target) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// (Include the solution function here or via header.)

int main() {
    // Basic case with duplicates
    std::vector<int> v1 = {1, 2, 3, 2, 4, 2};
    assert(countOccurrences(v1, 2) == 3);
    assert(countOccurrences(v1, 5) == 0);
    assert(countOccurrences(v1, 1) == 1);

    // Empty vector
    std::vector<int> empty;
    assert(countOccurrences(empty, 42) == 0);

    // Negative numbers and target
    std::vector<int> v2 = {-1, -2, -1, 0, -1};
    assert(countOccurrences(v2, -1) == 3);
    assert(countOccurrences(v2, 0) == 1);

    // All same values
    std::vector<int> v3 = {7, 7, 7, 7};
    assert(countOccurrences(v3, 7) == 4);

    // Single element vector
    std::vector<int> v4 = {100};
    assert(countOccurrences(v4, 100) == 1);
    assert(countOccurrences(v4, 0) == 0);

    return 0;
}
// The solution iterates through the entire vector once, comparing each element to the target. A counter is incremented whenever a match is found. This approach works for empty vectors (returns 0), and handles negative numbers and duplicates naturally because the comparison is exact. The main algorithm is a single linear pass, giving O(n) time complexity where n is the size of the vector. Space complexity is O(1) since we only use a single integer counter, and the input vector is not modified (passed by `const` reference). Edge cases include: empty vector (returns 0), target not present (returns 0), all elements matching (returns size), and large vectors where the counter must be an `int` (assuming typical int range; if needed, could use `size_t` but the specification implies counting occurrences as an integer count).
