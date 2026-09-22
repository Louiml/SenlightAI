Given an integer array `nums` and an integer `k`, write a C++ function `int maximumScore(vector<int>& nums, int k)` that returns the maximum possible score modulo \(10^9+7\). You may perform the following operation at most `k` times: choose any index `i` in the array, and multiply the current score by `nums[i]`. The array is immutable (you cannot change elements), but you can choose the same index multiple times. The catch is that you want to maximize the product, so you should only choose indices that have the highest value among those you are allowed to pick. However, the operation is further constrained: after you choose an index `i`, you may only choose it again if it becomes the "maximum" among a certain subarray determined by prime-factor scores. Specifically, each element has a "prime score" equal to the number of distinct prime factors of `nums[i]`. For each index `i`, define `left[i]` as the nearest index to the left such that `primeScore[left[i]] >= primeScore[i]` (if none, use -1), and `right[i]` as the nearest index to the right such that `primeScore[right[i]] > primeScore[i]` (if none, use `n`). The number of times you may select index `i` is exactly `(right[i] - i) * (i - left[i])`. You must choose indices in decreasing order of their value (ties arbitrary), and you can only perform the multiplication up to `k` times total, selecting each index at most its allowed count. Return the product of all selected `nums[i]` modulo \(10^9+7\).

#include <cassert>
#include <vector>

int main() {
    Solution sol;
    
    // Test 1: Basic case
    std::vector<int> nums1 = {8, 3, 9, 3, 8};
    assert(sol.maximumScore(nums1, 2) == 81);  // 9 * 9 = 81
    
    // Test 2: k larger than possible selections
    std::vector<int> nums2 = {19, 12, 14, 6, 25};
    assert(sol.maximumScore(nums2, 10) == 3761280); // 25*25*19*14*14*...
    
    // Test 3: Single element
    std::vector<int> nums3 = {5};
    assert(sol.maximumScore(nums3, 3) == 125); // 5*5*5 = 125
    
    // Test 4: k = 0
    std::vector<int> nums4 = {1, 2, 3};
    assert(sol.maximumScore(nums4, 0) == 1);
    
    // Test 5: All same values
    std::vector<int> nums5 = {2, 2, 2};
    assert(sol.maximumScore(nums5, 4) == 16); // 2*2*2*2 = 16
    
    // Test 6: Large number to check modulo
    std::vector<int> nums6 = {100000, 100000};
    assert(sol.maximumScore(nums6, 1) == 100000);
    
    // Test 7: Empty vector
    std::vector<int> nums7;
    assert(sol.maximumScore(nums7, 5) == 1);
    
    // Test 8: Mixed prime scores
    std::vector<int> nums8 = {4, 6, 15, 35};
    assert(sol.maximumScore(nums8, 3) == 35 * 35 * 15 % 1000000007); // 18375
    
    // Test 9: Edge case with k=1
    std::vector<int> nums9 = {5, 3, 5};
    assert(sol.maximumScore(nums9, 1) == 5);
    
    // Test 10: Two different values
    std::vector<int> nums10 = {7, 11, 13};
    assert(sol.maximumScore(nums10, 2) == 13 * 13); // 169
    
    return 0;
}

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <cstdint>

class Solution {
public:
    int maximumScore(std::vector<int>& nums, int k) {
        const int MOD = 1000000007;
        int n = static_cast<int>(nums.size());
        if (n == 0) return 1;
        
        // Compute prime scores
        std::vector<int> primeScores(n, 0);
        for (int i = 0; i < n; ++i) {
            primeScores[i] = countDistinctPrimeFactors(nums[i]);
        }
        
        // Find left boundary: nearest index to left with primeScore >= current
        std::vector<int> left(n, -1);
        std::stack<int> leftStack;
        for (int i = 0; i < n; ++i) {
            while (!leftStack.empty() && primeScores[i] >= primeScores[leftStack.top()]) {
                leftStack.pop();
            }
            left[i] = leftStack.empty() ? -1 : leftStack.top();
            leftStack.push(i);
        }
        
        // Find right boundary: nearest index to right with primeScore > current
        std::vector<int> right(n, n);
        std::stack<int> rightStack;
        for (int i = n - 1; i >= 0; --i) {
            while (!rightStack.empty() && primeScores[i] > primeScores[rightStack.top()]) {
                rightStack.pop();
            }
            right[i] = rightStack.empty() ? n : rightStack.top();
            rightStack.push(i);
        }
        
        // Max-heap ordered by nums value
        std::priority_queue<std::pair<int, int>> maxValues;
        for (int i = 0; i < n; ++i) {
            maxValues.emplace(nums[i], i);
        }
        
        long long answer = 1;
        while (!maxValues.empty() && k > 0) {
            auto [val, idx] = maxValues.top();
            maxValues.pop();
            
            long long times = 1LL * (right[idx] - idx) * (idx - left[idx]);
            long long steps = std::min(times, static_cast<long long>(k));
            
            answer = answer * modPow(val, static_cast<int>(steps), MOD) % MOD;
            k -= static_cast<int>(steps);
        }
        
        return static_cast<int>(answer);
    }
    
private:
    int countDistinctPrimeFactors(int n) const {
        int count = 0;
        if (n % 2 == 0) {
            ++count;
            while (n % 2 == 0) n /= 2;
        }
        for (long long i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                ++count;
                while (n % i == 0) n /= static_cast<int>(i);
            }
        }
        if (n > 1) ++count;
        return count;
    }
    
    int modPow(int base, int exp, int mod) const {
        long long result = 1;
        long long b = base % mod;
        while (exp > 0) {
            if (exp & 1) result = result * b % mod;
            b = b * b % mod;
            exp >>= 1;
        }
        return static_cast<int>(result);
    }
};

// The solution approach is based on a greedy strategy combined with monotonic stack preprocessing. First, compute the prime score for each element by counting distinct prime factors: handle factor 2 separately, then iterate odd factors up to the square root, and if a residual >1 remains, count it. Next, we need to determine how many times each index can be selected. This is equivalent to finding the "subarray" where this index is the maximum in terms of prime score (with tie-breaking to avoid double counting). For each index `i`, find the nearest index to the left with `primeScore >= primeScore[i]` (let's call it `L`) and the nearest index to the right with `primeScore > primeScore[i]` (call it `R`). The number of subarrays where `i` is the unique maximum (or the rightmost maximum for ties) is `(R - i) * (i - L)`. Use a monotonic decreasing stack (strictly decreasing for the right pass, non-increasing for the left pass) to compute these boundaries in O(n) total. Then push all indices into a max-heap ordered by their value (and index for tie-breaking). Pop the heap, and for each popped index, use its allowed count `t` (but cap it at remaining `k`), multiply the answer by `nums[i]^t` modulo \(10^9+7\) using fast exponentiation. Subtract `t` from `k`, and repeat until `k` becomes 0 or the heap is empty. The initial score is 1. Edge cases: if `nums` is empty, return 1 (though problem likely guarantees non-empty). Also, note that `k` could be larger than the sum of all allowed counts, so we simply multiply by all possible values, and if `k` remains, we ignore it because we cannot select more. The algorithm runs in O(n log n) due to heap operations, plus O(n) for prime factorization (each number up to about 10^4 or more). Space complexity is O(n) for the stacks, vectors, and heap.
