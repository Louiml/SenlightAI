// Write a C++ function that takes a positive integer `n` (1 ≤ n ≤ 10) and an array `a` of `n` integers, and returns a modified copy of the array with all numbers that are "Fibonacci-like" (each digit after the first two equals the sum of the two preceding digits, e.g., 112358) removed, and with the numbers shifted left to fill the gaps. The function should return the new logical size of the array (the number of remaining elements) and also output the modified array through a reference parameter. The original array must remain unchanged. Numbers that start with '1' and '1' only qualify if they have at least 3 digits (except "11" itself which is not considered Fibonacci-like). For example, given `[11, 112, 112358, 123, 13]`, the output should be `[11, 123, 13]` with new size 3, because 112 and 112358 are Fibonacci-like and are removed.

#include <cassert>
#include <vector>
#include <iostream>

int removeFibonacciLike(std::vector<int>& arr); // Forward declaration

int main() {
    // Test case 1: Basic example
    std::vector<int> a1 = {11, 112, 112358, 123, 13};
    int size1 = removeFibonacciLike(a1);
    assert(size1 == 3);
    assert(a1 == std::vector<int>({11, 123, 13}));

    // Test case 2: No Fibonacci-like numbers
    std::vector<int> a2 = {1, 22, 333, 111, 12};
    int size2 = removeFibonacciLike(a2);
    assert(size2 == 5);
    assert(a2 == std::vector<int>({1, 22, 333, 111, 12}));

    // Test case 3: All Fibonacci-like numbers
    std::vector<int> a3 = {112, 112358, 1123581321};
    int size3 = removeFibonacciLike(a3);
    assert(size3 == 0);
    assert(a3.empty());

    // Test case 4: Edge with single digit and "11"
    std::vector<int> a4 = {5, 11, 1122, 11235};
    int size4 = removeFibonacciLike(a4);
    assert(size4 == 3);
    assert(a4 == std::vector<int>({5, 11, 1122}));

    // Test case 5: Mixed with duplicate Fibonacci-like
    std::vector<int> a5 = {112, 112, 999, 112358};
    int size5 = removeFibonacciLike(a5);
    assert(size5 == 1);
    assert(a5 == std::vector<int>({999}));

    // Test case 6: Longer Fibonacci-like sequence
    std::vector<int> a6 = {112358132134, 2, 11235};
    int size6 = removeFibonacciLike(a6);
    assert(size6 == 1);
    assert(a6 == std::vector<int>({2}));

    // Test case 7: Large numbers not Fibonacci
    std::vector<int> a7 = {111111111, 12345, 112457};
    int size7 = removeFibonacciLike(a7);
    assert(size7 == 3);
    assert(a7 == std::vector<int>({111111111, 12345, 112457}));

    // Test case 8: Empty input
    std::vector<int> a8 = {};
    int size8 = removeFibonacciLike(a8);
    assert(size8 == 0);
    assert(a8.empty());

    std::cout << "All tests passed!\n";
    return 0;
}

#include <string>
#include <vector>

// Removes all Fibonacci-like numbers from the array and returns the new size.
// The modified array is returned via the reference parameter arr.
int removeFibonacciLike(std::vector<int>& arr) {
    int n = arr.size();
    int newSize = 0;
    for (int i = 0; i < n; ++i) {
        int num = arr[i];
        std::string s = std::to_string(num);
        bool isFib = false;
        if (s.length() >= 3 && s[0] == '1' && s[1] == '1') {
            isFib = true;
            for (int j = 2; j < s.length(); ++j) {
                int prev1 = s[j - 1] - '0';
                int prev2 = s[j - 2] - '0';
                int cur = s[j] - '0';
                if (cur != prev1 + prev2) {
                    isFib = false;
                    break;
                }
            }
        }
        if (!isFib) {
            arr[newSize] = arr[i];
            ++newSize;
        }
    }
    arr.resize(newSize);
    return newSize;
}

// The main idea is to iterate through the array, for each element check if it satisfies the Fibonacci-like digit property. Convert the number to a string to easily access digits. A number qualifies if it has at least 3 digits, starts with "11", and for every position `j` from 2 onward, the digit equals the sum of the two previous digits. If it qualifies, we delete that element by shifting all subsequent elements left by one position. Since deletion changes the array length, we must use a loop that does not skip elements after a deletion (e.g., decrement the index after removal). The new logical size is tracked separately. Edge cases include numbers with fewer than 3 digits, negative numbers (though task says positive, but handle by treating sign), and duplicate values. Time complexity: O(n * d) where d is the maximum number of digits (≤10 for typical 32-bit ints), so O(n). Space: O(1) extra, aside from the string conversion. We must ensure we do not go out of bounds when shifting, and that we process each element exactly once.
