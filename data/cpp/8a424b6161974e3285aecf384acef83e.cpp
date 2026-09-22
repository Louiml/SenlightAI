Write a C++ function that takes an array of digits `num` (each digit between 0 and 9) and its size `n`, and returns a `vector<int>` representing the smallest palindromic number greater than the given number that is strictly greater than the input number. The input is interpreted as a single integer with `num[0]` as the most significant digit. The returned vector must preserve the same number of digits as the input unless the input is all 9s, in which case the result has one more digit (e.g., input `9 9` → output `1 0 1`). The function must modify only the returned vector, not the input array (though it may copy values internally). If the input itself is already a palindrome, the function must still return the next larger palindrome. Example: input `[1,2,3]` → output `[1,3,1]`; input `[1,9,9]` → output `[2,0,2]`; input `[9,9]` → output `[1,0,1]`; input `[1,2,1]` → output `[1,3,1]`.
The core idea is to mirror the left half to the right half, which always yields a palindrome that is at least as large as the original. The only challenge is handling the case where the mirrored palindrome is not strictly greater than the input; then we need to increment the middle digit(s) and propagate any carry. The algorithm works as follows:  
1. Copy the input array into a mutable vector (or work on a copy of the input).  
2. Traverse from the outer ends toward the center, comparing `num[l]` and `num[r]`. While doing so, set `num[r] = num[l]` to form a palindrome. Track a flag: if at any point `num[l] > num[r]` (before overwriting), set `flag = 1`; if `num[l] < num[r]`, set `flag = -1`. Continue overwriting.  
3. After that, if `flag == 1`, the mirrored palindrome is already strictly greater than the original, so we can return it directly.  
4. If `flag == 0` (original was already palindrome) or `flag == -1` (mirroring produced a smaller number), we need to increment the palindrome. Start from the center index `i = (n-1)/2` and move leftward: add 1 to `num[i]`. If the digit becomes ≤9, set `num[n-1-i] = num[i]` and stop. If it becomes 10 (i.e., digit was 9), set it to 0, set its mirror to 0, and continue to the next left position.  
5. If we exhaust all positions and still need to carry (i.e., all digits were 9), the result is a number with one extra digit: a leading `1`, followed by `n` zeros in the middle, and a final `1`. Return that vector.  
Edge cases include: input with a single digit; input that is already a palindrome; input like `[9,9]` where all digits are 9; inputs with an odd number of digits where the center digit is handled specially.  
Time complexity is O(n) since we scan the array at most twice. Space complexity is O(n) for the returned vector, plus O(n) for the copy if we choose to copy the input (or we can modify a local vector, which is still O(n) auxiliary). The function should not modify the original array, so we make a copy first.
#include <vector>
#include <algorithm>

// Return the smallest palindrome strictly greater than the number represented by num[0..n-1].
std::vector<int> nextLargerPalindrome(const int num[], int n) {
    // Copy the input into a mutable vector.
    std::vector<int> digits(num, num + n);
    
    int flag = 0;
    int left = 0, right = n - 1;
    
    // Mirror the left half onto the right half, while tracking comparison.
    while (left < right) {
        if (digits[left] < digits[right]) {
            flag = -1;
        } else if (digits[left] > digits[right]) {
            flag = 1;
        }
        digits[right] = digits[left];
        ++left;
        --right;
    }
    
    // If the mirrored palindrome is already larger, return it.
    if (flag == 1) {
        return digits;
    }
    
    // Otherwise, increment the palindrome from the center outward.
    int center = (n - 1) / 2;
    while (center >= 0) {
        if (digits[center] < 9) {
            ++digits[center];
            digits[n - 1 - center] = digits[center];
            return digits;
        } else {
            digits[center] = 0;
            digits[n - 1 - center] = 0;
        }
        --center;
    }
    
    // If all digits became 0, we need an extra leading and trailing 1.
    std::vector<int> result(n + 1, 0);
    result[0] = 1;
    result[n] = 1;
    return result;
}
#include <cassert>
#include <vector>

// (Solution function is defined above; here we test it.)

int main() {
    // Basic cases
    int arr1[] = {1, 2, 3};
    assert(nextLargerPalindrome(arr1, 3) == std::vector<int>({1, 3, 1}));
    
    int arr2[] = {1, 9, 9};
    assert(nextLargerPalindrome(arr2, 3) == std::vector<int>({2, 0, 2}));
    
    // Input is already a palindrome, must return next larger
    int arr3[] = {1, 2, 1};
    assert(nextLargerPalindrome(arr3, 3) == std::vector<int>({1, 3, 1}));
    
    // All 9s (even length)
    int arr4[] = {9, 9};
    assert(nextLargerPalindrome(arr4, 2) == std::vector<int>({1, 0, 1}));
    
    // All 9s (odd length)
    int arr5[] = {9, 9, 9};
    assert(nextLargerPalindrome(arr5, 3) == std::vector<int>({1, 0, 0, 1}));
    
    // Mirroring yields smaller number, need carry propagation
    int arr6[] = {1, 2, 8};
    assert(nextLargerPalindrome(arr6, 3) == std::vector<int>({1, 3, 1}));
    
    // Single digit not 9
    int arr7[] = {7};
    assert(nextLargerPalindrome(arr7, 1) == std::vector<int>({8}));
    
    // Single digit 9
    int arr8[] = {9};
    assert(nextLargerPalindrome(arr8, 1) == std::vector<int>({1, 1}));
    
    // Larger middle carry
    int arr9[] = {1, 9, 2, 1};
    assert(nextLargerPalindrome(arr9, 4) == std::vector<int>({1, 9, 9, 1}));
    
    // Edge where left is greater
    int arr10[] = {3, 1, 2};
    assert(nextLargerPalindrome(arr10, 3) == std::vector<int>({3, 2, 3}));
    
    return 0;
}
