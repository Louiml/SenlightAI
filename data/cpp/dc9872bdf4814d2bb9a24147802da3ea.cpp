// Implement a C++ function that simulates a producer-consumer-style incrementer using two threads that alternately increment a shared counter up to a given limit `N`, while a consumer thread prints the counter after each increment step. The function should take an integer `N` as input and produce output from 1 to `N` in order, using binary semaphores and a mutex to ensure correct synchronization. The key challenge is to design the function so that two producer threads (even and odd) never interleave their increments incorrectly, and the consumer prints each value exactly once in ascending order. The function should return nothing (void) and should handle edge cases such as `N = 0` (no output) and `N = 1` (single increment). The implementation must be self-contained and use only standard C++ threading primitives (e.g., `std::thread`, `std::mutex`, `std::binary_semaphore`). The function must not block forever; it should terminate cleanly after printing all numbers up to `N`.
// The core idea is to use a binary semaphore pair to coordinate the flow of control between producer threads and the consumer. The two producer threads (call them `incrementerA` and `incrementerB`) are responsible for incrementing a shared counter `i` (initialized to 0). The consumer thread is responsible for printing the current value of `i` after each increment. To avoid races, all access to `i` must be protected by a mutex. The semaphores ensure that the producers only act when the consumer is ready to receive a new value, and the consumer only prints after a producer has incremented. Specifically, the consumer first releases a "go" semaphore that allows one producer to proceed. The producer acquires that go semaphore, then locks the mutex, increments `i` (if `i < N`), releases the "done" semaphore, and unlocks. The consumer acquires the "done" semaphore and then prints the value if it is within range. This pattern repeats until `i == N`, after which the threads exit. Edge cases: if `N == 0`, the function should do nothing and return; if `N == 1`, one producer increments to 1 and the consumer prints 1. The main challenge is ensuring that the producers alternate correctly (but the problem does not require them to strictly alternate; only that each increment is atomic and printed in order). However, to avoid both producers trying to increment at the same time (which could skip numbers), we use the mutex to serialize increments. The semaphore pair acts as a handshake: consumer releases one "go" token, a single producer acquires it, increments, and signals "done". The consumer then prints and repeats. Since only one "go" token exists initially, only one producer can be active at a time. This prevents over-incrementing. Time complexity: O(N) increments and prints. Space complexity: O(1) auxiliary (excluding thread overhead). Important edge cases include N=0 (no output, immediate return) and N being large (the loop terminates when i reaches N). The function must avoid deadlock: the initial state has the "done" semaphore with count 0, so the consumer blocks until a producer signals. The producers must not call `exit()` (as in the snippet) but instead return from their thread functions to allow clean joining.
#include <thread>
#include <mutex>
#include <semaphore>
#include <iostream>
#include <atomic>

// Simulates two producer threads incrementing a shared counter up to N,
// while a consumer prints each value in order. Uses binary semaphore handshake.
void printIncrementsUpTo(int N) {
    if (N <= 0) return; // Edge case: nothing to do

    std::mutex mtx;
    std::binary_semaphore signalProducer{0}; // Producer waits for this
    std::binary_semaphore signalConsumer{0}; // Consumer waits for this
    int counter = 0;
    bool done = false;

    // Producer thread: increments counter exactly once per activation
    auto producer = [&]() {
        while (!done) {
            signalProducer.acquire(); // Wait for consumer's go
            if (done) break;
            std::lock_guard<std::mutex> lock(mtx);
            if (counter < N) {
                ++counter;
                signalConsumer.release(); // Tell consumer data is ready
            } else {
                done = true;
                signalConsumer.release(); // Release consumer to see done
            }
        }
    };

    // Consumer thread: prints the counter after each increment
    auto consumer = [&]() {
        while (true) {
            signalConsumer.acquire(); // Wait for producer to increment
            int value;
            {
                std::lock_guard<std::mutex> lock(mtx);
                value = counter;
                if (value > N) break;
            }
            if (value > 0) {
                std::cout << value << " ";
            }
            if (value == N) {
                done = true;
                signalProducer.release(); // Let producers exit
                break;
            }
            signalProducer.release(); // Allow next increment
        }
        std::cout << std::endl;
    };

    std::thread t1(producer);
    std::thread t2(producer);
    std::thread t3(consumer);

    // Start the handshake: release one producer
    signalProducer.release();

    t1.join();
    t2.join();
    t3.join();
}
#include <cassert>
#include <sstream>
#include <iostream>

// Capture output of printIncrementsUpTo and return as string
std::string runAndCapture(int N) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printIncrementsUpTo(N);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // Test N=0 (no output, trimmed string should be empty)
    assert(runAndCapture(0) == "");
    
    // Test N=1
    assert(runAndCapture(1) == "1 \n");
    
    // Test N=5
    assert(runAndCapture(5) == "1 2 3 4 5 \n");
    
    // Test N=10
    assert(runAndCapture(10) == "1 2 3 4 5 6 7 8 9 10 \n");
    
    // Test N=20 (larger to check consistency)
    assert(runAndCapture(20) == "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 \n");
    
    return 0;
}
