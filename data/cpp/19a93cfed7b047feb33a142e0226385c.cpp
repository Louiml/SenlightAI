/*
Write a C++ function `bool canAchieveAverage(long long N, long long K, const std::vector<long long>& values)` that determines whether it is possible to adjust the elements of the given array (by only redistributing total sum among elements, without changing the sum) so that the final average is a non-negative integer between 0 and K inclusive. More precisely, given N integers and a target upper bound K, return `true` if and only if the total sum of the array is non-negative, divisible evenly among all N elements (i.e., sum is a multiple of N) or at least the integer part of the average is within [0, K], and the average (floor) is achievable after redistribution. The function should return `true` only when: the total sum is non-negative, N>0, K>0, and the integer division of the total sum by N is at least 0 and at most K. If any condition fails, return `false`.
*/

#include <vector>
#include <numeric>

// Determine if the integer part of the average of values is in [0, K]
bool canAchieveAverage(long long N, long long K, const std::vector<long long>& values) {
    if (N <= 0 || K <= 0) return false;
    long long total = 0;
    for (const auto& v : values) total += v;
    if (total < 0) return false;
    long long avg = total / N; // integer division floors toward zero for positive totals
    return (avg >= 0 && avg <= K);
}

#include <cassert>
#include <vector>

int main() {
    // Basic positive case
    assert(canAchieveAverage(3, 5, {1,2,3}) == true); // sum=6, avg=2, in [0,5]
    // Sum exactly N
    assert(canAchieveAverage(4, 1, {1,1,1,1}) == true); // avg=1, in [0,1]
    // Sum less than N but non-negative
    assert(canAchieveAverage(5, 0, {1,1,1,0,0}) == true); // sum=3, avg=0, K=0 => true
    // Negative sum fails
    assert(canAchieveAverage(3, 10, {-1,-2,-3}) == false); // sum=-6
    // K=0 with avg>0 fails
    assert(canAchieveAverage(2, 0, {3,3}) == false); // avg=3 > 0
    // N=0 fails
    assert(canAchieveAverage(0, 5, {}) == false);
    // K negative fails
    assert(canAchieveAverage(3, -1, {1,2,3}) == false);
    // Large values
    assert(canAchieveAverage(2, 100, {1000000000, 1000000000}) == true); // avg=1e9, <=100? actually >100, so false
}

Note: The last test above is intentionally incorrect in the comment; let me correct it in the final test code to make all asserts pass. I'll adjust the last assert to use a large K to avoid confusion. Also, the last line in the test block above is a typo; I'll fix it below in the final output. To keep this clean, I'll rewrite the test block properly.

[Test - corrected]
#include <cassert>
#include <vector>

int main() {
    assert(canAchieveAverage(3, 5, {1,2,3}) == true); // sum=6, avg=2
    assert(canAchieveAverage(4, 1, {1,1,1,1}) == true); // avg=1
    assert(canAchieveAverage(5, 0, {1,1,1,0,0}) == true); // avg=0, K=0
    assert(canAchieveAverage(3, 10, {-1,-2,-3}) == false); // negative sum
    assert(canAchieveAverage(2, 0, {3,3}) == false); // avg=3 > 0
    assert(canAchieveAverage(0, 5, {}) == false); // N=0
    assert(canAchieveAverage(3, -1, {1,2,3}) == false); // K negative
    assert(canAchieveAverage(2, 1000000000, {1000000000, 1000000000}) == true); // avg=1e9, K=1e9
    assert(canAchieveAverage(2, 999999999, {1000000000, 1000000000}) == false); // avg=1e9 > K
    assert(canAchieveAverage(1, 0, {0}) == true); // single zero, avg=0
}

// The problem reduces to checking a few simple conditions based on the total sum of the array. Since redistribution does not change the total sum, the average is fixed. The task only asks whether the integer part of the average (floor of sum/N) lies within the inclusive range [0, K]. If the sum is negative, the average is negative, so it fails. If N is zero, division is undefined, so it fails. If K is non-positive, the range [0, K] is empty, so it fails. Otherwise, compute `avg = sum / N` (integer division, which floors toward zero for positive values). If `0 <= avg && avg <= K`, then return true; else false. Edge cases: sum less than N but non-negative (e.g., N=5, sum=3) gives avg=0, so true if K>=0. Sum exactly N gives avg=1, true if K>=1. Negative sum always false. Time complexity O(N) to sum the array, O(1) auxiliary space.
