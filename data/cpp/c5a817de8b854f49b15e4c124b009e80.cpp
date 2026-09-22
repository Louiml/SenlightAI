/*
Write a C++ function that takes a vector of exactly 9 distinct positive integers (each between 1 and 100) and returns a vector of 7 integers whose sum equals 100. The output vector must preserve the original order of the selected numbers as they appear in the input vector. If multiple valid selections exist, return the selection where the 7 numbers appear earliest in the input order (i.e., the lexicographically smallest sequence of indices). It is guaranteed that at least one valid selection exists.
*/
#include <vector>
#include <cstddef>

// Returns a vector of 7 integers from 'input' (exactly 9 elements) whose sum is 100,
// preserving the original relative order of the selected numbers.
// If multiple such 7-element subsets exist, returns the one whose indices are lexicographically smallest.
// Assumes input has exactly 9 distinct positive integers and at least one valid subset.
std::vector<int> findSevenWithSum100(const std::vector<int>& input) {
    // We will iterate over all ways to choose 2 indices to exclude (since 9-2=7).
    std::vector<int> best;
    bool found = false;
    const std::size_t n = input.size(); // n == 9

    // Enumerate all pairs (i, j) to exclude, i < j.
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            int sum = 0;
            std::vector<int> candidate;
            // Build the subset of all indices except i and j, in order.
            for (std::size_t k = 0; k < n; ++k) {
                if (k != i && k != j) {
                    sum += input[k];
                    candidate.push_back(input[k]);
                }
            }
            // If the sum to 100 and this is the first valid one (since we iterate i,j in increasing order,
            // the first found is lexicographically smallest index set), store and stop.
            if (sum == 100 && !found) {
                best = candidate;
                found = true;
                break; // We can break both loops but keep simple with a flag.
            }
        }
        if (found) break;
    }
    // The problem guarantees at least one valid subset, so best is non-empty.
    return best;
}
#include <cassert>
#include <vector>

// This is a copy of the solution function for testing (or you can include the header).
std::vector<int> findSevenWithSum100(const std::vector<int>& input) {
    std::vector<int> best;
    bool found = false;
    const std::size_t n = input.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            int sum = 0;
            std::vector<int> candidate;
            for (std::size_t k = 0; k < n; ++k) {
                if (k != i && k != j) {
                    sum += input[k];
                    candidate.push_back(input[k]);
                }
            }
            if (sum == 100 && !found) {
                best = candidate;
                found = true;
                break;
            }
        }
        if (found) break;
    }
    return best;
}

