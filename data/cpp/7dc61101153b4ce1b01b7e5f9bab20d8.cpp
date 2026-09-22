You are given `n` bamboo sticks with integer lengths, and you need to make exactly three drinks of target lengths `a`, `b`, and `c`. You may combine any sticks arbitrarily: for each stick, you assign it to drink A, drink B, or drink C (it cannot remain unused, and you can use all sticks exactly once). Drinks can be made by mixing multiple sticks assigned to the same drink, and the resulting drink length is the sum of those sticks. However, each time you add a stick to a drink that already has at least one stick, you incur a "mixing cost" of 10. Additionally, after forming the sums for A, B, and C, you pay a penalty equal to the absolute difference between the target and your sum for each drink. Find the minimum total cost (sum of mixing costs and absolute differences) to make all three drinks. If it is impossible to make any drink (which would mean some target has zero assigned sticks), you must skip that assignment. Write a C++ function `int minimumCost(int n, int a, int b, int c, const std::vector<int>& lengths)` that returns the minimal possible total cost.

#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Example from original snippet: n=3, targets 1 2 3, lengths {1,2,3}
    // Assign each to its own drink: mixing cost 0, penalties |1-1|+|2-2|+|3-3| = 0
    assert(minimumCost(3, 1, 2, 3, {1, 2, 3}) == 0);

    // n=3, targets 10 10 10, lengths {5,5,10}
    // Assign 5 and 5 to A (cost 10), 10 to B, but C must have at least one stick -> impossible? Actually assign 5,5 to A, 10 to B, but C has zero. Need all three used, so possible assignments: {5,5,10} -> one drink sum 20, others 0 invalid. So actually this case may be impossible; but test with n=4.
    // Use n=4, lengths {5,5,5,5}, targets 10,10,0? But c must be positive? The problem says "exactly three drinks" and targets can be any non-negative? In original code c is input, but no positivity. But we assume valid.
    // Let's test a simple case:
    // n=3, a=10, b=10, c=10, lengths {10,10,10} -> cost 0.
    assert(minimumCost(3, 10, 10, 10, {10, 10, 10}) == 0);

    // n=4, lengths {1,1,1,1}, targets 2,1,1
    // Assign two sticks to A, one to B, one to C: mixing cost 10, penalties |2-2|=0,|1-1|=0,|1-1|=0 => total 10.
    assert(minimumCost(4, 2, 1, 1, {1, 1, 1, 1}) == 10);

    // n=2 with all sticks must be used, but need 3 drinks => impossible. But we skip test.

    // A case where mixing more sticks is cheaper than penalty:
    // n=3, targets 100 1 1, lengths {50,50,1}
    // Assign both 50's to A (cost 10) and 1 to B, but C has nothing -> invalid. Need at least one per drink.
    // With n=4, lengths {50,50,1,1}, targets 100,1,1
    // Assign 50+50 to A (cost 10), 1 to B, 1 to C => cost 10.
    assert(minimumCost(4, 100, 1, 1, {50, 50, 1, 1}) == 10);

    // Another: n=3, lengths {7,8,9}, targets 8,8,8
    // Assign 7 to A (penalty 1), 8 to B (0), 9 to C (1) => total 2.
    assert(minimumCost(3, 8, 8, 8, {7, 8, 9}) == 2);

    // Edge with large mixing cost but unavoidable:
    // n=6, lengths all 1, targets 3,3,1
    // Best: assign 3 sticks to A (cost 20), 3 sticks to B (cost 20), 0 to C invalid. Actually need C. So assign 2 to A, 2 to B, 2 to C: sums 2,2,2 penalties |3-2|+|3-2|+|1-2|=1+1+1=3, mixing cost 10+10+10=30, total 33. Or assign 3 to A, 2 to B, 1 to C: sumA=3, sumB=2, sumC=1 penalties 0+1+0=1, mixing cost 20+10+0=30, total 31. So min 31.
    assert(minimumCost(6, 3, 3, 1, {1,1,1,1,1,1}) == 31);

    return 0;
}

#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

// Compute the minimum total cost to make drinks of targets a, b, c from given sticks.
// Each stick must be assigned to exactly one drink; each drink must have at least one stick.
// Mixing cost: for a drink with k sticks, cost = 10 * (k - 1).
// Penalty: absolute difference between target and sum of assigned sticks.
int minimumCost(int n, int a, int b, int c, const std::vector<int>& lengths) {
    int best = INT_MAX;
    // Enumerate all 3^n assignments using a base-3 counter.
    int totalAssignments = 1;
    for (int i = 0; i < n; ++i) totalAssignments *= 3;

    for (int mask = 0; mask < totalAssignments; ++mask) {
        int temp = mask;
        int sumA = 0, sumB = 0, sumC = 0;
        int countA = 0, countB = 0, countC = 0;

        for (int i = 0; i < n; ++i) {
            int group = temp % 3;
            temp /= 3;
            if (group == 0) {
                sumA += lengths[i];
                ++countA;
            } else if (group == 1) {
                sumB += lengths[i];
                ++countB;
            } else {
                sumC += lengths[i];
                ++countC;
            }
        }

        if (countA == 0 || countB == 0 || countC == 0) continue;

        int cost = 0;
        cost += (countA - 1) * 10;
        cost += (countB - 1) * 10;
        cost += (countC - 1) * 10;
        cost += std::abs(a - sumA) + std::abs(b - sumB) + std::abs(c - sumC);

        if (cost < best) best = cost;
    }
    return best;
}

// The problem is a small exhaustive search over all assignments of each stick to one of three groups (A, B, C). Since `n` can be up to 8 in a typical constraint for this kind of brute-force (the original code uses a loop over `1 << (2*n)` enumerations, which is feasible for `n <= 8` or 10), we can directly enumerate all `3^n` assignments using a recursive function or an iterative base-3 counter. For each assignment, we compute the sums for A, B, C and count how many sticks are assigned to each group. If any group has zero sticks, the assignment is invalid because you cannot make a drink without any sticks (the original code explicitly skips those). For valid assignments, we compute the mixing cost: each group with `k` sticks costs `10 * (k-1)` because the first stick incurs no mixing cost, and each additional stick adds 10. Then we add `abs(sumA - a) + abs(sumB - b) + abs(sumC - c)`. We track the minimum across all valid assignments. Edge cases: if `n` is too small to assign at least one stick to each group, the answer is impossible; however, the problem guarantees a valid solution exists. Also, the original code uses a sentinel large value (`MX = 1001001`) for the initial answer, which is safe because the maximum possible cost is bounded by `10*(n-3) + 3*max(lengths,a,b,c)` which is far less than that. Time complexity is `O(3^n * n)` for enumerating assignments and summing lengths; with `n <= 8`, that is at most 6561 * 8 ≈ 52k operations, which is fine. Space complexity is `O(n)` for the assignment vector or we can use an iterative counter without extra space.
