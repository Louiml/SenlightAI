// Write a C++ function that reads a sequence of positive integers (one per line) and returns the maximum number of times any single input integer can be divided by 2 (using integer division) before it becomes odd. The function should accept a vector of integers as input and return the largest "twoness" count among all elements. For example, 8 can be divided by 2 three times (8→4→2→1), so its count is 3; 12 can be divided twice (12→6→3); 7 has count 0. The function must handle large values (up to 10^18) and return the maximum count. If the input vector is empty, return 0. You must implement the core logic as a free function named `maxPowerOfTwoDivisorCount` that takes a `const std::vector<long long>&` and returns an `int`. The function should not print anything—only return the result.
#include <cassert>
#include <vector>

// Declaration or include of the solution function here.
int maxPowerOfTwoDivisorCount(const std::vector<long long>& numbers);

int main() {
    // Basic cases
    assert(maxPowerOfTwoDivisorCount({8}) == 3);
    assert(maxPowerOfTwoDivisorCount({7}) == 0);
    assert(maxPowerOfTwoDivisorCount({12}) == 2);
    // Multiple numbers
    assert(maxPowerOfTwoDivisorCount({4, 16, 2, 5}) == 4);
    assert(maxPowerOfTwoDivisorCount({1, 3, 5, 9}) == 0);
    // Mixed large and small
    assert(maxPowerOfTwoDivisorCount({1000000000000000000LL, 2, 1024}) == 10);
    // Empty vector
    assert(maxPowerOfTwoDivisorCount({}) == 0);
    // Large power of two
    assert(maxPowerOfTwoDivisorCount({1LL << 60}) == 60);
    return 0;
}
#include <vector>

// Returns the maximum number of times any input integer can be divided by 2 before becoming odd.
// Empty input yields 0.
int maxPowerOfTwoDivisorCount(const std::vector<long long>& numbers) {
    int maxCount = 0;
    for (long long num : numbers) {
        int count = 0;
        while (num % 2 == 0) {
            num /= 2;
            ++count;
        }
        if (count > maxCount) {
            maxCount = count;
        }
    }
    return maxCount;
}
// The solution involves iterating over each number in the input vector. For each number, repeatedly divide by 2 while the number is even, counting how many divisions occur. This count represents the exponent of 2 in the number's prime factorization (i.e., the number of trailing zero bits). Track the maximum count across all numbers. Edge cases: an empty vector should return 0; numbers that are already odd have a count of 0; very large numbers (up to 10^18) still fit in `long long` and the loop terminates quickly because each division reduces the number by half. Time complexity is O(n * log(max_num)), where n is the size of the vector, and each number is divided at most 60 times (since 2^60 > 10^18). Space complexity is O(1) auxiliary, not counting the input vector itself.
