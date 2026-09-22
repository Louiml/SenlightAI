// Write a C++ function named `mergeAndReverse` that takes two integer arrays (via pointers) and their respective sizes as parameters, and returns a `std::vector<int>` containing the concatenation of the two arrays (first all elements of array A, then all elements of array B) in reverse order. For example, if A = {10, 20, 30} and B = {1, 2}, the result should be {2, 1, 30, 20, 10}. The function must handle empty arrays (size 0) gracefully and produce the correct result when one or both inputs are empty. Do not modify the input arrays. The function must be `const`-correct.

// The solution builds a new vector by first appending all elements from the first array, then all elements from the second array. This can be done using `std::vector::insert` or a simple loop. After the merged vector is built, reverse it using `std::reverse` from `<algorithm>`. Edge cases: if both sizes are zero, the result is an empty vector; if only one array is empty, the result is the reverse of the other array. Time complexity is O(n + m) where n and m are the sizes of the two arrays, because we make one pass to copy elements and another pass to reverse (the reverse itself is O(n+m)). Space complexity is O(n+m) for the returned vector (excluding input storage). The function takes pointers and sizes, so it must guard against null pointers when size is zero (though if size is zero, the pointer may be null or valid but unused; we can simply loop from 0 to size-1, which does nothing for size 0, so no dereference occurs).

#include <vector>
#include <algorithm>  // for std::reverse

// Merge two arrays into a vector, then reverse the order of all elements.
std::vector<int> mergeAndReverse(const int* arrA, int sizeA, const int* arrB, int sizeB) {
    std::vector<int> result;
    result.reserve(sizeA + sizeB);

    // Copy first array
    for (int i = 0; i < sizeA; ++i) {
        result.push_back(arrA[i]);
    }
    // Copy second array
    for (int i = 0; i < sizeB; ++i) {
        result.push_back(arrB[i]);
    }

    // Reverse the entire merged vector
    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>

// Assume the function declaration is available from the solution code
std::vector<int> mergeAndReverse(const int* arrA, int sizeA, const int* arrB, int sizeB);

int main() {
    // Test 1: Normal case
    int a1[] = {10, 20, 30, 40, 50};
    int b1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> r1 = mergeAndReverse(a1, 5, b1, 8);
    std::vector<int> expected1 = {8,7,6,5,4,3,2,1,50,40,30,20,10};
    assert(r1 == expected1);

    // Test 2: One array empty
    int a2[] = {5, 6};
    int b2[] = {};
    std::vector<int> r2 = mergeAndReverse(a2, 2, b2, 0);
    std::vector<int> expected2 = {6, 5};
    assert(r2 == expected2);

    // Test 3: Both empty
    int a3[] = {};
    int b3[] = {};
    std::vector<int> r3 = mergeAndReverse(a3, 0, b3, 0);
    assert(r3.empty());

    // Test 4: Single element each
    int a4[] = {7};
    int b4[] = {3};
    std::vector<int> r4 = mergeAndReverse(a4, 1, b4, 1);
    std::vector<int> expected4 = {3, 7};
    assert(r4 == expected4);

    // Test 5: Negative values and unequal sizes
    int a5[] = {-1, -2, -3};
    int b5[] = {100, 200};
    std::vector<int> r5 = mergeAndReverse(a5, 3, b5, 2);
    std::vector<int> expected5 = {200, 100, -3, -2, -1};
    assert(r5 == expected5);

    // Test 6: Larger second array than first
    int a6[] = {9};
    int b6[] = {1, 2, 3, 4};
    std::vector<int> r6 = mergeAndReverse(a6, 1, b6, 4);
    std::vector<int> expected6 = {4, 3, 2, 1, 9};
    assert(r6 == expected6);

    // Test 7: Identical values
    int a7[] = {0, 0};
    int b7[] = {0, 0, 0};
    std::vector<int> r7 = mergeAndReverse(a7, 2, b7, 3);
    std::vector<int> expected7(5, 0);
    assert(r7 == expected7);

    return 0;
}
