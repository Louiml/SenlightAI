Write a C++ function `int balanceArray(const std::vector<int>& arr)` that takes a non-empty array of integers and returns the minimum number of elements that must remain after repeatedly performing the following operation any number of times (possibly zero): choose any two distinct elements that are not equal in value and remove both of them. Equivalent to: you may delete pairs of different values; you want to minimize the final number of remaining elements. The array may contain up to 10^5 elements, and each element fits in a 32-bit signed integer. The result is always the same regardless of deletion order; your function must compute the final count.
The key observation is that the only thing that matters is the frequency of the most common value. If the most frequent value appears `mx` times, then every deletion removes one instance of that most frequent value along with one instance of some other value. If `mx` is greater than half the array size, then after pairing every non-majority element with a majority element, `mx - (n - mx)` majority elements remain, which is `n - 2*(n - mx)`. If no value occurs more than half the time, then we can always pair elements of different values until at most one element remains; whether one remains depends on parity: if `n` is odd, one element is left; if even, zero remain. Thus the result is `(n & 1)` when `mx <= n/2`; otherwise `n - 2*(n - mx)`. Edge cases include single-element arrays (returns 1), arrays with all equal values (returns `n`), and arrays where `mx == n/2` exactly (falls into the even case, returns 0 if `n` even, 1 if `n` odd). Time complexity is O(n) for counting frequencies using a hash map, and space O(n) in the worst case.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the minimum number of elements left after repeatedly removing two distinct values.
int balanceArray(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    std::unordered_map<int, int> freq;
    int max_freq = 0;
    for (const int value : arr) {
        max_freq = std::max(max_freq, ++freq[value]);
    }
    if (max_freq > n / 2) {
        return n - 2 * (n - max_freq);
    }
    return n & 1;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (assumed to be included from above)
int balanceArray(const std::vector<int>& arr);

int main() {
    // Single element
    assert(balanceArray({5}) == 1);
    // All equal
    assert(balanceArray({2, 2, 2, 2}) == 4);
    // Majority value appears 3 of 4
    assert(balanceArray({1, 1, 1, 2}) == 2);
    // No majority, even length
    assert(balanceArray({1, 2, 3, 4}) == 0);
    // No majority, odd length
    assert(balanceArray({1, 2, 3}) == 1);
    // Majority exactly half (n even)
    assert(balanceArray({1, 1, 2, 2}) == 0);
    // Majority exactly half (n odd, impossible, but test close case)
    assert(balanceArray({1, 1, 1, 2, 2}) == 1);
    // Large majority
    assert(balanceArray({7, 7, 7, 7, 7, 1, 2}) == 3);
    // Negative and duplicate values
    assert(balanceArray({-3, -3, -3, -3, -3, 0, 5}) == 3);
    // Two distinct values with count 3 and 2
    assert(balanceArray({1, 1, 1, 2, 2}) == 1);
    return 0;
}
