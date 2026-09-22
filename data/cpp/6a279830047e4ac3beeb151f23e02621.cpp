// Write a C++ function that simulates disease spread through a workplace contact network. Given the number of employees N (5 ≤ N ≤ 120), an adjacency matrix of size N×N (where matrix[i][j] = 1 indicates employees i and j are in contact), and an initial infected employee index (0-based), the function should simulate day-by-day spread: on each day, every currently infected employee infects all their direct contacts who are not yet infected. Continue until no new infections occur in a day. Return a vector<int> containing the day numbers (1-based) on which new infections occurred. The function should not modify the input matrix. Handle edge cases where the initial infected is already the only one or where the graph is disconnected.
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple chain 0-1-2, initial infected = 0.
    std::vector<std::vector<int>> m1 = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    assert(simulateInfectionDays(3, m1, 0) == (std::vector<int>{1, 2})); // Day1: 1, Day2: 2

    // Test 2: Isolated initial infected (no contacts).
    std::vector<std::vector<int>> m2 = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0}
    };
    assert(simulateInfectionDays(3, m2, 0).empty()); // No new infections

    // Test 3: Fully connected graph of 4 employees, initial infected = 2.
    std::vector<std::vector<int>> m3 = {
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 0}
    };
    assert(simulateInfectionDays(4, m3, 2) == (std::vector<int>{1})); // All infected on day 1

    // Test 4: Disconnected graph where one component is unreachable.
    std::vector<std::vector<int>> m4 = {
        {0, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0}
    };
    // Initial infected = 1 (component with 0 and 1), returns {1} only.
    assert(simulateInfectionDays(4, m4, 1) == (std::vector<int>{1}));

    // Test 5: Already all infected? Not possible with initial alone, but if N=1.
    std::vector<std::vector<int>> m5 = {{0}};
    assert(simulateInfectionDays(1, m5, 0).empty());

    // Test 6: Larger chain 5 nodes, start at one end.
    std::vector<std::vector<int>> m6(5, std::vector<int>(5, 0));
    for (int i = 0; i < 4; ++i) {
        m6[i][i+1] = 1;
        m6[i+1][i] = 1;
    }
    assert(simulateInfectionDays(5, m6, 0) == (std::vector<int>{1, 2, 3, 4}));

    // Test 7: Matrix with self-loop (should be ignored).
    std::vector<std::vector<int>> m7 = {
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };
    assert(simulateInfectionDays(3, m7, 0) == (std::vector<int>{1, 2}));
}
#include <vector>
#include <queue>

// Simulate day-by-day spread of infection through a contact network.
// Returns a vector of day numbers (1-based) on which new infections occurred.
// The input matrix is not modified.
std::vector<int> simulateInfectionDays(int N, const std::vector<std::vector<int>>& matrix, int initialInfected) {
    std::vector<bool> infected(N, false);
    infected[initialInfected] = true;
    
    std::queue<int> currentInfected;
    currentInfected.push(initialInfected);
    
    std::vector<int> daysWithNewInfections;
    int day = 1;
    
    while (!currentInfected.empty()) {
        std::vector<int> newlyInfected;
        int size = static_cast<int>(currentInfected.size());
        
        for (int cnt = 0; cnt < size; ++cnt) {
            int person = currentInfected.front();
            currentInfected.pop();
            
            for (int neighbor = 0; neighbor < N; ++neighbor) {
                if (matrix[person][neighbor] == 1 && !infected[neighbor]) {
                    infected[neighbor] = true;
                    newlyInfected.push_back(neighbor);
                }
            }
        }
        
        if (!newlyInfected.empty()) {
            daysWithNewInfections.push_back(day);
            for (int person : newlyInfected) {
                currentInfected.push(person);
            }
        }
        ++day;
    }
    
    return daysWithNewInfections;
}
// The solution simulates a breadth‑first style spread over days. Maintain a boolean vector `infected` marking all employees who have ever been infected, and a queue of employees infected on the current day. For each day, pop all currently infected employees (seeded by the initial infected), for each of them scan their row to find uninfected contacts, mark those as newly infected, and collect them into a list. If the list is non‑empty, record the day number and enqueue those employees for the next day's spread. Repeat until no new infections occur. Important edge cases: the initial infected may have no contacts (in which case the result is empty), the graph may have multiple connected components (some employees never get infected, but the loop still terminates), and self‑loops (matrix[i][i] = 1) are ignored since we check `!infected[j]` and a person is already infected. Time complexity is O(D * N^2) where D is the number of days (at most N‑1), so worst‑case O(N^3). Space complexity is O(N) for the infected vector and queue/list.
