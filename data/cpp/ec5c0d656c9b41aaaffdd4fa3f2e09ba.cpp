// Write a C++ function `int maxPairFrequency(int n, const std::vector<int>& arr)` that takes an integer `n` and an array `arr` of length `2*n` representing pairs of socks, where each element is a sock color ID. The socks are given in some arbitrary order, and the goal is to find the maximum number of socks of the same color in the entire collection. For example, if `n = 3` and the array is `[1, 2, 1, 3, 2, 1]`, the color `1` appears three times, `2` appears twice, and `3` appears once, so the answer is `3`. The function must handle duplicate values, any positive integer `n`, and return the maximum frequency of any color ID. The input array may contain any integer values (positive, zero, or negative) as color IDs, and the size of the array is guaranteed to be exactly `2*n`.
// The problem reduces to counting the frequency of each distinct element in the array and returning the largest count. Since the array size is `2*n`, we can iterate through all elements once and use a hash map (or `std::unordered_map`) to store the count of each color ID. For each element, increment its count in the map, and simultaneously track the maximum count seen so far. This avoids a second traversal to find the maximum after building the map. Edge cases include: (1) all socks have the same color, in which case the maximum frequency is `2*n`; (2) all colors are distinct, in which case the maximum frequency is `1`; (3) negative or zero color IDs—these are handled naturally by the map. The time complexity is `O(2*n) = O(n)` for a single pass, and the space complexity is `O(n)` in the worst case (when all colors are distinct). Using an unordered map gives expected constant-time insertion and lookup, so the overall complexity is linear.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum number of socks sharing the same color ID.
// The input array has length 2*n, where each integer is a color ID.
int maxPairFrequency(int n, const std::vector<int>& arr) {
    std::unordered_map<int, int> colorCount;
    int maxFreq = 0;
    for (int sock : arr) {
        int newCount = ++colorCount[sock];
        if (newCount > maxFreq) {
            maxFreq = newCount;
        }
    }
    return maxFreq;
}
#include <cassert>
#include <vector>

// Function declaration (already defined above, but included for clarity)
int maxPairFrequency(int n, const std::vector<int>& arr);

int main() {
    // Basic case: n=3, colors: 1 appears 3 times, 2 appears 2 times, 3 appears 1 time
    assert(maxPairFrequency(3, {1, 2, 1, 3, 2, 1}) == 3);
    // All same color for n=2
    assert(maxPairFrequency(2, {7, 7, 7, 7}) == 4);
    // All distinct for n=2
    assert(maxPairFrequency(2, {1, 2, 3, 4}) == 1);
    // Negative and zero IDs for n=1
    assert(maxPairFrequency(1, {-5, -5}) == 2);
    // Mixed duplicates with zero for n=3
    assert(maxPairFrequency(3, {0, 0, -1, 0, 2, 2}) == 3);
    // Already paired but repeated across multiple pairs
    assert(maxPairFrequency(4, {10, 20, 10, 20, 30, 30, 40, 50}) == 2);
    // Large single color frequency
    assert(maxPairFrequency(5, {42, 42, 42, 42, 42, 42, 42, 42, 42, 42}) == 10);
    // Check with n=1 and one pair
    assert(maxPairFrequency(1, {5, 3}) == 1);
    // Check with n=2 and exactly two colors appearing twice each
    assert(maxPairFrequency(2, {11, 11, 22, 22}) == 2);
    // Check with n=3 and one color appearing 4 times, another 2 times
    assert(maxPairFrequency(3, {9, 8, 9, 9, 8, 9}) == 4);
    return 0;
}
