Write a C++ function `generateFibonacci(int n)` that returns a `std::string` containing the first `n+2` Fibonacci numbers (starting with 0, 1, and then continuing) separated by single spaces, with no leading or trailing spaces. The function must handle non‑positive input gracefully by returning an empty string. You must implement the function without using `std::vector` or any container — only iterating and building the string manually. The input `n` represents how many additional Fibonacci numbers beyond `0` and `1` to include, matching the behavior of the original snippet (which prints `0`, `1`, then the next `n` numbers). For example, `generateFibonacci(5)` should return `"0 1 1 2 3 5 8"`.

#include <cassert>
#include <string>

// Function declaration to test (can be placed before main in the same file)
std::string generateFibonacci(int n);

int main() {
    assert(generateFibonacci(1) == "0 1 1");
    assert(generateFibonacci(2) == "0 1 1 2");
    assert(generateFibonacci(5) == "0 1 1 2 3 5 8");
    assert(generateFibonacci(0) == "");
    assert(generateFibonacci(-3) == "");
    assert(generateFibonacci(10) == "0 1 1 2 3 5 8 13 21 34 55 89");
    assert(generateFibonacci(3) == "0 1 1 2 3");
    assert(generateFibonacci(20) == "0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181 6765 10946");
    assert(generateFibonacci(7) == "0 1 1 2 3 5 8 13 21");
    return 0;
}

#include <string>

// Returns the first n+2 Fibonacci numbers (starting 0,1) as a space-separated string.
// Returns an empty string if n <= 0.
std::string generateFibonacci(int n) {
    if (n <= 0) {
        return "";
    }

    int a = 0;
    int b = 1;
    std::string result = "0 1";

    for (int i = 1; i <= n; ++i) {
        const int sum = a + b;
        a = b;
        b = sum;
        result += " " + std::to_string(sum);
    }

    return result;
}

// The main algorithm follows the classic iterative Fibonacci computation: start with two variables `a = 0` and `b = 1`, and append them to the output string. Then, for `i` from 1 to `n` inclusive, compute `sum = a + b`, shift `a = b` and `b = sum`, and append `sum` to the string. Each number is converted to a string using `std::to_string` and joined with spaces. Edge cases: when `n <= 0`, return an empty string — neither `0` nor `1` are included because the function’s contract states only positive `n` produce output. To avoid a trailing space, we only add a space before appending a new number if the string is not empty. Time complexity is O(n) because we iterate exactly `n` times, and each operation (addition, conversion, append) is constant time. Space complexity is O(n) for the resulting string (since the output length grows with `n`) plus O(1) for the integer variables.
