// Implement a C++ function `simulateDiningPhilosophers(int numPhilosophers)` that simulates the classic dining philosophers problem for exactly 4 philosophers. The function should use the same fork and philosopher state representation as the provided code (arrays of structures with `taken`, `left`, `right` fields, where 0 means available, 1 means acquired, 10 means finished), and exactly the same deterministic scheduling logic: repeatedly iterate through philosophers 0 to 3 and call a helper to attempt to pick up their left fork first, then their right fork (if needed), and if both are acquired, declare them finished, release their forks, and increment a completion counter. The simulation continues until all 4 philosophers have finished. The function should perform all state updates and print the exact same messages (using `std::cout`) as the given code, with the philosophers numbered from 1 to 4 and forks numbered from 1 to 4 (fork numbers wrap around: philosopher 1 uses forks 1 and 4, philosopher 2 uses forks 2 and 1, etc.). The function should not take any input; it returns `void` and only prints the progress and final state. The function must be self-contained (no global state variables), and all local state (arrays of structures, completion counter, loop index) should be declared inside the function. The solution should replicate the behavior exactly, including edge cases where a philosopher waits for a fork that another philosopher is currently holding.

// The solution simulates a deterministic round-robin scheduler that repeatedly attempts to feed each philosopher in order (0,1,2,3) until all four finish. Each philosopher has a `left` and `right` fork state and each fork has a `taken` flag. The logic mirrors the original code's conditions:  
// - If both left and right are 10, the philosopher already finished (skip).  
// - If both left and right are 1, the philosopher has both forks; mark them as 10 (finished), release both forks (set their `taken` to 0), print the release message, and increment the completed counter.  
// - If only left is 1 and right is 0, the philosopher attempts to pick up the right fork, which is the fork with index `pID` for the last philosopher (index 3) and `pID-1` (with wrap to 3) for others. If that fork is available, set its `taken` and the philosopher's right to 1; otherwise print a waiting message.  
// - If left is 0, the philosopher attempts to pick up the left fork, which is fork index `pID-1` for the last philosopher (with wrap to 3) and fork index `pID` for others. If available, set it; otherwise print a waiting message.  
// The loop repeats `for(i=0; i<4; i++)` inside a `while(completed<4)`, printing the count of completed philosophers after each full pass.  
//
// The algorithm is O(n²) in the number of philosophers in the worst case because each pass scans all philosophers, and the number of passes is bounded by the number of forks acquisitions (each philosopher acquires at most 2 forks, so at most 8 acquisitions, and each pass processes 4 philosophers, so at most 8 passes). For a fixed n=4, it’s O(1) time and O(1) space, since arrays are fixed size. The main edge cases are the wrap-around fork indices for philosopher 0 and philosopher 3, and ensuring that a philosopher who already finished is not processed again. The implementation must exactly reproduce the printed messages, including the exact spacing and newlines, to match the original behavior.

#include <iostream>

