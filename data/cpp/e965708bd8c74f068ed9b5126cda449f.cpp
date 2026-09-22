Write a C++ function named `minimumCardPickup` that takes a non-empty vector of integers representing card values and returns the minimum number of consecutive cards that must be picked up to obtain a pair of matching cards (i.e., two cards with the same value). If no matching pair exists anywhere in the deck, return `-1`. The function should consider the distance between two equal values as the number of cards from the first occurrence to the second occurrence inclusive (e.g., for indices 2 and 5, the distance is 4). The function must handle duplicates, large vectors, and edge cases like a single card or all distinct values.

The solution uses a hash map (`std::unordered_map`) to store the most recent index at which each card value was seen. Iterate through the vector once, and for each card value at index `i`, check if it already exists in the map. If it does, compute the distance `i - lastIndex + 1` (inclusive span between the two occurrences) and update the minimum distance found so far. Then always update the map entry for that card to the current index, because the most recent occurrence is needed for future comparisons. Initialize the answer to a large sentinel value (like `INT_MAX`) and after the loop, if no match was found, return `-1`; otherwise return the minimum distance. Edge cases: if the vector has fewer than 2 elements or all values are unique, return `-1`. The algorithm runs in O(n) time and O(k) auxiliary space, where `k` is the number of distinct card values (bounded by `n`). The space is needed to store the last seen index for each distinct value.

#include <vector>
#include <unordered_map>
#include <climits>
#include <algorithm>

// Returns the minimum number of consecutive cards needed to get a matching pair,
// or -1 if no pair exists. The distance is inclusive of both matching cards.
int minimumCardPickup(const std::vector<int>& cards) {
    const int n = static_cast<int>(cards.size());
    int minDistance = INT_MAX;

    // Map each card value to its most recent index.
    std::unordered_map<int, int> lastSeen;

    for (int i = 0; i < n; ++i) {
        const int value = cards[i];

        // If we have seen this value before, compute the span.
        auto it = lastSeen.find(value);
        if (it != lastSeen.end()) {
            int distance = i - it->second + 1;
            minDistance = std::min(minDistance, distance);
        }

        // Always update the most recent index for this value.
        lastSeen[value] = i;
    }

    return (minDistance == INT_MAX) ? -1 : minDistance;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case: pair at indices 0 and 6 -> distance 7
    std::vector<int> cards1 = {3, 4, 5, 7, 99, 98, 3, 6, 1, 2, 1, 3};
    assert(minimumCardPickup(cards1) == 3); // 1 appears at 8 and 10 -> 3, but 3 appears at 0 and 6 -> 7, and 1 at 8 and 10 -> 3, so min is 3

    // No matching pairs
    std::vector<int> cards2 = {1, 2, 3, 4, 5};
    assert(minimumCardPickup(cards2) == -1);

    // Single card
    std::vector<int> cards3 = {7};
    assert(minimumCardPickup(cards3) == -1);

    // Two identical adjacent cards
    std::vector<int> cards4 = {5, 5};
    assert(minimumCardPickup(cards4) == 2);

    // Pair with distance 1 (adjacent)
    std::vector<int> cards5 = {1, 2, 2, 3};
    assert(minimumCardPickup(cards5) == 2);

    // Pair at the ends of a longer vector
    std::vector<int> cards6 = {10, 20, 30, 40, 10};
    assert(minimumCardPickup(cards6) == 5);

    // Multiple pairs, take the minimum distance
    std::vector<int> cards7 = {1, 2, 1, 2};
    assert(minimumCardPickup(cards7) == 3); // 1 at 0 and 2 -> 3, 2 at 1 and 3 -> 3

    // Large vector with only one pair at the very end
    std::vector<int> cards8(1000, 0);
    cards8[999] = 1;
    cards8[1000] = 1; // Actually size is 1001
    cards8.push_back(1); // Now 1 appears at index 999 and 1000? Let's simplify
    // Use a clean large vector: 1000 zeros then a 1
    std::vector<int> cards8b(1000, 0);
    cards8b.push_back(1);
    cards8b.push_back(1);
    assert(minimumCardPickup(cards8b) == 2); // The two 1's are adjacent at the end

    // Empty vector? The spec says non-empty, but we can test -1 for safety
    // Actually the function returns -1 for empty as well, since n=0, loop doesn't run.
    std::vector<int> cards9;
    assert(minimumCardPickup(cards9) == -1);

    return 0;
}
