Write a C++ function named `allPositiveAndSum` that accepts a dynamically allocated integer array and its size `n` as parameters, and returns a boolean value. The function should return `true` if every element in the array is strictly greater than zero, and `false` otherwise. Additionally, the function should count the total sum of all elements in the array and pass that sum back to the caller through a reference parameter. If the array is empty (size 0), the function should return `false` and set the sum reference to 0. You must not alter the input array, and the function should be const-correct where appropriate.

// The solution must scan the array once, accumulating a sum while checking each element. For any element that is ≤ 0, the function can immediately return `false` (after still completing the sum? No—since the caller expects the sum regardless of positivity, we must compute the full sum even if we encounter a non-positive element early). Therefore, the algorithm iterates through all elements, adding each to the sum, and if any element is ≤ 0, we set a flag to indicate false. After the loop, assign the accumulated sum to the reference parameter and return the flag. Edge cases: empty array should return false and sum 0; single element positive returns true with that value; all negative returns false with their negative sum. Time complexity is O(n), space O(1) auxiliary.

#include <cstddef>

// Checks if all elements in the array are positive, and outputs their sum.
// Returns true if all elements > 0, false otherwise (including empty arrays).
// The sum is always output, even when returning false.
bool allPositiveAndSum(const int* arr, std::size_t n, long long& sumOut) {
    sumOut = 0;
    bool allPositive = true;
    for (std::size_t i = 0; i < n; ++i) {
        sumOut += arr[i];
        if (arr[i] <= 0) {
            allPositive = false;
        }
    }
    return allPositive;
}

#include <cassert>
#include <cstddef>

// The solution function declaration (omitted here for brevity, but included in the test file)

int main() {
    long long sum = 0;

    // Test 1: All positive
    int a1[] = {1, 2, 3};
    assert(allPositiveAndSum(a1, 3, sum) == true);
    assert(sum == 6);

    // Test 2: Contains zero
    int a2[] = {5, 0, 7};
    assert(allPositiveAndSum(a2, 3, sum) == false);
    assert(sum == 12);

    // Test 3: Contains negative
    int a3[] = {-1, 2, 3};
    assert(allPositiveAndSum(a3, 3, sum) == false);
    assert(sum == 4);

    // Test 4: Empty array
    assert(allPositiveAndSum(nullptr, 0, sum) == false);
    assert(sum == 0);

    // Test 5: Single positive
    int a5[] = {42};
    assert(allPositiveAndSum(a5, 1, sum) == true);
    assert(sum == 42);

    // Test 6: Single negative
    int a6[] = {-7};
    assert(allPositiveAndSum(a6, 1, sum) == false);
    assert(sum == -7);

    // Test 7: Large array with all positives
    int a7[] = {10, 20, 30, 40, 50};
    assert(allPositiveAndSum(a7, 5, sum) == true);
    assert(sum == 150);

    // Test 8: All negative
    int a8[] = {-1, -2, -3};
    assert(allPositiveAndSum(a8, 3, sum) == false);
    assert(sum == -6);

    // Test 9: Positive then negative
    int a9[] = {2, -3, 4};
    assert(allPositiveAndSum(a9, 3, sum) == false);
    assert(sum == 3);

    // Test 10: Mixed with large numbers
    int a10[] = {100, -200, 300};
    assert(allPositiveAndSum(a10, 3, sum) == false);
    assert(sum == 200);

    return 0;
}
