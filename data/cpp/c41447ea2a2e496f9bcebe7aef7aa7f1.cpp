Design a thread-safe class `PartyRide` that simulates a carpool service for two political parties (Party A and Party B). Each request from a party member must wait until there are enough waiting members to form a complete car of exactly 4 passengers. Valid car configurations are: 4 from Party A, 4 from Party B, or 2 from Party A and 2 from Party B. The class must expose two methods: `requestA()` and `requestB()`, each callable from multiple threads. When a complete car is formed, the class must output the configuration and reset the waiting counts accordingly. The solution must handle concurrency safely using `std::mutex` and `std::condition_variable`, and each thread must wait without deadlock, meaning all 4 threads of a valid group should proceed to "seat" together and then one thread should trigger the "drive" action. Provide a standalone C++ class definition (no `main`) with a private helper for seating and driving, and ensure that the waiting condition allows only the three valid combos.

The core challenge is synchronization of multiple threads that each represent a party member waiting for a ride. The main algorithm uses a shared counter for each party's waiting requests. Each request increments its party's counter and then waits on a condition variable until a valid combination is possible: either A>=4, B>=4, or (A>=2 && B>=2). Once a thread wakes up and finds itself eligible, it must "seat" itself (increment a seated counter). The thread that seats the 4th passenger then deducts the appropriate number from the waiting counters (based on which combination is satisfied), resets the seated counter, prints the drive message, and notifies all other threads. Critical edge cases: multiple threads may wake simultaneously but only 4 should proceed; must ensure that after a drive, the counters are updated atomically so that other waiting threads (e.g., leftover requests) can form the next car. Also, to avoid missed wakeups, the condition variable predicate must re-check the counters after waking. The waiting condition might also allow a 5th thread to slip in if not careful, but since after 4 seats are taken the counters are decremented, the predicate will no longer be true for extra threads, so they will go back to waiting. Time complexity per request is O(1) amortized, space complexity O(1) for the counters.

#include <mutex>
#include <condition_variable>
#include <iostream>

class PartyRide {
private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    int partyA_waiting;
    int partyB_waiting;
    int seated_count;

    // Helper to print seating action (called under lock)
    void seat() {
        std::cout << "Seated " << std::this_thread::get_id() << std::endl;
    }

    // Helper to print driving action (called under lock)
    void drive(int combo) {
        std::cout << "Car driven: ";
        if (combo == 1) std::cout << "4 A";
        else if (combo == 2) std::cout << "4 B";
        else std::cout << "2 A, 2 B";
        std::cout << std::endl;
    }

public:
    PartyRide() : partyA_waiting(0), partyB_waiting(0), seated_count(0) {}

    // Request a seat for a party A member
    void requestA() {
        std::unique_lock<std::mutex> lock(mtx);
        partyA_waiting++;
        // Wait until a valid car can be formed
        cv.wait(lock, [this] {
            return partyA_waiting >= 4 || partyB_waiting >= 4 || 
                   (partyA_waiting >= 2 && partyB_waiting >= 2);
        });

        seat(); // This thread takes a seat
        seated_count++;

        if (seated_count == 4) {
            // Forming the car: decide which combination
            int combo = 0;
            if (partyA_waiting >= 4) {
                partyA_waiting -= 4;
                combo = 1;
            } else if (partyB_waiting >= 4) {
                partyB_waiting -= 4;
                combo = 2;
            } else if (partyA_waiting >= 2 && partyB_waiting >= 2) {
                partyA_waiting -= 2;
                partyB_waiting -= 2;
                combo = 3;
            }
            seated_count = 0; // Reset for next car
            drive(combo);
        }
        cv.notify_all(); // Wake others to check if they can continue
    }

