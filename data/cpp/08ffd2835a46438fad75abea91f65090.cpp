Write a C++ function named `printMultiplesDescending` that takes two positive integers `n` and `x` and returns a `std::string` containing the first `n` positive multiples of `x` (i.e., `x*1, x*2, ..., x*n`) listed in descending order (from largest to smallest), separated by single spaces, with no leading or trailing spaces. The function must handle the case where `n` or `x` is zero or negative by returning an empty string. Assume the inputs are within the range that the multiplication `n*x` does not overflow an `int`. The function should be `const`-correct and use no global state.

#include <cassert>
#include <string>

// Assume the function is declared above or included here.
// Declare for testing:
std::string printMultiplesDescending(int n, int x);

int main() {
    // Basic case
    assert(printMultiplesDescending(4, 15) == "60 45 30 15");
    // Single multiple
    assert(printMultiplesDescending(1, 7) == "7");
    // x = 1 gives sequence 1..n descending
    assert(printMultiplesDescending(5, 1) == "5 4 3 2 1");
    // n = 5, x = 2
    assert(printMultiplesDescending(5, 2) == "10 8 6 4 2");
    // n = 0 or negative should return empty
    assert(printMultiplesDescending(0, 3) == "");
    assert(printMultiplesDescending(-3, 3) == "");
    // x = 0 or negative should return empty
    assert(printMultiplesDescending(3, 0) == "");
    assert(printMultiplesDescending(3, -1) == "");
    // Large n (but small product)
    assert(printMultiplesDescending(3, 1000) == "3000 2000 1000");
    // Check no extra spaces
    std::string s = printMultiplesDescending(2, 5);
    assert(s.front() != ' ' && s.back() != ' ');
    assert(s == "10 5");
    return 0;
}

#include <string>

// Return a string with the first n positive multiples of x in descending order.
std::string printMultiplesDescending(int n, int x) {
    if (n <= 0 || x <= 0) {
        return "";
    }

    std::string result;
    for (int i = n; i > 0; --i) {
        if (!result.empty()) {
            result += ' ';
        }
        result += std::to_string(i * x);
    }
    return result;
}

// The solution is a straightforward iteration from `n` down to `1`, computing `i * x` for each step and appending the result to a string with a space separator. Since we need descending order, starting at `i = n` and decrementing to `1` naturally produces the correct sequence. For edge cases: if `n <= 0` or `x <= 0`, we return an empty string immediately because the problem specifies positive inputs. In the loop, to avoid leading/trailing spaces, we can either append the first number without a space and then add a space before each subsequent number, or build the string and trim. A cleaner approach is to append the first value, then for each subsequent value prepend a space in the string. The time complexity is O(n) because we iterate n times, and each string append operation is amortized O(1) with respect to the total output length (since the total output length is O(n * digits of x*n), but we count string operations). The space complexity is O(n * digits of max multiple) for the string itself, but auxiliary space beyond the output is O(1). We must also consider that `n` could be large, but the problem assumes the result fits in memory. The function returns a `std::string` by value, so we use `std::to_string` for conversion.
