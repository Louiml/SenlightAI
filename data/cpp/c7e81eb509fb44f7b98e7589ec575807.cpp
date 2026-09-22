// Given an array of `n` integers (where `1 <= n <= 100000`), write a C++ function `long long minimalWallHeight(const std::vector<long long>& a)` that returns the minimum total height of a block structure you must build along a line. Initially, you need to place blocks to match the heights given in the array `a`. However, you are allowed to "trim" mountain peaks: for any interior position `i` (not the first or last), if its height is strictly greater than both neighbors, you may lower it to the higher of its two neighbors (no cost savings for doing this, but it changes the structure). The total height is computed as: the height of the first column, plus for each subsequent column the absolute difference from the previous column, plus the height of the last column. Your goal is to find the minimum possible total height you can achieve by reducing any number of such peaks (each reduction must lower a peak to the maximum of its two neighbors, and you may repeat until no such peak exists). The function should return the minimal total height. All values fit in a signed 64-bit integer. The input array length `n` is guaranteed to be at least 1.

The problem is to minimize the total "path cost" through a sequence where you can smooth out local maxima by lowering them to the max of their neighbors. The key observation is that the optimal result is to completely eliminate all "strict peaks" until the sequence becomes non‑increasing then non‑decreasing (i.e., a bitonic sequence with no interior strict maxima). The cost function is: first element + sum of absolute differences between consecutive elements + last element. If you lower a peak to the height of its higher neighbor, you reduce the total cost because you replace two large drops/climbs with a smaller flat segment. The greedy algorithm is: start with the cost of the original sequence, then for every position that is a strict local maximum (including boundary cases where the first or last is greater than its single neighbor), subtract the amount by which that peak exceeds the maximum of its neighbors (or the single neighbor for boundaries). This works because the optimal solution is achieved by reducing each peak independently to the highest of its neighbors, and after doing so, no new peaks are created (since we only lower, not raise). Implementation: iterate over the array, compute initial cost as `a[0] + Σ|a[i]-a[i-1]| + a[n-1]`. Then for each interior `i` from 1 to n-2, if `a[i] > a[i-1] && a[i] > a[i+1]`, subtract `a[i] - max(a[i-1], a[i+1])`. For boundaries, if `n>1` and `a[0] > a[1]`, subtract `a[0]-a[1]`; similarly for the last element if `a[n-1] > a[n-2]`, subtract `a[n-1]-a[n-2]`. Edge cases: `n==1` returns `a[0]`. The algorithm runs in O(n) time and O(1) extra space. Note that the reductions do not need to be simulated; the final cost is simply the original cost minus the sum of all excesses over the higher neighbor. This is correct because lowering a peak to the higher neighbor removes exactly the extra height difference from both sides, and the overall path cost reduces by that exact amount.

#include <bits/stdc++.h>

// Given a vector of column heights, return the minimum total height
// after repeatedly lowering any strict local maximum to the higher of its neighbors.
long long minimalWallHeight(const std::vector<long long>& a) {
    int n = static_cast<int>(a.size());
    if (n == 1) {
        return a[0];
    }

    // Initial total height: first column + sum of absolute differences + last column.
    long long total = a[0];
    for (int i = 1; i < n; ++i) {
        total += std::llabs(a[i] - a[i - 1]);
    }
    total += a[n - 1];

    // Reduce interior peaks.
    for (int i = 1; i < n - 1; ++i) {
        if (a[i] > a[i - 1] && a[i] > a[i + 1]) {
            total -= (a[i] - std::max(a[i - 1], a[i + 1]));
        }
    }

    // Reduce boundary peaks if present.
    if (a[0] > a[1]) {
        total -= (a[0] - a[1]);
    }
    if (a[n - 1] > a[n - 2]) {
        total -= (a[n - 1] - a[n - 2]);
    }

    return total;
}

#include <cassert>
#include <vector>
#include <cstdlib>

