// Write a C++ function that takes a vector of sorted integer vectors (each sorted in non-decreasing order) and returns a vector containing all integers that appear in every input vector, in sorted order. The function must run in O(NK) time where N is the length of each vector and K is the number of vectors, and must handle the case of no common elements by returning an empty vector. You may assume all input vectors are non-empty and sorted.

The core idea is to iteratively intersect the first vector with each subsequent vector, maintaining a running intersection result. Since all vectors are sorted, we can merge two sorted vectors in linear time using two pointers: compare elements, advance the pointer of the smaller one, and when equal, append to the result and advance both. This intersection of two vectors of lengths up to N takes O(N) time. We start with the first vector as the initial intersection, then for each of the remaining K-1 vectors, intersect the current result with the next vector. The total time is O(N) per merge and we do K-1 merges, so total O(NK). Space is O(N) for the running intersection result plus O(N) for temporary storage during each merge; we can reuse two buffers to avoid repeated allocations. Edge cases: if any intersection becomes empty, we can stop early and return an empty result. Duplicate common elements appear only once in the result because the intersection of two sorted arrays (which may each contain duplicates) produces each common value just once.

#include <vector>

// Return a sorted vector of integers that appear in every input vector.
// Input: sortedVectors - a vector of sorted (non-decreasing) vectors of integers.
// Time complexity: O(NK), space: O(N) auxiliary.
std::vector<int> findCommonElements(const std::vector<std::vector<int>>& sortedVectors) {
    if (sortedVectors.empty()) return {};

    // Start with the first vector as the initial intersection.
    std::vector<int> current = sortedVectors[0];

    // Temporary buffer for the next intersection result.
    std::vector<int> next;

    for (size_t idx = 1; idx < sortedVectors.size() && !current.empty(); ++idx) {
        const std::vector<int>& other = sortedVectors[idx];
        next.clear();
        size_t i = 0, j = 0;
        while (i < current.size() && j < other.size()) {
            if (current[i] < other[j]) {
                ++i;
            } else if (other[j] < current[i]) {
                ++j;
            } else {
                // Equal values: add to intersection once, advance both.
                next.push_back(current[i]);
                ++i;
                ++j;
            }
        }
        // Move the new intersection into current (swap to avoid copying).
        current.swap(next);
    }

    return current;
}

#include <cassert>
#include <vector>

// Include the solution function definition here (or link it).

int main() {
    // Basic case with common elements.
    std::vector<std::vector<int>> arrays1 = {{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}};
    assert(findCommonElements(arrays1) == std::vector<int>({3, 4}));

    // No common elements.
    std::vector<std::vector<int>> arrays2 = {{1, 2}, {3, 4}, {5, 6}};
    assert(findCommonElements(arrays2).empty());

    // Single vector: returns itself.
    std::vector<std::vector<int>> arrays3 = {{10, 20, 30}};
    assert(findCommonElements(arrays3) == std::vector<int>({10, 20, 30}));

    // Duplicates within each vector: each common value appears once.
    std::vector<std::vector<int>> arrays4 = {{1, 1, 2, 2}, {1, 2, 2, 3}, {1, 1, 2, 3}};
    assert(findCommonElements(arrays4) == std::vector<int>({1, 2}));

    // All identical values.
    std::vector<std::vector<int>> arrays5 = {{7, 7, 7}, {7, 7}, {7, 7, 7, 7}};
    assert(findCommonElements(arrays5) == std::vector<int>({7}));

    // Empty input (edge case).
    std::vector<std::vector<int>> arrays6;
    assert(findCommonElements(arrays6).empty());

    return 0;
}
