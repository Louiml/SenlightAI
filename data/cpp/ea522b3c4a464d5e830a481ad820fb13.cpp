Write a C++ function `std::vector<long long> solveMaximaTriplet(std::vector<long long> a)` that takes a vector of exactly three integers (each between 1 and 10^9) representing three provided side lengths. The function must determine whether it is possible to arrange these three numbers into a valid geometric configuration as follows: you may permute the three numbers arbitrarily, but you must output three numbers where at least two of them are equal to the maximum value among the inputs. More precisely, among the three output numbers, the frequency of the maximum input value must be at least 2. If this is impossible, return an empty vector. If it is possible, return any valid output triplet: if all three inputs are equal, return all three equal to that value; if exactly two inputs are equal and that value is the maximum, return that maximum repeated once and the third (smaller) value repeated twice; otherwise (i.e., the maximum appears only once) return an empty vector. The function must handle the logic exactly as the provided snippet does, including treating the input order as given without sorting beforehand.

The key observation is that the only way to output a valid triplet is if the maximum value appears at least twice among the three input numbers. If the maximum is unique (appears exactly once), no arrangement can make at least two numbers equal to the maximum, so the result is an empty vector (meaning "NO" in the original problem). If the maximum appears exactly twice, then the remaining number (which is strictly smaller) must be output twice alongside the maximum once, because we need exactly two copies of the maximum and one copy of the smaller. If the maximum appears three times (all numbers equal), output all three as that value. There is no need to sort or permute further because the reversed input in the original code is irrelevant to the logical condition; the condition depends only on the frequency of the maximum. Edge cases: all three equal (valid), two equal and that equal value is the maximum (valid), two equal but that equal value is not the maximum (actually then the maximum appears once, so invalid), and all distinct (maximum appears once, invalid). Time complexity is O(1) because we only scan the three elements a few times. Space complexity is O(1) besides the output vector.

#include <vector>
#include <algorithm>

// Given a vector of exactly three integers, return a valid output triplet
// where the maximum value appears at least twice, or an empty vector if impossible.
std::vector<long long> solveMaximaTriplet(const std::vector<long long>& a) {
    if (a.size() != 3) {
        return {};
    }
    long long maxVal = *std::max_element(a.begin(), a.end());
    int cntMax = 0;
    for (long long x : a) {
        if (x == maxVal) {
            ++cntMax;
        }
    }
    if (cntMax < 2) {
        return {};
    }
    if (cntMax == 3) {
        return {maxVal, maxVal, maxVal};
    }
    // cntMax == 2, find the non-maximum value
    long long other = 0;
    for (long long x : a) {
        if (x != maxVal) {
            other = x;
            break;
        }
    }
    return {maxVal, other, other};
}

#include <cassert>
#include <vector>

// The solution function is declared above. We include it here for completeness in the test.
std::vector<long long> solveMaximaTriplet(const std::vector<long long>& a) {
    if (a.size() != 3) return {};
    long long maxVal = *std::max_element(a.begin(), a.end());
    int cntMax = 0;
    for (long long x : a) if (x == maxVal) ++cntMax;
    if (cntMax < 2) return {};
    if (cntMax == 3) return {maxVal, maxVal, maxVal};
    long long other = 0;
    for (long long x : a) if (x != maxVal) { other = x; break; }
    return {maxVal, other, other};
}

int main() {
    // All equal -> all three max
    assert(solveMaximaTriplet({5, 5, 5}) == std::vector<long long>({5, 5, 5}));
    // Two equal max, one smaller -> valid
    assert(solveMaximaTriplet({7, 7, 3}) == std::vector<long long>({7, 3, 3}));
    // Same but different ordering of input
    assert(solveMaximaTriplet({3, 7, 7}) == std::vector<long long>({7, 3, 3}));
    // Maximum unique -> invalid
    assert(solveMaximaTriplet({1, 2, 3}).empty());
    // Two equal but not max -> invalid (max appears once)
    assert(solveMaximaTriplet({4, 4, 9}).empty());
    // Negative values allowed? Task says positive, but we test anyway for robustness
    assert(solveMaximaTriplet({-1, -1, -2}) == std::vector<long long>({-1, -2, -2}));
    // Edge: zero and negative
    assert(solveMaximaTriplet({0, 0, -1}) == std::vector<long long>({0, -1, -1}));
    // Maximum appears twice but input order reversed
    assert(solveMaximaTriplet({10, 1, 10}) == std::vector<long long>({10, 1, 1}));
    // Large values
    assert(solveMaximaTriplet({1000000000, 1000000000, 1}) == std::vector<long long>({1000000000, 1, 1}));
    return 0;
}