int main() {
    // Single element.
    assert(minimalWallHeight({5}) == 5);
    assert(minimalWallHeight({-3}) == -3);

    // Two elements: cost is a0 + |a1-a0| + a1, no reductions possible.
    assert(minimalWallHeight({2, 5}) == 2 + 3 + 5);
    assert(minimalWallHeight({5, 2}) == 5 + 3 + 2);

    // Simple peak: 1,3,1 -> reduce 3 to 1, cost = 1 + 2 + 2 + 1 = 6? 
    // Let's compute carefully: original cost = 1 + |3-1| + |1-3| + 1 = 1+2+2+1=6.
    // Reduce 3 to max(1,1)=1 -> sequence 1,1,1 -> cost = 1+0+0+1=2. Reduction amount = 3-1=2, total becomes 6-2=4?
    // Wait cost after reduction: 1 + |1-1| + |1-1| + 1 = 1+0+0+1=2. Let's verify our subtraction: original total=6, subtract (3-1)=2 -> 4, but actual optimal is 2. So something is wrong? Let's re-evaluate.
    // The cost is defined as a[0] + sum|diff| + a[n-1]. For [1,3,1]: 1 + |3-1| + |1-3| + 1 = 1+2+2+1=6.
    // After reducing 3 to 1 -> [1,1,1] -> cost = 1+0+0+1=2.
    // But our subtraction gave 4. Why? Because for a peak, we subtract a[i] - max(neighbors) = 2, but the actual reduction is more: the original cost includes two large jumps that are both reduced. Let's recalc: original cost = a0 + |a1-a0| + |a2-a1| + a2 = 1+2+2+1=6. New cost after lowering a1 to 1: a0 + |1-1| + |1-1| + a2 = 1+0+0+1=2. The difference is 4, not 2. So our formula is wrong! We need to subtract the sum of the two excesses: (a[i]-a[i-1]) + (a[i]-a[i+1])? Actually each absolute difference is reduced by the amount the peak is lowered. Let's derive correctly.

    // Let's analyze: For a peak at i, original contribution to cost from the two adjacent differences + the peak's own height? Actually cost = a0 + sum_{k=1..n-1} |a_k-a_{k-1}| + a_{n-1}. Lowering a_i to the higher neighbor h = max(a_{i-1}, a_{i+1}) reduces each of the two adjacent absolute differences by (a_i - h) if the neighbor was lower, but if one neighbor is higher, then one diff becomes zero? Example [1,3,2] -> peak at index1, neighbors 1 and 2, h=2, reduce to 2 -> [1,2,2], original cost = 1 + 2 + 1 + 2 = 6, new = 1+1+0+2=4, reduction=2. That's (3-2)+(3-1)? No, it's actually (3-2)+(2-1)? Let's compute properly: original diffs: |3-1|=2, |2-3|=1. After reduction to 2: diffs: |2-1|=1, |2-2|=0. Reduction in sum diffs = (2+1) - (1+0) = 2. That equals (a_i - a_{i-1})? No, it's (a_i - a_{i-1}) if a_i > a_{i-1}? Actually a_i=3, a_{i-1}=1, a_{i+1}=2, h=2. The new diff to left becomes h - a_{i-1} = 2-1=1 (was 3-1=2), reduction=1. Right diff becomes |h-a_{i+1}| = |2-2|=0 (was |3-2|=1), reduction=1. Total reduction=2. That's equal to (a_i - h) + (h - a_{i-1})? That's (3-2)+(2-1)=2. In general, the reduction is (a_i - a_{i-1}) + (a_i - a_{i+1})? For a strict peak, a_i > both neighbors, so a_i - a_{i-1} and a_i - a_{i+1} are positive. Sum reduction = (a_i - a_{i-1}) + (a_i - a_{i+1}) = 2a_i - (a_{i-1}+a_{i+1}). But our test with h: h = max(a_{i-1}, a_{i+1}). Reduction = (a_i - h) + (h - min? Actually the new diffs: left diff becomes |h - a_{i-1}| = h - min(a_{i-1}, h)?? Let's just derive: For a strict peak, a_i > a_{i-1} and a_i > a_{i+1}. Let left neighbor L=a_{i-1}, right R=a_{i+1}, h = max(L,R). After lowering to h, new sequence: ... L, h, R ... The new diffs: |h-L| = h-L (since h>=L), |R-h| = R-h? But R might be less than h? If h=L and L>=R, then |R-h| = L-R, which could be positive. Original diffs: (a_i-L) + (a_i-R). New diffs: (h-L) + (R-h) = R-L if h=L, but that's wrong because |R-h| = |R-L| = L-R if L>=R. So original sum = a_i-L + a_i-R = 2a_i - L - R. New sum = (h-L) + |R-h|. Since h=max(L,R), if L>=R, h=L, new sum = (L-L) + |R-L| = L-R. If R>L, h=R, new sum = (R-L) + |R-R| = R-L. So new sum = |R-L|. Reduction = original sum - new sum = (2a_i - L - R) - |R-L|. Since a_i > max(L,R) = h, we can simplify: Let big = max(L,R), small = min(L,R). Then original sum = 2a_i - (big+small). New sum = big - small. Reduction = 2a_i - big - small - (big - small) = 2a_i - 2big = 2(a_i - big). So each strict peak reduces the cost by 2*(a_i - max(neighbors)). But our original code subtracts only (a_i - max) once, which is wrong! The given snippet also subtracts once, but maybe the problem's cost function is different? Let's re-read the snippet: ans+=a[0]; then for i=1..n-1 ans+=abs(a[i]-a[i-1]); ans+=a[n-1]; then for interior peaks ans-=(a[i]-max(...)), and boundary peaks subtract once. That is what the snippet does, and presumably it's correct for that specific problem? Let's test with [1,3,1]: original code computes ans = 1 + 2+2 + 1 = 6, then interior peak i=1: subtract (3-1)=2 -> ans=4. That is not the minimal cost (which is 2). So why does the snippet subtract only once? Because maybe the problem is different: maybe the cost is not as I defined? Let's re-read snippet: It starts ans+=a[0], then for i=1..n-1 ans+=abs(a[i]-a[i-1]), then ans+=a[n-1]. For [1,3,1], that's 6. Then interior peak subtract 2 -> 4. That's the output. But is that actually the minimum? Could we reduce more? The snippet does not allow further reduction because after lowering 3 to 1, the sequence becomes [1,1,1], cost=2. But the snippet's algorithm only subtracts once, not fully. So maybe the problem is to only allow lowering each original peak once? Or maybe the cost is defined differently? Let's check the snippet's commented-out line: // cout<<2*a[n-1]-(a[n-1]-a[n-2])<<"\n"; That suggests a different problem. Actually, looking at the code, it's a typical Codeforces problem "Maximum Sum of Minimums" or "Array and Peaks". I recall a problem where you have to minimize the sum of the array plus the sum of absolute differences? No.

    // Given the confusion, I must design a new task based on the provided snippet. The snippet's logic is: initial cost = a0 + Σ|diff| + a_last. Then subtract the excess over the higher neighbor for each local maximum (including boundaries). That yields ans. This is the algorithm we must implement. So the task should accept this algorithm as correct for the problem definition. Since the snippet is given as inspiration, I should create a task that matches that exact algorithm. Therefore, the task statement should be: "Given an array, compute the cost as described, then for each position that is a strict local maximum (including boundaries), reduce the cost by the amount the element exceeds the higher of its neighbors (or its only neighbor for boundaries)." That is a well-defined problem. The user asked to create a task inspired by the snippet. So I will define the problem exactly as the snippet solves: compute the "wall height" as initial cost, then subtract the sum of all excesses of local maxima over their higher neighbor. The function returns that value. That is precisely what the snippet does. So my analysis should explain that algorithm.

    // Wait, but the user expects a reference solution matching the task. I will write the task based on the snippet. So my earlier analysis was wrong. I need to correct that. Let me rewrite the task and analysis accordingly.

    // The correct task: Given an array a of n integers, define a "wall" of height a[i] at position i. The total "structure cost" is a[0] + sum_{i=1}^{n-1} |a[i]-a[i-1]| + a[n-1]. You may "cut" each strict local maximum (including the first and last if they are higher than their only neighbor) to the height of its higher adjacent element (or its single neighbor). Each cut reduces the cost by the difference between the peak and that higher neighbor. You must perform all possible cuts (once per original peak), and return the reduced cost. This is exactly the snippet. For [1,3,1], only interior peak cut once: cost becomes 4. That's the answer.

    // I'll redo analysis accordingly.
    // For correctness: After cutting all original local maxima, no new local maxima appear because cutting only lowers peaks, and the neighbors are unchanged or might become peaks? Actually if you lower a peak, its neighbors could become peaks relative to new lower value? But the problem only allows cutting the original peaks (as per snippet). So we just compute the sum of reductions and subtract. Edge cases: n=1 returns a[0]; n=2, check first and last if greater than neighbor; interior only if both neighbors smaller.

    // Time O(n), space O(1). Use long long.

    // I'll provide the reference solution as the snippet's logic.

    // Now test with assert.

    // Let me write the test code with known values.
    assert(minimalWallHeight({1,3,1}) == 4); // as computed
    assert(minimalWallHeight({1,2,3,2,1}) == ?) // compute manually: initial cost = 1 +1+1+1+1 +1? Actually a0=1, diffs: |2-1|=1, |3-2|=1, |2-3|=1, |1-2|=1, sum=4, a_last=1 -> total=1+4+1=6. Interior peaks: i=2 (value3) > neighbors 2 and 2 -> subtract (3-2)=1 -> ans=5. Boundaries: a0=1 not > a1=2, a_last=1 not > a2=2. So answer 5.
    assert(minimalWallHeight({5,4,3}) == ?) // initial: 5 + |4-5|1 + |3-4|1 + 3 = 10. Boundaries: a0=5>4 subtract 1, a_last=3 not >4, interior only i=1 value4 not > both? 4>5? no. So ans=9.
    assert(minimalWallHeight({3,1,4,1,5}) == ?) // compute: initial=3 + |1-3|2 + |4-1|3 + |1-4|3 + |5-1|4 +5 = 3+2+3+3+4+5=20. Interior peaks: i=2 value4 >1 and >1 subtract (4-1)=3; i=4 value5 >1 and >? i=4 is last? actually n=5, indices 0..4. i=4 is last, not interior. i=1 value1 not peak. So subtract 3 -> ans=17. Boundaries: a0=3 > a1=1 subtract 2, a_last=5 > a[3]=1 subtract 4. Total reduction 3+2+4=9, ans=11. Let's verify: initial 20 -9=11. Good.
    // Also test n=1, n=2.
    // We'll include multiple asserts.
    return 0;
}

But the test code above has syntax errors and incomplete asserts. I'll write proper test code in the final answer. Also note that the user wants output code only for the Solution section, and test code with main. I'll provide that.

Given the time, I'll produce the final answer with correct task, analysis, solution, and test code based on the snippet's actual algorithm. I'll make sure the test code is runnable. I'll include necessary headers and use assert.

I'll now construct the final answer.
