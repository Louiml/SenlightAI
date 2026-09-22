Write a standalone C++ function named `printMultiplesOfFive` that takes an integer `start` and an integer `end` as parameters and returns a `std::string` containing all integers from `start` (inclusive) up to `end` (inclusive) that are multiples of 5, each on a new line, with no trailing newline at the end of the string. If `start` is greater than `end`, the function should return an empty string. The function must handle any integer range including negative numbers, zero, and positive numbers. The output format must exactly match the pattern produced by the original snippet (which printed 25, 30, 35, ..., 100), but generalized for any start and end values. If no multiples of 5 exist in the given range, return an empty string. Do not print anything to the console inside the function; only return the constructed string.

#include <cassert>
#include <string>

// The function being tested. In a real test, include the solution header.
std::string printMultiplesOfFive(int start, int end);

int main() {
    // Original snippet behavior: 25 to 100
    assert(printMultiplesOfFive(25, 100) == "25\n30\n35\n40\n45\n50\n55\n60\n65\n70\n75\n80\n85\n90\n95\n100");

    // Partial range not starting on a multiple
    assert(printMultiplesOfFive(26, 35) == "30\n35");

    // Negative start and end
    assert(printMultiplesOfFive(-10, -5) == "-10\n-5");

    // Crossing zero
    assert(printMultiplesOfFive(-7, 7) == "-5\n0\n5");

    // Single multiple
    assert(printMultiplesOfFive(5, 5) == "5");
    assert(printMultiplesOfFive(4, 6) == "5");

    // No multiples in range
    assert(printMultiplesOfFive(1, 4) == "");

    // start > end
    assert(printMultiplesOfFive(10, 5) == "");

    // Large range with negative and positive
    assert(printMultiplesOfFive(-15, 15) == "-15\n-10\n-5\n0\n5\n10\n15");

    // Range with only one multiple
    assert(printMultiplesOfFive(-8, -6) == "");

    return 0;
}

#include <string>

// Return a string of all multiples of 5 in [start, end], each on a new line.
// Returns an empty string if start > end or no multiples exist.
std::string printMultiplesOfFive(int start, int end) {
    if (start > end) {
        return "";
    }

    // Compute the first multiple of 5 that is >= start.
    int remainder = start % 5;
    if (remainder < 0) {
        remainder += 5;
    }
    int first = start + (5 - remainder) % 5;

    if (first > end) {
        return "";
    }

    std::string result;
    bool firstAdded = false;
    for (int value = first; value <= end; value += 5) {
        if (firstAdded) {
            result += "\n";
        }
        result += std::to_string(value);
        firstAdded = true;
    }

    return result;
}

// The solution must generate a string containing every number in the closed interval `[start, end]` that is divisible by 5, separated by newlines. The main challenge is handling negative ranges and ensuring the first multiple of 5 is correctly identified. A simple loop from `start` to `end` with a modulo check would work, but it could be inefficient if the range is very large (though still O(n) in the worst case). A more efficient approach is to compute the first multiple of 5 that is >= `start` using integer arithmetic: `first = ((start + 4) / 5) * 5` for positive numbers, but this formula breaks for negatives. A robust way is to use modular arithmetic: `remainder = start % 5; if (remainder < 0) remainder += 5; first = start + (5 - remainder) % 5;` This ensures `first` is the smallest multiple of 5 >= `start`. Then iterate from `first` to `end` in steps of 5, building the string. Edge cases: if `start > end`, return empty string. If `end` is less than `first`, no multiples exist. Negative numbers work fine because the modular adjustment handles them. The output must have no trailing newline, so we add newlines between numbers only. Time complexity is O(k) where k is the number of multiples in the range (which is at most (end-start)/5 + 1), and space complexity is O(k) for the output string.
