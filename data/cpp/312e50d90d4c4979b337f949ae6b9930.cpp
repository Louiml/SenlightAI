Write a C++ function named `countDistinctElements` that takes a pointer to a constant integer array and its size as parameters, and returns the number of distinct elements present in the array. The function must handle arrays of any length, including empty arrays (size 0, returning 0), arrays with all identical elements, and arrays with negative or large positive integers. The function should not modify the input array and must be declared with appropriate `const` correctness. The solution should use an efficient approach based on a hash-based set to achieve linear average time complexity.

The core idea is to insert every element from the input array into an `std::unordered_set<int>`. Since a set automatically discards duplicate values, the size of the set after inserting all elements directly equals the number of distinct integers in the array. This approach naturally handles edge cases: an empty array results in an empty set and returns 0; an array with all identical elements produces a set of size 1; negative numbers and large values are stored identically to positive ones. The algorithm iterates through the array exactly once, so its time complexity is O(n) on average, with worst-case O(n²) only in rare hash collision scenarios. The auxiliary space usage is O(k), where k is the number of distinct elements stored in the set, which is at most n.

#include <unordered_set>

// Return the count of distinct integers in the given array.
// The input array is not modified.
int countDistinctElements(const int array[], int size) {
    std::unordered_set<int> distinct;
    for (int i = 0; i < size; ++i) {
        distinct.insert(array[i]);
    }
    return static_cast<int>(distinct.size());
}

#include <cassert>

int main() {
    // Example from the original snippet: {9,8,1,1,9,1,5,3,8,3} has {1,3,5,8,9} → 5 distinct.
    int a1[] = {9,8,1,1,9,1,5,3,8,3};
    assert(countDistinctElements(a1, 10) == 5);

    // Empty array (size 0) returns 0.
    int a2[] = {};
    assert(countDistinctElements(a2, 0) == 0);

    // All identical elements → 1 distinct.
    int a3[] = {7,7,7,7};
    assert(countDistinctElements(a3, 4) == 1);

    // Negative and positive values.
    int a4[] = {-3,0,5,-3,0,5};
    assert(countDistinctElements(a4, 6) == 3);

    // Already unique elements.
    int a5[] = {1,2,3,4,5};
    assert(countDistinctElements(a5, 5) == 5);

    // Single element.
    int a6[] = {42};
    assert(countDistinctElements(a6, 1) == 1);

    // Large values (beyond int range? Here within range, but test big numbers).
    int a7[] = {1000000, -1000000, 1000000};
    assert(countDistinctElements(a7, 3) == 2);

    // Array with many duplicates.
    int a8[] = {1,1,1,2,2,3};
    assert(countDistinctElements(a8, 6) == 3);

    // Const-correctness: passing a const array works.
    const int a9[] = {5,5,6,7,6};
    assert(countDistinctElements(a9, 5) == 3);

    return 0;
}
