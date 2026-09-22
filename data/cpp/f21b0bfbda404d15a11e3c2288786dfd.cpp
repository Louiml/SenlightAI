You are given a directed functional graph of N vertices labeled 0 through N-1, where each vertex i has a single outgoing edge to vertex P[i] (with 0-based indexing supplied). Additionally, each vertex has a score C[i] that can be positive, negative, or zero. Starting from any chosen vertex, you may take exactly K steps (moves along the directed edges), collecting the score of each vertex you land on after each move (including the final vertex). You may also choose to stop earlier, but you must take at least one step. Write a C++ function `long long maxScoreInKSteps(int N, int K, const std::vector<int>& P, const std::vector<int>& C)` that returns the maximum total score obtainable. The graph may have self-loops and cycles, K can be very large (up to at least 1e9), and N up to at least 2000. Since you can start anywhere, consider all possible starting vertices and all possible step counts from 1 to K, but you must do so efficiently by detecting cycles and using the fact that once you are in a cycle, the optimal long-term gain per step is either cycle sum if positive or you should stop before completing a full cycle if negative.

#include <cassert>
#include <vector>

// Declaration (assume the function from Solution is defined above)
long long maxScoreInKSteps(int N, int K, const std::vector<int>& P, const std::vector<int>& C);

int main() {
    // Simple chain: 0->1, 1->0 (cycle of length 2), scores: 5, 3
    {
        int N = 2;
        int K = 5;
        std::vector<int> P = {1, 0};
        std::vector<int> C = {5, 3};
        long long res = maxScoreInKSteps(N, K, P, C);
        // Best: start at 0, take 5 steps: 5+3+5+3+5 = 21
        assert(res == 21);
    }
    // Self-loop with negative score
    {
        int N = 1;
        int K = 100;
        std::vector<int> P = {0};
        std::vector<int> C = {-7};
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == -7);
    }
    // Two nodes, one negative, one positive, cycle length 2
    {
        int N = 2;
        int K = 3;
        std::vector<int> P = {1, 0};
        std::vector<int> C = {-10, 2};
        // Best: start at 1, take steps: 2 + (-10) + 2 = -6
        // Or start at 1, stop after 1 step: 2 => better
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == 2);
    }
    // Larger cycle with positive sum
    {
        int N = 3;
        int K = 10;
        std::vector<int> P = {1, 2, 0};
        std::vector<int> C = {1, 2, 3}; // cycle sum = 6, best prefix per cycle 3
        // Start at 0: take 10 steps => 3 cycles (6+6+6=18) + 1 step (1) = 19
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == 19);
    }
    // K smaller than cycle length
    {
        int N = 4;
        int K = 2;
        std::vector<int> P = {1, 2, 3, 0};
        std::vector<int> C = {10, 20, 30, 40};
        // Best start at 3? Actually start at 3 -> 40+10=50, or start at 2 ->30+40=70, etc.
        // Max is start at 2: 30+40=70
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == 70);
    }
    // All negative, large K
    {
        int N = 3;
        int K = 1000000;
        std::vector<int> P = {1, 2, 0};
        std::vector<int> C = {-5, -6, -7};
        // Best is single step: -5 (from node 0) or -6, -7, max -5
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == -5);
    }
    // Cycle sum zero
    {
        int N = 2;
        int K = 10;
        std::vector<int> P = {1, 0};
        std::vector<int> C = {5, -5};
        // Best: take one step from 0: 5, or take many cycles sum stays same, best is 5
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == 5);
    }
    // Single step maximum
    {
        int N = 3;
        int K = 1;
        std::vector<int> P = {1, 2, 0};
        std::vector<int> C = {100, -50, 200};
        long long res = maxScoreInKSteps(N, K, P, C);
        assert(res == 200); // start at 2 -> 200
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum total score obtainable by starting at any vertex,
// moving along edges P for at most K steps (at least one step).
long long maxScoreInKSteps(int N, int K, const std::vector<int>& P, const std::vector<int>& C) {
    const long long NEG_INF = std::numeric_limits<long long>::min() / 4;
    long long ans = NEG_INF;
    
    // Try each possible starting vertex.
    for (int start = 0; start < N; ++start) {
        // Simulate from start until we return to start (forming a cycle).
        std::vector<long long> prefix;  // prefix[i] = sum after i steps (i from 1 to cnt)
        std::vector<int> path;
        int cur = P[start];
        int step = 1;
        prefix.push_back(C[cur]);
        path.push_back(cur);
        while (cur != start && step < K) {
            // We stop early if we hit the start before K steps.
            cur = P[cur];
            ++step;
            prefix.push_back(prefix.back() + C[cur]);
            path.push_back(cur);
        }
        int cnt = step;  // number of steps to return to start (cycle length)
        long long cycleSum = prefix.back();  // sum over one full cycle
        
        // Consider all possible stopping points from 1 up to min(cnt, K) steps.
        long long bestPrefix = NEG_INF;
        for (int i = 0; i < cnt && i < K; ++i) {
            bestPrefix = std::max(bestPrefix, prefix[i]);
        }
        ans = std::max(ans, bestPrefix);
        
        // If K is larger than cycle length, we can take whole cycles.
        if (cnt < K) {
            long long fullCycles = K / cnt;
            long long remainder = K % cnt;
            // Option A: take fullCycles whole cycles plus up to remainder steps.
            long long totalA = fullCycles * cycleSum;
            // For the remainder steps, we might start at the beginning of a new cycle.
            // The best prefix among the first remainder steps (if remainder > 0)
            // or we can take nothing extra if remainder == 0 (but we already have at least one step).
            long long bestRemainder = 0;
            if (remainder > 0) {
                bestRemainder = NEG_INF;
                for (int i = 0; i < remainder && i < cnt; ++i) {
                    bestRemainder = std::max(bestRemainder, prefix[i]);
                }
            }
            ans = std::max(ans, totalA + bestRemainder);
            
            // Option B: take (fullCycles - 1) whole cycles, then continue for cnt + remainder steps
            // from the start (i.e., not completing the last full cycle).
            // But our prefix array already contains sums for up to cnt steps from start.
            // For Option B, we simulate from start again for additional steps after fullCycles-1 cycles.
            // However, we can directly compute: after (fullCycles-1)*cnt steps, we are back at start.
            // Then we have remainder + cnt steps left. But we should not take more than K total steps.
            if (fullCycles >= 1) {
                long long totalB = (fullCycles - 1) * cycleSum;
                long long extra = cnt + remainder;
                if (extra > K - (fullCycles-1)*cnt) {
                    extra = K - (fullCycles-1)*cnt;
                }
                // Need max prefix sum up to extra steps from start, but extra could be larger than cnt.
                // We can simulate up to extra steps from start (at most 2*cnt).
                long long curSum = 0;
                int curNode = start;
                long long bestFromStart = NEG_INF;
                int stepsTaken = 0;
                while (stepsTaken < static_cast<int>(extra) && stepsTaken < K) {
                    curNode = P[curNode];
                    curSum += C[curNode];
                    bestFromStart = std::max(bestFromStart, curSum);
                    ++stepsTaken;
                }
                ans = std::max(ans, totalB + bestFromStart);
            }
            
            // Option C: If cycleSum is negative, it's better not to take any full cycles.
            // Already handled by bestPrefix (taking at most cnt steps), but we also need to consider
            // the case where we take exactly cnt steps (one full cycle) but not more. That's covered
            // by bestPrefix if cnt <= K.
            // If cycleSum is zero, taking extra cycles doesn't help, and bestPrefix already covers it.
        }
    }
    return ans;
}

// The naive simulation from every start for every step up to K is O(N*K) and impossible for large K. The key observation: from any starting vertex, the path eventually enters a cycle (because the graph is functional). If the total cycle sum is positive, the best long-term strategy is to traverse as many full cycles as possible (up to the floor of remaining steps), but you might want to take a partial cycle before or after to capture the best prefix sum. If the cycle sum is non-positive, you should never complete a full extra cycle; you may stop at the best prefix within the cycle. Therefore, for each start, first simulate along the path until you revisit the start (forming a cycle). Record the cumulative sums after each step. Let `cnt` be the cycle length and `cycleSum` be the sum of scores over one full cycle. For steps up to min(K, cnt), just simulate and update the answer (also allowing stopping early). Then, if `cnt < K`, consider how many whole cycles you can add. Let `q = K / cnt` and `r = K % cnt`. If `cycleSum > 0`, you want to take `q` whole cycles, but you may also need to take `r` extra steps from the start, and also consider taking only `q-1` cycles plus some suffix of the path (since the best cumulative may occur before completing the whole cycle). If `cycleSum <= 0`, you take only `q-1` cycles (or zero if q=1) and then the best prefix among the remaining steps. More precisely, the standard approach: precompute the prefix sums of the cycle starting from the cycle entry point, and for each starting position within the cycle, determine the maximum prefix sum over any length up to `min(cnt, remaining steps)`. Since N is at most 2000, O(N^2) is fine. For each start, we simulate up to cnt steps (which is at most N) to find the cumulative sums, then compute the maximum cumulative sum over the first `cnt` steps. Then, if K > cnt, we add full cycles times `cycleSum` and then handle the remainder. Careful handling: We need to consider the best cumulative sum within the first `cnt` steps, but also after adding whole cycles. The final answer is the maximum over all starts and over all possible stopping points. Since `K` can be huge, we only need to consider the cycle structure. Time complexity: For each of N starts, we simulate at most N steps to build the path and cycle, so O(N^2). Space O(N). Edge cases: self-loops (cycle length 1), negative scores, all scores negative (answer is the maximum single score), K less than cycle length, and large K with positive cycle sum.
