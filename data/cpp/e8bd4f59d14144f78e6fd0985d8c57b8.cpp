// Write a C++ function `countPlatesBetweenCandles` that takes a string `s` consisting only of characters `'*'` (plates) and `'|'` (candles), and a 2D vector `queries` where each query is a pair `[left, right]` (0-indexed inclusive bounds). For each query, the function must return the number of plates that are strictly between two candles within the substring `s[left..right]`. A plate is counted if and only if there exists at least one candle to its left and at least one candle to its right within the queried range, and the plate itself is not a candle. The function must efficiently handle up to 10^5 queries on a string of length up to 10^5. It should not modify the input string and should use `const` references where appropriate. The return type is `vector<int>` containing one answer per query in the same order as the input queries.

// The naive approach used in the provided snippet iterates over each query and scans the entire substring for each plate, expanding left and right for every plate, leading to O(|s| * number of queries) worst-case time, which is too slow for large inputs. Instead, we can preprocess the string in linear time to answer each query in O(1).
//
// We need to know, for any plate at position `i`, whether there is a candle to its left and right within the query range. This can be determined using two auxiliary arrays:
// - `leftCandle[i]`: the index of the closest candle to the left of or at position `i` (or -1 if none).
// - `rightCandle[i]`: the index of the closest candle to the right of or at position `i` (or -1 if none).
//
// Additionally, we need to count plates that are between candles. If we can precompute a prefix sum array `prefixPlates` where `prefixPlates[i]` = number of plates in `s[0..i-1]`, then the number of plates in a range `[L, R]` is `prefixPlates[R+1] - prefixPlates[L]`. However, not all plates in that range are between candles—only those that have a candle both to their left and to their right within the query. For any plate at position `i`, it is "covered" if there exists a candle index `c1` with `c1 <= i < c2` where `c1 >= left` and `c2 <= right`. This is exactly the same as saying that the closest candle to the left of `i` (call it `Lc`) and the closest candle to the right of `i` (call it `Rc`) satisfy `Lc >= left` and `Rc <= right`. But we need an efficient way to count such plates.
//
// A common trick: for each position `i` that is a plate, we can compute the "left boundary" candle index and "right boundary" candle index using the precomputed nearest-candle arrays. However, a simpler approach is: since a plate is counted only if there exists a candle to the left and a candle to the right within the query, and since candles themselves are not plates, we can think of the valid plates as those that lie between the first and last candle in that query range. Specifically, for a query `[left, right]`, let:
// - `firstCandle =` the first position `>= left` where `s[i] == '|'` (or -1 if none).
// - `lastCandle =` the last position `<= right` where `s[i] == '|'` (or -1 if none).
//
// Then the valid plates are exactly those plates that are strictly between `firstCandle` and `lastCandle`. Because any plate outside this interval either has no candle to its left or no candle to its right within the query. So the answer for a query is simply the number of plates in `(firstCandle, lastCandle)`, which is `prefixPlates[lastCandle] - prefixPlates[firstCandle+1]` (if firstCandle and lastCandle exist and firstCandle < lastCandle). We can precompute `nextCandle[i]` (next candle to the right or at i) and `prevCandle[i]` (previous candle to the left or at i) in O(n) time. Then for each query, get `firstCandle = nextCandle[left]`, `lastCandle = prevCandle[right]`, and if `firstCandle != -1 && lastCandle != -1 && firstCandle < lastCandle`, answer = `prefixPlates[lastCandle] - prefixPlates[firstCandle+1]` (since prefixPlates[x] counts plates in [0, x-1], so plates in (firstCandle, lastCandle) = prefixPlates[lastCandle] - prefixPlates[firstCandle+1]). Otherwise answer is 0.
//
// Edge cases: 
// - No candles in the substring → return 0.
// - Only one candle → return 0.
// - Candle at the left boundary and candle at the right boundary, but no plates between → prefix difference yields 0.
// - Queries with left > right? Problem statement likely assumes left <= right; we can handle gracefully by returning 0 if invalid.
//
// Time complexity: preprocessing O(n) where n = |s|. Each query O(1). Total O(n + q). Space complexity O(n) for auxiliary arrays and prefix sums.

#include <vector>
#include <string>

