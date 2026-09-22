// Write a C++ function that takes a non-empty vector of integers representing circularly sampled data (where the first and last elements are considered adjacent) and returns the number of "peaks" — elements that are strictly greater than both neighboring elements or strictly less than both neighboring elements. Each element's neighbors are the immediately preceding and following elements in the vector, except for the first element (whose "before" neighbor is the last element) and the last element (whose "after" neighbor is the first element). The function must count how many elements satisfy this local extremum condition. For example, in the vector `{1, 3, 2, 0}`, the elements 3 (greater than 1 and 2) and 0 (less than 2 and 1) are peaks, so the function returns 2. In `{5, 5, 5}`, no element is strictly greater or less than both neighbors, so returns 0.

#include <cassert>
#include <vector>

int main() {
    assert(countCircularPeaks({1, 3, 2, 0}) == 2);
    assert(countCircularPeaks({5, 5, 5}) == 0);
    assert(countCircularPeaks({1}) == 0);
    assert(countCircularPeaks({1, 2}) == 0);
    assert(countCircularPeaks({1, 2, 3}) == 1); // 3 is peak (>2, >1)
    assert(countCircularPeaks({3, 2, 1}) == 1); // 3 is peak (>2, >1) 
    assert(countCircularPeaks({1, 2, 1, 2, 1}) == 3); // indices 1,2,3 are extrema
    assert(countCircularPeaks({-1, 0, -1, 0, -1}) == 3); // same pattern with negatives
    assert(countCircularPeaks({0, 10, 20, 10, 0, 10}) == 3); // 20 and the two 0s
    assert(countCircularPeaks({1, 1, 2, 1, 1}) == 2); // 2 is peak, and the last 1 is valley
    return 0;
}

#include <vector>

// Count the number of circular peaks (strict local extrema) in a vector.
int countCircularPeaks(const std::vector<int>& samples) {
    int n = static_cast<int>(samples.size());
    int peaks = 0;
    for (int i = 0; i < n; ++i) {
        int before = (i - 1 + n) % n;
        int after = (i + 1) % n;
        if ((samples[i] > samples[before] && samples[i] > samples[after]) ||
            (samples[i] < samples[before] && samples[i] < samples[after])) {
            ++peaks;
        }
    }
    return peaks;
}

// The solution iterates over each index `i` in the range `[0, n)` where `n` is the vector size. For each index, compute the index of the previous element as `(i - 1 + n) % n` and the next element as `(i + 1) % n`. This handles the circular wrap‑around automatically, including when `n == 1` (in that case, both neighbors are the element itself, so neither strict inequality holds, resulting in 0 peaks). Then check if the current element is strictly greater than both neighbors or strictly less than both neighbors. If either condition holds, increment the peak count. The algorithm uses a single pass over all elements, so time complexity is O(n), and space complexity is O(1) beyond the input vector (which is passed by const reference, so no copy is made). No special handling of duplicate values is needed because the comparisons are strict. Edge cases include vectors of size 1 (always 0 peaks), size 2 (each element's two neighbors are the other element and itself, so the element cannot be strictly greater or less than itself, thus 0 peaks), and all-equal elements (0 peaks).
