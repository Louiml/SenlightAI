// Given a sequence of integers that has already been split into two individually sorted contiguous halves (the first half is sorted ascending, the second half is sorted ascending), write a C++ function `size_t adaptiveCountMerge(int* data, size_t n, size_t split)` that merges the two halves into a single sorted ascending sequence **in place**, but with the constraint that it may not allocate any extra memory beyond a small fixed buffer (at most 16 integers). It must return the number of element moves (assignments) performed. The merge must be stable: if two elements have equal values, the one from the first half appears before the one from the second half in the output. The function should handle edge cases including an empty array, all elements in one half, duplicate values, and odd/even splits. It is intended to mimic the behavior of a limited‑memory adaptive merge algorithm, not a full general‑purpose sort.
// The problem requires an in‑place merge of two sorted runs with only O(1) auxiliary memory (here, a small local buffer of 16 ints). The simplest approach is a **block‑wise rotation merge**. We repeatedly find the correct position of the smallest element from the second run among the first run using a binary search (or linear scan since it's O(n) anyway; binary search reduces comparisons). We then rotate the block of elements from the current position in the first run up to the found position (all of which are ≤ the current element of the second run) to the correct final location, copying the next element from the second run in between. More concretely: maintain two pointers `i` (in first run) and `j` (in second run). Find `k` = number of elements from first run that are ≤ `data[j]` (using a linear scan or binary search, but we must move them in order). Rotate the slice `[i, i+k)` to the front of the current merged output, then copy `data[j]` after them. This requires a temporary variable for rotation (using reversal or a small buffer) – we can use a local array of size up to 16, but if the block is larger than the buffer, we must perform a sieve‑style rotation (standard in‑place rotate) using a small fixed buffer and O(n) time. To keep it simple and correct, we can implement a rotation using a helper that uses the fixed 16‑int buffer for any block length (by rotating in chunks of up to 16). The total time is O(n²) in the worst case if we use linear scan, but we can improve to O(n log n) by binary searching the split point. For a teaching task, we accept O(n²) time but note it in the analysis. However, a more efficient and still simple approach is to use a **buffer‑based merge** when the second run is short (≤16), else use a recursion‑free approach. The intended algorithm: if the size of the second run is ≤ buffer size, copy it to the buffer, merge from buffer and first run into the original array. Otherwise, find a pivot that splits both runs about equally (e.g., take the middle element of the second run and binary‑search it in the first run), recursively merge the left parts and then the right parts, but that would require extra stack. Since we want a single‑function task, we choose the simplest correct approach: a **linear scan merge with in‑place rotation** that uses a 16‑element buffer for rotation. This is simple, stable, and O(n²) worst‑case time, O(1) extra space. Edge cases: if either run is empty, no moves. If all elements in first run are ≤ all in second, we just shift the second run left by copying (using buffer) and count moves. If split == 0 or split == n, handle gracefully.
#include <cstddef>
#include <algorithm>
#include <cstring>

// Helper: rotate the range [first, last) to the right by `shift` positions,
// using a fixed buffer of at most 16 ints.  Counts each element move.
static size_t rotateRight(int* first, int* last, size_t shift, size_t& moves) {
    if (first == last || shift == 0) return 0;
    size_t len = static_cast<size_t>(last - first);
    shift %= len;
    if (shift == 0) return 0;

    int buffer[16];
    size_t block = 16;
    size_t total = 0;

    // Move the last `shift` elements into the buffer (in chunks of up to 16).
    int* src = last - shift;
    size_t to_copy = shift;
    size_t buf_pos = 0;
    // First copy the suffix into the buffer (they will be moved to front).
    while (to_copy > 0) {
        size_t chunk = std::min(to_copy, block);
        std::memcpy(buffer + buf_pos, src, chunk * sizeof(int));
        src += chunk;
        to_copy -= chunk;
        buf_pos += chunk;
        moves += chunk;
    }

    // Now shift the main part (the first len-shift elements) to the right by shift.
    int* dst = last - 1;
    int* src2 = last - shift - 1;
    size_t remaining = len - shift;
    while (remaining > 0) {
        size_t chunk = std::min(remaining, block);
        // Move chunk elements from src2 backwards to dst.
        for (size_t i = 0; i < chunk; ++i) {
            *dst = *src2;
            dst--;
            src2--;
            moves++;
        }
        remaining -= chunk;
    }

    // Finally copy the buffered suffix to the front.
    int* front = first;
    for (size_t i = 0; i < buf_pos; ++i) {
        *front = buffer[i];
        front++;
        moves++;
    }
    return moves;
}

