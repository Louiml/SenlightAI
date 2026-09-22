/*
Given an integer `n` (where `1 <= n <= 10^5`), write a C++ function `generate_sequence(int n)` that returns a `std::vector<int>` of length `n` containing distinct positive integers. If `n` is odd and greater than 1, the sequence must consist of consecutive even integers starting from 2 (i.e., 2, 4, 6, ...). If `n` is even, or if `n == 1`, the sequence must consist of consecutive odd integers starting from 1 (i.e., 1, 3, 5, ...). The function must not take any input from the user, must not print anything, and must return the sequence as a vector. The sequence elements should be in increasing order.
*/

#include <vector>

// Generate a sequence of n distinct integers.
// - If n is odd and > 1: consecutive even integers starting from 2.
// - Otherwise (n even or n == 1): consecutive odd integers starting from 1.
std::vector<int> generate_sequence(int n) {
    std::vector<int> result;
    result.reserve(n);
    
    const bool use_even = (n % 2 != 0 && n != 1);
    int start = use_even ? 2 : 1;
    const int step = 2;
    
    for (int i = 0; i < n; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here (or via header)
// For brevity, the function is assumed to be defined above.

int main() {
    // n == 1 -> odd sequence: {1}
    assert((generate_sequence(1) == std::vector<int>{1}));
    
    // n == 2 (even) -> odd sequence: {1,3}
    assert((generate_sequence(2) == std::vector<int>{1, 3}));
    
    // n == 3 (odd, >1) -> even sequence: {2,4,6}
    assert((generate_sequence(3) == std::vector<int>{2, 4, 6}));
    
    // n == 4 (even) -> odd sequence: {1,3,5,7}
    assert((generate_sequence(4) == std::vector<int>{1, 3, 5, 7}));
    
    // n == 5 (odd, >1) -> even sequence: {2,4,6,8,10}
    assert((generate_sequence(5) == std::vector<int>{2, 4, 6, 8, 10}));
    
    // n == 6 (even) -> odd sequence: {1,3,5,7,9,11}
    assert((generate_sequence(6) == std::vector<int>{1, 3, 5, 7, 9, 11}));
    
    // n == 7 (odd, >1) -> even sequence: {2,4,6,8,10,12,14}
    assert((generate_sequence(7) == std::vector<int>{2, 4, 6, 8, 10, 12, 14}));
    
    // Large even n: check length and that all elements are odd
    std::vector<int> big_even = generate_sequence(100000);
    assert(big_even.size() == 100000);
    assert(big_even.front() == 1);
    assert(big_even.back() == 199999);
    // Verify parity: all odd
    for (int val : big_even) {
        assert(val % 2 == 1);
    }
    
    // Large odd n > 1: check length and that all elements are even
    std::vector<int> big_odd = generate_sequence(99999);
    assert(big_odd.size() == 99999);
    assert(big_odd.front() == 2);
    assert(big_odd.back() == 199998);
    // Verify parity: all even
    for (int val : big_odd) {
        assert(val % 2 == 0);
    }
    
    return 0;
}

// The solution is straightforward: determine the parity of `n`. If `n` is odd and greater than 1, use even numbers starting at 2 and increment by 2 each time; otherwise use odd numbers starting at 1 and increment by 2. This is because the original code's logic ensures that for odd `n` (excluding 1) it outputs evens (so that the last element is `2n`, which is even), and for even `n` or `n==1` it outputs odds (so that the last element is `2n-1`, which is odd). The algorithm runs in O(n) time and uses O(n) space to store the result (which is necessary to return a vector). Edge cases: `n==1` must produce `{1}` (odd case), and `n` being any other odd number (e.g., 3, 5) must produce even numbers starting at 2. For even `n` (e.g., 2, 4), odd numbers starting at 1 are used. The values are guaranteed to fit in a regular `int` because `n ≤ 10^5` gives max value `2n ≤ 2*10^5`.
