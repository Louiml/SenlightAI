/*
Write a C++ function `std::vector<int> sortAndDeduplicate(int input[], int size)` that accepts an array of integers and its size, returns a new `std::vector<int>` containing the same integers sorted in ascending order with all duplicate values removed (each distinct value appearing exactly once). The function must not modify the input array, and the input array may contain negative numbers, zeros, and positive numbers, with no constraint on size or duplicate frequency. For example, given `input = {2,5,3,6,4,2,4,2,2,8}`, the function should return a vector containing `{2,3,4,5,6,8}`. The function must handle edge cases such as an empty array (size 0) and an array with a single element, returning an empty vector or a single‑element vector respectively. You may use any standard library containers/algorithms, but you cannot modify the input array or rely on a pre‑existing sorting function that automatically deduplicates (i.e., you must implement both the sort and the deduplication logic yourself, though you can use `std::sort` and `std::unique` on a copy if you explain that approach).
*/
#include <vector>
#include <algorithm>

// Return a sorted, deduplicated vector of the input array's elements.
// The original array is not modified.
std::vector<int> sortAndDeduplicate(const int input[], int size) {
    std::vector<int> result(input, input + size);  // copy input
    std::sort(result.begin(), result.end());        // ascending sort
    auto last = std::unique(result.begin(), result.end()); // remove consecutive duplicates
    result.erase(last, result.end());               // drop the moved duplicates
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Declaration for the function under test (usually in a header)
std::vector<int> sortAndDeduplicate(const int input[], int size);

int main() {
    // Test 1: Example from the problem
    int arr1[] = {2,5,3,6,4,2,4,2,2,8};
    assert(sortAndDeduplicate(arr1, 10) == std::vector<int>({2,3,4,5,6,8}));

    // Test 2: Empty array
    int arr2[] = {};
    assert(sortAndDeduplicate(arr2, 0) == std::vector<int>());

    // Test 3: Single element
    int arr3[] = {7};
    assert(sortAndDeduplicate(arr3, 1) == std::vector<int>({7}));

    // Test 4: All duplicates
    int arr4[] = {5,5,5,5};
    assert(sortAndDeduplicate(arr4, 4) == std::vector<int>({5}));

    // Test 5: Negative numbers and zeros
    int arr5[] = {-3, 0, -1, -3, 2, 0, 1};
    assert(sortAndDeduplicate(arr5, 7) == std::vector<int>({-3, -1, 0, 1, 2}));

    // Test 6: Already sorted with no duplicates
    int arr6[] = {1,2,3,4};
    assert(sortAndDeduplicate(arr6, 4) == std::vector<int>({1,2,3,4}));

    // Test 7: Original array must not be modified
    int arr7[] = {2,1,2,3};
    std::vector<int> before(arr7, arr7+4);
    sortAndDeduplicate(arr7, 4);
    assert(std::vector<int>(arr7, arr7+4) == before);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution copies the input array into a local `std::vector<int>` to preserve the original data, then sorts the copy in ascending order using the standard `sort` from `<algorithm>`. After sorting, a second pass with `std::unique` moves all duplicate elements to the end of the vector and returns an iterator to the new logical end; then `erase` removes the duplicated tail. This approach is simple and leverages standard library correctness. The time complexity is dominated by the sort: \(O(n \log n)\) for \(n\) elements. The deduplication pass is linear \(O(n)\). The space complexity is \(O(n)\) because we create a copy of the input array. Edge cases: empty input returns an empty vector; single‑element input returns a one‑element vector; negative numbers and zeros sort correctly; arrays with all identical elements reduce to one element. No mutation of the original array occurs because we operate on the copy. The function is declared `const`‑correct by taking `const int input[]` (or `const int*`) and returning by value.
