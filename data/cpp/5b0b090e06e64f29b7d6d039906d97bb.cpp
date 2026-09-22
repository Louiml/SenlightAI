// Write a C++ function that finds the 10,001st prime number and returns it as an `int`. The function must implement its own primality test (do not use external libraries like `euler.h`). The primality test should be efficient enough to handle numbers up to around 105,000. The solution should be self-contained, using only standard headers, and should not include a `main` function or any I/O – only the core computation function.

The key challenge is computing the 10,001st prime efficiently. Start with the first prime, 2, and then iterate through odd numbers (since all primes >2 are odd). Maintain a counter for how many primes have been found. For each candidate number, test primality by checking divisibility only up to the square root of the candidate, which dramatically reduces the number of operations compared to checking all smaller numbers. Additionally, skip even numbers entirely, halving the search space. The primality test returns false immediately for numbers ≤1, handles 2 and 3 as special cases, then checks divisibility by 2 and 3 before testing numbers of the form 6k±1 up to sqrt(n). The time complexity is O(p sqrt(p)) where p is the 10,001st prime (approximately 104,729), but in practice this runs in milliseconds because the square root grows slowly. Space complexity is O(1), as only a few scalar variables are used.

#include <cmath>

// Check if a given positive integer is prime.
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    // Check divisors of the form 6k ± 1 up to sqrt(n)
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Return the 10,001st prime number.
int tenthousand_first_prime() {
    int count = 1;      // We start with prime 2
    int candidate = 1;  // Next odd candidate to test (will become 3)
    
    while (count < 10001) {
        candidate += 2;  // Move to next odd number
        if (is_prime(candidate)) {
            ++count;
        }
    }
    return candidate;
}

#include <cassert>

// Declare the function from the solution (or include the header)
int tenthousand_first_prime();
bool is_prime(int n);

int main() {
    // Known primes for small indices
    assert(is_prime(2) == true);
    assert(is_prime(3) == true);
    assert(is_prime(4) == false);
    assert(is_prime(9) == false);
    assert(is_prime(17) == true);
    
    // First few primes: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    // We can't easily call the function for each index without modification,
    // so test the direct result for the 10,001st prime.
    // The 10,001st prime is known to be 104729.
    assert(tenthousand_first_prime() == 104729);
    
    return 0;
}
