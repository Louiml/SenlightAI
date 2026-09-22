Write a C++ function that reads exactly 10 integers from standard input and returns the count of prime numbers among them. The function must prompt the user for each number with the exact text "Ingrese numero: " (without quotes) and must print the results in the exact format shown: "Hay X numeros primos" followed by a newline and then "====== FIN DEL PROGRAMA ======" (no trailing newline after that). The function should correctly handle numbers less than or equal to 1 (which are not prime) and should consider 2 as prime. The function must not use external libraries beyond standard I/O and must be implementable in a standalone program.

#include <cassert>
#include <sstream>
#include <iostream>

// Declare the solution function (it is defined in the same translation unit in a real setting)
bool isPrime(int n);
void countPrimesFromInput();

int main() {
    // Test the isPrime helper directly with various edge cases
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);
    assert(isPrime(1) == false);
    assert(isPrime(0) == false);
    assert(isPrime(-7) == false);
    assert(isPrime(17) == true);
    assert(isPrime(100) == false);
    assert(isPrime(97) == true);
    assert(isPrime(999983) == true); // a known large prime

    // Test the full function by redirecting cin and capturing cout
    std::string input = "2 3 4 5 6 7 8 9 10 11\n";
    std::istringstream in(input);
    std::streambuf* oldCin = std::cin.rdbuf(in.rdbuf());

    std::ostringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

    countPrimesFromInput();

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    std::string expected = "====== INICIO DEL PROGRAMA ======\n"
                           "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                           "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                           "Hay 5 numeros primos\n\n"
                           "====== FIN DEL PROGRAMA =====";
    assert(out.str() == expected);

    // Test with no primes (all 1s or 0s)
    std::string input2 = "1 1 1 1 1 1 1 1 1 1\n";
    std::istringstream in2(input2);
    std::cin.rdbuf(in2.rdbuf());

    std::ostringstream out2;
    std::cout.rdbuf(out2.rdbuf());

    countPrimesFromInput();

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    std::string expected2 = "====== INICIO DEL PROGRAMA ======\n"
                            "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                            "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                            "Hay 0 numeros primos\n\n"
                            "====== FIN DEL PROGRAMA =====";
    assert(out2.str() == expected2);

    // Test with all primes (small primes)
    std::string input3 = "2 3 5 7 11 13 17 19 23 29\n";
    std::istringstream in3(input3);
    std::cin.rdbuf(in3.rdbuf());

    std::ostringstream out3;
    std::cout.rdbuf(out3.rdbuf());

    countPrimesFromInput();

    std::cin.rdbuf(oldCin);
    std::cout.rdbuf(oldCout);

    std::string expected3 = "====== INICIO DEL PROGRAMA ======\n"
                            "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                            "Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: Ingrese numero: "
                            "Hay 10 numeros primos\n\n"
                            "====== FIN DEL PROGRAMA =====";
    assert(out3.str() == expected3);

    return 0;
}

#include <iostream>

// Returns true if the given number is prime, false otherwise.
// Handles numbers <= 1 as non-prime. Uses trial division up to sqrt(n).
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Reads exactly 10 integers from standard input, prompts with "Ingrese numero: ",
// counts how many are prime, and prints the result in the specified format.
void countPrimesFromInput() {
    int primeCount = 0;
    std::cout << "====== INICIO DEL PROGRAMA ======" << std::endl;
    for (int i = 0; i < 10; ++i) {
        int number;
        std::cout << "Ingrese numero: ";
        std::cin >> number;
        if (isPrime(number)) {
            ++primeCount;
        }
    }
    std::cout << "Hay " << primeCount << " numeros primos" << std::endl << std::endl;
    std::cout << "====== FIN DEL PROGRAMA ======";
}

// The main challenge is correctly identifying prime numbers. A number `n` is prime if it is greater than 1 and has no divisors other than 1 and itself. The simplest approach is to test divisibility from 2 up to `sqrt(n)` to reduce work, but a naive loop from 2 to `n-1` is also acceptable. For each of the 10 input numbers, check primality with a helper function. Edge cases: numbers ≤ 1 are not prime; 2 is prime; even numbers > 2 are not prime (can be optimized but not required). Count the primes and output the result. Time complexity: O(10 * sqrt(m)) where m is the maximum input number; space complexity O(1). The code must read exactly 10 integers, one per prompt. Ensure outputs match the exact formatting. The solution function should be self-contained and not rely on global state.
