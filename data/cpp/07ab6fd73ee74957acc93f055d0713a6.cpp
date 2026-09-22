Write a C++ function that, given a positive integer `k`, returns the `k`-th smallest positive integer whose decimal digit sum equals 10. For example, the first several such numbers are 19, 28, 37, 46, 55, 64, 73, 82, 91, 109, 118, ... (note that 100 is not included because its digit sum is 1). The function should handle `k` values that might be large (e.g., up to 10,000) efficiently, and must not produce numbers with leading zeros. The input integer `k` is guaranteed to be at least 1.

The straightforward approach is to iterate through positive integers starting from 1, compute the sum of digits for each, and count how many have a sum of 10 until we reach the `k`-th one. However, a smarter incremental approach seen in the snippet is to start from 19 (the first valid number) and add 9 repeatedly. Why add 9? Because adding 9 to a number typically increases its digit sum by 1 unless there is a carry that reduces the sum (e.g., 109 → 118, digit sum goes from 10 to 10, not 11). In fact, adding 9 to any number changes its digit sum by either +1 (no carry) or −8, −17, etc. (when carry occurs). Therefore, simply iterating every integer from 1 upward and checking digit sums is more robust and still O(n) where n is the actual value of the k-th number (which grows roughly linearly with k but with a coefficient around 9). For k up to 10,000, the number will be around 90,000, so the naive check is perfectly fine. The algorithm: initialize count = 0 and candidate = 1; while count < k: compute sum of digits of candidate; if sum == 10, increment count; if count == k, return candidate; otherwise candidate++. Edge cases: k=1 returns 19; numbers with digit sum 10 include 109 (digits 1+0+9=10) and 118, etc.; there is no upper bound issue for reasonable k. Time complexity is O(k * d), where d is the average number of digits (about 5 for 100k), so effectively O(k). Space is O(1) besides the return value.

#include <cstdint>

// Returns the k-th smallest positive integer whose decimal digit sum equals 10.
int kthDigitSumTen(int k) {
    int count = 0;
    int candidate = 1;
    while (count < k) {
        int num = candidate;
        int digitSum = 0;
        while (num > 0) {
            digitSum += num % 10;
            num /= 10;
        }
        if (digitSum == 10) {
            ++count;
            if (count == k) {
                return candidate;
            }
        }
        ++candidate;
    }
    return -1; // Should never reach here for valid positive k.
}

#include <cassert>

int main() {
    assert(kthDigitSumTen(1) == 19);
    assert(kthDigitSumTen(2) == 28);
    assert(kthDigitSumTen(3) == 37);
    assert(kthDigitSumTen(9) == 91);
    assert(kthDigitSumTen(10) == 109);
    assert(kthDigitSumTen(11) == 118);
    assert(kthDigitSumTen(12) == 127);
    assert(kthDigitSumTen(13) == 136);
    assert(kthDigitSumTen(14) == 145);
    assert(kthDigitSumTen(15) == 154);
}
