Write a C++ function that takes an array of integers and its size, and returns the sum of the second largest element from even-indexed positions (0, 2, 4, ...) and the second smallest element from odd-indexed positions (1, 3, 5, ...). If the array has 3 or fewer elements, return 0. Assume all elements are unique, and treat the 0th position as an even position.

The solution separates elements into two groups based on index parity. Even-indexed values are collected into one vector, odd-indexed values into another. After sorting both vectors, the second largest even-position value is the second-to-last element of the even vector, and the second smallest odd-position value is the second element (index 1) of the odd vector. The function returns their sum. Edge cases: if the array length is 3 or less, return 0 immediately. For valid lengths, both vectors will have at least two elements because when n≥4, even positions contain at least 2 elements (0 and 2) and odd positions contain at least 2 elements (1 and 3). Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the two vectors.

#include <vector>
#include <algorithm>

int LargeSmallSum(const std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 3) {
        return 0;
    }
    
    std::vector<int> evenPositions, oddPositions;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            evenPositions.push_back(arr[i]);
        } else {
            oddPositions.push_back(arr[i]);
        }
    }
    
    std::sort(evenPositions.begin(), evenPositions.end());
    std::sort(oddPositions.begin(), oddPositions.end());
    
    int secondLargestEven = evenPositions[evenPositions.size() - 2];
    int secondSmallestOdd = oddPositions[1];
    
    return secondLargestEven + secondSmallestOdd;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above

int main() {
    // Example from the problem statement
    std::vector<int> arr1 = {3, 2, 1, 7, 5, 4};
    assert(LargeSmallSum(arr1) == 7); // even: {3,1,5} second largest=3, odd: {2,7,4} second smallest=4 -> 3+4=7

    // Array with 3 elements returns 0
    std::vector<int> arr2 = {1, 2, 3};
    assert(LargeSmallSum(arr2) == 0);

    // Array with exactly 4 elements
    std::vector<int> arr3 = {10, 20, 30, 40};
    // even: {10, 30} second largest=10, odd: {20, 40} second smallest=20 -> 30
    assert(LargeSmallSum(arr3) == 30);

    // Array with 5 elements
    std::vector<int> arr4 = {5, 1, 9, 8, 3};
    // even: {5, 9, 3} sorted {3,5,9} second largest=5, odd: {1,8} sorted {1,8} second smallest=8 -> 13
    assert(LargeSmallSum(arr4) == 13);

    // Array with 6 elements, descending order
    std::vector<int> arr5 = {6, 5, 4, 3, 2, 1};
    // even: {6,4,2} sorted {2,4,6} second largest=4, odd: {5,3,1} sorted {1,3,5} second smallest=3 -> 7
    assert(LargeSmallSum(arr5) == 7);

    // Large array to test size handling
    std::vector<int> arr6 = {100, 1, 99, 2, 98, 3};
    // even: {100,99,98} sorted {98,99,100} second largest=99, odd: {1,2,3} sorted {1,2,3} second smallest=2 -> 101
    assert(LargeSmallSum(arr6) == 101);

    // Array with 2 elements returns 0
    std::vector<int> arr7 = {1, 2};
    assert(LargeSmallSum(arr7) == 0);

    // Array with 1 element returns 0
    std::vector<int> arr8 = {10};
    assert(LargeSmallSum(arr8) == 0);

    // Empty array returns 0
    std::vector<int> arr9;
    assert(LargeSmallSum(arr9) == 0);

    // Array with 3 elements but large values
    std::vector<int> arr10 = {1000, 2000, 3000};
    assert(LargeSmallSum(arr10) == 0);

    return 0;
}