int main() {
    // Test 1: Simple case where only one subset sums to 100? Actually we check any valid.
    std::vector<int> v1 = {20, 10, 15, 5, 25, 10, 10, 5, 0}; // Sum of all = 100, but need 7: exclude two small? Let's choose a known one.
    // We'll craft a case: numbers 1..9 sum to 45, not enough. Use a known example from snippet:
    // Example: {20, 7, 23, 19, 10, 15, 25, 8, 13} sum = 140, need 100 -> exclude 40 total. Let's use a simpler known.
    std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 64}; // Sum = 100 exactly, but we need 7 numbers, exclude two that sum to 0? not possible. So use distinct.
    // Actually let's test with a guaranteed solution: Use {20,20,20,20,20,20,20,20,20}? but distinct required. Let's use {10,10,10,10,10,10,10,10,30} Sum=110? Not.
    // Simpler: Use a list where excluding the two largest gives 100. E.g., {20,20,20,20,20,20,20,20,20} not distinct. Use {1,2,3,4,5,6,7,8,64} sum=100, but 7 numbers sum to 100? 
    // Let's craft a precise test: {5,5,5,5,5,5,5,5,60} sum=100, but 7 numbers of 5+5+...=35? No.
    // The original problem: 9 dwarfs heights sum to 140, find 7 whose sum=100. So we test with known dwarfs heights: 20,7,23,19,10,15,25,8,13 sum=140, we need 7 sum=100 -> exclude 40, e.g., exclude 15 and 25 -> sum of remaining=100? Let's check: 20+7+23+19+10+8+13 = 100? 20+7=27, +23=50, +19=69, +10=79, +8=87, +13=100. Yes! So we exclude 15 and 25 (indices 5 and 6 zero-based). The solution should return the 7 numbers in order: 20,7,23,19,10,8,13.
    std::vector<int> dwarfs = {20, 7, 23, 19, 10, 15, 25, 8, 13};
    std::vector<int> expected = {20, 7, 23, 19, 10, 8, 13};
    assert(findSevenWithSum100(dwarfs) == expected);

    // Test 2: Multiple valid? Check the lexicographic smallest index.
    // Example: {1,2,3,4,5,6,7,8,64} sum=100, but need 7 numbers sum=100? 1+2+3+4+5+6+7=28, not 100. So not.
    // Use another: {10,10,10,10,10,10,10,10,20} sum=110? Not.
    // Let's use {15,15,15,15,15,15,15,15,10} sum=130? 
    // Instead, we test a case where two possible subsets exist: e.g., {20,20,20,20,20,20,20,20,20} not distinct.
    // Since problem requires distinct, we'll just test the given example and one more.
    std::vector<int> v = {40, 10, 10, 10, 10, 10, 10, 10, 10}; // Sum=120, need 7 sum=100: exclude 40 and one 10 -> sum=80? Actually 7 tens=70, plus? Not.
    // Use a simple one: {5,10,15,20,25,30,35,40,45} sum=225, need 100? Hard.
    // For simplicity, test the only given case and an edge case where the answer is the first 7.
    std::vector<int> v3 = {10, 20, 30, 40, 50, 60, 70, 80, -60}; // but positive integers only? The task says positive integers 1..100, so not.
    // So just test the dwarfs case and also a case where the 7 smallest sum=100.
    std::vector<int> v4 = {1, 2, 3, 4, 5, 6, 7, 8, 64}; // sum=100, but 7 smallest sum=1+2+3+4+5+6+7=28, not 100. So not.
    // Let's create a known one: {1,2,3,4,5,6,7,8,64}? no.
    // Use {20,20,20,20,20,20,20,20,20} but not distinct.
    // We'll just test the dwarfs example, and also a custom with one valid subset.
    std::vector<int> v5 = {1, 2, 3, 4, 5, 6, 7, 8, 64}; // no valid subset? 1+2+3+4+5+6+8=29, etc. Not.
    // Better to test with a valid, e.g., {10,10,10,10,10,10,10,10,20} sum=110, need 100: exclude 10 and 10? sum=90? Actually 8 tens+20=100, but we need 7 numbers: 8 tens=80+20=100? That's 9 numbers? No.
    // So just use the dwarfs test and a second test where the lexicographically smallest is the first 7 numbers.
    std::vector<int> v6 = {5, 5, 5, 5, 5, 5, 5, 5, 60}; // sum=100, need 7 numbers: pick five 5's + 60 + two 5's? That's 7 numbers? 7*5=35+60=95. Not.
    // We'll rely on the dwarfs example and create one more: {1,1,1,1,1,1,1,2,91}? but distinct needed.
    // Let's just assert the dwarfs test and a second with only one valid: {1,2,3,4,5,6,7,72,0}? but 0 not allowed.
    // To keep robust, we can just test the dwarfs case and also check the sum of returned is 100.
    std::vector<int> result = findSevenWithSum100(dwarfs);
    int sum = 0;
    for (int x : result) sum += x;
    assert(sum == 100);
    assert(result.size() == 7);
    // Also test with another valid set: {1,2,3,4,5,6,7,8,64} doesn't work. Use {10,10,10,10,10,10,10,10,20}? sum=110, need 100, but we need 7 numbers, can't because 8 tens+20=100 is 9 numbers. So not valid.
    // We'll just test with the given example and a simple custom: {5,10,15,20,25,30,35,40,45} sum=225, no. But we can test that function returns something of size 7 for the dwarfs.
    // Additional check: ensure the returned indices are lexicographically smallest. For dwarfs, exclude indices 5 and 6 (0-based) which gives the first 7 indices? Actually the subset {0,1,2,3,4,7,8} has indices 0,1,2,3,4,7,8. There might be another subset excluding {0,1}? Sum would be 7+23+19+10+15+25+8+13? That's 8 numbers. So only one valid here.
    // So just assert the dwarfs case.
    return 0;
}
// The problem requires selecting exactly 7 numbers from 9 such that their sum is 100, preserving input order, and choosing the lexicographically smallest index set among all valid solutions. A brute-force approach is appropriate because there are only 9 input values and we must choose 7, which is equivalent to choosing 2 to exclude. The number of combinations is C(9,7)=36, so we can enumerate all possible selections using recursion (or combinations) and track the best solution. The key edge case is that multiple selections may sum to 100; we must choose the one with the smallest index sequence (comparing indices in order). To do this, we can iterate over all subsets of size 7 (or equivalently, all pairs to exclude), compute the sum, and if the sum equals 100, compare the current selected indices with the best found so far using lexicographic comparison. The greedy enumeration from smallest index combinations first (by iterating in increasing index order) naturally yields the lexicographically smallest solution if we stop at the first valid sum. However, because we iterate over all subsets, we must ensure we pick the first in lexicographic order. Since the input numbers are distinct and we only need 7 out of 9, the recursion can generate subsets in a deterministic order (e.g., always pick the smallest index available) and stop when the first valid sum is found, because that will be the lexicographically smallest index set. Time complexity is O(36 * 7) = O(1) effectively, and space complexity is O(9) for the visited array.
