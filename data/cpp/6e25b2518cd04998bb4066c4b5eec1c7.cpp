Write a C++ function named `averageOdd` that takes a vector of integers as input and returns the average (as a double) of all odd numbers in the vector. If there are no odd numbers, the function should return 0.0. The function must handle positive, negative, and zero values correctly, and the average must be computed as a floating-point value (not integer division). For example, given `{1, 3, 5}`, the function returns 4.0; given `{-1, 2, 3}` it returns 1.0 (since (-1+3)/2 = 1.0); given `{2, 4}` it returns 0.0. The input vector may be empty, in which case the result is 0.0.
The solution iterates once over the input vector, summing all elements that are odd (i.e., `value % 2 != 0`). We also count how many odd numbers are found. After the loop, if the count is zero, return 0.0 to avoid division by zero. Otherwise, compute the average as `static_cast<double>(sum) / count` to ensure floating-point division, since integer division would truncate the fractional part. The function must be `const`-correct: the input vector is read-only, so take it by `const std::vector<int>&`. Time complexity is O(n) for n elements in the vector, and space complexity is O(1) beyond the input storage. Edge cases include an empty vector, a vector with no odd numbers, negative odd numbers (which are correctly handled by `% 2 != 0`), and mixtures of positive/negative odds.
#include <vector>

// Return the average of all odd numbers in the vector; 0.0 if none exist.
double averageOdd(const std::vector<int>& numbers) {
    double sum = 0.0;
    int count = 0;
    
    for (int value : numbers) {
        if (value % 2 != 0) {  // odd numbers include negatives
            sum += value;
            ++count;
        }
    }
    
    if (count == 0) {
        return 0.0;
    }
    
    return sum / count;
}
#include <cassert>
#include <vector>

// The solution function is declared above (included).
int main() {
    // Basic positive odds
    assert(averageOdd({1, 3, 5}) == 4.0);
    // Mixed positive and negative odds
    assert(averageOdd({-1, 2, 3}) == 1.0);
    // No odds -> 0
    assert(averageOdd({2, 4, 6}) == 0.0);
    // Empty vector -> 0
    assert(averageOdd({}) == 0.0);
    // Single negative odd
    assert(averageOdd({-7}) == -7.0);
    // All same value
    assert(averageOdd({7, 7, 7}) == 7.0);
    // Zero is even, should be ignored
    assert(averageOdd({0, 5, -3}) == 1.0); // (5 + -3)/2 = 1.0
}
