Write a C++ function `long long countValidPairs(const std::vector<int>& firstArray, const std::vector<int>& secondArray)` that takes two vectors of equal length `n` (1 ≤ n ≤ 10^5), each containing positive integers up to 10^9. The function must return the number of ordered pairs `(i, j)` with `0 ≤ i, j < n` such that `firstArray[i]` is not equal to `secondArray[j]`. For example, if `firstArray = {1, 2, 3}` and `secondArray = {1, 2, 3}`, then the total possible ordered pairs (there are `n^2 = 9`) minus the pairs where `firstArray[i] == secondArray[j]` (there are 3 such pairs: (0,0), (1,1), (2,2)) gives an answer of `6`. The vectors may contain duplicates, and order matters in the pair.

#include <cassert>
#include <vector>

// (Solution function is already defined above)

int main() {
    // Both arrays identical: each i matches exactly one j, so all pairs are invalid.
    std::vector<int> a1 = {1, 2, 3};
    std::vector<int> b1 = {1, 2, 3};
    assert(countValidPairs(a1, b1) == 6);

    // No common values: all n*n pairs are valid.
    std::vector<int> a2 = {1, 2, 3};
    std::vector<int> b2 = {4, 5, 6};
    assert(countValidPairs(a2, b2) == 9);

    // Duplicate values in secondArray.
    std::vector<int> a3 = {1, 1, 2};
    std::vector<int> b3 = {1, 1, 1};
    // total pairs = 9, equal pairs = for each i where val=1, count=3 => 2*3 = 6; val=2 count=0 => answer 3.
    assert(countValidPairs(a3, b3) == 3);

    // Single element arrays.
    std::vector<int> a4 = {5};
    std::vector<int> b4 = {5};
    assert(countValidPairs(a4, b4) == 0);

    // Single element arrays with different values.
    std::vector<int> a5 = {5};
    std::vector<int> b5 = {6};
    assert(countValidPairs(a5, b5) == 1);

    // Larger arrays with mixed matches.
    std::vector<int> a6 = {1, 2, 1, 3};
    std::vector<int> b6 = {1, 4, 1, 2};
    // n=4, total=16, freq: 1->2, 4->1, 2->1. Sum over firstArray: val=1 appears 2 times * freq(1)=2 -> 4; val=2 appears 1 * freq(2)=1 -> 1; val=1 again -> +2; val=3 -> 0. total equal=7, answer=9.
    assert(countValidPairs(a6, b6) == 9);

    return 0;
}

#include <vector>
#include <unordered_map>

// Returns the number of ordered pairs (i, j) where firstArray[i] != secondArray[j].
long long countValidPairs(const std::vector<int>& firstArray, const std::vector<int>& secondArray) {
    const size_t n = firstArray.size();

    // Count occurrences of each value in secondArray.
    std::unordered_map<int, long long> freq;
    for (const int value : secondArray) {
        ++freq[value];
    }

    // Compute total pairs minus pairs where values are equal.
    long long totalPairs = static_cast<long long>(n) * static_cast<long long>(n);
    long long equalPairs = 0;
    for (const int value : firstArray) {
        auto it = freq.find(value);
        if (it != freq.end()) {
            equalPairs += it->second;
        }
    }
    return totalPairs - equalPairs;
}

// The total number of ordered pairs is `n * n`. The number of pairs where the two values are equal can be computed efficiently by counting how many times each value appears in `secondArray`. For each element `x` in `firstArray`, if `x` appears `count[x]` times in `secondArray`, then there are exactly `count[x]` pairs `(i, j)` with `firstArray[i] == secondArray[j]` and a fixed `i`. So we sum `count[firstArray[i]]` over all `i`, and subtract that total from `n * n`. This avoids iterating over all `n^2` pairs. The main edge cases are arrays with all equal values (answer becomes 0), arrays with no matching values (answer becomes `n^2`), and very large counts potentially exceeding 32-bit range, so use `long long`. Time complexity: O(n) to build the frequency map and O(n) to compute the sum, giving O(n). Space complexity: O(n) for the frequency map.
