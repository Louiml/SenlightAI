// Write a C++ function that takes a non-negative integer `N` and returns an array of five integers `[A0, A1, A2, A3, A4]` representing the counts of 60-minute, 10-minute forward, 10-minute backward, 1-minute forward, and 1-minute backward increments needed to represent `N` minutes from midnight using a greedy rounding strategy. The function must implement the following algorithm: First, compute the number of full hours (60-minute blocks) in `N`. If the remainder after subtracting those hours is greater than 35, round up by using one additional hour and set the remainder to negative (i.e., `N` becomes `N - (hours+1)*60`); otherwise use the exact hours and set the remainder to `N % 60`. Then, for the 10-minute units, if the remainder is positive, take as many 10-minute forward blocks as possible; if the remainder after that is greater than 5, round up to the next 10-minute forward block and set the remainder to negative; similarly, if the remainder is negative, take as many 10-minute backward blocks as possible, and if the magnitude of the remainder after that is greater than 5, round up to the next 10-minute backward block and set the remainder to positive. Finally, fill the remaining minute counts: positive remainder goes to `A3`, negative remainder goes to `A4` (as absolute value). The function should return `std::array<int,5>` with these five counts. Ensure that all values are non-negative and the representation is minimal under this greedy rule.

// The algorithm proceeds in three stages: hours, tens, and ones. For any input `N`, we first try to fit as many 60-minute blocks as possible, but with a rounding rule: if the leftover minutes after removing `N/60` hours is greater than 35, we prefer to use one more hour and treat the remainder as negative (meaning we go backward from that hour). This mimics rounding to the nearest hour. Then, for the remaining amount (which could be positive, negative, or zero), we apply a similar rule for 10-minute blocks: if the remainder is positive, we take `remainder/10` forward 10-minute blocks, but if the remainder modulo 10 is greater than 5, we use one more forward 10-minute block and set the new remainder to `remainder - (count+1)*10` (which will be negative). If the remainder is negative, we take `abs(remainder)/10` backward 10-minute blocks, and if `abs(remainder)%10 > 5`, we use one more backward 10-minute block and set the new remainder to `remainder + (count+1)*10` (which becomes positive). Finally, the remaining value (between -5 and 5) is allocated to either forward single-minute count `A3` (if positive) or backward single-minute count `A4` (if absolute value of negative). Edge cases include `N=0` (all zeros), `N` exactly divisible by 60 or 10, and remainders like 6, 35, 36, etc. The time complexity is O(1) because only constant arithmetic operations are performed regardless of input size. Space complexity is O(1) for the output array.

#include <array>
#include <cstdlib>

// Returns counts of 60-min, 10-min forward, 10-min backward,
// 1-min forward, 1-min backward increments for a given N minutes.
std::array<int, 5> timeRepresentation(int N) {
    std::array<int, 5> A = {0, 0, 0, 0, 0};
    
    // Hour blocks
    A[0] = N / 60;
    if (N % 60 > 35) {
        // Round up to next hour, making remainder negative
        N -= (A[0] + 1) * 60;
        A[0]++;
    } else {
        N -= A[0] * 60;
    }
    
    // 10-minute blocks
    if (N > 0) {
        A[1] = N / 10;
        if (N % 10 > 5) {
            N -= (A[1] + 1) * 10;
            A[1]++;
        } else {
            N -= A[1] * 10;
        }
    } else if (N < 0) {
        A[2] = (-N) / 10;
        if ((-N) % 10 > 5) {
            N += (A[2] + 1) * 10;
            A[2]++;
        } else {
            N += A[2] * 10;
        }
    }
    
    // 1-minute blocks
    if (N > 0) {
        A[3] = N;
    } else if (N < 0) {
        A[4] = -N;
    }
    
    return A;
}

#include <cassert>
#include <array>
#include <iostream>

int main() {
    // Test exact hour
    auto r1 = timeRepresentation(120);
    assert(r1[0] == 2 && r1[1] == 0 && r1[2] == 0 && r1[3] == 0 && r1[4] == 0);
    
    // Test rounding hour up (36 minutes -> use 1 hour, 24 minutes backward)
    auto r2 = timeRepresentation(96);
    assert(r2[0] == 2 && r2[1] == 0 && r2[2] == 0 && r2[3] == 0 && r2[4] == 24);
    
    // Test normal hour and tens rounding (e.g., 41 minutes -> 0h, 4*10=40, 1 min forward? Actually 41%60=41, 41/10=4, 41%10=1 <=5 so 4 tens and 1 one)
    auto r3 = timeRepresentation(41);
    assert(r3[0] == 0 && r3[1] == 4 && r3[2] == 0 && r3[3] == 1 && r3[4] == 0);
    
    // Test tens rounding up (e.g., 66 -> 1h, remainder 6 -> 0 tens? Actually 66%60=6, 6<=35 so 1h, rem=6>5 => use 1 ten forward, remainder -4 => 4 ones backward)
    auto r4 = timeRepresentation(66);
    assert(r4[0] == 1 && r4[1] == 1 && r4[2] == 0 && r4[3] == 0 && r4[4] == 4);
    
    // Test negative remainder from hour rounding (e.g., 9 -> 0h, rem=9>5? 9%10=9>5 => use 1 ten forward, rem=-1 => 1 backward one)
    auto r5 = timeRepresentation(9);
    assert(r5[0] == 0 && r5[1] == 1 && r5[2] == 0 && r5[3] == 0 && r5[4] == 1);
    
    // Test zero
    auto r6 = timeRepresentation(0);
    assert(r6[0] == 0 && r6[1] == 0 && r6[2] == 0 && r6[3] == 0 && r6[4] == 0);
    
    // Test large value
    auto r7 = timeRepresentation(3600);
    assert(r7[0] == 60 && r7[1] == 0 && r7[2] == 0 && r7[3] == 0 && r7[4] == 0);
    
    // Test rounding hour down with small remainder (e.g., 35 -> 0h, rem=35 not >35, tens=3, rem=5 <=5 => 3 tens, 5 ones forward)
    auto r8 = timeRepresentation(35);
    assert(r8[0] == 0 && r8[1] == 3 && r8[2] == 0 && r8[3] == 5 && r8[4] == 0);
    
    // Test backward tens rounding (e.g., 54 -> 0h, rem=54, tens=5, rem=4 <=5 => 5 tens, 4 ones)
    auto r9 = timeRepresentation(54);
    assert(r9[0] == 0 && r9[1] == 5 && r9[2] == 0 && r9[3] == 4 && r9[4] == 0);
    
    // Test negative remainder from tens rounding (e.g., 56 -> 0h, rem=56, tens=5, rem=6>5 => use 6 tens, rem=-4 => 4 ones backward)
    auto r10 = timeRepresentation(56);
    assert(r10[0] == 0 && r10[1] == 6 && r10[2] == 0 && r10[3] == 0 && r10[4] == 4);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
