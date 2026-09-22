/*
Write a C++ function `int nextDistinctiveYear(int year)` that takes a positive integer representing a year and returns the smallest year strictly greater than the given year that has all distinct decimal digits (i.e., no digit repeats). For example, given 1987, the next year with all distinct digits is 2013. The input year is guaranteed to be a positive integer, but it may be as large as 10,000,000 (so the answer may need to go beyond 7 digits). Assume the input is within the range where `int` can safely hold the answer.
*/
#include <string>
#include <vector>

// Returns true if the given positive integer has no repeated decimal digits.
bool hasDistinctDigits(int number) {
    std::vector<bool> seen(10, false);
    while (number > 0) {
        int digit = number % 10;
        if (seen[digit]) {
            return false;
        }
        seen[digit] = true;
        number /= 10;
    }
    return true;
}

// Returns the smallest year strictly greater than `year` with all distinct digits.
int nextDistinctiveYear(int year) {
    int candidate = year + 1;
    while (!hasDistinctDigits(candidate)) {
        ++candidate;
    }
    return candidate;
}
#include <cassert>

int main() {
    // Basic example from the problem statement
    assert(nextDistinctiveYear(1987) == 2013);
    // Simple small cases
    assert(nextDistinctiveYear(1) == 2);
    assert(nextDistinctiveYear(9) == 10);       // carries to two digits
    assert(nextDistinctiveYear(10) == 12);      // 11 repeats, 12 is next
    // Years with repeated digits internally
    assert(nextDistinctiveYear(100) == 102);
    assert(nextDistinctiveYear(111) == 120);
    // A year already having distinct digits: next must also have distinct digits
    assert(nextDistinctiveYear(1023) == 1024);
    // Larger gap due to repeating pattern
    assert(nextDistinctiveYear(1999) == 2013);
    // Edge near 10-digit boundary (within int range)
    assert(nextDistinctiveYear(987654321) == 1023456789); // next is 10-digit distinct
    return 0;
}
// The main algorithm is straightforward: start from `year + 1` and check each consecutive integer until one has all distinct digits. To check distinctness, convert the integer to a string (or extract digits directly) and verify that no digit appears more than once. A common efficient method is to use a boolean array or a set of size 10 to mark seen digits while iterating through the number's digits. The loop will always terminate because, in the decimal system, there are infinite numbers with distinct digits (e.g., 1234567890 for 10 digits, and beyond that, numbers with 11+ digits must have repeats by the pigeonhole principle, but the answer is always bounded well below that for the given input range). Edge cases: years ending in 9 cause carries, e.g., 1999 → 2013; numbers like 9876543210 (10 distinct digits) have no larger 10-digit number with all distinct digits, so the next would be 1023456789 (11 digits) — but that's beyond the test range. The time complexity is \(O(k \cdot d)\) where \(k\) is the number of years tested and \(d\) is the number of digits (at most 10 for reasonable inputs); in the worst case, the gap between consecutive "distinct-digit years" can be a few hundred, so this is efficient. Space complexity is \(O(1)\) if using a boolean array, or \(O(d)\) if using a set.
