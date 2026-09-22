// Design a C++ function that simulates a simplified single-server queueing system with discrete events, mirroring the logic of the provided code snippet. The function should take as parameters: `numMachines` (total machines), `spareParts` (initial spare parts), `repairers` (number of repairers, but simplify to 1 for this task), `meanRepairTime`, `meanFailureTime`, `simulationTime`, and `iterations`. It should simulate the system event-by-event using an event list ordered by time, tracking five performance metrics accumulated over all iterations: mean failure duration (`DMF`), mean time between system failures (`TMEFS`), mean number of machines in repair (`NMMR`), repairer idle percentage (`TOR`), and system failure duration percentage (`DTF`). After each iteration, reset the system state (initialize events, clocks, counters) and accumulate the computed metrics. Return a `std::array<double, 5>` containing the averaged metrics over all iterations (divide each accumulated sum by `iterations`). Use a fixed random seed (e.g., `srandom(123456)`) and an exponential random number generator with a given mean. Simplify the logic: since there is only one repairer, when a machine fails and the repairer is free, schedule a repair event; otherwise queue the machine. When a repair ends, if there are queued machines, immediately schedule another repair. Manage spare parts: on failure, if spare parts are available, decrement and schedule a new failure for that machine; else the system enters a "system failure" state until a repair completes and a spare part is available. Events are `FAILURE`, `REPAIR_DONE`, and `SIM_END`. Use a list of event structs (`{type, time}`) and sort after each insertion, or use a priority queue. Ensure the function is const-correct and uses only standard headers.

#include <cassert>
#include <cmath>
#include <array>

// Declaration of the solution function (normally in header)
std::array<double, 5> simulateRepairSystem(
    int numMachines,
    int spareParts,
    int repairers,
    double meanRepairTime,
    double meanFailureTime,
    double simulationTime,
    int iterations);

int main() {
    // Test 1: No iterations => all zeros
    auto res0 = simulateRepairSystem(5, 2, 1, 1.0, 10.0, 100.0, 0);
    assert(res0[0] == 0.0 && res0[1] == 0.0 && res0[2] == 0.0 && res0[3] == 0.0 && res0[4] == 0.0);

    // Test 2: Single iteration, very long simulation time, many spare parts => no system failure, repairer idle most of time
    auto res1 = simulateRepairSystem(3, 100, 1, 0.5, 1000.0, 1.0, 1);
    // With mean failure time 1000, no failures in 1 time unit, so NMMR=0, TOR=100, DTF=0, DMF must be 0 (no failures)
    assert(fabs(res1[0]) < 1e-6); // DMF should be 0
    assert(fabs(res1[2]) < 1e-6); // NMMR = 0
    assert(fabs(res1[3] - 100.0) < 1e-6); // TOR = 100% idle
    assert(fabs(res1[4]) < 1e-6); // DTF = 0

    // Test 3: Repeated with same seed gives reproducible results
    auto resA = simulateRepairSystem(4, 1, 1, 0.8, 5.0, 50.0, 3);
    auto resB = simulateRepairSystem(4, 1, 1, 0.8, 5.0, 50.0, 3);
    for (int i = 0; i < 5; ++i) {
        assert(fabs(resA[i] - resB[i]) < 1e-9);
    }

    // Test 4: With very short simulation time, no repairs complete, but failures may occur
    auto res2 = simulateRepairSystem(2, 0, 1, 0.1, 0.01, 0.001, 1);
    // In 0.001 time, likely at least one failure, no repair finished, so NMMR > 0, DTF > 0
    assert(res2[2] > 0.0);
    assert(res2[4] > 0.0);

    // Test 5: With more spare parts than machines, system never enters failure state, so DMF=TMEFS=0
    auto res3 = simulateRepairSystem(5, 10, 1, 0.2, 1.0, 10.0, 2);
    assert(fabs(res3[0]) < 1e-6); // DMF = 0
    assert(fabs(res3[1]) < 1e-6); // TMEFS = 0

    // Test 6: Sanity bound: TOR between 0 and 100
    auto res4 = simulateRepairSystem(3, 1, 1, 0.5, 2.0, 20.0, 5);
    assert(res4[3] >= 0.0 && res4[3] <= 100.0);
    assert(res4[2] >= 0.0 && res4[2] <= 1.0); // single repairer

    // Test 7: If mean repair time is very small, repairer likely less idle (TOR smaller)
    auto resFast = simulateRepairSystem(5, 2, 1, 0.001, 1.0, 20.0, 10);
    auto resSlow = simulateRepairSystem(5, 2, 1, 10.0, 1.0, 20.0, 10);
    assert(resFast[3] < resSlow[3]); // fast repairs => less idle time

    return 0;
}

