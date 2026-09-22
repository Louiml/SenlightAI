// Write a C++ function named `updateAndDisplayAmount` that takes an initial amount (an integer) and returns a string describing the amount before and after an increment, similar to the provided snippet's logic. Specifically, the function should accept an integer `startingAmount` and an integer `increment` (both non-negative, but the function should handle any integer values gracefully). It should create a local variable holding `startingAmount`, append to a result string `"Jumlah uang semula:"` followed by that value, then add `increment` to the amount, and finally append `"Jumlah uang sekarang: "` followed by the new value. The function should return the complete concatenated string. You must not use any external libraries beyond `<string>` and `<iostream>` (the latter only for the test harness if needed, not in the solution function). Ensure the function is `const`-correct where applicable and uses descriptive variable names.

The solution is straightforward: start by initializing a local integer variable with the parameter `startingAmount`. Build a string incrementally using `std::to_string(int)` to convert integers to strings. Append the first label and amount, then add the `increment` to the local amount (so the original parameter remains unchanged, preserving const-correctness by taking parameters by value). Append the second label and the updated amount. Return the final string. Edge cases: negative amounts or negative increments are handled naturally since integer addition and string conversion support negatives; zero and single-digit numbers work the same. Time complexity is O(1) because there are only a fixed number of string concatenations and integer operations. Space complexity is O(length of the output string) due to the returned string, which is proportional to the number of digits in the numbers, effectively O(log10(max(|a|,|b|))) but treated as O(1) for typical sizes.

#include <string>

// Returns a string describing the amount before and after adding increment.
std::string updateAndDisplayAmount(int startingAmount, int increment) {
    int currentAmount = startingAmount;
    std::string result = "Jumlah uang semula:" + std::to_string(currentAmount);
    
    currentAmount += increment;
    result += "\nJumlah uang sekarang: " + std::to_string(currentAmount);
    
    return result;
}

#include <cassert>
#include <string>

// Solution function declaration (or include the header if separate)
std::string updateAndDisplayAmount(int startingAmount, int increment);

int main() {
    // Basic case from snippet
    assert(updateAndDisplayAmount(1000, 200) == "Jumlah uang semula:1000\nJumlah uang sekarang: 1200");
    
    // Zero increment
    assert(updateAndDisplayAmount(500, 0) == "Jumlah uang semula:500\nJumlah uang sekarang: 500");
    
    // Negative start and increment
    assert(updateAndDisplayAmount(-10, -5) == "Jumlah uang semula:-10\nJumlah uang sekarang: -15");
    
    // Negative increment leading to smaller value
    assert(updateAndDisplayAmount(50, -20) == "Jumlah uang semula:50\nJumlah uang sekarang: 30");
    
    // Large numbers
    assert(updateAndDisplayAmount(2147483647, 1) == "Jumlah uang semula:2147483647\nJumlah uang sekarang: 2147483648");
    
    // Single-digit numbers
    assert(updateAndDisplayAmount(7, 3) == "Jumlah uang semula:7\nJumlah uang sekarang: 10");
}
