/*
Write a C++ function `int getWinner(const std::vector<int>& arr, int k)` that simulates a card‑game tournament where the first element of `arr` is the current champion. In each round, the champion compares itself with the next element in the array: if the next element is larger, it becomes the new champion; otherwise the current champion stays. The champion accumulates one “win” per round (either by defeating a challenger or by successfully defending its title). The function returns the first champion that reaches `k` consecutive wins. If `k` is greater than or equal to the number of elements in the array, the function returns the maximum element of the array (because after enough rounds, the largest element will eventually become champion and never lose again). The input vector is not modified (use `const`). You must not use any special‑case that mutates the input; you may only read it. The array may contain duplicate values, and the first element is not automatically considered to have a win before any round. If `k == 0`, the function should return the first element (since zero wins are already achieved). Assume `arr` is non‑empty and `k >= 0`.
*/

#include <vector>
#include <algorithm>

// Simulate the tournament and return the first winner with k consecutive wins.
// If k is zero, the first element is immediately the winner.
int getWinner(const std::vector<int>& arr, int k) {
    if (k == 0) return arr[0];
    
    // If k is at least the number of elements, the maximum will never lose after becoming champion.
    if (k >= static_cast<int>(arr.size())) {
        return *std::max_element(arr.begin(), arr.end());
    }
    
    int currentChampion = arr[0];
    int consecutiveWins = 0;
    
    for (std::size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > currentChampion) {
            currentChampion = arr[i];
            consecutiveWins = 0;
        }
        ++consecutiveWins;
        if (consecutiveWins == k) {
            return currentChampion;
        }
    }
    
    // If we never reached k, the current champion is the final answer (maximum element).
    return currentChampion;
}

#include <cassert>
#include <vector>

int getWinner(const std::vector<int>& arr, int k); // declaration from solution

int main() {
    // Basic case: 2 wins needed
    std::vector<int> arr1 = {2, 1, 3, 5, 4, 6};
    assert(getWinner(arr1, 2) == 3);
    
    // k larger than array size: return maximum
    std::vector<int> arr2 = {1, 2, 3, 4};
    assert(getWinner(arr2, 5) == 4);
    
    // k == 0: return first element
    std::vector<int> arr3 = {7, 8, 9};
    assert(getWinner(arr3, 0) == 7);
    
    // Monotonically increasing array with k=1: first element beats no one, second wins immediately
    std::vector<int> arr4 = {1, 2, 3};
    assert(getWinner(arr4, 1) == 2);
    
    // Duplicate values: champion stays but earns a win
    std::vector<int> arr5 = {5, 5, 5, 5};
    assert(getWinner(arr5, 3) == 5);
    
    // k exactly equal to array size: return maximum
    std::vector<int> arr6 = {3, 1, 2};
    assert(getWinner(arr6, 3) == 3);
    
    // Champion never loses and reaches k wins before end
    std::vector<int> arr7 = {10, 1, 2, 3};
    assert(getWinner(arr7, 3) == 10);
    
    // Large k but not >= size: loop ends and returns current champion
    std::vector<int> arr8 = {4, 5, 1, 2};
    assert(getWinner(arr8, 5) == 5);
    
    // Single element
    std::vector<int> arr9 = {42};
    assert(getWinner(arr9, 10) == 42);
    
    return 0;
}

// The correct approach simulates the tournament in a single pass over the array from index 1 onward. Maintain a variable `currentChampion` (initialized from `arr[0]`) and a counter `consecutiveWins` (initialized to 0). For each subsequent element, compare it with `currentChampion`. If the new element is larger, update `currentChampion` to that element and reset the counter to 0 (because the champion changed, so the new champion has not yet won any rounds). Then always increment the counter by 1 (this represents the round just completed, regardless of who won). If the counter equals `k`, return `currentChampion`. After the loop ends (all elements processed), return `currentChampion` — this correctly handles cases where `k` is larger than the number of rounds needed to reach the maximum (the maximum will eventually be the champion and will keep winning, so after the loop it is the answer). Edge cases: if `k == 0`, the loop will increment the counter to 1 after the first comparison, so we must return `arr[0]` immediately at the start if `k == 0`. If `k >= arr.size()`, we could also return `*max_element` but the loop naturally handles it because the maximum will become champion and then the loop ends without reaching `k` wins, so it returns `currentChampion` which is the maximum. However, the optimized early return for `k >= arr.size()` is valid and avoids unnecessary work. Time complexity is O(n) in the worst case (single pass), space complexity O(1) extra.
