// Write a C++ function `rotateAndSum` that takes a vector of integers and a non-negative rotation amount `k`. The function should rotate the vector to the right by `k` positions (i.e., elements shift toward the end, wrapping around) and then return the sum of all elements in the rotated vector. If the vector is empty, the function must return 0. The function should not modify the input vector; it should return the sum as an `int`. You may assume `k` fits in `size_t`. For example, given `{1, 2, 3, 4, 5}` and `k = 2`, the rotated vector becomes `{4, 5, 1, 2, 3}` and the sum is `15` (unchanged because rotation preserves order, but the function must actually perform the rotation internally to demonstrate understanding). The key requirement is to correctly handle `k` larger than the vector size (use modulo) and edge cases like empty vectors or `k = 0`.
#include <cassert>
#include <vector>

int main() {
    std::vector<int> empty;
    assert(rotateAndSum(empty, 10) == 0);

    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(rotateAndSum(v1, 0) == 15);      // no rotation
    assert(rotateAndSum(v1, 2) == 15);      // rotation preserves sum
    assert(rotateAndSum(v1, 5) == 15);      // rotation by size
    assert(rotateAndSum(v1, 7) == 15);      // k > size, modulo 2

    std::vector<int> v2 = {10, -3, 4};
    assert(rotateAndSum(v2, 1) == 11);      // sum = 11, rotation does not matter
    assert(rotateAndSum(v2, 3) == 11);

    std::vector<int> v3 = {42};
    assert(rotateAndSum(v3, 100) == 42);    // single element

    std::vector<int> v4 = {0, 0, 0};
    assert(rotateAndSum(v4, 12345) == 0);

    return 0;
}
#include <vector>
#include <numeric>

// Rotate the input vector to the right by k positions and return the sum of its elements.
int rotateAndSum(const std::vector<int>& values, size_t k) {
    if (values.empty()) {
        return 0;
    }

    size_t n = values.size();
    size_t shift = k % n;

    std::vector<int> rotated(n);
    for (size_t i = 0; i < n; ++i) {
        rotated[(i + shift) % n] = values[i];
    }

    return std::accumulate(rotated.begin(), rotated.end(), 0);
}
// The solution must first handle the empty vector case, returning 0 immediately. For a non‑empty vector, since rotating the vector does not change the set of elements, the sum is simply the sum of the original elements. However, the task explicitly requires performing the rotation to demonstrate the algorithm, so the reference implementation will create a new vector of the same size, place the elements in rotated positions, and then compute the sum using `std::accumulate`. For a vector of size `n`, the effective rotation is `k % n`. For each index `i` in the original vector, its new position after right‑rotation is `(i + k) % n`, because the element originally at index `i` moves to `(i + k) % n`. Building the rotated vector takes `O(n)` time and `O(n)` auxiliary space for the copy. The sum computation is `O(n)`. Edge cases: `k = 0` leaves the vector unchanged; `k` exactly equal to `n` also leaves it unchanged; `k` much larger than `n` is handled by modulo; an empty vector returns 0 without any modulo operation (avoid division by zero). The function is `const`‑correct because it takes the input by `const std::vector<int>&`.
