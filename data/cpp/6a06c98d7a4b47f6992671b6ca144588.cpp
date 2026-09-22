/*
Write a C++ function `shrinkToFit` that takes a `std::vector<int>` by value, removes all elements that are greater than 50, and then reduces the vector's `capacity()` to match its new `size()` using the swap-with-temporary technique (e.g., `std::vector<int>(v).swap(v)`). The function should return the modified vector. The input vector may be empty, may contain only elements ≤ 50, or only elements > 50. The function must preserve the relative order of remaining elements and must not use `std::remove_if` or any other standard algorithm; only manual iteration and `erase` are allowed. The function must be `const`‑correct where appropriate (though the parameter is by value, so no `const` on the parameter is needed). For an empty input, return an empty vector with `capacity()` equal to the result of `shrink_to_fit` on an empty vector (which in practice is 0, but you may assert it is ≤ the original capacity).
*/

#include <vector>

// Remove all elements greater than 50, then shrink capacity to match size.
// Returns the modified vector (original order of remaining elements preserved).
std::vector<int> shrinkToFit(std::vector<int> v) {
    // Manual iteration and erase
    for (std::size_t i = 0; i < v.size(); ) {
        if (v[i] > 50) {
            v.erase(v.begin() + static_cast<std::ptrdiff_t>(i));
            // Do not increment i because the next element shifts into position i
        } else {
            ++i;
        }
    }
    // Shrink capacity using the swap-with-temporary idiom
    std::vector<int>(v).swap(v);
    return v;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Basic case with mixed values
    std::vector<int> v1 = {10, 60, 30, 90, 5};
    auto r1 = shrinkToFit(v1);
    assert((r1 == std::vector<int>{10, 30, 5}));
    assert(r1.capacity() == r1.size());

    // All elements > 50 -> empty
    std::vector<int> v2 = {51, 100, 200};
    auto r2 = shrinkToFit(v2);
    assert(r2.empty());
    assert(r2.capacity() == 0);

    // All elements <= 50 -> unchanged values and capacity shrinks to size
    std::vector<int> v3 = {1, 2, 3};
    auto r3 = shrinkToFit(v3);
    assert((r3 == std::vector<int>{1, 2, 3}));
    assert(r3.capacity() == r3.size());

    // Empty input -> empty output with capacity 0
    std::vector<int> v4;
    auto r4 = shrinkToFit(v4);
    assert(r4.empty());
    assert(r4.capacity() == 0);

    // Large capacity with few elements, all <= 50
    std::vector<int> v5(1000, 7); // 1000 elements all 7
    auto r5 = shrinkToFit(v5);
    assert(r5.size() == 1000);
    assert(r5.capacity() == 1000);

    // Mixed with duplicate values and boundary 50 (not > 50, so kept)
    std::vector<int> v6 = {50, 51, 50, 52, 49};
    auto r6 = shrinkToFit(v6);
    assert((r6 == std::vector<int>{50, 50, 49}));
    assert(r6.capacity() == r6.size());
}

// The solution manually iterates over the vector using an index or iterator, erasing each element that is > 50. Since `erase` invalidates iterators after the removal point, a simple forward loop with an index works: while `i < v.size()`, if `v[i] > 50`, call `v.erase(v.begin() + i)`; otherwise increment `i`. This ensures that after erasing, the next element is checked without skipping. After all removals, apply the swap‑with‑temporary trick to shrink capacity: `std::vector<int>(v).swap(v)`. This constructs a temporary copy (with size equal to new size and capacity at least that), then swaps its internal buffer with `v`, releasing the old large buffer. Edge cases: empty vector – no erasures, then swap with an empty temporary will result in `capacity()` being implementation‑defined (usually 0). Vector with all > 50 becomes empty; vector with all ≤ 50 is unchanged except capacity may shrink if the original capacity was larger than size (but note: after erasures, if nothing removed, `size` = original size, and the swap creates a temporary with capacity ≥ size, but that may be larger than original capacity? Actually, constructing a temporary from `v` will allocate exactly enough for `size()` elements, so `capacity` of the temporary equals `size()`. After swap, `v` gets that capacity, which might be smaller than original capacity even if no elements were removed – that is acceptable). If the vector originally had capacity > size, the swap will shrink it to exactly `size()`. Time complexity: O(n) for iteration over `n` elements, plus O(k) for each erase (since `erase` shifts elements), leading to worst-case O(n²) when many erasures occur (e.g., all > 50). Space: O(n) for the temporary copy during the shrink operation (or O(1) if using move semantics, but per spec we use the typical copy‑based swap). For typical input sizes this is fine. The function must not use any standard algorithms.
