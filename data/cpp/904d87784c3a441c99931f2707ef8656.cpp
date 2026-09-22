Write a C++ function named `zipInterleave` that takes two `std::vector<int>` inputs (`a` and `b`) and returns a new `std::vector<int>` where the elements are interleaved starting with `a[0]`, then `b[0]`, then `a[1]`, then `b[1]`, and so on, up to the length of the shorter vector. If the vectors have different lengths, the remaining elements of the longer vector should be appended at the end in their original order. For example, given `a = {1, 2, 3}` and `b = {10, 20}`, the result should be `{1, 10, 2, 20, 3}`. The function must not modify the input vectors and should work correctly for empty vectors (returning a copy of the non‑empty one, or an empty vector if both are empty). Use `const` references for parameters.
The solution iterates through the indices `0` to `min(a.size(), b.size()) - 1`, pushing `a[i]` and then `b[i]` into the result. After that, if `a.size() > b.size()`, append the remaining elements from `a` starting at index `b.size()`. Similarly, if `b.size() > a.size()`, append the remaining elements from `b` starting at index `a.size()`. The overall time complexity is `O(n + m)` where `n = a.size()` and `m = b.size()`, because we process each element exactly once. Auxiliary space is `O(n + m)` for storing the result (excluding input vectors). Edge cases: both empty → result empty; one empty → result is a copy of the other; equal length → no trailing append needed. No integer overflow concerns since we just copy values. The function uses `const` references to avoid copying inputs.
#include <vector>
#include <cstddef>

// Interleave elements of two vectors, appending the remainder of the longer vector.
std::vector<int> zipInterleave(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    result.reserve(a.size() + b.size());  // Preallocate for efficiency.

    const std::size_t common = a.size() < b.size() ? a.size() : b.size();

    for (std::size_t i = 0; i < common; ++i) {
        result.push_back(a[i]);
        result.push_back(b[i]);
    }

    // Append remaining elements from the longer vector.
    if (a.size() > b.size()) {
        for (std::size_t i = b.size(); i < a.size(); ++i) {
            result.push_back(a[i]);
        }
    } else if (b.size() > a.size()) {
        for (std::size_t i = a.size(); i < b.size(); ++i) {
            result.push_back(b[i]);
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (zipInterleave).
int main() {
    // Equal length vectors.
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {10, 20, 30};
    assert(zipInterleave(a1, b1) == std::vector<int>({1, 10, 2, 20, 3, 30}));

    // a longer than b.
    std::vector<int> a2 = {1, 2, 3, 4, 5};
    std::vector<int> b2 = {10, 20};
    assert(zipInterleave(a2, b2) == std::vector<int>({1, 10, 2, 20, 3, 4, 5}));

    // b longer than a.
    std::vector<int> a3 = {7};
    std::vector<int> b3 = {100, 200, 300};
    assert(zipInterleave(a3, b3) == std::vector<int>({7, 100, 200, 300}));

    // Both empty.
    std::vector<int> empty1, empty2;
    assert(zipInterleave(empty1, empty2).empty());

    // One empty.
    std::vector<int> a4 = {9, 8};
    assert(zipInterleave(a4, empty2) == std::vector<int>({9, 8}));
    assert(zipInterleave(empty1, a4) == std::vector<int>({9, 8}));

    // Duplicate values and zeros.
    std::vector<int> a5 = {0, 0, 5};
    std::vector<int> b5 = {1, 2};
    assert(zipInterleave(a5, b5) == std::vector<int>({0, 1, 0, 2, 5}));

    // Large vectors (check first and last elements for correctness).
    std::vector<int> bigA(1000, 1);
    std::vector<int> bigB(500, 2);
    auto bigResult = zipInterleave(bigA, bigB);
    assert(bigResult.size() == 1500);
    assert(bigResult[0] == 1 && bigResult[1] == 2);
    assert(bigResult[998] == 1 && bigResult[999] == 2);
    assert(bigResult[1000] == 1 && bigResult[1499] == 1);

    return 0;
}
