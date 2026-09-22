// Write a C++ function that takes an integer `n` (1 ≤ n ≤ 10^9) and returns the smallest positive integer that has the same digit sum as `n` and is strictly greater than `n`. If no such integer exists (which happens only when `n` consists entirely of 9s, e.g., 9, 99, 999), return `-1`. For example, given `n = 20`, the digit sum is 2, and the smallest number greater than 20 with digit sum 2 is 101 (since 30 has sum 3, 40→4, etc., and 101 is the first with sum 2). The function should handle all numbers up to 10^9 efficiently.
#include <cassert>
#include <string>

int main() {
    assert(nextSameDigitSum(20) == 101);
    assert(nextSameDigitSum(1) == 10);
    assert(nextSameDigitSum(9) == -1);
    assert(nextSameDigitSum(19) == 28);
    assert(nextSameDigitSum(99) == -1);
    assert(nextSameDigitSum(100) == 1000);
    assert(nextSameDigitSum(123) == 132);
    assert(nextSameDigitSum(555) == 564);
    assert(nextSameDigitSum(999) == -1);
    assert(nextSameDigitSum(1000000000) == 10000000000LL); // careful: 10^10 fits in long long
}
#include<bits/stdc++.h>

// Returns the smallest integer > n with the same digit sum as n, or -1 if none exists.
long long nextSameDigitSum(long long n) {
    // Convert n to string for digit manipulation.
    std::string digits = std::to_string(n);
    int len = digits.size();
    int total_sum = 0;
    for (char c : digits) total_sum += c - '0';

    // Try increasing digits from rightmost (least significant) to left.
    for (int i = len - 1; i >= 0; --i) {
        int original = digits[i] - '0';
        // Remove current digit's contribution from prefix sum (digits left of i).
        int prefix_sum = 0;
        for (int j = 0; j < i; ++j) prefix_sum += digits[j] - '0';

        for (int d = original + 1; d <= 9; ++d) {
            int new_prefix = prefix_sum + d;
            int rem = total_sum - new_prefix;
            if (rem < 0) continue;
            // Positions to the right of i: (len - 1 - i) positions.
            int right_cnt = len - 1 - i;
            if (rem > 9 * right_cnt) continue;

            // Build the result: keep prefix (digits[0..i-1]), set digit i to d, then fill right minimally.
            std::string result = digits.substr(0, i);
            result += char('0' + d);
            // Fill the right part with the smallest arrangement for sum rem in right_cnt digits.
            if (right_cnt > 0) {
                int nines = rem / 9;
                int remainder = rem % 9;
                // Positions: (right_cnt - nines - (remainder>0?1:0)) zeros, then remainder (if >0), then nines.
                int zeros = right_cnt - nines - (remainder > 0 ? 1 : 0);
                for (int j = 0; j < zeros; ++j) result += '0';
                if (remainder > 0) result += char('0' + remainder);
                for (int j = 0; j < nines; ++j) result += '9';
            }
            // Parse and return.
            return std::stoll(result);
        }
    }
    return -1;
}
// The key insight is to work from the decimal representation. The digit sum of `n` must be preserved in a larger number. To get the smallest larger number with the same digit sum, we need to find the rightmost digit that can be incremented without making the total sum exceed the target, then fill the digits to the right with the smallest possible arrangement (lowest digits first, but ensuring the number is larger). More concretely: iterate from the least significant digit (rightmost) to the left. At each position `i`, try to increase the digit at `i` to some value `d` (where `d` > current digit and `d` ≤ 9) such that the remaining sum (target sum minus the new prefix sum) can be distributed among the positions to the right. The remaining positions must be filled with digits in ascending order (smallest at the rightmost? Actually, to minimize the number, we want the largest digits as far right as possible? No—to minimize the number, after the increment, we want the smallest possible digits to the right, but the overall number increases because the incremented digit is more significant than any changes to the right. So after incrementing at position `i`, we set all digits to the right to be as small as possible: start with 0s, but we must distribute the remaining sum `rem`. The smallest arrangement of digits with sum `rem` in `k` positions is to put as many 9s as possible at the far right (least significant positions) and the remainder in the next position? Actually, to minimize the numeric value, you want the smaller digits in more significant positions (left of the suffix). So fill from left to right with smallest possible digits while ensuring the remaining sum can be achieved. The optimal is to put `0`s and then the remainder at the end, but careful: you have exactly `k` digits. The minimum number with a fixed sum is obtained by putting the remainder (which is ≤ 9) in the most significant of those `k` positions, and all zeros after? No, that would be like 10...0 but sum is remainder. For example, sum=1 in 3 digits: 001 is 1, but as a number, you cannot have leading zeros, but here these are middle digits, so you can have zeros. To minimize the number, you want the most significant digit of the suffix to be as small as possible, then the next, etc. So you fill from left to right: put `min(9, rem)`? That would put a large digit in the most significant position, making the number larger. Better: put as many zeros as possible, but if `rem > 0`, you must eventually put the remainder. The minimal arrangement is to put `rem` in the least significant position and all preceding zeros? For example, rem=5, k=3: 005 gives number 5, but 050 gives 50, so 005 is smaller. So indeed, put all zeros except the last digit which gets `rem`. But `rem` could be >9? No, because each time you try, you ensure `rem ≤ 9*k`? Actually, `rem` could be large, but you can put multiple digits. The minimal number with sum `rem` in `k` digits (allowing leading zeros) is to put the remainder modulo 9 as the first digit? Let's think: you want the number as small as possible, so you want the most significant digit (leftmost of the suffix) as small as possible. So you set it to 0 if possible, but you need to distribute `rem` across `k` digits. The minimal number is achieved by putting `floor(rem/9)` 9s at the far right, and the remainder in the next digit to the left? Actually, if you have k=3, rem=11, then you need digits summing to 11. Options: 029 (29) vs 119 (119) vs 209 (209). 029 is smallest. So pattern: put as many 9s as possible at the rightmost positions, and the remainder (0-8) immediately to the left of those 9s, and all leading positions are 0. So suffix = (zeros) + remainder + (9s). But remainder could be 0, then just 9s. So algorithm: Given `k` positions and a required sum `rem`, the minimal number (as a string of length k, allowing leading zeros) is: let q = rem/9, r = rem%9. Then if r>0, put (k-q-1) zeros, then digit r, then q nines. If r==0, put (k-q) zeros then q nines. However, this may cause leading zeros in the suffix, but that's fine because it's embedded in the number. Then we just need to check if we can increase digit at position i and still have `rem` non-negative and fillable. We must ensure that the resulting number is larger: since we increase a more significant digit (position i) and then arrange the right part minimal, the entire number becomes larger. So we scan from rightmost digit (least significant) to leftmost. For each position i (0-indexed from right), we have current prefix (digits left of i) and we try to set the digit at i to any value `d` greater than current digit, up to 9. Compute new prefix sum = sum of digits left of i plus d. Then rem = total_sum - new_prefix_sum. We need rem ≥ 0 and also rem can be distributed among the i digits to the right (positions 0..i-1). Since each digit can be at most 9, we need rem ≤ 9*i. Also, rem must be achievable: any rem from 0 to 9*i is achievable? Yes, because you can represent any sum from 0 to 9*i with i digits (each 0-9) – this is true for i≥1? For i=0 (no digits to right), then rem must be 0. So condition: rem ≥ 0 and rem ≤ 9*i. Additionally, we need the digit d to be at most 9, and we need to ensure that after filling the right part with the minimal arrangement, the entire number is strictly greater than n. Since we are increasing a more significant digit, it is automatically greater regardless of the right suffix. However, we must be careful: if we increase the most significant digit (leftmost), it is definitely larger. For positions further right, increasing a less significant digit might not always produce a number greater if the left part is same and the right part becomes smaller? But we increase the digit at i, and the right part is arranged to be as small as possible, but the digit at i is larger than original, so the number is larger. For example, n=199, total sum=19. Try from right: i=0 (units), current digit 9, cannot increase. i=1 (tens), current digit 9, cannot increase. i=2 (hundreds), current digit 1, try d=2: prefix sum = 0? Actually left of i is none, prefix sum becomes 2, rem=17, i=2 positions to right, max 18, 17≤18, fill minimal with 2 digits sum 17: q=1,r=8 => suffix = 8 then 9? sequence: put (2-1-1)=0 zeros, digit8, then one 9 => "89", so number 289. Indeed 289 > 199 and sum 19. Good. Edge case: n=9, sum=9, only one digit, cannot increase, so return -1. For n=99, sum=18, similar. Time complexity: at most 10 digits (since n ≤ 1e9, but actually up to 10^9 has 10 digits? 1,000,000,000 has 10 digits, sum 1. But constraints say n ≤ 1e9, so at most 10 digits). For each position, we try up to 9 values, so O(10*9) = O(1). Space O(1) aside from string conversion.
