/*
Write a C++ function named `convertChineseNumber` that takes a non-negative integer represented as a string (e.g., `"191274399021"`) and returns its Chinese-style reading as a string. The output format must follow the traditional Chinese numeral system with units `Q` (千), `B` (百), `S` (十), and section markers `W` (万), `Y` (亿), with lowercase `q` (also 万? Actually from code: `ord[4] = {'q', '?', 'W', 'Y'}` — note `'?'` is unused; but in output for `191274399021`, we get `1Q9B1S2W7Q4B3S9W...`? Let's clarify from snippet: The code uses `ord[1] = '?'` only when `o=2`? Wait `o` starts at 1, increments after processing each 4-digit group. For the first group (units), no marker is added. For second group (万), `ord[2]` = `'W'`? Actually `ord[0]='q'`, `ord[1]='?'`, `ord[2]='W'`, `ord[3]='Y'`. The code writes `res = ord[o] + res` where `o` is the marker index. For first group `o=1`? Let's trace: While loop: `o=1`, process first group, then `++o` becomes 2, then for next iteration `ord[2]` is 'W'? Yes. So after first group, for each subsequent higher section, we prepend header from `ord[o]` where `o` is 2,3,4... That means for 10000^2 (亿), use `ord[3]='Y'`, for 10000^1 use `ord[2]='W'`. The code also has `ord[0]='q'` but never used? Actually `o` starts at 1, then increments to 2, then 3, then 4... but `ord` length is 4, indices 0-3. For `o=4` out of bounds? But `base` max for typical 64-bit is 9e18, that's 5 groups? Actually 10000^4 = 1e16, so up to 5 groups? But `ord` only 4 elements, so if more than 4 groups, out-of-bounds. In the given snippet, input `"191274399021"` has 12 digits → 3 groups (万, 亿? Actually 12 digits → 3 groups of 4: units, 万, 亿). The output would be: For units 399021? Wait let's compute: base=191274399021. Group1: 9021? Actually last 4 digits: 9021 → makepart(9021) → 9Q 0? S? 2S 1? That gives "9Q2S1" but code also skips zero units? `if(q!=0)`, `if(b!=0)`, `if(s!=0)`, `if(g!=0)`. So 9021 → 9Q, b=0 skip, s=2 → 2S, g=1 → 1 → "9Q2S1". Then `o=2`, res = ord[2] + res = 'W' + "9Q2S1"? Wait order: In loop, `res = ord[o] + res` before `makepart`? The code does: `if (o != 1) res = ord[o] + res; ++o; res = makepart(part) + res;` Actually order: check `if(o!=1)` then `++o` then `res = makepart(part) + res`. Wait the snippet:
```cpp
while (base)
{
    part = base % 10000;
    base /= 10000;
    if (o != 1)
        res = ord[o] + res;
    ++o;
    res = makepart(part) + res;
}
```
So first iteration o=1, no marker. After processing, res = makepart(part1). Then ++o → o=2. Next iteration, part = next higher group, then if(o!=1) res = ord[2] + res → 'W' + current res (which is lower group). Then ++o → 3. Then res = makepart(part2) + res → higher group's part + 'W' + lower group. So final string is from highest to lowest: highest group's makepart, then marker for next lower? Actually after processing all, res contains highest group first? Let's simulate: Suppose two groups: lower L, higher H. First iter: res = makepart(L). o becomes 2. Second iter: part=H, res = ord[2] + res = 'W' + makepart(L), then ++o, then res = makepart(H) + res → makepart(H) + 'W' + makepart(L). So output is higher group, then 'W', then lower group. Correct. For three groups: H,M,L: After processing L, then M (with 'W'), then H (with 'Y'? ord[3]='Y'). So final: H + 'Y' + M + 'W' + L. So for 191274399021: groups: 1912? Wait split from right: 399021? Actually 191274399021 has digits: 1912 7439 9021? Let's split: 191274399021 → from right: 9021 (units), 7439 (万), 1912 (亿). So output should be: 1Q9B1S2 + Y + 7Q4B3S9 + W + 9Q2S1? Note: In makepart for 7439: 7Q4B3S9, and 1912: 1Q9B1S2. So final: "1Q9B1S2Y7Q4B3S9W9Q2S1". But the snippet's output would include that, but also note the code prints `cvttoint` separately. For zero handling: if a group is 0 (e.g., 10000 → groups: 1 and 0), the code would skip the zero group? Let's see: while(base) stops when base becomes 0 after dividing. So if middle group is zero, e.g., 100000001 → groups: 1 (亿), 0 (万), 1 (units). Processing: first L=1, then M=0, then H=1. For M=0, base was 100000001, after first division base=10000, part=1? Wait let's compute: base=100000001. First iter: part=1 (units), base=10000. Then o=2. Second iter: part=10000? Actually base=10000, part=10000%10000=0, base=1. Then if(o!=1) res = ord[2] + res → 'W' + "1". ++o=3. Then res = makepart(0) + res → "" + "W1" → "W1". Third iter: part=1, base=0. if(o!=1) res = ord[3] + res → 'Y' + "W1". ++o=4. res = makepart(1) + res → "1Y W1"? Actually makepart(1) returns "1". So res = "1" + "YW1" = "1YW1". That is correct: 1亿零1万? Actually in Chinese convention, 100000001 is "一亿零一" not "一亿零一万"? Standard: 100000001 = 一亿零一 (no 万). But the code would produce "1YW1" which reads 1亿 1万? That's incorrect. So the snippet's method has a bug for zero groups. The task should specify that zero digits within a group are simply omitted, and if an entire group is zero, it should be omitted entirely without a marker. For simplicity, the task can state: "If a 4-digit section is all zeros, skip it entirely (do not include its marker). For a non-zero section, represent each non-zero digit with its unit (Q, B, S) and the digit itself, omitting zero digits. Do not use any 'zero' character; just omit." Also note that for numbers like 10, the output should be "10" or "1S"? Actually 10 → groups: 10 → makepart(10) → s=1, g=0 → "1S". But Chinese for 10 is "十" not "一十"? Commonly "十" for 10, but in this style it's "1S". The snippet uses digit + unit, so 10 -> "1S". The task can define that.

To make the task self-contained, we'll specify: Convert a string of digits (no leading zeros except "0" itself) to Chinese-style representation. Group digits into sections of four from the right. For each section, from highest to lowest, append the section's representation (digits with units Q, B, S for thousands, hundreds, tens respectively; units digit just the digit) and then a section marker: for the 2nd lowest group (10^4) append 'W', for the 3rd (10^8) append 'Y', for higher groups, append 'Y' repeatedly? Actually the given code only handles up to Y (亿). To keep it simple, we'll restrict to numbers up to 10^12 (trillions) which can be covered with W and Y. The task can specify: "The input string will represent an integer from 0 to 999,999,999,999 (inclusive). For zero, return an empty string or "0"? The snippet returns empty for 0 because makepart(0) = "" and while(base) doesn't run. But let's decide: For 0, return "0". Edge case: leading zeros not allowed. For a section like 2001, output "2Q1"? Actually 2001: q=2, b=0,s=0,g=1 → "2Q1". But Chinese would say "二千零一", but this style omits zero. So we follow snippet.

So the function `convertChineseNumber(const std::string& s)` should return a string. Let's write a high-quality solution.
*/
#include <string>
#include <vector>

