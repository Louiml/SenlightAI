// Write a C++ function named `classifyNumbers` that reads exactly five integers from standard input (one per prompt) and returns a `std::string` reporting the counts of positive, negative, and zero values in the format `"Positivos: X - Negativos: Y - Ceros: Z"`, where X, Y, and Z are the respective counts. The function must handle any mix of positive, negative, and zero values, and the order of the counts must match the given format exactly, with a space before and after each hyphen. The function must not use global variables and must be self-contained, using only the standard library.
The solution uses three integer counters initialized to zero: `positiveCount`, `negativeCount`, and `zeroCount`. For each of five iterations, the function prompts the user with `"Ingrese #: "`, reads an integer into a variable, and increments the appropriate counter based on whether the value is greater than zero, less than zero, or equal to zero. After processing all five inputs, it constructs the result string using `std::to_string` for each count and concatenates them with the required labels and separators. Edge cases include all zeros, all positives, all negatives, or any mixture; each case is handled correctly because the conditionals are mutually exclusive. The algorithm runs in \(O(1)\) time since the input size is fixed at five numbers, and uses \(O(1)\) auxiliary space for the counters and the resulting string. No special handling for invalid input is required because the problem assumes valid integer input.
#include <string>
#include <iostream>

// Reads five integers from standard input and returns counts of positives, negatives, and zeros.
std::string classifyNumbers() {
    int positiveCount = 0;
    int negativeCount = 0;
    int zeroCount = 0;

    for (int i = 0; i < 5; ++i) {
        int number;
        std::cout << "Ingrese #: ";
        std::cin >> number;

        if (number > 0) {
            ++positiveCount;
        } else if (number < 0) {
            ++negativeCount;
        } else {
            ++zeroCount;
        }
    }

    return "Positivos: " + std::to_string(positiveCount) +
           " - Negativos: " + std::to_string(negativeCount) +
           " - Ceros: " + std::to_string(zeroCount);
}
#include <cassert>
#include <sstream>
#include <iostream>

// The solution function is declared here; in a real test, include the header.
// For this test, the function is defined above. We simulate input using std::istringstream.
// Since the function reads from std::cin, we redirect std::cin for testing.

#include <string>

std::string classifyNumbers(); // Assume the solution is available.

int main() {
    // Helper to run the function with simulated input.
    auto runWithInput = [](const std::string& input) {
        std::istringstream iss(input);
        std::streambuf* oldCin = std::cin.rdbuf(iss.rdbuf());
        std::string result = classifyNumbers();
        std::cin.rdbuf(oldCin);
        return result;
    };

    assert(runWithInput("1\n2\n3\n4\n5\n") == "Positivos: 5 - Negativos: 0 - Ceros: 0");
    assert(runWithInput("-1\n-2\n-3\n-4\n-5\n") == "Positivos: 0 - Negativos: 5 - Ceros: 0");
    assert(runWithInput("0\n0\n0\n0\n0\n") == "Positivos: 0 - Negativos: 0 - Ceros: 5");
    assert(runWithInput("1\n-1\n0\n2\n-2\n") == "Positivos: 2 - Negativos: 2 - Ceros: 1");
    assert(runWithInput("0\n1\n-1\n0\n0\n") == "Positivos: 1 - Negativos: 1 - Ceros: 3");
    assert(runWithInput("10\n-10\n20\n-20\n30\n") == "Positivos: 3 - Negativos: 2 - Ceros: 0");
    assert(runWithInput("-7\n8\n-9\n10\n0\n") == "Positivos: 2 - Negativos: 2 - Ceros: 1");
    assert(runWithInput("5\n5\n5\n5\n5\n") == "Positivos: 5 - Negativos: 0 - Ceros: 0");
    assert(runWithInput("-5\n-5\n-5\n-5\n-5\n") == "Positivos: 0 - Negativos: 5 - Ceros: 0");
    assert(runWithInput("0\n1\n2\n-3\n-4\n") == "Positivos: 2 - Negativos: 2 - Ceros: 1");

    return 0;
}
