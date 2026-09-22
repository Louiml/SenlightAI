// Write a C++ function `int minimumBoats(int people, int capacity)` that determines the minimum number of boat trips needed to transport a group of people across a river. Each person has a specific weight, and a boat can carry up to `capacity` total weight. A single boat trip can carry one person, or if the combined weight of two people does not exceed the capacity, it can carry two people. Given `people` as the number of individuals and `capacity` as the maximum weight per trip, the function should read `people` weights from standard input and return the minimum number of trips required. The input format follows: first line contains two integers `n` and `h` (where `n` is the number of people and `h` is the boat capacity), and the second line contains `n` integers representing each person's weight. The function must return the total number of trips. For example, if `n=5, h=10` and weights are `5 5 5 5 5`, the answer is 5 (each person alone, pairs exceed 10). If `n=4, h=10` and weights are `4 6 3 7`, the answer is 4 because pairs like 4+6=10 and 3+7=10 can pair up, but since the problem only allows at most two per boat, and the maximum pairings are 2 boats, but the given snippet treats each person separately with integer division when a person's weight exceeds capacity, which isn't the case here. The intended logic from the snippet is: for each person, if weight ≤ capacity, count 1 trip; otherwise, count `ceil(weight/capacity)` trips (since heavy items need multiple trips alone). This is a simpler interpretation: each person is transported independently, and a person heavier than capacity requires multiple trips with that same person alone. Implement this logic.
The problem is straightforward: iterate through all `n` people, reading their weight `x`. For each person, if `x <= h`, exactly one trip is needed. If `x > h`, the person must be carried alone in multiple trips because they exceed the boat's capacity by themselves. The number of trips for that person is `ceil(x / h)`, which can be computed as `(x + h - 1) / h` using integer arithmetic to avoid floating point. Sum these trip counts for all people and return the total. Important edge cases: if `h` is very large (e.g., 10^9) and weights are small, all count as 1 each; if a weight is exactly a multiple of `h`, the ceiling formula works correctly (e.g., `x=10, h=5` gives `(10+4)/5=2`). If `h=0`, the problem constraints likely avoid this, but if encountered, division by zero occurs; we assume `h >= 1`. Time complexity is O(n) for reading and processing, and space complexity is O(1) auxiliary (excluding input storage). The function uses a loop and constant-time arithmetic per element.
#include <bits/stdc++.h>

// Reads n people and boat capacity h, returns minimum number of trips.
int minimumTrips(int n, int h) {
    int totalTrips = 0;
    for (int i = 0; i < n; ++i) {
        int weight;
        std::cin >> weight;
        if (weight <= h) {
            totalTrips += 1;
        } else {
            totalTrips += (weight + h - 1) / h;
        }
    }
    return totalTrips;
}
#include <bits/stdc++.h>

// Declare the solution function (assumed to be defined above).
int minimumTrips(int n, int h);

int main() {
    // Test case 1: all light people
    std::istringstream input1("3 10\n4 5 6\n");
    std::cin.rdbuf(input1.rdbuf());
    assert(minimumTrips(3, 10) == 3);

    // Test case 2: one heavy person
    std::istringstream input2("2 5\n3 12\n");
    std::cin.rdbuf(input2.rdbuf());
    assert(minimumTrips(2, 5) == 1 + 3); // 3 alone, 12 needs ceil(12/5)=3 trips

    // Test case 3: exact multiples
    std::istringstream input3("3 4\n4 8 12\n");
    std::cin.rdbuf(input3.rdbuf());
    assert(minimumTrips(3, 4) == 1 + 2 + 3); // 1, 2, 3

    // Test case 4: single person heavy
    std::istringstream input4("1 2\n7\n");
    std::cin.rdbuf(input4.rdbuf());
    assert(minimumTrips(1, 2) == 4); // ceil(7/2)=4

    // Test case 5: all equal to capacity
    std::istringstream input5("2 5\n5 5\n");
    std::cin.rdbuf(input5.rdbuf());
    assert(minimumTrips(2, 5) == 2);

    // Test case 6: mixed with zero weight (if allowed)
    std::istringstream input6("3 5\n0 5 6\n");
    std::cin.rdbuf(input6.rdbuf());
    assert(minimumTrips(3, 5) == 1 + 1 + 2); // 0 ≤5, 5 ≤5, 6 needs 2 trips

    // Test case 7: large h
    std::istringstream input7("2 1000\n1 2000\n");
    std::cin.rdbuf(input7.rdbuf());
    assert(minimumTrips(2, 1000) == 1 + 2); // 1<=1000, 2000 needs 2 trips

    // Test case 8: all heavy but multiple trips
    std::istringstream input8("3 3\n7 8 9\n");
    std::cin.rdbuf(input8.rdbuf());
    assert(minimumTrips(3, 3) == 3 + 3 + 3); // each needs 3,3,3

    // Test case 9: n=0 is not expected, but assume zero trips
    // Not applicable due to input format; skip.

    // Test case 10: edge with h=1
    std::istringstream input10("3 1\n1 2 3\n");
    std::cin.rdbuf(input10.rdbuf());
    assert(minimumTrips(3, 1) == 1 + 2 + 3);

    return 0;
}
