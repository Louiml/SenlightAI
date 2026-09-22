/*
Write a C++ function named `fizzBuzzValues` that takes an integer `n` (where `0 <= n <= 10^4`) and returns a `std::vector<std::string>` containing the FizzBuzz sequence from 1 to `n` inclusive. For each integer `i` from 1 to `n`, the corresponding entry should be `"FizzBuzz"` if `i` is divisible by both 3 and 5, `"Fizz"` if divisible by 3 only, `"Buzz"` if divisible by 5 only, and otherwise the decimal representation of `i` as a string. The function must handle the edge case `n = 0` by returning an empty vector, and must be `const`-correct with respect to its input (the input is a simple integer, so no `const` qualifier on it is needed, but the function itself can be marked `const` if desired). Do not include a `main` function.
*/

#include <vector>
#include <string>

// Generate the FizzBuzz sequence from 1 to n inclusive.
// Returns a vector of strings where:
// - "FizzBuzz" if the number is divisible by both 3 and 5
// - "Fizz" if divisible by 3 only
// - "Buzz" if divisible by 5 only
// - Otherwise the number as a decimal string.
std::vector<std::string> fizzBuzzValues(int n) {
    std::vector<std::string> result;
    result.reserve(n); // Optional: preallocate for efficiency

    for (int i = 1; i <= n; ++i) {
        if (i % 3 == 0 && i % 5 == 0) {
            result.push_back("FizzBuzz");
        } else if (i % 3 == 0) {
            result.push_back("Fizz");
        } else if (i % 5 == 0) {
            result.push_back("Buzz");
        } else {
            result.push_back(std::to_string(i));
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declare the function from the solution.
std::vector<std::string> fizzBuzzValues(int n);

int main() {
    // Test n = 0: empty vector
    assert(fizzBuzzValues(0) == std::vector<std::string>{});

    // Test n = 1: single non-divisible number
    assert(fizzBuzzValues(1) == std::vector<std::string>{"1"});

    // Test small sequence up to 5
    assert(fizzBuzzValues(5) == std::vector<std::string>{"1", "2", "Fizz", "4", "Buzz"});

    // Test n = 15: first time all types appear
    assert(fizzBuzzValues(15) == std::vector<std::string>{
        "1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz",
        "11", "Fizz", "13", "14", "FizzBuzz"
    });

    // Test n = 20: check that FizzBuzz appears again at 15 and 30 is out of range
    assert(fizzBuzzValues(20).back() == "Buzz");

    // Test n = 100: verify the count matches and certain indices
    auto result100 = fizzBuzzValues(100);
    assert(result100.size() == 100);
    assert(result100[2] == "Fizz");
    assert(result100[4] == "Buzz");
    assert(result100[14] == "FizzBuzz");
    assert(result100[99] == "Buzz"); // 100 is divisible by 5 only

    return 0;
}

// The algorithm is a straightforward linear pass over the integers from 1 to `n`. For each integer `i`, we check divisibility by both 3 and 5 first (or equivalently by 15), then by 3 alone, then by 5 alone, and finally default to converting `i` to a string using `std::to_string`. The order of checks is important because if `i` is divisible by both 3 and 5, we must output `"FizzBuzz"` rather than `"Fizz"` or `"Buzz"`. Key edge cases include: `n = 0` (loop never runs, return empty vector), `i = 1` (not divisible by 3 or 5, output `"1"`), and multiples of 15 (output `"FizzBuzz"`). Time complexity is O(n) because we iterate exactly `n` times and each iteration does constant work. Space complexity is O(n) for the output vector that stores `n` strings. No auxiliary data structures beyond the output vector are needed, so auxiliary space is O(1) aside from the output.
