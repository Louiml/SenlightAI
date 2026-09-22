/*
Write a C++ function that takes an array of exactly 10 integers (each between 0 and 1000 inclusive) and returns the count of distinct remainders when each integer is divided by 42. For example, if the input array is {39, 40, 41, 42, 43, 44, 83, 84, 85, 86}, the remainders mod 42 are {39, 40, 41, 0, 1, 2, 41, 0, 1, 2}, so the distinct set is {0,1,2,39,40,41} giving a count of 6. The function should not rely on any global mutable state and must be reusable.
*/

#include <array>
#include <cstddef>

// Count the number of distinct remainders when each of the 10 integers is divided by 42.
int distinctRemainders(const std::array<int, 10>& values) {
    std::array<bool, 42> seen{};
    int count = 0;

    for (const int value : values) {
        const int remainder = value % 42;
        if (!seen[remainder]) {
            seen[remainder] = true;
            ++count;
        }
    }

    return count;
}

#include <cassert>
#include <array>

int distinctRemainders(const std::array<int, 10>&);

int main() {
    // All distinct remainders (0..9)
    std::array<int, 10> test1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(distinctRemainders(test1) == 10);

    // All same remainder
    std::array<int, 10> test2 = {42, 84, 126, 0, 42, 84, 0, 42, 84, 126};
    assert(distinctRemainders(test2) == 1);

    // Mixed as in example
    std::array<int, 10> test3 = {39, 40, 41, 42, 43, 44, 83, 84, 85, 86};
    assert(distinctRemainders(test3) == 6);

    // Remainders wrap around: 41 and 83 both give 41
    std::array<int, 10> test4 = {41, 83, 41, 83, 0, 42, 0, 42, 1, 43};
    assert(distinctRemainders(test4) == 3); // {41,0,1}

    // Edge: zeros and small values
    std::array<int, 10> test5 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    assert(distinctRemainders(test5) == 1);

    // Values up to 1000
    std::array<int, 10> test6 = {1000, 999, 998, 997, 996, 995, 994, 993, 992, 991};
    // remainders: 1000%42=34, 999%42=33, 998%42=32, 997%42=31, 996%42=30,
    // 995%42=29, 994%28, 993%27, 992%26, 991%25 → all distinct
    assert(distinctRemainders(test6) == 10);

    return 0;
}

// The goal is to count how many different values appear among the 10 remainders after dividing each input by 42. The simplest approach is to use a boolean array (or a fixed-size frequency array) of size 42, initialized to false. Iterate over the 10 input values, compute `value % 42`, and mark that index as seen. After processing all inputs, count how many indices are marked true. Since the modulo operation always yields a value from 0 to 41, we know the exact size of the tracking array, making it constant space. Important edge cases: all numbers may have the same remainder (count = 1), or all may be distinct (count = 10). The input constraints guarantee values are non-negative, but the modulo operation works for any integer in the given range. Time complexity is O(10) = O(1) since the input size is fixed, and space complexity is O(42) = O(1) as well.
