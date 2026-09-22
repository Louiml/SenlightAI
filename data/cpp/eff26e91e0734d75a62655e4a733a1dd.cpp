// Given an array of \(n\) integers where each integer is in the range \([1, 1000]\), write a C++ function `int minimumRemovals(const std::vector<int>& values)` that returns the minimum number of elements that must be removed so that all remaining elements are equal. The removal process is not restricted to any order; you can delete any subset of elements. For instance, if the input is `[1, 2, 2, 3]`, the answer is `2` (keep both `2`s, remove `1` and `3`). The result must be non‑negative, and for an empty vector return `0` (trivially all remaining elements are equal). The function should handle duplicate values correctly and should not modify the input.
// The goal is to keep the largest possible group of identical numbers, because removing the fewest elements means retaining the most elements that are already equal. Thus, the problem reduces to finding the frequency (count) of the most common value in the array, and then the answer is `n - max_frequency`. Since the values are constrained to a small fixed range \([1,1000]\), we can use a frequency array of size \(1001\) (indices 1–1000 are used; index 0 is unused). We iterate over the input vector, incrementing the frequency for each element. Then we find the maximum value in the frequency array using `std::max_element`. For empty input, the frequency array is all zeros, the max is 0, and `n - 0 = 0`, which satisfies the specification. Time complexity is \(O(n + 1001) = O(n)\) because we scan the vector once and then scan 1001 fixed entries; auxiliary space is \(O(1001)\), constant relative to input size. Edge cases: all elements distinct → answer is `n-1`; all elements identical → answer is `0`; empty vector → answer is `0` (though problem likely expects non‑empty, handling it safely is good practice).
#include <vector>
#include <algorithm>

// Returns the minimum number of elements to remove so that all remaining elements are equal.
int minimumRemovals(const std::vector<int>& values) {
    const int MAX_VAL = 1000;
    std::vector<int> freq(MAX_VAL + 1, 0); // index 0 unused, 1..1000 used

    for (int val : values) {
        if (val >= 1 && val <= MAX_VAL) {
            freq[val]++;
        }
    }

    int max_freq = *std::max_element(freq.begin(), freq.end());
    return static_cast<int>(values.size()) - max_freq;
}
#include <cassert>
#include <vector>

int minimumRemovals(const std::vector<int>& values); // function under test

int main() {
    assert(minimumRemovals({}) == 0);
    assert(minimumRemovals({7}) == 0);
    assert(minimumRemovals({1, 2, 3, 4}) == 3);
    assert(minimumRemovals({1, 2, 2, 3}) == 2);
    assert(minimumRemovals({5, 5, 5, 5}) == 0);
    assert(minimumRemovals({1, 1, 2, 2, 2}) == 2);
    assert(minimumRemovals({10, 20, 10, 30, 10}) == 2);
    assert(minimumRemovals({999, 1000, 999}) == 1);
    assert(minimumRemovals({1, 1, 1, 1, 2}) == 1);
    assert(minimumRemovals({1000, 1, 1000, 1}) == 2);
    return 0;
}
