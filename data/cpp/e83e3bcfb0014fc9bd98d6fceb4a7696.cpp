/*
Write a C++ function that finds the smallest odd square spiral side length for which the ratio of primes along the two diagonals falls below 10%. The spiral is constructed by starting with the number 1 at the center and moving in a clockwise spiral outward, placing consecutive integers in a square grid. The diagonals consist of the four corner numbers of each layer. Given the requirement that the ratio (number of primes on both diagonals) / (total numbers on both diagonals) must be strictly less than 0.1, return the side length of the square when this condition is first met. The function should take no arguments and return an integer type. You may assume a helper function to test primality is available, but you must implement it yourself in your solution. The spiral starts with a 1×1 square containing only the number 1 (which is not prime), and the side length grows by 2 each layer.
*/
#include <cstdint>

// Helper function to test primality of a positive integer.
bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

// Returns the side length of the spiral when the diagonal prime ratio < 10%.
uint_fast32_t find_spiral_sidelength() {
    uint64_t current_number = 9;      // The largest number in the current layer (side 3)
    uint64_t layer_increment = 2;     // Step size between corners in the current layer
    uint64_t prime_count = 3;         // Primes among diagonal numbers for side 3: 3,5,7
    uint32_t total_count = 5;         // Total diagonal numbers for side 3: 1,3,5,7,9
    
    // Continue until ratio is strictly less than 0.1
    while ((static_cast<double>(prime_count) / total_count) >= 0.1) {
        layer_increment += 2;         // Move to the next layer
        // Process the first three corners of this new layer
        for (uint_fast32_t i = 0; i < 3; ++i) {
            current_number += layer_increment;
            if (is_prime(current_number)) ++prime_count;
        }
        current_number += layer_increment; // Fourth corner (bottom-right) is never prime except edge case
        total_count += 4;
    }
    return static_cast<uint_fast32_t>(layer_increment + 1);
}
#include <cassert>

int main() {
    // The known answer from Project Euler 58 is 26241.
    assert(find_spiral_sidelength() == 26241);
    
    // Additional sanity checks based on intermediate states (not required by function but validate logic).
    // We can only check the final result reliably, but the function is deterministic.
    // A quick direct check that the returned value is odd and greater than 1.
    uint_fast32_t result = find_spiral_sidelength();
    assert((result % 2) == 1);
    assert(result > 1);
    
    // Test that our primality helper works correctly
    assert(is_prime(2));
    assert(is_prime(3));
    assert(is_prime(5));
    assert(!is_prime(1));
    assert(!is_prime(4));
    assert(!is_prime(9));
    assert(is_prime(7));
    
    return 0;
}
// The spiral is built layer by layer, where each layer adds a new ring of numbers. The side length of an n×n square is odd, and the corners of each layer are \(n^2 - 3(n-1), n^2 - 2(n-1), n^2 - (n-1), n^2\) (for n ≥ 3). We start with the first layer (side length 1) with total diagonal numbers = 1 and primes = 0. Then for each subsequent layer, we increase the side length by 2 and process the four new corner numbers. For each corner, we test if it is prime and update the prime count. We also increase the total diagonal count by 4 (since the center diagonal number is already counted, and each new layer adds four new corners). We stop when the ratio is strictly less than 0.1. This is exactly the logic from the given snippet. Edge cases: ensure the initial layer (side length 1) is handled correctly, and that the first side length returned is 7 (since for side 3 the ratio is 3/5 = 0.6, side 5 ratio is 5/9 ≈ 0.555, side 7 ratio is 8/13 ≈ 0.615? Actually the classic Project Euler problem 58 answer is 26241). The algorithm runs in \(O(L \cdot \sqrt{n})\) time for the primality checks, where L is the final side length, and uses \(O(1)\) space. The primality test should be efficient for numbers up to the square of the final side length.
