Write a C++ function `vector<int> stableSortWithLambda(vector<int> input)` that takes a vector of integers and returns a new vector with the elements sorted in ascending order, but using a lambda expression as the comparison predicate passed to `std::stable_sort`. The function must not modify the original input vector. If the input vector is empty, return an empty vector. The sorting must be stable, meaning that equal elements retain their original relative order. For example, given `{5, 1, 5, 2, 3}`, the output must be `{1, 2, 3, 5, 5}` where the two 5's appear in the same order as in the input. Your solution must demonstrate the concept that a lambda expression creates a function object (closure) that can be passed to standard algorithms, similar to the code snippet provided.

#include <cassert>
#include <vector>

int main() {
    // Basic sorting
    std::vector<int> v1 = {5, 2, 8, 1, 9};
    assert((stableSortWithLambda(v1) == std::vector<int>{1, 2, 5, 8, 9}));

    // Stability: two 3's should appear in original order (first 3, second 3)
    std::vector<int> v2 = {3, 1, 3, 2, 3};
    assert((stableSortWithLambda(v2) == std::vector<int>{1, 2, 3, 3, 3}));

    // Empty input
    std::vector<int> empty;
    assert(stableSortWithLambda(empty).empty());

    // Single element
    std::vector<int> single = {42};
    assert((stableSortWithLambda(single) == std::vector<int>{42}));

    // All equal elements
    std::vector<int> allEqual = {7, 7, 7};
    assert((stableSortWithLambda(allEqual) == std::vector<int>{7, 7, 7}));

    // Negative numbers and duplicates
    std::vector<int> neg = {-3, 5, -1, 0, -3, 5};
    assert((stableSortWithLambda(neg) == std::vector<int>{-3, -3, -1, 0, 5, 5}));

    // Original vector not modified (check by passing by value and verifying caller's copy)
    std::vector<int> original = {3, 1, 2};
    std::vector<int> result = stableSortWithLambda(original);
    assert(original == std::vector<int>({3, 1, 2})); // original untouched
    assert(result == std::vector<int>({1, 2, 3}));
}

#include <vector>
#include <algorithm>

// Return a new vector with the elements of 'input' sorted ascending using a lambda.
std::vector<int> stableSortWithLambda(std::vector<int> input) {
    // The parameter 'input' is a copy, so original remains unmodified.
    // Apply stable sort with a lambda as the comparison predicate.
    std::stable_sort(input.begin(), input.end(),
                     [](int a, int b) { return a < b; });
    return input;
}

// The solution copies the input vector to a local variable so the original remains unmodified (pass-by-value already gives a copy, but we can also declare the parameter as `const` and then copy explicitly to make intent clear). We then call `std::stable_sort` with a lambda expression `[](int a, int b) { return a < b; }` as the comparator. This lambda is compiled into a function object with an `operator()` that performs the comparison. For an empty or single-element vector, `stable_sort` correctly does nothing. Time complexity is \(O(n \log n)\) for the sort, and space complexity is \(O(1)\) auxiliary (excluding the copy of the vector, which is \(O(n)\) if we copy explicitly; however, since the parameter is passed by value, the caller already pays copying cost). Edge cases include empty input (returns empty), all equal elements (stable sort keeps them all, order preserved), and negative numbers (comparison works fine). The use of `stable_sort` is important because a plain `sort` might reorder equal elements, though for integers with a strict weak ordering it's not guaranteed to preserve order; stable_sort guarantees stability.
