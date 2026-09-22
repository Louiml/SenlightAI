Write a C++ function named `GetSortedIndices` that takes a `std::vector<float>` of values and returns a `std::vector<int>` containing the indices of the original vector sorted in ascending order of the corresponding float values. The function must not modify the input vector, must handle duplicate values (preserving their relative order or any stable order is acceptable), and must work correctly for empty vectors (returning an empty vector). The returned indices should be of type `int` (matching typical index types for small-to-medium sized vectors). You may use any standard library facilities, but ensure the solution is self-contained and does not rely on external code beyond standard headers.
The primary algorithm is to create an index vector `indices` of the same size as the input, filled sequentially from 0 to `n-1` using `std::iota`. Then we sort these indices using `std::sort` with a custom comparator that compares the original values at those indices. The comparator captures the input vector by reference (using `[&values]` or `[&]`) to avoid copying the data. The sort is not stable by default in C++ (though `std::sort` generally is not stable; for equal values, any order is acceptable per the task). Edge cases: an empty input vector yields an empty index vector, and `std::iota` on an empty range is safe. The size of `indices` is the same type as `values.size()` (which is `size_t`), but we need to cast to `int` when filling or returning. A safe approach is to store `size_t` indices during construction and then convert to `int` at the end, or simply use `int` directly in the `iota` loop by casting. Using `int` directly is fine for typical vector sizes, but to avoid warnings, we can explicitly cast `values.size()` to `int` when constructing the vector. Time complexity: `O(n log n)` due to sorting. Space complexity: `O(n)` for the index vector. The solution is straightforward and robust.
#include <vector>
#include <numeric>
#include <algorithm>

// Returns a vector of indices that sorts the input 'values' in ascending order.
// The original vector is not modified. Empty input yields an empty index vector.
std::vector<int> GetSortedIndices(const std::vector<float>& values) {
    const int n = static_cast<int>(values.size());
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    
    // Sort indices by comparing the corresponding float values.
    std::sort(indices.begin(), indices.end(),
        [&values](int a, int b) { return values[a] < values[b]; });
    
    return indices;
}
#include <cassert>
#include <vector>
#include <cmath>

// (The solution function is assumed to be included above.)

int main() {
    // Test 1: Basic sorting
    std::vector<float> v1 = {3.0f, 1.0f, 2.0f};
    std::vector<int> r1 = GetSortedIndices(v1);
    assert(r1 == std::vector<int>({1, 2, 0}));

    // Test 2: Already sorted
    std::vector<float> v2 = {1.0f, 2.0f, 3.0f};
    std::vector<int> r2 = GetSortedIndices(v2);
    assert(r2 == std::vector<int>({0, 1, 2}));

    // Test 3: Reverse sorted
    std::vector<float> v3 = {3.0f, 2.0f, 1.0f};
    std::vector<int> r3 = GetSortedIndices(v3);
    assert(r3 == std::vector<int>({2, 1, 0}));

    // Test 4: Duplicate values (any stable or arbitrary order is acceptable, but check size and that values map correctly)
    std::vector<float> v4 = {2.0f, 2.0f, 1.0f, 2.0f};
    std::vector<int> r4 = GetSortedIndices(v4);
    assert(r4.size() == 4);
    // Verify that following the indices yields non-decreasing values
    for (size_t i = 1; i < r4.size(); ++i) {
        assert(v4[r4[i-1]] <= v4[r4[i]]);
    }

    // Test 5: Single element
    std::vector<float> v5 = {42.0f};
    std::vector<int> r5 = GetSortedIndices(v5);
    assert(r5 == std::vector<int>({0}));

    // Test 6: Empty vector
    std::vector<float> v6 = {};
    std::vector<int> r6 = GetSortedIndices(v6);
    assert(r6.empty());

    // Test 7: Negative and fractional values
    std::vector<float> v7 = {-1.5f, 0.0f, 2.5f, -0.5f};
    std::vector<int> r7 = GetSortedIndices(v7);
    assert(r7 == std::vector<int>({0, 3, 1, 2}));

    // Test 8: Ensure original vector is not modified
    std::vector<float> v8 = {5.0f, 1.0f, 3.0f};
    std::vector<float> original = v8;
    (void)GetSortedIndices(v8);
    assert(v8 == original);

    return 0;
}
