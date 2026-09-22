Write a C++ function named `countPrimes` that takes a `std::vector<int>` of integers and returns the number of prime numbers in the vector. The function must treat 1 and any non‑positive integer as non‑prime, and correctly identify primes up to the largest integer in the vector. The input vector may contain duplicates, negative values, and zeros; each occurrence should be counted separately. The function must not modify the input vector and must be efficient for vectors with up to 10⁶ elements and integer values up to 10⁶.
// The solution iterates over each element in the vector and tests it for primality using a helper `isPrime` function. The helper handles edge cases: numbers ≤ 1 are not prime, 2 is prime, and for any even number > 2 we can return false immediately (optimization). For odd numbers, we test divisors from 3 up to √n, stepping by 2 to halve the iterations. This is O(√n) per number, leading to a total worst‑case time of O(m · √M), where m is the vector size and M is the maximum value. Space usage is O(1) auxiliary, not counting the input vector. The main function counts each valid occurrence, including duplicates, without altering the input.
#include <vector>
#include <cmath>

// Helper: return true if n is a prime number, false otherwise.
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= std::sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Count the number of prime numbers in the given vector.
int countPrimes(const std::vector<int>& nums) {
    int count = 0;
    for (int value : nums) {
        if (isPrime(value)) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(countPrimes({2, 3, 5, 7}) == 4);
    assert(countPrimes({1, 4, 6, 8}) == 0);
    assert(countPrimes({2}) == 1);
    assert(countPrimes({}) == 0);
    // Edge cases: negatives, zero, duplicates
    assert(countPrimes({-3, 0, 1, 2, 2, 3}) == 3); // two 2s and one 3
    assert(countPrimes({-1, -2, 0, 1}) == 0);
    // Large prime and composite
    assert(countPrimes({999983, 1000000}) == 1); // 999983 is prime
    assert(countPrimes({1000000, 1000001}) == 0);
    // Mix of all types
    assert(countPrimes({-5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 4); // 2,3,5,7
    return 0;
}
