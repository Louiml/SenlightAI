// Write a C++ function named `sumFrontAndBack` that takes a non-empty constant reference to a `std::deque<float>` and returns a `float` which is the sum of the first and last elements of the deque. The function must handle deques with a single element (in which case that element is added to itself). Assume the input deque is always non-empty. The function should not modify the input deque and should be `const`-correct. Additionally, show how to correctly use `push_front` and indexed access in a test environment.
// The solution uses direct indexed access to the deque: since `std::deque` provides random access via `operator[]`, we can grab the first element at index `0` and the last element at index `size()-1`. For a single-element deque, `size()==1`, so both indices point to the same element; adding it to itself is the intended behavior. No iteration is needed. Edge cases: empty deque is not allowed per the specification, but we could add a defensive `assert` for non-emptiness. Time complexity is O(1) because only two elements are accessed. Space complexity is O(1) as no extra storage is used. The function is marked `const` because it does not modify the deque, and we pass the deque by `const&` to avoid copying.
#include <deque>
#include <cassert>

// Returns the sum of the first and last elements of a non-empty deque<float>.
float sumFrontAndBack(const std::deque<float>& coll) {
    // Defensive check: ensures the caller respects the precondition.
    assert(!coll.empty());
    // Access first (index 0) and last (index size-1) elements directly.
    return coll.front() + coll.back();
}
#include <cassert>
#include <deque>
#include <cmath>

// The solution function is declared above; here we test it.
int main() {
    // Test with multiple elements
    std::deque<float> d1 = {1.5f, 2.0f, 3.5f};
    assert(std::fabs(sumFrontAndBack(d1) - 5.0f) < 1e-6); // 1.5 + 3.5 = 5.0

    // Test with two elements
    std::deque<float> d2 = {10.0f, -2.5f};
    assert(std::fabs(sumFrontAndBack(d2) - 7.5f) < 1e-6); // 10.0 + (-2.5) = 7.5

    // Test with a single element (added to itself)
    std::deque<float> d3 = {4.25f};
    assert(std::fabs(sumFrontAndBack(d3) - 8.5f) < 1e-6); // 4.25 + 4.25 = 8.5

    // Test with negative numbers
    std::deque<float> d4 = {-1.0f, 5.0f, -3.0f};
    assert(std::fabs(sumFrontAndBack(d4) - (-4.0f)) < 1e-6); // -1.0 + (-3.0) = -4.0

    // Test with zeros
    std::deque<float> d5 = {0.0f, 0.0f};
    assert(std::fabs(sumFrontAndBack(d5) - 0.0f) < 1e-6);

    // Test with fractional values that might have floating error
    std::deque<float> d6 = {0.1f, 0.2f, 0.3f};
    assert(std::fabs(sumFrontAndBack(d6) - 0.4f) < 1e-6); // 0.1 + 0.3 = 0.4

    return 0;
}
