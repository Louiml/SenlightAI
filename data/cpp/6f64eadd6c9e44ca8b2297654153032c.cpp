Write a C++ function `distributeCandies(int candies, int num_people)` that simulates distributing `candies` to `num_people` people sitting in a circle, where the first person gets 1 candy, the second gets 2, and so on, increasing by one each turn. After giving candies to the last person (index `num_people-1`), the next turn returns to the first person. If the remaining candies are less than the amount due for the current turn, give all remaining candies to that person and stop. The function should return a `std::vector<int>` of length `num_people` representing the total candies each person received. Use a loop that continues until all candies are gone, and handle the edge case where `candies` is 0 (return a vector of all zeros) and where `num_people` is positive. The solution must be efficient for any number of candies up to `10^9` and any `num_people` up to `1000`.

The algorithm simulates the distribution turn by turn. We maintain an index `i` starting at 0 (representing the current turn number, where the amount to give is `i+1`). For each iteration, we give `min(candies, i+1)` to the person at position `i % num_people` in the answer vector. We subtract the given amount from `candies` (note: we subtract `i+1` even if we only gave fewer because the loop condition `candies > 0` will stop appropriately—but we must subtract the actual given amount to avoid negative values; the original snippet uses `candies -= i+1` which works because the loop stops when `candies <= 0`, and `min(candies, i+1)` ensures we never give more than available; however, to be safe, subtract `given` not `i+1`). Then increment `i`. This effectively simulates the process. Edge cases: if `candies == 0`, the loop doesn't run and the vector is all zeros. If `num_people` is 1, all candies are given to the single person across turns—the loop works because `i % 1` is always 0. Time complexity is O(number of turns) = O(ceil(sqrt(2*candies))) because the total candies given after k turns is k(k+1)/2, so k is roughly sqrt(2*candies). For candies up to 10^9, that's about 44721 turns, which is fine. Space complexity is O(num_people) for the result vector.

#include <vector>
#include <algorithm>

// Distribute candies to num_people in a circle, giving increasing amounts each turn.
// Returns a vector where result[i] is the total candies received by person i.
std::vector<int> distributeCandies(int candies, int num_people) {
    std::vector<int> result(num_people, 0);
    int turn = 0; // turn starts at 0, amount given is turn + 1
    while (candies > 0) {
        int give = std::min(candies, turn + 1);
        result[turn % num_people] += give;
        candies -= give;
        ++turn;
    }
    return result;
}

#include <cassert>
#include <vector>

// Declaration (assuming the solution is in the same translation unit)
std::vector<int> distributeCandies(int candies, int num_people);

int main() {
    assert(distributeCandies(7, 4) == std::vector<int>({1, 2, 3, 1}));
    assert(distributeCandies(10, 3) == std::vector<int>({5, 2, 3}));
    assert(distributeCandies(0, 5) == std::vector<int>({0, 0, 0, 0, 0}));
    assert(distributeCandies(1, 1) == std::vector<int>({1}));
    assert(distributeCandies(10, 1) == std::vector<int>({10}));
    assert(distributeCandies(15, 6) == std::vector<int>({1, 2, 3, 4, 5, 0}));
    assert(distributeCandies(60, 4) == std::vector<int>({15, 18, 15, 12}));
    assert(distributeCandies(1000000000, 1000).size() == 1000);
    // Spot check total distribution equals original candies
    auto v = distributeCandies(12345, 7);
    int total = 0;
    for (int x : v) total += x;
    assert(total == 12345);
    return 0;
}
