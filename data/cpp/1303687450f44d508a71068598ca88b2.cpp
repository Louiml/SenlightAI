Write a C++ function `computeSJFStats` that takes a vector of processes, where each process is represented by a struct containing `pid` (char), `burstTime` (int), and `arrivalTime` (int). The function should schedule these processes using the Shortest Job First (SJF) non-preemptive algorithm: at any time, among all processes that have arrived, select the one with the smallest burst time (ties broken by stable order of input). The function must return a struct containing: a vector of scheduled processes in execution order (each with computed `waitTime`), the average waiting time, and the average turn-around time (both as floats). Assume all process IDs are unique uppercase letters, burst times are positive integers, arrival times are non-negative integers, and all processes will eventually be scheduled. The implementation must be self-contained, use `const` correctly, and handle the general case where multiple processes may arrive at different times (including time 0). Do not modify the input vector.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple case, all arrive at 0, sorted by burst.
    {
        std::vector<Process> procs = {
            {'A', 4, 0}, {'B', 2, 0}, {'C', 1, 0}
        };
        SJFResult res = computeSJFStats(procs);
        assert(res.scheduled.size() == 3);
        assert(res.scheduled[0].pid == 'C');
        assert(res.scheduled[1].pid == 'B');
        assert(res.scheduled[2].pid == 'A');
        assert(res.scheduled[0].waitTime == 0);
        assert(res.scheduled[1].waitTime == 1);
        assert(res.scheduled[2].waitTime == 3);
        assert(std::abs(res.avgWaitTime - 4.0f/3.0f) < 1e-5);
        assert(std::abs(res.avgTurnAroundTime - (1+3+7)/3.0f) < 1e-5);
    }
    // Test 2: Processes with different arrival times, idle gaps.
    {
        std::vector<Process> procs = {
            {'A', 5, 0},
            {'B', 2, 5},
            {'C', 3, 10}
        };
        SJFResult res = computeSJFStats(procs);
        assert(res.scheduled[0].pid == 'A');
        assert(res.scheduled[1].pid == 'B');
        assert(res.scheduled[2].pid == 'C');
        assert(res.scheduled[0].waitTime == 0);
        assert(res.scheduled[1].waitTime == 5 - 5 == 0);
        assert(res.scheduled[2].waitTime == 7 - 10 < 0 ? 0 : 0);
        assert(res.scheduled[2].waitTime == 0);
        assert(std::abs(res.avgWaitTime - 0.0f) < 1e-5);
        assert(std::abs(res.avgTurnAroundTime - (5+2+3)/3.0f) < 1e-5);
    }
    // Test 3: Tie-breaking by stable order (same burst, same arrival).
    {
        std::vector<Process> procs = {
            {'X', 3, 0},
            {'Y', 3, 0},
            {'Z', 3, 0}
        };
        SJFResult res = computeSJFStats(procs);
        assert(res.scheduled[0].pid == 'X');
        assert(res.scheduled[1].pid == 'Y');
        assert(res.scheduled[2].pid == 'Z');
        assert(res.scheduled[0].waitTime == 0);
        assert(res.scheduled[1].waitTime == 3);
        assert(res.scheduled[2].waitTime == 6);
    }
    // Test 4: Later arrival but shorter burst; should not preempt, but selected when available.
    {
        std::vector<Process> procs = {
            {'A', 10, 0},
            {'B', 1, 5}
        };
        SJFResult res = computeSJFStats(procs);
        assert(res.scheduled[0].pid == 'A');
        assert(res.scheduled[1].pid == 'B');
        assert(res.scheduled[0].waitTime == 0);
        assert(res.scheduled[1].waitTime == 10 - 5 == 5);
        assert(std::abs(res.avgWaitTime - 2.5f) < 1e-5);
    }
    // Test 5: Single process.
    {
        std::vector<Process> procs = {{'P', 7, 2}};
        SJFResult res = computeSJFStats(procs);
        assert(res.scheduled.size() == 1);
        assert(res.scheduled[0].pid == 'P');
        assert(res.scheduled[0].waitTime == 0);
        assert(std::abs(res.avgWaitTime - 0.0f) < 1e-5);
        assert(std::abs(res.avgTurnAroundTime - 7.0f) < 1e-5);
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <numeric>

// Process structure for SJF scheduling.
struct Process {
    char pid;
    int burstTime;
    int arrivalTime;
    int waitTime;
    int turnAroundTime;
    int originalIndex; // for stable tie-breaking
};

// Result structure returned by computeSJFStats.
struct SJFResult {
    std::vector<Process> scheduled; // in execution order, with waitTime/turnAroundTime set
    float avgWaitTime;
    float avgTurnAroundTime;
};

// Compute SJF non-preemptive scheduling stats.
SJFResult computeSJFStats(const std::vector<Process>& processes) {
    int n = static_cast<int>(processes.size());
    // Make a copy with original indices for stable tie-breaking.
    std::vector<Process> sorted(processes);
    for (int i = 0; i < n; ++i) {
        sorted[i].originalIndex = i;
        sorted[i].waitTime = 0;
        sorted[i].turnAroundTime = 0;
    }
    // Sort by arrival time, stable (equal arrivals keep input order).
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const Process& a, const Process& b) {
                         return a.arrivalTime < b.arrivalTime;
                     });

    std::vector<Process> scheduled;
    scheduled.reserve(n);
    std::vector<bool> selected(n, false);
    int currentTime = 0;
    int totalWait = 0;
    int totalTurnAround = 0;

    for (int completed = 0; completed < n; ++completed) {
        // Find the index of the process with smallest burst among arrived ones.
        int bestIdx = -1;
        for (int i = 0; i < n; ++i) {
            if (!selected[i] && sorted[i].arrivalTime <= currentTime) {
                if (bestIdx == -1 || sorted[i].burstTime < sorted[bestIdx].burstTime ||
                    (sorted[i].burstTime == sorted[bestIdx].burstTime &&
                     sorted[i].originalIndex < sorted[bestIdx].originalIndex)) {
                    bestIdx = i;
                }
            }
        }
        // If no process has arrived yet, jump to the next arrival.
        if (bestIdx == -1) {
            // Find the earliest arrival among remaining.
            int minArrival = INT32_MAX;
            for (int i = 0; i < n; ++i) {
                if (!selected[i] && sorted[i].arrivalTime < minArrival) {
                    minArrival = sorted[i].arrivalTime;
                }
            }
            currentTime = minArrival;
            // Re-scan to pick the best now (at least one exists).
            for (int i = 0; i < n; ++i) {
                if (!selected[i] && sorted[i].arrivalTime <= currentTime) {
                    if (bestIdx == -1 || sorted[i].burstTime < sorted[bestIdx].burstTime ||
                        (sorted[i].burstTime == sorted[bestIdx].burstTime &&
                         sorted[i].originalIndex < sorted[bestIdx].originalIndex)) {
                        bestIdx = i;
                    }
                }
            }
        }
        // Schedule the selected process.
        Process& p = sorted[bestIdx];
        int wait = std::max(0, currentTime - p.arrivalTime);
        p.waitTime = wait;
        p.turnAroundTime = wait + p.burstTime;
        currentTime = std::max(currentTime, p.arrivalTime) + p.burstTime;
        totalWait += wait;
        totalTurnAround += p.turnAroundTime;
        selected[bestIdx] = true;
        scheduled.push_back(p);
    }

    SJFResult result;
    result.scheduled = std::move(scheduled);
    result.avgWaitTime = static_cast<float>(totalWait) / n;
    result.avgTurnAroundTime = static_cast<float>(totalTurnAround) / n;
    return result;
}

// The solution first sorts a copy of the input processes by arrival time while preserving the original input order for processes with equal arrival times (stable sort). The main scheduling loop simulates time starting at 0. At each step, it scans the remaining unscheduled processes and selects from those whose arrival time ≤ current time the one with the smallest burst time (using a tie-breaker by original index to maintain stability). The selected process is moved to the result. Its waiting time is `max(0, currentTime - arrivalTime)` because if the process arrived after the current time, it would have waited zero and the current time jumps forward. The current time is updated to `max(currentTime, arrivalTime) + burstTime`. The turn-around time for each process is stored as waiting time plus burst time. After scheduling all, averages are computed by summing and dividing by the number of processes. Important edge cases: if a process arrives after the last scheduled process finishes, the current time jumps to its arrival time (no idle waiting counted). If multiple processes have the same burst time, the one that arrived earlier (or appears earlier in the input) is chosen first. Complexity is O(n²) time due to the linear scan for each of the n processes, and O(n) auxiliary space for the sorted copy and result. The reference implementation avoids modifying the input.
