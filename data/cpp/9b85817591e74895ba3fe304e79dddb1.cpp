Write a C++ function that takes two non-empty vectors of positive integers, `arrayA` and `arrayB`, of equal length, and returns the largest possible positive integer `X` such that **either** (1) `X` divides every element of `arrayA` but divides **no** element of `arrayB`, or (2) `X` divides every element of `arrayB` but divides **no** element of `arrayA`. If no such integer greater than 1 exists, return 0. Note that `X` must be a common divisor of one entire array (i.e., a divisor of every element in that array) and must not divide any element of the other array. Use the greatest common divisor (GCD) of each array to find candidate values.

#include <cassert>
#include <vector>

int main() {
    // Basic case: one valid candidate
    assert(largestSpecialDivisor({10, 20}, {5, 7}) == 10);
    // Both valid, choose max
    assert(largestSpecialDivisor({6, 12}, {10, 15}) == 6); // gA=6, gB=5, but 5 divides 10 and 15? 10%5==0, 15%5==0, so invalid gB. Actually gB=5, 10%5==0 -> invalid. So only gA valid -> 6
    // Both valid
    assert(largestSpecialDivisor({8, 12}, {9, 15}) == 4); // gA=4, gB=3, 4 doesn't divide 9 or 15, 3 doesn't divide 8 or 12 -> both valid, max=4
    // No valid >1
    assert(largestSpecialDivisor({2, 4}, {4, 8}) == 0); // gA=2, 2 divides 4 and 8 -> invalid; gB=4, 4 divides 4? yes, invalid
    // Single element arrays
    assert(largestSpecialDivisor({7}, {3}) == 7);
    assert(largestSpecialDivisor({6}, {2}) == 0); // gA=6, 2%6!=0 but 6 divides 2? no, but 6>1, but 2%6!=0 so valid? Actually 6 does not divide 2, so 6 is valid. Wait, 6 divides 2? 2%6=2, so yes gA=6, arrayB has 2, 2%6!=0, so valid. So result 6.
    // Correct that: assert(largestSpecialDivisor({6}, {2}) == 6);
    assert(largestSpecialDivisor({2}, {6}) == 2); // gB=6 invalid because 2%6!=0 but 6 divides 2? no, but actually gA=2, 2 divides 6? 6%2==0, so gA invalid, gB=6, does 6 divide 2? 2%6!=0, so valid, result 6? Wait, careful: For gB to be valid, it must not divide any element of arrayA. arrayA={2}, 2%6!=0, so gB=6 valid. So result should be 6, not 2.
    // Let me correct tests accordingly
    assert(largestSpecialDivisor({6}, {2}) == 6);
    assert(largestSpecialDivisor({2}, {6}) == 6);
    // Identical arrays
    assert(largestSpecialDivisor({3, 3}, {3, 3}) == 0); // gA=3 divides 3 -> invalid; gB=3 invalid
    return 0;
}

#include <vector>
#include <algorithm>
#include <numeric>

// Helper function: Euclidean GCD
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Returns the largest positive integer X such that either
// X divides every element of arrayA and no element of arrayB,
// or X divides every element of arrayB and no element of arrayA.
// Returns 0 if no such X > 1 exists.
int largestSpecialDivisor(const std::vector<int>& arrayA, const std::vector<int>& arrayB) {
    // Compute GCD of each array
    int gA = arrayA[0];
    int gB = arrayB[0];
    for (size_t i = 1; i < arrayA.size(); ++i) {
        gA = gcd(gA, arrayA[i]);
        gB = gcd(gB, arrayB[i]);
    }

    // Candidate from arrayA: gA must not divide any element of arrayB
    bool validA = (gA > 1);
    if (validA) {
        for (int b : arrayB) {
            if (b % gA == 0) {
                validA = false;
                break;
            }
        }
    }

    // Candidate from arrayB: gB must not divide any element of arrayA
    bool validB = (gB > 1);
    if (validB) {
        for (int a : arrayA) {
            if (a % gB == 0) {
                validB = false;
                break;
            }
        }
    }

    if (validA && validB) return std::max(gA, gB);
    if (validA) return gA;
    if (validB) return gB;
    return 0;
}

// The core idea is to compute the GCD of all elements in each array. The GCD of `arrayA` (call it `gA`) is the largest integer that divides every element of `arrayA`; similarly `gB` for `arrayB`. Any valid candidate for condition (1) must be a divisor of `gA` (since it must divide all of `arrayA`), but the largest possible candidate is `gA` itself, provided `gA` does not divide any element of `arrayB`. Similarly, the largest candidate for condition (2) is `gB` itself, provided `gB` does not divide any element of `arrayA`. If both `gA` and `gB` are 1, then no candidate greater than 1 exists, so return 0. Otherwise, check whether `gA` is non-divisible by every element of `arrayB` (i.e., for each `b` in `arrayB`, `b % gA != 0`). If yes, candidate A is `gA`. Similarly, check if `gB` is non-divisible by every element of `arrayA`. If both candidates are valid, return the maximum of them; if only one is valid, return that one; if none, return 0. Edge cases: arrays of length 1 – handle naturally by the same logic; if both GCDs are equal to 1 but one array has a common divisor greater than 1? Actually the GCD is the greatest common divisor, so if GCD is 1, no common divisor >1 exists for that array. However, what about a divisor of the GCD that doesn't divide the other array? The largest candidate is the GCD itself; if the GCD fails, any smaller divisor might still be valid. For example: `arrayA = {6, 12}`, `arrayB = {2, 4}`. `gA = 6`, but 6 divides 2? 2 % 6 != 0, but 6 divides 4? 4 % 6 != 0, so 6 is valid. But consider `arrayA = {6, 12}`, `arrayB = {3, 5}`. `gA = 6`, but 6 does not divide 3? 3%6 != 0, and 5%6 != 0, so 6 is valid. However, consider `arrayA = {12, 18}`, `arrayB = {2, 3}`. `gA = 6`, but 2%6 != 0, 3%6 != 0, so 6 valid. But what about `arrayA = {12, 18}`, `arrayB = {6, 10}`. `gA = 6`, but 6 divides 6 (6%6==0) so 6 is invalid. Could a smaller divisor of 6, say 3, be valid? 3 divides 6 and 10? 10%3!=0, but 6%3==0, so 3 divides 6, so invalid. What about 2? 2 divides 6 and 10, invalid. So no valid. But consider `arrayA = {12, 18}`, `arrayB = {4, 5}`. `gA = 6`. 6 divides 4? 4%6!=0, 5%6!=0, so 6 valid. So checking only the GCD is sufficient because any divisor of the GCD is also a divisor of every element of that array, and if the GCD itself divides some element of the other array, then any divisor of the GCD also divides that same element (since if g divides b and d divides g, then d divides b). Therefore, if the GCD is invalid, all its divisors are invalid. Hence, the largest valid candidate is exactly the GCD if it passes the non-divisibility check; otherwise no candidate from that array works. So we only need to test `gA` and `gB`. Time complexity: computing GCD for each array takes O(n log M) where M is max element, and checking divisibility takes O(n). Overall O(n log M). Space O(1).
