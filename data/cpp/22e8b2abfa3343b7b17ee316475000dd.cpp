// Write a C++ function `int sumOddEvenDifference(int limit, const int arr[], int size)` that takes a positive integer `limit` and an array of integers `arr` along with its size. The function should compute and return the difference between two values: (1) the sum of all integers from 1 to `limit` inclusive, and (2) the sum of all elements in `arr` that are at odd indices (i.e., indices 1, 3, 5, ...). If the array is empty (size 0) or the limit is not positive, the function should return 0. The function must not modify the input array and must handle edge cases such as a limit of 1, an array with fewer than 2 elements, and arrays containing negative numbers or repeated values.

// The solution has three main parts: compute the sum from 1 to limit, sum elements at odd indices of the array, and return the difference. For the sum from 1 to limit, use the closed-form formula `limit * (limit + 1) / 2`, which works in O(1) time and O(1) space, avoiding overflow by using a 64-bit integer internally (but returning an `int` as per spec, so assume inputs are small enough). If limit ≤ 0, return 0 immediately. For the array sum, iterate over indices `i` from 0 to size-1, and if `i % 2 != 0` (odd index), add `arr[i]` to a running total. The edge cases: empty array yields sum 0; array of size 1 has no odd indices, so sum 0; negative values are simply added as is. Complexity is O(size) time and O(1) extra space, plus O(1) for the formula. The difference is computed as `(sum1ToLimit - sumOddIndex)`. If the limit is positive but the array is empty, the result is just the sum1ToLimit. The function should be `const` correct by taking the array as `const int*` or `const int[]`.

#include <cstddef>

// Computes difference between sum of 1..limit and sum of elements at odd indices in arr.
// Returns 0 if limit <= 0 or if arr is null with size > 0 (undefined), but per spec size 0 returns 0.
int sumOddEvenDifference(int limit, const int arr[], std::size_t size) {
    if (limit <= 0) {
        return 0;
    }
    // Sum from 1 to limit using closed-form.
    long long sumLimit = static_cast<long long>(limit) * (limit + 1) / 2;
    
    // Sum elements at odd indices.
    long long oddSum = 0;
    for (std::size_t i = 0; i < size; ++i) {
        if (i % 2 != 0) {
            oddSum += arr[i];
        }
    }
    
    return static_cast<int>(sumLimit - oddSum);
}

int main() {
    // Basic case: limit=5, sum=15, arr={0,1,2,3,4}, odd indices sum=1+3=4, diff=11
    int arr1[] = {0,1,2,3,4};
    assert(sumOddEvenDifference(5, arr1, 5) == 11);
    
    // Empty array: limit=3, sum=6, odd sum=0, diff=6
    int arr2[] = {};
    assert(sumOddEvenDifference(3, arr2, 0) == 6);
    
    // Non-positive limit: limit=0, array anything, returns 0
    int arr3[] = {10,20};
    assert(sumOddEvenDifference(0, arr3, 2) == 0);
    
    // limit=1, array has no elements at odd indices if size=1
    int arr4[] = {7};
    assert(sumOddEvenDifference(1, arr4, 1) == 1);
    
    // Array with odd index negative values: limit=2, sum=3, arr={1,-5,2,3}, odd sum=-5+3=-2, diff=5
    int arr5[] = {1,-5,2,3};
    assert(sumOddEvenDifference(2, arr5, 4) == 5);
    
    // Array with size=2, only index 1 is odd
    int arr6[] = {100, 50};
    assert(sumOddEvenDifference(10, arr6, 2) == 55 - 50); // 5
    
    // Large limit but moderate array: limit=100, sum=5050, arr={1,2,3}, odd sum=2, diff=5048
    int arr7[] = {1,2,3};
    assert(sumOddEvenDifference(100, arr7, 3) == 5048);
    
    // Array with many elements, all odd indices sum to 0 if all values are 0
    int arr8[] = {0,0,0,0,0,0};
    assert(sumOddEvenDifference(4, arr8, 6) == 10);
    
    // Limit negative, returns 0
    int arr9[] = {1};
    assert(sumOddEvenDifference(-1, arr9, 1) == 0);
}
