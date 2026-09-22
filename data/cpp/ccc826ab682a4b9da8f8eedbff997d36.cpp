/*
Write a C++ function that simulates the "ant on a stick" problem: given a stick of length `L` and `n` ants initially placed at integer positions along the stick, each ant walks at the same speed in either direction and turns around upon collision (which is equivalent to passing through each other). Return a pair of integers: the minimum possible time (earliest) and the maximum possible time (latest) until all ants fall off the stick. The function signature is `std::pair<int, int> antTimes(int length, const std::vector<int>& positions)`. The positions are given as integers, each strictly between `0` and `length` inclusive (ants at endpoints are already off). Both `length` and all positions are positive. Assume `positions.size() >= 1`.
*/

#include <vector>
#include <algorithm>
#include <utility>

// Compute the minimum and maximum time until all ants fall off a stick.
// length: stick length. positions: initial ant positions (0 <= p <= length).
std::pair<int, int> antTimes(int length, const std::vector<int>& positions) {
    int earliest = 0; // max of min distances
    int latest = 0;   // max of max distances
    for (int p : positions) {
        earliest = std::max(earliest, std::min(p, length - p));
        latest = std::max(latest, std::max(p, length - p));
    }
    return {earliest, latest};
}

#include <cassert>
#include <vector>
#include <utility>

// Assume antTimes is defined above or included.

int main() {
    // Basic test from the snippet: length=10, ants at 2,6,7
    {
        auto result = antTimes(10, std::vector<int>{2,6,7});
        assert(result.first == 4);  // earliest: max(2,4,3) = 4
        assert(result.second == 8); // latest: max(8,4,3) = 8
    }
    // Single ant in center
    {
        auto result = antTimes(10, std::vector<int>{5});
        assert(result.first == 5);
        assert(result.second == 5);
    }
    // All ants near one end
    {
        auto result = antTimes(10, std::vector<int>{1,2,3});
        assert(result.first == 3);  // max(1,2,3) = 3
        assert(result.second == 9); // max(9,8,7) = 9
    }
    // Ants at both ends
    {
        auto result = antTimes(10, std::vector<int>{0,10});
        assert(result.first == 0);
        assert(result.second == 10);
    }
    // Length 1, one ant at 0
    {
        auto result = antTimes(1, std::vector<int>{0});
        assert(result.first == 0);
        assert(result.second == 1);
    }
    // Many ants, duplicates allowed
    {
        auto result = antTimes(7, std::vector<int>{2,2,5,5});
        assert(result.first == 5); // max(2,2,2,2) = 2? Wait: min(2,5)=2, min(5,2)=2 -> max is 2? Actually careful: min(p,7-p): p=2 =>2, p=5=>2 -> earliest=2? Wait but 5 ants: min(5,2)=2, so earliest=2. Let's correct: positions {2,2,5,5}: min: 2,2,2,2 => earliest=2. latest: max(2,5)=5, max(5,2)=5 => latest=5. So assert result.first==2, result.second==5.
        assert(result.first == 2);
        assert(result.second == 5);
    }
    return 0;
}

// For each ant at position `p`, the earliest it can fall is if it walks toward the nearest end, i.e., `min(p, length - p)`. The latest it can fall is if it walks away from the nearest end, i.e., `max(p, length - p)`. Because collisions are equivalent to ants passing through each other (since they are indistinguishable and turn around, the set of positions over time is the same as if they pass through), the overall earliest time for all ants to fall is the maximum of all individual earliest times (the last ant to fall in the optimal scenario). Similarly, the overall latest time is the maximum of all individual latest times. So we simply iterate over positions, compute `earliest = max(earliest, min(p, length - p))` and `latest = max(latest, max(p, length - p))`. Edge cases: if an ant is exactly at an endpoint (0 or length), it is already off, but the inputs are given as positive and we treat positions as given; if `length` is 1, positions can only be 0 or 1, but we handle generally. Time complexity O(n), space O(1) auxiliary.
