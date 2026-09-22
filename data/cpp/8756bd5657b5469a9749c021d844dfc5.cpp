/*
Write a C++ function that processes an array of the first `n` positive integers (where `n` is a parameter, 1 ≤ n ≤ 1000), and returns a struct containing four integer statistics: the count of values exactly equal to 30 (if 30 appears in the range), the sum of all elements, the count of elements strictly greater than the arithmetic mean, and the count of elements exactly equal to the mean (when the mean is an integer). The function must handle cases where 30 is outside the range (then the count is 0), where the mean is not an integer (then the "equal to mean" count is 0), and must correctly compute the mean as a double for comparisons. The struct should be returned by value. Do not use any standard containers; use a plain array allocated inside the function.
*/
#include <cstddef>

// Statistics for the first n positive integers.
struct Stats {
    int countEqualTo30;
    int countGreaterThanMean;
    int countEqualToMean;
    long long sum;
};

/**
 * Computes statistics for an array of the first n positive integers.
 * @param n number of elements (1 <= n <= 1000)
 * @return Stats containing sums and counts.
 */
Stats analyzeFirstNIntegers(int n) {
    // Use a fixed-size array since n <= 1000; can also use dynamic allocation.
    int values[1000];
    
    long long totalSum = 0;
    int count30 = 0;
    
    // Fill array and compute sum, count 30 if present.
    for (int i = 0; i < n; ++i) {
        values[i] = i + 1;
        totalSum += values[i];
        if (values[i] == 30) {
            ++count30;
        }
    }
    
    double mean = static_cast<double>(totalSum) / n;
    
    int countGreater = 0;
    int countEqualMean = 0;
    
    // Second pass to compare with mean.
    for (int i = 0; i < n; ++i) {
        if (values[i] > mean) {
            ++countGreater;
        } else if (values[i] == mean) {
            ++countEqualMean;
        }
    }
    
    Stats result;
    result.countEqualTo30 = count30;
    result.countGreaterThanMean = countGreater;
    result.countEqualToMean = countEqualMean;
    result.sum = totalSum;
    return result;
}
#include <cassert>

int main() {
    // n = 5, array {1,2,3,4,5}, sum=15, mean=3.0
    Stats s1 = analyzeFirstNIntegers(5);
    assert(s1.countEqualTo30 == 0);
    assert(s1.countGreaterThanMean == 2); // 4,5 > 3
    assert(s1.countEqualToMean == 1);     // 3 == 3
    assert(s1.sum == 15);

    // n = 30, array includes 30 exactly once
    Stats s2 = analyzeFirstNIntegers(30);
    assert(s2.countEqualTo30 == 1);
    assert(s2.sum == 465); // sum 1..30 = 30*31/2
    double mean2 = 465.0 / 30; // 15.5
    assert(s2.countEqualToMean == 0); // mean not integer
    assert(s2.countGreaterThanMean == 15); // 16..30 > 15.5

    // n = 1, array {1}, mean=1.0
    Stats s3 = analyzeFirstNIntegers(1);
    assert(s3.countEqualTo30 == 0);
    assert(s3.countGreaterThanMean == 0);
    assert(s3.countEqualToMean == 1);
    assert(s3.sum == 1);

    // n = 2, array {1,2}, mean=1.5
    Stats s4 = analyzeFirstNIntegers(2);
    assert(s4.countEqualTo30 == 0);
    assert(s4.countGreaterThanMean == 1); // 2 > 1.5
    assert(s4.countEqualToMean == 0);
    assert(s4.sum == 3);

    // n = 1000, mean = 500.5, no equal mean, count of 30 = 1
    Stats s5 = analyzeFirstNIntegers(1000);
    assert(s5.countEqualTo30 == 1);
    assert(s5.countEqualToMean == 0);
    assert(s5.countGreaterThanMean == 500); // 501..1000 > 500.5
    assert(s5.sum == 500500);

    // n = 31, mean = 16.0, equal count = 1 (value 16)
    Stats s6 = analyzeFirstNIntegers(31);
    assert(s6.countEqualTo30 == 1);
    assert(s6.countGreaterThanMean == 15); // 17..31 > 16
    assert(s6.countEqualToMean == 1);      // 16 == 16
    assert(s6.sum == 496); // 31*32/2
}
// The solution generates an array `v` of size `n` with values `1, 2, ..., n` (so `v[i] = i+1`). We compute the sum by iterating once. The count of values equal to 30 is simply `1` if `n >= 30`, otherwise `0`; but the algorithm should still check each element to be general. After computing the sum, we compute the mean as `soma / (double)n`. Then we iterate again to compare each element with the mean. For the "equal to mean" count, we compare `v[i] == media` where `media` is a double; this is safe because `media` is either an integer exactly (when the sum is divisible by n) or a non-integer. If it is non-integer, no element can equal it because all elements are integers. Edge cases: n < 30 means no 30; n=1 gives mean = 1, so count equal to mean is 1; n=2 gives array {1,2}, mean=1.5, no equal elements. Time complexity is O(n) for two passes (or one pass for sum and one for comparisons), space O(n) for the array. The struct returned holds four `int` fields.
