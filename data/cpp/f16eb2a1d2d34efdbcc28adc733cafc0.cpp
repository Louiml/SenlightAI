// Write a C++ function named `fizzBuzz` that takes a positive integer `n` and returns a `std::vector<std::string>` containing the FizzBuzz sequence from 1 up to and including `n`. For each integer `i` from 1 to `n`, the corresponding string should be `"Fizz"` if `i` is divisible by 3, `"Buzz"` if divisible by 5, `"FizzBuzz"` if divisible by both 3 and 5, and otherwise the decimal representation of `i` as a string. The order of the output must match the increasing order of `i`. The function should remain valid for `n = 0`, returning an empty vector, and must handle large `n` (up to at least 10^5) without performance issues. Use `const` for parameters that should not be modified, and ensure the function signature is `std::vector<std::string> fizzBuzz(int n)` (note: `n` is passed by value, so no need for `const` qualification on the parameter itself, but you may use `const int n` if you prefer; avoid modifying `n` inside).

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; here we test it.
int main() {
    // n = 1
    std::vector<std::string> r1 = fizzBuzz(1);
    assert(r1.size() == 1);
    assert(r1[0] == "1");

    // n = 5, covers pure Fizz and pure Buzz
    std::vector<std::string> r2 = fizzBuzz(5);
    assert(r2.size() == 5);
    assert(r2[0] == "1");
    assert(r2[1] == "2");
    assert(r2[2] == "Fizz");
    assert(r2[3] == "4");
    assert(r2[4] == "Buzz");

    // n = 15, covers FizzBuzz at 15
    std::vector<std::string> r3 = fizzBuzz(15);
    assert(r3.size() == 15);
    assert(r3[2] == "Fizz");
    assert(r3[4] == "Buzz");
    assert(r3[5] == "Fizz");
    assert(r3[9] == "Buzz");
    assert(r3[10] == "11");
    assert(r3[14] == "FizzBuzz");

    // n = 0, empty vector
    std::vector<std::string> r4 = fizzBuzz(0);
    assert(r4.empty());

    // n = 100, spot-check pattern
    std::vector<std::string> r5 = fizzBuzz(100);
    assert(r5.size() == 100);
    assert(r5[2] == "Fizz");
    assert(r5[4] == "Buzz");
    assert(r5[14] == "FizzBuzz");
    assert(r5[29] == "FizzBuzz");
    assert(r5[98] == "Fizz");
    assert(r5[99] == "Buzz");

    return 0;
}

#include <string>
#include <vector>

// Return the FizzBuzz sequence from 1 to n as a vector of strings.
std::vector<std::string> fizzBuzz(int n) {
    std::vector<std::string> result;
    result.reserve(n);  // Pre-allocate to avoid reallocations

    for (int i = 1; i <= n; ++i) {
        std::string current;
        if (i % 3 == 0) {
            current += "Fizz";
        }
        if (i % 5 == 0) {
            current += "Buzz";
        }
        if (current.empty()) {
            current = std::to_string(i);
        }
        result.push_back(current);
    }

    return result;
}

// The solution is a straightforward single-pass loop from 1 to `n`. For each iteration, we build a string by conditionally appending `"Fizz"` if the current index is divisible by 3, then appending `"Buzz"` if divisible by 5. If the string remains empty (neither condition is true, i.e., the number is not divisible by 3 or 5), we convert the integer to a string using `std::to_string`. This approach naturally handles the case where the number is divisible by both 3 and 5, because both conditions are checked independently and the pieces are concatenated. Edge cases: `n = 0` yields an empty vector because the loop does not execute; `n = 1` yields `{"1"}`; values like 3, 5, and 15 are correctly handled by the modular checks. The time complexity is O(n) because we iterate exactly `n` times, and each iteration does constant work (a couple of modulo operations, maybe a string concatenation, and a push_back). The space complexity is O(n) for the output vector storing `n` strings, plus O(1) auxiliary space for the temporary string. The solution is robust and does not require any special data structures.
