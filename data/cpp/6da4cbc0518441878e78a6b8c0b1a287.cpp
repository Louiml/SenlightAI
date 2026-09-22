/*
Write a C++ function named `isPermutation` that takes two integer arrays `a` and `b` along with their sizes `na` and `nb`, and returns a `bool` indicating whether array `b` is a permutation (an arrangement) of array `a`. The function must handle the case where the arrays have different lengths (immediately returning `false`) and must correctly identify permutations even when elements are duplicated. The solution should not modify the original arrays; instead, it should work on copies. The function should be self-contained, using only standard library facilities, and must be efficient for arrays up to 100 elements.
*/

#include <vector>
#include <algorithm>

// Returns true if array b is a permutation of array a (same elements, possibly in different order).
bool isPermutation(const int* a, int na, const int* b, int nb) {
    if (na != nb) {
        return false;
    }
    
    // Create copies to sort without modifying the original arrays.
    std::vector<int> sortedA(a, a + na);
    std::vector<int> sortedB(b, b + nb);
    
    std::sort(sortedA.begin(), sortedA.end());
    std::sort(sortedB.begin(), sortedB.end());
    
    return sortedA == sortedB;
}

#include <cassert>

int main() {
    // Test 1: Basic permutation
    int a1[] = {1, 2, 3, 4};
    int b1[] = {4, 3, 2, 1};
    assert(isPermutation(a1, 4, b1, 4) == true);
    
    // Test 2: Different sizes -> not a permutation
    int a2[] = {1, 2, 3};
    int b2[] = {1, 2, 3, 4};
    assert(isPermutation(a2, 3, b2, 4) == false);
    
    // Test 3: Same elements but different counts (duplicates mismatch)
    int a3[] = {1, 1, 2, 2};
    int b3[] = {1, 2, 2, 2};
    assert(isPermutation(a3, 4, b3, 4) == false);
    
    // Test 4: Same elements with duplicates, different order
    int a4[] = {5, 5, 7, 7, 9};
    int b4[] = {7, 5, 9, 7, 5};
    assert(isPermutation(a4, 5, b4, 5) == true);
    
    // Test 5: Empty arrays are permutations
    int a5[] = {};
    int b5[] = {};
    assert(isPermutation(a5, 0, b5, 0) == true);
    
    // Test 6: One empty, one non-empty
    int a6[] = {1};
    assert(isPermutation(a6, 1, b5, 0) == false);
    
    // Test 7: Negative numbers
    int a7[] = {-3, 0, 2, -3};
    int b7[] = {2, -3, -3, 0};
    assert(isPermutation(a7, 4, b7, 4) == true);
    
    // Test 8: Arrays that are already sorted but not identical
    int a8[] = {1, 2, 3};
    int b8[] = {1, 2, 4};
    assert(isPermutation(a8, 3, b8, 3) == false);
    
    // Test 9: Arrays that are identical (trivial permutation)
    int a9[] = {42, 7, 13};
    int b9[] = {42, 7, 13};
    assert(isPermutation(a9, 3, b9, 3) == true);
    
    // Test 10: Large range with zeros
    int a10[] = {0, 100, -100, 50};
    int b10[] = {-100, 50, 100, 0};
    assert(isPermutation(a10, 4, b10, 4) == true);
    
    return 0;
}

// The core idea is to compare the multisets of elements in both arrays. The simplest robust approach is to sort copies of both arrays and then compare element by element. If the sorted versions are identical, then the original arrays contain exactly the same elements with the same multiplicities, so `b` is a permutation of `a`. Sorting copies ensures the original arrays remain unchanged, satisfying the non-modification requirement.
//
// Edge cases to consider:
// - Different sizes: immediately return `false` without any further computation.
// - Empty arrays: two empty arrays are permutations of each other (sorting both yields empty vectors which are equal). If one is empty and the other is not, sizes differ, so return `false`.
// - Duplicates: sorting and comparing handles duplicates correctly because identical values are placed adjacently and counted consistently.
//
// Time complexity: Sorting each array takes \(O(n_a \log n_a + n_b \log n_b)\). If sizes are equal \(n\), this is \(O(n \log n)\). Space complexity: The copies of the arrays use \(O(n_a + n_b)\) auxiliary space. If we were required to avoid extra space, we could use a frequency map, but that would still require \(O(n)\) space and add overhead; sorting is simpler and standard.
//
// The implementation uses `std::vector<int>` as a convenient dynamic copy, but we could also use `std::array` or pass a second set of arrays. However, for clarity and to handle arbitrary sizes up to 100, a vector is fine. The function signature should take `const int* a, int na, const int* b, int nb` to enforce const-correctness.
