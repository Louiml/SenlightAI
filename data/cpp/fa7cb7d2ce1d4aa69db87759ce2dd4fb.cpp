/*
Write a C++ function `minBoats(std::vector<int>& weights, int limit)` that takes a vector of people's weights (each weight is a positive integer) and a boat weight limit, and returns the minimum number of boats required to rescue everyone. Each boat can carry at most two people, provided their combined weight does not exceed the limit. The function must modify the input vector (it may sort it in place). Assume the limit is at least as large as the maximum individual weight, so every person can be carried alone if necessary.
*/

#include <vector>
#include <algorithm>

// Return the minimum number of boats needed so that each boat carries at most two people
// and the total weight per boat does not exceed the limit.
int minBoats(std::vector<int>& weights, int limit) {
    if (weights.empty()) return 0;

    std::sort(weights.begin(), weights.end());

    int left = 0;
    int right = static_cast<int>(weights.size()) - 1;
    int boats = 0;

    while (left < right) {
        // Try to pair the lightest and heaviest remaining person.
        if (weights[left] + weights[right] <= limit) {
            ++left;  // Both are assigned to a boat.
        }
        // The heaviest person must go alone.
        ++boats;
        --right;
    }

    // If exactly one person remains, they need their own boat.
    if (left == right) {
        ++boats;
    }

    return boats;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> w1 = {3, 2, 2, 1};
    assert(minBoats(w1, 3) == 3);

    std::vector<int> w2 = {1, 2, 3, 4};
    assert(minBoats(w2, 5) == 2);

    std::vector<int> w3 = {5, 5, 5, 5};
    assert(minBoats(w3, 5) == 4);

    std::vector<int> w4 = {1, 1, 1, 1};
    assert(minBoats(w4, 2) == 2);

    std::vector<int> w5 = {7};
    assert(minBoats(w5, 7) == 1);

    std::vector<int> w6 = {};
    assert(minBoats(w6, 10) == 0);

    std::vector<int> w7 = {3, 2, 2, 1};
    assert(minBoats(w7, 5) == 2);

    std::vector<int> w8 = {1, 2, 4, 5, 6};
    assert(minBoats(w8, 6) == 3);

    std::vector<int> w9 = {10, 1, 9, 2, 8, 3};
    assert(minBoats(w9, 11) == 3);

    std::vector<int> w10 = {2, 4, 6, 8};
    assert(minBoats(w10, 10) == 3);
}

// The core idea is a greedy two-pointer approach after sorting the weights in ascending order. Since each boat holds at most two people, the optimal strategy is to always try to pair the lightest remaining person with the heaviest remaining person. Place a left pointer at the start and a right pointer at the end. If the sum of the two pointed-to weights is ≤ the limit, we pair them, moving both pointers inward. If the sum exceeds the limit, the heaviest person cannot be paired with anyone (because everyone else is at least as heavy as the lightest), so they must go alone—move only the right pointer. Each boat corresponds to one pair or one solo trip, so increment the boat count each iteration. The loop continues while left < right. After the loop, if left == right, one unpaired person remains, requiring one extra boat. Edge cases include an empty vector (return 0), a single person (return 1), and cases where many people must travel alone. Sorting takes O(n log n) time; the two-pointer pass is O(n). Auxiliary space is O(1) besides the input vector's storage.
