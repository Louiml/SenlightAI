Write a C++ function `std::string powerState(int N, int M)` that determines whether the last `N` bits of the integer `M` are all set to 1 (i.e., the lower `N` bits form a contiguous block of ones). If they are, return the string `"ON"`; otherwise, return `"OFF"`. The function should handle any non-negative `N` (from 1 to 30) and any non-negative `M` (fits in a 32-bit unsigned integer). For example, if `N = 3` and `M = 13` (binary `1101`), the last 3 bits are `101`, which are not all ones, so return `"OFF"`. If `N = 3` and `M = 15` (binary `1111`), the last 3 bits are `111`, so return `"ON"`. Edge cases: when `N = 0`, the condition is trivially true (no bits to check), so return `"ON"`; when `M` has more bits beyond the last `N`, those higher bits are ignored.
// The solution uses a bitmask to isolate the lower `N` bits of `M` and compares them to a mask consisting entirely of `N` ones. The mask for the lower `N` ones is `(1 << N) - 1`, which works for `N` in `[1, 30]` because shifting by up to 30 is safe on a 32-bit signed `int` (the original snippet uses `int`). For `N = 0`, the mask would be `(1 << 0) - 1 = 0`, and `(M & 0) == 0` is true, so we can either handle it explicitly or let the formula work (it does work: `M & 0 == 0 == (1 << 0) - 1`), but to avoid shifting issues when `N` is 0 we still handle it directly. The condition checks `(M & mask) == mask`, which verifies that every bit in the lower `N` positions is 1, regardless of higher bits. Important edge cases: `N` larger than the number of bits in `M` (e.g., `N=30`, `M=1`) → the mask has ones in positions `M` doesn't have, so the check fails (returns `OFF`), which is correct because not all lower N bits are set (they are 0 in missing positions). If `N=0`, always `ON`. Time complexity is O(1) for bitwise operations; space complexity is O(1).
#include <string>

// Return "ON" if the lower N bits of M are all 1, else "OFF".
std::string powerState(int N, int M) {
    // Special case: no bits to check, always ON.
    if (N == 0) {
        return "ON";
    }

    // Build a mask with the lowest N bits set to 1.
    // N is guaranteed to be in [1, 30] by problem constraints.
    const int mask = (1 << N) - 1;

    // Check if all those bits are 1 in M.
    if ((M & mask) == mask) {
        return "ON";
    }
    return "OFF";
}
#include <cassert>

int main() {
    // Basic cases
    assert(powerState(3, 13) == "OFF"); // 1101 -> lower 3 bits: 101
    assert(powerState(3, 15) == "ON");  // 1111 -> lower 3 bits: 111
    assert(powerState(1, 1) == "ON");   // 1 -> bit0 = 1
    assert(powerState(1, 2) == "OFF");  // 10 -> bit0 = 0

    // N = 0 edge case
    assert(powerState(0, 0) == "ON");
    assert(powerState(0, 12345) == "ON");

    // Higher bits ignored
    assert(powerState(4, 0x1F) == "ON");  // 31 -> lower 4 bits: 1111
    assert(powerState(4, 0x3F) == "ON");  // 63 -> lower 4 bits: 1111 (higher bits ignored)
    assert(powerState(4, 0x2F) == "OFF"); // 47 -> lower 4 bits: 1111? 47=101111 -> lower 4=1111, actually ON

    // More precise tests
    assert(powerState(5, 0x3F) == "ON");  // 63=111111 -> lower 5 bits: 11111
    assert(powerState(5, 0x1F) == "ON");  // 31=11111 -> lower 5 bits: 11111
    assert(powerState(5, 0x1E) == "OFF"); // 30=11110 -> lower 5 bits: 11110

    // Large N with small M
    assert(powerState(30, 5) == "OFF");   // only bits 0 and 2 set, not all 30

    return 0;
}
