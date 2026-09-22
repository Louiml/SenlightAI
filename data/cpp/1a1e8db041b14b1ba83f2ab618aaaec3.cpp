Write a C++ function `findMaximum(const std::vector<double>& arr)` that returns the largest value in a non-empty vector of doubles using a single-pass linear scan. The function must correctly handle negative values, duplicate values, and cases where the maximum occurs at the first or last element. You are also required to provide a second function `findMaximumWithIndex(const std::vector<double>& arr)` that returns a `std::pair<double, int>` containing the maximum value and its zero‑based index (in case of duplicates, the first occurrence). Ensure both functions are `const`‑correct and use no external libraries beyond standard headers. The task tests will compare the returned values and indices against expected results.
#include <cassert>
#include <vector>
#include <utility>

// Function declarations from the solution (included here for completeness)
double findMaximum(const std::vector<double>&);
std::pair<double, int> findMaximumWithIndex(const std::vector<double>&);

int main() {
    // Test 1: Simple positive values
    std::vector<double> v1 = {1.5, 3.2, 2.8, 4.1};
    assert(findMaximum(v1) == 4.1);
    auto r1 = findMaximumWithIndex(v1);
    assert(r1.first == 4.1 && r1.second == 3);

    // Test 2: All negative values
    std::vector<double> v2 = {-5.0, -2.3, -8.7, -1.1};
    assert(findMaximum(v2) == -1.1);
    auto r2 = findMaximumWithIndex(v2);
    assert(r2.first == -1.1 && r2.second == 3);

    // Test 3: Duplicate maximum values
    std::vector<double> v3 = {7.7, 9.9, 5.5, 9.9, 3.3};
    assert(findMaximum(v3) == 9.9);
    auto r3 = findMaximumWithIndex(v3);
    assert(r3.first == 9.9 && r3.second == 1); // first occurrence

    // Test 4: Single element
    std::vector<double> v4 = {42.42};
    assert(findMaximum(v4) == 42.42);
    auto r4 = findMaximumWithIndex(v4);
    assert(r4.first == 42.42 && r4.second == 0);

    // Test 5: Maximum at the beginning
    std::vector<double> v5 = {100.0, -1.0, 0.5, 50.0};
    assert(findMaximum(v5) == 100.0);
    auto r5 = findMaximumWithIndex(v5);
    assert(r5.first == 100.0 && r5.second == 0);

    // Test 6: Maximum at the end
    std::vector<double> v6 = {1.0, 2.0, 3.0, 4.0, 5.0};
    assert(findMaximum(v6) == 5.0);
    auto r6 = findMaximumWithIndex(v6);
    assert(r6.first == 5.0 && r6.second == 4);

    // Test 7: Large vector with random pattern (simple simulation)
    std::vector<double> v7 = {0.5, -3.2, 2.7, 1.1, -0.9, 4.4};
    assert(findMaximum(v7) == 4.4);
    auto r7 = findMaximumWithIndex(v7);
    assert(r7.first == 4.4 && r7.second == 5);

    // Test 8: Edge with zeros and negatives mixed
    std::vector<double> v8 = {0.0, -0.0, 0.0, -1.0};
    assert(findMaximum(v8) == 0.0);
    auto r8 = findMaximumWithIndex(v8);
    assert(r8.first == 0.0 && r8.second == 0);

    return 0;
}
#include <vector>
#include <utility>
#include <limits>

// Return the largest value in a non-empty vector of doubles.
double findMaximum(const std::vector<double>& arr) {
    double max_val = arr.front();
    for (std::size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

// Return the largest value and the index of its first occurrence.
std::pair<double, int> findMaximumWithIndex(const std::vector<double>& arr) {
    double max_val = arr.front();
    int max_index = 0;
    for (std::size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_index = static_cast<int>(i);
        }
    }
    return {max_val, max_index};
}
// The core algorithm is a straightforward linear scan. Initialize `max_val` to the first element (or a very small sentinel like `std::numeric_limits<double>::lowest()`) and `max_index` to 0. Then iterate through the vector from index 1 onward, updating the maximum value and its index whenever a strictly larger element is found. For the value‑only version, duplicate values are ignored because only `>` is used. Edge cases include: an empty vector (undefined per task, but we can assert non‑empty in tests), a single‑element vector where both functions return that element and index 0, all‑negative values where the maximum is the least negative, and large vectors (time complexity is O(n)). Space complexity is O(1) since we only store a few variables. The index function must return the first occurrence of the maximum, meaning we update only on `>` not `>=`.
