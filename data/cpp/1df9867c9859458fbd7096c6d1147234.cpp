// Write a C++ function named `generateDescending` that accepts a single positive integer `n` and returns a `std::vector<int>` containing all integers from `n` down to `1` in strictly descending order. The function must handle the edge case `n == 1` correctly (returning `{1}`), and it should reject invalid input like `n == 0` or negative numbers by returning an empty vector. You may assume `n` is not extremely large (fits in a standard `int`), but the function should avoid any unnecessary memory or performance overhead. The task focuses on correct iteration, proper use of standard containers, and const-correctness where applicable.

The solution uses a simple loop that iterates from `n` down to `1`, appending each value to a `std::vector<int>` using `push_back`. The main algorithm is straightforward: initialize an empty vector, reserve capacity for `n` elements to avoid reallocations (though optional, it improves efficiency), then loop `for (int i = n; i >= 1; --i)` and add `i`. Edge cases: if `n <= 0`, return an empty vector immediately. If `n == 1`, the loop runs once and returns `{1}`. Time complexity is O(n) because we perform `n` iterations and each `push_back` is amortized O(1). Space complexity is O(n) because the result vector stores `n` integers (plus the negligible reserved capacity). There are no other hidden pitfalls; the loop condition `i >= 1` correctly terminates when `i` becomes 0, and the decrement is safe because `i` is an `int` (no overflow for reasonable inputs).

#include <vector>

// Generate a vector containing integers from n down to 1 inclusive.
// Returns an empty vector if n <= 0.
std::vector<int> generateDescending(int n) {
    std::vector<int> result;
    if (n <= 0) {
        return result;
    }
    result.reserve(n);
    for (int i = n; i >= 1; --i) {
        result.push_back(i);
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function declaration is assumed to be available (either included or defined above).
std::vector<int> generateDescending(int n);

int main() {
    // Basic descending order for n=5
    assert(generateDescending(5) == std::vector<int>({5, 4, 3, 2, 1}));
    // Edge case n=1
    assert(generateDescending(1) == std::vector<int>({1}));
    // Edge case n=0 returns empty
    assert(generateDescending(0).empty());
    // Negative input returns empty
    assert(generateDescending(-3).empty());
    // Larger n=10
    assert(generateDescending(10) == std::vector<int>({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}));
    // Check size matches n
    assert(generateDescending(7).size() == 7);
    // Check first and last elements for n=2
    std::vector<int> v = generateDescending(2);
    assert(v[0] == 2 && v[1] == 1);
    // Check no extra elements for n=3
    assert(generateDescending(3) == std::vector<int>({3, 2, 1}));
    // Double-check n=4 with explicit equality
    assert(generateDescending(4) == std::vector<int>({4, 3, 2, 1}));
    return 0;
}
