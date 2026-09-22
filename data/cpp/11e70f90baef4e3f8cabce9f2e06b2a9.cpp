// Write a C++ function named `constructLuckyNumber` that takes a positive integer `n` (the number of digits) and returns a string representing the smallest lucky number with exactly `n` digits. A lucky number is defined as a positive integer that contains only the digits `2`, `3`, and `4`. Additionally, the sum of its digits must be divisible by `3`, and the number must not have any leading zeros (which is automatic since it starts with a non-zero digit). If no such number exists for the given `n`, return `"-1"`. The returned string should be the lexicographically smallest (i.e., smallest numeric value) among all valid lucky numbers with exactly `n` digits. For example, for `n = 1`, no valid number exists (since 2+? single digit 2,3,4 sums are 2,3,4; only 3 is divisible by 3, but 3 is allowed, wait check: single digit 3 has sum 3 divisible by 3, so it is valid, but the original snippet returns -1 for n=1 because it uses a different rule; your task must follow the given definitions). Actually, careful: the original snippet does not match this new definition; you must ignore the snippet's logic and create a new, independent task based on a similar spirit (digit construction with modular condition). So: The number's digit sum must be divisible by 3. For `n = 1`, the only possible digits are 2,3,4; sums: 2 (not divisible), 3 (divisible), 4 (not). So smallest is "3". For `n = 2`, possible numbers like "22" sum 4, "23" sum 5, "24" sum 6 (valid), "32" sum 5, "33" sum 6, "34" sum 7, etc. The smallest valid is "24" because 24 < 33, etc. Return the string representation of that smallest number. If no such number exists (which happens only when `n` is 0? But n is positive; also for n=??? Actually always exists for n>=1 because you can always use digit 3 repeated? For n=2, "33" works, but "24" is smaller). So the task is: given `n`, return the smallest string of length `n` consisting only of characters '2','3','4' such that the sum of digits (converted to integers) is divisible by 3. If `n` is 0 or negative, return "-1". Also, the function must be efficient for `n` up to 10^5, so do not generate all possible numbers. Output code only in the section, and provide test assertions in . Ensure your solution is self-contained with `#include <bits/stdc++.h>` and uses `std::string`. Provide a descriptively named free function.

// The problem reduces to constructing the lexicographically smallest string of length `n` over alphabet {'2','3','4'} whose digit sum modulo 3 is 0. Since lexicographic order for same-length strings corresponds to numeric order, we try to place as many small digits ('2') as possible at the most significant positions, but we must adjust the suffix to make the total sum divisible by 3. Let `sum = 2 * n` initially if we fill all positions with '2'. The remainder `r = (2*n) % 3`. If `r == 0`, the all-'2' string is valid and is the smallest possible. If `r != 0`, we need to change some of the trailing digits to '3' or '4' to fix the remainder. Since changing a '2' to '3' adds 1 modulo 3, and changing to '4' adds 2 modulo 3, we can adjust the last one or two digits:
// - If `r == 1`: we need to add 2 modulo 3. Change the last digit from '2' to '4' (adds 2). So the string is `n-1` copies of '2' followed by '4'. But check: if `n` is small? For any n>=1, this works. However, if `n == 1` and r = 2%3 = 2, not this case. For n=1, r=2, so need add 1 mod 3: change last to '3' -> "3". For r==1 occurs when n%3 == 2 (since 2*n mod 3 = 1). Example n=2: r=4%3=1, change last to '4' -> "24" which is valid and smallest (since "23" sum 5 not divisible, "24" works). For r==2 occurs when n%3 == 1 (2*1=2, 2*4=8 mod 3=2). For n=1, r=2, change last to '3' -> "3". For n=4, r=8%3=2, change last to '3' -> "2223" (sum 2+2+2+3=9 divisible). But is there a smaller? "2224" sum 10 not divisible. So works.
// - For `r == 2`: change the last digit to '3' (adds 1) to make sum divisible. So string is `n-1` copies of '2' followed by '3'.
// - However, consider the case when `n` is large and `r == 1` but we might need to change two digits? No, one change suffices because adding 2 is possible via '4'. Similarly for r==2, adding 1 via '3'. So the construction is: if `(2*n) % 3 == 0`, return string of n '2's; else if `(2*n)%3 == 1`, return string of n-1 '2's + "4"; else (==2), return string of n-1 '2's + "3". But careful: for n=1, n-1=0, so "4" or "3" works. For n=1, r=2, so return "3". Correct. Edge case: n <= 0, return "-1". Also, for n=1, the all-'2' case doesn't happen. Time O(n) to build string, space O(n) for result.

#include <bits/stdc++.h>

// Returns the smallest lucky number (digits only 2,3,4) with exactly n digits
// whose digit sum is divisible by 3. Returns "-1" if n is non-positive.
std::string constructLuckyNumber(int n) {
    if (n <= 0) {
        return "-1";
    }
    int remainder = (2 * n) % 3;
    if (remainder == 0) {
        return std::string(n, '2');
    } else if (remainder == 1) {
        // Need to add 2 modulo 3: change last digit to '4'
        return std::string(n - 1, '2') + '4';
    } else { // remainder == 2
        // Need to add 1 modulo 3: change last digit to '3'
        return std::string(n - 1, '2') + '3';
    }
}

#include <cassert>
#include <string>

// Forward declaration
std::string constructLuckyNumber(int n);

int main() {
    // Basic cases
    assert(constructLuckyNumber(1) == "3");
    assert(constructLuckyNumber(2) == "24");
    assert(constructLuckyNumber(3) == "222"); // sum=6 divisible by 3
    assert(constructLuckyNumber(4) == "2223"); // sum=9 divisible by 3
    assert(constructLuckyNumber(5) == "22224"); // sum=12 divisible by 3
    // Large n
    std::string large = constructLuckyNumber(100000);
    assert(large.size() == 100000);
    assert(std::all_of(large.begin(), large.end(), [](char c){ return c=='2'||c=='3'||c=='4'; }));
    int sum = 0;
    for (char c : large) sum += c - '0';
    assert(sum % 3 == 0);
    // Invalid input
    assert(constructLuckyNumber(0) == "-1");
    assert(constructLuckyNumber(-5) == "-1");
    // Verify lexicographic smallest for n=6 (r=(2*6)%3=0 -> all twos)
    assert(constructLuckyNumber(6) == "222222");
    // n=7 (r=14%3=2 -> change last to '3')
    assert(constructLuckyNumber(7) == "2222223");
    // n=8 (r=16%3=1 -> change last to '4')
    assert(constructLuckyNumber(8) == "22222224");
    return 0;
}
