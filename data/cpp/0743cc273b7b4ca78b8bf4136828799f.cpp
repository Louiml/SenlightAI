// Write a standalone C++ function named `firstElements` that takes a vector of integers (`num_list`) and an integer (`n`), and returns a new vector containing the first `n` elements of the input vector. If `n` is greater than or equal to the size of the input vector, return a copy of the entire input vector. If `n` is zero or negative, return an empty vector. The function must not modify the input vector. The input vector can be empty, in which case the result is always empty regardless of `n`. You should implement this without using any standard library algorithms other than `push_back` and vector construction/copying.
#include <cassert>
#include <vector>

// function declaration (from the solution)
std::vector<int> firstElements(const std::vector<int>& num_list, int n);

int main() {
    // Basic case: n is less than size
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(firstElements(v1, 3) == std::vector<int>({1, 2, 3}));

    // n equals size -> returns copy of entire vector
    assert(firstElements(v1, 5) == v1);

    // n greater than size -> returns copy of entire vector
    assert(firstElements(v1, 100) == v1);

    // n = 0 -> empty
    assert(firstElements(v1, 0).empty());

    // n negative -> empty
    assert(firstElements(v1, -5).empty());

    // Empty input vector
    std::vector<int> empty;
    assert(firstElements(empty, 3).empty());
    assert(firstElements(empty, 0).empty());

    // Single-element vector
    std::vector<int> single = {42};
    assert(firstElements(single, 1) == single);
    assert(firstElements(single, 0).empty());

    // Verify input not modified
    std::vector<int> original = {10, 20, 30};
    std::vector<int> result = firstElements(original, 2);
    assert(original == std::vector<int>({10, 20, 30}));
    assert(result == std::vector<int>({10, 20}));

    return 0;
}
#include <vector>

// Returns a new vector containing the first n elements of num_list.
// If n <= 0, returns an empty vector.
// If n >= num_list.size(), returns a copy of the entire input vector.
std::vector<int> firstElements(const std::vector<int>& num_list, int n) {
    // Clamp n to a valid range: [0, num_list.size()].
    int count = n;
    if (count < 0) {
        count = 0;
    }
    if (count > static_cast<int>(num_list.size())) {
        count = static_cast<int>(num_list.size());
    }

    // Build the result by copying the first 'count' elements.
    std::vector<int> answer;
    answer.reserve(count); // optional optimization to avoid reallocations
    for (int i = 0; i < count; ++i) {
        answer.push_back(num_list[i]);
    }
    return answer;
}
// The core algorithm is straightforward: iterate from index 0 up to (but not including) `n`, but stop early if the index reaches the size of the vector. The number of elements to copy is `min(n, num_list.size())`, but `n` could be negative, so effective count = `max(0, min(n, (int)num_list.size()))`. Since `num_list.size()` returns an unsigned type, it's safest to convert `n` to `size_t` after clamping to non-negative, or handle the comparison carefully. For simplicity, compute `int count = n; if (count > (int)num_list.size()) count = (int)num_list.size(); if (count < 0) count = 0;` then loop `i < count`. Important edge cases: empty input vector, `n` equal to zero, `n` larger than vector size, negative `n`, and `n` exactly equal to the vector size. Time complexity is O(k) where k is the number of copied elements, and space complexity is O(k) for the result vector (excluding the input). The function should accept the input by `const` reference to avoid copying, and return a new vector.
