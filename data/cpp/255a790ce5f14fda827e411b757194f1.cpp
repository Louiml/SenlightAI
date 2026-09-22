Write a C++ function `sumOfExtrema` that takes a `std::vector<float>` (by const reference) and returns the sum of all local extrema (both local minima and local maxima) in the array. A local extremum is defined as an element that is strictly greater than its immediate neighbors (local maximum) or strictly less than its immediate neighbors (local minimum), with special handling for the first and last elements: the first element is a local maximum (or minimum) if it is strictly greater (or less) than its single neighbor, and similarly for the last element. If the vector has exactly one element, treat that single element as a local minimum (and also as a local maximum? — the original code treats it as a local minimum, and as no local maximum; but for clarity, define that for a single-element array, the element is considered both a local minimum and a local maximum, so it counts once). Your function must handle empty arrays by returning 0. The array may contain duplicate values, but comparisons must be strict (e.g., equal neighbors do not count as extrema). Provide a clean implementation with proper `const` correctness.
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// We include the function here for completeness in a single file.
#include "solution.h" // In actual test, the function definition would be placed here.

int main() {
    // Empty vector
    assert(sumOfExtrema({}) == 0.0f);
    // Single element
    assert(sumOfExtrema({3.5f}) == 3.5f);
    // Monotonic increasing: first and last are extrema
    std::vector<float> v1 = {1.0f, 2.0f, 3.0f, 4.0f};
    // first (1.0) is min, last (4.0) is max => sum = 1+4 = 5
    assert(sumOfExtrema(v1) == 5.0f);
    // Monotonic decreasing: first is max, last is min
    std::vector<float> v2 = {5.0f, 4.0f, 3.0f, 2.0f};
    // first (5) max, last (2) min => sum = 5+2 = 7
    assert(sumOfExtrema(v2) == 7.0f);
    // Alternating: 1, 3, 2 → 1 is min, 3 is max, 2 is min? Actually 2 is less than 3? No, 2 has right neighbor? Last element 2 < 3 => min. Sum = 1+3+2 = 6
    std::vector<float> v3 = {1.0f, 3.0f, 2.0f};
    assert(sumOfExtrema(v3) == 6.0f);
    // Equal duplicates: no extrema except endpoints? 2,2,2 → no strict comparisons: first no, interior no, last no => sum=0
    std::vector<float> v4 = {2.0f, 2.0f, 2.0f};
    assert(sumOfExtrema(v4) == 0.0f);
    // Mixed with duplicates: 1,3,3,2 → first 1 min, last 2 min? 2<3 => min, interior have equal neighbor so none. Sum = 1+2 = 3
    std::vector<float> v5 = {1.0f, 3.0f, 3.0f, 2.0f};
    assert(sumOfExtrema(v5) == 3.0f);
    return 0;
}
#include <vector>

/**
 * Returns the sum of all local extrema (local minima and maxima) in the vector.
 * A local extremum is an element strictly greater or strictly less than its immediate neighbors.
 * For a single-element vector, the element is counted once as an extremum.
 * Empty vector returns 0.
 */
float sumOfExtrema(const std::vector<float>& a) {
    int n = (int)a.size();
    if (n == 0) return 0.0f;
    if (n == 1) return a[0]; // count the single element once

    float sum = 0.0f;
    // Check first element
    if (a[0] > a[1]) {
        sum += a[0]; // local maximum
    } else if (a[0] < a[1]) {
        sum += a[0]; // local minimum
    }
    // Check interior elements
    for (int i = 1; i <= n - 2; ++i) {
        if (a[i] > a[i-1] && a[i] > a[i+1]) {
            sum += a[i]; // local maximum
        } else if (a[i] < a[i-1] && a[i] < a[i+1]) {
            sum += a[i]; // local minimum
        }
    }
    // Check last element
    if (a[n-1] > a[n-2]) {
        sum += a[n-1]; // local maximum
    } else if (a[n-1] < a[n-2]) {
        sum += a[n-1]; // local minimum
    }
    return sum;
}
// The approach is to iterate through the vector and check each position for being a local extremum. For the first element (index 0), compare it with the second element (if exists): if `a[0] > a[1]`, it's a local maximum; if `a[0] < a[1]`, it's a local minimum. For the last element (index n-1), compare with the previous element: if `a[n-1] > a[n-2]`, it's a local maximum; if `a[n-1] < a[n-2]`, it's a local minimum. For interior elements (1 ≤ i ≤ n-2), check both neighbors: if `a[i] > a[i-1] && a[i] > a[i+1]`, it's a local maximum; if `a[i] < a[i-1] && a[i] < a[i+1]`, it's a local minimum. Edge cases: empty vector → return 0; single element → we can count it once (e.g., as a local minimum, or as a single extremum) — but to avoid double-counting, we decide to count it exactly once. The time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary. Duplicates are naturally handled because strict comparisons fail for equal values.
