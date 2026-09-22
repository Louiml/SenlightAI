// Write a C++ function that takes a vector of integers (which may contain zeros, negative numbers, and duplicates) and returns `true` if the array can be reordered so that every element at an even index is exactly half of the following element at the odd index (i.e., for every pair `(a, b)` where `b = 2*a`), and `false` otherwise. The pairs must be formed by rearranging the array arbitrarily, but each element must be used exactly once in exactly one pair. For example, `[1,2,1,2]` is valid (pairs: `(1,2)` and `(1,2)`), but `[1,2,4]` is not because 4 would need a partner 8 or 2, but 2 is already used. Also handle zeros: a zero can only pair with another zero (since 0 = 2*0). The function must be efficient for arrays up to length 100,000.
// The key insight is to count frequencies of each number using a hash map. The pairing condition `b = 2*a` is symmetric for negative numbers but not for zero. For zero, it pairs only with zero, so the count of zeros must be even. For all other numbers, sort the distinct values by their absolute value, processing from smallest absolute value to largest. For each value `k` (with count `cnt`), since we process in increasing absolute value, any `k` that is a double of another value will appear later, and any `k` that is half of another value will have been processed earlier. So we need `freq[2*k] >= cnt`, and then we subtract `cnt` from `freq[2*k]` to account for the pairs. If at any point this fails, return `false`. This works because the absolute value ordering ensures that if `|a| < |b|`, then `b` cannot be half of `a` (unless a=0, handled separately). For negative numbers, doubling flips sign but absolute value doubles, so the absolute-order still works. Time complexity is O(n log n) due to sorting distinct keys (at most n keys), and space O(n) for the hash map and key vector.
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <cstdlib>

// Returns true if the array can be rearranged into pairs (a, 2*a) for every element.
bool canReorderDoubled(const std::vector<int>& arr) {
    std::unordered_map<int, int> freq;
    for (int v : arr) {
        ++freq[v];
    }
    // Zero must pair with zero, so count must be even.
    if (freq[0] % 2 != 0) return false;

    // Collect distinct keys.
    std::vector<int> keys;
    for (const auto& [k, _] : freq) {
        keys.push_back(k);
    }

    // Sort by absolute value ascending.
    std::sort(keys.begin(), keys.end(),
              [](int a, int b) { return std::abs(a) < std::abs(b); });

    for (int k : keys) {
        if (k == 0) continue; // Already handled.
        int need = freq[k];
        if (need == 0) continue; // Already consumed as a double of another.
        int partner = k * 2;
        if (freq[partner] < need) {
            return false;
        }
        freq[partner] -= need;
        freq[k] = 0; // Mark as used.
    }
    return true;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Basic valid cases
    assert(canReorderDoubled({1,2,1,2}) == true);
    assert(canReorderDoubled({4,-2,2,-4}) == true);
    assert(canReorderDoubled({0,0}) == true);
    assert(canReorderDoubled({2,4,0,0,1,2}) == true);

    // Invalid cases
    assert(canReorderDoubled({1,2,4}) == false);
    assert(canReorderDoubled({1,2,4,8}) == false); // needs 4,8 pair but 4 already used
    assert(canReorderDoubled({0,0,0}) == false);   // odd zeros
    assert(canReorderDoubled({3,1,3,6}) == false); // 3 needs 6, but only one 6
    assert(canReorderDoubled({-1,-2,-2}) == false); // -1 needs -2, but two -2s need partners

    // Edge: single element
    assert(canReorderDoubled({5}) == false);
    assert(canReorderDoubled({0}) == false);

    // Large values
    assert(canReorderDoubled({100000,200000,100000,200000}) == true);

    // Mixed signs
    assert(canReorderDoubled({-4,-2,2,4}) == true);
    assert(canReorderDoubled({-1,1}) == false);

    return 0;
}
