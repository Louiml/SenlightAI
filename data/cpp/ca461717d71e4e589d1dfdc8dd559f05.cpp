Write a C++ function named `countEvenNumbers` that takes a constant reference to a `std::vector<int>` and returns the number of even integers in that vector. The function must handle empty vectors gracefully (returning 0), and it should treat zero as an even number. The implementation should use a range-based for loop for clarity and efficiency, and it must not modify the input vector. The function signature must be `int countEvenNumbers(const std::vector<int>& values);`. You are only required to write the function; a test harness will be provided separately, but your solution must compile and pass all given test cases without relying on any external libraries beyond the C++ standard library.
The solution approach is straightforward: iterate through each element of the vector and increment a counter whenever an element is divisible by 2 (i.e., `value % 2 == 0`). Important edge cases include an empty vector (the loop simply runs zero times, returning 0) and negative even numbers (e.g., `-4` is even because `-4 % 2 == 0`). Zero is also even and will be counted. The range-based for loop avoids index management and is safer for vectors. Time complexity is O(n), where n is the number of elements, because each element is examined exactly once. Space complexity is O(1), as only a single integer accumulator is used. No special cases for overflow arise because we only count, not sum, and the count itself fits comfortably in an `int` for any reasonable vector size (though for extremely large vectors beyond `INT_MAX`, the type would need to be larger, but that is unlikely in a typical exercise).
#include <vector>

// Count the number of even integers in a vector.
// Returns 0 for an empty vector. Zero is considered even.
int countEvenNumbers(const std::vector<int>& values) {
    int evenCount = 0;
    for (int value : values) {
        if (value % 2 == 0) {
            ++evenCount;
        }
    }
    return evenCount;
}
#include <cassert>
#include <vector>

// Declaration of the function to be tested (matches the solution).
int countEvenNumbers(const std::vector<int>& values);

int main() {
    // Test with a mixed vector.
    assert(countEvenNumbers({1, 2, 3, 4, 5, 6}) == 3);
    // Test with an empty vector.
    assert(countEvenNumbers({}) == 0);
    // Test with all odd numbers.
    assert(countEvenNumbers({1, 3, 5, 7}) == 0);
    // Test with all even numbers, including zero and negatives.
    assert(countEvenNumbers({0, -2, 4, -6, 8}) == 5);
    // Test with a single even number.
    assert(countEvenNumbers({10}) == 1);
    // Test with a single odd number.
    assert(countEvenNumbers({7}) == 0);
    // Test with negative numbers only.
    assert(countEvenNumbers({-1, -2, -3, -4}) == 2);
    // Test with duplicate values.
    assert(countEvenNumbers({2, 2, 2, 3}) == 3);
    // Test with large number of elements (1000 evens out of 1000).
    std::vector<int> large(1000, 2);
    assert(countEvenNumbers(large) == 1000);
    // Test with large number of elements (500 evens out of 1000).
    std::vector<int> mixed;
    for (int i = 0; i < 1000; ++i) {
        mixed.push_back(i);
    }
    assert(countEvenNumbers(mixed) == 500);
    return 0;
}
