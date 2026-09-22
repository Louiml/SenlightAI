// Write a C++ function named `applyTransformToRange` that takes a pointer to a contiguous array of integers, its length `n`, and a unary function object (or lambda) `transform`, and returns a `std::vector<int>` containing the result of applying `transform` to each element in the array in order. The transform function should accept an `int` and return an `int`. The input array may be empty (length 0), in which case the function returns an empty vector. The function must handle negative numbers, zeros, and large values without overflow in the transform operation itself. The transform operation must be applied exactly once per element, in the order they appear in the array.

#include <cassert>
#include <vector>
#include <functional>

// Solution function declared above (or include the header).

int main() {
    // Test 1: Multiply by 2
    int x1[] = {1, 2, 3, 4};
    auto mul2 = [](int v) { return v * 2; };
    assert(applyTransformToRange(x1, 4, mul2) == std::vector<int>({2, 4, 6, 8}));

    // Test 2: Add 10 to negative and zero values
    int x2[] = {-3, 0, 5};
    auto add10 = [](int v) { return v + 10; };
    assert(applyTransformToRange(x2, 3, add10) == std::vector<int>({7, 10, 15}));

    // Test 3: Empty array
    int* empty = nullptr;
    auto identity = [](int v) { return v; };
    assert(applyTransformToRange(empty, 0, identity).empty());

    // Test 4: Square values (handling negatives)
    int x4[] = {-2, -1, 0, 1, 2};
    auto square = [](int v) { return v * v; };
    assert(applyTransformToRange(x4, 5, square) == std::vector<int>({4, 1, 0, 1, 4}));

    // Test 5: Use std::negate
    int x5[] = {7, -7, 0};
    assert(applyTransformToRange(x5, 3, std::negate<int>()) == std::vector<int>({-7, 7, 0}));

    // Test 6: Large values (no overflow beyond int range)
    int x6[] = {1000000, -1000000};
    auto addLarge = [](int v) { return v + 2000000; };
    assert(applyTransformToRange(x6, 2, addLarge) == std::vector<int>({3000000, 1000000}));

    // Test 7: Double the length with a more complex lambda capturing a factor
    int factor = 3;
    auto multiplyByFactor = [factor](int v) { return v * factor; };
    int x7[] = {1, 2, 3};
    assert(applyTransformToRange(x7, 3, multiplyByFactor) == std::vector<int>({3, 6, 9}));

    // Test 8: All zeros
    int x8[] = {0, 0, 0, 0};
    auto toFive = [](int v) { (void)v; return 5; };
    assert(applyTransformToRange(x8, 4, toFive) == std::vector<int>({5, 5, 5, 5}));

    return 0;
}

#include <vector>
#include <functional>

// Applies a unary transform function to each element of a contiguous array.
// Returns a vector containing the transformed elements in the same order as the input.
std::vector<int> applyTransformToRange(const int* arr, size_t n,
                                       const std::function<int(int)>& transform) {
    std::vector<int> result;
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        result.push_back(transform(arr[i]));
    }
    return result;
}

// The solution is straightforward: loop from index 0 to `n-1`, call the passed `transform` function on each element of the array, and push the result into a vector. The function signature should use `const int*` for the array to ensure we do not modify the input, and take `size_t n` for clarity. The transform function is passed by const reference to allow both function objects and lambdas. Edge cases include `n == 0` (returns empty vector) and negative numbers (handled naturally by the unary operation). Time complexity is `O(n)` because we visit each element once, and space complexity is `O(n)` for the output vector (plus the vector overhead). There is no special complexity from the transform function itself unless it is costly, but we assume it is constant time.
