/*
Write a C++ function `int longestCommonPrefixLength(const std::vector<int>& arr1, const std::vector<int>& arr2)` that returns the length of the longest common prefix (i.e., the longest sequence of leading decimal digits) shared by any number from `arr1` and any number from `arr2`. For example, the numbers 1234 and 1239 share the prefix "123" (length 3), while 567 and 56 share "56" (length 2). The prefix must start at the most significant digit of both numbers. If no common prefix exists, return 0. Numbers are positive integers (≥1), arrays can be empty, and you may assume the result fits in an `int`.
*/
#include <unordered_set>
#include <vector>
#include <string>

// Returns the length of the longest common decimal prefix between any number
// from arr1 and any number from arr2. Returns 0 if none exists.
int longestCommonPrefixLength(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::unordered_set<int> prefixes;
    
    // Insert all possible prefixes of numbers in arr1
    for (int num : arr1) {
        while (num > 0) {
            prefixes.insert(num);
            num /= 10;
        }
    }
    
    int maxLength = 0;
    
    // For each number in arr2, check its prefixes from longest to shortest
    for (int num : arr2) {
        while (num > 0) {
            if (prefixes.find(num) != prefixes.end()) {
                int length = std::to_string(num).size();
                if (length > maxLength) {
                    maxLength = length;
                }
                break; // No need to check shorter prefixes for this number
            }
            num /= 10;
        }
    }
    
    return maxLength;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(longestCommonPrefixLength({1, 10, 100}, {1000, 12}) == 2); // "10" shared with 1000/12? Actually 10 and 1000 -> "10" length 2, 10 and 1? no, 12 and 1 -> "1" length 1, so max is 2
    assert(longestCommonPrefixLength({123, 456}, {1234, 67}) == 3);  // 123 and 1234 share "123"
    assert(longestCommonPrefixLength({5, 55, 555}, {5555}) == 3);    // 555 and 5555 share "555"
    
    // No common prefix
    assert(longestCommonPrefixLength({1, 2, 3}, {4, 5, 6}) == 0);
    assert(longestCommonPrefixLength({}, {1, 2}) == 0);
    
    // Single digit prefix
    assert(longestCommonPrefixLength({7}, {71}) == 1);
    
    // Same numbers
    assert(longestCommonPrefixLength({1234}, {1234}) == 4);
    
    // One number is prefix of another
    assert(longestCommonPrefixLength({12}, {12345}) == 2);
    
    // Large numbers
    assert(longestCommonPrefixLength({987654321}, {987654322}) == 8); // "98765432" 
    
    return 0;
}
// The solution leverages a hash set to store all possible prefixes of numbers from `arr1`. For each number in `arr1`, repeatedly divide by 10 (integer division) and insert the current value into the set until the number becomes 0. This captures every prefix: e.g., for 1234, insert 1234, 123, 12, 1. Then for each number in `arr2`, similarly strip digits from the right (divide by 10) and check if the current value exists in the set. The first match encountered (which is the longest prefix for that particular number, since we go from longest to shortest) is used to update the global maximum. The process stops for that number once a match is found. Edge cases: empty arrays (return 0), numbers with only one digit (prefix length 1 if shared), and numbers that share no digits (return 0). Time complexity is O((N+M)·D) where D is the maximum number of digits (≤10 for 32-bit int), effectively O(N+M). Space complexity is O(N·D) for the hash set, worst-case O(N·10) which simplifies to O(N).
