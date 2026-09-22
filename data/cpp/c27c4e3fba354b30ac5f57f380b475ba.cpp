Write a C++ function `long long sumOfPalindromicSquareSums(int limit)` that finds all integers that can be expressed as the sum of squares of consecutive positive integers \(i^2 + (i+1)^2 + \dots + j^2\) (with at least two terms, so \(j \ge i+1\)), where the resulting sum is a palindrome (reads the same forward and backward), and the sum does not exceed the given `limit`. The function must return the sum of all such distinct palindromic sums. For example, if `limit` is 100, the valid palindromic sums are 5 (from \(1^2+2^2\)), 55 (from \(1^2+2^2+3^2+4^2+5^2\)), and 77 (from \(4^2+5^2+6^2\)); all ≤ 100, so return `5+55+77=137`. Handle duplicates (e.g., the same sum from different ranges) only once. The function should be efficient enough for `limit` up to \(10^8\) (do not iterate to `limit` itself, but iterate over start indices and end indices with early termination when the sum exceeds the limit).
#include <cassert>

int main() {
    // Basic small cases
    assert(sumOfPalindromicSquareSums(5) == 5);          // only 1^2+2^2=5
    assert(sumOfPalindromicSquareSums(55) == 5 + 55);    // 5 and 1^2+...+5^2=55
    assert(sumOfPalindromicSquareSums(77) == 5 + 55 + 77); // includes 4^2+5^2+6^2=77
    assert(sumOfPalindromicSquareSums(100) == 5 + 55 + 77);
    
    // Duplicate handling: sum 5050 is not palindrome, but sum 1001 is not possible from squares. Test a case where same sum appears twice.
    // 1^2+2^2+...+11^2 = 506 (not palindrome), but 5^2+6^2+7^2+8^2 = 174 (not palindrome).
    // Instead check that a known duplicate-like case with limit 1000 gives same as set.
    long long result1000 = sumOfPalindromicSquareSums(1000);
    assert(result1000 > 0);
    
    // Edge case: limit too small to have any valid pair
    assert(sumOfPalindromicSquareSums(4) == 0);
    assert(sumOfPalindromicSquareSums(1) == 0);
    assert(sumOfPalindromicSquareSums(2) == 0);
    
    // Larger limit to ensure early termination works
    assert(sumOfPalindromicSquareSums(1000000) > 0);
    
    return 0;
}
#include <set>
#include <string>

// Returns the sum of all distinct palindromic sums of consecutive squares (with at least two terms) that are <= limit.
long long sumOfPalindromicSquareSums(int limit) {
    std::set<long long> palindromicSums;
    
    // Outer loop over starting index i
    for (long long i = 1; ; ++i) {
        // The smallest sum for this i is i^2 + (i+1)^2
        long long minSum = i * i + (i + 1) * (i + 1);
        if (minSum > limit) break;
        
        long long currentSum = 0;
        // Inner loop over ending index j
        for (long long j = i; ; ++j) {
            currentSum += j * j;
            // We need at least two terms, so skip when j == i (only one term)
            if (j == i) continue;
            if (currentSum > limit) break;
            
            // Check if currentSum is a palindrome
            std::string s = std::to_string(currentSum);
            std::string rev(s.rbegin(), s.rend());
            if (s == rev) {
                palindromicSums.insert(currentSum);
            }
        }
    }
    
    long long total = 0;
    for (long long value : palindromicSums) {
        total += value;
    }
    return total;
}
// The solution iterates over every possible starting index `i` from 1 upward. For a fixed `i`, it then increments the ending index `j` starting at `i+1`, adding the square `j*j` to a running sum. The running sum for fixed `i` grows monotonically with `j`, so we stop the inner loop as soon as the sum exceeds `limit`. Also, we can break the outer loop when the smallest possible sum for a given `i` (i.e., `i^2 + (i+1)^2`) already exceeds `limit`, because any larger `i` will only produce larger sums. For each valid sum, we check if it’s a palindrome (convert to string and compare with its reverse, or use digit extraction). If yes, insert it into a `std::set` to automatically handle duplicates. Finally, sum all elements in the set. Complexity is roughly \(O(\sqrt{limit})\) outer iterations and \(O(\sqrt{limit})\) inner iterations per outer, leading to \(O(limit)\) total operations in the worst case, but with early termination. The palindrome check costs \(O(\log_{10}(sum))\) per candidate. Memory usage is \(O(\text{number of distinct palindromic sums})\), which is small in practice.
