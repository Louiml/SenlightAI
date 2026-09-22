/*
Given a permutation of integers from 1 to N (inclusive), write a C++ function that returns a string of length N consisting of only '0' and '1' characters. For each prefix of the permutation (considering the first k elements for k = 1 to N), the function should output '1' if the set of numbers in that prefix forms a contiguous range of integers (i.e., contains all integers from some minimum value to some maximum value without any gaps), and '0' otherwise. The input is provided as a vector of integers representing the permutation. For example, for the permutation {2, 3, 1, 5, 4}, the output should be "01011": after the first element {2} is contiguous, after the second {2,3} is contiguous, after the third {2,3,1} is contiguous, after the fourth {2,3,1,5} is not (missing 4), and after the fifth all are contiguous.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Returns a binary string where the k-th character (0-indexed) is '1'
// if the first k+1 elements of the permutation form a contiguous range,
// and '0' otherwise.
std::string prefixContiguity(const std::vector<int>& permutation) {
    const int n = static_cast<int>(permutation.size());
    std::vector<int> position(n + 1);
    
    for (int i = 0; i < n; ++i) {
        position[permutation[i]] = i + 1; // 1-indexed position
    }
    
    int min_pos = INT_MAX;
    int max_pos = INT_MIN;
    std::string result;
    result.reserve(n);
    
    for (int value = 1; value <= n; ++value) {
        min_pos = std::min(min_pos, position[value]);
        max_pos = std::max(max_pos, position[value]);
        
        if (max_pos - min_pos + 1 == value) {
            result.push_back('1');
        } else {
            result.push_back('0');
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declare the function from the solution (must be included or defined above)
std::string prefixContiguity(const std::vector<int>& permutation);

int main() {
    // Example from the problem statement
    std::vector<int> perm1 = {2, 3, 1, 5, 4};
    assert(prefixContiguity(perm1) == "01011");
    
    // Sorted permutation: every prefix is contiguous
    std::vector<int> perm2 = {1, 2, 3, 4};
    assert(prefixContiguity(perm2) == "1111");
    
    // Reverse permutation: only the full set is contiguous
    std::vector<int> perm3 = {4, 3, 2, 1};
    assert(prefixContiguity(perm3) == "0001");
    
    // Single element
    std::vector<int> perm4 = {1};
    assert(prefixContiguity(perm4) == "1");
    
    // Larger permutation with mixed results
    std::vector<int> perm5 = {3, 1, 2, 5, 4};
    // Steps: {3}->0, {3,1}->0, {3,1,2}->1, {3,1,2,5}->0, {3,1,2,5,4}->1
    assert(prefixContiguity(perm5) == "00101");
    
    // Another case: first two form contiguous, then break
    std::vector<int> perm6 = {2, 1, 4, 3};
    // {2}->1, {2,1}->1, {2,1,4}->0, {2,1,4,3}->1
    assert(prefixContiguity(perm6) == "1101");
    
    // All elements in the middle, check for large N
    std::vector<int> perm7 = {5, 1, 2, 3, 4};
    // {5}->1, {5,1}->0, {5,1,2}->0, {5,1,2,3}->0, {5,1,2,3,4}->1
    assert(prefixContiguity(perm7) == "10001");
    
    return 0;
}

// We first note that the input is a permutation, so each integer from 1 to N appears exactly once. We can precompute the position (1-indexed) of each value in the permutation. Then, we iterate through values from 1 to N in increasing order, simulating adding the elements in the order they appear in the permutation. Specifically, when we process value i (i from 1 to N), we consider the set of positions of values 1 through i. The key observation is that this set of positions forms a contiguous block in the permutation if and only if (maximum position among 1..i) - (minimum position among 1..i) + 1 == i. This is because the positions are distinct, and the count of positions equals i, so if they form a contiguous block, the span (max-min+1) must equal the count. We maintain running minimum and maximum of positions as we iterate. For each i, we compare the current span length to i and append '1' or '0' to the result string. The algorithm is O(N) time and O(N) auxiliary space (for the position array). Edge cases include N=1 (always '1') and permutations where early prefixes are already contiguous (e.g., sorted permutation gives all '1's). No special handling is needed for duplicates since it's a permutation.
