// Write a C++ function that takes an integer `n` followed by `n` integers (each between 1 and 100 inclusive) from a standard input stream and returns the minimum number of distinct values that must be removed so that all remaining values are identical. In other words, given a multiset of size `n`, find the frequency of the most common value and output `n - max_frequency`. For example, if the input is `5 1 2 2 3 3`, the most frequent value is 2 (appears twice) or 3 (appears twice), so the answer is `5 - 2 = 3`. The function must be testable without reading from `cin` directly; instead, it should accept a `std::vector<int>` as input and return an `int`. Assume the vector is non-empty and all elements are in the range 1–100.
#include <cassert>
#include <vector>

// The solution function declaration is assumed available via the header inclusion above.
int minRemovalsToMakeUniform(const std::vector<int>& values);

int main() {
    assert(minRemovalsToMakeUniform({1, 1, 1, 1}) == 0);
    assert(minRemovalsToMakeUniform({1}) == 0);
    assert(minRemovalsToMakeUniform({1, 2, 3}) == 2);
    assert(minRemovalsToMakeUniform({1, 2, 2, 3, 3, 3}) == 3);
    assert(minRemovalsToMakeUniform({100, 100, 50, 50, 50, 1}) == 3);
    assert(minRemovalsToMakeUniform({5, 5, 4, 4, 4, 4}) == 2);
    assert(minRemovalsToMakeUniform({7, 7, 7, 8, 8}) == 2);
    assert(minRemovalsToMakeUniform({1, 2, 2, 2, 2, 2}) == 1);
    assert(minRemovalsToMakeUniform({10, 20, 30, 40}) == 3);
    return 0;
}
#include <vector>
#include <algorithm>

// Return the number of elements to remove so that all remaining are identical.
int minRemovalsToMakeUniform(const std::vector<int>& values) {
    int freq[101] = {0}; // indices 1..100 used; index 0 unused

    for (int v : values) {
        freq[v]++;
    }

    int maxFreq = 0;
    for (int i = 1; i <= 100; ++i) {
        maxFreq = std::max(maxFreq, freq[i]);
    }

    return static_cast<int>(values.size()) - maxFreq;
}
// The solution approach is to count the frequency of each possible value (1 through 100) using a fixed-size frequency array initialized to zero. Iterate over the input vector and increment the count for each element’s value. After counting, find the maximum frequency among all 100 frequencies. The answer is the total number of elements `n` minus that maximum frequency. Edge cases: if all elements are identical, the maximum frequency equals `n`, so the answer is 0. If the vector is size 1, the answer is 0. Because the value range is small and fixed, a constant-size array suffices; no sorting or hash map is needed. Time complexity is O(n + 100) = O(n), and space complexity is O(100) = O(1) auxiliary space (the array size is constant, independent of n). Const-correctness applies to the input vector reference.
