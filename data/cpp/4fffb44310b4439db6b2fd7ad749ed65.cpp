/*
Given two integers `l` and `r` (where `0 ≤ l ≤ r ≤ 1e18`), write a C++ function that determines the number of times you can repeatedly divide `r` by 2 (using integer division) until `r` becomes 0, then subtract 1 from that count, and return the result. In other words, return the floor of log base 2 of `r`. If `r` is 0, return 0 (since the loop in a reference implementation would decrement after the division, yielding 0). The function should handle very large values correctly using a 64-bit integer type.
*/
#include <cstdint>

// Returns floor(log2(r)) for r > 0, and 0 for r == 0.
// Equivalent to the number of times r can be halved until it reaches 1,
// which is the index of the most significant set bit.
std::int64_t floorLog2(std::int64_t r) {
    if (r == 0) return 0;
    std::int64_t count = 0;
    while (r > 0) {
        r /= 2;
        ++count;
    }
    return count - 1;
}
#include <cassert>
#include <cstdint>

// Placeholder for the solution function (include the actual implementation above).
std::int64_t floorLog2(std::int64_t r);

int main() {
    assert(floorLog2(0) == 0);
    assert(floorLog2(1) == 0);
    assert(floorLog2(2) == 1);
    assert(floorLog2(3) == 1);
    assert(floorLog2(4) == 2);
    assert(floorLog2(7) == 2);
    assert(floorLog2(8) == 3);
    assert(floorLog2(15) == 3);
    assert(floorLog2(16) == 4);
    assert(floorLog2(1000000000000000000LL) == 59); // 2^59 ≈ 5.76e17, 2^60 > 1e18
    assert(floorLog2(1LL << 60) == 60); // 2^60 exactly
    assert(floorLog2((1LL << 60) - 1) == 59); // one less than 2^60
    return 0;
}
// The problem reduces to computing the bit length of `r` minus 1, which is the position of the most significant set bit (0-indexed). For example, `r=8` (binary 1000) has bit length 4, so the answer is 3; `r=7` (111) has bit length 3, answer is 2. The reference snippet repeatedly divides `r` by 2, counting iterations `i`, then outputs `i-1`. For `r=0`, the loop doesn't execute (since `while(r)` is false), `i` stays 0, then `i--` gives -1? Actually careful: In the snippet, `i` starts at 0, loop condition `while(r)` — if `r` is 0, loop skips, then `i--` makes `i=-1`, and prints -1. But the task should avoid negative result for `r=0`; typical definition of log2(0) is undefined, but we can define it as 0 for safety. Since the problem likely expects a non-negative result, we'll define that if `r==0`, return 0. The algorithm: if `r==0` return 0. Else, initialize `count=0`, while `r>0` do `r/=2; count++`. Then return `count-1`. This yields the floor of log2(r). Edge cases: `r=1` → count=1 → return 0; `r=2` → count=2 → return 1; `r=1e18` fits in 64-bit. Time complexity: O(log r) ≈ 60 iterations maximum, constant space O(1).
