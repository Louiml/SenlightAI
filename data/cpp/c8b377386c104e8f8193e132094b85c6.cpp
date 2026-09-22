Write a C++ function that takes a positive integer `n` as input and returns the smallest integer greater than `n` that has no repeated decimal digits. For example, given `1987`, the function should return `2013` because `1988` repeats 8, `1989` repeats 9, and the sequence continues until `2013` which is the first number after `1987` with all distinct digits. The function must handle inputs up to `100000` (so the result may exceed that bound), and it must be efficient enough to work for all such inputs. You may assume the input is always positive and no larger than `100000`.

#include <cassert>

int main() {
    assert(nextDistinctDigitNumber(1) == 2);
    assert(nextDistinctDigitNumber(9) == 10);
    assert(nextDistinctDigitNumber(10) == 12);
    assert(nextDistinctDigitNumber(99) == 102);
    assert(nextDistinctDigitNumber(1987) == 2013);
    assert(nextDistinctDigitNumber(100000) == 102345);
    assert(nextDistinctDigitNumber(987654321) == 9876543210LL); // note: this fits in long long
    return 0;
}

#include <array>

// Return the smallest integer greater than n with all distinct decimal digits.
long long nextDistinctDigitNumber(long long n) {
    for (long long candidate = n + 1; ; ++candidate) {
        std::array<bool, 10> seen{};
        long long temp = candidate;
        bool distinct = true;
        while (temp > 0) {
            int digit = static_cast<int>(temp % 10);
            if (seen[digit]) {
                distinct = false;
                break;
            }
            seen[digit] = true;
            temp /= 10;
        }
        if (distinct) {
            return candidate;
        }
    }
}

// The algorithm repeatedly increments a candidate number starting from `n+1` until a number with all distinct digits is found. To check distinctness, we can use a fixed-size boolean or integer array of size 10 (for digits 0–9). For each candidate, we extract its digits one by one via modulo 10 and division by 10, marking each digit as seen. If we ever encounter a digit already seen, the number is invalid. When a valid number is found, we return it. Edge cases: numbers like `9` → next is `10` (valid), `99` → `101` (but that has repeated 1, so the correct answer is `102`), and `100000` → the next distinct-digit number is `102345` (since numbers like `100001` repeat 0 and 1, etc.). The worst-case number of candidates is bounded by a small constant because the count of numbers with repeated digits grows, but for the given input limit, a brute-force loop is fine. Time complexity is O(D * k) where D is the number of digits (at most 6) and k is the number of candidates checked (at most around 1000 for worst case near 100000, but typically much less). Space complexity is O(1) because we only use a fixed-size array.
