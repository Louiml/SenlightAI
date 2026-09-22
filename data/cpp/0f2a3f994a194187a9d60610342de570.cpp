// Given a non-negative integer `N` (0 <= N <= 10^9), write a C++ function `bool isReorderedPowerOfTwo(int N)` that returns `true` if it is possible to reorder the digits of `N` (including the case of no reordering) to form a valid power of 2 (i.e., 1, 2, 4, 8, 16, 32, ...). Leading zeros are not allowed in the reordered number (so a digit string like `"0012"` is invalid, but `"1002"` is valid). The original `N` may have leading zeros in its decimal representation? No — `N` is a standard integer, so it has no leading zeros as given. However, when rearranging, you must ensure the resulting number does not start with zero. For example, `N=10` (digits "10") can be reordered to "01" which is invalid, but "10" itself is valid (since 10 is not a power of 2, the function returns false). For `N=1`, the digits "1" form 1 = 2^0, so return true. For `N=46`, digits "46" can be reordered to "64" = 2^6, so return true. For `N=23`, none of the permutations (23 or 32) are powers of 2, so return false. Implement the function efficiently for up to 10^9 (at most 10 digits).

The key observation is that the property "can be reordered to form a power of 2" depends only on the multiset of digits of `N`, not on their order. Therefore, we can precompute the canonical digit multiset (sorted string) for every power of two that has at most 10 digits (since 2^29 = 536,870,912 is the largest power of 2 ≤ 10^9; 2^30 = 1,073,741,824 exceeds the limit). There are only 30 such powers. For each power, convert it to a string and sort its digits. Store these sorted strings in a set. Then, for the input `N`, convert it to a string, sort its digits, and check if that string exists in the set. If it does, return true; otherwise false. Edge cases: `N=0` — the digits are "0", but no power of 2 has only the digit '0' (since every power of 2 is positive and has at least one non-zero digit, and at least '1' for the smallest), so false. `N=1` → true. `N` up to 10^9 but powers of 2 with leading zeros in their decimal representation? Not applicable, as powers of 2 have no leading zeros. The integer `N` also has no leading zeros. The reordering constraint about not starting with zero is automatically satisfied because we are only checking digit multisets; if a permutation starting with zero were the only way to form a power of 2, then the multiset would not match any power of 2 (since every power of 2 starts with a non-zero digit). But careful: Could a power of 2 have a digit multiset that, when sorted, contains a leading zero? No, sorted strings start with the smallest digit; if there is a zero, the sorted string starts with '0', but the power of 2 itself never starts with zero, so its sorted string will have exactly the same digits but in sorted order, so if the power of 2 contains a zero, the sorted string starts with '0' — that's fine because we are not constructing a number, we are just comparing multisets. The constraint "leading zeros not allowed" is automatically enforced by the fact that we compare sorted digit strings: if the input has a zero and the power of 2 also has a zero, then there exists a permutation that starts with a non-zero digit (the power itself), so it's valid. Thus, no extra check needed. Time complexity: Precomputing 30 powers, each with up to 10 digits, takes O(30 * 10 log 10) ≈ O(1). For each call, converting N to string and sorting its digits takes O(d log d) where d ≤ 10, so O(1). Space complexity: O(30) strings stored, O(1). The solution is constant time per query.

#include <string>
#include <set>
#include <algorithm>

// Returns true if the decimal digits of N can be rearranged (including original order)
// to form a power of two (1, 2, 4, 8, 16, ...). Leading zeros are not allowed,
// but this constraint is automatically satisfied by comparing digit multisets.
bool isReorderedPowerOfTwo(int N) {
    // Precompute sorted digit strings for all powers of two up to 10^9.
    static const std::set<std::string> powers = [] {
        std::set<std::string> s;
        long long value = 1;
        while (value <= 1000000000LL) {
            std::string digits = std::to_string(value);
            std::sort(digits.begin(), digits.end());
            s.insert(digits);
            value *= 2;
        }
        return s;
    }();

    // Build sorted digit string for the given number.
    std::string digits = std::to_string(N);
    std::sort(digits.begin(), digits.end());
    return powers.count(digits) > 0;
}

#include <cassert>

int main() {
    // Basic cases
    assert(isReorderedPowerOfTwo(1) == true);        // 1 = 2^0
    assert(isReorderedPowerOfTwo(2) == true);        // 2 = 2^1
    assert(isReorderedPowerOfTwo(4) == true);        // 4 = 2^2
    assert(isReorderedPowerOfTwo(8) == true);        // 8 = 2^3
    assert(isReorderedPowerOfTwo(16) == true);       // 16 = 2^4

    // Reordering cases
    assert(isReorderedPowerOfTwo(61) == true);       // 61 -> 16 = 2^4
    assert(isReorderedPowerOfTwo(46) == true);       // 46 -> 64 = 2^6
    assert(isReorderedPowerOfTwo(1024) == true);     // 1024 = 2^10
    assert(isReorderedPowerOfTwo(4210) == true);     // 4210 -> 1024? no, actually 4210 digits {0,1,2,4} -> can form 1024? yes, 1024 uses digits 1,0,2,4, so true
    assert(isReorderedPowerOfTwo(125) == true);      // 125 -> 512 = 2^9? 512 digits are {5,1,2}, same, so true

    // False cases
    assert(isReorderedPowerOfTwo(0) == false);       // no power of 2 has digit '0' alone
    assert(isReorderedPowerOfTwo(3) == false);       // 3 cannot be reordered to power of 2
    assert(isReorderedPowerOfTwo(23) == false);      // 23 or 32 not powers of 2
    assert(isReorderedPowerOfTwo(10) == false);      // only permutations "01" (invalid leading zero) and "10" (not power of 2)
    assert(isReorderedPowerOfTwo(99) == false);      // 99 not a power of 2, no permutation helps
}
