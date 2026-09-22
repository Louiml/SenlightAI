/*
Write a C++ function `describeMultipleOfThree` that takes an integer `n` and returns a `std::string` message according to the following rules: if `n` is divisible by 3, the function must check whether `n` is also even; if both conditions hold, return `"n is a multiple of 3 and even."` (using the exact English phrasing with `n` replaced by the actual integer value, e.g., `"9 is a multiple of 3 and even."`). If `n` is divisible by 3 but odd, return nothing (empty string). If `n` is not divisible by 3, return `"<n> is not a multiple of 3."`. The function must handle all integer values, including negative numbers, zero, and extreme large/small values. The original code snippet used `n % 3 == 0` and an inner `n % 2 == 0` check, but it produced no output in the case of `n=9` because it is odd; your function must replicate this exact semantics: only produce output for the even multiple-of-3 case or the non-multiple-of-3 case, not for odd multiples of 3.
*/

#include <string>

// Returns a message based on divisibility by 3 and parity.
// - If n is a multiple of 3 and even: returns "X is a multiple of 3 and even."
// - If n is a multiple of 3 but odd: returns empty string.
// - If n is not a multiple of 3: returns "X is not a multiple of 3."
std::string describeMultipleOfThree(int n) {
    if (n % 3 == 0) {
        if (n % 2 == 0) {
            return std::to_string(n) + " is a multiple of 3 and even.";
        }
        // Odd multiple of 3: no output.
        return "";
    }
    // Not a multiple of 3.
    return std::to_string(n) + " is not a multiple of 3.";
}

#include <cassert>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Even multiple of 3.
    assert(describeMultipleOfThree(6) == "6 is a multiple of 3 and even.");
    assert(describeMultipleOfThree(0) == "0 is a multiple of 3 and even.");
    assert(describeMultipleOfThree(-12) == "-12 is a multiple of 3 and even.");

    // Odd multiple of 3 => empty string.
    assert(describeMultipleOfThree(9) == "");
    assert(describeMultipleOfThree(-3) == "");
    assert(describeMultipleOfThree(21) == "");

    // Not a multiple of 3.
    assert(describeMultipleOfThree(1) == "1 is not a multiple of 3.");
    assert(describeMultipleOfThree(7) == "7 is not a multiple of 3.");
    assert(describeMultipleOfThree(-5) == "-5 is not a multiple of 3.");
    assert(describeMultipleOfThree(1000000) == "1000000 is not a multiple of 3.");

    return 0;
}

// The solution is straightforward: use the modulo operator to test divisibility by 3. If `n % 3 == 0`, then check `n % 2 == 0`. If even, return a formatted string using `std::to_string` to embed the integer; if odd, return an empty string. Otherwise (`n % 3 != 0`), return the not‑multiple message. Edge cases: zero is divisible by 3 and even, so it returns the “multiple and even” message. Negative numbers behave the same under modulo in C++ (e.g., `-9 % 3 == 0`, `-9 % 2 == -1`), so the odd/even check works. Very large integers that fit in `int` are handled by `std::to_string`. Time complexity is O(1) (constant number of operations), space complexity is O(1) for the function itself, excluding the returned string whose length is proportional to the number of digits in `n`.
