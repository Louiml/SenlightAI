/*
Write a C++ function `int closestPairSum(const std::vector<int>& numbers)` that receives a non-empty vector of integers and returns the sum of two distinct elements from the vector whose absolute value is closest to zero (i.e., the pair whose sum has the smallest absolute value). If multiple pairs have the same minimal absolute sum, return the minimum possible actual sum among those pairs. The vector may contain negative numbers, zeros, and duplicates. For example, for the input {1, 60, -10, 70, -80, 85}, the closest pair is -80 and 85 with absolute sum 5, so the function returns 5; for {-2, 2, 4}, both (-2+2=0) and (2+4=6) have absolute sums 0 and 6, so the minimal absolute sum is 0, and the returned sum is 0. The function must handle vectors with only two elements, and must not modify the input.
*/
#include <vector>
#include <cstdlib> // for std::abs

// Return the sum of two distinct elements whose sum has the smallest absolute value.
// Among pairs with equal absolute sum, return the smallest actual sum.
int closestPairSum(const std::vector<int>& numbers) {
    // The vector is guaranteed to have at least two elements.
    int bestAbs = std::abs(numbers[0] + numbers[1]);
    int bestSum = numbers[0] + numbers[1];

    // Check all unordered pairs (i < j)
    for (std::size_t i = 0; i < numbers.size() - 1; ++i) {
        for (std::size_t j = i + 1; j < numbers.size(); ++j) {
            int currentSum = numbers[i] + numbers[j];
            int currentAbs = std::abs(currentSum);
            if (currentAbs < bestAbs) {
                bestAbs = currentAbs;
                bestSum = currentSum;
            } else if (currentAbs == bestAbs && currentSum < bestSum) {
                bestSum = currentSum;
            }
        }
    }
    return bestSum;
}
#include <cassert>
#include <vector>

// The function to test is declared above (in the solution section).
// This main function only contains tests.

int main() {
    // Basic example from the problem
    std::vector<int> v1 = {1, 60, -10, 70, -80, 85};
    assert(closestPairSum(v1) == 5);  // -80 + 85 = 5, abs = 5

    // Pair sums to zero
    std::vector<int> v2 = {-2, 2, 4};
    assert(closestPairSum(v2) == 0);  // -2+2=0

    // Ties: same absolute sum but smaller actual sum chosen
    // Pairs: 1+4=5 (abs 5), 2+3=5 (abs 5), also 1+2=3 (abs 3) -> best abs is 3
    std::vector<int> v3 = {1, 2, 3, 4};
    assert(closestPairSum(v3) == 3);  // 1+2=3

    // Another tie example: pairs -1+2=1 (abs 1), 0+1=1 (abs 1) -> choose 0+1=1 and -1+2=1 both give 1, actual sum same
    // Let's make a tie with different actual sums: {-3, 2, 1} -> pairs: -3+2=-1 (abs1), -3+1=-2 (abs2), 2+1=3 (abs3) -> best abs=1, sum=-1
    std::vector<int> v4 = {-3, 2, 1};
    assert(closestPairSum(v4) == -1);

    // Duplicate values: {5,5, -10} -> pairs: 5+5=10 (abs10), 5+(-10)=-5 (abs5), 5+(-10)=-5 (abs5) -> best abs=5, sum=-5
    std::vector<int> v5 = {5, 5, -10};
    assert(closestPairSum(v5) == -5);

    // Only two elements
    std::vector<int> v6 = {7, -3};
    assert(closestPairSum(v6) == 4);

    // All same sign: {10, 20, 30} -> pairs: 30,40,50 -> best abs=30, sum=30
    std::vector<int> v7 = {10, 20, 30};
    assert(closestPairSum(v7) == 30);

    // Mixed with zero: {0, 5, -5} -> pairs: 0+5=5 (abs5), 0-5=-5 (abs5), 5-5=0 (abs0) -> best abs=0, sum=0
    std::vector<int> v8 = {0, 5, -5};
    assert(closestPairSum(v8) == 0);

    // Large numbers: {1000, -1001, 2} -> pairs: -1 (abs1), 1002 (abs1002), -999 (abs999) -> best abs=1, sum=-1
    std::vector<int> v9 = {1000, -1001, 2};
    assert(closestPairSum(v9) == -1);

    return 0;
}
// The core problem is to find the two distinct elements whose sum has the minimum absolute value, and among ties, choose the smallest sum. The most straightforward approach is a brute-force double loop that checks every unordered pair (i < j). For each pair, compute the sum `s = numbers[i] + numbers[j]`, its absolute value `absS`, and compare with the current best. Initialize the best absolute value and best sum from the first pair (indices 0 and 1). Then, for each subsequent pair, if the new absolute sum is strictly smaller than the current best absolute sum, update both best absolute and best sum; if it is equal, then update the best sum only if the new sum is smaller. This correctly handles ties by preferring the smaller actual sum. Edge cases include vectors with exactly two elements (only one pair), vectors containing zeros (a pair summing to zero gives absolute value zero, which is optimal), and duplicates (they are distinct indices, so a pair like (5,5) is allowed if two occurrences exist). The time complexity is O(n²) due to the nested loops, and space complexity is O(1) auxiliary because only a few scalars are used. This is acceptable for moderate-sized inputs; a more efficient O(n log n) approach using sorting and two pointers is possible but not required for correctness.
