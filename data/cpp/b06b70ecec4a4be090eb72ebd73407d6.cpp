// Write a C++ function `sortAndPrint` that takes a vector of integers, sorts it in non-decreasing order, and returns a new vector containing the sorted integers. The input may contain duplicate values and negative numbers, and the vector may be empty. The function must preserve the original vector (do not modify it) and must use a stable sort to guarantee that equal elements retain their original relative order (though for plain integers this is not observable, it demonstrates good practice).
The simplest approach is to create a copy of the input vector, then call `std::stable_sort` on the copy. Using a copy ensures the original vector is not modified, satisfying the const correctness requirement. If the vector is empty, the copy is also empty and the sort simply does nothing; no special case is needed. Duplicates are handled naturally because stable sort maintains relative order among equal elements. For `n` elements, the time complexity of `std::stable_sort` is \(O(n \log n)\) in the worst case (it may use extra memory for merging, typically \(O(n)\) auxiliary space). This is acceptable for most practical scenarios. An alternative would be `std::sort`, which is generally faster but not stable; since the task emphasizes stability, `std::stable_sort` is the correct choice. Edge cases include: empty vector (returns empty), single element (returns that element), all identical values (returns the same vector), and negative numbers (sorted correctly by default comparator).
#include <vector>
#include <algorithm>

// Return a new vector containing the elements of input sorted in non-decreasing order.
// The original vector is left unmodified.
std::vector<int> sortAndPrint(const std::vector<int>& input) {
    std::vector<int> sorted = input;         // copy original
    std::stable_sort(sorted.begin(), sorted.end()); // stable sort on the copy
    return sorted;
}
#include <cassert>
#include <vector>

// The solution function is declared above (in the same translation unit).
int main() {
    // Empty vector
    std::vector<int> empty{};
    assert(sortAndPrint(empty).empty());

    // Single element
    std::vector<int> one{42};
    assert(sortAndPrint(one) == std::vector<int>{42});

    // Duplicates and negatives
    std::vector<int> dupNeg{3, -1, 3, -2, 0, -2};
    std::vector<int> expected1{-2, -2, -1, 0, 3, 3};
    assert(sortAndPrint(dupNeg) == expected1);

    // Already sorted
    std::vector<int> sortedInput{-5, 0, 7, 10};
    assert(sortAndPrint(sortedInput) == sortedInput);

    // Reverse sorted
    std::vector<int> reverseInput{9, 4, 1, -3};
    assert(sortAndPrint(reverseInput) == std::vector<int>{-3, 1, 4, 9});

    // Ensure original vectors are not modified
    std::vector<int> original{5, 2, 8, 1};
    std::vector<int> copyOriginal = original;
    sortAndPrint(original);
    assert(original == copyOriginal);

    // Large vector with many duplicates
    std::vector<int> large(1000, 7);
    large[0] = -1;
    large[999] = 100;
    std::vector<int> largeExpected(1000, 7);
    largeExpected[0] = -1;
    largeExpected[999] = 100;
    assert(sortAndPrint(large) == largeExpected);

    // All equal values
    std::vector<int> allEqual{5, 5, 5};
    assert(sortAndPrint(allEqual) == allEqual);
}
