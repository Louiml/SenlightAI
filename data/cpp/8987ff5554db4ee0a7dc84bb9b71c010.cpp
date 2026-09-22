/*
Write a C++ function named `maxDistinctPairs` that takes a non-empty vector of integers (`std::vector<int>`) and returns the maximum number of distinct pairs that can be formed from the elements, where each pair consists of two different values. Specifically, given an even-sized vector (size is always even and ≥ 2), you may pair up elements arbitrarily, but each pair must contain two numbers with different values (no pair may have two equal numbers). The function should return the largest possible number of such valid pairs. If it is impossible to form any valid pair (e.g., all elements are identical), return 0. The input may contain negative numbers, zero, and duplicates. The vector size is always even.
*/
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum number of pairs that can be formed, where each pair
// must contain two distinct values. The input vector size is even and >= 2.
int maxDistinctPairs(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return 0; // though spec says non-empty, safe guard

    // Count frequencies and find the maximum frequency.
    std::unordered_map<int, int> freq;
    int maxFreq = 0;
    for (int x : nums) {
        int current = ++freq[x];
        maxFreq = std::max(maxFreq, current);
    }

    // Maximum pairs cannot exceed n/2, and also cannot exceed n - maxFreq,
    // because each pair needs two distinct values.
    return std::min(n / 2, n - maxFreq);
}
#include <cassert>

int main() {
    // Basic cases
    assert(maxDistinctPairs({1, 2, 3, 4}) == 2);
    assert(maxDistinctPairs({1, 1, 2, 2}) == 2);
    assert(maxDistinctPairs({1, 1, 1, 2}) == 1);
    assert(maxDistinctPairs({1, 1, 1, 1}) == 0);

    // Negative and zero values
    assert(maxDistinctPairs({-1, -1, 0, 0, 1, 1}) == 3);
    assert(maxDistinctPairs({-5, -5, -5, -5}) == 0);

    // Larger vector with one dominant value
    assert(maxDistinctPairs({1, 1, 1, 1, 1, 2, 3, 4}) == 3); // n=8, maxFreq=5, n-maxFreq=3

    // All distinct values (even size)
    assert(maxDistinctPairs({10, 20, 30, 40, 50, 60}) == 3);

    // Duplicates but balanced
    assert(maxDistinctPairs({7, 7, 7, 8, 8, 9}) == 3); // n=6, maxFreq=3, n-maxFreq=3
}
// The key insight is that the maximum number of valid pairs is limited by two constraints: (1) the total number of pairs we can form is `n / 2` (where `n` is the vector size), and (2) we cannot pair two equal numbers together, so each distinct value can contribute at most one "side" of a pair per occurrence, but since duplicates of the same value cannot pair with each other, the limiting factor is how many distinct values we have. In fact, if we have `d` distinct values, the most pairs we can form is `min(n/2, d)` because we need two different values per pair, and with `d` distinct values, we can match each distinct value against another different one, but once we use a value in a pair, we can reuse it if we have multiple copies. However, a simpler reasoning: the maximum possible pairs is the minimum of `n/2` and the number of distinct values. This works because if there are at least `n/2` distinct values, we can pair each copy with a different value (e.g., sort and pair extremes). If there are fewer distinct values than pairs needed, some pairs must be invalid because we'd be forced to pair equal values. The algorithm: insert all elements into a `std::set` to count distinct values, then return `min(n/2, set.size())`. Edge cases: if `n=2` and both numbers equal, set size is 1, min(1,1)=1? Wait, but that would be a pair of equal numbers which is invalid. Actually let’s reconsider: For `n=2` with `[1,1]`, set size is 1, `min(1,1)=1` but that pair is invalid. So the correct formula is `min(n/2, d)` gives 1, but we need 0. So the correct approach: the number of valid pairs cannot exceed the number of elements minus the maximum frequency of any single value, because each pair needs two different values. More precisely, the maximum pairs is `min(n/2, n - maxFreq)`. Because if one value appears `f` times, you can pair at most `n - f` of those copies with others. But actually a known result: maximum number of pairs with distinct values is `min(n/2, n - maxFreq)`. Let's derive: If the most frequent element appears `f` times, then the remaining `n - f` elements are all different from it, so you can pair at most `n - f` of the frequent ones with them, giving at most `n - f` pairs overall (if `n-f` ≤ n/2), otherwise limited by n/2. So answer = `min(n/2, n - maxFreq)`. For `[1,1]`, `f=2`, `n-f=0`, min(1,0)=0. For `[1,2,3,4]`, `f=1`, `n-f=3`, min(2,3)=2. For `[1,1,2,2,3,3]`, `f=2`, `n-f=4`, min(3,4)=3. That works. So we need to compute the maximum frequency of any value. We can do this with a `std::map` or `unordered_map`. Time complexity O(n) average, O(n log n) worst for map. Space O(n). Edge cases: vector size ≥2 always even, but we handle any even size. If all elements same, answer 0. Implementation: use `unordered_map<int,int>` to count frequencies, track max frequency, then return `min(n/2, n - maxFreq)`. Since the original snippet used a set but was incorrect for the edge case, this task specifically requires the correct solution.
