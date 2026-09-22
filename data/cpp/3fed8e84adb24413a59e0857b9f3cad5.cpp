/*
Write a C++ function `long long countGoodTriples(const std::string& s)` that, given a string `s` of length `n` consisting only of characters `'G'` and `'H'`, counts the number of triples of indices `(i, j, k)` with `0 <= i < j < k < n` such that when you look at the three characters at these positions, at least one of the two characters (`'G'` or `'H'`) appears exactly once among them. In other words, a triple is counted if the multiset `{s[i], s[j], s[k]}` contains both `'G'` and `'H'` and one of them appears exactly once (so it is not all same, and not a 2-1 split? Actually careful: A triple is good if neither `'G'` nor `'H'` appears more than once? Let’s define precisely: The triple is good if the number of `'G'` characters is exactly 1 OR the number of `'H'` characters is exactly 1 (equivalently, the three characters are not all the same, and not a 2-1 split? Wait, if count of G =1 then count of H=2 (good), if count of H=1 then count of G=2 (good), if count of G=3 then neither count=1 (bad), if count of G=0 then H=3 (bad). So actually a triple is good iff it contains both characters, i.e., it has at least one `'G'` and at least one `'H'`. Because with 3 positions, if you have both letters, one of them must appear exactly once. So "good triple" = triple that contains at least one `'G'` and at least one `'H'`. That is the intended behavior from the given snippet. However the snippet has bugs (e.g., missing parentheses, incorrect logic, and it counts from a specific s index). In our task, we will define: count the number of triples (i<j<k) such that the triple contains both `'G'` and `'H'`. Return that count as a `long long`. Input length n is up to 5000. Provide a function with signature `long long countMixedTriples(const std::string& s)`. The function must be self-contained, not modify input, and handle empty or small strings returning 0. Ensure no overflow (use long long).
*/
#include <string>
#include <algorithm>

// Count the number of index triples (i<j<k) that contain both 'G' and 'H'.
// A triple is "mixed" if it is not all the same letter.
long long countMixedTriples(const std::string& s) {
    long long n = static_cast<long long>(s.size());
    if (n < 3) return 0;

    long long countG = 0;
    for (char ch : s) {
        if (ch == 'G') ++countG;
    }
    long long countH = n - countG;

    // Total triples: n*(n-1)*(n-2)/6
    long long totalTriples = n * (n - 1) * (n - 2) / 6;

    // Triples with all G: C(countG,3)
    long long allGTriples = 0;
    if (countG >= 3) {
        allGTriples = countG * (countG - 1) * (countG - 2) / 6;
    }

    // Triples with all H: C(countH,3)
    long long allHTriples = 0;
    if (countH >= 3) {
        allHTriples = countH * (countH - 1) * (countH - 2) / 6;
    }

    return totalTriples - allGTriples - allHTriples;
}
#include <cassert>
#include <string>

// solution function declaration (for test purposes, we include it here)
long long countMixedTriples(const std::string& s);

int main() {
    // n<3 returns 0
    assert(countMixedTriples("") == 0);
    assert(countMixedTriples("G") == 0);
    assert(countMixedTriples("GH") == 0);

    // n=3: "GGG" -> 0, "GGH" -> 1, "GHH" -> 1, "GHG" -> 1, "HGG" -> 1, "HHH" -> 0
    assert(countMixedTriples("GGG") == 0);
    assert(countMixedTriples("HHH") == 0);
    assert(countMixedTriples("GGH") == 1);
    assert(countMixedTriples("GHG") == 1);
    assert(countMixedTriples("HGG") == 1);
    assert(countMixedTriples("GHH") == 1);
    assert(countMixedTriples("HGH") == 1);
    assert(countMixedTriples("HHG") == 1);

    // n=4: "GGGH" -> triples: total C(4,3)=4, all G triples C(3,3)=1, all H=0, ans=3
    assert(countMixedTriples("GGGH") == 3);
    assert(countMixedTriples("GHHH") == 3);

    // n=5: "GGGGG" -> all G, total C(5,3)=10, all G=10, ans=0
    assert(countMixedTriples("GGGGG") == 0);
    // "GGGHH" -> total 10, all G C(3,3)=1, all H C(2,3)=0, ans=9
    assert(countMixedTriples("GGGHH") == 9);

    // Mixed evenly: "GGHHH" -> total 10, all G C(2,3)=0, all H C(3,3)=1, ans=9
    assert(countMixedTriples("GGHHH") == 9);

    // Alternating: "GHGHG" -> countG=3, countH=2, total 10, all G C(3,3)=1, all H 0, ans=9
    assert(countMixedTriples("GHGHG") == 9);

    // Large test: all mixed? "GH" repeated 2500 times => n=5000, countG=2500, countH=2500
    // total C(5000,3)=20833350000, allG C(2500,3)=2601562500, allH same, ans=15630225000
    std::string large;
    for (int i = 0; i < 2500; ++i) large += "GH";
    long long res = countMixedTriples(large);
    long long n = 5000;
    long long total = n * (n-1) * (n-2) / 6;
    long long g = 2500;
    long long allG = g * (g-1) * (g-2) / 6;
    long long expected = total - 2 * allG;
    assert(res == expected);

    return 0;
}
// The problem reduces to counting all 3-element index subsets that include both letters. The total number of triples is C(n,3). The number of triples that are all `'G'` is C(countG,3), and all `'H'` is C(countH,3). Since a triple that is not all same must contain both letters (because only two letters exist), the answer is totalTriples - C(countG,3) - C(countH,3). This is O(n) to count characters and O(1) arithmetic. Edge cases: n<3 returns 0, counts might be large (n up to 5000, C(5000,3) ~ 2.08e10, which fits in 64-bit signed, but long long is safe). Also the original snippet seems to iterate a weird DFS that counts triples of consecutive? Actually reading the snippet, it does dfs from a starting position s (in original it's for s from 1 to n-2) and expands left/right from s? But the code has many bugs (like `g+c[r]=='G'` is actually `(g+c[r])=='G'` which is wrong, also `c[s-1]=='G' + c[s]=='G' + c[s+1]=='G'` is integer addition of booleans but then assigned to long long g and h incorrectly). The intended task is to count triples where both letters appear, but we'll present a clean version. The approach is simple combinatorics: For each triple, the condition is that it is not monochromatic. So answer = total triples - all-G triples - all-H triples. Time O(n), space O(1). We must be careful to use `long long` for multiplication. Also note that n might be up to 10^5? In our task we set n up to 5000 for clarity, butGB function works for any n. We'll write the function with a descriptive name `countMixedTriples`.
