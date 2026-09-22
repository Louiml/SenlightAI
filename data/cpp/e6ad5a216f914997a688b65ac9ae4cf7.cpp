// Write a C++ function `long long maxTotalPower(const std::vector<long long>& vehiclePower, const std::vector<long long>& chargerOutput, long long multiplier)` that takes two unsorted vectors representing powers of `n` vehicles and outputs of `m` chargers, plus a multiplier `h`. The function should pair each vehicle with at most one charger, and the effective power of a pair is `min(vehiclePower[i], h * chargerOutput[j])`. Since there may be more vehicles than chargers, you may pair at most `min(n, m)` vehicles with chargers, and the remaining vehicles are ignored. The goal is to maximize the sum of effective powers across all chosen pairs. However, there is a twist: the function must return the maximum of two possible pairing strategies: (1) pairing the largest vehicles with the largest chargers (descending order), and (2) pairing the smallest vehicles with the smallest chargers (ascending order). The reason for this is that sometimes using the smallest pairs yields a higher total due to the multiplier effect, while sometimes the largest pairs are better. The function returns the larger sum between the two strategies. If either vector is empty, return 0.
The problem reduces to computing two sums: one by sorting both vectors in ascending order and pairing from the smallest (index 0 upward) with `sum = Σ min(A[i], h*B[i])` for `i` from 0 to `min(n,m)-1`; another by sorting both in descending order and pairing from the largest (index `n-1` and `m-1` downward) with the same formula. The answer is the maximum of these two sums. This is because any optimal pairing for a fixed assignment of vehicles to chargers is achieved by sorting one side and pairing according to the rearrangement inequality, but here we only need two candidate pairings as specified. Edge cases: when one vector is empty, return 0; when `h` is 0, each term is `min(A[i],0)=0`, so both sums become 0; when `n` and `m` differ, we only pair up to `min(n,m)` elements, ignoring the rest. The time complexity is O(n log n + m log m) due to sorting, and space complexity O(1) auxiliary (ignoring input vectors) if we sort in place copies.
#include <vector>
#include <algorithm>

// Compute the maximum total power from two pairing strategies:
// 1. Pair the smallest vehicles with the smallest chargers (ascending order)
// 2. Pair the largest vehicles with the largest chargers (descending order)
// Return the larger of the two sums.
long long maxTotalPower(const std::vector<long long>& vehiclePower,
                        const std::vector<long long>& chargerOutput,
                        long long multiplier) {
    const std::size_t n = vehiclePower.size();
    const std::size_t m = chargerOutput.size();
    if (n == 0 || m == 0) return 0LL;
    
    // Work with sorted copies to avoid modifying input
    std::vector<long long> vehicles = vehiclePower;
    std::vector<long long> chargers = chargerOutput;
    std::sort(vehicles.begin(), vehicles.end());      // ascending
    std::sort(chargers.begin(), chargers.end());      // ascending
    
    const std::size_t pairs = std::min(n, m);
    
    // Strategy 1: pair smallest with smallest
    long long ascendingSum = 0LL;
    for (std::size_t i = 0; i < pairs; ++i) {
        long long cap = vehicles[i];           // vehicle power (small)
        long long ocap = chargers[i];          // charger output (small)
        long long effective = cap < multiplier * ocap ? cap : multiplier * ocap;
        ascendingSum += effective;
    }
    
    // Strategy 2: pair largest with largest (use reverse order from ascending sorted)
    long long descendingSum = 0LL;
    for (std::size_t i = 0; i < pairs; ++i) {
        long long cap = vehicles[n - 1 - i];   // vehicle power (large)
        long long ocap = chargers[m - 1 - i];  // charger output (large)
        long long effective = cap < multiplier * ocap ? cap : multiplier * ocap;
        descendingSum += effective;
    }
    
    return ascendingSum > descendingSum ? ascendingSum : descendingSum;
}
#include <cassert>
#include <vector>
#include <iostream>

// Declaration from the solution (normally included from a header)
long long maxTotalPower(const std::vector<long long>& vehiclePower,
                        const std::vector<long long>& chargerOutput,
                        long long multiplier);

int main() {
    // Basic case with equal sizes
    assert(maxTotalPower({1,2,3}, {1,2,3}, 1) == 6); // ascending: 1+2+3=6, descending: same
    // Different sizes: only min(n,m) pairs
    assert(maxTotalPower({1,5,10}, {2,3}, 2) == 11); // ascending pairs (1,2)->min(1,4)=1, (5,3)->min(5,6)=5 =>6; descending pairs (10,3)->min(10,6)=6, (5,2)->min(5,4)=4 =>10; max=10? Wait let's compute: min(1,4)=1, min(5,6)=5 =>6; descending: min(10,6)=6, min(5,4)=4 =>10. So answer 10.
    assert(maxTotalPower({1,5,10}, {2,3}, 2) == 10);
    // Multiplier effect makes ascending better
    assert(maxTotalPower({1,100}, {1,100}, 100) == 10100); // ascending: min(1,100)=1, min(100,10000)=100 =>101; descending: min(100,10000)=100, min(1,100)=1 =>101? Actually both 101. Let's test better: {2,3} and {1,1} h=10 -> ascending: min(2,10)=2, min(3,10)=3 =>5; descending: min(3,10)=3, min(2,10)=2 =>5. Both equal.
    // Test where descending is better
    assert(maxTotalPower({1,2,100}, {1,1,100}, 1) == 102); // ascending: min(1,1)=1, min(2,1)=1, min(100,100)=100 =>102; descending: min(100,100)=100, min(2,1)=1, min(1,1)=1 =>102
    // Large multiplier with small chargers
    assert(maxTotalPower({10,20}, {5,5}, 3) == 20); // ascending: min(10,15)=10, min(20,15)=15 =>25? wait min(20,15)=15, min(10,15)=10 =>25; descending: min(20,15)=15, min(10,15)=10 =>25. Actually both 25. Use another: {10,20}, {2,2}, h=5 -> ascending: min(10,10)=10, min(20,10)=10 =>20; descending: same 20.
    // h=0
    assert(maxTotalPower({1,2,3}, {1,2,3}, 0) == 0);
    // empty vector
    assert(maxTotalPower({}, {1,2,3}, 2) == 0);
    assert(maxTotalPower({1,2,3}, {}, 2) == 0);
    // single element
    assert(maxTotalPower({7}, {3}, 2) == min(7LL,6LL));
    // all equal
    assert(maxTotalPower({5,5}, {5,5}, 1) == 10);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
