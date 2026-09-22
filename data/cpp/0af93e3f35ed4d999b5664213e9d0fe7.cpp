// Write a standalone C++ function that computes both the median and the sample variance (using Bessel's correction, dividing by size-1 instead of size) of an array of `double` values provided via a pointer and its size. The function must return a `std::pair<double, double>` where the first element is the median and the second is the sample variance. To avoid modifying the input array, make a copy before sorting for the median calculation. Handle edge cases: for size 1, the sample variance is undefined (return 0.0 as a sentinel) and the median is the single element. For size 0, return `{0.0, 0.0}`. The function must not use any global or static variables, must not allocate memory unnecessarily, and must be `const`-correct with respect to the input array.
#include <cassert>
#include <cmath>
#include <iostream>

// The function is declared here (in a real project it would be in a header).
// For this test, we include the definition directly (or link it).
// The test uses #include "solution.h" or copy-paste the function.

int main() {
    // Test 1: Empty array
    {
        double arr[] = {};
        auto [med, var] = medianAndSampleVariance(arr, 0);
        assert(med == 0.0 && var == 0.0);
    }

    // Test 2: Single element
    {
        double arr[] = {42.0};
        auto [med, var] = medianAndSampleVariance(arr, 1);
        assert(med == 42.0 && var == 0.0);
    }

    // Test 3: Odd size, known values
    {
        double arr[] = {3.0, 1.0, 2.0};
        auto [med, var] = medianAndSampleVariance(arr, 3);
        // Median: sorted [1,2,3] -> 2
        // Sample variance: (( (3-2)^2+(1-2)^2+(2-2)^2 ) / 2 ) = (1+1+0)/2 = 1
        assert(std::fabs(med - 2.0) < 1e-12);
        assert(std::fabs(var - 1.0) < 1e-12);
    }

    // Test 4: Even size, known values
    {
        double arr[] = {10.0, 20.0, 30.0, 40.0};
        auto [med, var] = medianAndSampleVariance(arr, 4);
        // Median: (20+30)/2 = 25
        // Sample variance: mean=25, ( (225+25+25+225)/3 ) = 500/3 ≈ 166.6667
        assert(std::fabs(med - 25.0) < 1e-12);
        assert(std::fabs(var - 500.0/3.0) < 1e-12);
    }

    // Test 5: All identical values
    {
        double arr[] = {5.0, 5.0, 5.0, 5.0};
        auto [med, var] = medianAndSampleVariance(arr, 4);
        assert(med == 5.0);
        assert(std::fabs(var) < 1e-12);
    }

    // Test 6: Negative values
    {
        double arr[] = {-4.0, -2.0, -6.0};
        auto [med, var] = medianAndSampleVariance(arr, 3);
        // Sorted: [-6,-4,-2], median=-4
        // Mean = -4, squared deviations = (4+0+4)=8, /2=4
        assert(std::fabs(med + 4.0) < 1e-12);
        assert(std::fabs(var - 4.0) < 1e-12);
    }

    // Test 7: Large size, small values to check variance formula
    {
        double arr[100];
        for (int i = 0; i < 100; ++i) arr[i] = i; // 0..99
        auto [med, var] = medianAndSampleVariance(arr, 100);
        // Median: (49+50)/2 = 49.5
        assert(std::fabs(med - 49.5) < 1e-10);
        // Population variance of 0..99 = (n^2-1)/12 = 833.25, sample variance = n/(n-1)*pop = 100/99*833.25 ≈ 841.666...
        double expected = (100.0/99.0) * ( (100.0*100.0 - 1.0) / 12.0 );
        assert(std::fabs(var - expected) < 1e-6);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <utility>   // for std::pair
#include <algorithm> // for std::sort
#include <cstddef>   // for std::size_t

// Computes the median and sample variance (n-1 denominator) of an array.
// For size 0, returns {0.0, 0.0}; for size 1, returns {data[0], 0.0}.
std::pair<double, double> medianAndSampleVariance(const double* data, std::size_t size) {
    if (size == 0) {
        return {0.0, 0.0};
    }
    if (size == 1) {
        return {data[0], 0.0};
    }

    // --- Median: copy and sort ---
    double* copy = new double[size];
    for (std::size_t i = 0; i < size; ++i) {
        copy[i] = data[i];
    }
    std::sort(copy, copy + size);

    double median;
    if (size % 2 == 1) {
        median = copy[size / 2];
    } else {
        median = (copy[size / 2 - 1] + copy[size / 2]) / 2.0;
    }
    delete[] copy;

    // --- Sample variance (Bessel's correction) ---
    double sum = 0.0;
    double sumSq = 0.0;
    for (std::size_t i = 0; i < size; ++i) {
        sum += data[i];
        sumSq += data[i] * data[i];
    }
    double mean = sum / static_cast<double>(size);
    double variance = (sumSq - static_cast<double>(size) * mean * mean) / static_cast<double>(size - 1);

    return {median, variance};
}
// The problem requires computing two distinct statistics: the median (which needs sorting) and the sample variance (which needs a single pass for sums). The solution approach:  
// 1. **Median**: Copy the input array into a new dynamically allocated array (or use `std::vector` for safety, but the task expects a raw-pointer-based function). Sort the copy using a standard sort (e.g., `std::sort`). If the size is odd, return the middle element; if even, return the average of the two middle elements.  
// 2. **Sample variance**: Use the two-pass or one-pass formula. The one-pass formula `sumx2 - (sumx^2)/n` can suffer from numerical cancellation, but for a teaching task it's acceptable. However, Bessel's correction divides by `(size-1)`. For size 1, division by zero occurs; we return 0.0 as a sentinel. For size 0, also return 0.0.  
// 3. **Edge cases**:  
//    - size 0 and size 1: return `{0.0, 0.0}` and `{value, 0.0}` respectively.  
//    - All identical values: variance is 0, median is that value.  
//    - Negative values: handled naturally.  
// 4. **Complexity**:  
//    - Time: O(n log n) due to sorting; the variance pass is O(n).  
//    - Space: O(n) for the copy used in sorting.  
// 5. **Const correctness**: The input pointer is `const double*`, and we copy anyway, so we never modify the original.
