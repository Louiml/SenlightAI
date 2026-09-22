Write a C++ function `addArrays` that takes two vectors of non-negative integers representing the digits of two numbers (where each digit is in the range 0–9 and the most significant digit is at index 0, i.e., the vectors are in normal left-to-right decimal order) and returns a new vector of integers representing the sum of these two numbers as a decimal number (also with the most significant digit at index 0). The vectors may be of different lengths, and each vector is guaranteed to contain at least one digit (non-empty). The result should not contain leading zeros unless the sum itself is zero, in which case return a vector containing exactly one element `0`. The function must handle a carry properly, including a final carry that creates an extra most significant digit at the front.

// The solution processes the two input vectors from their least significant digits (the end of each vector) toward the front, summing digit pairs along with a carry variable, which starts at 0. Since the vectors may have different lengths, after the common-length loop, we handle the remaining digits of the longer vector (or the carry alone if both are exhausted). Because pushing digits during backward traversal yields the result in reverse order (least significant digit first), we reverse the accumulated vector before returning it. Edge cases include: vectors of length 1 (e.g., {0} and {0} should give {0}), unequal lengths with a carry propagating through the longer vector, and a final carry after the longest vector is processed (e.g., {9,9} plus {1} gives {1,0,0}). Time complexity is O(max(n,m)) where n and m are the sizes of the input vectors, and auxiliary space is O(max(n,m)) for the result storage (not counting the input vectors themselves). The algorithm is iterative and uses only a few integer variables plus the result vector.

#include <vector>
#include <algorithm>

// Returns a vector representing the decimal sum of two digit-vectors.
// Each input digit must be 0-9, most significant digit at index 0.
std::vector<int> addArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    int i = static_cast<int>(nums1.size()) - 1;
    int j = static_cast<int>(nums2.size()) - 1;
    std::vector<int> ans;
    int carry = 0;

    while (i >= 0 && j >= 0) {
        int sum = nums1[i] + nums2[j] + carry;
        carry = sum / 10;
        ans.push_back(sum % 10);
        --i;
        --j;
    }

    while (i >= 0) {
        int sum = nums1[i] + carry;
        carry = sum / 10;
        ans.push_back(sum % 10);
        --i;
    }

    while (j >= 0) {
        int sum = nums2[j] + carry;
        carry = sum / 10;
        ans.push_back(sum % 10);
        --j;
    }

    if (carry > 0) {
        ans.push_back(carry);
    }

    std::reverse(ans.begin(), ans.end());

    // Remove leading zeros if result is not zero
    while (ans.size() > 1 && ans.front() == 0) {
        ans.erase(ans.begin());
    }

    return ans;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // Basic equal-length addition
    assert(addArrays({1, 2, 3}, {4, 5, 6}) == std::vector<int>({5, 7, 9}));
    // Different lengths
    assert(addArrays({9, 9}, {1}) == std::vector<int>({1, 0, 0}));
    // Long vector plus single digit
    assert(addArrays({1, 0, 0}, {1}) == std::vector<int>({1, 0, 1}));
    // All zeros
    assert(addArrays({0}, {0}) == std::vector<int>({0}));
    // Carry through entire vector
    assert(addArrays({9, 9, 9}, {1}) == std::vector<int>({1, 0, 0, 0}));
    // Single digits with carry
    assert(addArrays({9}, {9}) == std::vector<int>({1, 8}));
    // No carry, different sizes
    assert(addArrays({1, 2, 3, 4}, {5}) == std::vector<int>({1, 2, 3, 9}));
    // Leading zeros are stripped
    assert(addArrays({0, 1}, {0, 9}) == std::vector<int>({1, 0}));
    // Larger example
    assert(addArrays({1, 2, 3, 4, 5}, {9, 8, 7}) == std::vector<int>({1, 3, 3, 3, 2}));
    return 0;
}