std::vector<int> countPlatesBetweenCandles(const std::string& s, const std::vector<std::vector<int>>& queries) {
    int n = static_cast<int>(s.size());

    // Prefix count of plates
    std::vector<int> prefixPlates(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefixPlates[i + 1] = prefixPlates[i] + (s[i] == '*' ? 1 : 0);
    }

    // nextCandle[i] = index of first candle in s[i..n-1], or -1 if none
    std::vector<int> nextCandle(n, -1);
    int next = -1;
    for (int i = n - 1; i >= 0; --i) {
        if (s[i] == '|') next = i;
        nextCandle[i] = next;
    }

    // prevCandle[i] = index of last candle in s[0..i], or -1 if none
    std::vector<int> prevCandle(n, -1);
    int prev = -1;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '|') prev = i;
        prevCandle[i] = prev;
    }

    std::vector<int> result;
    result.reserve(queries.size());
    for (const auto& q : queries) {
        int left = q[0];
        int right = q[1];
        if (left < 0 || right >= n || left > right) {
            result.push_back(0);
            continue;
        }

        int firstCandle = nextCandle[left];
        int lastCandle = prevCandle[right];

        if (firstCandle == -1 || lastCandle == -1 || firstCandle >= lastCandle) {
            result.push_back(0);
        } else {
            // Plates strictly between firstCandle and lastCandle
            int count = prefixPlates[lastCandle] - prefixPlates[firstCandle + 1];
            result.push_back(count);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Assume the function is defined above

int main() {
    // Example 1 from description
    std::string s1 = "**|**|***|";
    std::vector<std::vector<int>> q1 = {{2, 5}, {5, 9}};
    std::vector<int> r1 = countPlatesBetweenCandles(s1, q1);
    assert(r1 == std::vector<int>({2, 3})); // Check: [2,5] substring "|**|" has 2 plates; [5,9] substring "|***|" has 3 plates.

    // Example 2 from description
    std::string s2 = "***|**|*****|**||**|*";
    std::vector<std::vector<int>> q2 = {{1, 17}, {4, 5}, {14, 17}, {5, 11}, {15, 16}};
    std::vector<int> r2 = countPlatesBetweenCandles(s2, q2);
    assert(r2 == std::vector<int>({7, 0, 0, 2, 0}));

    // Edge: no candles
    std::string s3 = "*****";
    std::vector<std::vector<int>> q3 = {{0, 4}, {1, 3}};
    assert(countPlatesBetweenCandles(s3, q3) == std::vector<int>({0, 0}));

    // Edge: single candle
    std::string s4 = "**|**";
    std::vector<std::vector<int>> q4 = {{0, 4}};
    assert(countPlatesBetweenCandles(s4, q4) == std::vector<int>({0}));

    // Edge: exactly two candles with plates between
    std::string s5 = "|**|";
    std::vector<std::vector<int>> q5 = {{0, 3}};
    assert(countPlatesBetweenCandles(s5, q5) == std::vector<int>({2}));

    // Edge: query where left/right are at candle positions and no plates
    std::string s6 = "||";
    std::vector<std::vector<int>> q6 = {{0, 1}};
    assert(countPlatesBetweenCandles(s6, q6) == std::vector<int>({0}));

    // Edge: query with left>right (invalid input), should return 0
    std::string s7 = "|*|";
    std::vector<std::vector<int>> q7 = {{2, 1}};
    assert(countPlatesBetweenCandles(s7, q7) == std::vector<int>({0}));

    // Larger test: string with alternating pattern
    std::string s8 = "*|*|*|*";
    // Query [0,6] gives plates between first and last candle: positions 2,4 are plates? Actually s[0]='*',1='|',2='*',3='|',4='*',5='|',6='*'
    // firstCandle at 1, lastCandle at 5. Plates between: positions 2 and 4 → count=2.
    std::vector<std::vector<int>> q8 = {{0, 6}};
    assert(countPlatesBetweenCandles(s8, q8) == std::vector<int>({2}));

    // Query covering all plates but missing one candle on the right
    std::vector<std::vector<int>> q9 = {{0, 4}}; // substring "*|*|*" → firstCandle=1, lastCandle=3, plates between: position 2 → count=1
    assert(countPlatesBetweenCandles(s8, q9) == std::vector<int>({1}));

    return 0;
}