#include <array>
#include <cmath>
#include <cstdint>
#include <list>
#include <random>

// Event types for the discrete-event simulation
enum EventType { FAILURE = 0, REPAIR_DONE = 1, SIM_END = 2 };

// Event structure: type and scheduled time
struct Event {
    EventType type;
    double time;
};

// Order events by time (ascending)
bool compareEvents(const Event& a, const Event& b) {
    return a.time < b.time;
}

// Exponential random number generator with given mean (uses std::mt19937)
double expRandom(double mean, std::mt19937& rng) {
    std::exponential_distribution<double> dist(1.0 / mean);
    return dist(rng);
}

// Main simulation function: simplified single-repairer queue with spare parts
std::array<double, 5> simulateRepairSystem(
    int numMachines,
    int spareParts,
    int repairers,               // should be 1 for this simplified version
    double meanRepairTime,
    double meanFailureTime,
    double simulationTime,
    int iterations)
{
    // Accumulated metrics over all iterations
    double sumDMF = 0.0;
    double sumTMEFS = 0.0;
    double sumNMMR = 0.0;
    double sumTOR = 0.0;
    double sumDTF = 0.0;

    // Fixed seed for reproducibility
    std::mt19937 rng(123456);

    for (int iter = 0; iter < iterations; ++iter) {
        // System state
        double clock = 0.0;
        bool repairerBusy = false;
        int numInRepair = 0;        // machines currently being repaired (0 or 1)
        int queueLength = 0;        // machines waiting for repair
        int brokenNoSpare = 0;      // machines that failed with no spare part
        bool systemFailure = false; // true when all spare parts exhausted
        double failureStart = 0.0;  // when system failure began
        int systemFailures = 0;     // number of distinct system failures
        double totalFailureDuration = 0.0;
        double lastFailureEnd = 0.0;
        double accBetweenFailures = 0.0;

        // Timing accumulators
        double busyTimeAcc = 0.0;   // total time repairer was busy
        double idleTimeAcc = 0.0;   // total time repairer was idle
        double lastBusyUpdate = 0.0;
        double lastIdleUpdate = 0.0;

        // Event list
        std::list<Event> eventList;

        // Schedule initial failure events for each machine
        for (int i = 0; i < numMachines; ++i) {
            Event e;
            e.type = FAILURE;
            e.time = expRandom(meanFailureTime, rng);
            eventList.push_back(e);
        }

        // Schedule simulation end
        Event endEvent;
        endEvent.type = SIM_END;
        endEvent.time = simulationTime;
        eventList.push_back(endEvent);
        eventList.sort(compareEvents);

        // Main event loop
        while (!eventList.empty()) {
            Event current = eventList.front();
            eventList.pop_front();
            clock = current.time;

            if (current.type == FAILURE) {
                // Update busy time if repairer was busy
                if (repairerBusy) {
                    busyTimeAcc += (clock - lastBusyUpdate) * numInRepair;
                    lastBusyUpdate = clock;
                }

                numInRepair++;

                if (!repairerBusy) {
                    // Repairer becomes busy
                    repairerBusy = true;
                    // Update idle time
                    idleTimeAcc += (clock - lastIdleUpdate) * 1; // repairers=1
                    lastIdleUpdate = clock;

                    Event repairEvent;
                    repairEvent.type = REPAIR_DONE;
                    repairEvent.time = clock + expRandom(meanRepairTime, rng);
                    eventList.push_back(repairEvent);
                    eventList.sort(compareEvents);
                } else {
                    queueLength++;
                }

                // Spare parts logic
                if (spareParts > 0) {
                    spareParts--;
                    Event newFail;
                    newFail.type = FAILURE;
                    newFail.time = clock + expRandom(meanFailureTime, rng);
                    eventList.push_back(newFail);
                    eventList.sort(compareEvents);
                } else {
                    brokenNoSpare++;
                    if (!systemFailure) {
                        systemFailure = true;
                        failureStart = clock;
                        systemFailures++;
                        if (systemFailures > 1) {
                            accBetweenFailures += clock - lastFailureEnd;
                        }
                    }
                }
            }
            else if (current.type == REPAIR_DONE) {
                // Update busy time
                if (repairerBusy) {
                    busyTimeAcc += (clock - lastBusyUpdate) * numInRepair;
                    lastBusyUpdate = clock;
                }

                numInRepair--;

                if (queueLength > 0) {
                    queueLength--;
                    Event nextRepair;
                    nextRepair.type = REPAIR_DONE;
                    nextRepair.time = clock + expRandom(meanRepairTime, rng);
                    eventList.push_back(nextRepair);
                    eventList.sort(compareEvents);
                } else {
                    repairerBusy = false;
                    idleTimeAcc += (clock - lastIdleUpdate) * 1; // repairers=1
                    lastIdleUpdate = clock;
                }

                // Spare parts logic on repair completion
                if (brokenNoSpare == 0) {
                    spareParts++;
                } else {
                    brokenNoSpare--;
                    Event newFail;
                    newFail.type = FAILURE;
                    newFail.time = clock + expRandom(meanFailureTime, rng);
                    eventList.push_back(newFail);
                    eventList.sort(compareEvents);

                    if (brokenNoSpare == 0) {
                        totalFailureDuration += clock - failureStart;
                        systemFailure = false;
                        lastFailureEnd = clock;
                    }
                }
            }
            else if (current.type == SIM_END) {
                // Finalize timing accumulators
                if (repairerBusy) {
                    busyTimeAcc += (clock - lastBusyUpdate) * numInRepair;
                } else {
                    idleTimeAcc += (clock - lastIdleUpdate) * 1; // repairers=1
                }

                if (systemFailure) {
                    totalFailureDuration += clock - failureStart;
                }

                // Compute per-iteration metrics
                double DMF = (systemFailures > 0) ? totalFailureDuration / systemFailures : 0.0;
                double TMEFS = (systemFailures > 1) ? accBetweenFailures / (systemFailures - 1) : 0.0;
                double NMMR = (clock > 0) ? busyTimeAcc / clock : 0.0;
                double TOR = (clock * repairers > 0) ? 100.0 * idleTimeAcc / (clock * repairers) : 0.0;
                double DTF = (clock > 0) ? 100.0 * totalFailureDuration / clock : 0.0;

                sumDMF += DMF;
                sumTMEFS += TMEFS;
                sumNMMR += NMMR;
                sumTOR += TOR;
                sumDTF += DTF;

                // Clear event list to exit loop
                eventList.clear();
            }
        }
    }

    // Average over iterations
    std::array<double, 5> result;
    if (iterations > 0) {
        result[0] = sumDMF / iterations;
        result[1] = sumTMEFS / iterations;
        result[2] = sumNMMR / iterations;
        result[3] = sumTOR / iterations;
        result[4] = sumDTF / iterations;
    } else {
        result.fill(0.0);
    }
    return result;
}

