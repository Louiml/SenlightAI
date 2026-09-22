/*
Write a standalone C++ function that, given a non-empty vector of integers, returns a new vector containing only the elements whose indices are in the range `[begin, end)` (where `begin` and `end` are non-negative integers, with `begin <= end`, and `end` may be larger than the vector size). The function should use only iterator-based construction (not direct indexing) to mimic the behavior of vector range constructors. If the input vector is empty or `begin` is greater than the vector size, the function should return an empty vector. Also handle the case where `begin` equals the vector size (empty range). The return type must be `std::vector<int>`.
*/

#include <vector>
#include <algorithm>

// Return a subvector of 'vec' containing elements with indices [begin, end).
// If begin >= end, begin > vec.size(), or vec is empty, returns an empty vector.
// The end index is clamped to vec.size().
std::vector<int> rangeSubvector(const std::vector<int>& vec, size_t begin, size_t end) {
    if (vec.empty() || begin >= vec.size() || begin >= end) {
        return {};
    }
    size_t actual_begin = begin;
    size_t actual_end = std::min(end, vec.size());
    if (actual_begin >= actual_end) {
        return {};
    }
    return std::vector<int>(vec.begin() + actual_begin, vec.begin() + actual_end);
}

#include <cassert>
#include <vector>

// The function signature is as declared in the solution.
std::vector<int> rangeSubvector(const std::vector<int>& vec, size_t begin, size_t end);

int main() {
    std::vector<int> a = {10, 20, 30, 40, 50};
    // Normal range
    assert(rangeSubvector(a, 1, 3) == std::vector<int>({20, 30}));
    // End beyond size
    assert(rangeSubvector(a, 2, 10) == std::vector<int>({30, 40, 50}));
    // Begin == size (empty)
    assert(rangeSubvector(a, 5, 10).empty());
    // Begin > size (empty)
    assert(rangeSubvector(a, 6, 10).empty());
    // begin == end (empty)
    assert(rangeSubvector(a, 2, 2).empty());
    // Full vector when begin=0, end large
    assert(rangeSubvector(a, 0, 100) == a);
    // Empty input
    std::vector<int> b;
    assert(rangeSubvector(b, 0, 5).empty());
    // Single element, end at size
    std::vector<int> c = {7};
    assert(rangeSubvector(c, 0, 1) == std::vector<int>({7}));
    assert(rangeSubvector(c, 0, 0).empty());
    return 0;
}

// The solution must carefully compute the actual valid range of indices to copy, since the requested `end` may exceed the vector's size. The algorithm:
// 1. Determine `actual_begin = min(begin, vec.size())` (since we can't start beyond the last element; but if begin > size, we return empty).
// 2. Determine `actual_end = min(end, vec.size())`.
// 3. If `actual_begin >= actual_end` or `vec` is empty, return an empty vector.
// 4. Otherwise, construct the result using `std::vector<int>(vec.begin() + actual_begin, vec.begin() + actual_end)`. This uses forward iterators and matches the original snippet's iterator-based construction.
// Edge cases: `end` larger than size (must clamp); `begin` larger than size (empty); `begin == end` (empty); empty input vector (empty). Time complexity is O(k) where k is the number of copied elements. Space complexity is O(k) for the returned vector.
