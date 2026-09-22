Given a vector of integers where each integer represents the type of fruit in an orchard row, write a C++ function that returns the maximum number of fruits that can be collected into exactly two baskets, with each basket holding only one type of fruit. You may start collecting from any tree and move to the next tree to the right; you must stop once you encounter a third distinct fruit type. The function should handle an empty vector by returning 0, and vectors with one or two distinct types by returning the full length. Implement an efficient sliding-window algorithm.

#include <cassert>
#include <vector>

// forward declaration for the test
int maxFruitsTwoBaskets(const std::vector<int>& fruits);

int main() {
    // Basic cases
    assert(maxFruitsTwoBaskets({}) == 0);
    assert(maxFruitsTwoBaskets({1}) == 1);
    assert(maxFruitsTwoBaskets({1, 1, 1}) == 3);
    assert(maxFruitsTwoBaskets({1, 2, 1}) == 3);

    // Two distinct types throughout
    assert(maxFruitsTwoBaskets({1, 2, 1, 2, 1}) == 5);

    // Three types, must stop at third
    assert(maxFruitsTwoBaskets({1, 2, 3}) == 2);
    assert(maxFruitsTwoBaskets({1, 2, 1, 3, 2}) == 3); // window [1,2,1] length 3, then stop at 3

    // Multiple transitions
    assert(maxFruitsTwoBaskets({3, 3, 3, 1, 2, 1, 1, 2, 3, 3, 4}) == 5); // longest is [2,1,1,2] length 4? Actually check: last burst of two types is [3,3] length 2, but earlier [1,2,1,1,2] length 5? Let's verify: from index3=1,4=2,5=1,6=1,7=2 -> length 5, then at index8=3 breaks. So result 5.
    assert(maxFruitsTwoBaskets({1, 2, 3, 2, 2}) == 4); // window [2,3,2,2] length 4

    return 0;
}

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum number of fruits that can be collected using exactly two baskets.
int maxFruitsTwoBaskets(const std::vector<int>& fruits) {
    if (fruits.empty()) return 0;

    std::unordered_map<int, int> count;  // fruit type -> count in current window
    int left = 0;
    int max_length = 0;

    for (int right = 0; right < static_cast<int>(fruits.size()); ++right) {
        ++count[fruits[right]];

        // Shrink window until at most two distinct fruit types remain.
        while (count.size() > 2) {
            int left_fruit = fruits[left];
            --count[left_fruit];
            if (count[left_fruit] == 0) {
                count.erase(left_fruit);
            }
            ++left;
        }

        max_length = std::max(max_length, right - left + 1);
    }

    return max_length;
}

// The optimal approach uses a sliding window with two pointers (`i` and `j`). We expand the right pointer `j` to include new fruit types while maintaining a map that records the count of each fruit type currently in the window. If the map size exceeds 2, we shrink the window from the left by decrementing or removing the fruit at index `i`, then increment `i`. This ensures the window always contains at most two distinct fruit types. At each step after adjusting the window, we update the maximum window length as `j - i + 1`. Edge cases: empty vector → return 0; vector with one or two distinct types → the window can expand to the entire length; the algorithm must correctly handle cases where a fruit type appears multiple times before shrinking. Time complexity is O(n) since each index is visited at most twice (once by `j` and once by `i`). Space complexity is O(1) because the map stores at most three entries at any time (but is constant-size independent of n).
