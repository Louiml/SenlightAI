Write a C++ function `mergeThreeVectors` that takes three constant references to `std::vector<int>` and returns a new `std::vector<int>` containing all elements from the three input vectors concatenated in the order: first vector, then second vector, then third vector. The function must not modify the input vectors, must reserve the appropriate capacity for efficiency, and must handle cases where any of the input vectors are empty. The returned vector should preserve the original element order within each input vector. Provide a clear and efficient implementation using STL algorithms.

#include <cassert>
#include <vector>

// Declare the function (it is defined separately, but for this test we include it inline for simplicity)
std::vector<int> mergeThreeVectors(const std::vector<int>& v1,
                                   const std::vector<int>& v2,
                                   const std::vector<int>& v3) {
    std::vector<int> merged;
    merged.reserve(v1.size() + v2.size() + v3.size());
    merged.insert(merged.end(), v1.begin(), v1.end());
    merged.insert(merged.end(), v2.begin(), v2.end());
    merged.insert(merged.end(), v3.begin(), v3.end());
    return merged;
}

int main() {
    // Basic concatenation
    std::vector<int> a = {1, 2};
    std::vector<int> b = {3, 4, 5};
    std::vector<int> c = {6};
    std::vector<int> result = mergeThreeVectors(a, b, c);
    assert(result == std::vector<int>({1, 2, 3, 4, 5, 6}));

    // Empty first vector
    std::vector<int> empty;
    a = {10, 20};
    b = {30};
    c = {40, 50};
    result = mergeThreeVectors(empty, a, b);
    // Note: c is not used, but we call with three arguments: empty, a, b
    assert(mergeThreeVectors(empty, a, b) == std::vector<int>({10, 20, 30}));

    // Empty middle vector
    a = {1};
    b = empty;
    c = {2, 3};
    assert(mergeThreeVectors(a, b, c) == std::vector<int>({1, 2, 3}));

    // Empty last vector
    a = {7, 8};
    b = {9};
    c = empty;
    assert(mergeThreeVectors(a, b, c) == std::vector<int>({7, 8, 9}));

    // All empty
    assert(mergeThreeVectors(empty, empty, empty) == std::vector<int>());

    // Single element per vector
    a = {5};
    b = {6};
    c = {7};
    assert(mergeThreeVectors(a, b, c) == std::vector<int>({5, 6, 7}));

    // Large sequence with duplicates
    a = {1, 1, 1};
    b = {2, 2};
    c = {3};
    assert(mergeThreeVectors(a, b, c) == std::vector<int>({1, 1, 1, 2, 2, 3}));

    // Negative and zero values
    a = {-1, 0, -5};
    b = {10, -2};
    c = {0};
    assert(mergeThreeVectors(a, b, c) == std::vector<int>({-1, 0, -5, 10, -2, 0}));

    // Check no modification of inputs
    a = {1, 2};
    b = {3};
    c = {4};
    mergeThreeVectors(a, b, c);
    assert(a == std::vector<int>({1, 2}));
    assert(b == std::vector<int>({3}));
    assert(c == std::vector<int>({4}));

    return 0;
}

#include <vector>

// Merge three integer vectors into a single vector, preserving order.
std::vector<int> mergeThreeVectors(const std::vector<int>& v1,
                                   const std::vector<int>& v2,
                                   const std::vector<int>& v3) {
    std::vector<int> merged;
    merged.reserve(v1.size() + v2.size() + v3.size());

    merged.insert(merged.end(), v1.begin(), v1.end());
    merged.insert(merged.end(), v2.begin(), v2.end());
    merged.insert(merged.end(), v3.begin(), v3.end());

    return merged;
}

// The solution concatenates three vectors by creating an empty result vector, reserving memory for the total size of all three inputs to avoid repeated reallocations (which would degrade performance), and then inserting each input vector's range into the result using `insert`. The `insert` overload that takes two iterators is used, which copies the elements in order. Edge cases include one or more input vectors being empty—those simply contribute nothing, and the function handles them naturally because `reserve` with zero additional size is fine, and `insert` with empty ranges does nothing. The time complexity is O(n1 + n2 + n3) where n1, n2, n3 are the sizes of the input vectors, because each element is copied exactly once. The space complexity is O(n1 + n2 + n3) for the returned vector, since it stores all elements; no additional significant memory is used apart from the input vectors themselves (which are accessed via const references, not copied).
