// Given a positive integer `n` and a string `s` of length `n` consisting only of decimal digits `'0'`–`'9'`, write a C++ function `oddEvenLargestNumber` that returns the largest possible even number that can be formed by deleting zero or more digits from `s` (without reordering the remaining digits), where the resulting number must have no leading zeros. If no such even number can be formed, return the string `"-1"`. The input is guaranteed to be non-empty, but the string may contain any digits. The function should preserve the relative order of the remaining digits as they appeared in the original string. For example, from `"1234"` the function returns `"1234"` (even and largest), from `"13579"` it returns `"-1"` (all odd digits, no even number possible), from `"102"` it returns `"102"`, and from `"1001"` it returns `"10"` (even, no leading zero, largest possible). The function should handle the case where deleting all digits might be necessary but a leading zero is not allowed, so a result like `"0"` is valid only if the original string contains a `'0'` and no other digits are kept.
The problem is a greedy removal problem on a digit string. We need to find the longest (and lexicographically largest, since digits are compared left-to-right) subsequence that is an even number without a leading zero. The first observation is that any even number must end with an even digit (`0,2,4,6,8`). Therefore, we must select some even digit as the last character of the result. To maximize the value, we want the longest possible sequence that ends with that even digit, but we also need to avoid leading zeros (unless the whole number is `0`). 

