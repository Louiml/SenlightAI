/*
Write a C++ function `bool isSafeBankers(const std::vector<std::vector<int>>& allocation, const std::vector<std::vector<int>>& max, const std::vector<int>& available)` that determines whether a system of `n` processes and `m` resource types is in a safe state according to the Banker's Algorithm. The input matrices are given with `allocation[i][j]` = number of resources of type `j` currently allocated to process `i`, `max[i][j]` = maximum demand of process `i` for resource `j`, and `available[j]` = number of currently available resources of type `j`. The function must return `true` if there exists a safe sequence (i.e., all processes can finish without deadlock), and `false` otherwise. Assume `allocation[i][j] <= max[i][j]` for all `i,j`, and all inputs are non-negative integers. The function must be `const`-correct (accept constant references) and must not modify the inputs. It should handle the edge case where some processes already have all their resources (Need = 0) and can finish immediately, and the case where no process can be satisfied initially (system unsafe). Do not print any output; just return the boolean result.
*/
#include <vector>

// Determine if the system is in a safe state using the Banker's Algorithm.
bool isSafeBankers(const std::vector<std::vector<int>>& allocation,
                   const std::vector<std::vector<int>>& max,
                   const std::vector<int>& available) {
    int n = allocation.size();
    if (n == 0) return true; // no processes, trivially safe
    int m = available.size();
    if (m == 0) return true; // no resource types, trivial

    std::vector<int> work = available; // copy of available
    std::vector<bool> finish(n, false);

    int finishedCount = 0;
    bool progress;
    do {
        progress = false;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                // Check if process i can be satisfied with current Work
                bool canAllocate = true;
                for (int j = 0; j < m; ++j) {
                    int need = max[i][j] - allocation[i][j];
                    if (need > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    // Simulate finishing process i
                    for (int j = 0; j < m; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    finishedCount++;
                    progress = true;
                }
            }
        }
    } while (progress && finishedCount < n);

    return finishedCount == n;
}
#include <cassert>
#include <vector>

// The solution function is declared here (already included in the same file in a real test).
bool isSafeBankers(const std::vector<std::vector<int>>& allocation,
                   const std::vector<std::vector<int>>& max,
                   const std::vector<int>& available);

int main() {
    // Example from classic Banker's Algorithm (5 processes, 3 resources) – safe state.
    std::vector<std::vector<int>> alloc1 = {{0,1,0},{2,0,0},{3,0,2},{2,1,1},{0,0,2}};
    std::vector<std::vector<int>> max1 = {{7,5,3},{3,2,2},{9,0,2},{2,2,2},{4,3,3}};
    std::vector<int> avail1 = {3,3,2};
    assert(isSafeBankers(alloc1, max1, avail1) == true);

    // Same but with reduced available resources – unsafe state.
    std::vector<int> avail2 = {1,0,1};
    assert(isSafeBankers(alloc1, max1, avail2) == false);

    // Single process with zero need – trivially safe.
    std::vector<std::vector<int>> alloc3 = {{1,1}};
    std::vector<std::vector<int>> max3 = {{1,1}};
    std::vector<int> avail3 = {0,0};
    assert(isSafeBankers(alloc3, max3, avail3) == true);

    // Single process with unmet need – unsafe.
    std::vector<std::vector<int>> alloc4 = {{1,0}};
    std::vector<std::vector<int>> max4 = {{2,2}};
    std::vector<int> avail4 = {0,1};
    assert(isSafeBankers(alloc4, max4, avail4) == false);

    // Two processes where one can finish then the other can – safe.
    std::vector<std::vector<int>> alloc5 = {{0,0},{1,0}};
    std::vector<std::vector<int>> max5 = {{1,0},{2,1}};
    std::vector<int> avail5 = {1,1};
    assert(isSafeBankers(alloc5, max5, avail5) == true);

    // Two processes both waiting for the same resource – unsafe.
    std::vector<std::vector<int>> alloc6 = {{0,1},{0,1}};
    std::vector<std::vector<int>> max6 = {{1,1},{1,1}};
    std::vector<int> avail6 = {1,0};
    assert(isSafeBankers(alloc6, max6, avail6) == false);

    // Edge case: empty process list – safe.
    assert(isSafeBankers({}, {}, {}) == true);

    // Edge case: zero resources – safe regardless of processes.
    assert(isSafeBankers({{0},{0}}, {{2},{1}}, {}) == true);

    return 0;
}
// The solution uses the standard Banker's Algorithm safety check:  
// 1. Compute the `Need` matrix as `Need[i][j] = max[i][j] - allocation[i][j]`.  
// 2. Initialize `Work = available` and a boolean vector `Finish` of size `n` all `false`.  
// 3. Loop until no progress is made in an iteration:  
//    - Find an index `i` such that `Finish[i] == false` and for every resource `j`, `Need[i][j] <= Work[j]`.  
//    - If found, simulate finishing process `i` by adding its allocation to `Work` (`Work[j] += allocation[i][j]`) and set `Finish[i] = true`. Count finished processes.  
//    - If no such `i` exists in a full iteration, break.  
// 4. The system is safe if and only if all processes are finished (`finished == n`).  
// Edge cases:  
// - A process may have all zero needs (already finished); it can be selected immediately.  
// - If any `Need[i][j] > available[j]` for all remaining processes, system is unsafe.  
// - Duplicate or zero-size inputs: if `n==0` or `m==0`, the function should return `true` (trivially safe).  
// Time complexity: The outer loop runs at most `n` times (each successful selection finishes one process), and each scan checks `n` processes with `m` resource comparisons, giving `O(n^2 * m)`. Space complexity: `O(n*m)` for the Need matrix (or we could compute on the fly), but we can also compute Need without storing it if we compare `max[i][j] - allocation[i][j]` directly, reducing to `O(n)` for the Finish and Work vectors. We'll store Need for clarity, but we can avoid extra memory by computing on the fly; the provided solution will compute Need on the fly to keep auxiliary space `O(n + m)`.
