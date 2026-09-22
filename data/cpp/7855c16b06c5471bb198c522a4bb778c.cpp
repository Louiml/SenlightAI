/*
Given a sequence of \( n \) integers and a limit \( c \), write a C++ function that determines the maximum number of elements that can be kept from the sequence such that each distinct value appears at most \( c \) times in the kept set. The function should take as input a vector of integers and the integer \( c \), and return the maximum number of elements that can be retained. For example, if the sequence is `[1,1,2,2,3]` and \( c=1 \), the answer is 3 (keep one of each distinct value). If \( c=2 \), the answer is 5 (keep both 1s and 2s and the 3, since each appears at most 2 times). The input values are positive integers between 1 and 100 inclusive, and \( n \) can be up to 1000. The function must be autonomous—no global variables—and should be efficient for the given constraints.
*/
#include <vector>
#include <algorithm>

// Given a vector of integers (each between 1 and 100) and a limit c,
// return the maximum number of elements that can be kept such that
// each distinct value appears at most c times.
int maxKeptElements(const std::vector<int>& nums, int c) {
    // Frequency array for values in range 1..100
    int freq[101] = {0};
    for (int num : nums) {
        ++freq[num];
    }
    
    int total = 0;
    for (int i = 1; i <= 100; ++i) {
        if (freq[i] > 0) {
            total += std::min(freq[i], c);
        }
    }
    return total;
}
#include <cassert>
#include <vector>

int maxKeptElements(const std::vector<int>& nums, int c); // declaration from solution

int main() {
    // Test 1: basic case with c=1
    assert(maxKeptElements({1,1,2,2,3}, 1) == 3);
    // Test 2: c=2 allows keeping everything (freqs are 2,2,1)
    assert(maxKeptElements({1,1,2,2,3}, 2) == 5);
    // Test 3: c very large, answer is n
    assert(maxKeptElements({5,5,5,7,7}, 10) == 5);
    // Test 4: all same values, c limits
    assert(maxKeptElements({4,4,4,4}, 3) == 3);
    // Test 5: single element
    assert(maxKeptElements({42}, 5) == 1);
    // Test 6: empty vector
    assert(maxKeptElements({}, 3) == 0);
    // Test 7: c=1 with many duplicates across different values
    assert(maxKeptElements({1,1,2,2,3,3,4}, 1) == 4);
    // Test 8: mixed frequencies with c=2
    assert(maxKeptElements({1,1,1,2,2,3}, 2) == 5); // keep two 1s, two 2s, one 3
    return 0;
}
// The problem reduces to counting the frequency of each distinct integer in the input vector. For each distinct value with frequency \( f \), we can keep at most \( \min(f, c) \) copies of that value. Summing these contributions over all distinct values gives the maximum number of elements we can retain. This is because the constraint is independent per value: values with frequency greater than \( c \) are capped to \( c \), while those with frequency ≤ \( c \) can all be kept. The answer is simply the sum of capped frequencies. Edge cases: if \( c \) is larger than the maximum frequency, answer equals \( n \); if \( c=0 \), answer is 0 (but the problem likely assumes \( c \ge 1 \), since the original code uses `min(m[i], c)` and `c` is positive). Since values are bounded to 1..100, we can use a fixed-size frequency array instead of a map. Complexity is \( O(n + 100) \) time and \( O(100) \) auxiliary space.