// The solution implements a discrete-event simulation with a single server (repairer) and a finite population of machines with spare parts. The core algorithm loops over iterations; each iteration initializes the system: a clock at 0, an event list containing `numMachines` failure events (one per machine, each with an exponentially distributed time) and a simulation end event at `simulationTime`. The main loop processes events in chronological order: the event with the smallest timestamp is popped, the clock advances to that time. For a failure event: accumulate repairer busy time (if the server is busy, `numInRepair` times the time since last update), increment `numInRepair`. If the repairer is free, set it busy and schedule a repair completion at current time + exponential(meanRepairTime). Otherwise, queue the failure (increment queue length). For spare parts: if available, decrement and schedule a new failure for that machine at current time + exponential(meanFailureTime). If no spare parts, increment the count of machines lacking parts (`brokenMachines`). If this is the first such machine, record the start of a system failure (when all spare parts exhausted) and increment `systemFailures`. For a repair completion event: update busy-time accumulator, decrement `numInRepair`. If there are queued machines, dequeue one and schedule a repair completion immediately. Else free the repairer (record idle time if needed). For spare parts: if no broken machines waiting for parts, increment spare parts; else decrement broken machines and schedule a new failure for that machine (now that a spare part is available). If this was the last broken machine, record the end of the system failure, add the duration to `totalFailureDuration`, and update the time of last system failure end. The simulation ends when the SIM_END event is processed: at that point, finalize any ongoing busy/idle time and, if still in system failure, add the remaining duration. Then compute the five per-iteration metrics: `DMF = totalFailureDuration / systemFailures` (guard against division by zero; if no system failures, use 0), `TMEFS = accumulatedTimeBetweenFailures / (systemFailures - 1)` if >1 else 0, `NMMR = totalBusyTime / finalClock`, `TOR = 100 * idleTime / (finalClock * repairers)` (with repairers=1, so just idleTime/finalClock*100), `DTF = 100 * totalFailureDuration / finalClock`. Accumulate these into the totals. After all iterations, divide each accumulator by `iterations` and return. Edge cases: iterations=0 (return zeros), simulationTime=0, all machines never fail (systemFailures=0), spare parts larger than machines (no system failure possible), repair time zero (but assume positive means). Complexity: each iteration processes at most `O((numMachines + failures + repairs))` events, each insertion into a sorted list is O(k) if using a vector/list plus sort, or O(log k) with a priority queue. For a simulation with E events, total complexity per iteration is O(E log E) or O(E^2) in the worst case with a list. Space is O(E) for the event list.
