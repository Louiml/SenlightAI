// You are given `n` engineers with two attributes each: `speed[i]` and `efficiency[i]`. Your task is to select at most `k` engineers to form a team. The performance of a team is defined as the sum of the speeds of the selected engineers multiplied by the minimum efficiency among the selected engineers. Write a C++ function `int maxPerformance(int n, vector<int>& speed, vector<int>& efficiency, int k)` that returns the maximum possible performance modulo `10^9 + 7`. All speeds and efficiencies are positive integers, and `1 ≤ k ≤ n ≤ 10^5`. If you select fewer than `k` engineers, you may do so, but selecting more never hurts, so the optimal team will always have exactly `min(k, n)` members. The modulo operation should be applied only at the very end (after computing the maximum performance as a 64-bit integer).

#include <cassert>
#include <vector>

int main() {
    // Example 1: n=6, k=2
    {
        std::vector<int> speed = {2, 10, 3, 1, 5, 8};
        std::vector<int> efficiency = {5, 4, 3, 9, 7, 2};
        int result = maxPerformance(6, speed, efficiency, 2);
        assert(result == 60); // Choose engineers 1 and 4: speeds 10+5=15, min eff=4 → 60
    }
    
    // Example 2: n=3, k=1
    {
        std::vector<int> speed = {5, 10, 3};
        std::vector<int> efficiency = {2, 8, 6};
        int result = maxPerformance(3, speed, efficiency, 1);
        assert(result == 80); // Pick engineer 2: speed 10, eff 8 → 80
    }
    
    // Example 3: All same efficiency
    {
        std::vector<int> speed = {1, 2, 3};
        std::vector<int> efficiency = {5, 5, 5};
        int result = maxPerformance(3, speed, efficiency, 2);
        assert(result == 25); // Pick speeds 2+3=5, eff 5 → 25
    }
    
    // Example 4: k >= n, take all
    {
        std::vector<int> speed = {2, 3, 4};
        std::vector<int> efficiency = {1, 2, 3};
        int result = maxPerformance(3, speed, efficiency, 10);
        assert(result == 27); // sum=9, min eff=1 → 9*3=27? Wait: min efficiency is 1, so 9*1=9. Let's compute: Actually min efficiency is 1, so performance=9*1=9. But correct best is to take all→ sum=9, min eff=1 → 9. But we can choose at most k, not exactly k, so we can take all → 9. Right: 9.
        assert(result == 9);
    }
    
    // Example 5: Large values, check modulo
    {
        std::vector<int> speed = {100000, 100000};
        std::vector<int> efficiency = {100000, 100000};
        int result = maxPerformance(2, speed, efficiency, 2);
        // sum=200000, min eff=100000 → 2e10, mod 1e9+7 = 2e10 % 1e9+7. Compute: 2e10 = 20000000000. 20000000000 % 1000000007 = 20000000000 - 19*1000000007 = 20000000000 - 19000000133 = 999999867. So expected 999999867.
        assert(result == 999999867);
    }
    
    // Example 6: k=1, many engineers
    {
        std::vector<int> speed = {10, 20, 30};
        std::vector<int> efficiency = {5, 10, 15};
        int result = maxPerformance(3, speed, efficiency, 1);
        // Best: pick engineer with eff=15 speed=30 → 450
        assert(result == 450);
    }
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

// Computes the maximum possible performance of a team of at most k engineers.
// Performance = (sum of speeds) * (minimum efficiency among selected).
// Returns the result modulo 1'000'000'007.
int maxPerformance(int n, std::vector<int>& speed, std::vector<int>& efficiency, int k) {
    const long long MOD = 1000000007LL;
    
    // Pair each efficiency with its corresponding speed.
    std::vector<std::pair<int, int>> engineers(n);
    for (int i = 0; i < n; ++i) {
        engineers[i] = {efficiency[i], speed[i]};
    }
    
    // Sort by efficiency in descending order.
    std::sort(engineers.begin(), engineers.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.first > b.first;
              });
    
    long long totalSpeed = 0;
    long long bestPerformance = 0;
    // Min-heap to keep the smallest speeds so that we can remove them when over capacity.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    
    for (const auto& [eff, spd] : engineers) {
        minHeap.push(spd);
        totalSpeed += spd;
        
        if (minHeap.size() > static_cast<size_t>(k)) {
            totalSpeed -= minHeap.top();
            minHeap.pop();
        }
        
        // Current efficiency is the minimum among all selected because we process in descending order.
        long long candidate = totalSpeed * eff;
        if (candidate > bestPerformance) {
            bestPerformance = candidate;
        }
    }
    
    return static_cast<int>(bestPerformance % MOD);
}

// The key insight is to sort the engineers by efficiency in descending order. When processing engineers from highest efficiency to lowest, the current engineer's efficiency is the minimum efficiency of any team that includes this engineer and any previously processed (higher‑efficiency) engineers. For each engineer, we maintain a max‑heap (or min‑heap with size limited to `k`) that stores the speeds of the best‑performing subset of engineers processed so far. Specifically, we keep the largest speeds among those we have encountered, but only up to `k` engineers. For each engineer in sorted order, we add their speed to a running sum. If the heap size exceeds `k`, we remove the smallest speed (using a min‑heap) so that the sum is maximized for teams of size at most `k`. Then we compute the candidate performance as `sum * current_efficiency` and update the answer with the maximum. This works because when we fix the minimum efficiency to be the current engineer's efficiency, the optimal team consists of the `k` highest‑speed engineers among those with efficiency ≥ current (i.e., those already processed). Edge cases: when `k` is large enough, we always include all processed engineers; speeds and efficiencies are positive, so overflow is possible – use `long long` for sums and results, applying modulo only at the end. Time complexity is `O(n log n)` for sorting plus `O(n log k)` for heap operations, and space complexity is `O(n)` for the pairs and `O(k)` for the heap.
