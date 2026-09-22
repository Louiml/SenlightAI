Given an array of integers `arr` and a target integer `target`, write a C++ function `threeSumMulti` that returns the number of triples `(i, j, k)` with `i < j < k` such that `arr[i] + arr[j] + arr[k] == target`. Since the result can be very large, return it modulo `1,000,000,007`. The array may contain duplicate values, and each occurrence of a value is treated as distinct (so different indices with the same value count as different elements). The function should handle arrays of length up to 10^5, and the time complexity must be better than O(n^3). You may assume `target` fits within a 32-bit signed integer, but the values in `arr` can be negative.

// The core idea is to avoid enumerating all three indices. Instead, we iterate over the array while maintaining a frequency map of previously seen numbers. For each pair `(i, j)` with `j > i`, we compute the needed third value as `target - arr[i] - arr[j]`. Then we add to the answer the number of times that needed value has already appeared among indices `< i` (i.e., the frequency map). After processing all `j` for a fixed `i`, we insert `arr[i]` into the frequency map so that future pairs can use it as the "third" value. This ensures that for every triple, we count it exactly once: when the largest index is treated as `j` and the two smaller indices are `i` and some earlier index represented by the map. Since the map only contains elements with index `< i`, and `j` runs from `i+1` upward, we never double-count permutations; we count each unordered triple (distinct indices) exactly once. Edge cases: if `arr` has fewer than 3 elements, the result is 0; negative numbers are handled naturally; modulo is applied after each accumulation to avoid overflow. Time complexity is O(n^2) because for each i we loop j from i+1 to n-1 and map lookups are O(1) on average. Space complexity is O(n) for the hash map in the worst case.

#include <vector>
#include <unordered_map>

// Returns the number of triples (i<j<k) with arr[i]+arr[j]+arr[k]==target, modulo 1e9+7.
int threeSumMulti(const std::vector<int>& arr, int target) {
    const long long MOD = 1000000007LL;
    int n = static_cast<int>(arr.size());
    if (n < 3) {
        return 0;
    }
    
    std::unordered_map<int, int> seen;
    long long count = 0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long need = static_cast<long long>(target) - arr[i] - arr[j];
            auto it = seen.find(static_cast<int>(need));
            if (it != seen.end()) {
                count += it->second;
                if (count >= MOD) {
                    count %= MOD;
                }
            }
        }
        // After processing all j for i, add arr[i] to the map for future pairs.
        ++seen[arr[i]];
    }
    
    return static_cast<int>(count % MOD);
}

#include <cassert>
#include <vector>

int main() {
    // Basic case
    assert(threeSumMulti({1, 1, 2, 2, 3, 3, 4, 4, 5, 5}, 8) == 20);
    // All identical values
    assert(threeSumMulti({1, 1, 1, 1}, 3) == 4);
    // No triple exists
    assert(threeSumMulti({1, 2, 3}, 100) == 0);
    // Negative numbers
    assert(threeSumMulti({-1, 0, 1, 2, -1, -4}, 0) == 3);
    // Array with fewer than 3 elements
    assert(threeSumMulti({1, 2}, 3) == 0);
    // Larger target and duplicates
    assert(threeSumMulti({0, 0, 0, 0, 0}, 0) == 10);
    // Mixed signs with duplicates
    assert(threeSumMulti({2, -2, 0, 2, -2, 0}, 0) == 8);
    // Modulo check: large count
    std::vector<int> large(100, 1);
    // All triples from 100 identical ones: C(100,3) = 161700, target=3
    assert(threeSumMulti(large, 3) == 161700);
    // Another modulo test with a moderate array
    assert(threeSumMulti({1, 2, 3, 4, 5, 6}, 9) == 2);
    return 0;
}
