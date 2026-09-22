Write a C++ function that takes a non-empty `std::vector<int>` containing integers where every element appears exactly twice except for one element that appears only once, and returns that single element. The function must work correctly for vectors with negative numbers, zeros, and large positive values, and must handle vectors of any size (including size 1). The input vector is read-only, and the solution should not use any extra data structures beyond constant auxiliary space. The function signature should be `int findSingleNumber(const std::vector<int>& nums)`.

#include <cassert>
#include <vector>

int findSingleNumber(const std::vector<int>& nums); // declaration for test

int main() {
    assert(findSingleNumber({1}) == 1);
    assert(findSingleNumber({2, 2, 1}) == 1);
    assert(findSingleNumber({4, 1, 2, 1, 2}) == 4);
    assert(findSingleNumber({-1, -1, -2}) == -2);
    assert(findSingleNumber({0, 0, 0, 0, 5}) == 5);
    assert(findSingleNumber({3, 3, 7, 7, 9, 9, 11}) == 11);
    assert(findSingleNumber({100000, 100000, -100000, -100000, 42}) == 42);
    assert(findSingleNumber({1, 2, 3, 4, 5, 1, 2, 3, 4}) == 5);
    assert(findSingleNumber({-5, -5, 10, 10, 20, 20, -7}) == -7);
    return 0;
}

#include <vector>

// Returns the element that appears exactly once, given that all others appear twice.
int findSingleNumber(const std::vector<int>& nums) {
    int result = 0;
    for (int value : nums) {
        result ^= value; // XOR cancels pairs, leaving the unique element
    }
    return result;
}

// The key insight is the XOR (exclusive OR) operation's properties: it is commutative, associative, and each number XORed with itself gives 0, while any number XORed with 0 gives itself. Therefore, if we XOR all elements together, all pairs cancel out because `x ^ x = 0`, leaving only the element that appears once, because `0 ^ single = single`. The order of elements does not matter due to commutativity and associativity. Edge cases to consider: a vector with exactly one element (then the loop XORs it with 0, returning that element), and negative numbers (since XOR works on bit patterns, negatives behave correctly). The time complexity is O(n), where n is the number of elements, since we traverse the vector once. The space complexity is O(1), as we only use a single integer accumulator and the loop variable.
