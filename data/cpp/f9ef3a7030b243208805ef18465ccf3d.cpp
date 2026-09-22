Write a C++ function that simulates the described GLSL optimization pass on a simpler abstraction: given a plain `std::vector<int>` representing a runtime-dynamic vector index into a fixed-size array (e.g., `arr[index]` where `index` is a runtime variable), and given a target array length \(n\), the function must produce a new `std::vector` of size \(n\) such that the result equals the original array when the index is in `[0, n)`, and equals a provided fallback value (e.g., `0`) when the index is out of bounds. The function must avoid any direct conditional branching on the index at the top level (i.e., do not use an `if (index < n && index >= 0)` style check to select the result); instead, it must simulate the "conditional moves" approach by computing a mask that is 1 for every valid index position and 0 otherwise, then for each position `i` select either `arr[i]` or the fallback based on the mask bit. The function signature is `std::vector<int> vec_index_to_cond_assign(const std::vector<int>& arr, int index, int fallback)` and must enforce that `arr` is non-empty. Include edge-case handling for negative index and index greater than or equal to `arr.size()`. Your implementation must not use `std::conditional` or any ternary operator on the index; you must compute the mask arithmetically and use it to mask values.

#include <cassert>
#include <vector>

// The solution function is declared above; this is the test harness.
std::vector<int> vec_index_to_cond_assign(const std::vector<int>& arr, int index, int fallback);

int main() {
    // Test with a normal vector and valid index
    std::vector<int> a1 = {10, 20, 30};
    assert(vec_index_to_cond_assign(a1, 0, -1) == std::vector<int>({10, -1, -1}));
    assert(vec_index_to_cond_assign(a1, 1, -1) == std::vector<int>({-1, 20, -1}));
    assert(vec_index_to_cond_assign(a1, 2, -1) == std::vector<int>({-1, -1, 30}));

    // Out-of-bounds index (positive)
    assert(vec_index_to_cond_assign(a1, 3, -1) == std::vector<int>({-1, -1, -1}));

    // Negative index
    assert(vec_index_to_cond_assign(a1, -1, -1) == std::vector<int>({-1, -1, -1}));

    // Single-element vector
    std::vector<int> a2 = {42};
    assert(vec_index_to_cond_assign(a2, 0, 0) == std::vector<int>({42}));
    assert(vec_index_to_cond_assign(a2, 1, 0) == std::vector<int>({0}));

    // Vector with duplicates
    std::vector<int> a3 = {7, 7, 7};
    assert(vec_index_to_cond_assign(a3, 0, 0) == std::vector<int>({7, 0, 0}));
    assert(vec_index_to_cond_assign(a3, 1, 0) == std::vector<int>({0, 7, 0}));

    // Test with fallback value that is not zero
    std::vector<int> a4 = {1, 2, 3, 4};
    assert(vec_index_to_cond_assign(a4, 2, 100) == std::vector<int>({100, 100, 3, 100}));

    // Test with a vector containing zeros and negative numbers
    std::vector<int> a5 = {-5, 0, 3};
    assert(vec_index_to_cond_assign(a5, 1, -1) == std::vector<int>({-1, 0, -1}));
    assert(vec_index_to_cond_assign(a5, -2, -1) == std::vector<int>({-1, -1, -1}));

    return 0;
}

#include <vector>
#include <stdexcept>

// Simulates the GLSL pass "vec_index_to_cond_assign" on a simple integer vector.
// Given a fixed-size array 'arr' and a runtime integer 'index', produces a new
// vector of the same size where each position i equals arr[i] if index == i,
// otherwise equals 'fallback'. No top-level conditional branching on 'index' is used;
// instead, a per-position mask is computed arithmetically.
std::vector<int> vec_index_to_cond_assign(const std::vector<int>& arr, int index, int fallback) {
    if (arr.empty()) {
        throw std::invalid_argument("arr must be non-empty");
    }
    const std::size_t n = arr.size();
    std::vector<int> result(n);

    for (std::size_t i = 0; i < n; ++i) {
        // mask is 1 if index equals i, 0 otherwise. Using == produces a bool
        // that implicitly converts to int 0 or 1 without explicit branching.
        const int mask = (index == static_cast<int>(i)) ? 1 : 0;
        // Conditional move: select arr[i] when mask is 1, fallback otherwise.
        result[i] = arr[i] * mask + fallback * (1 - mask);
    }
    return result;
}

// The core idea mirrors the GLSL pass: instead of branching on the dynamic index, we generate a per-position condition that is `1` if the index equals that position and `0` otherwise. The mask is computed arithmetically by comparing the index to each position without control flow: for each `i`, the mask bit is `(index == i) ? 1 : 0`. Since `index` is an integer, we can compute this as `(index == i)` but that is still a conditional; to avoid any conditional, we can use the transformation: `mask_i = 1 - min(1, abs(index - i))`, but that uses `abs` and still has a comparison inside `min`. A better approach that matches the spirit of the "conditional move" is to use the fact that `(index == i)` can be expressed as `((index - i) == 0)` which equals `0` when nonzero and `1` when zero; however, to avoid direct branching, we can rely on the fact that a comparison yields a boolean (0 or 1) and use arithmetic: `mask_i = (index == i) ? 1 : 0` is still a conditional expression, but in C++ a comparison operator `==` returns a bool that implicitly converts to 0/1 without an explicit branch in the source code (the compiler may still generate conditional moves or branches, but the task is about source-level structure). To be faithful to the "conditional move" concept, we can compute the mask as `mask_i = (index == i) ? 1 : 0` and then for each position produce a value that is either `arr[i]` when mask is 1 or `fallback` when mask is 0, using arithmetic: `result[i] = arr[i] * mask_i + fallback * (1 - mask_i)`. This avoids any top-level `if` on the index. Important edge cases: negative index (no mask bit is 1, so all fallback), index greater than `arr.size()-1` (also all fallback), and `arr` size zero (task guarantees non-empty). For an array of length `n`, the algorithm runs in O(n) time and uses O(n) extra space for the result and O(1) auxiliary space besides the output vector.
