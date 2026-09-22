Write a C++ function that takes a positive integer `n` and returns a string containing the first two multiples of `n` that are divisible by 5, in the order they appear, separated by a space. Multiples are generated sequentially starting from `n * 1`, `n * 2`, `n * 3`, and so on. If `n` itself is divisible by 5, it counts as the first such multiple. The function should handle any positive integer input, including cases where two required multiples may be far apart. Do not include any user interaction; all processing must be inside the function, and the function should be reusable.

#include <cassert>
#include <string>

// Free function declaration (as if included from solution)
std::string firstTwoMultiplesOfFive(int n);

int main() {
    // n divisible by 5: first two multiples are n and 5n
    assert(firstTwoMultiplesOfFive(5) == "5 25");
    // n not divisible by 5 but one step away: first multiple is 5n, second is 10n
    assert(firstTwoMultiplesOfFive(4) == "20 40");
    // n = 1: multiples divisible by 5 are 5 and 10
    assert(firstTwoMultiplesOfFive(1) == "5 10");
    // n = 7: 7*5=35, 7*10=70
    assert(firstTwoMultiplesOfFive(7) == "35 70");
    // n = 10: multiples are 10 and 50
    assert(firstTwoMultiplesOfFive(10) == "10 50");
    // n = 2: 2*5=10, 2*10=20
    assert(firstTwoMultiplesOfFive(2) == "10 20");
    // n = 3: 3*5=15, 3*10=30
    assert(firstTwoMultiplesOfFive(3) == "15 30");
    // n = 6: 6*5=30, 6*10=60
    assert(firstTwoMultiplesOfFive(6) == "30 60");
    // n = 15: 15 and 75
    assert(firstTwoMultiplesOfFive(15) == "15 75");
    // n = 99: 99*5=495, 99*10=990
    assert(firstTwoMultiplesOfFive(99) == "495 990");
    return 0;
}

#include <string>

// Return a string containing the first two multiples of n that are divisible by 5.
// Multiples are generated starting from n*1, n*2, etc., and the two found are space-separated.
std::string firstTwoMultiplesOfFive(int n) {
    const int targetCount = 2;
    int found = 0;
    int multiplier = 1;
    std::string result;

    while (found < targetCount) {
        int multiple = n * multiplier;
        if (multiple % 5 == 0) {
            if (found > 0) {
                result += " ";
            }
            result += std::to_string(multiple);
            ++found;
        }
        ++multiplier;
    }

    return result;
}

// We need to generate multiples of `n` sequentially using a multiplier counter that starts at 1 and increments by 1 each iteration. For each multiple, check if it is divisible by 5 using the modulo operator `%`. We maintain a count of how many qualifying multiples have been found; once this count reaches 2, we stop. The two found multiples are concatenated into a string with a space separator. Edge cases: If `n` is already divisible by 5, the first iteration immediately qualifies, and the second candidate is found at the next multiple that is divisible by 5; since multiples of 5 occur every 5 steps in the multiplier sequence, the loop will always terminate. If `n` is not divisible by 5, the first qualifying multiple will be at some later multiplier, but because the multiplier increases by 1 each time, eventually `n * multiplier` will be divisible by 5 when `multiplier` becomes a multiple of 5 (since `n` is coprime to 5 if not divisible by 5). Thus, the loop always finds two multiples. Time complexity is O(k) where k is the multiplier value at which the second qualifying multiple is found; in the worst case, `k` is at most 10 (since if `n % 5 == 0`, first at k=1, second at k=5; if `n % 5 != 0`, the first multiple is at k = (5 - n%5) mod 5, and the second at k+5, so k ≤ 10). So O(1) effective. Space complexity is O(1) aside from the returned string.
