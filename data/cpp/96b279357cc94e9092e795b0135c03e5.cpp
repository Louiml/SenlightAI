Write a C++ function `findCycle` that takes a pointer to an integer function `f(int)`, an initial value `x0`, and returns a `std::pair<int, int>` where the first element is the smallest index `mu` (the index of the first repeated element in the sequence `x0, f(x0), f(f(x0)), ...`) and the second element is the cycle length `lambda` (the smallest positive integer such that `x(mu) == x(mu + lambda)`). The function must work for any function `f` that maps integers to integers, including cases where the function produces negative values or large values that may overflow only if the function itself overflows (your code should not introduce additional overflow). The sequence is guaranteed to eventually repeat (i.e., it is not infinite without repetition). The function must use constant extra space (O(1) auxiliary memory) and must not modify the input function or the initial value. You may assume that `f` is a pure function (no side effects) and that `x0` and all generated values fit in `int`.
// The solution uses Floyd’s cycle-finding algorithm (also known as the “tortoise and hare” algorithm). The approach has three phases:
// 1. **Detection phase**: Initialize `tortoise = f(x0)` and `hare = f(f(x0))`. Then repeatedly advance `tortoise` by one step (`tortoise = f(tortoise)`) and `hare` by two steps (`hare = f(f(hare))`) until they meet. This meeting point is guaranteed to be inside the cycle, and the number of steps taken is proportional to `mu + lambda`. This phase works because the hare moves twice as fast as the tortoise, so they must collide somewhere in the cycle.
// 2. **Finding `mu`**: Reset `tortoise = x0` and set `hare` to the current meeting point (which is in the cycle). Then advance both by one step at a time until they meet again. The number of steps counted until they meet is exactly `mu`, because the distance from the start to the cycle entrance is the same for the tortoise starting at `x0` and the hare starting at the meeting point.
// 3. **Finding `lambda`**: Keep `tortoise` at the meeting point (which is `x(mu)`) and set `hare = f(tortoise)`. Then advance `hare` one step at a time, counting steps, until `hare` equals `tortoise`. The count is `lambda`, the exact cycle length.
//
// Edge cases: If the sequence has a cycle of length 1 (i.e., `f` maps some value to itself), the algorithm still works: in phase 1, `tortoise` and `hare` meet immediately after first steps; phase 2 yields `mu` as the index where the self-loop starts; phase 3 will count 1 step. If `mu = 0` (meaning `x0` itself is in the cycle), the detection phase still works because `tortoise = f(x0)` and `hare = f(f(x0))` may not be equal to `x0`, but they will meet, and phase 2 will produce `mu = 0` because `tortoise = x0` already equals `hare` (the meeting point). However, note: in phase 2, we set `hare = tortoise` (meeting point) and `tortoise = x0`; if `x0` equals the meeting point, the while loop doesn’t execute, leaving `mu = 0`. This correctly handles the case where `x0` is part of the cycle. Negative values and large values are fine because the algorithm only applies `f` to values already produced, so no additional overflow is introduced beyond what `f` itself does.
//
// Time complexity is `O(mu + lambda)` because the detection phase runs at most `mu + lambda` steps, and each of the other phases runs at most `mu` and `lambda` steps respectively. Space complexity is `O(1)` because only a few integer variables are used.
#include <utility> // for std::pair

/*
 * Finds the cycle in the sequence defined by x0, f(x0), f(f(x0)), ...
 * Returns a pair (mu, lambda) where:
 *   - mu is the smallest index i such that x(i) repeats.
 *   - lambda is the smallest positive integer such that x(mu) == x(mu + lambda).
 * Uses Floyd's cycle-finding algorithm with O(1) auxiliary space.
 */
std::pair<int, int> findCycle(int (*f)(int), int x0) {
    // Phase 1: detect a meeting point inside the cycle.
    int tortoise = f(x0);
    int hare = f(f(x0));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    // Phase 2: find the start of the cycle (mu).
    int mu = 0;
    hare = tortoise; // hare is now at the meeting point in the cycle.
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(hare);
        ++mu;
    }

    // Phase 3: find the length of the cycle (lambda).
    int lambda = 1;
    hare = f(tortoise);
    while (tortoise != hare) {
        hare = f(hare);
        ++lambda;
    }

    return std::make_pair(mu, lambda);
}
#include <cassert>
#include <utility>

