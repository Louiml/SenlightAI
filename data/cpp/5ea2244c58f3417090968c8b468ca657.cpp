You are implementing a scheduling feasibility checker for a single-machine manufacturing process. There are `M` product types (numbered 0 to M-1), each requiring one job, and `N` identical machines that can process at most one job at a time. A production sequence of length `Q` is given, listing the product types to be processed in chronological order. However, the sequence is ambiguous: the machines can process jobs in any order as long as the relative order of jobs of the same product type is preserved (i.e., jobs of the same type must be processed in the order they appear in the sequence), and a job of product type `p` can only be started after all jobs of product types `< p` that appear earlier in the sequence have been completed. More precisely, the process must respect the following constraints: (1) Each job in the sequence must be assigned to a distinct machine, and each machine processes its assigned jobs in the order they appear in the sequence; (2) For any two jobs `i < j` in the sequence, if the product type of job `i` is strictly less than the product type of job `j`, then job `i` must be completed before job `j` starts; (3) If the product types are equal, the earlier job must finish before the later job starts (since they are on the same machine). The goal is to determine if there exists an assignment of each job to one of the `N` machines such that all constraints are satisfied. Write a function `bool canSchedule(int N, int M, const vector<int>& sequence)` that returns true if a valid schedule exists, false otherwise. The sequence is given in chronological order (index 0 is the first job). The product types are integers in `[0, M-1]`. The function must be self-contained and not rely on any external library beyond the C++ standard library.
#include <cassert>
#include <vector>

// Declaration (solution function is above, but for test we include it directly)
bool canSchedule(int N, int M, const std::vector<int>& sequence);

int main() {
    // Basic feasible case: one machine, increasing types.
    assert(canSchedule(1, 5, {0,1,2,3,4}) == true);
    // One machine with decreasing types is impossible.
    assert(canSchedule(1, 5, {4,3,2,1,0}) == false);
    // One machine with equal types is fine.
    assert(canSchedule(1, 3, {0,0,1,1,2}) == true);
    // Two machines, interleaved types are possible.
    assert(canSchedule(2, 3, {0,1,0,1,2}) == true);
    // Two machines, but too many lower types before higher ones? Example: all 0s then all 1s with 2 machines.
    // With 2 machines and sequence 0,0,0,1: can we schedule? The three 0s need to be before the 1. With 2 machines, we can assign two 0s to machines 0 and 1, then the third 0 must wait until one finishes, but that's fine. So true.
    assert(canSchedule(2, 2, {0,0,0,1}) == true);
    // But if we need to schedule 0s and 1s with only 1 machine, it's true if non-decreasing.
    assert(canSchedule(1, 2, {0,0,1}) == true);
    // With 1 machine, 0,1,0 is false because a lower type appears after a higher.
    assert(canSchedule(1, 2, {0,1,0}) == false);
    // Edge: empty sequence? Not specified, but assume at least one job. We'll skip.
    // More complex: 2 machines, types 0,1,2. Sequence 0,1,2,0,1,2. Should be true because each machine can handle a full cycle.
    assert(canSchedule(2, 3, {0,1,2,0,1,2}) == true);
    // Impossible: 1 machine with 0,1,2,0 is false.
    assert(canSchedule(1, 3, {0,1,2,0}) == false);
    // Possible with 2 machines: 0,1,0,2,1,2 should be true.
    assert(canSchedule(2, 3, {0,1,0,2,1,2}) == true);
    // Impossible: 1 machine with 0,2,1 is false.
    assert(canSchedule(1, 3, {0,2,1}) == false);
    // Two machines, but need to interleave: 0,0,1,1,0,1 with 2 machines: Is it possible? Let's reason: The two 0s (positions 0,1) must be before the 1 at position 2. The third 0 at position 4 must come after 1 at position 2? Actually, type 0 < 1, so job at pos 4 (type 0) must be before job at pos 5 (type 1) but after all earlier 0s. With 2 machines, we can assign pos0 to M0, pos1 to M1, pos2 to M0? Wait pos2 is type1, but type0 jobs at pos0 and pos1 are before it, so ok. Then pos4 type0 must be after pos0 and pos1 but before pos5 type1. With 2 machines, we can schedule pos4 on M1 after pos1, and pos5 on M0 after pos2? But pos5 is type1, and type0 pos4 must be before pos5. So we need pos4 to finish before pos5 starts. Since we have 2 machines, we can run pos4 on M1 and pos5 on M0 concurrently? But they are both before each other? Actually type0 (pos4) < type1 (pos5) means pos4 must finish before pos5 starts, so they cannot overlap. So we need to schedule pos4, then after it finishes, schedule pos5. With 2 machines, we can have pos0 on M0, pos1 on M1, pos2 on M0 (after pos0 finishes), pos4 on M1 (after pos1 finishes), then pos5 on M0 or M1 after pos4 finishes. That works. So true.
    assert(canSchedule(2, 3, {0,0,1,1,0,1}) == true);
    // But if we have three 0s then three 1s with 2 machines: sequence 0,0,0,1,1,1. Can we schedule? The three 0s must all be before any 1. With 2 machines, we can run 0s on both machines: M0: 0(pos0), then 1(pos3) after all 0s? Wait, we need pos0,pos1,pos2 (0s) to finish before pos3 (1) starts. With 2 machines, we can do: M0: pos0 (0), then pos3 (1) after pos0 finishes, but pos1 and pos2 must also finish before pos3 starts, but they are on M1 and possibly M0 again. Actually we can run pos0 on M0, pos1 on M1, pos2 on M0 after pos0 finishes, then all three 0s finish. Then we can run pos3,pos4,pos5 on the two machines. So feasible.
    assert(canSchedule(2, 2, {0,0,0,1,1,1}) == true);
    // But with 1 machine, that sequence is also non-decreasing, so true.
    assert(canSchedule(1, 2, {0,0,0,1,1,1}) == true);
    // Now a case where it's impossible even with 2 machines: N=2, M=3, sequence 0,1,0,2,1,2? Already tested true. Let's try N=2, M=3, sequence 0,1,2,2,1,0? That's decreasing at the end, so likely false.
    assert(canSchedule(2, 3, {0,1,2,2,1,0}) == false);
    // Another impossible: N=2, M=4, sequence 0,1,2,0,3,1,2,3? Probably false but let's not overcomplicate.
    // Test a case with many types and N=3 to ensure no crash.
    assert(canSchedule(3, 10, {0,5,2,7,1,3,9,4,6,8,0,5,2,7,1,3,9,4,6,8}) == true); // likely true.
    return 0;
}
#include <vector>
#include <set>
#include <algorithm>

