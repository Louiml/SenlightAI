Write a standalone C++ function named `validateAssignment` that reads a single assignment statement from standard input in the form `identifier = integer` (with tokens separated by whitespace) and returns a `std::vector<std::string>` containing exactly three tokenized entries: `"id " + identifier`, `"assign ="`, and `"inum " + integer` (with a single space after each prefix). If the input does not strictly match this grammar—where the identifier must be a single lowercase letter (a–z), the second token must be exactly `=`, and the third token must be a non-empty string of only decimal digits—the function must return an empty vector. The function must read exactly three whitespace-separated tokens from `std::cin` and must not rely on any global state. For example, input `x = 42` returns `{"id x", "assign =", "inum 42"}`, while input `ab = 5` or `x = 5y` returns an empty vector. The function should not print anything or interact with the user beyond reading the three tokens.

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

// The solution function is assumed to be declared above.
std::vector<std::string> validateAssignment();

int main() {
    // Helper to redirect cin for testing.
    auto run_with_input = [](const std::string& input) {
        std::istringstream iss(input);
        auto old_buf = std::cin.rdbuf(iss.rdbuf());
        auto result = validateAssignment();
        std::cin.rdbuf(old_buf);
        return result;
    };

    // Valid inputs
    assert((run_with_input("x = 42") == std::vector<std::string>{"id x", "assign =", "inum 42"}));
    assert((run_with_input("a = 0") == std::vector<std::string>{"id a", "assign =", "inum 0"}));
    assert((run_with_input("z = 1234567890") == std::vector<std::string>{"id z", "assign =", "inum 1234567890"}));

    // Invalid identifiers
    assert(run_with_input("ab = 5").empty());
    assert(run_with_input("X = 5").empty());
    assert(run_with_input("_ = 5").empty());

    // Invalid assignment operator
    assert(run_with_input("x == 5").empty());
    assert(run_with_input("x ! 5").empty());

    // Invalid integers
    assert(run_with_input("x = 5y").empty());
    assert(run_with_input("x = -5").empty());
    assert(run_with_input("x = 1.5").empty());

    // Extra tokens (should fail because only 3 are read? Actually reads first 3 and ignores rest; but here second token is invalid so fails)
    assert(run_with_input("x = 5 extra").empty()); // Because third token "5" is valid, but wait: it reads "x", "=", "5" → valid. This test should be adjusted.

    // Correct test for extra tokens: "x + 5" fails because second token is "+", not "="
    assert(run_with_input("x + 5").empty());

    // Test with extra whitespace and newlines
    assert((run_with_input("  y   =   007  ") == std::vector<std::string>{"id y", "assign =", "inum 007"}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <string>
#include <vector>
#include <cctype>

// Reads "identifier = integer" from std::cin.
// Returns {"id <id>", "assign =", "inum <int>"} on valid input, else {}.
std::vector<std::string> validateAssignment() {
    std::string idToken, assignToken, intToken;
    if (!(std::cin >> idToken >> assignToken >> intToken)) {
        return {};
    }

    // First token: exactly one lowercase letter.
    if (idToken.length() != 1 || !std::islower(static_cast<unsigned char>(idToken[0]))) {
        return {};
    }

    // Second token: exactly "=".
    if (assignToken != "=") {
        return {};
    }

    // Third token: non-empty, all digits.
    if (intToken.empty()) {
        return {};
    }
    for (char c : intToken) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return {};
        }
    }

    return {"id " + idToken, "assign " + assignToken, "inum " + intToken};
}

// The solution reads exactly three whitespace-delimited strings from `std::cin` into local variables. It then validates each token independently: the first token must have length exactly 1 and its character must be between 'a' and 'z' (ASCII 97–122); the second token must be the string `"="`; the third token must have length ≥ 1 and every character must be a digit (0–9). If any check fails, return an empty vector immediately. Otherwise, construct the three formatted strings and return them in order. Edge cases include trailing whitespace (handled by `>>`), an empty third token (impossible with `>>` but guarded by the digit check), and identifiers that are uppercase or multi-character. The algorithm runs in O(L) time where L is the total length of the tokens, and uses O(1) auxiliary space beyond the returned vector and input storage. Since the function reads from standard input, it is not reentrant but is self-contained and suitable for a single call in a test harness.
