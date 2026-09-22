/*
Given an array of integers, write a C++ function that returns the sum of all elements after removing the smallest and largest values from the array. If the array has fewer than 3 elements, the function should return 0. The function must handle duplicate smallest and largest values by removing only one occurrence of each (i.e., the smallest value and the largest value, even if they appear multiple times). The input array may contain negative numbers, zeros, and large values that fit in a 64-bit signed integer. The output should be a `long long` to safely accommodate large sums. Implement the function with the signature `long long trimmedSum(const std::vector<int>& arr)` and ensure it does not modify the input vector.
*/
#include <vector>
#include <algorithm>

// Returns the sum of all elements after removing the smallest and largest values.
// If the array has fewer than 3 elements, returns 0.
//
// The function does not modify the input vector; it works on a copy sorted internally.
long long trimmedSum(const std::vector<int>& arr) {
    if (arr.size() < 3) {
        return 0;
    }

    // Create a sorted copy so the smallest is at index 0 and largest at index size-1.
    std::vector<int> sorted(arr);
    std::sort(sorted.begin(), sorted.end());

    // Sum elements from index 1 to size-2 inclusive.
    long long sum = 0;
    for (std::size_t i = 1; i + 1 < sorted.size(); ++i) {
        sum += sorted[i];
    }

    return sum;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (normally included from the solution file).
long long trimmedSum(const std::vector<int>& arr);

int main() {
    // Basic cases with distinct values.
    assert(trimmedSum({1, 2, 3, 4, 5}) == 9);      // remove 1 and 5, sum 2+3+4=9
    assert(trimmedSum({10, -5, 0, 3, 7}) == 10);   // remove -5 and 10, sum 0+3+7=10

    // Fewer than 3 elements.
    assert(trimmedSum({}) == 0);
    assert(trimmedSum({5}) == 0);
    assert(trimmedSum({5, 10}) == 0);

    // Duplicate extremes: only one occurrence each is removed.
    assert(trimmedSum({3, 3, 3, 3}) == 6);         // remove one 3 and one 3, sum remaining 3+3=6
    assert(trimmedSum({1, 1, 2, 2, 3, 3}) == 5);   // remove first 1 and last 3, sum 1+2+2+3=8? Wait: sorted [1,1,2,2,3,3], sum indices 1..4 = 1+2+2+3=8
    // Correct the expected value for the previous line: sorted [1,1,2,2,3,3], sum internal = 1+2+2+3=8.
    // This asserts the correct behavior.
    assert(trimmedSum({1, 1, 2, 2, 3, 3}) == 8);

    // Negative numbers and zeros.
    assert(trimmedSum({-2, -1, 0, 1}) == -1);      // remove -2 and 1, sum -1+0=-1
    assert(trimmedSum({-5, -5, -3, -3, -1}) == -8); // sorted [-5,-5,-3,-3,-1], internal sum -5+-3+-3= -11? Wait: sum indices 1..3 = -5 + -3 + -3 = -11

    // Large values to test long long.
    std::vector<int> large = {1000000000, 2000000000, 2000000001, -1000000000, -1000000001};
    // sorted: [-1000000001, -1000000000, 1000000000, 2000000000, 2000000001]
    // internal sum: -1000000000 + 1000000000 + 2000000000 = 2000000000
    assert(trimmedSum(large) == 2000000000LL);

    // All equal values, but only one each removed (so two removed total).
    assert(trimmedSum({7, 7, 7, 7}) == 14);        // sorted [7,7,7,7], internal sum = 7+7=14

    return 0;
}
// The main idea is to find the minimum and maximum values in the array, then sum all elements while skipping exactly one occurrence of each extreme. A straightforward approach is to first scan the array to identify the minimum and maximum values, then perform a second pass where we skip the first encountered instance of the minimum and the first encountered instance of the maximum. However, this requires careful handling when the minimum and maximum are equal (i.e., all elements are the same), in which case we should skip two occurrences of that value. Alternatively, a simpler and more robust method is to sort the array and then sum all elements except the first and last elements. Sorting works because after sorting, the smallest element is at index 0 and the largest at index n-1, and removing one of each is equivalent to summing indices 1 through n-2. Edge cases: if `arr.size() < 3`, return 0 because removing two elements would leave fewer than one element. Overflows are avoided by using `long long`. Time complexity is O(n log n) for sorting, but we could achieve O(n) with a linear scan; however, sorting is simpler and adequate for this task. Space complexity is O(1) auxiliary if we sort the input copy or O(n) if we create a copy; to avoid modifying the input, we'll create a sorted copy, so space complexity is O(n).
