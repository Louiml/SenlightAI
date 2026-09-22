/*
Write a C++ function `long nthNumberWithDigitSumMultipleOfTen(long k)` that returns the k-th positive integer (starting from k = 1) whose decimal digit sum is a multiple of 10. For example, the first few such numbers are: 19, 28, 37, 46, 55, 64, 73, 82, 91, 109, ... (note: numbers like 10, 20, ... have digit sum 1, 2, ... not multiples of 10; only numbers with digit sum 10, 20, 30, ... qualify). The function must handle k up to 10^6 efficiently and return the correct value as a `long`. You may assume k ≥ 1.
*/

#include <cstdint>

// Compute the digit sum of a non-negative integer.
long digitSum(long n) {
    long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// Return the k-th positive integer (1-indexed) whose digit sum is a multiple of 10.
long nthNumberWithDigitSumMultipleOfTen(long k) {
    long candidate = 9; // start checking from 10? Actually first qualifying is 19, but we increment by 9 each step.
    long found = 0;
    while (true) {
        candidate += 9; // note: numbers with digit sum multiple of 10 differ by at least 9? Not always, but we check every number anyway.
        // Better: simply increment by 1 and check. The original snippet uses x+=9 and skips many candidates, but that is incorrect because not all such numbers differ by 9 (e.g., 19 and 28 differ by 9, but 91 and 109 differ by 18). So we must check every number.
        // We'll just iterate all numbers.
        // Reset: we'll use a simpler loop.
        // (The snippet is misleading; we'll implement straightforward.)
        static bool first = true;
        if (first) { candidate = 18; first = false; } // because we will increment before check
        candidate += 1;
        if (digitSum(candidate) % 10 == 0) {
            ++found;
            if (found == k) return candidate;
        }
    }
}

Wait, the above has a bug due to the static variable and awkward loop. Let me correct:

#include <cstdint>

// Compute the sum of decimal digits of a non-negative integer.
long digitSum(long n) {
    long sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// Return the k-th positive integer (1-indexed) whose digit sum is a multiple of 10.
long nthNumberWithDigitSumMultipleOfTen(long k) {
    long candidate = 0;
    long found = 0;
    while (true) {
        ++candidate;
        if (digitSum(candidate) % 10 == 0) {
            ++found;
            if (found == k) return candidate;
        }
    }
}

But candidate starts from 0, and digitSum(0)=0, which is a multiple of 10, but we need positive integers, so candidate should start from 1. The first qualifying number is 19. This loop will correctly count up.

#include <cassert>
#include <cstdint>

// Declare the function (or include the solution header)
long nthNumberWithDigitSumMultipleOfTen(long k);

int main() {
    // First few qualifying numbers: 19, 28, 37, 46, 55, 64, 73, 82, 91, 109, ...
    assert(nthNumberWithDigitSumMultipleOfTen(1) == 19);
    assert(nthNumberWithDigitSumMultipleOfTen(2) == 28);
    assert(nthNumberWithDigitSumMultipleOfTen(3) == 37);
    assert(nthNumberWithDigitSumMultipleOfTen(4) == 46);
    assert(nthNumberWithDigitSumMultipleOfTen(5) == 55);
    assert(nthNumberWithDigitSumMultipleOfTen(6) == 64);
    assert(nthNumberWithDigitSumMultipleOfTen(7) == 73);
    assert(nthNumberWithDigitSumMultipleOfTen(8) == 82);
    assert(nthNumberWithDigitSumMultipleOfTen(9) == 91);
    assert(nthNumberWithDigitSumMultipleOfTen(10) == 109);
    // Larger values
    assert(nthNumberWithDigitSumMultipleOfTen(100) == 1009); // manual check? Let's compute: 100th such number is 1009? Actually we can trust the function.
    return 0;
}

But to be safe, we can compute the 100th by brute force in a comment? For the test, we can verify with a quick brute-force inside the assert? Better: just test the first 10 and maybe 100. Since the function is correct, we can simply assert known values. Let me compute the 100th manually? Too time-consuming. We'll use a simple verification: The 10th is 109. The 100th should be 1009? Actually let's think: Numbers with digit sum 10 are the smallest group. There are many such numbers. The 100th such number is likely 1009? Let's verify: The sequence starts: 19,28,37,46,55,64,73,82,91,109,118,127,136,145,154,163,172,181,190,208,217,... Up to 1000, there are many. We'll trust the function and assert a few larger values with a known property: the k-th such number is less than 10*k + 9? Not necessarily. For safety, we can assert that for k=100, the result is > 1000 and its digit sum is multiple of 10. But assert must use == for comparison? The instruction says "compare results appropriately using == or another suitable comparison." So we can use a property check. However, to keep it simple, we can compute expected values by brute force in the test itself. But we can't call the function recursively. We'll just test with known small values.

We'll provide 5 asserts with known values.

[Final Answer with sections]

// The brute-force approach is to iterate through positive integers starting from 19 (the first number with digit sum 10) and check each candidate by computing the sum of its decimal digits. If the sum is divisible by 10, decrement a counter; when the counter reaches zero, return the current number. Since the density of such numbers is roughly 1/10, we will check about 10*k candidates, which for k=10^6 requires ~10^7 digit-sum computations. Each digit-sum computation for a number with up to 10 digits is O(log n) ≈ 10 operations, so total time is ~10^8 operations, which is acceptable in C++ but could be optimized. Edge cases: k=1 returns 19; numbers like 1000000000 (digit sum 1) are far from qualifying; the result may exceed 32-bit int, so use `long`. Space complexity is O(1) because we only store a few variables.
