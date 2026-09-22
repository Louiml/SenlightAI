// Write a C++ function `progressiveCharge` that takes two integers: `totalBill` (the total amount charged for electricity in arbitrary currency units) and `billDifference` (a non-negative integer representing the absolute difference between the bills for two neighboring households), and returns the smaller of the two household bills. The billing system uses progressive tiers: the first 100 units cost 2 per unit, the next 9,900 units cost 3 per unit, the next 990,000 units cost 5 per unit, and any remaining units cost 7 per unit. The input `totalBill` is guaranteed to be a valid total bill for some integer number of consumed units. The output should be the smaller bill (in currency units) such that both households have integer consumption, their total consumption equals the consumption corresponding to `totalBill`, and the absolute difference between their bills equals `billDifference`. If multiple valid pairs exist, choose the one with the largest total consumption for the larger household (i.e., the pair closest to equal sharing that satisfies the difference). If no such pair exists, the function should return -1.
The solution proceeds in two phases. First, convert the input `totalBill` back to the total number of units consumed using an inverse of the progressive charge function. This inverse walks through the same tier boundaries: since the first 100 units cost 2 each, a bill of up to 200 corresponds to `bill / 2` units. For bills greater than that, subtract the cost of the first block (200) and add 100 units, then handle the next block of 9,900 units at 3 per unit (cost up to 29,700), then the next block of 990,100 units at 5 per unit (cost up to 4,950,500), and finally the remainder at 7 per unit. Because the input is guaranteed valid, this inversion is exact. Let `totalUnits` be the computed total consumption. Next, we need to find two integers `i` and `j` such that `i + j == totalUnits`, both non-negative, and `abs(charge(i) - charge(j)) == billDifference`. We want the pair that maximizes `max(i, j)` (i.e., the pair where the larger share is as large as possible, which means the smaller share is as small as possible). Since the charge function is monotonically increasing, iterating `i` from `totalUnits` down to `totalUnits/2` (so that `i >= j`) and checking `j = totalUnits - i` yields the pair with the largest possible `i` first. For each candidate, compute the two charges and compare the absolute difference. If found, return the smaller bill (which corresponds to `charge(j)` since `j <= i`). If the loop completes without a match, return -1. Edge cases: if `totalBill` is 0, then `totalUnits` is 0 and only `i=0,j=0` works; if `billDifference` is 0, the only valid pair is when `i == j` (so `totalUnits` must be even). Complexity: Inverting the bill takes O(1) time. The search loop iterates at most `totalUnits/2` times, but in the worst case (very large units) this could be impractical; however, for typical programming contest inputs the loop is bounded by the input magnitude, so we treat it as O(N) where N is total units, and O(1) auxiliary space.
#include <cstdlib>
#include <cmath>

// Compute the bill for a given number of consumed units.
long long chargeForUnits(long long units) {
    long long cost = 0;
    if (units <= 100) {
        return units * 2;
    }
    cost = 200;
    units -= 100;
    if (units <= 9900) {
        return cost + units * 3;
    }
    cost += 29700;
    units -= 9900;
    if (units <= 990100) {
        return cost + units * 5;
    }
    cost += 4950500;
    units -= 990100;
    return cost + units * 7;
}

// Convert a total bill back to the total units consumed (inverse of chargeForUnits).
long long unitsFromBill(long long bill) {
    long long units = 0;
    if (bill <= 200) {
        return bill / 2;
    }
    units = 100;
    bill -= 200;
    if (bill <= 29700) {
        return units + bill / 3;
    }
    units += 9900;
    bill -= 29700;
    if (bill <= 4950500) {
        return units + bill / 5;
    }
    units += 990100;
    bill -= 4950500;
    return units + bill / 7;
}

// Given totalBill and billDifference, return the smaller household bill.
// Return -1 if no valid pair exists.
long long progressiveCharge(long long totalBill, long long billDifference) {
    long long totalUnits = unitsFromBill(totalBill);
    
    // Search for the pair with the largest larger share.
    for (long long i = totalUnits; i >= totalUnits / 2; --i) {
        long long j = totalUnits - i;
        long long billI = chargeForUnits(i);
        long long billJ = chargeForUnits(j);
        long long diff = std::llabs(billI - billJ);
        if (diff == billDifference) {
            // Since i >= j, billJ is the smaller (or equal) bill.
            return (billJ < billI) ? billJ : billI;
        }
    }
    return -1;
}
#include <cassert>

int main() {
    // Test case from the original snippet style: bill=300, diff=100
    // totalUnits for 300 = 100 + (100/3)=133 (since 300-200=100, /3=33, total 133)
    // For total=133, pairs: 66+67 -> charges 132 and 134, diff 2; 50+83 ->100 and 166, diff 66; etc.
    // Actually test with small numbers: totalBill=200 (100 units), diff=0 -> i=50,j=50, bills 100 and 100, return 100.
    assert(progressiveCharge(200, 0) == 100);
    
    // totalBill=202 (101 units: 100 units + 1 unit at 3) -> charges for 50 and 51 = 100 and 102, diff 2
    // The smaller bill is 100.
    assert(progressiveCharge(202, 2) == 100);
    
    // totalBill=4 (2 units) -> total units=2, i=2,j=0 -> charges 4 and 0, diff 4
    assert(progressiveCharge(4, 4) == 0);
    
    // totalBill=4, diff=0 -> only i=j=1 -> charges 2 and 2, diff 0, smaller=2
    assert(progressiveCharge(4, 0) == 2);
    
    // totalBill=0, diff=0 -> units=0, i=j=0 -> charges 0 and 0
    assert(progressiveCharge(0, 0) == 0);
    
    // No valid pair: totalBill=4 (2 units), diff=1 -> impossible because only bills (2,2) or (0,4)
    assert(progressiveCharge(4, 1) == -1);
    
    // Larger test: totalBill=29700+200=29900 (100+9900=10000 units) -> total units=10000
    // For diff=0, i=j=5000 -> charges: each 200+ (4900*3)=200+14700=14900, smaller=14900
    assert(progressiveCharge(29900, 0) == 14900);
    
    // Test that we return the smaller bill when diff is not zero: totalBill=29904 (10000 units + 2 units at 5? Actually 29904 - 29900 = 4, but 4/5 is not integer, so invalid. Use valid: totalBill=29905 (10000 units + 1 unit at 5) -> units=10001, pair i=5001,j=5000 -> charges: i=200+14700+5=14905, j=14900, diff=5, smaller=14900
    assert(progressiveCharge(29905, 5) == 14900);
    
    // Test no valid pair for large diff: totalBill=29900 (10000 units), diff=1 -> impossible because all charges are integers and only possible diffs are even? Actually charges differ by multiples of 3 or 5 in this region, but diff=1 is impossible
    assert(progressiveCharge(29900, 1) == -1);
    
    return 0;
}