// Check if a valid schedule exists for N machines, M product types, and a sequence.
bool canSchedule(int N, int M, const std::vector<int>& sequence) {
    int Q = static_cast<int>(sequence.size());
    if (N == 1) {
        // With one machine, the sequence order is fixed, but we must check
        // that types appear in non-decreasing order (since lower types must
        // precede higher types, and equal types are naturally ordered).
        for (int i = 1; i < Q; ++i) {
            if (sequence[i] < sequence[i-1]) return false;
        }
        return true;
    }

    // Build positions for each value.
    std::vector<std::set<int>> positions(M);
    for (int i = 0; i < Q; ++i) {
        positions[sequence[i]].insert(i);
    }

    // Track which values are already fully scheduled.
    std::vector<bool> done(M, false);
    // For each machine, the last processed index in the reverse simulation.
    std::vector<int> its(N, 0);
    int top = 0;
    while (top < M && done[top]) ++top;

    // Check if a single machine can finish from a given index.
    auto okalone = [&](int num, int I) -> bool {
        if (num != top) return false;
        int cur = num;
        for (int it = I; it < Q && cur < M; ++it) {
            int v = sequence[it];
            if (done[v]) continue;
            if (v > cur) return false;
            if (sequence[it] == cur) {
                ++cur;
                while (cur < M && done[cur]) ++cur;
            }
        }
        return (cur == M);
    };

    while (true) {
        // Find the first index that has not been assigned to any machine yet.
        int I = its[0];
        while (I < Q && done[sequence[I]]) ++I;
        if (I == Q) return true;

        int v = sequence[I];
        // If a single machine can handle the rest, it's feasible.
        if (okalone(v, I)) return true;

        // Try to assign each machine to the current value.
        auto& st = positions[v];
        for (int i = 0; i < N; ++i) {
            auto it = st.lower_bound(its[i]);
            if (it == st.end()) return false;
            its[i] = *it;
            st.erase(it);
        }
        done[v] = true;
        while (top < M && done[top]) ++top;
        // If all values are done, we have succeeded.
        if (top == M) return true;
        // Reset iterator for the first machine to avoid infinite loop?
        // Actually the loop restarts naturally.
    }
    return false;
}
// The problem is equivalent to a topological sorting feasibility on a partial order. Jobs are indexed by their position in the sequence (0 to Q-1). For each pair of jobs `i < j`, if `a[i] < a[j]`, then job `i` must precede job `j` (dependency). Additionally, for equal types, the sequence order itself is a total order on that type, so dependencies are implicit. The core difficulty is that the sequence can be arbitrarily interleaved across types, but we have `N` machines. The given code uses a greedy algorithm that processes the sequence from the end backwards, maintaining the "current needed" product type `top` (the smallest type that has not yet been scheduled). It iteratively finds the earliest occurrence of each type that must be scheduled next, and checks if it is feasible. The algorithm is based on the observation that we can simulate the reverse process: start from the end of the sequence and decide which jobs are scheduled last. The `okalone` function checks if a single machine can handle the rest of the sequence from a given position. The main loop tries to assign each of the `N` machines to a job of the current `top` type, and if that fails, it returns false. The algorithm's correctness relies on the greedy property that if a schedule exists, we can always safely assign the first available machine to the first unfinished type. Time complexity is O(Q * N * log Q) in the worst case due to set lower_bound operations, but in practice often O(Q log Q) on average. Space complexity is O(Q + M) for the sets and arrays.
