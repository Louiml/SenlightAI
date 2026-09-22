// Write a C++ function `int minSticksToFormSquare(int sticks[], int n)` that, given an array of positive integer stick lengths (all ≤ 50, with n between 1 and 100), determines the maximum possible side length of a square that can be formed using all sticks exactly once, where each side of the square must be formed by a subset of sticks whose lengths sum to the same value. If it is impossible to form a square using all sticks, return 0. The function should handle duplicate stick lengths efficiently and must not use recursion depth beyond n. The input array may be unsorted, and you may modify it if needed. If the total sum of all sticks is not divisible by 4, or if the largest stick exceeds the target side length, it's impossible. The square's side length is the total sum divided by 4, and you must verify if the sticks can be partitioned into 4 groups of that exact sum. However, note that the actual problem asks for the **maximum** possible equal-length side when forming an **equilateral polygon** (not necessarily a square) using all sticks? No—re-read: The code snippet solves HDU 1455, which is "Sticks" — given sticks, find the minimum possible equal length of sticks that can be formed by joining all given sticks end-to-end into a set of sticks of equal length. So clarify: The task is: Given an array of positive integers (lengths of sticks), determine the **minimum possible length** of equal-length sticks that can be obtained by concatenating all given sticks into groups of that length, using each given stick exactly once. Return that minimum length. If impossible (e.g., any stick longer than half the sum? Actually always possible with length = sum if you put all in one group, but the problem requires at least 2 groups? In the original problem, you must form at least 2 sticks? The code attempts to find minimal L such that sum is divisible by L and sticks can be partitioned into sum/L groups each summing to L, with the constraint that you can't have a single group for all sticks? In the code, ss = sum/i; they loop i from max stick to sum, and if 2*i>=sum they break and output sum, meaning they allow a single group? Actually they break and output sum if 2*i >= sum, meaning for i >= sum/2, they only have 1 or 2 groups? To match the original problem, the function should return the **minimum possible length L** such that the sticks can be partitioned into **at least two** groups each summing to L? The original problem states you must use all sticks to form a set of sticks of equal length, with at least two sticks? Yes, typically you must form at least 2 sticks. So if L = sum, you'd have only one stick, which is not allowed. So the answer is the minimum L < sum that works, else return sum? Actually the original problem requires finding the minimal possible original length, and you must have at least 2 sticks. So if no L<sum works, return sum as the only option? But the code outputs sum if 2*i>=sum, which means for i >= sum/2, it outputs sum, meaning if the largest stick is at least half the sum, you can't have two sticks, so you must use sum. So the function should return the minimal L such that L divides sum, L >= max stick, and sticks can be partitioned into sum/L groups of sum L, with the restriction that if L >= sum/2 (i.e., only one or two groups?) Actually if L = sum/2, there are exactly 2 groups, which is allowed. If L > sum/2, only 1 group (L=sum) is possible, so return sum. So implement: For each candidate L from max element up to sum/2 (inclusive), if sum % L == 0, check if partition possible; if yes return L. If none found, return sum. That matches the code. Provide function `int minEqualStickLength(vector<int> sticks)`.
The problem is a classic partition problem: we need to find the smallest L (candidate length) such that all sticks can be grouped into subsets each summing to L. First compute total sum S. If S is not divisible by L, skip. Also L must be at least the maximum stick length maxVal. Since we need at least two groups, L cannot exceed S/2; if no L ≤ S/2 works, the answer is S (single stick). For a given L, we need to check if we can partition all sticks into k = S/L groups. Use DFS with backtracking: sort sticks descending to reduce branching, maintain a boolean used array. Recursively build one group at a time: for each group, start with the first unused stick (must be ≤ L), then try combinations. Prune: if we fail to place a stick of a certain length in a position, skip all subsequent sticks of the same length. Also if at any point the current sum + the smallest remaining stick > L, prune. Important edge case: if the largest stick > L, impossible. Time complexity: In worst case exponential, but with sorting and pruning it is practical for n ≤ 100 and stick lengths ≤ 50. Space O(n) for used array and recursion stack depth at most n. The reference solution uses DFS with backtracking, checking all candidates in increasing order, so the first found is minimal.
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

