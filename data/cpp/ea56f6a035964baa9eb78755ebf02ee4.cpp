Write a standalone C++ function named `fordJohnsonSort` that takes a `std::vector<unsigned int>` (or any sequence of positive integers) and returns a new sorted `std::vector<unsigned int>` using the merge-insert sort algorithm (Ford-Johnson). The function must handle at least 3000 distinct positive integers, preserve all input values (including duplicates), and must not modify the input vector. If the input contains any value that is not a positive integer (i.e., zero or negative), or if the vector is empty, the function should throw a `std::invalid_argument` exception. The solution must be implemented without using the standard library’s sorting algorithms (e.g., `std::sort`). The algorithm should follow the classic Ford-Johnson approach: pair elements, sort pairs, recursively sort the larger elements, then insert the smaller elements in a specific Jacobsthal-based order using binary search.

// The Ford-Johnson merge-insert sort (also called the "binary insertion sort with pairing") is an efficient comparison sort that minimizes worst-case comparisons. The algorithm proceeds as follows:
// 1. **Validation**: Check each value is greater than zero; if not, throw `std::invalid_argument`. If the vector is empty, throw as well (or handle separately, but for simplicity we require non-empty positive sequence).
// 2. **Base cases**: If size ≤ 1, return a copy. If size == 2, return a sorted copy.
// 3. **Pairing**: Partition the elements into pairs `(a, b)` where a <= b (or we store them as a pair; we will sort each pair so that the smaller is first). If there is an odd leftover element, set it aside as the "main chain" starter (the smallest element in the final sorted sequence).
// 4. **Recursion**: Take the larger elements from each pair (the "big" elements) and recursively sort them using the same function. This yields a sorted list of the larger elements.
// 5. **Insertion order**: We need to insert the corresponding smaller elements (the "small" elements of each pair) into the sorted list of big elements. To minimize comparisons, we use the Jacobsthal sequence (1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...) to determine which small element to insert next. More precisely, we process the small elements in an order given by the Jacobsthal numbers: for each k, we take the small elements from pairs at indices in descending order from the k-th Jacobsthal number down to the previous Jacobsthal number + 1. This ensures that the binary search window is neither too small nor too large.
// 6. **Binary insertion**: For each small element to insert, we binary search its position in the already-sorted big list and insert it there.
// 7. **Final assembly**: If there was an odd leftover, we insert that smallest element at the beginning (it is smaller than all big elements, so it goes first).
//
// **Edge cases**: 
// - Empty input → throw.
// - Input with zero or negative → throw.
// - Duplicates are allowed and should be preserved.
// - Odd number of elements → handle the leftover separately.
//
// **Time complexity**: The Ford-Johnson algorithm achieves an upper bound of `O(n log n)` comparisons, but with a lower constant than typical merge sort. In practice, the number of comparisons is asymptotically optimal for comparison-based sorts. The binary insertions take `O(log n)` each, and the total complexity is `O(n log n)`. Space complexity is `O(n)` due to recursive calls and temporary vectors.
//
// **Implementation details**: 
// - We'll use a helper function that takes a vector and returns a sorted vector.
// - For binary insertion, we'll implement a custom binary search to find the insertion position.
// - The Jacobsthal sequence can be generated on the fly.

#include <vector>
#include <stdexcept>
#include <algorithm>

// Helper: binary search insertion position for a value in a sorted vector
static size_t binarySearchInsertPos(const std::vector<unsigned int>& sorted, unsigned int value) {
    size_t left = 0, right = sorted.size();
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] <= value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

