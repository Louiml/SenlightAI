/*
Write a C++ function that takes five integers: the damage cycle lengths of four dragons (a, b, c, d) and a total number of days (e). Each day, if the day number is divisible by any of the four dragon cycle lengths, that dragon attacks and the day is marked as "damaged." The function should return the count of distinct damaged days from day 1 to day e inclusive. Assume all inputs are positive integers and e can be as large as 10^9, but the cycle lengths are at most 10^5. The function must be efficient enough to handle large e without iterating through all days.
*/
#include <cstdint>
#include <numeric>
#include <vector>

// Count number of distinct damaged days from 1 to e inclusive,
// where a day is damaged if divisible by any of a, b, c, or d.
long long countDamagedDays(int a, int b, int c, int d, long long e) {
    // Use unsigned long long for LCM to avoid overflow, but cap at e+1
    std::vector<unsigned long long> cycles = {
        static_cast<unsigned long long>(a),
        static_cast<unsigned long long>(b),
        static_cast<unsigned long long>(c),
        static_cast<unsigned long long>(d)
    };

    long long damaged = 0;
    // Enumerate all non-empty subsets via bitmask from 1 to 15
    for (int mask = 1; mask < 16; ++mask) {
        unsigned long long lcm = 1;
        int bitCount = 0;
        for (int i = 0; i < 4; ++i) {
            if (mask & (1 << i)) {
                ++bitCount;
                // Compute LCM incrementally; cap to e+1 to avoid overflow
                unsigned long long g = std::gcd(lcm, cycles[i]);
                unsigned long long temp = (lcm / g) * cycles[i];
                if (temp > static_cast<unsigned long long>(e)) {
                    // If LCM exceeds e, no day in range is divisible by it
                    lcm = static_cast<unsigned long long>(e) + 1;
                    break;
                }
                lcm = temp;
            }
        }
        if (lcm > static_cast<unsigned long long>(e)) continue;
        long long count = e / static_cast<long long>(lcm);
        // Inclusion-exclusion: add for odd bitCount, subtract for even
        if (bitCount % 2 == 1) {
            damaged += count;
        } else {
            damaged -= count;
        }
    }
    return damaged;
}
#include <cassert>
#include <iostream>

// Prototype of the solution function (already defined above)
long long countDamagedDays(int a, int b, int c, int d, long long e);

int main() {
    // Example from original problem: a=2, b=3, c=4, d=5, e=12
    // Days divisible by 2: 2,4,6,8,10,12 (6 days)
    // By 3: 3,6,9,12 (4 days) -> but 6,12 already counted
    // By 4: 4,8,12 (3 days) -> 4,8,12 already counted? Actually 12 new? Let's compute manually:
    // Distinct damaged days: 2,3,4,5,6,8,9,10,12 = 9 days
    assert(countDamagedDays(2, 3, 4, 5, 12) == 9);

    // All cycles divide every day: a=b=c=d=1, e=10 -> all days damaged = 10
    assert(countDamagedDays(1, 1, 1, 1, 10) == 10);

    // No damage? Not possible since any day divisible by 1, but assume large cycles
    // a=6, b=10, c=15, d=30, e=5 -> none divide 1..5 except? 30>5, 15>5, 10>5, 6>5 -> 0
    assert(countDamagedDays(6, 10, 15, 30, 5) == 0);

    // Single divisible cycle: a=7, b=11, c=13, d=17, e=100 -> only multiples of 7,11,13,17
    // Count = floor(100/7)+floor(100/11)+floor(100/13)+floor(100/17) minus overlaps
    // Overlaps are multiples of lcm pairs, triplets, quadruple – but all cycles are coprime? Actually 7,11,13,17 are pairwise coprime.
    // So count = 14+9+7+5 = 35
    assert(countDamagedDays(7, 11, 13, 17, 100) == 35);

    // Handle large e (e.g., 10^9) with small cycles
    assert(countDamagedDays(1, 2, 3, 4, 1000000000LL) == 1000000000LL); // a=1 divides all

    // Edge case: e=1, cycles >1 -> only day 1 if any cycle is 1 or divides 1? None except 1
    assert(countDamagedDays(2, 3, 4, 5, 1) == 0);

    std::cout << "All tests passed!\n";
    return 0;
}
// The naive approach of iterating from 1 to e and checking divisibility by each of the four numbers would be O(e) time, which is infeasible for e up to 10^9. Instead, we can use the inclusion-exclusion principle to count the number of days divisible by at least one of the four numbers. For each non-empty subset of {a, b, c, d}, compute the least common multiple (LCM) of the subset, then add or subtract floor(e / LCM) based on the subset size (add for odd-sized subsets, subtract for even-sized). This yields the count of distinct damaged days in O(1) time after computing LCMs, where each LCM computation is O(log max_value) using the GCD. The number of subsets is 2^4 - 1 = 15, so the total operations are constant. Edge cases include when two cycle lengths are equal (e.g., a and b both 5), but inclusion-exclusion still works because LCM handles duplicates correctly. Also, LCM can overflow 64-bit if the numbers are large (e.g., 10^5 each, LCM can be up to 10^20), so we need to use a larger integer type like `unsigned long long` or `__int128` and cap the LCM if it exceeds e to avoid overflow (since if LCM > e, floor(e/LCM) = 0). Time complexity is O(1) after constant subset enumeration, and space complexity is O(1).
