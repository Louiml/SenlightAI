Write a C++ function `simulateRoundRobin` that takes a vector of tuples `(arrival_time, burst_time)`, a time quantum `q`, and a context switch overhead `overhead` as parameters, and returns a `SimulationStats` struct (containing `completed_processes`, `avg_turnaround_time`, and `avg_waiting_time` as doubles). The function must simulate a round-robin CPU scheduling algorithm with the following rules: processes arrive at their specified arrival times and are placed into a ready queue in arrival order (ties broken by lower arrival time then lower original index). The CPU runs a single process for at most `q` time units consecutively; if a process finishes within its quantum, it terminates immediately; otherwise, it is preempted and moved to the back of the ready queue. After every preemption or process completion (including when the CPU becomes idle and needs to fetch a new process), the simulator incurs a context switch overhead of `overhead` time units (added to the current time before the next execution step). If no process is ready, the CPU idles (time advances by 1 per unit without overhead). The simulation ends when all processes have completed. The function must compute the average turnaround time (completion time − arrival time) and average waiting time (turnaround time − burst time) over all processes. Only consider processes with non-negative burst times; ignore any process with a negative burst time. The function should handle an empty input by returning a `SimulationStats` with all fields set to 0.0.

#include <cassert>
#include <tuple>
#include <vector>

// The solution function is assumed to be included above.

int main() {
    // Single process, no overhead
    {
        std::vector<std::tuple<int,int>> procs = {{0, 5}};
        SimulationStats s = simulateRoundRobin(procs, 2, 0);
        assert(s.completed_processes == 1);
        assert(s.avg_turnaround_time == 5.0);
        assert(s.avg_waiting_time == 0.0);
    }

    // Multiple processes, simple round-robin with no overhead
    {
        std::vector<std::tuple<int,int>> procs = {{0, 3}, {0, 3}};
        SimulationStats s = simulateRoundRobin(procs, 1, 0);
        // Timeline: t0-1 P0, t1-2 P1, t2-3 P0, t3-4 P1, t4-5 P0? Actually P0 finishes at 5? Let's compute:
        // P0 runs 0-1, P1 runs 1-2, P0 runs 2-3, P1 runs 3-4, P0 runs 4-5 (completes at 5), P1 finishes at 6? Actually after P0 completes at 5, P1 runs 5-6. So completion times: P0=5, P1=6. Turnaround: 5,6 avg=5.5; waiting: 5-3=2, 6-3=3 avg=2.5.
        assert(s.completed_processes == 2);
        assert(s.avg_turnaround_time == 5.5);
        assert(s.avg_waiting_time == 2.5);
    }

    // With context switch overhead
    {
        std::vector<std::tuple<int,int>> procs = {{0, 2}, {1, 2}};
        SimulationStats s = simulateRoundRobin(procs, 1, 1);
        // Timeline with overhead 1:
        // t=0: add P0, start P0 (overhead 1 -> t=1), run P0 for 1 unit (t=1 to 2) remaining=1, preempt at quantum? run_units=1 == quantum -> preempt P0 to back, time=2. add arrivals at t=2? P1 arrives at t=1, so at start of iteration t=1 (before execution) we add P1? Actually our simulation adds arrivals at the beginning of loop. Let's manually simulate:
        // Start: time=0, add all arrivals with arrival<=0: P0. ready=[0], current=-1.
        // Loop: current=-1, ready nonempty -> start P0, time+=1 -> time=1, add arrivals with arrival<=1: P1 arrives at 1 -> ready=[1], continue loop (current=0).
        // Loop: current=0, execute one unit: remaining 2->1, run_units=1==quantum -> preempt P0 to back (ready now [1,0]), current=-1, time=2. add arrivals with arrival<=2: none.
        // Loop: current=-1, ready nonempty -> start P1 (front is 1), time+=1 -> time=3, add arrivals <=3: none, continue.
        // Loop: execute P1 one unit: remaining 2->1, run_units=1==q -> preempt to back (ready [0,1]), time=4, add arrivals none.
        // Loop: start P0 again: time+=1 ->5, execute one unit: remaining 1->0, completes at time=5+1=6? Wait execution: at time=5, we start P0, then execute one unit: we execute from time=5 to 6, so completion time=6. But we incremented time to 5 before execution? Let's check: In loop, we start process by time+=overhead (time becomes 5), then continue, then in next loop iteration we execute one unit: we do procs[idx].remaining--, then time++ -> time becomes 6. So completion time = time after the unit? Actually we set completion = time+1 before incrementing time. In our code, when remaining hits 0, we set completion = time + 1 (where time is the current time at the start of the execution unit). So for P0's last unit, time at start of that unit was 5? Let's trace with code logic. It’s messy. Better to just trust the code and test with expected values from a known correct simulation. Let's compute manually with the intended semantics: Overhead 1, quantum 1, two processes arriving at 0 (P0 burst 2) and 1 (P1 burst 2).
        // Standard round-robin with overhead: At t=0, P0 arrives. CPU idle, so context switch to P0, overhead 1 => P0 starts at t=1, runs for 1 unit to t=2 (remaining=1). P1 arrives at t=1 (during P0's run). At t=2, P0 preempted, context switch to P1 (overhead 1) => P1 starts at t=3, runs to t=4 (remaining=1). Then context switch to P0 (overhead 1) => starts t=5, runs to t=6 finishes. Then context switch to P1 (overhead 1) => starts t=7, runs to t=8 finishes. Completion times: P0=6, P1=8. Turnaround: P0=6-0=6, P1=8-1=7 => avg=6.5. Waiting: P0=6-2=4, P1=7-2=5 => avg=4.5.
        SimulationStats s = simulateRoundRobin(procs, 1, 1);
        assert(s.completed_processes == 2);
        assert(s.avg_turnaround_time == 6.5);
        assert(s.avg_waiting_time == 4.5);
    }

    // Negative burst ignored
    {
        std::vector<std::tuple<int,int>> procs = {{0, -1}, {2, 3}};
        SimulationStats s = simulateRoundRobin(procs, 2, 0);
        assert(s.completed_processes == 1);
        assert(s.avg_turnaround_time == 3.0); // arrival 2, burst 3 -> finishes at 5, tt=3
        assert(s.avg_waiting_time == 0.0);
    }

    // Zero burst process
    {
        std::vector<std::tuple<int,int>> procs = {{0, 0}, {1, 2}};
        SimulationStats s = simulateRoundRobin(procs, 1, 0);
        // P0 completes at 0, P1 runs 1 to 3, tt=2, wt=0. Avg tt=(0+2)/2=1, avg wt=0.
        assert(s.completed_processes == 2);
        assert(s.avg_turnaround_time == 1.0);
        assert(s.avg_waiting_time == 0.0);
    }

    // Empty input
    {
        std::vector<std::tuple<int,int>> procs;
        SimulationStats s = simulateRoundRobin(procs, 1, 0);
        assert(s.completed_processes == 0);
        assert(s.avg_turnaround_time == 0.0);
        assert(s.avg_waiting_time == 0.0);
    }

    return 0;
}

