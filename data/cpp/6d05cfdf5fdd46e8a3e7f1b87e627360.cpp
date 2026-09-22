// Write a C++ function `std::string expandSequence(int N, const std::vector<char>& characters, const std::vector<int64_t>& lengths)` that takes a count `N`, a vector of characters, and a parallel vector of non-negative lengths. The function must output (return) a string obtained by repeating each character `characters[i]` exactly `lengths[i]` times, in the given order. However, if the total sum of all lengths exceeds 100, the function must immediately return the special string `"Too Long"` instead of building the expansion. The input is guaranteed to have exactly `N` elements in both vectors, and all lengths are `int64_t` values (may be zero). Edge cases: `N` can be 0 (return empty string or "Too Long" depending on sum, which is 0, so empty string), lengths can be large positive numbers causing overflow if summed naively, but the function must detect if the cumulative sum exceeds 100 as soon as possible, before any overflow occurs. The function should not read from standard input; it receives all data as parameters. The returned string must be exactly as described.

// The main algorithm is straightforward: iterate through the indices from 0 to N-1, maintaining a running sum of `lengths[i]`. Before adding each length, check if the current sum plus the next length would exceed 100. If so, return `"Too Long"` immediately without further processing. This early exit prevents overflow and avoids unnecessary work. If no overflow occurs, then after the sum check passes for all elements, build the output string by appending each character `lengths[i]` times using a loop or `std::string::append`. Important edge cases: (1) `N=0` results in an empty string because the sum is 0 (not greater than 100). (2) Zero-length entries are harmless; they contribute nothing but do not break the sum check. (3) Large lengths like 10^18 must be handled carefully; we add them to a running sum and compare against 100, but since we check after each addition, we can detect when the sum exceeds 100 and return early. The check should be done *before* adding the next length to avoid overflow when adding two numbers that each individually fit in int64_t but their sum overflows (e.g., 10^18 + 10^18). Instead, use `if (sum > 100 - lengths[i])` to avoid overflow. However, since we only care about exceeding 100, and lengths are non-negative, a simpler approach is: `if (sum + lengths[i] > 100)` but this could overflow if sum and lengths[i] are both large positive. The safe check is `if (lengths[i] > 100 - sum)` which works because sum is always ≤ 100 at that point. Then update `sum += lengths[i]`. After passing all checks, build the string using nested loops or `std::string::append(count, char)`. Time complexity: O(N + total output length) in the worst case where sum ≤ 100, so total output length is at most 100; building the string is O(100). Thus overall O(N + 100) = O(N). Space complexity: O(100) for the output string, plus O(N) for input vectors (already given). We must return a string, so extra space linear in the output length.

#include <string>
#include <vector>
#include <cstdint>

// Expands characters according to lengths, returns "Too Long" if total exceeds 100.
std::string expandSequence(int N, const std::vector<char>& characters, const std::vector<int64_t>& lengths) {
    int64_t sum = 0;
    
    // First verify that total length does not exceed 100, with overflow-safe check.
    for (int i = 0; i < N; ++i) {
        // If lengths[i] is larger than (100 - sum), total will exceed 100.
        if (lengths[i] > 100 - sum) {
            return "Too Long";
        }
        sum += lengths[i];
    }
    
    // Build the output string.
    std::string result;
    result.reserve(static_cast<size_t>(sum));
    for (int i = 0; i < N; ++i) {
        result.append(static_cast<size_t>(lengths[i]), characters[i]);
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

// The solution function is declared here (for the test file).
std::string expandSequence(int N, const std::vector<char>& characters, const std::vector<int64_t>& lengths);

int main() {
    // Basic case: expansion in order.
    assert(expandSequence(3, {'a','b','c'}, {2,3,1}) == "aabbb c");
    // Actually correct: "aabbb c" has a space? No, it's "aabbbc" but let's fix: lengths 2,3,1 => "aa"+"bbb"+"c" = "aabbbc".
    assert(expandSequence(3, {'a','b','c'}, {2,3,1}) == "aabbbc");
    
    // Empty vectors: N=0 gives empty string.
    assert(expandSequence(0, {}, {}) == "");
    
    // Total exactly 100 is allowed.
    std::vector<char> chars(1, 'x');
    std::vector<int64_t> lens(1, 100);
    std::string expected(100, 'x');
    assert(expandSequence(1, chars, lens) == expected);
    
    // Total exceeds 100 returns "Too Long".
    assert(expandSequence(2, {'a','b'}, {60,41}) == "Too Long");
    
    // Zero lengths are fine and don't affect sum.
    assert(expandSequence(4, {'a','b','c','d'}, {0,5,0,2}) == "bbbbbdd");
    
    // Large length that would overflow int32 but check occurs early.
    assert(expandSequence(2, {'a','b'}, {1000000000000LL, 1}) == "Too Long");
    
    // Single character repeated many times but total under 100.
    std::vector<char> oneChar(1, 'z');
    std::vector<int64_t> oneLen(1, 50);
    std::string fiftyZ(50, 'z');
    assert(expandSequence(1, oneChar, oneLen) == fiftyZ);
    
    // Mix of zero and positive, sum less than 100.
    assert(expandSequence(3, {'p','q','r'}, {2,0,3}) == "pprrr");
    
    return 0;
}
