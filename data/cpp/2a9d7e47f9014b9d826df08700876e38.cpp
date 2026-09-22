Write a C++ function `std::string smallestAndLargestNumber(int m, int s)` that takes the number of digits `m` (1 ≤ m ≤ 100) and the desired sum of digits `s` (0 ≤ s ≤ 900), and returns a string containing the smallest and largest `m`-digit numbers (with no leading zeros) whose digits sum to exactly `s`, separated by a single space. If no such numbers exist, return the string `"-1 -1"`. The numbers must be represented as strings (not integers) because they can be very large. The function should be robust to all valid input ranges and handle the edge case where `m` is 1 and `s` is 0 (which produces "0 0").

// The problem reduces to constructing the smallest and largest possible `m`-digit numbers with a given digit sum `s`.  
//
// **Feasibility check:**  
// - An `m`-digit number cannot have sum less than 1 (since the first digit must be at least 1) unless `m` is 1 and `s` is 0 (which is valid as number "0").  
// - The maximum digit sum is `9*m` (all digits are 9). So if `s == 0` and `m > 1`, or if `s > 9*m`, the answer is `"-1 -1"`.  
//
// **Smallest number:**  
// To make the number as small as possible, we want the most significant digits as small as possible. So we initialize an array of `m` digits with all zeros, set the first digit to 1, and reduce `s` by 1 (since that 1 is already used). Then we iterate from the *last* digit (least significant) to the first, filling each with as much of the remaining sum as possible, up to 9. This places the leftover sum in the least significant positions, yielding the minimum numeric value.
//
// **Largest number:**  
// To make the number as large as possible, we want the most significant digits as large as possible. Initialize all digits to 0, set the first digit to 1, reduce `s` by 1, then iterate from the *first* digit to the last, filling each with as much of the remaining sum as possible, up to 9. This places the leftover sum in the most significant positions, yielding the maximum numeric value.
//
// **Edge cases:**  
// - If `m == 1` and `s == 0`, both smallest and largest are "0".  
// - After subtracting 1 for the leading digit, if `s` is negative (which only happens if `s == 0` and `m > 1`), we already handled that in the feasibility check.  
// - The algorithm works for any valid `m` up to 100, so the output string length can be 100 digits, which exceeds 64-bit integers, hence we use strings.
//
// **Time Complexity:** O(m) for each number construction, so O(m) total.  
// **Space Complexity:** O(m) to store the digit array.

#include <string>
#include <vector>

// Returns the smallest and largest m-digit numbers (as strings) whose digit sum is exactly s.
// If no such numbers exist, returns "-1 -1".
std::string smallestAndLargestNumber(int m, int s) {
    // Handle the single-digit zero case specially.
    if (m == 1 && s == 0) {
        return "0 0";
    }
    
    // Invalid sums: either zero with more than one digit, or too large a required sum.
    if (s == 0 || s > 9 * m) {
        return "-1 -1";
    }
    
    // Helper lambda to build a number with a given digit placement order.
    auto buildNumber = [&](bool smallest) -> std::string {
        std::vector<int> digits(m, 0);
        digits[0] = 1; // leading digit must be at least 1
        int remaining = s - 1;
        
        // For smallest, place large digits at the end; for largest, place at the front.
        int start = smallest ? m - 1 : 0;
        int end   = smallest ? 0 : m - 1;
        int step  = smallest ? -1 : 1;
        
        for (int i = start; ; i += step) {
            int capacity = 9 - digits[i];
            int add = (remaining >= capacity) ? capacity : remaining;
            digits[i] += add;
            remaining -= add;
            if (remaining == 0) break;
            if (i == end) break; // safety, should not happen after feasibility check
        }
        
        std::string result;
        result.reserve(m);
        for (int digit : digits) {
            result.push_back(static_cast<char>('0' + digit));
        }
        return result;
    };
    
    std::string smallest = buildNumber(true);
    std::string largest = buildNumber(false);
    
    return smallest + " " + largest;
}

#include <cassert>
#include <string>

// The solution function is declared above (not repeated here).
// Global main for testing.
int main() {
    // Basic valid cases.
    assert(smallestAndLargestNumber(2, 9) == "18 90"); // smallest: 18, largest: 90
    assert(smallestAndLargestNumber(3, 1) == "100 100"); // only 100 works
    assert(smallestAndLargestNumber(3, 27) == "999 999"); // all nines
    assert(smallestAndLargestNumber(1, 5) == "5 5");
    
    // Edge case: m=1, s=0.
    assert(smallestAndLargestNumber(1, 0) == "0 0");
    
    // Invalid sums.
    assert(smallestAndLargestNumber(2, 0) == "-1 -1");
    assert(smallestAndLargestNumber(2, 19) == "-1 -1"); // max sum is 18
    assert(smallestAndLargestNumber(3, 28) == "-1 -1");
    
    // Larger m to check string output length.
    std::string result = smallestAndLargestNumber(100, 900);
    // Both numbers are 100 nines.
    assert(result == std::string(100, '9') + " " + std::string(100, '9'));
    
    // Case with leading digit minimum and remaining sum spread.
    assert(smallestAndLargestNumber(4, 5) == "1004 5000"); // smallest: 1004, largest: 5000
}
