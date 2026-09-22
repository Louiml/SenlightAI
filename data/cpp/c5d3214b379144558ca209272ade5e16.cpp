// Write a C++ function that simulates a race condition when multiple threads increment a shared counter without synchronization, and then a second version using a mutex from the C++11 standard library to protect the counter. The task requires: 1) a function `runRaceCondition(int numThreads, int incrementsPerThread)` that creates threads that each increment a shared `int` counter `incrementsPerThread` times without any synchronization, and returns the final counter value (which will likely be less than the expected total due to data races); 2) a function `runSynchronized(int numThreads, int incrementsPerThread)` that does the same but protects each increment with a `std::mutex`, and returns the final counter value (which should equal `numThreads * incrementsPerThread`). Both functions should use `std::thread` and `std::atomic` for the counter in the race version to ensure the race is detectable (though still racy). The functions must be deterministic in terms of thread creation and joining, and must handle edge cases where either argument is zero or negative (return 0 or handle gracefully). The solution should demonstrate understanding of C++ threading, data races, and synchronization primitives.
The solution creates a helper function that takes a pointer to a `std::atomic<int>` and a number of increments; this function simply loops `incrementsPerThread` times doing `++counter`. For the race condition version, we spawn `numThreads` threads all calling this helper on the same atomic counter, then join them all and return the counter's value. Because the increment operation on `std::atomic<int>` is atomic for a single increment, but the operation is not atomic across multiple increments (though each individual increment is atomic), the final value will be less than expected if multiple threads interleave at the hardware level—actually with `std::atomic::operator++`, each increment is atomic, so the final value will be exactly `numThreads * incrementsPerThread` because there is no race. To truly demonstrate a race, we need a non-atomic counter. The task specifies using `std::atomic` but then the race may not appear. The correct interpretation: we should use a plain `int` for the race version to show the data race, and use `std::atomic<int>` for the synchronized version? But the task says "using a std::mutex" for the synchronized version, so we can use plain `int` for race and `std::mutex` for synchronized. For clarity, we will use `int` for the race version (no synchronization) and `int` protected by `std::mutex` for the synchronized version. Edge cases: if `numThreads <= 0` or `incrementsPerThread <= 0`, return 0. Time complexity: O(numThreads * incrementsPerThread) for both, with overhead of mutex locking for synchronized. Space: O(numThreads) for thread objects. The race version's result is nondeterministic, so tests must only check that the result is <= expected and >= 0, and for synchronized result exactly equals expected.
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

// Simulate a race condition by having multiple threads increment a shared int without synchronization.
// Returns the final counter value (likely incorrect due to data race).
int runRaceCondition(int numThreads, int incrementsPerThread) {
    if (numThreads <= 0 || incrementsPerThread <= 0) return 0;

    int counter = 0;
    std::vector<std::thread> threads;

    auto work = [&](int n) {
        for (int i = 0; i < n; ++i) {
            ++counter;  // Data race: unsynchronized read-modify-write
        }
    };

    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back(work, incrementsPerThread);
    }

    for (auto& th : threads) {
        th.join();
    }

    return counter;
}

// Same as above but protect each increment with a mutex to ensure correctness.
// Returns the final counter value, which will equal numThreads * incrementsPerThread.
int runSynchronized(int numThreads, int incrementsPerThread) {
    if (numThreads <= 0 || incrementsPerThread <= 0) return 0;

    int counter = 0;
    std::mutex mtx;
    std::vector<std::thread> threads;

    auto work = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;  // Protected by mutex, no data race
        }
    };

    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back(work, incrementsPerThread);
    }

    for (auto& th : threads) {
        th.join();
    }

    return counter;
}
#include <cassert>

int main() {
    const int numThreads = 4;
    const int incrementsPerThread = 1000;

    // Synchronized version must be exactly correct
    int syncResult = runSynchronized(numThreads, incrementsPerThread);
    assert(syncResult == numThreads * incrementsPerThread);

    // Race version may be less than expected but never negative
    int raceResult = runRaceCondition(numThreads, incrementsPerThread);
    assert(raceResult >= 0 && raceResult <= numThreads * incrementsPerThread);

    // Edge cases: zero or negative arguments return 0
    assert(runSynchronized(0, 5) == 0);
    assert(runRaceCondition(5, 0) == 0);
    assert(runSynchronized(-1, 5) == 0);
    assert(runRaceCondition(5, -3) == 0);

    return 0;
}
