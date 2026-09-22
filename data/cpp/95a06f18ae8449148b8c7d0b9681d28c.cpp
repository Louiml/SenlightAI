// Write a C++ function that takes two integer containers of the same size and type — specifically `std::vector<int>` — and returns a `std::vector<int>` containing the merged elements of both input vectors in non-decreasing order. The function must be named `mergeSortedVectors` and must handle duplicate values naturally. The inputs are assumed to be already sorted in non-decreasing order. The function should not modify the input vectors and must work for empty vectors as well as vectors of arbitrary size. The returned vector should contain exactly `size1 + size2` elements.

The task is a classic merge of two sorted sequences, analogous to the merge step in mergesort. The main algorithm uses two indices (one for each input vector) and repeatedly selects the smaller of the two current elements, advancing the corresponding index. If one vector is exhausted, the remaining elements of the other vector are appended directly. Edge cases include: both inputs empty (return empty vector), one empty and the other non‑empty (return a copy of the non‑empty vector), duplicate values across or within inputs (handled naturally by using `<=` or `<` comparisons appropriately to preserve stability — here, order does not matter since values are equal). Time complexity is \(O(n + m)\) where \(n\) and \(m\) are the sizes of the two input vectors, and auxiliary space is \(O(n + m)\) for the output vector. The implementation uses `const` references to avoid copying inputs and returns by value, relying on move semantics or copy elision for efficiency.

#include <vector>

// Merge two sorted vectors into a single sorted vector.
// The inputs are not modified; the result contains all elements in non-decreasing order.
std::vector<int> mergeSortedVectors(const std::vector<int>& left, const std::vector<int>& right) {
    std::vector<int> result;
    result.reserve(left.size() + right.size());

    std::size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i]);
            ++i;
        } else {
            result.push_back(right[j]);
            ++j;
        }
    }

    // Append any remaining elements from the non-exhausted vector.
    while (i < left.size()) {
        result.push_back(left[i]);
        ++i;
    }
    while (j < right.size()) {
        result.push_back(right[j]);
        ++j;
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include the header or paste it here.
// For brevity, the function definition is repeated here in a real solution file.

int main() {
    // Both empty
    std::vector<int> empty1, empty2;
    assert(mergeSortedVectors(empty1, empty2) == std::vector<int>());

    // One empty
    std::vector<int> a = {1, 3, 5};
    std::vector<int> empty;
    assert(mergeSortedVectors(a, empty) == a);
    assert(mergeSortedVectors(empty, a) == a);

    // Basic merge
    std::vector<int> b = {2, 4, 6};
    assert(mergeSortedVectors(a, b) == std::vector<int>({1, 2, 3, 4, 5, 6}));

    // Duplicates across inputs
    std::vector<int> c = {1, 2, 2, 3};
    std::vector<int> d = {2, 3, 4};
    assert(mergeSortedVectors(c, d) == std::vector<int>({1, 2, 2, 2, 3, 3, 4}));

    // Duplicates within one input
    std::vector<int> e = {1, 1, 1};
    std::vector<int> f = {1};
    assert(mergeSortedVectors(e, f) == std::vector<int>({1, 1, 1, 1}));

    // Different lengths, all elements from one side first
    std::vector<int> g = {0, 1, 2};
    std::vector<int> h = {3, 4};
    assert(mergeSortedVectors(g, h) == std::vector<int>({0, 1, 2, 3, 4}));

    // Single element each
    std::vector<int> i = {7};
    std::vector<int> j = {3};
    assert(mergeSortedVectors(i, j) == std::vector<int>({3, 7}));

    // Large input (simple sanity check with sorted vectors)
    std::vector<int> big1, big2;
    for (int k = 0; k < 1000; ++k) {
        big1.push_back(k * 2);
        big2.push_back(k * 2 + 1);
    }
    std::vector<int> mergedBig = mergeSortedVectors(big1, big2);
    assert(mergedBig.size() == 2000);
    for (std::size_t k = 0; k < mergedBig.size(); ++k) {
        assert(mergedBig[k] == static_cast<int>(k));
    }

    // Inputs are not modified
    std::vector<int> originalA = a;
    std::vector<int> originalB = b;
    mergeSortedVectors(a, b);
    assert(a == originalA);
    assert(b == originalB);
}
