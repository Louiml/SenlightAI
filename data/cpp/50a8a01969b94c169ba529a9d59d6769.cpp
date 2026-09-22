/*
Given five integers as input, write a C++ function named `minMaxOfFourSums` that takes a `const std::array<long, 5>&` (or a `std::vector<long>` of length 5) and returns a `std::pair<long, long>` where the first element is the minimum possible sum of exactly four of the five integers, and the second element is the maximum possible sum of exactly four of the five integers. The function must work for any five integers, including negative numbers, very large positive/negative values (up to the limits of `long`), and duplicates. Do not modify the input. Return the pair as {minSum, maxSum}.
*/

#include <array>
#include <utility>
#include <algorithm>

// Returns {minSum, maxSum} where minSum is the smallest sum of any four elements
// and maxSum is the largest sum of any four elements from the given array of five.
std::pair<long, long> minMaxOfFourSums(const std::array<long, 5>& arr) {
    long total = 0;
    long minVal = arr[0];
    long maxVal = arr[0];

    for (long value : arr) {
        total += value;
        minVal = std::min(minVal, value);
        maxVal = std::max(maxVal, value);
    }

    return {total - maxVal, total - minVal};
}

#include <cassert>
#include <array>
#include <utility>

// The solution function declaration (for clarity, not strictly needed if included above)
std::pair<long, long> minMaxOfFourSums(const std::array<long, 5>& arr);

int main() {
    // Basic positive numbers
    std::array<long, 5> a1 = {1, 2, 3, 4, 5};
    auto r1 = minMaxOfFourSums(a1);
    assert(r1.first == 10 && r1.second == 14); // sums: 1+2+3+4=10, 2+3+4+5=14

    // All negative numbers
    std::array<long, 5> a2 = {-5, -3, -1, -2, -4};
    auto r2 = minMaxOfFourSums(a2);
    // Total = -15, minVal=-5 -> maxSum = -15 - (-5) = -10; maxVal=-1 -> minSum = -15 - (-1) = -14
    assert(r2.first == -14 && r2.second == -10);

    // Mixed positive and negative
    std::array<long, 5> a3 = {-10, 5, 0, 3, 7};
    auto r3 = minMaxOfFourSums(a3);
    // Total = 5, minVal=-10 -> maxSum = 15, maxVal=7 -> minSum = -2
    assert(r3.first == -2 && r3.second == 15);

    // Duplicates
    std::array<long, 5> a4 = {4, 4, 4, 4, 4};
    auto r4 = minMaxOfFourSums(a4);
    assert(r4.first == 16 && r4.second == 16);

    // Large values within long range
    std::array<long, 5> a5 = {1000000, 2000000, 3000000, 4000000, 5000000};
    auto r5 = minMaxOfFourSums(a5);
    // Total = 15000000, minVal=1000000 -> maxSum=14000000, maxVal=5000000 -> minSum=10000000
    assert(r5.first == 10000000 && r5.second == 14000000);

    // All zeros
    std::array<long, 5> a6 = {0, 0, 0, 0, 0};
    auto r6 = minMaxOfFourSums(a6);
    assert(r6.first == 0 && r6.second == 0);

    // Edge with two extremes
    std::array<long, 5> a7 = {-100, 1, 1, 1, 100};
    auto r7 = minMaxOfFourSums(a7);
    // Total = 3, minVal=-100 -> maxSum=103, maxVal=100 -> minSum=-97
    assert(r7.first == -97 && r7.second == 103);

    // Single non‑zero with others zero
    std::array<long, 5> a8 = {5, 0, 0, 0, 0};
    auto r8 = minMaxOfFourSums(a8);
    // Total=5, minVal=0 -> maxSum=5, maxVal=5 -> minSum=0
    assert(r8.first == 0 && r8.second == 5);

    return 0;
}

// The core idea is that, given five numbers, the sum of any four is simply the total sum of all five minus one of the numbers. Therefore, to get the minimum possible four‑element sum, subtract the largest element from the total sum; to get the maximum possible four‑element sum, subtract the smallest element from the total sum. This avoids explicitly computing all five combinations (as the original snippet did) and is more efficient.  
// - **Algorithm**: Compute `total = sum of all five elements`. Find `minVal` and `maxVal` among the array. Then `minSum = total - maxVal`, `maxSum = total - minVal`.  
// - **Edge cases**:  
//   - All five numbers identical: minSum == maxSum (since subtracting the same value twice gives the same result).  
//   - Negative numbers: subtracting the smallest (most negative) gives the largest sum, and subtracting the largest (most positive) gives the smallest sum – logic holds.  
//   - Very large numbers: using `long` is safe (matching the original snippet’s data type).  
// - **Complexity**: Time is O(n) where n=5 (a single traversal to find min, max, and total). Space is O(1) auxiliary.
