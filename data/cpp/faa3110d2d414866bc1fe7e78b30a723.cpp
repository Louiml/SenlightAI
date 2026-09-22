/*
Given an even-length sequence of integers where each value appears exactly twice, write a C++ function `int minimumAdjacentSwaps(std::vector<int>& values)` that returns the minimum number of adjacent swaps needed to arrange the sequence so that all pairs of equal numbers are adjacent. The input vector is guaranteed to contain `2n` integers (for some positive integer `n`), with each integer appearing exactly twice. You may modify the input vector during the process. The function should return the total number of adjacent swaps performed.
*/

#include <vector>

// Given a vector where each integer appears exactly twice,
// return the minimum number of adjacent swaps needed to make all pairs adjacent.
int minimumAdjacentSwaps(std::vector<int>& values) {
    int swapCount = 0;
    int size = static_cast<int>(values.size());

    for (int i = 0; i < size; i += 2) {
        int j = i + 1;
        // Find the matching duplicate for the element at i.
        while (j < size && values[j] != values[i]) {
            ++j;
        }
        // If no duplicate found (should not happen per constraints), stop.
        if (j == size) {
            break;
        }
        // Move the duplicate leftward to position i+1 using adjacent swaps.
        while (j > i + 1) {
            int temp = values[j];
            values[j] = values[j - 1];
            values[j - 1] = temp;
            ++swapCount;
            --j;
        }
    }
    return swapCount;
}

#include <vector>
#include <cassert>

// Declare the function (since its definition is separate).
int minimumAdjacentSwaps(std::vector<int>& values);

int main() {
    // Basic case: two identical adjacent pairs
    std::vector<int> v1 = {1, 1, 2, 2};
    assert(minimumAdjacentSwaps(v1) == 0);

    // Classic shuffling: all pairs interleaved
    std::vector<int> v2 = {1, 2, 1, 2};
    assert(minimumAdjacentSwaps(v2) == 1);

    // Larger example: 1 2 3 1 2 3 → swap 3 times
    std::vector<int> v3 = {1, 2, 3, 1, 2, 3};
    assert(minimumAdjacentSwaps(v3) == 3);

    // Reverse order: 1 2 3 3 2 1 → cost 3
    std::vector<int> v4 = {1, 2, 3, 3, 2, 1};
    assert(minimumAdjacentSwaps(v4) == 3);

    // Already all pairs adjacent but in arbitrary order
    std::vector<int> v5 = {9, 9, 4, 4, 7, 7};
    assert(minimumAdjacentSwaps(v5) == 0);

    // Simple single pair
    std::vector<int> v6 = {5, 5};
    assert(minimumAdjacentSwaps(v6) == 0);

    // Two pairs with one swap needed
    std::vector<int> v7 = {2, 3, 2, 3};
    assert(minimumAdjacentSwaps(v7) == 1);

    // Three pairs, worst case: 1 2 3 1 2 3 already done, alternate: 1 2 3 2 3 1
    std::vector<int> v8 = {1, 2, 3, 2, 3, 1};
    assert(minimumAdjacentSwaps(v8) == 2); // (swap 3 left once, swap 1 left once)

    // Larger interleaved sequence: 1 2 3 4 1 2 3 4
    std::vector<int> v9 = {1, 2, 3, 4, 1, 2, 3, 4};
    assert(minimumAdjacentSwaps(v9) == 6); // (1: 3 swaps, 2: 2 swaps, 3: 1 swap)

    // Mixed with large numbers and duplicates
    std::vector<int> v10 = {10, 20, 10, 30, 20, 30};
    assert(minimumAdjacentSwaps(v10) == 2); // (10:1, 20:1, 30 already)

    return 0;
}

// The optimal strategy is a greedy left‑to‑right scan. Process pairs starting from the first unpaired element. For each element at index `i` (which will be the left member of a pair), find its matching duplicate at some later index `j`. To make them adjacent, repeatedly swap the duplicate leftward until it reaches index `i+1`. Each swap counts as one operation, so the cost for this pair is the distance `j - (i+1)`. After placing the pair adjacent, continue from the next unprocessed pair. This greedy approach is optimal because moving the duplicate leftward never increases the number of swaps needed for the remaining elements—it only brings that pair together, and swapping with intervening elements does not affect relative order of other pairs better than necessary. Edge cases: if the duplicate is already adjacent, cost is zero. If duplicate is never found (should not happen per problem guarantee, but handle by breaking). Complexity: worst‑case is `O(n²)` time because each scan may traverse far to find a duplicate and each swap shifts elements; space is `O(1)` extra beyond the input.
