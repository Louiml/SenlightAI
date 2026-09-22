// Write a C++ function named `whoGetsTheDrink` that takes a single positive integer `n` representing the position in a queue of people waiting to order a drink at a counter. The line starts with 5 people in this order: Sheldon, Leonard, Penny, Rajesh, Howard. At each full “round,” each person at the front of the line orders exactly one drink, then moves to the back of the line, but the number of times each person appears in the line doubles every time the queue fully cycles. Specifically, in the first round, each of the 5 names appears once; after all 5 have ordered, the second round begins with each name appearing twice (so the line becomes Sheldon, Sheldon, Leonard, Leonard, Penny, Penny, Rajesh, Rajesh, Howard, Howard), then after all 10 have ordered, the third round has each name appear 4 times, and so on. Given `n` (1 ≤ n ≤ 10^18), return the name of the person who orders the n-th drink. The function must be efficient for very large `n` and must return the name as a `std::string`. Do not simulate the queue; instead, use arithmetic to determine which round the n-th order falls into and which person within that round. Edge cases include `n` exactly at a round boundary and very large `n` near 10^18.
The queue grows as a geometric progression per round. Let `r` be the number of times each name appears in the current round. Initially `r = 1`. In each complete round, the total number of orders is `r * 5`. If `n` is larger than that, we subtract `r * 5` from `n` and double `r`, moving to the next round. We repeat until `n` is no longer larger than `r * 5`. At that point, `n` lies within the current round, and each name appears `r` consecutive times. The index of the name is `(n - 1) / r` (integer division), which maps to the names array. This works because each person occupies `r` consecutive slots in the round. The algorithm runs in O(log n) time because the round sizes double each iteration, so the number of iterations is at most about 60 for n up to 10^18. Space usage is O(1) beyond the constant names array. Edge cases: if `n` is exactly at the end of a round, the loop exits when `n <= r * 5`, and the calculation still gives the last person. For `n = 1`, the loop doesn't run and `(0) / 1 = 0` gives Sheldon. The use of `long long` is essential to avoid overflow when `r` doubles beyond 10^18 (but since n ≤ 10^18, it's safe because we stop as soon as `r * 5 >= n`, and `r` never exceeds roughly `n/5`).
#include <string>
#include <vector>

// Return the name of the person who receives the n-th order in the doubling queue.
std::string whoGetsTheDrink(long long n) {
    // Fixed order of names in the queue for a single round.
    const std::vector<std::string> names = {"Sheldon", "Leonard", "Penny", "Rajesh", "Howard"};
    
    // Number of times each person appears consecutively in the current round.
    long long repeat = 1;
    
    // Skip full rounds until the n-th order falls inside the current round.
    while (repeat * 5 < n) {
        n -= repeat * 5;  // Remove the entire round's orders.
        repeat *= 2;      // Each person's repetitions double next round.
    }
    
    // In the current round, names are repeated 'repeat' times in order.
    // The index is computed by integer division of (n-1) by repeat.
    int index = static_cast<int>((n - 1) / repeat);
    return names[index];
}
#include <cassert>
#include <string>

// Declaration of the function to test.
std::string whoGetsTheDrink(long long n);

int main() {
    // Basic cases from the first round.
    assert(whoGetsTheDrink(1) == "Sheldon");
    assert(whoGetsTheDrink(5) == "Howard");
    
    // Start of second round (n=6) and end of second round (n=10).
    assert(whoGetsTheDrink(6) == "Sheldon");
    assert(whoGetsTheDrink(7) == "Sheldon");
    assert(whoGetsTheDrink(8) == "Leonard");
    assert(whoGetsTheDrink(10) == "Howard");
    
    // Third round: each appears 4 times, n=11 to n=30.
    assert(whoGetsTheDrink(11) == "Sheldon");
    assert(whoGetsTheDrink(12) == "Sheldon");
    assert(whoGetsTheDrink(13) == "Sheldon");
    assert(whoGetsTheDrink(14) == "Sheldon");
    assert(whoGetsTheDrink(15) == "Leonard");
    assert(whoGetsTheDrink(30) == "Howard");
    
    // Boundary between rounds: n=5 (last of round1) and n=6 (first of round2).
    assert(whoGetsTheDrink(5) == "Howard");
    assert(whoGetsTheDrink(6) == "Sheldon");
    
    // Very large n near 10^18; verify it returns a valid name (non-empty).
    std::string result = whoGetsTheDrink(1000000000000000000LL);
    assert(result == "Leonard"); // Precomputed result for this input.
    
    // Another large test: n = 5 * (2^40 - 1) + 1 → start of the 41st round.
    // 2^40 = 1099511627776, so 5*(2^40 -1) = 5497558138875, plus 1.
    long long boundary = 5LL * ((1LL << 40) - 1) + 1;
    assert(whoGetsTheDrink(boundary) == "Sheldon");
    
    return 0;
}
