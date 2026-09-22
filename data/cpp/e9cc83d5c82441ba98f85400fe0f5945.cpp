Given two vectors of equal non-zero length, `gallons` and `distances`, where `gallons[i]` is the amount of gas (in gallons) available in city `i`, and `distances[i]` is the distance (in miles) from city `i` to the next city (cities form a cycle), write a C++ function `size_t findAmpleCity(const std::vector<int>& gallons, const std::vector<int>& distances)` that returns the index of a city from which a car can start with an empty tank and complete a full loop around all cities, ending at the same city, without ever running out of gas. The car has a constant fuel efficiency of 20 miles per gallon (i.e., `kMPG = 20`). The total gas available across all cities is guaranteed to be at least the total fuel required (i.e., sum of `distances[i] / kMPG`), but there is no guarantee which city is ample. If multiple valid starting cities exist, your function must return the first one in the cycle (i.e., the smallest index) that is valid. The input may contain arbitrary non-negative integers for `gallons` and positive integers for `distances`.

The problem is a classic "gas station" problem. Since the total gas is at least the total required fuel, a solution always exists. The key insight is that if you iterate through the cycle starting from any city, once you find a city where the current gas (accumulated surplus/deficit) becomes negative, that city cannot be the start; instead, the next city is a candidate. Starting from that next city, the accumulated surplus from all previous cities is non-negative (because the total sum is non-negative), so if you keep track of the city where the prefix sum of `(gallons[i] - distances[i]/kMPG)` is minimized, that city's next index is the answer. However, the problem asks for the smallest index that works, which can be found by scanning from index 0 to n-1, but maintaining a candidate start: while iterating, if at any point the current gas (starting from a candidate) goes negative, move the candidate to the next index and reset gas to 0. At the end, the candidate is the answer. This works because the total sum is non-negative, so the candidate found must be valid (the total surplus from that candidate onward will never be negative). Edge cases: single city (must have enough gas to travel its own distance), large distances causing integer division (use integer division as given, but ensure consistency), and values that exactly zero out at some point. Time complexity is O(n) with one pass, and space complexity O(1) auxiliary.

#include <vector>
#include <cstddef>

// Given gallons[i] and distances[i] (miles), with fuel efficiency 20 mpg,
// return the smallest index of a city from which a full loop is possible.
// The total gas is guaranteed sufficient; assumes non-empty input.
size_t findAmpleCity(const std::vector<int>& gallons, const std::vector<int>& distances) {
    const int kMPG = 20;
    size_t n = gallons.size();
    size_t candidate = 0;
    int currentGas = 0;
    int totalSurplus = 0; // not strictly needed but helps reasoning

    for (size_t i = 0; i < n; ++i) {
        int surplus = gallons[i] - distances[i] / kMPG;
        currentGas += surplus;
        totalSurplus += surplus;
        if (currentGas < 0) {
            // This city cannot be the start; move to next.
            candidate = (i + 1) % n;
            currentGas = 0;
        }
    }
    // Since totalSurplus >= 0, candidate is valid.
    return candidate;
}

#include <cassert>
#include <vector>
#include <cstddef>

// Declaration (or include solution header)
size_t findAmpleCity(const std::vector<int>& gallons, const std::vector<int>& distances);

int main() {
    const int kMPG = 20;

    // Example from book
    std::vector<int> gallons1 = {20, 15, 15, 15, 35, 25, 30, 15, 65, 45, 10, 45, 25};
    std::vector<int> distances1 = {15 * kMPG, 20 * kMPG, 50 * kMPG, 15 * kMPG,
                                   15 * kMPG, 30 * kMPG, 20 * kMPG, 55 * kMPG,
                                   20 * kMPG, 50 * kMPG, 10 * kMPG, 15 * kMPG,
                                   15 * kMPG};
    assert(findAmpleCity(gallons1, distances1) == 8);

    // Single city: gas exactly enough
    std::vector<int> g2 = {10};
    std::vector<int> d2 = {200}; // 200/20 = 10
    assert(findAmpleCity(g2, d2) == 0);

    // All cities exactly break even: any start works, but smallest index is 0
    std::vector<int> g3 = {5, 5, 5};
    std::vector<int> d3 = {100, 100, 100}; // each requires 5
    assert(findAmpleCity(g3, d3) == 0);

    // Only last city has extra gas, others deficit
    std::vector<int> g4 = {1, 1, 1, 10};
    std::vector<int> d4 = {40, 40, 40, 40}; // each requires 2
    // Need to check: city 3 (index 3) has 10, gives surplus 8, then covers others.
    // Iteration: start candidate 0, after city0 gas = -1 -> move to 1, after city1 gas=-1 -> move to 2, after city2 gas=-1 -> move to 3, after city3 gas=8. End candidate 3.
    assert(findAmpleCity(g4, d4) == 3);

    // Large distance but total gas positive
    std::vector<int> g5 = {100, 0, 0};
    std::vector<int> d5 = {1000, 1000, 1000}; // each requires 50, total need 150, gas 100 -> insufficient? Actually sum =100, need 150, invalid. So ensure valid: let's use total gas 200
    std::vector<int> g5b = {200, 0, 0};
    std::vector<int> d5b = {1000, 1000, 1000}; // need 50 each, total 150, gas 200 -> valid
    // Check: start from 0: gas 200 - 50 = 150, then 1: -50 -> fail? Actually after city0 gas=150, then city1: 0-50 = -50, so fail at city1, move candidate to 2. Then city2: 0-50=-50, move candidate to 0 again? But that loops? Actually candidate becomes (2+1)%3=0, but we've already passed? The algorithm may not find correct if we wrap. However since total surplus is positive, the candidate that works must be one where prefix sum is minimal. Let's compute: surpluses: [150, -50, -50]. Starting from 0: after 0:150, after1:100, after2:50 -> never negative, so 0 works. My algorithm: i=0 currentGas=150 >=0; i=1 currentGas=100>=0; i=2 currentGas=50>=0; candidate remains 0. So correct. So test with 0.
    assert(findAmpleCity(g5b, d5b) == 0);

    // Another case: need smallest index, e.g., both city 2 and 3 work, but must return 2
    std::vector<int> g6 = {0, 0, 10, 0};
    std::vector<int> d6 = {100, 100, 100, 100}; // each requires 5, total need 20, gas 10 -> insufficient? Actually total gas 10, need 20, invalid. Make gas bigger:
    std::vector<int> g6b = {0, 0, 20, 10};
    std::vector<int> d6b = {100, 100, 100, 100}; // each requires 5, total need 20, gas 30 -> surplus 10. surpluses: [-5,-5,15,5]. Check starts: city0 fails, city1 fails, city2 works, city3 works (after city3 surplus 5, then city0 -5 =>0, city1 -5=>-5 fail, so city3 fails actually? Let's compute from 3: after 3 gas=5, after0:0, after1:-5 -> fail. So only city2 works. So answer 2.
    assert(findAmpleCity(g6b, d6b) == 2);

    // Case where total exactly zero: smallest index works
    std::vector<int> g7 = {2, 3, 1};
    std::vector<int> d7 = {40, 60, 20}; // requires 2,3,1 => total 6, gas 6, exactly break even. surpluses [0,0,0]; any start works, answer 0.
    assert(findAmpleCity(g7, d7) == 0);

    // Test with negative? Not allowed by task, but ensure no overflow handling needed.
    return 0;
}
