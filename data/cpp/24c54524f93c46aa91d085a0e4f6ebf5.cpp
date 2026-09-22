Write a C++ function that takes a non-empty vector of integers and returns the sum of all elements using a recursive pairwise-outside-in trimming approach: the function should add the current leftmost and rightmost elements together, then recursively sum the remaining subarray between them (excluding those two elements). The recursion base cases are when the subarray has one element (return that element) or two elements (return their sum). The function must be named `sumOutsideIn`, accept a `const std::vector<int>&`, and return an `int`. It should handle both even and odd lengths, including when the input length is 1 or 2. You may assume the vector contains all integers in the range [-25, 25], but your solution must work for any integer values.

// The approach is to recursively process the vector from both ends toward the center. At each recursive call, the function receives the full vector plus two indices `left` and `right` representing the current boundary of the subarray still to be summed. The main algorithm: if `left > right`, the subarray is empty (should not happen with proper base cases), return 0. If `left == right`, return the single element at that index. If `left + 1 == right`, return the sum of the two elements. Otherwise, compute `data[left] + data[right]` and add the result of recursively calling the function on `left+1` and `right-1`. This effectively adds the outermost pair first, then recurses inward. Edge cases: odd-length vectors have a middle element that will be handled by the `left == right` base case; even-length vectors will eventually hit the `left+1 == right` base case. An empty vector is not expected but could be handled by returning 0 if desired. Time complexity is O(n) because each element is visited exactly once across all recursive calls, and there are O(n) recursive calls. Auxiliary space complexity is O(n) due to the recursion stack depth (in the worst case, about n/2 calls).

#include <vector>

// Recursively sum the elements of the vector by repeatedly adding the outermost pair.
// left and right are the current inclusive boundaries of the subarray to sum.
int sumOutsideIn(const std::vector<int>& data, int left, int right) {
    // Base case: empty subarray (should not normally occur).
    if (left > right) return 0;
    // Base case: single element.
    if (left == right) return data[left];
    // Base case: exactly two elements.
    if (left + 1 == right) return data[left] + data[right];
    // Recursive step: add outermost pair and recurse inward.
    return data[left] + data[right] + sumOutsideIn(data, left + 1, right - 1);
}

// Public wrapper that starts the recursion on the whole vector.
int sumOutsideIn(const std::vector<int>& data) {
    if (data.empty()) return 0;
    return sumOutsideIn(data, 0, static_cast<int>(data.size()) - 1);
}

#include <cassert>
#include <vector>

// The solution function is declared above (or included here).
int sumOutsideIn(const std::vector<int>& data);

int main() {
    // Single element
    assert(sumOutsideIn({5}) == 5);
    assert(sumOutsideIn({-3}) == -3);
    
    // Two elements
    assert(sumOutsideIn({1, 2}) == 3);
    assert(sumOutsideIn({-5, 5}) == 0);
    
    // Odd length
    assert(sumOutsideIn({1, 2, 3, 4, 5}) == 15);
    assert(sumOutsideIn({-1, 2, -3, 4, -5}) == -3);
    
    // Even length
    assert(sumOutsideIn({1, 2, 3, 4}) == 10);
    assert(sumOutsideIn({-10, -20, 30, 40}) == 40);
    
    // All same values
    assert(sumOutsideIn({7, 7, 7}) == 21);
    
    // Mixed negative and positive
    assert(sumOutsideIn({-2, -1, 0, 1, 2}) == 0);
    
    // Empty (though spec says non-empty, test defensive behavior)
    assert(sumOutsideIn({}) == 0);
    
    return 0;
}
