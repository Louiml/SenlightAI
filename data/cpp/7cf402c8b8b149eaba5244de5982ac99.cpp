Write a C++ function `long long maxCouncils(int k, const std::vector<long long>& groups)` that takes the required council size `k` (2 ≤ k ≤ 20) and a vector `groups` of size `n` (k ≤ n ≤ 50) where each element is the number of students in a group (1 ≤ a[i] ≤ 10^9). The function must return the maximum number of councils that can be formed, where each council consists of exactly `k` students from distinct groups, and each student can belong to at most one council (students may be left unused). The function should use binary search on the number of councils to find the maximum feasible value.
#include <cassert>
#include <vector>

// Declare the solution function
long long maxCouncils(int k, const std::vector<long long>& groups);

int main() {
    // Example 1 from problem statement
    assert(maxCouncils(3, {4,4,4,4,4}) == 6);
    // Example 2
    assert(maxCouncils(4, {1,2,3,4,5,6}) == 5);
    // Single council
    assert(maxCouncils(2, {1,1}) == 1);
    // Not enough students in total
    assert(maxCouncils(3, {1,1,1,1}) == 1); // total=4, need 3 per council => at most 1
    // Many groups, small k
    assert(maxCouncils(2, {10,10,10}) == 10); // each council uses 2, can form 10
    // Large groups with one small group
    assert(maxCouncils(3, {100,100,1}) == 1); // only one council possible
    // Edge: k equals n
    assert(maxCouncils(5, {5,5,5,5,5}) == 5);
    // Edge: all groups exactly match m
    assert(maxCouncils(2, {1,1,2,2}) == 2); // can form 2 councils
    // Large numbers
    long long big = 1000000000LL;
    assert(maxCouncils(10, {big,big,big,big,big,big,big,big,big,big}) == 100000000LL);
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the maximum number of councils that can be formed.
// Each council has exactly k students from distinct groups.
long long maxCouncils(int k, const std::vector<long long>& groups) {
    long long n = groups.size();
    long long total_students = 0;
    long long max_group = 0;
    for (long long g : groups) {
        total_students += g;
        max_group = std::max(max_group, g);
    }

    // Upper bound for number of councils
    long long low = 0;
    long long high = total_students / k; // at most this many

    auto feasible = [&](long long m) {
        if (m == 0) return true;
        long long capacity = 0;
        for (long long g : groups) {
            capacity += std::min(g, m);
            if (capacity >= m * k) return true; // early exit
        }
        return capacity >= m * k;
    };

    long long answer = 0;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (feasible(mid)) {
            answer = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return answer;
}
// The problem is a classic feasibility-with-binary-search task. We want the largest integer `m` such that we can form `m` councils of size `k`, each with students from different groups. For a candidate `m`, we need to check if it's possible to assign students from the groups to fill `m` "council slots" per group. A key observation: for each group with `a[i]` students, at most one student from that group can go into any particular council. Therefore, for `m` councils, each group can contribute at most `min(a[i], m)` students, but also the total number of students needed is `m * k`. So feasibility is not just a simple sum; we must also respect that no group can provide more than `m` students total across all councils (since each council takes at most one from each group), and we cannot split a student across councils.
//
// The standard greedy check: Maintain a counter `filled` for how many council "sets" have been fully completed (each set requires `k` students from distinct groups). Iterate through groups. For each group size `a[i]`, we try to fill as many slots as possible. The trick is to process groups in any order and track a "carry" `last` that represents leftover capacity from incomplete councils. However, the provided code uses a somewhat subtle simulation: it traverses groups and tries to fill slots one council at a time? Actually, the provided `ispossible` is not obviously correct. Let me derive a correct approach.
//
// A correct feasibility check: For `m` councils, the maximum number of students we can use from group `i` is `min(a[i], m)` (since each council can take at most one from that group). The total number of distinct groups available is `n`. We need to select, for each of the `m` councils, `k` students from `k` different groups. This is a bipartite matching problem? But since groups are independent, a simpler necessary and sufficient condition is: the total sum of `min(a[i], m)` must be at least `m*k`, and also each group can provide at most `m` students, but the downside is that we might have too many students from a few groups and not enough from others. Actually, the condition is exactly that the sum of `min(a[i], m)` ≥ `m*k`. Reason: each of the `m` councils needs `k` distinct groups. For each group, it can contribute at most 1 student per council, so at most `m` students total. If the sum of these caps is at least `m*k`, can we always achieve it? Yes, because we can think of assigning each group's students to different councils in a round-robin fashion; since the total demand per group is at most `m`, and total capacity is enough, a straightforward greedy works. For example, sort groups by size, then for each council take the next available student from a distinct group. This is reminiscent of the "scheduling" problem where we have `n` tasks each with capacity `min(a[i],m)` and we need to fill `m*k` slots across `m` "bins" each taking at most one from each group. This is always feasible if the sum of capacities ≥ total demand and each capacity ≤ `m`? Actually, there is another constraint: each bin needs `k` distinct groups, but since there are at least `k` groups (n ≥ k) and each group can supply at most one per bin, the only limitation is total capacity. I believe the simple sum condition is sufficient. Let's verify with a counterexample: k=3, n=3, m=2, groups [2,2,2] -> sum of min(2,2)=2 per group, total=6, need=6, feasible? We need 2 councils each with 3 distinct groups. With groups each having 2 students, we can make council1: g1,g2,g3; council2: g1,g2,g3. Yes. Another: k=2, m=2, groups [2,0?] but a[i]≥1. groups [2,1] n=2, but n≥k=2, so groups [2,1] sum min=2+1=3, need=4, not feasible. Good. If groups [3,3,0?] but a[i]≥1, groups [3,1,1] k=3, m=2, sum min = min(3,2)+min(1,2)+min(1,2)=2+1+1=4, need=6, not feasible. If groups [3,3,3] k=3, m=3, sum min=3+3+3=9, need=9 feasible (each council uses one from each). If groups [10,1,1] k=3, m=2, sum min = 2+1+1=4, need=6 not feasible (correct because we only have 4 usable students). If groups [10,10,1] k=3, m=5, sum min =5+5+1=11, need=15 not feasible. If groups [10,10,10] k=3, m=5, sum=15, need=15 feasible. So the condition seems correct. However, a subtle case: groups [6,6,6,0?] but a[i]≥1, k=4, n=4, m=5, groups [6,6,6,6] sum min=5+5+5+5=20, need=20, feasible? Each of 5 councils needs 4 distinct groups, and each group has at least 5 students, so yes. So the condition is indeed sufficient. Therefore the feasibility check simplifies to: `sum(min(a[i], m)) >= m * k`. That's it.
//
// So the binary search: low = 0, high = (max(a) * n) / k (since at most total students / k). Check using the above. The answer is the largest m for which the condition holds. Complexity: O(n log (total/k)) time, O(1) extra space. Edge cases: m=0 always true, m=1 requires at least k groups? Actually if n≥k, but if some groups have 0? But a[i]≥1, so at least k groups? Since n≥k and each group has at least 1 student, m=1 is always feasible because we can pick k groups with one student each. The binary search handles it.
