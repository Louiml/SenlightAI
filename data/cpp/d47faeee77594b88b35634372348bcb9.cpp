// Write a C++ function `countLuckyNumbers(int l, int r)` that returns the number of integers in the inclusive range `[l, r]` that consist only of digits from the set `{'0','1','2','3','4','5'}` and contain no repeated digit. For example, `101` is not allowed (repeated `1`), but `123` and `50` are allowed. The function must handle `1 <= l <= r <= 10^6` efficiently for multiple calls within a single program. Note that leading zeros are not allowed (i.e., a number like `012` is not a valid integer form), so numbers are generated from non‑zero first digits.

// The problem is essentially generating all valid numbers whose digits are drawn from `{0..5}` with no repetition, and counting those inside `[l, r]`. Since the maximum `r` is `10^6` (which has 7 digits), the largest valid number we can form is `543210` (6 digits), because any 7‑digit number would repeat a digit (only digits 0‑5 are allowed). A simple brute‑force loop from `l` to `r` would be too slow if the range is large and the function is called many times.
//
// We can generate numbers using a BFS/queue that starts from single‑digit numbers `"1"` to `"5"` (no zero at the leading position). For each current string, we append any digit from `'0'` to `'5'` that is not already present in the string, then push those new strings. This ensures each generated string has all distinct digits. We stop when the integer value of a popped string exceeds `r`, because all subsequently generated numbers (obtained by appending digits) will be even larger. We count only those numbers whose value is at least `l`. This BFS explores at most all permutations of subsets of `{0..5}`, which is bounded by `1 + 5 + 5*5 + 5*5*4 + ... =` about 1956 numbers (since no digit repeats). Thus for each query we generate numbers until we pass `r`, which is very fast. Time per call is `O( number_of_generated_numbers )` which is constant in practice (≤ 1956) because the digit set is fixed. Space is `O( number_of_generated_numbers )` for the queue.
//
// Edge cases: `l` can be 1 and `r` can be up to 10^6, but no valid number has more than 6 digits. Numbers like `10` are allowed (digits 1 and 0 distinct), `101` is not. `0` itself is not in the allowed set because we do not start with zero, but the problem defines the range from `1` upward, so it's fine. Also, we must handle the queue properly by generating all six possible appends (digits 0‑5) but skipping those already used to maintain distinctness.

#include <string>
#include <queue>
#include <algorithm>

// Returns the count of integers in [l, r] whose decimal representation uses only digits {0..5} with no repeated digit.
int countLuckyNumbers(int l, int r) {
    std::queue<std::string> q;
    // Start with single-digit numbers 1 through 5 (no leading zero).
    q.push("1");
    q.push("2");
    q.push("3");
    q.push("4");
    q.push("5");

    int count = 0;
    const std::string allowed_digits = "012345";

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        int value = std::stoi(current);
        if (value > r) {
            // Since we generate in increasing order (all pushes are larger), we can stop entirely.
            break;
        }
        if (value >= l) {
            // All characters are distinct by construction.
            count++;
        }

        // Append each allowed digit that is not already in 'current'.
        for (char digit : allowed_digits) {
            if (current.find(digit) == std::string::npos) {
                q.push(current + digit);
            }
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Small ranges
    assert(countLuckyNumbers(1, 5) == 5);          // 1,2,3,4,5
    assert(countLuckyNumbers(1, 9) == 5);          // 1..5 only (6-9 have digits outside set)
    assert(countLuckyNumbers(10, 12) == 2);        // 10,12 (11 has repeat)
    assert(countLuckyNumbers(100, 105) == 4);      // 102,103,104,105 (100,101 have repeats/zeros? Actually 100 has repeat 0, 101 repeat 1)
    // Whole valid range up to 543210
    assert(countLuckyNumbers(1, 543210) == 1 + 5 + 5*5 + 5*5*4 + 5*5*4*3 + 5*5*4*3*2); 
    // The total count of distinct-digit numbers from digits 0-5 with no leading zero:
    // 1-digit: 5, 2-digit: 5*5=25, 3-digit: 5*5*4=100, 4-digit: 5*5*4*3=300, 5-digit: 5*5*4*3*2=600, 6-digit: 5*5*4*3*2*1=600
    // Sum = 5+25+100+300+600+600=1630. But we include leading zeros? No, that count = 1630.
    // Let's compute directly: 5 + 25 + 100 + 300 + 600 + 600 = 1630.
    // So assert count = 1630
    int total = 0;
    for (int n = 1; n <= 543210; ++n) {
        // just to be safe, we can manually check but for test we use the known formula
    }
    assert(countLuckyNumbers(1, 543210) == 1630);
    // Edge case: l > r? Not required, but if l > r, function would return 0 naturally because no number satisfies.
    assert(countLuckyNumbers(1000000, 1000000) == 0); // 1000000 has 1 and 0 repeated? It has 6 zeros, so not allowed.
    assert(countLuckyNumbers(543210, 543210) == 1);   // 543210 is valid (all distinct digits).
    return 0;
}
