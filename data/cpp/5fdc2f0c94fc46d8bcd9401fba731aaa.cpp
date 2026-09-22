/*
Write a C++ function named `finalPositionAfterAlternatingJumps` that takes three integer parameters: `forwardDistance` (a positive or zero value representing the jump distance in the positive direction), `backwardDistance` (a positive or zero value representing the jump distance in the negative direction), and `numberOfJumps` (a non-negative integer representing the total number of alternating jumps starting with a forward jump). The function must return a `long long` representing the net displacement after executing exactly `numberOfJumps` alternating jumps: first forward by `forwardDistance`, then backward by `backwardDistance`, then forward again, and so on. If `numberOfJumps` is even, the number of forward and backward jumps are equal; if odd, there is one more forward jump than backward. Ensure the function handles large values (up to 1e9) and computes the result without overflow using appropriate types.
*/

#include <cstdint>

// Returns net displacement after k alternating jumps: +a, -b, +a, -b, ...
long long finalPositionAfterAlternatingJumps(long long forwardDistance,
                                             long long backwardDistance,
                                             long long numberOfJumps) {
    long long forwardJumps = (numberOfJumps + 1) / 2; // ceil(k/2)
    long long backwardJumps = numberOfJumps / 2;      // floor(k/2)
    return forwardJumps * forwardDistance - backwardJumps * backwardDistance;
}

#include <cassert>

int main() {
    // Basic example from snippet: k=3, a=5, b=2 => 8
    assert(finalPositionAfterAlternatingJumps(5, 2, 3) == 8);

    // Even number of jumps: k=2, a=5, b=2 => +5-2 = 3
    assert(finalPositionAfterAlternatingJumps(5, 2, 2) == 3);

    // Zero jumps: result is 0
    assert(finalPositionAfterAlternatingJumps(100, 50, 0) == 0);

    // Zero distances: all stays at 0
    assert(finalPositionAfterAlternatingJumps(0, 0, 10) == 0);

    // Large values to test no overflow: k=1e9, a=1e9, b=1e9
    // forward jumps = 500000000, backward jumps = 500000000, net = 0
    assert(finalPositionAfterAlternatingJumps(1000000000LL, 1000000000LL, 1000000000LL) == 0);

    // Odd k with backward larger than forward: k=5, a=1, b=10 => +1-10+1-10+1 = -17
    assert(finalPositionAfterAlternatingJumps(1, 10, 5) == -17);

    // All backward jumps zero distance: k=4, a=7, b=0 => +7+7 = 14 (since forward jumps=2, backward=2 but backward contributes 0)
    assert(finalPositionAfterAlternatingJumps(7, 0, 4) == 14);

    // Mixed: k=1, a=3, b=100 => only one forward jump = 3
    assert(finalPositionAfterAlternatingJumps(3, 100, 1) == 3);

    // Large but asymmetric: k=1000000, a=123456789, b=987654321
    // forward = 500000, backward = 500000, net = 500000*(123456789-987654321) = -432098766000000
    assert(finalPositionAfterAlternatingJumps(123456789LL, 987654321LL, 1000000LL) == -432098766000000LL);

    return 0;
}

// The problem reduces to counting how many forward jumps and how many backward jumps occur. For `k = numberOfJumps`, the number of forward jumps is `(k + 1) / 2` (integer division, which rounds up for odd k) and the number of backward jumps is `k / 2` (integer division, which floors). The net displacement is `forwardJumps * forwardDistance - backwardJumps * backwardDistance`. This avoids simulating each jump, giving an O(1) time solution. Edge cases: when `k = 0`, both counts are 0, result is 0; when `forwardDistance` or `backwardDistance` is 0, it still works because multiplication by zero gives zero. Use `long long` for all intermediate multiplications to avoid overflow, since `k` can be up to 1e9 and distances up to 1e9, product can be up to 1e18 which fits in `long long` (max ~9e18). Space complexity is O(1) auxiliary.