// Recursive Ford-Johnson merge-insert sort
std::vector<unsigned int> fordJohnsonSort(const std::vector<unsigned int>& input) {
    // Validate input
    if (input.empty()) {
        throw std::invalid_argument("Input vector cannot be empty");
    }
    for (unsigned int v : input) {
        if (v == 0) {
            throw std::invalid_argument("Values must be positive integers (zero not allowed)");
        }
    }

    // Base cases
    if (input.size() == 1) {
        return input;
    }
    if (input.size() == 2) {
        if (input[0] <= input[1]) {
            return input;
        } else {
            return {input[1], input[0]};
        }
    }

    // Pair elements: (small, big) where small <= big
    std::vector<std::pair<unsigned int, unsigned int>> pairs;
    const size_t n = input.size();
    size_t i = 0;
    for (; i + 1 < n; i += 2) {
        unsigned int a = input[i];
        unsigned int b = input[i + 1];
        if (a > b) std::swap(a, b);
        pairs.emplace_back(a, b);
    }
    // Odd leftover
    bool hasLeftover = false;
    unsigned int leftover = 0;
    if (i < n) {
        hasLeftover = true;
        leftover = input[i];
    }

    // Recursively sort the "big" elements
    std::vector<unsigned int> bigs;
    bigs.reserve(pairs.size());
    for (const auto& p : pairs) {
        bigs.push_back(p.second);
    }
    std::vector<unsigned int> sortedBigs = fordJohnsonSort(bigs);

    // Now insert the "small" elements in Jacobsthal order
    std::vector<unsigned int> result = sortedBigs;

    // Generate Jacobsthal numbers: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
    std::vector<size_t> jacob;
    jacob.push_back(0);
    if (pairs.size() >= 1) jacob.push_back(1);
    while (jacob.back() < pairs.size()) {
        size_t next = jacob[jacob.size()-1] + 2 * jacob[jacob.size()-2];
        jacob.push_back(next);
    }

    // Process small elements in reversed Jacobsthal order
    std::vector<bool> inserted(pairs.size(), false);
    for (size_t k = 1; k < jacob.size(); ++k) {
        size_t current = jacob[k];
        size_t prev = jacob[k-1];
        // We want indices from current-1 (in 0-based) down to prev (inclusive) if within range
        // But note: jacob[k] can be larger than pairs.size().
        size_t upper = std::min(current, pairs.size());
        if (upper == 0) continue;
        for (size_t idx = upper; idx > prev; --idx) {
            size_t realIdx = idx - 1; // 0-based index
            if (realIdx < pairs.size() && !inserted[realIdx]) {
                // Insert the small element from this pair
                unsigned int smallVal = pairs[realIdx].first;
                size_t pos = binarySearchInsertPos(result, smallVal);
                result.insert(result.begin() + pos, smallVal);
                inserted[realIdx] = true;
            }
        }
    }

    // Insert any remaining small elements that weren't covered (shouldn't happen, but safe)
    for (size_t idx = 0; idx < pairs.size(); ++idx) {
        if (!inserted[idx]) {
            unsigned int smallVal = pairs[idx].first;
            size_t pos = binarySearchInsertPos(result, smallVal);
            result.insert(result.begin() + pos, smallVal);
        }
    }

    // If there's a leftover, it's the smallest overall, insert at beginning
    if (hasLeftover) {
        result.insert(result.begin(), leftover);
    }

    return result;
}

#include <cassert>
#include <vector>
#include <stdexcept>
#include <algorithm>

// Include the function declaration or the whole definition here
// (Assuming the solution code is placed above this test)

int main() {
    // Basic sort
    std::vector<unsigned int> v1 = {5, 2, 8, 1, 9};
    std::vector<unsigned int> s1 = fordJohnsonSort(v1);
    std::vector<unsigned int> e1 = {1, 2, 5, 8, 9};
    assert(s1 == e1);

    // Already sorted
    std::vector<unsigned int> v2 = {1, 2, 3, 4};
    assert(fordJohnsonSort(v2) == v2);

    // Reverse sorted
    std::vector<unsigned int> v3 = {7, 6, 5, 4};
    std::vector<unsigned int> e3 = {4, 5, 6, 7};
    assert(fordJohnsonSort(v3) == e3);

    // Duplicates
    std::vector<unsigned int> v4 = {3, 1, 2, 1, 3};
    std::vector<unsigned int> e4 = {1, 1, 2, 3, 3};
    assert(fordJohnsonSort(v4) == e4);

    // Odd number of elements
    std::vector<unsigned int> v5 = {10, 20, 30};
    std::vector<unsigned int> e5 = {10, 20, 30};
    assert(fordJohnsonSort(v5) == e5);

    // Larger input (with duplicates) — compare with std::sort to verify correctness
    std::vector<unsigned int> v6 = {42, 17, 99, 23, 42, 5, 1, 88, 33, 50, 12};
    std::vector<unsigned int> e6 = v6;
    std::sort(e6.begin(), e6.end());
    assert(fordJohnsonSort(v6) == e6);

    // 3000 distinct values
    std::vector<unsigned int> v7;
    for (unsigned int i = 3000; i > 0; --i) v7.push_back(i);
    std::vector<unsigned int> e7 = v7;
    std::sort(e7.begin(), e7.end());
    assert(fordJohnsonSort(v7) == e7);

    // Single element
    std::vector<unsigned int> v8 = {123};
    assert(fordJohnsonSort(v8) == v8);

    // Exception on empty
    std::vector<unsigned int> v9;
    bool threw = false;
    try { fordJohnsonSort(v9); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Exception on zero
    std::vector<unsigned int> v10 = {5, 0, 7};
    threw = false;
    try { fordJohnsonSort(v10); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    return 0;
}
