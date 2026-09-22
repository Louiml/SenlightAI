// Write a C++ function `bool canBePalindromeByLeadingZeros(int N)` that determines whether the decimal representation of a non‑negative integer `N` can be turned into a palindrome by prepending some number (possibly zero) of leading `0` digits. For example, `N = 1210` is possible because prepending one zero gives `"01210"`, which is a palindrome. The function must handle the full range `0 ≤ N ≤ 10⁹` and return `true` if such a palindrome is possible, `false` otherwise. The function should not read from standard input or print; it should only perform the check.

The key observation is that leading zeros only appear at the beginning of the string. If we can add some leading zeros to make the entire string a palindrome, then the existing digits of the string must already "match" from the outside in, except possibly for a prefix of zeros that pairs with trailing zeros or with nothing. More concretely, we can simulate the check directly on the decimal string `s = to_string(N)`. Let `l = 0` and `r = s.size()-1`. We can allow adding zeros only on the left, which effectively means we can extend the left side with extra zeros, but we cannot remove any digits. A valid configuration requires that for any pair `(i, j)` of positions in the final string, the characters match. If we add `k` leading zeros, then positions `0..k-1` are `'0'`, and positions `k..k+len(s)-1` are `s`. For the string to be a palindrome, we need:  
- All added leading zeros must match the corresponding trailing characters if they exceed the length of `s`; but since `N ≤ 10⁹` has at most 10 digits, the number of added zeros can be large, up to `10⁹` in theory but we only care about matching within the length of `s` because any extra zeros beyond that would need corresponding zeros at the end, which would require the original string to be all zeros.  
- A simpler approach: We can "force" matching by walking from the two ends of `s`. Let `l` start at 0, `r` at `len-1`. While `l < r`: if `s[l] == s[r]`, move both inward. If they differ, we can only add a zero on the left to match `s[r]` if `s[r] == '0'`. But adding a zero on the left means we are effectively inserting a `'0'` at the front, which will shift all positions of `s` right by one; however, for the palindrome check, the matching is symmetric, so we can simulate by allowing `l` to stay (since the added zero is at position `l` now) and checking if `'0' == s[r]`; if so, we move `r` left and let `l` stay (because the added zero is already matched). But we must ensure that we don't add more zeros than allowed—there is no upper bound, so as long as the rightmost unmatched character is `'0'`, we can always add a zero to match it. The only impossible case is when `s[r] != '0'` and `s[l] != s[r]` cannot be fixed. Also, if after processing we have `l == r` (odd length), any remaining single middle character can be matched by adding zeros on the left until it becomes `'0'`? Actually, if the middle character is `'0'`, it's already fine; if it's non-zero, we could add zeros to the left to place a `'0'` in the middle? But adding zeros shifts the entire original string right, so the middle position might be occupied by a different character. The correct and simple algorithm:  

Convert `N` to string `s`. Let `left = 0`, `right = s.size()-1`. While `left < right`: if `s[left] == s[right]`, increment `left`, decrement `right`. Else, if `s[right] == '0'`, then we can add a zero to the left to match this rightmost zero; this effectively lets `right--` (the right zero is matched) while `left` stays (because the added zero is at the left, but we haven't yet matched `s[left]`). However, we must also ensure that after adding zeros, the left side of the original string doesn't become misaligned. The standard trick is: we can "skip" a leading zero on the left side instead, because we are allowed to prepend zeros, but those zeros are outside the original string. Actually, the correct simulation is: Instead of adding zeros physically, we can check whether the original string `s` is a palindrome after possibly removing some leading zeros? No, we add zeros. The correct and robust method: The only way to make a palindrome by adding leading zeros is that the original string must be of the form: `[some prefix of zeros]` + `[palindromic core]` + `[some suffix of zeros]` where the number of leading zeros we add must be at least the length of the trailing zeros to balance, but in fact we can add as many as we want. The condition simplifies to: after stripping all trailing zeros from `s`, the remaining prefix (which includes leading zeros of the original number, e.g., `N=1210` gives string `"1210"` which has no leading zeros in the representation but we can add zeros) must be a palindrome. Wait, that's not right either. For `1210`, strip trailing zeros → `"121"` which is a palindrome. For `100` → strip trailing zeros → `"1"` which is palindrome → yes, add two zeros to left? Actually `"100"` → prepend two zeros → `"00100"` which is palindrome. For `120` → strip trailing zeros → `"12"` which is not palindrome → but can we prepend zeros? Let's check: `"120"` → possible? Try: add one zero → `"0120"` not palindrome. add two zeros → `"00120"` not. add three zeros → `"000120"`? not. It seems impossible because the `2` and `1` are not symmetric. So the condition is: after removing all trailing zeros from `s`, the remaining string must be a palindrome. Why? Because any trailing zeros in `s` can be matched by an equal number of leading zeros we add. After that, the middle part must be symmetric. And we can always add enough zeros to match all trailing zeros, because we can add as many as needed. So the algorithm: let `r = s.size()-1; while (r >=0 && s[r]=='0') r--;` now consider substring `s[0..r]` (if `r<0` all zeros → always possible). Then check if that substring is a palindrome. Why does this work? Because any trailing zeros can be paired with leading zeros we add, and the remaining interior must be a palindrome. Also note that leading zeros in the original representation of `N` are not present (since `N` is given as an int, `to_string` doesn't include them), but that's fine—we can add as many zeros as we want. So the condition is simply: the string after stripping all trailing zeros must be a palindrome. Edge cases: `N=0` → string `"0"`, strip trailing zeros → empty substring, which is trivially palindrome → output Yes. `N=10` → `"10"`, strip trailing zeros → `"1"` palindrome → Yes (add one zero → `"010"` palindrome). `N=101` → strip trailing zeros → `"101"` palindrome → Yes (add zero zeros). `N=1001` → strip trailing zeros → `"1001"` palindrome → Yes. `N=12` → strip → `"12"` not palindrome → No. Time complexity: O(length of decimal string) ≤ O(10) because N ≤ 10⁹ has at most 10 digits, so effectively O(1). Space O(1) aside from string.

#include <string>

// Determines if N can be turned into a palindrome by adding some number of leading zeros.
bool canBePalindromeByLeadingZeros(int N) {
    std::string s = std::to_string(N);
    
    // Remove all trailing zeros from the string representation.
    int r = static_cast<int>(s.size()) - 1;
    while (r >= 0 && s[r] == '0') {
        --r;
    }
    
    // If all digits are zeros, an empty core is trivially a palindrome.
    if (r < 0) return true;
    
    // Check if the remaining substring s[0..r] is a palindrome.
    int l = 0;
    while (l < r) {
        if (s[l] != s[r]) return false;
        ++l;
        --r;
    }
    return true;
}

#include <cassert>

int main() {
    assert(canBePalindromeByLeadingZeros(0) == true);
    assert(canBePalindromeByLeadingZeros(5) == true);
    assert(canBePalindromeByLeadingZeros(1210) == true);
    assert(canBePalindromeByLeadingZeros(1000) == true);
    assert(canBePalindromeByLeadingZeros(101) == true);
    assert(canBePalindromeByLeadingZeros(10) == true);
    assert(canBePalindromeByLeadingZeros(120) == false);
    assert(canBePalindromeByLeadingZeros(12) == false);
    assert(canBePalindromeByLeadingZeros(100) == true);
    assert(canBePalindromeByLeadingZeros(12321) == true);
}
