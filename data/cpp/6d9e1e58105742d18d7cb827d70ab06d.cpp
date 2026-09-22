Write a C++ function `minimumSwapsToFixParity` that takes a vector of integers and returns the minimum number of adjacent swaps needed to make every element at an even index (0-based) even, and every element at an odd index odd. If it is impossible (because the counts of misplaced even and odd elements do not match), return -1. The function should handle empty vectors (return 0), vectors with only one element (auto-correct if already correct, else -1), and vectors with negative numbers (consider parity based on `n % 2` where negative odd yields 1, negative even yields 0). Note that each misplaced even at an odd index must be swapped with a misplaced odd at an even index; each such swap fixes both positions, so the answer is the count of either type of misplaced elements (they must be equal).

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimumSwapsToFixParity({}) == 0);
    assert(minimumSwapsToFixParity({2}) == 0);
    assert(minimumSwapsToFixParity({1}) == -1);
    
    // Example from problem: n=4, arr = [4, 3, 2, 1] -> 2 misplaced evens at odd indices, 2 misplaced odds at even indices -> answer 2
    assert(minimumSwapsToFixParity({4,3,2,1}) == 2);
    
    // Already correct
    assert(minimumSwapsToFixParity({2,1,4,3}) == 0);
    
    // Impossible: count mismatch
    assert(minimumSwapsToFixParity({2,1,3}) == -1); // evens: index0 even ok, index1 odd? 1 is odd but index1 odd ok, index2 odd? 3 odd at even index -> 1 odd_in_wrong, evens=0 -> -1
    
    // Negative numbers
    assert(minimumSwapsToFixParity({-2,-1,-4,-3}) == 0); // -2 even at even, -1 odd at odd, -4 even at even, -3 odd at odd
    assert(minimumSwapsToFixParity({-2,1,-4,3}) == 0); // already correct
    
    // Single wrong element
    assert(minimumSwapsToFixParity({3}) == -1);
    assert(minimumSwapsToFixParity({4}) == 0);
    
    // Larger mixed case
    std::vector<int> v = {1,2,3,4,5,6}; // indices:0 odd->even? 1 odd at even -> wrong, 2 even at odd -> wrong, 3 odd at even -> wrong, 4 even at odd -> wrong, 5 odd at even -> wrong, 6 even at odd -> wrong→ evens at odd: positions1,3,5 ->3, odds at even: positions0,2,4 ->3 → answer 3
    assert(minimumSwapsToFixParity(v) == 3);
    
    // All even length and all wrong
    std::vector<int> w = {1,2,1,2}; // index0 odd wrong, index1 even wrong, index2 odd wrong, index3 even wrong → evens at odd=2, odds at even=2 → 2
    assert(minimumSwapsToFixParity(w) == 2);
    
    return 0;
}

#include <vector>
#include <cstdlib> // for std::abs

// Returns the minimum number of swaps needed to make even-indexed positions even
// and odd-indexed positions odd. Returns -1 if not possible.
int minimumSwapsToFixParity(const std::vector<int>& arr) {
    if (arr.empty()) return 0;
    
    int even_in_wrong_pos = 0; // even value at odd index (0-based)
    int odd_in_wrong_pos = 0;  // odd value at even index (0-based)
    
    for (std::size_t i = 0; i < arr.size(); ++i) {
        bool is_even = (std::abs(arr[i]) % 2 == 0);
        bool index_is_even = (i % 2 == 0);
        if (is_even && !index_is_even) {
            ++even_in_wrong_pos;
        } else if (!is_even && index_is_even) {
            ++odd_in_wrong_pos;
        }
    }
    
    if (even_in_wrong_pos == odd_in_wrong_pos) {
        return even_in_wrong_pos;
    }
    return -1;
}

// The core idea is to count two types of violations: (1) an even number placed at an odd index (0-based), and (2) an odd number placed at an even index. For the array to be fixable, every misplaced even must be swapped with a misplaced odd, so these two counts must be equal. If they are equal, the minimum number of adjacent swaps is exactly that count because each swap of two adjacent misplaced elements (an even at odd index and an odd at even index) corrects both positions simultaneously. If the counts differ, it’s impossible (−1). Edge cases: an empty vector has no violations (return 0); a single-element vector is only correct if it matches the parity of index 0 (even), else −1. The algorithm loops through the vector once, checking parity of each element using `abs(arr[i]) % 2` to safely handle negatives. Time complexity is O(n), space complexity O(1). No actual swaps are performed; we only count the violations.
