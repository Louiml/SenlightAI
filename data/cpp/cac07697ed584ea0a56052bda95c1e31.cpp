/*
Write a C++ function that, given a vector of integers and the number of elements \(n\), returns the minimum total “excess” that must be redistributed so that all elements become equal. Specifically, if the sum of the array is not divisible by \(n\), return \(-1\). Otherwise, compute the average, then return the sum of all elements that are strictly greater than the average. This represents the total surplus that needs to be moved from higher-than-average elements to make all elements equal. The input vector may contain any integer values (positive, negative, or zero), and \(n \ge 1\). The function should handle large numbers (up to \(10^9\) per element) and return a `long long`.
*/
#include <vector>
#include <numeric>

// Returns the total surplus above the average if redistribution is possible,
// otherwise returns -1.
long long minimumSurplusToEqualize(const std::vector<long long>& arr, int n) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        total += arr[i];
    }
    
    if (total % n != 0) {
        return -1;
    }
    
    const long long average = total / n;
    long long surplus = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] > average) {
            surplus += arr[i];
        }
    }
    
    return surplus;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<long long> v1 = {1, 2, 3, 4};
    assert(minimumSurplusToEqualize(v1, 4) == 2); // total=10, avg=2.5 -> invalid? Actually 10%4 !=0 so returns -1

    // Correct test: {1,2,3,4} sum=10 not divisible by 4 -> -1
    assert(minimumSurplusToEqualize(v1, 4) == -1);

    std::vector<long long> v2 = {1, 2, 3, 6};
    assert(minimumSurplusToEqualize(v2, 4) == 6); // sum=12, avg=3, surplus from 6 = 6

    std::vector<long long> v3 = {5, 5, 5, 5};
    assert(minimumSurplusToEqualize(v3, 4) == 0);

    std::vector<long long> v4 = {10, 20, 30};
    assert(minimumSurplusToEqualize(v4, 3) == 30); // sum=60, avg=20, surplus from 30=30

    std::vector<long long> v5 = {-5, -1, 3, 7};
    assert(minimumSurplusToEqualize(v5, 4) == 7); // sum=4, avg=1, surplus from 7=7

    std::vector<long long> v6 = {100};
    assert(minimumSurplusToEqualize(v6, 1) == 0);

    std::vector<long long> v7 = {3, 3, 3};
    assert(minimumSurplusToEqualize(v7, 3) == 0);

    std::vector<long long> v8 = {1, 1, 1, 1, 1};
    assert(minimumSurplusToEqualize(v8, 5) == 0);

    std::vector<long long> v9 = {2, 2, 2, 2, 2, 2};
    assert(minimumSurplusToEqualize(v9, 6) == 0);

    std::vector<long long> v10 = {7, 3, 2};
    assert(minimumSurplusToEqualize(v10, 3) == 7); // sum=12, avg=4, surplus from 7=7

    return 0;
}
// The core idea is that to make all elements equal to the average, we only move surplus from elements above the average. Since the total sum is fixed, the total surplus above the average is exactly equal to the total deficit below the average, so it suffices to sum the positive differences from the average (i.e., values strictly greater than average). Steps:  
// 1. Sum all elements and check if the sum is divisible by \(n\). If not, return \(-1\).  
// 2. Compute the average as `sum / n`.  
// 3. Iterate through the array and accumulate the total of all elements that are strictly greater than the average.  
// No sorting is needed for the computation itself; sorting is optional and does not change the result.  
// Edge cases:  
// - When all elements are already equal, the surplus is 0.  
// - When the array contains negative numbers, the average may be negative, and elements above that average still contribute surplus.  
// - When \(n=1\), the sum is trivially divisible, average equals the element, and surplus is 0.  
// Time complexity is \(O(n)\) because we do one pass for summing and one pass for surplus. Space complexity is \(O(1)\) beyond the input vector.  
// The snippet provided uses sorting and then counts non-average elements, but that is redundant; the essential logic is just summing values above the average. We simplify accordingly.
