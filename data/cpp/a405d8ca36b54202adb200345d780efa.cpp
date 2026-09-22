Write a C++ function `bool canTransformSuffix(const std::string& a, const std::string& b)` that determines whether string `a` can be transformed into string `b` under the following rule: you may only remove characters from the *beginning* of `a` (i.e., delete characters from the front), and you may also change the *first remaining character* of `a` to any other lowercase English letter, but only if that first character currently matches the first character of `b`. More precisely, the function should return `true` if and only if there exists a way to repeatedly delete zero or more characters from the front of `a` (keeping the order of the remaining characters), and then optionally modify the first character of the resulting string (if it matches `b[0]`) so that the final string exactly equals `b`. For example, if `a = "abc"` and `b = "bc"`, you can delete the leading `'a'` to get `"bc"`, which equals `b` — return `true`. If `a = "xabc"` and `b = "abc"`, you can delete `'x'` to leave `"abc"` — return `true`. If `a = "abx"` and `b = "axb"`, no sequence works because you cannot reorder characters or delete from the middle; return `false`. The strings consist of lowercase English letters, and the lengths of `a` and `b` satisfy `1 <= b.size() <= a.size() <= 100000`. The function must handle the case where `a` and `b` are identical (return `true`), and also handle when `b` is longer than `a` (return `false`). The algorithm must run in O(n) time and O(1) extra space.

#include <cassert>
#include <string>

// The function to test.
bool canTransformSuffix(const std::string& a, const std::string& b);

int main() {
    // Basic suffix match cases.
    assert(canTransformSuffix("abc", "bc") == true);          // b[0]='b' at index 1, suffix "c" matches.
    assert(canTransformSuffix("abc", "abc") == true);         // identical.
    assert(canTransformSuffix("xabc", "abc") == true);        // suffix "abc" matches, b[0] at index 1.
    assert(canTransformSuffix("ab", "b") == true);            // m=1, b[0]='b' appears.
    assert(canTransformSuffix("ab", "a") == true);            // m=1, b[0]='a' appears.
    assert(canTransformSuffix("abc", "c") == true);           // m=1, b[0]='c' appears.

    // Cases where suffix condition fails.
    assert(canTransformSuffix("abc", "adc") == false);        // last char difference.
    assert(canTransformSuffix("axb", "ab") == false);         // suffix 'b' matches, but no 'a' in [0,1]? Actually a[0]='a' -> would be true? Wait a[0]='a', b[0]='a', suffix 'b' matches, so true. Let's use a failing example: "ba" and "ab" -> suffix 'a' != 'b'? m=2, b[1]='b', a[1]='a' -> false.
    assert(canTransformSuffix("ba", "ab") == false);          // suffix mismatch.

    // Case where b[0] not found in allowed range.
    assert(canTransformSuffix("cde", "ab") == false);         // suffix "de" vs "ab"? last char 'e' vs 'b' mismatch actually false; better example: "axb" and "xb" -> suffix 'b' matches, b[0]='x' appears at index 1 (allowed range 0..1) -> true. For false, need suffix match but b[0] missing: "xac" and "ac" -> suffix 'c' vs 'c' ok, b[0]='a' appears at index 1 -> true. To get false: "abc" and "bc" -> true. "abc" and "cb" -> suffix 'b' vs 'b'? b[1]='b'? Actually b="cb", m=2, last char of a is 'c', b[1]='b'? mismatch -> false. So use "abc" and "cd": suffix last char 'c' vs 'd' mismatch -> false.
    assert(canTransformSuffix("abc", "cd") == false);         // suffix mismatch.

    // Edge: m=1, b[0] not in a.
    assert(canTransformSuffix("abc", "z") == false);          // 'z' not in "abc".

    // Edge: equal lengths but not equal.
    assert(canTransformSuffix("abc", "abd") == false);        // suffix mismatch.

    // Edge: n = m+1, b[0] only at last allowed index.
    assert(canTransformSuffix("xabc", "abc") == true);        // b[0]='a' at index 1, allowed range 0..1.
    assert(canTransformSuffix("xabc", "xbc") == true);        // b[0]='x' at index 0, suffix "bc" matches.
    assert(canTransformSuffix("xabc", "ybc") == false);       // no 'y' in a[0..1].

    // Edge: n=m, only checks a[0]==b[0] with suffix.
    assert(canTransformSuffix("abc", "abc") == true);
    assert(canTransformSuffix("abc", "abb") == false);        // suffix mismatch.
    assert(canTransformSuffix("abc", "bbc") == false);        // a[0] != b[0], but suffix "bc" matches, yet b[0]='b' appears at index 1? But allowed range is [0,0] only, so false.

    return 0;
}

#include <string>

// Returns true if the last |b|-1 characters of a match the last |b|-1 of b,
// and b[0] appears somewhere in a[0 .. |a|-|b|].
bool canTransformSuffix(const std::string& a, const std::string& b) {
    const std::size_t n = a.size();
    const std::size_t m = b.size();

    if (m > n) {
        return false;
    }

    // Condition 1: the last m-1 characters of a must equal the last m-1 of b.
    if (m > 1) {
        // Compare from the end.
        for (std::size_t t = 0; t < m - 1; ++t) {
            if (a[n - 1 - t] != b[m - 1 - t]) {
                return false;
            }
        }
    }

    // Condition 2: b[0] must appear in a[0 .. n-m].
    const std::size_t limit = n - m;
    for (std::size_t i = 0; i <= limit; ++i) {
        if (a[i] == b[0]) {
            return true;
        }
    }

    return false;
}

// The algorithm directly implements the two given conditions. First, if `b.size() > a.size()`, no match is possible, so return `false`. Then, for the suffix condition, we need to compare the last `m-1` characters of `a` with the last `m-1` characters of `b`, where `m = b.size()`. If `m == 1`, there are zero characters to compare, so this condition passes automatically. For `m > 1`, we iterate `k` from 1 to `m-1` (or equivalently, compare `a[a.size() - 1 - t]` with `b[b.size() - 1 - t]` for `t = 0` to `m-2`). If any mismatch occurs, return `false`. Next, we need to check whether `b[0]` appears anywhere in `a` at indices from `0` to `n - m` inclusive, where `n = a.size()`. This is a simple linear scan of at most `n - m + 1` characters; if any matches, return `true`. If no match, return `false`. Edge cases: when `m == 1`, we only need to check if `b[0]` appears anywhere in `a` (since `n - m + 1 = n`), which correctly handles the case where `b` is a single character. If `n == m` (equal lengths), the allowed index range is just `{0}`, so we only check `a[0] == b[0]` combined with the suffix condition, which together mean `a == b`. Time complexity is O(n) because we scan the suffix of `a` of length `m-1` and the prefix of `a` of length `n-m+1`, totaling at most `n` operations. Space complexity is O(1) beyond the input strings.
