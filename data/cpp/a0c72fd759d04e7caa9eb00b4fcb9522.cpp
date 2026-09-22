Write a C++ function that simulates a game between Alice and Bob where two large random numbers are generated, and Alice wins if the two numbers share at least one common prime factor, while Bob wins otherwise. To keep the simulation computationally feasible, instead of generating actual large integers, generate a random set of "simulated prime factors" for each number, where the number of distinct factors in each set follows a Poisson distribution with mean 5, and each factor is a uniformly random integer in the range [2, 10^6]. The function must accept two parameters: `num_simulations` (the total number of games to simulate) and `num_threads` (the number of worker threads to use). It must return a `std::pair<long long, long long>` where the first element is the total count of Alice's wins and the second is Bob's wins. Use `std::thread` (not pthreads) for parallelization, and ensure that each thread has its own independent random number generator (e.g., using `std::mt19937` seeded with a unique value per thread). The function must be thread-safe with respect to aggregating results, and it must ensure that all threads complete before returning.

// The core algorithm spawns `num_threads` worker threads, each processing an equal share of the total simulations (`num_simulations / num_threads`; the remainder can be distributed to the first few threads if needed, but the specification can assume `num_simulations` is evenly divisible). Each thread maintains local counters for Alice wins and Bob wins to avoid contention. For each simulation, two sets of random "prime factors" are generated: first, the number of factors is drawn from a Poisson distribution with mean 5 (using `std::poisson_distribution<int>`), then that many distinct integers are drawn uniformly from [2, 10^6] and inserted into a `std::set` (duplicates are naturally ignored). Two sets are then checked for any common element by iterating over the smaller set and probing the other; since set sizes are small (average 5, but capped by randomness), the intersection check is cheap. After all simulations in a thread, the local counters are added to the global result under a mutex (or using `std::atomic` if preferred for simplicity). Edge cases: if `num_threads` is 0, clamp to 1; if `num_simulations` is 0, return `{0,0}`; ensure the Poisson distribution never yields negative counts (it doesn't, but guard anyway). Complexity: For each simulation, generating two sets costs \(O(k \log k)\) where \(k\) is the average set size (about 5), and intersection is \(O(k \log k)\). With \(N\) total simulations across \(T\) threads, overall time is \(O(N \cdot k \log k)\) plus thread overhead; space is \(O(T \cdot k)\) for per-thread sets and local counters.

#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <set>
#include <cstdint>
#include <utility>
#include <algorithm>

// Simulate a game where Alice wins if two random factor sets intersect.
// Returns a pair of (alice_wins, bob_wins) over all simulations.
std::pair<long long, long long> simulate_prime_factor_game(int num_simulations, int num_threads) {
    if (num_simulations <= 0) {
        return {0, 0};
    }
    if (num_threads <= 0) {
        num_threads = 1;
    }

    const int sims_per_thread = num_simulations / num_threads;
    const int remainder = num_simulations % num_threads;

    long long total_alice = 0;
    long long total_bob = 0;
    std::mutex result_mutex;

    auto worker = [&](int thread_id, int sim_count) {
        // Unique seed per thread: use random_device + thread_id to avoid collisions
        std::random_device rd;
        std::mt19937 gen(rd() ^ (thread_id + 1) * 0x9E3779B97F4A7C15ULL);

        std::poisson_distribution<int> factor_count_dist(5.0);
        std::uniform_int_distribution<uint64_t> factor_dist(2, 1000000ULL);

        long long local_alice = 0;
        long long local_bob = 0;

        for (int i = 0; i < sim_count; ++i) {
            std::set<uint64_t> factors_a;
            int count_a = factor_count_dist(gen);
            for (int j = 0; j < count_a; ++j) {
                factors_a.insert(factor_dist(gen));
            }

            std::set<uint64_t> factors_b;
            int count_b = factor_count_dist(gen);
            for (int j = 0; j < count_b; ++j) {
                factors_b.insert(factor_dist(gen));
            }

            // Check intersection efficiently by iterating over the smaller set
            bool common = false;
            if (factors_a.size() <= factors_b.size()) {
                for (uint64_t f : factors_a) {
                    if (factors_b.find(f) != factors_b.end()) {
                        common = true;
                        break;
                    }
                }
            } else {
                for (uint64_t f : factors_b) {
                    if (factors_a.find(f) != factors_a.end()) {
                        common = true;
                        break;
                    }
                }
            }

            if (common) {
                ++local_alice;
            } else {
                ++local_bob;
            }
        }

        // Aggregate under lock
        {
            std::lock_guard<std::mutex> lock(result_mutex);
            total_alice += local_alice;
            total_bob += local_bob;
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        int extra = (t < remainder) ? 1 : 0;
        threads.emplace_back(worker, t, sims_per_thread + extra);
    }

    for (auto& th : threads) {
        th.join();
    }

    return {total_alice, total_bob};
}

#include <iostream>
#include <utility>
#include <cassert>

// The solution function is assumed to be declared above.
// (In a real test, you would include the solution header or paste the function here.)

int main() {
    // Small simulations to check correctness and determinism
    auto result0 = simulate_prime_factor_game(0, 4);
    assert(result0.first == 0 && result0.second == 0);

    // With 1 simulation and 1 thread, the result must be exactly (0,1) or (1,0)
    auto result1 = simulate_prime_factor_game(1, 1);
    assert(result1.first + result1.second == 1);

    // Two simulations with 2 threads: total wins must equal 2
    auto result2 = simulate_prime_factor_game(2, 2);
    assert(result2.first + result2.second == 2);

    // Ten simulations split across 3 threads: total must be 10
    auto result10 = simulate_prime_factor_game(10, 3);
    assert(result10.first + result10.second == 10);

    // Larger test for multi-thread consistency: 1000 simulations, 8 threads
    auto result1000 = simulate_prime_factor_game(1000, 8);
    assert(result1000.first + result1000.second == 1000);

    // Edge: zero threads should not crash; effectively 1 thread
    auto resultEdge = simulate_prime_factor_game(5, 0);
    assert(resultEdge.first + resultEdge.second == 5);

    // Verify each count is non-negative
    assert(result1000.first >= 0 && result1000.second >= 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
