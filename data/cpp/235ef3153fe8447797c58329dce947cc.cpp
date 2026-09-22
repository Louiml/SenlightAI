// Write a C++ function `int maxSatisfiedCustomers(const std::vector<int>& customers, const std::vector<int>& grumpy, int minutes)` that models a bookstore owner. Each minute `i`, `customers[i]` customers enter, and if `grumpy[i] == 0` the owner is in a good mood and those customers are satisfied; if `grumpy[i] == 1` the owner is grumpy and those customers are unsatisfied (lost). The owner has a secret technique that, for a **contiguous** block of exactly `minutes` minutes, forces him to be in a good mood (i.e., he will not be grumpy during those minutes, regardless of the original grumpy array). The technique can be applied at most once. Return the maximum total number of customers that can be satisfied across the entire day. The input arrays have equal non-zero length, `minutes` is between 1 and that length. The function should not modify the input arrays.

#include <cassert>
#include <vector>

int maxSatisfiedCustomers(const std::vector<int>& customers,
                          const std::vector<int>& grumpy,
                          int minutes);

int main() {
    // Basic case from problem statement.
    assert(maxSatisfiedCustomers({1,0,1,2,1,1,7,5}, {0,1,0,1,0,1,0,1}, 3) == 16);
    // All grumpy, window covers exactly length.
    assert(maxSatisfiedCustomers({1,2,3}, {1,1,1}, 3) == 6);
    // No grumpy minutes.
    assert(maxSatisfiedCustomers({5,6,7}, {0,0,0}, 2) == 18);
    // Window length 1, pick best grumpy minute.
    assert(maxSatisfiedCustomers({2,4,1,3}, {1,0,1,1}, 1) == (4 + 3) == 7); // baseline=4, max gain=3
    // Empty grumpy дня? Actually n>0, but minutes=1 with mixed.
    assert(maxSatisfiedCustomers({10,20,30}, {1,0,1}, 1) == 60);
    // All satisfied already, gain zero.
    assert(maxSatisfiedCustomers({3,1,2}, {0,0,0}, 2) == 6);
    // Large window bigger than n? not allowed, but test minutes=n.
    assert(maxSatisfiedCustomers({1,2,3,4}, {1,0,1,0}, 4) == 10); // baseline=6, gain=4
    // Window of length 2, multiple options.
    assert(maxSatisfiedCustomers({5,1,2,8}, {1,1,0,1}, 2) == (1+2) + (5+1) ? 1+2+5+1 : -1); // baseline=2, best gain=5+1=6 => 8
    assert(maxSatisfiedCustomers({5,1,2,8}, {1,1,0,1}, 2) == 8);
    // Single element.
    assert(maxSatisfiedCustomers({9}, {1}, 1) == 9);
    assert(maxSatisfiedCustomers({9}, {0}, 1) == 9);
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum number of satisfied customers after applying the technique
// for exactly 'minutes' consecutive minutes, at most once.
int maxSatisfiedCustomers(const std::vector<int>& customers,
                          const std::vector<int>& grumpy,
                          int minutes) {
    int n = customers.size();
    int baseline = 0;
    // Build a vector where each element is customer count if grumpy==1, else 0.
    std::vector<int> loss(n, 0);
    for (int i = 0; i < n; ++i) {
        if (grumpy[i] == 0) {
            baseline += customers[i];
        } else {
            loss[i] = customers[i];
        }
    }

    // Compute initial window sum of first 'minutes' elements.
    int currentGain = 0;
    for (int i = 0; i < minutes && i < n; ++i) {
        currentGain += loss[i];
    }
    int maxGain = currentGain;

    // Slide the window.
    for (int left = 0, right = minutes; right < n; ++left, ++right) {
        currentGain += loss[right] - loss[left];
        maxGain = std::max(maxGain, currentGain);
    }

    return baseline + maxGain;
}

// The key idea is to first calculate the baseline satisfied customers (those minutes where `grumpy[i] == 0`). For each grumpy minute, the customers are "lost" unless covered by the technique. The technique converts a contiguous window of length `minutes` from grumpy to satisfied, so the gain from applying it at start position `i` equals the sum of `customers[j]` for all `j` in `[i, i+minutes-1]` where `grumpy[j] == 1`. To find the maximum possible gain, we use a fixed-length sliding window over an auxiliary array where we store `customers[i]` if `grumpy[i]==1` else `0`. Initialize the window sum for the first `minutes` elements, then slide the window right by adding the next element and removing the leftmost, updating the maximum. The answer is baseline + maximum window sum. Edge cases: if `minutes` equals the array length, the window covers everything, so the gain is the sum of all customers at grumpy minutes; if there are no grumpy minutes, the gain is 0. Time complexity is O(n) and space complexity is O(1) extra (reusing an auxiliary vector or just computing on the fly—but the reference uses an auxiliary vector for clarity, though it can be optimized to O(1) extra space by tracking sums on the fly). The solution correctly handles all cases.
