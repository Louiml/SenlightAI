/*
Write a C++ function `trainJourneyTime` that takes two positive integers `n` and `m`, representing the seat numbers of two passengers on a train with 10 seats per row (seats numbered sequentially starting from 1). The function should return the absolute difference in row numbers between the two passengers. Specifically, row 1 contains seats 1–10, row 2 contains seats 11–20, and so on. The function must compute the row number for each seat using integer arithmetic (avoiding floating point), then return the absolute difference. Assume inputs are positive integers (≥1) and within the range of a 32-bit signed integer.
*/
#include <cstdlib> // for std::abs

// Compute the absolute row difference between two seat numbers on a train with 10 seats per row.
// Seat numbers are 1-indexed; row 1 = seats 1-10, row 2 = seats 11-20, etc.
int trainJourneyTime(int n, int m) {
    // Compute row numbers directly using integer division after decrementing by 1.
    int rowN = (n - 1) / 10 + 1;
    int rowM = (m - 1) / 10 + 1;
    return std::abs(rowN - rowM);
}
#include <cassert>

int main() {
    // Same row
    assert(trainJourneyTime(1, 10) == 0);
    assert(trainJourneyTime(5, 5) == 0);
    
    // Adjacent rows (boundary cases)
    assert(trainJourneyTime(10, 11) == 1);
    assert(trainJourneyTime(20, 21) == 1);
    
    // Different rows
    assert(trainJourneyTime(1, 11) == 1);
    assert(trainJourneyTime(1, 21) == 2);
    assert(trainJourneyTime(11, 1) == 1);
    
    // Larger gaps
    assert(trainJourneyTime(1, 100) == 10);
    assert(trainJourneyTime(95, 5) == 9);
    
    // High numbers
    assert(trainJourneyTime(1000000000, 1) == 99999999);
    assert(trainJourneyTime(123456789, 987654321) == 86419753);
    
    return 0;
}
// The core challenge is mapping a seat number to its row. Since each row has exactly 10 seats, the row number for seat `s` is `((s - 1) / 10) + 1`. This formula works because for `s = 1`, we get `(0)/10 + 1 = 1`; for `s = 10`, we get `(9)/10 + 1 = 1`; for `s = 11`, we get `(10)/10 + 1 = 2`. This avoids off-by-one errors and handles boundary seats correctly. After computing the row for both seats, the answer is `abs(row_n - row_m)`. Edge cases include both passengers in the same row (difference 0), and seats at row boundaries (e.g., 10 and 11). Time complexity is O(1) because only constant arithmetic operations are performed. Space complexity is O(1) as no extra storage is used.
