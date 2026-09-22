// Given an integer `n` followed by a permutation of the numbers from 1 to `n`, write a C++ function `countMisplaced` that returns the number of positions `i` (1-indexed) where the value at position `i` is not equal to `i`. For each test case, input consists of an integer `n`, then `n` integers (the permutation). The function should receive the permutation as a `std::vector<int>` (with values 1..n) and return the count of misplaced elements. The result must be computed in linear time and constant extra space.

#include <cassert>
#include <vector>

// assume countMisplaced is defined above

int main() {
    // Basic already sorted permutation
    assert(countMisplaced({1, 2, 3, 4}) == 0);
    // Single element
    assert(countMisplaced({1}) == 0);
    // Reverse of size 3
    assert(countMisplaced({3, 2, 1}) == 2); // positions 1 and 3 are wrong
    // All elements shifted right
    assert(countMisplaced({4, 1, 2, 3}) == 4); // none in correct position
    // Only one element misplaced
    assert(countMisplaced({1, 3, 2, 4}) == 2); // positions 2 and 3 are wrong
    // Larger permutation
    assert(countMisplaced({5, 2, 3, 4, 1}) == 2); // positions 1 and 5
    return 0;
}

#include <vector>

// Count positions where the value is not equal to its 1-indexed position.
int countMisplaced(const std::vector<int>& permutation) {
    int misplaced = 0;
    for (std::size_t i = 0; i < permutation.size(); ++i) {
        if (permutation[i] != static_cast<int>(i) + 1) {
            ++misplaced;
        }
    }
    return misplaced;
}

// The problem is straightforward: we compare each element of the vector with its expected index (which is the loop index + 1, since indices are 0-based). If the element is not equal to `index + 1`, we increment a counter. Since the input is guaranteed to be a permutation, there are no duplicates or missing numbers, so the only source of "misplacement" is that some values are not at their natural positions. Edge cases: `n = 1` (the single element must be 1, so count is 0 if it is 1; otherwise, the input is invalid but we still compute). For any valid permutation, the worst case is when every element is misplaced (e.g., a right rotation), giving a maximum count of `n`. Time complexity: O(n) because we scan the vector once. Space complexity: O(1) auxiliary (excluding the input vector storage).
