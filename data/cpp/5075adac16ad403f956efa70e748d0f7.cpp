/*
Write a standalone C++ function named `subsetXORSum` that takes a vector of non-negative integers (possibly empty) and returns the sum of the XOR totals of all possible subsets (including the empty subset, which contributes 0). The function must be self-contained, use recursion or iteration, handle the empty vector case by returning 0, and adhere to `const` correctness by taking the vector by `const std::vector<int>&`. The algorithm should be derived from the concept of exploring each element with a choice to include or exclude it, computing the XOR of the selected elements for each complete subset, and accumulating the totals. Provide a reference implementation and test it with several cases, including an empty input, a single-element input, a vector with repeated values, and a larger vector with known expected outputs.
*/

#include <vector>

// Compute the sum of XOR totals for all subsets of the given vector.
// The empty subset contributes 0, so an empty input yields 0.
int subsetXORSum(const std::vector<int>& nums) {
    // Helper function for recursive backtracking.
    void backtrack(int pos, int currXOR, int& total) {
        if (pos == static_cast<int>(nums.size())) {
            total += currXOR;
            return;
        }
        // Exclude the current element.
        backtrack(pos + 1, currXOR, total);
        // Include the current element (toggle via XOR).
        backtrack(pos + 1, currXOR ^ nums[pos], total);
    }
    
    if (nums.empty()) {
        return 0;
    }
    int total = 0;
    backtrack(0, 0, total);
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Empty vector: only empty subset exists, XOR = 0, sum = 0.
    std::vector<int> empty;
    assert(subsetXORSum(empty) == 0);

    // Single element: subsets are {} and {5}, XORs = 0 + 5 = 5.
    std::vector<int> single = {5};
    assert(subsetXORSum(single) == 5);

    // Two elements: subsets {} (0), {1} (1), {2} (2), {1,2} (3) -> sum = 6.
    std::vector<int> two = {1, 2};
    assert(subsetXORSum(two) == 6);

    // With a zero: {0,3} => {} (0), {0}(0), {3}(3), {0,3}(3) -> sum = 6.
    std::vector<int> withZero = {0, 3};
    assert(subsetXORSum(withZero) == 6);

    // Repeated values: {2,2} => {} (0), {2}(2), {2}(2), {2,2}(0) -> sum = 4.
    std::vector<int> repeated = {2, 2};
    assert(subsetXORSum(repeated) == 4);

    // Larger example: {1, 3} => {} (0), {1}(1), {3}(3), {1,3}(2) -> sum = 6.
    std::vector<int> larger = {1, 3};
    assert(subsetXORSum(larger) == 6);

    // Classic example: {1, 2, 3} => all subsets:
    // {} = 0, {1}=1, {2}=2, {3}=3, {1,2}=3, {1,3}=2, {2,3}=1, {1,2,3}=0.
    // Sum = 0+1+2+3+3+2+1+0 = 12.
    std::vector<int> classic = {1, 2, 3};
    assert(subsetXORSum(classic) == 12);

    // Larger set with known total: {5, 1, 6} => sum is 28.
    // Let's compute: subsets XORs:
    // {} 0, {5} 5, {1} 1, {6} 6, {5,1} 4, {5,6} 3, {1,6} 7, {5,1,6} 2
    // Sum = 0+5+1+6+4+3+7+2 = 28.
    std::vector<int> complex = {5, 1, 6};
    assert(subsetXORSum(complex) == 28);

    return 0;
}

// The core idea is to enumerate all possible subsets via a backtracking (depth-first) approach. At each position in the vector, we have two choices: either include the current element (which toggles the current XOR value via XOR operation) or exclude it (leaving the current XOR unchanged). When we reach the end of the vector (i.e., `pos == nums.size()`), we have fully constructed a subset, and we add its XOR total to a running sum. The empty subset is naturally covered because at the start the current XOR is 0, and if we choose to exclude every element, at the end we add 0. The backtracking pattern ensures that we explore both branches for every element, leading to \(2^n\) leaf nodes, where \(n\) is the number of elements. Important edge cases: an empty input vector should immediately return 0, since there is only the empty subset whose XOR is 0. A vector with a single element yields two subsets: the empty one (XOR 0) and the subset containing the element itself (XOR equals that element), so the sum is that element's value. Repeated values are handled naturally by the algorithm because each element is considered independently. The time complexity is \(O(2^n)\) because each subset is visited exactly once, and the space complexity is \(O(n)\) due to the recursion stack depth in the worst case.
