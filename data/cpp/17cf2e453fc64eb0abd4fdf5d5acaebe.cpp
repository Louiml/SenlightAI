/*
Write a C++ function named `beautifulPermutation` that takes a positive integer `n` and returns a `std::string` containing a permutation of the numbers from 1 to `n` such that no two consecutive elements in the permutation differ by exactly 1. If no such permutation exists for the given `n`, return the string `"NO SOLUTION"`. For any valid `n`, output the numbers separated by spaces in a single line. The permutation must include every integer from 1 to `n` exactly once. This is a classic constructive permutation problem where adjacency of consecutive integers must be avoided.
*/
#include <string>

// Constructs a permutation of 1..n where no two adjacent numbers differ by 1.
// Returns "NO SOLUTION" if impossible (n == 2 or n == 3).
std::string beautifulPermutation(int n) {
    if (n == 2 || n == 3) {
        return "NO SOLUTION";
    }
    std::string result;
    // Append all even numbers from 2 up to n
    for (int i = 2; i <= n; i += 2) {
        result += std::to_string(i) + " ";
    }
    // Append all odd numbers from 1 up to n
    for (int i = 1; i <= n; i += 2) {
        result += std::to_string(i);
        if (i + 2 <= n) {
            result += " ";
        }
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above (include its code here for the test)

int main() {
    // n=1 -> trivial single element
    assert(beautifulPermutation(1) == "1");
    // n=2 and n=3 are impossible
    assert(beautifulPermutation(2) == "NO SOLUTION");
    assert(beautifulPermutation(3) == "NO SOLUTION");
    // n=4 -> evens 2,4 then odds 1,3: difference 4-1=3, 2-1=1? wait, check: 2 4 1 3 -> 4 and 1 diff 3, 2 and 4 diff 2, 4 and 1 diff 3, 1 and 3 diff 2 -> valid
    assert(beautifulPermutation(4) == "2 4 1 3");
    // n=5 -> 2 4 1 3 5 -> check adjacents: 2-4 diff2, 4-1 diff3, 1-3 diff2, 3-5 diff2 -> valid
    assert(beautifulPermutation(5) == "2 4 1 3 5");
    // n=6 -> 2 4 6 1 3 5 -> 6-1 diff5, others even/odd diff2 -> valid
    assert(beautifulPermutation(6) == "2 4 6 1 3 5");
    // n=7 -> 2 4 6 1 3 5 7 -> last diff 5-7 diff2, 6-1 diff5 -> valid
    assert(beautifulPermutation(7) == "2 4 6 1 3 5 7");
    // n=10 -> check manually that no adjacent diff is 1
    std::string s = beautifulPermutation(10);
    // Simple verification: no adjacent numbers differ by exactly 1
    // We'll just check the string contains all numbers 1..10 and length is correct
    // (A full parse would be done in a real test, here we just assert key parts)
    assert(s == "2 4 6 8 10 1 3 5 7 9");
    // n=100 -> just ensure it doesn't crash and starts correctly
    std::string large = beautifulPermutation(100);
    assert(large.find("2 4") == 0);
    assert(large.find("100 1") != std::string::npos);
}
// The problem requires constructing a permutation of `1..n` where no adjacent pair has absolute difference equal to 1. The key observation is that if we separate all even numbers from all odd numbers, within each parity group the difference between any two numbers is at least 2, so they are safe. However, the boundary between the last even and the first odd (or vice versa) might cause a difference of 1. For example, if we list evens first: 2,4,6,... then odds: 1,3,5,..., the transition from largest even to smallest odd is `largest_even` and `1`. The difference is `largest_even - 1`, which equals 1 only when `largest_even = 2`. That happens when `n=2` or `n=3`. For `n=2`, the only permutations are `[1,2]` or `[2,1]`, both have adjacent difference 1. For `n=3`, permutations like `[2,1,3]` have 2-1 difference 1, `[1,3,2]` has 3-2 difference 1, all fail. For `n=1`, the single element trivially works. For all `n >= 4`, the arrangement "all evens ascending then all odds ascending" works because the last even is at least 4, and first odd is 1, difference ≥3, and within each group differences are ≥2. Also, putting odds first then evens also works for `n >= 4`. The time complexity is O(n) because we iterate to output each number once. The space complexity is O(1) auxiliary, ignoring the output string which is O(n) by necessity.