/**
 * Converts a non-negative integer (given as a decimal string) to a
 * Chinese-style representation using units Q (千), B (百), S (十),
 * and section markers W (万) and Y (亿). Zero digits are omitted,
 * and any completely zero 4-digit section is skipped entirely.
 *
 * The input must not contain leading zeros, except for the string "0".
 * The maximum supported value is 999,999,999,999 (12 digits).
 */
std::string convertChineseNumber(const std::string& s) {
    if (s == "0") {
        return "0";
    }

    // Split into groups of four digits from the right.
    std::vector<std::string> groups;
    int len = s.size();
    for (int i = len; i > 0; i -= 4) {
        int start = (i - 4 > 0) ? i - 4 : 0;
        groups.push_back(s.substr(start, i - start));
    }
    // groups[0] is the rightmost (units) group, groups.back() is highest.

    std::string result;
    const char sectionMarkers[] = {'W', 'Y'};  // for 万 and 亿

    int groupCount = groups.size();
    for (int idx = groupCount - 1; idx >= 0; --idx) {
        const std::string& g = groups[idx];
        // Skip groups that are all zeros.
        bool allZero = true;
        for (char c : g) {
            if (c != '0') {
                allZero = false;
                break;
            }
        }
        if (allZero) {
            continue;
        }

        // Pad group to four digits with leading zeros for ease.
        std::string padded = g;
        while (padded.size() < 4) {
            padded = "0" + padded;
        }

        int q = padded[0] - '0';
        int b = padded[1] - '0';
        int s_ = padded[2] - '0';
        int u = padded[3] - '0';

        if (q != 0) {
            result += char(q + '0');
            result += 'Q';
        }
        if (b != 0) {
            result += char(b + '0');
            result += 'B';
        }
        if (s_ != 0) {
            result += char(s_ + '0');
            result += 'S';
        }
        if (u != 0) {
            result += char(u + '0');
        }

        // Append section marker for non-units groups.
        if (idx != 0) {
            // idx==1 means 万, idx==2 means 亿, idx>=3 not supported.
            if (idx <= 2) {
                result += sectionMarkers[idx - 1];
                // idx=1 -> 'W', idx=2 -> 'Y'
            }
            // For idx>=3, we simply ignore (should not happen per constraints).
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the solution function goes here (or include header).
std::string convertChineseNumber(const std::string& s);

int main() {
    // Basic small numbers
    assert(convertChineseNumber("0") == "0");
    assert(convertChineseNumber("1") == "1");
    assert(convertChineseNumber("10") == "1S");
    assert(convertChineseNumber("100") == "1B");
    assert(convertChineseNumber("1000") == "1Q");
    
    // Compound numbers
    assert(convertChineseNumber("1100") == "1Q1B");
    assert(convertChineseNumber("2001") == "2Q1");
    assert(convertChineseNumber("3040") == "3Q4S");
    
    // 万 group
    assert(convertChineseNumber("10000") == "1W");
    assert(convertChineseNumber("12345") == "1W2Q3B4S5");
    assert(convertChineseNumber("100000000") == "1Y");
    assert(convertChineseNumber("100000001") == "1Y1");
    assert(convertChineseNumber("100010001") == "1Y1W1");
    
    // Full sample from the snippet
    assert(convertChineseNumber("191274399021") == "1Q9B1S2Y7Q4B3S9W9Q2S1");
    
    // Edge case: zero groups interleaved
    assert(convertChineseNumber("100000000000") == "1Y");  // 1000亿? Actually 100,000,000,000 = 1000亿? Wait 10^11, groups: 1000 0000 0000 -> 1Q (亿) + 0 + 0? Let's compute: 100000000000 has 12 digits -> groups: 1000 (亿), 0000 (万), 0000 (units). So output should be "1QY"? Actually highest group 1000 -> "1Q", then idx=2? Wait groups vector: units "0000", 万 "0000", 亿 "1000" -> idx=2 (亿) -> we append marker 'Y' after highest group? In our loop, from highest idx=2: process "1000" -> "1Q", then idx!=0, idx=2 -> append 'Y' -> "1QY". Then next idx=1 (0000) skip, idx=0 (0000) skip. So result "1QY". That is correct: 一千亿. But wait 1000亿 = 1QY? Actually Chinese for 1000亿 is 一千亿, so yes. Let's assert that.
    assert(convertChineseNumber("100000000000") == "1QY");
    assert(convertChineseNumber("100000000001") == "1QY1");  // 1000亿零1
    
    return 0;
}
// The main challenge is to correctly group the input string into chunks of four digits from the right, and for each chunk, produce its Chinese-style representation. The algorithm works as follows:
// 1. Convert the input string to a 64-bit integer (or directly process digits to avoid overflow, but since the limit is 10^12, a 64-bit integer is safe). However, the snippet processes as integer to avoid leading zeros issues. We'll directly use string manipulation to be robust.
// 2. If the integer is 0, return "0".
// 3. Split the string into groups from the right, each of length 4 (the leftmost group may have fewer digits). For example, "191274399021" becomes ["1912","7439","9021"].
// 4. Process groups from highest to lowest. For each group, if the group's integer value is zero, skip it entirely (but be careful: if it's the highest non-zero group, we need to handle; for middle zeros, skip). Then for each non-zero group, generate its representation:
//    - Let `q`, `b`, `s`, `g` be the thousands, hundreds, tens, and ones digits (with leading zeros for shorter group).
//    - Append digit+unit for each non-zero: if q != 0, append char(q+'0') and 'Q'; if b != 0, append char(b+'0') and 'B'; if s != 0, append char(s+'0') and 'S'; if g != 0, append char(g+'0').
//    - After the group (except the units group, i.e., the lowest group), append a section marker based on its position: for the second group from the bottom (10^4), append 'W'; for the third (10^8), append 'Y'; for the fourth (10^12) we can append 'Y' again? But limit is 10^12, so groups: units (10^0), 10^4 (W), 10^8 (Y), 10^12 (we'll use 'Y' again? Actually 10^12 is 万亿, but snippet doesn't handle. To keep within spec, we only allow up to 10^12-1, which is 12 digits -> groups: units, 万, 亿, and a fourth? 10^12 has 13 digits? 999,999,999,999 has 12 digits, so only three groups: units, 万, 亿. So markers: 'W' for second group, 'Y' for third group. So we only need those two. But the snippet had ord[0] unused, but we'll just implement two markers.
// 5. Edge cases: When a higher group is zero but a lower group is non-zero, we just skip the high zero group (no marker). For example, 100000001 -> groups: [1,0,1] (from high to low: 1 (亿), 0 (万), 1 (units)) -> we skip 0 group, so output is "1Y1" (亿 and units). That is correct Chinese for 一亿零一? Usually there is a zero between, but our simplified style omits it. The task should specify that zero sections are omitted without any placeholder.
// 6. For a group like 2001, output "2Q1". For 10 -> "1S". For 100 -> "1B". For 1000 -> "1Q". For 0 as a whole, return "0".
//
// Time complexity: O(n) where n is number of digits in input, as we process each group. Space complexity O(1) excluding output string.
//
// The provided snippet has a bug with `ord` indexing (uses `ord[o]` with o up to 4, but ord length 4 indices 0-3, so for numbers > 10^12 it overflows). Our solution avoids that.
//
// Now write the solution.