The greedy strategy is: we want to keep as many leading digits as possible while ensuring the final digit is even. Start from the left and scan right; if we keep a prefix, we must eventually append an even digit at the end. To maximize the length while preserving order, the best even digit to end with is the rightmost even digit that appears after the kept prefix. However, we also need to ensure no leading zeros. So the algorithm is:
1. Find all even digits in `s`. If there are none, return `"-1"`.
2. We need to choose a suffix that ends with an even digit. The simplest approach: iterate from the left, and for each position `i`, consider removing all characters before `i` that are leading zeros. But a more robust approach is:
   - If the string already ends with an even digit and has no leading zero (or is exactly `"0"`), that is the largest possible because we wouldn't want to delete anything (deleting only reduces the number). So return `s` as is if `s[0] != '0'` and `s.back()` is even.
   - If the string ends with an even digit but has a leading zero (e.g., `"002"`), we must remove leading zeros to avoid a leading zero. The largest such number is obtained by removing all leading zeros but keeping all other digits. So return the substring starting from the first non-zero digit, which will end with the same even digit.
   - If the string ends with an odd digit, we need to delete one or more digits to make the last digit even. We want to delete as few digits as possible (to keep the number as long as possible) and also avoid leading zeros. The optimal strategy: scan from the right to find the first even digit to be the new last digit. Then we need to remove all digits after that even digit, and also possibly remove leading zeros before it. But we must ensure the resulting string does not start with zero. If the digit before the chosen even digit (or the chosen even digit itself) is not zero and there is at least one non-zero digit before it, we keep the prefix up to that even digit after removing trailing odd digits. If the only non-zero digits are after leading zeros, we need to remove the leading zeros. 
   Simpler refined algorithm: 
   - Let `last_even_pos` be the index of the rightmost even digit in `s`. If none, return `"-1"`.
   - Consider all possible positions `j` such that `s[j]` is even and `j >= last_even_pos` (but since we want the largest number, we should take the rightmost even digit as the last digit, because keeping more digits on the right gives a longer number, and any digit after an even digit would be either even (we could end there) or odd (we must delete it). So the optimal last digit is the rightmost even digit. Proof: If we end earlier than the rightmost even digit, we would have deleted at least one digit that is to the right of our last digit. To maximize length, we want the last digit as far right as possible, provided it is even. So we pick `j = last_even_pos`.
   - Now, we need to take a prefix of `s` that ends at `j` and delete some digits to avoid a leading zero. Specifically, we can delete any leading zeros. If after deleting leading zeros the resulting string starts with a non-zero digit (or is empty except for the zero itself), we are good. But if the substring `s[0..j]` contains only zeros, then the only possible number is `"0"`. 
   - Actually, the result is the substring `s[0..j]` with all leading zeros removed (i.e., find the first non-zero character in `s[0..j]`; if found, take from there to `j`; if not, the result is `"0"` if `s[j]=='0'` and no other non-zero digit exists in `s[0..j]`, otherwise `"-1"`). 
   - But wait: what if there are non-zero digits before `j` but also leading zeros? For example `s = "001234"`, `j=5` (last even digit '4'), substring `"001234"` after removing leading zeros gives `"1234"`, which is correct. 
   - What if the rightmost even digit is zero but there is a non-zero digit before it? For example `s = "120"`, `j=2`, substring `"120"` after removing leading zeros (none) gives `"120"` which is fine. 
   - What if the rightmost even digit is odd? Not possible. 
   - What if the rightmost even digit is zero and all digits before it are zeros? Example `s = "0000"`, `j=3`, substring all zeros, result is `"0"`. That is correct because the largest even number is `"0"`. 
   What about cases where we might want to delete some non-leading digits to get a larger number? Since we are only deleting digits, the largest number is the longest subsequence, and among subsequences of the same length, the lexicographically largest. But if the original string already ends with even and no leading zero, it is the largest (length = n). If it has a leading zero, we must remove leading zeros, which reduces length but is unavoidable. If it ends with odd, we must delete trailing odd digits until we hit an even, and we also may need to delete leading zeros. Deleting any other digits would only reduce length, so not helpful. So the greedy above is optimal. 
   Edge cases: if after removing leading zeros the string starts with '0' but has length > 1? That cannot happen because we remove all leading zeros, so the first character is non-zero unless the entire substring is zeros. If the entire substring is zeros, result is `"0"` (which is valid). 
   Complexity: we scan the string once to find the rightmost even digit, then scan from left to find first non-zero up to that index. O(n) time, O(1) extra space excluding result string.
#include <string>
#include <algorithm>

// Returns the largest possible even number obtainable by deleting digits from s.
// Preserves digit order. No leading zeros allowed. Returns "-1" if impossible.
std::string oddEvenLargestNumber(const std::string& s) {
    // Find rightmost even digit position.
    int last_even_pos = -1;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        if ((s[i] - '0') % 2 == 0) {
            last_even_pos = i;
            break;
        }
    }
    if (last_even_pos == -1) {
        return "-1";
    }

    // Find the first non-zero digit in the prefix [0..last_even_pos].
    int first_non_zero = -1;
    for (int i = 0; i <= last_even_pos; ++i) {
        if (s[i] != '0') {
            first_non_zero = i;
            break;
        }
    }

    // If there is no non-zero digit before (or at) the last even digit,
    // the only possible number is "0" (since the last even digit must be '0').
    if (first_non_zero == -1) {
        return "0";
    }

    // Build result from first_non_zero to last_even_pos inclusive.
    return s.substr(first_non_zero, last_even_pos - first_non_zero + 1);
}
#include <cassert>
#include <string>
#include "solution.h"  // assume the solution is in this header, or paste above

int main() {
    // Basic cases
    assert(oddEvenLargestNumber("1234") == "1234");
    assert(oddEvenLargestNumber("13579") == "-1");
    assert(oddEvenLargestNumber("102") == "102");
    assert(oddEvenLargestNumber("1001") == "10");
    
    // Edge cases
    assert(oddEvenLargestNumber("0") == "0");
    assert(oddEvenLargestNumber("00") == "0");
    assert(oddEvenLargestNumber("0002") == "2");
    assert(oddEvenLargestNumber("2000") == "2000");
    assert(oddEvenLargestNumber("123456") == "123456");
    assert(oddEvenLargestNumber("24680") == "24680");
    
    // More scenarios
    assert(oddEvenLargestNumber("135712") == "135712");
    assert(oddEvenLargestNumber("135713") == "-1");
    assert(oddEvenLargestNumber("0012345") == "1234");
    assert(oddEvenLargestNumber("1020304050") == "1020304050");
    
    // Cases where trailing odd digits are removed
    assert(oddEvenLargestNumber("1234567") == "123456");
    assert(oddEvenLargestNumber("10001") == "1000");
    assert(oddEvenLargestNumber("111110") == "111110");
    
    return 0;
}
