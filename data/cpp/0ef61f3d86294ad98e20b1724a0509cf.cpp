/*
Write a C++ function that takes a `std::vector<double>` and an index range (start and end indices, both inclusive) and returns a new `std::vector<double>` containing a copy of the elements in that range. The function should handle a full-range projection (0 to size-1) as well as partial ranges, and should correctly handle edge cases like an empty vector, a range that covers the entire vector, and a range of a single element. The function must accept the vector by `const` reference and the range as two `size_t` parameters (start and end), and it should throw `std::out_of_range` if the range is invalid (start > end, or end >= vector size, or vector is empty with a non-empty range). For a valid range, the function returns a new vector with the elements from `start` to `end` inclusive.
*/

#include <vector>
#include <stdexcept>
#include <cstddef>

// Returns a copy of the elements in the inclusive range [start, end] of the input vector.
// Throws std::out_of_range on invalid range.
std::vector<double> sliceVector(const std::vector<double>& vec, std::size_t start, std::size_t end) {
    // Validate range
    if (start > end) {
        throw std::out_of_range("sliceVector: start index greater than end index");
    }
    if (end >= vec.size()) {
        throw std::out_of_range("sliceVector: end index out of range");
    }
    // Handle empty vector with a valid empty range (0,0)
    if (vec.empty() && start == 0 && end == 0) {
        return std::vector<double>(); // empty
    }
    // Use iterator range constructor to copy [start, end] inclusive
    return std::vector<double>(vec.begin() + start, vec.begin() + end + 1);
}

#include <cassert>
#include <vector>
#include <stdexcept>

// The solution function is assumed to be defined above (sliceVector).
int main() {
    // Full range on a typical vector
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> full = sliceVector(v1, 0, 3);
    assert((full == std::vector<double>{1.0, 2.0, 3.0, 4.0}));
    // Partial range in the middle
    std::vector<double> mid = sliceVector(v1, 1, 2);
    assert((mid == std::vector<double>{2.0, 3.0}));
    // Single element range
    std::vector<double> single = sliceVector(v1, 2, 2);
    assert((single == std::vector<double>{3.0}));
    // Range starting at 0, but not full
    std::vector<double> front = sliceVector(v1, 0, 1);
    assert((front == std::vector<double>{1.0, 2.0}));
    // Empty vector with valid empty range (0,0)
    std::vector<double> empty;
    std::vector<double> emptySlice = sliceVector(empty, 0, 0);
    assert(emptySlice.empty());
    // Invalid range: start > end
    bool threw = false;
    try { sliceVector(v1, 3, 2); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    // Invalid range: end >= size
    threw = false;
    try { sliceVector(v1, 0, 4); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    // Invalid range on empty vector with non-zero indices
    threw = false;
    try { sliceVector(empty, 0, 1); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

// The task is essentially to implement a manual slice operation for a `std::vector<double>`. The main algorithm is straightforward: first validate the input parameters. If the vector is empty and the range is anything other than start=0 and end=0 (which would be a degenerate valid empty range), throw. Also, if start > end or end >= vector.size(), throw `std::out_of_range`. After validation, we create a result vector of size `end - start + 1` and copy each element from `v[start]` to `v[end]` into the result in order. Alternatively, we can use the vector range constructor: `std::vector<double>(v.begin()+start, v.begin()+end+1)`, which is simpler and less error-prone. Edge cases: empty vector with start=0 and end=0 (a valid empty range) returns an empty vector; a single-element range returns a one-element vector; a full range (0 to size-1) returns a copy of the entire vector. Time complexity is O(k) where k is the number of elements in the range, since we copy each element once. Space complexity is O(k) for the new vector. The function is `const`-correct because it only reads from the input vector.
