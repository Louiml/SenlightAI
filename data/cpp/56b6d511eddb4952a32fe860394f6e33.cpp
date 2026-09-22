// Write a C++ function that takes two integers `left` and `right` (inclusive) and returns a `std::vector<int>` containing all numbers in that range which are "self-dividing." A self-dividing number is a positive integer that is divisible by every digit it contains. For example, 128 is self-dividing because 128 % 1 == 0, 128 % 2 == 0, and 128 % 8 == 0. A number that contains the digit 0 is not self-dividing because division by zero is undefined. The function should return the self-dividing numbers in increasing order. Assume `left` and `right` are positive integers with `left <= right`, but handle cases where only one number qualifies or none do.

The solution iterates through every integer `curr` from `left` to `right` inclusive. For each number, we examine its digits by repeatedly taking the remainder when dividing by 10. If any digit is 0, the flag is set to false because division by zero is undefined. Otherwise, we check if `curr` is divisible by that digit using the modulo operator; if not, the number is not self-dividing. We break the loop early as soon as a failing condition is found for efficiency. If the loop completes without flagging an issue, we add `curr` to the result vector. Edge cases include numbers with a single digit (all single-digit numbers 1-9 are self-dividing), numbers containing zero (excluded), and numbers that are not divisible by one of their digits (e.g., 10 → digit 0 excluded). Time complexity is O(n * d) where n = right - left + 1 and d is the maximum number of digits (which is at most 7 for typical int range), so effectively O(n log(right)). Space complexity is O(k) where k is the number of self-dividing numbers returned, otherwise O(1) extra space besides the output.

#include <vector>

// Return all self-dividing numbers in the inclusive range [left, right].
std::vector<int> findSelfDividingNumbers(int left, int right) {
    std::vector<int> result;
    for (int current = left; current <= right; ++current) {
        int num = current;
        bool isSelfDividing = true;
        while (num > 0) {
            int digit = num % 10;
            if (digit == 0 || current % digit != 0) {
                isSelfDividing = false;
                break;
            }
            num /= 10;
        }
        if (isSelfDividing) {
            result.push_back(current);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Forward declaration for the function under test.
std::vector<int> findSelfDividingNumbers(int left, int right);

int main() {
    assert(findSelfDividingNumbers(1, 22) == (std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 15, 22}));
    assert(findSelfDividingNumbers(47, 85) == (std::vector<int>{48, 55, 66, 77}));
    assert(findSelfDividingNumbers(100, 120) == (std::vector<int>{111, 112}));
    assert(findSelfDividingNumbers(20, 20) == (std::vector<int>{}));
    assert(findSelfDividingNumbers(7, 7) == (std::vector<int>{7}));
    assert(findSelfDividingNumbers(10, 11) == (std::vector<int>{11}));
    assert(findSelfDividingNumbers(128, 128) == (std::vector<int>{128}));
    assert(findSelfDividingNumbers(1, 9) == (std::vector<int>{1,2,3,4,5,6,7,8,9}));
    assert(findSelfDividingNumbers(100, 100) == (std::vector<int>{}));
    assert(findSelfDividingNumbers(2, 2) == (std::vector<int>{2}));
    return 0;
}
