// Write a C++ function `sumOfRepeatedDigits` that takes a non-empty vector of non-negative integers and returns a vector of integers. For each element in the input vector, the function must repeatedly compute the sum of its decimal digits until a single digit is obtained (i.e., the digital root), then add that single digit to a running total. The returned vector must contain, at each index, the cumulative sum of all digital roots processed from the beginning of the vector up to and including that index. For example, given input {123, 45, 7}, the digital roots are 6 (1+2+3=6), 9 (4+5=9), and 7, producing cumulative sums {6, 15, 22}. The function must handle zeros (digital root 0) and large numbers, and must not modify the input vector.

The core task is to compute the digital root of each number (repeatedly summing digits until one digit remains), which can be done elegantly using the mathematical property: for any non-negative integer `n`, the digital root is `1 + (n - 1) % 9` when `n > 0`, and `0` when `n == 0`. This avoids loops and works in O(1) per number. Alternatively, a loop can be written to sum digits iteratively, which is also correct but less efficient. After computing the digital root for each element, maintain a running cumulative sum and push it to the result vector as we iterate. Edge cases: input containing zeros (digital root 0), single-element vectors, very large integers (within `int` range), and vectors where all elements produce the same digital root. The algorithm processes each element exactly once, so time complexity is O(m) where m is the number of elements, and space complexity is O(m) for the output vector (excluding any temporary storage used in digit summation, which is O(1) if using the modular formula). No special handling is needed for negative numbers because the input is constrained to non-negative.

#include <vector>

// Compute the digital root of a non-negative integer.
int digitalRoot(int n) {
    if (n == 0) return 0;
    return 1 + (n - 1) % 9;
}

// Return cumulative sums of digital roots for each element in the input vector.
std::vector<int> sumOfRepeatedDigits(const std::vector<int>& numbers) {
    std::vector<int> result;
    result.reserve(numbers.size());
    int runningSum = 0;
    for (int value : numbers) {
        runningSum += digitalRoot(value);
        result.push_back(runningSum);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example from the task statement
    std::vector<int> input1 = {123, 45, 7};
    assert((sumOfRepeatedDigits(input1) == std::vector<int>{6, 15, 22}));

    // Single element
    assert((sumOfRepeatedDigits({0}) == std::vector<int>{0}));
    assert((sumOfRepeatedDigits({9}) == std::vector<int>{9}));
    assert((sumOfRepeatedDigits({100}) == std::vector<int>{1}));

    // All zeros
    assert((sumOfRepeatedDigits({0, 0, 0}) == std::vector<int>{0, 0, 0}));

    // Large numbers
    std::vector<int> input5 = {999999, 888888};
    // Digital root of 999999 is 9, of 888888 is 3 (8+8+8+8+8+8=48, 4+8=12, 1+2=3)
    assert((sumOfRepeatedDigits(input5) == std::vector<int>{9, 12}));

    // Mixed values including zeros
    std::vector<int> input6 = {10, 0, 99, 1};
    // Digital roots: 1, 0, 9, 1 → cumulative: 1, 1, 10, 11
    assert((sumOfRepeatedDigits(input6) == std::vector<int>{1, 1, 10, 11}));

    // Numbers that reduce to same digital root
    std::vector<int> input7 = {18, 27, 36};
    // All digital roots are 9 → cumulative: 9, 18, 27
    assert((sumOfRepeatedDigits(input7) == std::vector<int>{9, 18, 27}));

    // Large vector of length 10
    std::vector<int> input8 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // Digital roots: 1,2,3,4,5,6,7,8,9,1 → cumulative: 1,3,6,10,15,21,28,36,45,46
    assert((sumOfRepeatedDigits(input8) == std::vector<int>{1,3,6,10,15,21,28,36,45,46}));

    // Zero followed by non-zeros
    assert((sumOfRepeatedDigits({0, 1}) == std::vector<int>{0, 1}));

    // Large number with many zeros inside
    assert((sumOfRepeatedDigits({1000, 2000}) == std::vector<int>{1, 3}));
}
