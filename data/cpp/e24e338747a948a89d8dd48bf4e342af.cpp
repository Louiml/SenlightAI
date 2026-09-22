Implement a C++ function `int averageScore(const std::vector<int>& scores)` that, given a non-empty vector of integer scores, returns the integer arithmetic mean rounded to the nearest integer (with halves rounded up, e.g., 2.5 becomes 3). The function must handle values that may overflow a 32-bit integer when summed, so the calculation should use a 64-bit accumulator to avoid overflow. The input vector will not be empty, but may contain negative, zero, or positive scores, and duplicate values are allowed. Return type is `int`, and the result must be truncated (rounded) to the nearest integer using standard rounding: round half away from zero (e.g., 2.5→3, -2.5→-3).

The solution computes the sum of all scores using a `long long` (64-bit) accumulator to prevent integer overflow when many large values are summed. The average is then obtained by dividing the sum by the number of elements. Since integer division truncates toward zero, we need to implement rounding to the nearest integer, with halves rounded away from zero. A common approach: compute `sum / n` as the truncated quotient, and `sum % n` as the remainder. To decide rounding, compare the absolute value of the remainder with `n/2`. If the remainder's magnitude is greater than or equal to half of `n` (and the remainder is not zero), we adjust the quotient by adding 1 if the sum is positive, or subtracting 1 if negative. For cases where `n` is even, `n/2` is exact; for odd `n`, the half threshold is not an integer, so we compare using `2 * abs(remainder) >= n`, which avoids floating point and precisely handles half cases. For example, sum=5, n=2 => remainder=1, 2*1 >=2 true, so quotient=2+1=3 (round up). sum=-5,n=2 => quotient=-2, remainder=-1, 2*1>=2 => quotient=-3. Edge case: if n=1, remainder=0, quotient=sum, no rounding adjustment. Time complexity is O(n) for summing, space O(1). This approach avoids floating-point precision issues and handles all integer ranges.

#include <vector>
#include <cstdlib> // for std::abs (C++11) or use conditional for older

// Compute the integer average of scores, rounded to nearest, halves away from zero.
int averageScore(const std::vector<int>& scores) {
    long long sum = 0;
    int n = static_cast<int>(scores.size());
    for (int s : scores) {
        sum += s;
    }
    
    // Truncated division (toward zero)
    long long quotient = sum / n;
    long long remainder = sum % n;
    
    // If remainder is zero, no rounding needed
    if (remainder == 0) {
        return static_cast<int>(quotient);
    }
    
    // Determine if we need to round away from zero.
    // Compare |remainder| with n/2 using 2*|remainder| >= n.
    long long abs_rem = remainder < 0 ? -remainder : remainder;
    if (2 * abs_rem >= n) {
        // Round away from zero: if sum positive, add 1; if negative, subtract 1
        if (sum > 0) {
            quotient += 1;
        } else {
            quotient -= 1;
        }
    }
    return static_cast<int>(quotient);
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(averageScore({1, 2, 3, 4}) == 2);  // 10/4=2.5 -> 3
    assert(averageScore({-1, -2, -3, -4}) == -3); // -10/4=-2.5 -> -3
    assert(averageScore({5}) == 5);
    assert(averageScore({-5}) == -5);
    
    // No rounding needed
    assert(averageScore({10, 20, 30}) == 20); // 60/3=20
    assert(averageScore({-10, -20, -30}) == -20);
    
    // Half rounding cases
    assert(averageScore({1, 2}) == 2);   // 3/2=1.5 -> 2
    assert(averageScore({-1, -2}) == -2); // -3/2=-1.5 -> -2
    assert(averageScore({0, 1}) == 1);    // 1/2=0.5 -> 1 (1*2>=2 true)
    assert(averageScore({0, -1}) == -1);  // -1/2=-0.5 -> -1

    // Large values to test overflow (sum > 2^31)
    std::vector<int> large(1000000, 2000000000); // each 2e9, sum = 2e15 fits in long long
    assert(averageScore(large) == 2000000000);   // exact average

    // Mixed positive and negative
    assert(averageScore({-3, 3}) == 0); // 0/2=0
    assert(averageScore({-2, 3}) == 1); // 1/2=0.5 -> 1
    assert(averageScore({2, -3}) == -1); // -1/2=-0.5 -> -1
}
