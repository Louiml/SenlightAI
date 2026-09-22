/*
Write a C++ function that takes an array of integer test scores (passed as a `const std::vector<int>&`) and returns the average score rounded down to the nearest integer (i.e., using integer division). The function must handle an empty vector by returning 0. The function should be named `averageScore`, take a `const std::vector<int>&` parameter, and return an `int`. Use a free function (not a member of a class) with proper `const` correctness.
*/
#include <vector>

// Compute the integer average of test scores; return 0 for an empty vector.
int averageScore(const std::vector<int>& scores) {
    if (scores.empty()) {
        return 0;
    }

    long long total = 0;  // Use long long to prevent overflow for large sums.
    for (int score : scores) {
        total += score;
    }

    return static_cast<int>(total / static_cast<long long>(scores.size()));
}
#include <cassert>
#include <vector>

// Declaration for the function under test (normally from a header).
int averageScore(const std::vector<int>& scores);

int main() {
    // Basic case: average of 23, 45, 32 = 100/3 = 33 (integer division)
    std::vector<int> scores1 = {23, 45, 32};
    assert(averageScore(scores1) == 33);

    // Single element
    std::vector<int> scores2 = {85};
    assert(averageScore(scores2) == 85);

    // Empty vector
    std::vector<int> scores3;
    assert(averageScore(scores3) == 0);

    // All equal values
    std::vector<int> scores4 = {70, 70, 70, 70};
    assert(averageScore(scores4) == 70);

    // Values that sum exactly to a multiple of size
    std::vector<int> scores5 = {100, 80, 60};  // sum=240, avg=80
    assert(averageScore(scores5) == 80);

    // Large values to test overflow safety (sum fits in long long)
    std::vector<int> scores6 = {2000000000, 2000000000};  // sum=4e9, avg=2e9
    assert(averageScore(scores6) == 2000000000);

    // Mixed values with rounding down
    std::vector<int> scores7 = {90, 91, 92};  // sum=273, avg=91
    assert(averageScore(scores7) == 91);

    // Negative values (though unlikely for scores, tests robustness)
    std::vector<int> scores8 = {-10, -20, -30};  // sum=-60, avg=-20
    assert(averageScore(scores8) == -20);

    return 0;
}
// The solution computes the sum of all elements in the vector using a range-based loop or `std::accumulate`. If the vector is empty, the function returns 0 immediately to avoid division by zero. Otherwise, the total sum is divided by the size of the vector using integer division, which automatically truncates toward zero (rounds down for positive values). The algorithm processes each element once, so it runs in O(n) time, where n is the number of scores. Space usage is O(1) auxiliary, excluding the input vector itself, since only a sum variable and possibly a loop index are used. Edge cases include an empty vector (return 0) and vectors with only one element (that element is the average). Negative scores would also work correctly with integer division, but the problem implies normal test scores (non-negative), so no special handling is needed beyond the empty case.
