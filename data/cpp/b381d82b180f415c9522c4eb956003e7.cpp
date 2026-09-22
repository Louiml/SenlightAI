// Write a C++ function named `josephusSurvivor` that takes two integer parameters: the number of people `n` (where people are numbered from 1 to n) and a counting step `m`. The function simulates the Josephus problem: starting from person 1, count `m` people in a circle (including the starting person), eliminate the m-th person, and continue counting from the next person, repeating until only one person remains. The function must return the original position number of the last remaining person. The input values are guaranteed to be positive integers (n ≥ 1, m ≥ 1). The function should handle the case where m is larger than n and where n=1. Do not use any external libraries beyond the standard C++ headers.

The Josephus problem can be solved by simulating the elimination process using a boolean array (or a vector) to track eliminated people, as shown in the original snippet. The algorithm iterates through the circle, maintaining a counter `count` that increments each time we pass a person who has not yet been eliminated. When `count` reaches `m`, we mark that person as eliminated, reset the count, and reduce the number of remaining people. We continue until only one person remains, then return that person's 1-based index. Edge cases: if n=1, the function should return 1 immediately since no counting occurs. If m=1, each person is eliminated one after another, and the last person is the n-th person. The simulation runs in O(n*m) time in the worst case (since each elimination may require scanning up to n positions) and uses O(n) space for the boolean array. A simpler O(n) mathematical solution exists, but the simulation matches the snippet's approach and is easier to verify.

#include <vector>

// Simulates the Josephus problem and returns the position of the last survivor.
// n: number of people (numbered 1..n), m: counting step. Precondition: n >= 1, m >= 1.
int josephusSurvivor(int n, int m) {
    if (n <= 0 || m <= 0) return 0; // invalid input, but not expected

    std::vector<bool> alive(n, true);
    int remaining = n;
    int index = 0; // current position (0-based)
    int count = 0; // steps counted since last elimination

    while (remaining > 1) {
        // Count m alive people starting from current position (circular)
        while (true) {
            if (alive[index]) {
                ++count;
                if (count == m) {
                    alive[index] = false;
                    --remaining;
                    count = 0;
                    break;
                }
            }
            index = (index + 1) % n;
        }
        // Move to the next position after the eliminated one
        index = (index + 1) % n;
    }

    // Find the last surviving person
    for (int i = 0; i < n; ++i) {
        if (alive[i]) {
            return i + 1; // 1-based position
        }
    }
    return 0; // Should never reach here if n >= 1
}

#include <cassert>

int main() {
    // Basic cases
    assert(josephusSurvivor(1, 5) == 1);      // single person
    assert(josephusSurvivor(5, 1) == 5);      // step of 1 eliminates sequentially
    assert(josephusSurvivor(5, 2) == 3);      // classic: n=5, m=2 -> survivor 3
    assert(josephusSurvivor(7, 3) == 4);      // n=7, m=3 -> survivor 4
    assert(josephusSurvivor(10, 3) == 4);     // known result
    // Large m relative to n
    assert(josephusSurvivor(4, 10) == 1);     // m > n wraps around
    // Two people
    assert(josephusSurvivor(2, 3) == 2);      // eliminate 1, then 2 survives
    assert(josephusSurvivor(2, 1) == 2);      // step 1 eliminates 1, then 2 survives
    // More tests
    assert(josephusSurvivor(3, 2) == 3);      // n=3, m=2 -> survivor 3
    assert(josephusSurvivor(6, 2) == 5);      // n=6, m=2 -> survivor 5
    return 0;
}