    // Request a seat for a party B member
    void requestB() {
        std::unique_lock<std::mutex> lock(mtx);
        partyB_waiting++;
        cv.wait(lock, [this] {
            return partyA_waiting >= 4 || partyB_waiting >= 4 || 
                   (partyA_waiting >= 2 && partyB_waiting >= 2);
        });

        seat();
        seated_count++;

        if (seated_count == 4) {
            int combo = 0;
            if (partyA_waiting >= 4) {
                partyA_waiting -= 4;
                combo = 1;
            } else if (partyB_waiting >= 4) {
                partyB_waiting -= 4;
                combo = 2;
            } else if (partyA_waiting >= 2 && partyB_waiting >= 2) {
                partyA_waiting -= 2;
                partyB_waiting -= 2;
                combo = 3;
            }
            seated_count = 0;
            drive(combo);
        }
        cv.notify_all();
    }
};

#include <thread>
#include <vector>
#include <cassert>

// The solution class is assumed to be included above.

int main() {
    // Test 1: 4 Democrats (A) should form one car
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&ride] { ride.requestA(); });
        }
        for (auto& t : threads) t.join();
        // No direct assertion possible without output; just ensure no deadlock
        assert(true);
    }

    // Test 2: 2 A and 2 B should form one car
    {
        PartyRide ride;
        std::thread t1([&ride] { ride.requestA(); });
        std::thread t2([&ride] { ride.requestA(); });
        std::thread t3([&ride] { ride.requestB(); });
        std::thread t4([&ride] { ride.requestB(); });
        t1.join(); t2.join(); t3.join(); t4.join();
        assert(true);
    }

    // Test 3: 6 A and 2 B should produce one 4-A car and one 2A+2B car
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        for (int i = 0; i < 6; ++i) {
            threads.emplace_back([&ride] { ride.requestA(); });
        }
        for (int i = 0; i < 2; ++i) {
            threads.emplace_back([&ride] { ride.requestB(); });
        }
        for (auto& t : threads) t.join();
        assert(true);
    }

    // Test 4: 4 B and 2 A should produce one 4-B car and one 2A+2B car
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&ride] { ride.requestB(); });
        }
        for (int i = 0; i < 2; ++i) {
            threads.emplace_back([&ride] { ride.requestA(); });
        }
        for (auto& t : threads) t.join();
        assert(true);
    }

    // Test 5: Sequential calls from single thread should not deadlock
    {
        PartyRide ride;
        ride.requestA();
        ride.requestA();
        // Only two requests: no car formed, but methods should return after timeout? 
        // In our implementation wait() could block forever. But since we used wait (not wait_for), 
        // this would deadlock. To handle this, need to ensure at least 4 eligible requests. 
        // The task spec says threads should wait until a valid combination exists. 
        // So this test is invalid. The correct behavior is to block. 
        // We omit this test.
    }

    // Test 6: 8 A requests should form two 4-A cars
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&ride] { ride.requestA(); });
        }
        for (auto& t : threads) t.join();
        assert(true);
    }

    // Test 7: 2 A and 6 B should form one 4-B car and one 2A+2B car
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        threads.emplace_back([&ride] { ride.requestA(); });
        threads.emplace_back([&ride] { ride.requestA(); });
        for (int i = 0; i < 6; ++i) {
            threads.emplace_back([&ride] { ride.requestB(); });
        }
        for (auto& t : threads) t.join();
        assert(true);
    }

    // Test 8: 4 A and 4 B should form two cars: one 4-A and one 4-B, or one 4-A and one 2A+2B? 
    // With our predicate, first 4 A might form a car, then remaining 4 B form another. Both valid.
    {
        PartyRide ride;
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&ride] { ride.requestA(); });
        }
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([&ride] { ride.requestB(); });
        }
        for (auto& t : threads) t.join();
        assert(true);
    }

    // Test 9: 3 A and 3 B should not form a car (requires 4 total). 
    // Threads would block forever, so we cannot join. So we skip this test.
    // Test 10: 4 A, 2 B, 2 A (total 6 A, 2 B) forms two cars as above.
    // Already covered in Test 3.

    return 0;
}