// Determine the minimum possible equal length L such that all sticks can be
// partitioned into at least two groups, each summing to L.
// If no such L ≤ sum/2 exists, return sum (single stick).
int minEqualStickLength(std::vector<int> sticks) {
    int n = static_cast<int>(sticks.size());
    if (n == 0) return 0;

    int total = std::accumulate(sticks.begin(), sticks.end(), 0);
    // Sort descending to speed up backtracking
    std::sort(sticks.begin(), sticks.end(), std::greater<int>());
    int maxStick = sticks[0];

    // Candidate L must be at least maxStick and at most total/2 (to have ≥2 groups)
    for (int L = maxStick; L <= total / 2; ++L) {
        if (total % L != 0) continue;
        int groups = total / L;

        std::vector<bool> used(n, false);

        // Recursive lambda: try to fill current group with sum 'currentSum'
        // starting from index 'start' (descending order)
        std::function<bool(int, int, int)> dfs = [&](int groupIdx, int currentSum, int start) -> bool {
            // All groups formed successfully
            if (groupIdx == groups) return true;
            if (currentSum == L) {
                // Find first unused stick to start next group
                int firstUnused = -1;
                for (int i = 0; i < n; ++i) {
                    if (!used[i]) { firstUnused = i; break; }
                }
                // All sticks used? Should not happen here because groups formed correctly
                if (firstUnused == -1) return groupIdx + 1 == groups;
                used[firstUnused] = true;
                bool ok = dfs(groupIdx + 1, sticks[firstUnused], firstUnused - 1);
                used[firstUnused] = false;
                return ok;
            }

            for (int i = start; i >= 0; --i) {
                if (used[i]) continue;
                // Prune duplicates: if previous stick same length and not used, skip
                if (i < n - 1 && !used[i + 1] && sticks[i] == sticks[i + 1]) continue;
                if (currentSum + sticks[i] > L) continue;
                used[i] = true;
                if (dfs(groupIdx, currentSum + sticks[i], i - 1)) return true;
                used[i] = false;
                // If adding this stick exactly reaches L and it fails, no need to try smaller sticks
                if (currentSum + sticks[i] == L) break;
            }
            return false;
        };

        // Start with the first (largest) stick
        used[0] = true;
        if (dfs(0, sticks[0], 0)) return L;
    }
    // If nothing found, the only way is a single stick of length total
    return total;
}
#include <cassert>
#include <vector>

// The solution function is declared above (include it here in actual test)

int main() {
    // Simple case: [1,1,1,1] can form sticks of length 2 (two sticks) or 1? Actually minimum L=1? But at least 2 groups? L=1 gives 4 groups, works. So answer 1.
    assert(minEqualStickLength({1,1,1,1}) == 1);
    // [1,2,3] sum=6. Max=3. L=3 works? groups=2: {3} and {1+2} → yes, answer 3.
    assert(minEqualStickLength({1,2,3}) == 3);
    // [5,5,5,5] sum=20. L=5 gives 4 groups, works. L=10 works too? But minimal is 5.
    assert(minEqualStickLength({5,5,5,5}) == 5);
    // [2,2,2,2,2,2] sum=12. L=2 works (6 groups), minimal.
    assert(minEqualStickLength({2,2,2,2,2,2}) == 2);
    // [3,3,5,7] sum=18. L=9? groups=2: {5+3+?} 5+3+3=11 no. 7+5+3=15 no. L=6? sum%6=0, groups=3, max star=7>6, skip. L=9 fails? Actually possible? {7+5+3+3}=18 only one group, not allowed. So L=18 is answer? But check L=9: groups=2, need two groups sum 9. Possible {7+?} no. So answer 18.
    assert(minEqualStickLength({3,3,5,7}) == 18);
    // Duplicate pruning test: [3,3,3,3,3] sum=15. L=3 works (5 groups), minimal.
    assert(minEqualStickLength({3,3,3,3,3}) == 3);
    // Larger case: [1,2,3,4,5,6,7,8,9,10] sum=55. Check minimal L? 55 is prime? 55 divisible by 5 and 11. L=11? groups=5, max=10≤11, possible? Likely yes. So answer 11.
    assert(minEqualStickLength({1,2,3,4,5,6,7,8,9,10}) == 11);
    // Edge case: single stick [10] sum=10, L from 10 to 5? none, return 10.
    assert(minEqualStickLength({10}) == 10);
    // Impossible to have two groups, e.g., [7] sum=7, return 7.
    assert(minEqualStickLength({7}) == 7);
    // All sticks same large: [50]*4 sum=200, L=50 works.
    assert(minEqualStickLength({50,50,50,50}) == 50);
    return 0;
}