#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <limits>

struct SimulationStats {
    double completed_processes;
    double avg_turnaround_time;
    double avg_waiting_time;
};

// Simulate round-robin CPU scheduling with context switch overhead.
// Processes are given as (arrival_time, burst_time). Negative burst times are ignored.
// Returns average turnaround and waiting times over all completed processes.
SimulationStats simulateRoundRobin(const std::vector<std::tuple<int,int>>& processes, int quantum, int overhead) {
    // Collect valid processes: burst >= 0
    struct Proc {
        int arrival;
        int burst;
        int remaining;
        int completion;
        int id;
    };
    std::vector<Proc> procs;
    procs.reserve(processes.size());
    int valid_count = 0;
    for (const auto& [arr, burst] : processes) {
        if (burst >= 0) {
            procs.push_back({arr, burst, burst, 0, valid_count});
            valid_count++;
        }
    }

    // Sort by arrival time, then by original id
    std::sort(procs.begin(), procs.end(), [](const Proc& a, const Proc& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.id < b.id;
    });

    std::deque<int> ready_queue; // indices into procs
    int next_arrival_idx = 0;
    int current_idx = -1; // index of running process, -1 if none
    int current_run_units = 0;
    int time = 0;
    int total_completed = 0;

    auto add_arrivals = [&](int t) {
        while (next_arrival_idx < (int)procs.size() && procs[next_arrival_idx].arrival <= t) {
            // Zero-burst processes complete instantly at arrival
            if (procs[next_arrival_idx].burst == 0) {
                procs[next_arrival_idx].completion = procs[next_arrival_idx].arrival;
                total_completed++;
            } else {
                ready_queue.push_back(next_arrival_idx);
            }
            next_arrival_idx++;
        }
    };

    // Handle zero-burst processes that arrive at time 0 and any early arrivals
    add_arrivals(time);

    while (total_completed < valid_count) {
        // If no current process and idle, jump to next arrival if any
        if (current_idx == -1 && ready_queue.empty()) {
            if (next_arrival_idx < (int)procs.size()) {
                time = std::max(time, procs[next_arrival_idx].arrival);
                add_arrivals(time);
                continue;
            } else {
                // should not happen if all incomplete have burst >0 and all arrivals added
                break;
            }
        }

        // If no current process, start one (with context switch overhead)
        if (current_idx == -1) {
            if (!ready_queue.empty()) {
                current_idx = ready_queue.front();
                ready_queue.pop_front();
                current_run_units = 0;
                time += overhead;
                // During overhead, new processes may arrive; add them
                add_arrivals(time);
                continue; // go to top to handle new arrivals and start execution
            }
            // If ready_queue empty but no current, we handled idle jump above, so continue
            continue;
        }

        // Execute one time unit for current process
        procs[current_idx].remaining--;
        current_run_units++;

        // Check if process finishes
        if (procs[current_idx].remaining == 0) {
            // Completion time is the end of this unit: we executed from time to time+1
            procs[current_idx].completion = time + 1;
            current_idx = -1;
            current_run_units = 0;
            total_completed++;
        } else {
            // Check quantum expiry
            if (current_run_units == quantum) {
                // Preempt: move to back of ready queue
                ready_queue.push_back(current_idx);
                current_idx = -1;
                current_run_units = 0;
            }
        }

        time++; // advance one unit

        // At start of next iteration, we'll add arrivals with arrival_time <= new time
        add_arrivals(time);
    }

    // Compute averages
    SimulationStats stats;
    stats.completed_processes = valid_count;
    double sum_tt = 0.0;
    double sum_wt = 0.0;
    for (const auto& p : procs) {
        int tt = p.completion - p.arrival;
        int wt = tt - p.burst;
        sum_tt += tt;
        sum_wt += wt;
    }
    stats.avg_turnaround_time = (valid_count > 0) ? sum_tt / valid_count : 0.0;
    stats.avg_waiting_time = (valid_count > 0) ? sum_wt / valid_count : 0.0;

    return stats;
}