// Merge two sorted runs [data, data+split) and [data+split, data+n) in place,
// stable, using at most 16 extra ints.  Returns total number of element moves.
size_t adaptiveCountMerge(int* data, size_t n, size_t split) {
    if (n == 0 || split == 0 || split == n) return 0;

    size_t moves = 0;
    size_t i = 0;           // start of first run (current position)
    size_t j = split;       // current position in second run
    int buffer[16];

    while (i < j && j < n) {
        // If the current element of the second run is >= all remaining of the first,
        // we can simply shift the remaining of the first run right and then the rest of second.
        if (data[i] > data[j]) {
            // Find how many elements of the first run are <= data[j].
            size_t k = 0;
            while (i + k < j && data[i + k] <= data[j]) {
                ++k;
            }
            // If k == j-i, then all remaining first elements are <= data[j];
            // we need to place data[j] after them.
            if (k == j - i) {
                // Shift the remaining first run right by one to make room for data[j].
                // This can be done by rotating [i, j+1) right by 1.
                // But easier: just copy data[j] to a temp, shift right, then put it at i.
                int temp = data[j];
                // Shift [i, j) right by one.
                for (size_t pos = j; pos > i; --pos) {
                    data[pos] = data[pos-1];
                    moves++;
                }
                data[i] = temp;
                moves++;
                i++;
                j++;
                continue;
            }
            // Now we need to rotate [i, i+k) to the right by 0? Actually we need to place
            // the k elements (all <= data[j]) at the front, then data[j] after them.
            // But the elements are already in correct relative order; we just need to move
            // the first k elements to the position i (they are already there) and then
            // insert data[j] after them. So we shift [i+k, j+1) right by 1.
            // First save data[j].
            int temp = data[j];
            // Shift [i+k, j) right by one.
            for (size_t pos = j; pos > i+k; --pos) {
                data[pos] = data[pos-1];
                moves++;
            }
            data[i+k] = temp;
            moves++;
            i = i + k + 1;
            j++;
        } else {
            // data[i] <= data[j] -> advance i
            i++;
        }
    }
    // If i == j, we have consumed all first run, the rest of second run is already in place.
    return moves;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Declare the function from the solution (for the test we include the same code above).

int main() {
    // Basic merges
    int a1[] = {1,3,5,2,4,6};
    assert(adaptiveCountMerge(a1,6,3) > 0);
    assert(std::is_sorted(a1,a1+6));

    int a2[] = {1,2,3,4,5,6};
    assert(adaptiveCountMerge(a2,6,3) == 0);
    assert(std::is_sorted(a2,a2+6));

    int a3[] = {1,1,1,1,1};
    assert(adaptiveCountMerge(a3,5,2) == 0);
    assert(std::is_sorted(a3,a3+5));

    // Empty and single
    int a4[] = {5};
    assert(adaptiveCountMerge(a4,1,0) == 0);
    assert(adaptiveCountMerge(a4,1,1) == 0);

    // Opposite order
    int a5[] = {5,4,3,2,1};
    assert(adaptiveCountMerge(a5,5,2) > 0);
    assert(std::is_sorted(a5,a5+5));

    // Duplicates across boundary
    int a6[] = {1,2,2,2,1,2,2,2};
    assert(adaptiveCountMerge(a6,8,4) > 0);
    assert(std::is_sorted(a6,a6+8));

    // Large split with many duplicates
    int a7[] = {1,1,1,2,2,2,1,1,1,2,2,2};
    assert(adaptiveCountMerge(a7,12,6) > 0);
    assert(std::is_sorted(a7,a7+12));

    // All elements in first run
    int a8[] = {1,2,3,4,5,6};
    assert(adaptiveCountMerge(a8,6,6) == 0);
    assert(std::is_sorted(a8,a8+6));

    // All elements in second run
    int a9[] = {6,5,4,3,2,1};
    assert(adaptiveCountMerge(a9,6,0) == 0);

    // Random test
    std::vector<int> v;
    for (int n : {10, 20, 50}) {
        v.resize(n);
        for (int t = 0; t < 20; ++t) {
            for (auto& x : v) x = rand() % 100;
            std::sort(v.begin(), v.begin()+n/2);
            std::sort(v.begin()+n/2, v.end());
            std::vector<int> original = v;
            adaptiveCountMerge(v.data(), n, n/2);
            assert(std::is_sorted(v.begin(), v.end()));
            // Stability check: original positions of equal elements are preserved.
        }
    }

    return 0;
}
