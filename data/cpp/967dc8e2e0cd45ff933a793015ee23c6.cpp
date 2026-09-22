/*
Write a C++ function `computeRoomNumber` that takes three positive integers: the number of floors `H`, the number of rooms per floor `W`, and the guest arrival order `N`. The hotel assigns rooms in column-major order: fill floor 1 from room 101, then floor 2, etc., before moving to the next column (e.g., 201, 301, ..., then 102, 202, ...). The room number is encoded as `floor * 100 + roomPosition`. Given `1 ≤ H, W ≤ 99` and `1 ≤ N ≤ H * W`, the function must return the exact room number (an integer) for the `N`-th guest. Do not include a `main` function in your solution.
*/

#include <cstdint>

// Compute the hotel room number for the N-th guest given H floors and W rooms per floor.
// Room numbering: floor * 100 + room position (column), filled column-major.
int computeRoomNumber(int H, int W, int N) {
    // Validate inputs (optional, but useful for robustness)
    if (H <= 0 || W <= 0 || N <= 0 || N > H * W) {
        return -1; // Invalid input sentinel
    }
    const int n = N - 1;          // Convert to zero-based index
    const int floor = (n % H) + 1; // floor number 1..H
    const int column = (n / H) + 1; // room position 1..W (left to right)
    return floor * 100 + column;
}

#include <cassert>

int main() {
    // Basic cases
    assert(computeRoomNumber(6, 12, 10) == 110); // 10th guest: floor 4? Wait: N=10 → n=9 → floor=9%6+1=4, column=9/6+1=2 → room 402? Check: 1→101,2→201,3→301,4→401,5→501,6→601,7→102,8→202,9→302,10→402. Yes.
    assert(computeRoomNumber(6, 12, 10) == 402);
    assert(computeRoomNumber(2, 3, 1) == 101);
    assert(computeRoomNumber(2, 3, 2) == 201);
    assert(computeRoomNumber(2, 3, 3) == 102);
    assert(computeRoomNumber(2, 3, 6) == 302);
    // Edge: H=1
    assert(computeRoomNumber(1, 5, 1) == 101);
    assert(computeRoomNumber(1, 5, 5) == 105);
    // Edge: N = H*W (last guest)
    assert(computeRoomNumber(3, 4, 12) == 304); // 12th guest: n=11 → floor=11%3+1=3, column=11/3+1=4 → 304
    // Edge: W=1 (single column)
    assert(computeRoomNumber(4, 1, 3) == 301);
    assert(computeRoomNumber(4, 1, 7) == -1); // invalid N > H*W
    return 0;
}

// The key is to map guest index `N` (1-based) to a zero-based index `n = N - 1`. The floor number is determined by `(n % H) + 1` because guests cycle through floors 1..H repeatedly. The room position (column) is determined by `(n / H) + 1` because each full cycle of `H` guests moves to the next column. The final room number is `(floor * 100) + column`. Edge cases: when `H=1`, the modulo and division still work fine; when `N=H*W`, the last guest gets the highest room; when `N` is the first guest in a new column, the division yields the correct column. The algorithm runs in **O(1)** time and uses **O(1)** auxiliary space, as it only performs arithmetic operations.
