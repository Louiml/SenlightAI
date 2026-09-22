// Write a C++ function named `findSafePrime` that generates random 256-bit positive integers using `boost::multiprecision::cpp_int` and cryptographic-quality randomness, tests each for primality using the Miller–Rabin test with 25 rounds, and for each probable prime `n`, also tests whether `(n-1)/2` is prime. The function should return the first safe prime `n` it finds (where both `n` and `(n-1)/2` are probable primes), or throw a `std::runtime_error` if none is found within 100,000 attempts. Use distinct random number generators for number generation and primality testing to avoid correlation. Return the safe prime as `cpp_int`. The function must be self-contained, include necessary Boost headers, and be `const`-correct.
The solution generates random 256-bit integers using an `independent_bits_engine` wrapped around a base generator (`mt11213b`) seeded with the current time. To avoid false positives, a separate `mt19937` generator is used for the Miller–Rabin tests. For each generated integer `n`, we apply `miller_rabin_test(n, 25, gen2)`; if it returns true, we then test `(n-1)/2` similarly. The first `n` that passes both tests is returned. Edge cases: we must ensure `n` is positive and sufficiently large (since 256-bit generation ensures this), and we must handle the case where `(n-1)/2` is zero (but for 256-bit numbers, this is impossible). The search is bounded to 100,000 iterations; if no safe prime is found, throw an exception. Time complexity: each Miller–Rabin test is `O(k * log^3 n)` for `k` rounds, and here `log n` ≈ 256; total is at most 100,000 such tests, but in practice a safe prime appears much sooner. Space complexity is `O(1)` beyond the numbers themselves.
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random.hpp>
#include <stdexcept>
#include <ctime>

// Finds and returns a 256-bit safe prime (n such that n and (n-1)/2 are probable primes).
// Throws std::runtime_error if no safe prime is found within 100,000 attempts.
boost::multiprecision::cpp_int findSafePrime()
{
    using namespace boost::multiprecision;
    using namespace boost::random;

    typedef cpp_int int_type;
    mt11213b base_gen(static_cast<unsigned>(std::time(nullptr)));
    independent_bits_engine<mt11213b, 256, int_type> gen(base_gen);
    mt19937 gen2(static_cast<unsigned>(std::time(nullptr)));

    const unsigned MAX_ATTEMPTS = 100000;
    const unsigned TRIALS = 25;

    for (unsigned i = 0; i < MAX_ATTEMPTS; ++i)
    {
        int_type n = gen();  // n is positive and 256-bit
        if (miller_rabin_test(n, TRIALS, gen2))
        {
            int_type half = (n - 1) / 2;
            if (miller_rabin_test(half, TRIALS, gen2))
            {
                return n;
            }
        }
    }

    throw std::runtime_error("No safe prime found within 100,000 attempts.");
}
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random.hpp>
#include <iostream>
#include <ctime>

boost::multiprecision::cpp_int findSafePrime(); // declaration from solution

int main()
{
    using boost::multiprecision::cpp_int;
    using boost::multiprecision::miller_rabin_test;
    using boost::random::mt19937;
    using boost::random::mt11213b;
    using boost::random::independent_bits_engine;

    // Test 1: function returns a value (not throws) — safe prime found.
    cpp_int safe_prime = findSafePrime();
    mt19937 test_gen(static_cast<unsigned>(std::time(nullptr)));
    assert(safe_prime > 0);
    assert(miller_rabin_test(safe_prime, 25, test_gen));
    assert(miller_rabin_test((safe_prime - 1) / 2, 25, test_gen));

    // Test 2: check that the returned number is 256-bit (fits within 256 bits).
    assert(safe_prime < (cpp_int(1) << 256));

    // Test 3: every 256-bit number generated may be even, but safe primes are odd.
    assert((safe_prime % 2) == 1);

    // Test 4: (n-1)/2 must be an integer and prime.
    assert(((safe_prime - 1) % 2) == 0);

    // Test 5: For the found safe prime, (n-1)/2 is less than n.
    assert((safe_prime - 1) / 2 < safe_prime);

    // Test 6: call again to ensure reproducibility (different seeds may give different safe primes, but still valid).
    cpp_int another = findSafePrime();
    assert(miller_rabin_test(another, 25, test_gen));
    assert(miller_rabin_test((another - 1) / 2, 25, test_gen));

    std::cout << "All tests passed. Safe prime: " << safe_prime << std::endl;
    return 0;
}
