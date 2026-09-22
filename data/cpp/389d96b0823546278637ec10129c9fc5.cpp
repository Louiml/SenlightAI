/*
Write a C++ function named `printEvenNumbers` that takes a positive integer `n` as input and returns a `std::string` containing all even numbers from 2 up to `n` (inclusive), separated by single spaces. If `n` is less than 2, the function should return an empty string. The function must use a `do-while` loop for the iteration logic (to match the spirit of the provided snippet). The output string should have no trailing spaces. The function should be `const`-correct and operate on a value parameter (since it only reads it).
*/
#include <string>

// Returns a string of even numbers from 2 to n (inclusive), separated by spaces.
// Uses a do-while loop. Returns an empty string if n < 2.
std::string printEvenNumbers(int n) {
    if (n < 2) {
        return std::string(); // No even numbers exist
    }

    std::string result;
    int i = 2;

    do {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(i);
        i += 2;
    } while (i <= n);

    return result;
}
#include <cassert>
#include <string>

// Declaration of the function (for the test, assume it's defined above or in a header)
std::string printEvenNumbers(int n);

int main() {
    // Basic cases
    assert(printEvenNumbers(1) == "");
    assert(printEvenNumbers(2) == "2");
    assert(printEvenNumbers(3) == "2");
    assert(printEvenNumbers(4) == "2 4");
    assert(printEvenNumbers(5) == "2 4");
    assert(printEvenNumbers(6) == "2 4 6");
    assert(printEvenNumbers(7) == "2 4 6");
    assert(printEvenNumbers(8) == "2 4 6 8");
    
    // Larger case
    assert(printEvenNumbers(10) == "2 4 6 8 10");
    
    // Edge case: n = 0 (though input says positive, test defensive)
    assert(printEvenNumbers(0) == "");

    return 0;
}
// The algorithm initializes an integer counter `i` to 2 (the first even number) and a `std::string` result. Since we need to use a `do-while` loop, we must handle the edge case where `n < 2` separately; if `n < 2`, there are no even numbers to print, so we return an empty string immediately. Otherwise, we enter the `do-while` loop, where we append the current `i` to the result (adding a space before each number after the first to avoid leading/trailing spaces), then increment `i` by 2. The loop continues while `i <= n`. After the loop, the result string contains the even numbers separated by single spaces. The time complexity is O(n/2) = O(n) because we iterate through roughly half the numbers from 2 to n. The space complexity is O(n) because we build a result string proportional to the number of even numbers (worst case when n is large, but practically bounded by the output length). Edge cases include `n = 1` (returns empty), `n = 2` (returns "2"), and `n = 0` (returns empty). The function correctly handles any positive integer, and for `n` less than 2, the early return avoids the `do-while` executing accidentally (since a `do-while` always runs at least once).