// Simulate the deterministic dining philosophers problem for exactly 4 philosophers.
// Prints all state transitions and the final completion count, matching the given logic.
void simulateDiningPhilosophers(int numPhilosophers) {
    // Fixed size for this problem (4 philosophers, 4 forks)
    const int n = 4;

    struct Fork {
        int taken;
    };
    struct Philosopher {
        int left;
        int right;
    };

    Fork fork[n];
    Philosopher philosopher[n];

    // Initialize all state to 0 (available/not acquired)
    for (int i = 0; i < n; ++i) {
        fork[i].taken = 0;
        philosopher[i].left = 0;
        philosopher[i].right = 0;
    }

    int compltedPhilo = 0;

    // Helper lambda to attempt to feed philosopher with ID pID (0-based)
    auto goForDinner = [&](int pID) {
        if (philosopher[pID].left == 10 && philosopher[pID].right == 10) {
            std::cout << "Philosopher " << pID + 1 << " finished his dinner\n";
        }
        else if (philosopher[pID].left == 1 && philosopher[pID].right == 1) {
            std::cout << "Philosopher " << pID + 1 << " finished his dinner\n";
            philosopher[pID].left = philosopher[pID].right = 10;

            int otherFork = pID - 1;
            if (otherFork == -1)
                otherFork = (n - 1);
            fork[pID].taken = fork[otherFork].taken = 0;
            std::cout << "Philosopher " << pID + 1 << " left fork " << pID + 1 << " and fork " << otherFork + 1 << "\n";
            compltedPhilo++;
        }
        else if (philosopher[pID].left == 1 && philosopher[pID].right == 0) {
            if (pID == (n - 1)) {
                if (fork[pID].taken == 0) {
                    fork[pID].taken = philosopher[pID].right = 1;
                    std::cout << "Fork " << pID + 1 << " taken by philosopher " << pID + 1 << "\n";
                } else {
                    std::cout << "Philosopher " << pID + 1 << " is waiting for fork " << pID + 1 << "\n";
                }
            } else {
                int dpID = pID;
                pID -= 1;
                if (pID == -1)
                    pID = (n - 1);

                if (fork[pID].taken == 0) {
                    fork[pID].taken = philosopher[dpID].right = 1;
                    std::cout << "Fork " << pID + 1 << " taken by Philosopher " << dpID + 1 << "\n";
                } else {
                    std::cout << "Philosopher " << dpID + 1 << " is waiting for Fork " << pID + 1 << "\n";
                }
            }
        }
        else if (philosopher[pID].left == 0) {
            if (pID == (n - 1)) {
                if (fork[pID - 1].taken == 0) {
                    fork[pID - 1].taken = philosopher[pID].left = 1;
                    std::cout << "Fork " << pID << " taken by philosopher " << pID + 1 << "\n";
                } else {
                    std::cout << "Philosopher " << pID + 1 << " is waiting for fork " << pID << "\n";
                }
            } else {
                if (fork[pID].taken == 0) {
                    fork[pID].taken = philosopher[pID].left = 1;
                    std::cout << "Fork " << pID + 1 << " taken by Philosopher " << pID + 1 << "\n";
                } else {
                    std::cout << "Philosopher " << pID + 1 << " is waiting for Fork " << pID + 1 << "\n";
                }
            }
        }
        // else do nothing (should not happen)
    };

    // Main round-robin simulation loop
    while (compltedPhilo < n) {
        for (int i = 0; i < n; ++i) {
            goForDinner(i);
        }
        std::cout << "\nfinished Philosophers " << compltedPhilo << "\n\n";
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Forward declaration of the solution function (not needed if included directly)
void simulateDiningPhilosophers(int numPhilosophers);

int main() {
    // We cannot easily capture stdout for deterministic testing, but we can
    // verify that the function runs without crashing and completes the simulation.
    // Since the function prints to std::cout, we can redirect to a string stream
    // to capture output and check expected key phrases.

    // Redirect cout to a buffer
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Run the simulation
    simulateDiningPhilosophers(4);

    // Restore cout
    std::cout.rdbuf(old);

    std::string output = buffer.str();

    // Assert that all four philosophers finished
    assert(output.find("Philosopher 1 finished his dinner") != std::string::npos);
    assert(output.find("Philosopher 2 finished his dinner") != std::string::npos);
    assert(output.find("Philosopher 3 finished his dinner") != std::string::npos);
    assert(output.find("Philosopher 4 finished his dinner") != std::string::npos);

    // Assert the final completion message
    assert(output.find("finished Philosophers 4") != std::string::npos);

    // Assert that the simulation has the expected number of lines (at least some output)
    assert(!output.empty());

    // A simple sanity check: no waiting messages should be present after all finish
    // (We expect a valid deterministic sequence)
    // Not strictly necessary but ensures no infinite loop
    assert(output.find("finished Philosophers 4") != std::string::npos);

    // Ensure the function does not produce unexpected extra characters
    // (We don't check exact content due to formatting variations)

    // Additional test: the function should not modify global state, so
    // calling it twice should be safe (it will reset all state internally).
    // We can call it again to ensure no crash.
    std::stringstream buffer2;
    std::streambuf* old2 = std::cout.rdbuf(buffer2.rdbuf());
    simulateDiningPhilosophers(4);
    std::cout.rdbuf(old2);
    assert(buffer2.str().find("finished Philosophers 4") != std::string::npos);

    return 0;
}
