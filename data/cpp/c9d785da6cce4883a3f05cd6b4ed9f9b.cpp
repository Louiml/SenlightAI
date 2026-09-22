// Write a C++ function that reads a non-negative integer from standard input, then prints its decimal digits in forward order (most significant to least significant) and then in reverse order (least significant to most significant), one digit per line. The function must work for any non-negative integer, including 0 (print a single 0 in both orders), and must not use arrays, strings, or recursion. Your solution should be robust for numbers up to at least 2,000,000,000 (fits in a 32-bit signed int). The function should not return a value; it should perform all input and output directly using `std::cin` and `std::cout`.

#include <cassert>
#include <iostream>
#include <sstream>

void test_printDigitsForwardAndReverse() {
    // Redirect cin and cout for testing
    auto cinbuf = std::cin.rdbuf();
    auto coutbuf = std::cout.rdbuf();
    std::istringstream input;
    std::ostringstream output;

    // Test 1: simple number
    input.str("123");
    std::cin.rdbuf(input.rdbuf());
    output.str("");
    std::cout.rdbuf(output.rdbuf());
    printDigitsForwardAndReverse();
    assert(output.str() == "1\n2\n3\n3\n2\n1\n");

    // Test 2: zero
    input.str("0");
    std::cin.rdbuf(input.rdbuf());
    output.str("");
    std::cout.rdbuf(output.rdbuf());
    printDigitsForwardAndReverse();
    assert(output.str() == "0\n0\n");

    // Test 3: number with trailing zeros
    input.str("1000");
    std::cin.rdbuf(input.rdbuf());
    output.str("");
    std::cout.rdbuf(output.rdbuf());
    printDigitsForwardAndReverse();
    assert(output.str() == "1\n0\n0\n0\n0\n0\n0\n1\n");

    // Test 4: single digit
    input.str("7");
    std::cin.rdbuf(input.rdbuf());
    output.str("");
    std::cout.rdbuf(output.rdbuf());
    printDigitsForwardAndReverse();
    assert(output.str() == "7\n7\n");

    // Test 5: large number
    input.str("987654321");
    std::cin.rdbuf(input.rdbuf());
    output.str("");
    std::cout.rdbuf(output.rdbuf());
    printDigitsForwardAndReverse();
    assert(output.str() == "9\n8\n7\n6\n5\n4\n3\n2\n1\n1\n2\n3\n4\n5\n6\n7\n8\n9\n");

    // Restore buffers
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
}

int main() {
    test_printDigitsForwardAndReverse();
    return 0;
}

#include <iostream>

// Read a non-negative integer from standard input and print its digits
// in forward order, then in reverse order, one per line.
void printDigitsForwardAndReverse() {
    int n;
    std::cin >> n;

    if (n == 0) {
        std::cout << "0\n0\n";
        return;
    }

    // Find the highest power of 10 <= n
    int divisor = 1;
    while (divisor <= n / 10) {
        divisor *= 10;
    }

    // Forward order
    int remaining = n;
    for (int d = divisor; d > 0; d /= 10) {
        int digit = (remaining / d) % 10;
        std::cout << digit << "\n";
    }

    // Reverse order
    int rev = n;
    while (rev > 0) {
        std::cout << rev % 10 << "\n";
        rev /= 10;
    }
}

// The main challenge is printing digits in forward order without storing them. Since reading all digits into an array is not allowed, and we process digits from least significant to most significant when extracting them via `%10`, we need a way to produce them in reverse. A standard technique is to first count the number of digits (or find the highest power of 10) by repeatedly dividing a copy of the number by 10. Then, for forward order, use a divisor that starts at that highest power of 10 and iterates downward, extracting each digit via `(n / divisor) % 10`. For reverse order, simply extract with `%10` and divide by 10 until the number becomes 0. Edge cases: if the input is 0, both loops should print exactly one digit 0. For numbers with trailing zeros (e.g., 1000), the forward method using divisor logic works correctly because it uses division and modulo on the original number, and the loop condition should be based on the divisor being > 0, not the number being non-zero, to avoid skipping zeros. Time complexity is O(d) where d is the number of digits, and space complexity is O(1).