// Example function: linear congruential generator with offset.
int f_mod(int x) {
    // x -> (3*x + 2) % 7
    return (3 * x + 2) % 7;
}

// Example function with a self-loop at a non-zero index.
int self_loop(int x) {
    if (x < 0) return x + 1; // never used in our tests
    if (x == 5) return 5;    // self-loop at 5
    return (x + 1) % 10;     // simple increment mod 10
}

// Example function with a short cycle and a tail.
int tail_cycle(int x) {
    if (x == 0) return 1;  // start
    if (x == 1) return 2;
    if (x == 2) return 3;
    if (x == 3) return 1;  // 1->2->3->1 cycle
    return 0; // unreachable in our sequence
}

int main() {
    // Test 1: sequence from 2: 2,1,5,3,4,0,2,... cycle starts at 0? Let's compute:
    // x0=2 -> f(2)= (6+2)%7=1 -> f(1)=5 -> f(5)= (15+2)%7=3 -> f(3)= (9+2)%7=4 -> f(4)= (12+2)%7=0 -> f(0)=2 ...
    // So sequence: 2,1,5,3,4,0,2,... The cycle is from index 0 (x0=2) with length 6? But wait: check from index 0: 2,1,5,3,4,0,2 -> yes, cycle period 6, mu=0, lambda=6.
    std::pair<int, int> ans = findCycle(&f_mod, 2);
    assert(ans.first == 0 && ans.second == 6);

    // Test 2: starting from 0 with same function:
    // x0=0 -> f(0)=2 -> f(2)=1 -> f(1)=5 -> f(5)=3 -> f(3)=4 -> f(4)=0 ... cycle from 0: 0,2,1,5,3,4,0 -> mu=0, lambda=6.
    ans = findCycle(&f_mod, 0);
    assert(ans.first == 0 && ans.second == 6);

    // Test 3: start from 5 with same function:
    // x0=5 -> f(5)=3 -> f(3)=4 -> f(4)=0 -> f(0)=2 -> f(2)=1 -> f(1)=5 ... cycle from index 0 with mu? The cycle is 5,3,4,0,2,1,5 -> mu=0, lambda=6.
    ans = findCycle(&f_mod, 5);
    assert(ans.first == 0 && ans.second == 6);

    // Test 4: self-loop function: start at 0 -> sequence: 0->1->2->3->4->5->5->5...
    // The cycle begins at index 5 (value 5) with lambda=1. mu=5? Let's see: indices: 0:0,1:1,2:2,3:3,4:4,5:5,6:5,7:5,... so mu=5 (first repeat at index 5 because x4=4, x5=5, x6=5? Wait: index 5 has value 5, index 6 also 5, so the first index where a value repeats? Actually the definition: mu is the smallest index i such that x(i) appears later. For i=0, x0=0 never appears again. i=1 ->1 not repeated, i=2->2, i=3->3, i=4->4, i=5->5 appears at index 6, so mu=5. Lambda is the distance from mu to the next occurrence: 1 (since x5==x6). So (mu=5, lambda=1).
    ans = findCycle(&self_loop, 0);
    assert(ans.first == 5 && ans.second == 1);

    // Test 5: self-loop with start at 5 directly: x0=5 -> sequence: 5,5,5,... mu=0, lambda=1.
    ans = findCycle(&self_loop, 5);
    assert(ans.first == 0 && ans.second == 1);

    // Test 6: tail + cycle: start at 0 -> 0->1->2->3->1->2->3... The tail is 0 (index 0), then cycle is 1->2->3->1 (length 3). mu=1 (since index 1 is first repeated? Let's see: values: index0:0, index1:1, index2:2, index3:3, index4:1 (repeat of index1), so mu=1, lambda=3.
    ans = findCycle(&tail_cycle, 0);
    assert(ans.first == 1 && ans.second == 3);

    // Test 7: tail_cycle with start at 1: sequence: 1->2->3->1... mu=0, lambda=3.
    ans = findCycle(&tail_cycle, 1);
    assert(ans.first == 0 && ans.second == 3);

    // Test 8: tail_cycle with start at 3: 3->1->2->3... mu=0, lambda=3.
    ans = findCycle(&tail_cycle, 3);
    assert(ans.first == 0 && ans.second == 3);

    return 0;
}
