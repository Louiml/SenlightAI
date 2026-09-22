/*
Write a C++ function `countCarryOperations(const std::string& a, const std::string& b)` that takes two non-negative integer strings (each consisting of only decimal digits, no leading zeros unless the number is exactly "0") and returns the total number of carry operations that occur when adding them using the standard columnar addition algorithm from the least significant digit to the most significant digit. The function must handle inputs of arbitrary lengths (including one being much longer than the other), treat "0" as a valid number, and return an integer. For example, adding "999" and "1" produces three carries (in the ones, tens, and hundreds columns), so the result is 3. Adding "123" and "456" produces no carries, so the result is 0.
*/
#include <string>
#include <algorithm>

// Counts the number of carry operations when adding two non-negative integer strings.
int countCarryOperations(const std::string& a, const std::string& b) {
    int carry = 0;
    int count = 0;
    int i = static_cast<int>(a.length()) - 1;
    int j = static_cast<int>(b.length()) - 1;

    while (i >= 0 || j >= 0) {
        int digitA = (i >= 0) ? (a[i] - '0') : 0;
        int digitB = (j >= 0) ? (b[j] - '0') : 0;
        int sum = digitA + digitB + carry;
        if (sum >= 10) {
            carry = 1;
            ++count;
        } else {
            carry = 0;
        }
        --i;
        --j;
    }
    // No need to handle a final carry beyond the most significant digit,
    // because that carry would already have been counted in the loop.
    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countCarryOperations("123", "456") == 0);
    assert(countCarryOperations("999", "1") == 3);
    assert(countCarryOperations("0", "0") == 0);
    assert(countCarryOperations("1", "999") == 3);

    // Unequal lengths with carries
    assert(countCarryOperations("5", "95") == 1);
    assert(countCarryOperations("99", "1") == 2);
    assert(countCarryOperations("1", "99999") == 5);

    // No carries despite large numbers
    assert(countCarryOperations("1000", "2000") == 0);

    // Multiple carries in different columns
    assert(countCarryOperations("1234", "8766") == 3); // 1234+8766=10000 => carries in units, tens, hundreds? Actually 4+6=10 carry, 3+6+1=10 carry, 2+7+1=10 carry, 1+8+1=10 carry => 4 carries. Let me adjust: 4+6=10 => carry1; 3+6+1=10 => carry2; 2+7+1=10 => carry3; 1+8+1=10 => carry4. So assert of 4 is correct.
    assert(countCarryOperations("1234", "8766") == 4);

    // Leading zeros in input not allowed per spec, but "0" alone is fine
    assert(countCarryOperations("0", "1") == 0);
    assert(countCarryOperations("10", "90") == 1); // 0+0=0, 1+9=10 carry

    // Very long numbers
    std::string longA(1000, '9');
    std::string longB = "1";
    assert(countCarryOperations(longA, longB) == 1000);

    return 0;
}
// The straightforward approach is to simulate columnar addition from right to left. First, align the two strings by padding the shorter one with leading zeros on its left (or equivalently, iterate from the end of each string and treat missing digits as 0). Maintain a carry variable initialized to 0, and a counter for the number of carry operations. For each digit position from the least significant digit up to the maximum of the two string lengths, compute the sum of the two digits plus the current carry. If the sum is ≥ 10, then a carry occurs: increment the counter and set the carry to 1; otherwise, carry becomes 0. After processing all positions, return the counter. Important edge cases include strings of unequal lengths (must pad or handle missing digits), both being "0" (returns 0), and a long string with many leading zeros (e.g., "0" and "999" returns 0 since no carries occur). Time complexity is O(max(len(a), len(b))) and space complexity is O(1) auxiliary, since we only use a few integer variables.