// We implement a round-robin scheduler using a queue of process indices (or shared pointers) and a current time variable. At each step, we first add to the ready queue all processes whose arrival time equals the current time (processes are added in order of their index in the input vector, which preserves arrival-time tie-breaking if we sort by arrival time first, but the problem states arrivals are given in arbitrary order; we should sort by arrival time, then original index). Then we check if a process is currently running. We simulate execution in quantum-sized chunks: for the currently running process, we execute min(q, remaining_time) units, advancing time by that many units. During this execution, new processes may arrive; we need to add them to the ready queue at their exact arrival times. So we must advance time step-by-step or handle arrival events carefully. A simpler approach: maintain a priority queue (or min-heap) of arrival events, and at each loop iteration, we determine the next time when either (a) the current quantum ends, (b) the current process finishes, or (c) a new process arrives. We jump time to the earliest of these events, handle arrivals, and update state accordingly. However, a simpler but still correct approach is to simulate time tick by tick, but that would be inefficient for long burst times. To keep it simple and correct, we can use an event-driven simulation: we have a ready queue (deque) of process IDs. We maintain an array of remaining times and completion times. The algorithm: sort processes by arrival time (and original index). Set current_time = 0. Have a pointer to the next arriving process. While any process incomplete: first, add all processes with arrival_time <= current_time (that have not been added yet) to the ready queue. If ready queue is empty and no process is currently running, advance current_time to the next arrival time (or if no arrivals, break). If a process is running, we simulate its execution for up to q units. But to handle arrivals during that quantum, we need to know the next arrival time; if the next arrival occurs before the quantum end, we must preempt at that arrival time and add the arriving process, then optionally continue the current process (but in round-robin, we would put the current process back to the back of the queue if it hasn't finished). This is a typical event-driven round-robin implementation. For simplicity, we can implement a time-driven simulation with a small step (1 unit) but that is O(total burst time) which may be large but acceptable for a typical exercise. The problem statement does not impose performance constraints, so a straightforward simulation incrementing time by 1 unit each iteration is acceptable and much simpler, but we must account for context switch overhead correctly: when the CPU becomes idle (no process running) and we need to select a next process from the ready queue, we add overhead to current_time before that process starts executing. Similarly, when a running process is preempted (quantum expires) and we put it back, we incur overhead. Also when a process completes, we incur overhead to select the next process (if any). The overhead is added whenever we transition from having no current process to running one (i.e., we fetch a new process from the ready queue). So in a per-unit time loop: at the start of each iteration, add newly arrived processes (arrival_time == current_time). If current_process is null: if ready queue non-empty, pop next, set as current, add overhead to current_time (and then we need to adjust: we must be careful not to double count time). A common method: at the top of the loop, we have current_time already incremented from previous iteration. We can instead handle overhead by adding it to current_time when we start a new process, but then we need to ensure we don't skip over arrival times during the overhead. Since overhead is a fixed integer, and we add it to current_time, we should then add any processes that arrived during that overhead period (i.e., arrival_time <= new current_time) before executing. This is manageable. The per-unit simulation is easiest to reason about: we loop while not all processes complete. At each iteration, we increment current_time by 1 after performing the actions. We must carefully add overhead before executing the current process. A clean approach: use a while loop that continues until all processes are done. Inside, if current_process is null and ready queue not empty, we pop the next process, set current_process = it, and we need to add overhead time. But we should not increment time by 1 in that same iteration? Let's design: We'll maintain `current_time` as the current simulation time. At the start of each loop, we call `add_arrived_processes(current_time)` (processes with arrival_time == current_time). Then if current_process is null: if ready queue not empty, pop next, set as current, and add overhead to current_time (but we must immediately check if any new processes arrived during that overhead time, so we call add_arrived_processes again for the new current_time). However, this can get messy. A simpler alternative: simulate time in discrete steps of 1 but treat overhead as an extra 1-unit delay before starting a process. We can incorporate overhead as a counter: when we need to start a process, we add `overhead` to a "pending_overhead" variable that we consume one unit per loop iteration. But that would slow execution unnecessarily. Given the task is for a teaching assistant, a per-unit time simulation with explicit overhead additions at the moment of context switch (immediately adding overhead to current_time and then continuing the loop) is acceptable. Let's define the algorithm clearly:
//
// 1. Filter out negative burst times. For each valid process, store arrival_time, burst_time, remaining_time, completion_time.
// 2. Sort processes by arrival_time (and original index for ties).
// 3. Initialize `current_time = 0`, `ready_queue` (deque of indices), `next_arrival_index = 0`, `current_process` = -1.
// 4. While any process has remaining_time > 0:
//    - Add all processes with arrival_time <= current_time that haven't been added yet to the ready queue (in sorted order).
//    - If current_process == -1:
//      - If ready_queue empty: set current_time = max(current_time, arrival_time of next unadded process). Continue loop.
//      - Else: pop front, set current_process = popped index, and add `overhead` to current_time. Then we need to go back to the top of the loop to add any processes that arrived during the overhead period. So we `continue` the loop (or use a flag).
//    - Else (current_process running):
//      - Execute one unit: decrement remaining_time of current_process. (Since we are using per-unit simulation, we only decrement by 1 each iteration, but that is inefficient; we can optimize by executing min(q, remaining) in one step, but for simplicity we stay per-unit.) However, per-unit is fine for a teaching task and correct. Let's implement per-unit to avoid complex event handling. But then overhead is added only when we start a process, which is fine.
//      - Increment current_time by 1 after execution? Actually we need to decide: the standard approach: at the beginning of a loop, we have current_time. We add arrivals. Then if no current process, we start one (with overhead time added). Then we execute the current process for one unit of time, and then increment current_time by 1. That means the process runs from current_time to current_time+1. But if we just added overhead to current_time, we skip that overhead time, so the process runs after the overhead. That is correct. So the loop structure:
//        ```
//        while (not complete) {
//            add_arrivals(current_time);
//            if (current_process == -1) {
//                if (ready_queue empty) { current_time = next_arrival_time; continue; }
//                current_process = ready_queue.front(); ready_queue.pop_front();
//                current_time += overhead;
//                continue; // go back to top to add any arrivals during overhead
//            }
//            // execute one unit
//            remaining_time[current_process]--;
//            if (remaining_time[current_process] == 0) {
//                completion_time[current_process] = current_time + 1; // because we execute from current_time to current_time+1
//                completed_count++;
//                current_process = -1;
//            } else {
//                // check if quantum expired. Since we are per-unit, we need to count how many units the current process has run consecutively. Maintain a `run_units` counter. If run_units == q, preempt: push current_process to back of ready queue, set current_process = -1, and add overhead? Actually overhead is added when we start a new process, not when we preempt. But the problem says "After every preemption or process completion (including when the CPU becomes idle and needs to fetch a new process), the simulator incurs a context switch overhead of overhead time units (added to the current time before the next execution step)." So we should add overhead when we preempt as well, because we switch to the next process. So after preemption or completion, if there is a next process to run, we will add overhead when we start it. If we preempt and immediately there is a new process in the ready queue, we will add overhead when we start the next one (in the next iteration). That matches: the overhead is added before the next execution step, which is when we fetch a new process. So we don't need to add overhead at preemption time separately; we add it when we start a new process. However, if after a preemption the ready queue is empty, we don't incur overhead (no context switch) but the CPU idles. That seems consistent with the statement: overhead is incurred when a context switch happens, i.e., when the CPU switches from one process to another. If it idles, no switch. So we are fine.
//            }
//            current_time++; // move to next time unit
//        }
//        ```
//    This per-unit simulation is correct and simple, though O(total burst time + overhead * number of context switches) which is acceptable for small inputs.
//
// But we can optimize by executing a whole quantum at once. However, the above is simpler to implement correctly. Since this is a teaching task, the solution can be per-unit. We need to handle the `run_units` counter: we need to track how many consecutive units the current process has run. So we maintain `current_run_units`, reset to 0 when we start a process, increment each unit we execute, and if it reaches q, we preempt (push to back of ready queue, set current_process=-1, and also we should set current_run_units=0). Also if the process finishes, we reset. We also need to add arrivals that occur during the one-unit execution. Since we only move time by 1, at the next loop iteration we'll add arrivals with arrival_time == (old current_time + 1) = new current_time. That works.
//
// Complexity: Let n be number of processes, total burst = sum of burst times. In worst case, each unit of CPU execution we do O(1) work, plus each arrival we process once. Number of iterations = total burst time + total idle time + overhead additions (which are not iterations but added to current_time). Since we add overhead by jumping current_time forward, we don't iterate over overhead units. So the loop runs once per unit of CPU execution (i.e., once per unit of burst time). So O(total burst + n log n for sorting). Space O(n).
//
// Edge cases: empty input, all negative bursts, zero burst times (a process with burst 0 should complete immediately at arrival? In per-unit simulation, we would start it, then execute one unit: but with burst 0, remaining becomes -1? We should handle burst 0 by immediately completing at arrival time without running. So before starting, if remaining_time == 0, we should mark completed at arrival time. We'll handle that: when adding a process to ready queue, if its remaining_time == 0, we set completion_time = arrival_time and mark as done, and we do not put it in ready queue. Alternatively, we can filter out zero-burst processes as well? But the problem doesn't say to ignore zero bursts; typical scheduling includes them. We'll handle them explicitly. Also careful with overhead: if we start a process and its burst is 0, we wouldn't incur overhead? Actually if the process has zero burst, it should be completed immediately upon arrival, no CPU time, no context switch? The problem says only processes with negative burst times are ignored. Zero burst is valid. In practice, a process with burst 0 arrives and finishes at its arrival time, without using CPU, and no context switch overhead unless it is the only process? The overhead is incurred when switching to a new process. If we never run it, we don't switch. So we should complete zero-burst processes instantly when they arrive, not adding them to ready queue. For simplicity, we can do that.
//
// Now, the function signature: `SimulationStats simulateRoundRobin(const std::vector<std::tuple<int,int>>& processes, int quantum, int overhead)`.
//
// We need to define `SimulationStats` as a struct with double fields. The solution will be a free function.
