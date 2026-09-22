Write a C++ function `long long maximumProductOfDigits(const std::string& n)` that takes a non-empty string `n` representing a positive integer (no leading zeros) and returns the maximum possible product of the digits of any number that can be obtained by decreasing exactly one digit of `n` by 1 (if that digit is > 0). For example, given "123", possible decreases are "023" (product 0), "113" (product 3), "122" (product 4), so the maximum is 4. Given "100", the only valid decrease is "090" (product 0) because the first digit is 1 and decreasing gives 0, but the resulting number would have a leading zero, but we still consider its digit product as 0 (ignore leading zeros for the product, treat them as digit 0). If no digit can be decreased (all digits are 0, but a positive integer cannot be all zeros), handle edge cases: if the input is "1", decreasing gives "0", product is 0. Return the maximum product as a `long long`. Assume `n` has at least one digit and contains only digits 0-9, with the first digit nonzero.
#include <cassert>
#include <string>

// forward declaration of the function being tested
long long maximumProductOfDigits(const std::string& n);

int main() {
    assert(maximumProductOfDigits("123") == 4);      // "122" gives 4
    assert(maximumProductOfDigits("1") == 0);        // "0" gives 0
    assert(maximumProductOfDigits("100") == 0);      // "000" gives 0
    assert(maximumProductOfDigits("999") == 8 * 9 * 9); // 8*81=648 from "899" or "989" or "998" all product 648
    assert(maximumProductOfDigits("234") == 2 * 2 * 4); // "224" gives 16, "133" gives 3, "233" gives 18? Wait: 234: decrease 2->1: 134 product 12; decrease 3->2: 224 product 16; decrease 4->3: 233 product 18. So max 18. But check: 2*3*3 = 18, so assert 18.
    assert(maximumProductOfDigits("234") == 18);
    assert(maximumProductOfDigits("111") == 0);      // "011" product 0, "101" product 0, "110" product 0
    assert(maximumProductOfDigits("710") == 0);      // 610 product 0, 700 product 0, 709? wait decrease 0 not allowed, decrease 1->0: "700" product 0, decrease 7->6: "610" product 0. Max 0.
    assert(maximumProductOfDigits("987") == 8 * 8 * 7); // decrease first: 887 product 448, decrease second: 977 product 441, decrease third: 986 product 432, so 448.
    return 0;
}
#include <string>
#include <algorithm>
#include <cctype>

// Given a string representing a positive integer, returns the maximum possible
// product of digits after decreasing exactly one digit (that is > 0) by 1.
// If no digit can be decreased (all zeros except possibly first but first is nonzero), 
// the function still produces at least one candidate (e.g., decreasing the first digit).
long long maximumProductOfDigits(const std::string& n) {
    long long best = 0;
    const int len = static_cast<int>(n.size());

    for (int i = 0; i < len; ++i) {
        if (n[i] == '0') continue;  // cannot decrease a zero digit

        // Create a modified string with digit i decreased by 1
        std::string candidate = n;
        candidate[i] = static_cast<char>(candidate[i] - 1);

        // Compute product of digits in candidate (including zeros)
        long long product = 1;
        for (char ch : candidate) {
            product *= static_cast<long long>(ch - '0');
        }

        if (product > best) {
            best = product;
        }
    }

    return best;
}
// The main idea is to try decreasing each digit that is greater than 0 by 1, compute the product of all digits of the resulting string, and track the maximum. Because the input length is at most, say, 18 digits (to fit in long long for product), we can iterate over all positions. For each position i, if s[i] > '0', create a copy, decrement that character by 1, then iterate over all characters of the copy, multiply their integer values (including zeros, since leading zeros are fine for product computation). Keep the maximum product. Edge cases: if the string is "1", the only candidate is "0", product 0, return 0. If string is all zeros except first digit (e.g., "100"), decreasing first digit gives "000", product 0; decreasing any other digit (0) is not allowed, so max is 0. The time complexity is O(L^2) where L is length of string, because for each of L positions we O(L) multiply. Space O(L) for copies. For typical constraints L ≤ 19, this is fine.
