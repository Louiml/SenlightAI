// Write a C++ function template named `selectionSort` that sorts an array of any comparable type in ascending order using the selection sort algorithm. The function must accept a pointer to the first element of the array and an integer `n` representing the number of elements. It must work for built-in types (e.g., `int`, `double`) and custom classes that overload `operator<`. The function should modify the array in place and return `void`. Additionally, write a separate function template named `printArray` that prints the elements of the array to `std::cout` separated by spaces. The solution must not use any standard sorting functions, and must handle edge cases such as `n = 0` and `n = 1` gracefully (doing nothing). Provide a `main` function that tests the sorting with an integer array, a double array, and a small struct with custom comparison, verifying correctness with `assert` statements.
The selection sort algorithm works by repeatedly finding the minimum element from the unsorted part of the array and swapping it with the first unsorted element. For each position `i` from 0 to `n-1`, we scan the subarray from `i+1` to `n-1` to find the index of the minimum element, then swap it with the element at `i`. This guarantees that after `i` iterations, the first `i` elements are sorted. Edge cases: when `n` is 0 or 1, the loops simply do nothing, so the function safely returns without modifying the array. For duplicate values, the algorithm still works correctly because it always picks the first occurrence of the minimum. The time complexity is \(O(n^2)\) in all cases (best, average, worst) because the inner loop always scans the remaining unsorted portion, and the outer loop runs `n` times. Space complexity is \(O(1)\) since we only use a few temporary variables and perform swaps in place. For the custom struct, we must define `operator<` to enable comparison; the template works seamlessly as long as `operator<` is available. The `printArray` function iterates through the array and outputs each element separated by spaces, followed by a newline.
#include <iostream>
#include <utility> // for std::swap
#include <string>
#include <vector>

// Sorts an array of type T in ascending order using selection sort.
// arr: pointer to the first element, n: number of elements.
template<typename T>
void selectionSort(T* arr, int n) {
    for (int i = 0; i < n; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

// Prints the elements of an array of type T separated by spaces.
template<typename T>
void printArray(const T* arr, int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i];
        if (i != n - 1) std::cout << " ";
    }
    std::cout << std::endl;
}
#include <cassert>
#include <string>
#include <vector>

// Simple struct for testing custom types
struct Person {
    std::string name;
    int age;
    // Define operator< for comparing two Person objects
    bool operator<(const Person& other) const {
        return age < other.age;
    }
};

int main() {
    // Test 1: Integer array
    int intArr[] = {5, 2, 9, 1, 5, 6};
    int n1 = sizeof(intArr) / sizeof(int);
    selectionSort(intArr, n1);
    for (int i = 0; i < n1 - 1; ++i) assert(intArr[i] <= intArr[i + 1]);
    assert(intArr[0] == 1 && intArr[n1 - 1] == 9);

    // Test 2: Double array
    double doubleArr[] = {3.5, -1.2, 0.0, 7.8, 2.2};
    int n2 = sizeof(doubleArr) / sizeof(double);
    selectionSort(doubleArr, n2);
    for (int i = 0; i < n2 - 1; ++i) assert(doubleArr[i] <= doubleArr[i + 1]);
    assert(doubleArr[0] == -1.2 && doubleArr[n2 - 1] == 7.8);

    // Test 3: Edge case - empty array (size 0)
    int* emptyArr = nullptr;
    selectionSort(emptyArr, 0); // Should do nothing without crashing

    // Test 4: Edge case - single element
    double singleArr[] = {42.0};
    selectionSort(singleArr, 1);
    assert(singleArr[0] == 42.0);

    // Test 5: Custom struct with operator<
    Person people[] = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"David", 28}};
    int n3 = sizeof(people) / sizeof(Person);
    selectionSort(people, n3);
    for (int i = 0; i < n3 - 1; ++i) assert(people[i] < people[i + 1] || !(people[i + 1] < people[i]));
    assert(people[0].age == 25 && people[n3 - 1].age == 35);

    // Test 6: Array with duplicates
    int dupArr[] = {7, 7, 7, 7};
    selectionSort(dupArr, 4);
    for (int i = 0; i < 3; ++i) assert(dupArr[i] == 7 && dupArr[i + 1] == 7);

    // Test 7: Already sorted array
    int sortedArr[] = {1, 2, 3, 4, 5};
    selectionSort(sortedArr, 5);
    for (int i = 0; i < 4; ++i) assert(sortedArr[i] < sortedArr[i + 1]);

    // Test 8: Reverse sorted array
    int revArr[] = {5, 4, 3, 2, 1};
    selectionSort(revArr, 5);
    for (int i = 0; i < 4; ++i) assert(revArr[i] < revArr[i + 1]);

    // Test 9: Large array (sanity check, not fully verified but no crash)
    std::vector<int> large(1000);
    for (int i = 0; i < 1000; ++i) large[i] = 1000 - i;
    selectionSort(large.data(), 1000);
    for (int i = 0; i < 999; ++i) assert(large[i] <= large[i + 1]);

    // All tests passed
    return 0;
}
