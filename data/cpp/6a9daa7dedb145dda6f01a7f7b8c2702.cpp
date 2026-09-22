Write a C++ function `int alternatingSum(const std::vector<int>& numbers)` that takes a non-empty vector of integers and returns the alternating sum, where each element at an even index (0-based) is added and each element at an odd index is subtracted. For example, for `{5, 3, 8, 1}`, the result is `5 - 3 + 8 - 1 = 9`. The vector may contain negative numbers, zeros, and duplicates. The function must handle vectors of any length greater than zero, including length 1. You must implement the function without modifying the input vector and apply `const` correctness appropriately.
#include <cassert>
#include <vector>

int alternatingSum(const std::vector<int>& numbers);

int main() {
    assert(alternatingSum({5, 3, 8, 1}) == 9);
    assert(alternatingSum({10}) == 10);
    assert(alternatingSum({1, 2}) == -1);
    assert(alternatingSum({-1, -2, -3}) == -2);
    assert(alternatingSum({0, 0, 0}) == 0);
    assert(alternatingSum({7, -4, 2}) == 13);
    assert(alternatingSum({100, 1, 100, 1, 100}) == 298);
    assert(alternatingSum({-5}) == -5);
    assert(alternatingSum({2, 2, 2, 2}) == 0);
    assert(alternatingSum({3, -1, -2, 4, 5}) == 11);
    return 0;
}
#include <vector>

// Compute the alternating sum of a non-empty vector.
// Even indices add, odd indices subtract.
int alternatingSum(const std::vector<int>& numbers) {
    int total = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (i % 2 == 0) {
            total += numbers[i];
        } else {
            total -= numbers[i];
        }
    }
    return total;
}
// The alternating sum is straightforward: iterate through the vector once, maintaining a running total. For each index `i`, if `i` is even, add the value; if odd, subtract it. The sign alternates strictly by index parity, regardless of the values. Edge cases: a single-element vector returns that element itself (added at index 0). An empty vector is not allowed per the specification, but if defensive coding is desired, one could assert non-empty. Time complexity is O(n) where n is the number of elements, since we traverse the vector exactly once. Space complexity is O(1), using only a running total variable.
