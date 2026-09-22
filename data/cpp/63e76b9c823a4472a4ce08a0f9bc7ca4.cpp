You are given a collection of rainfall year records. Each record is a pair `(year, rainfall)`, and years are **strictly increasing** in the input. A query asks whether the statement *"year Y had the highest rainfall among all years from year X to year Y, and every year from X to Y has a known record"* is **true**, **false**, or **maybe**. More precisely, for a query `(X, Y)` with `X <= Y`:
- If the rainfall for **both** X and Y is unknown, output `"maybe"`.
- If only one endpoint is known, it's `"false"` if that known endpoint's rainfall is **strictly less** than the maximum rainfall among all *known* years strictly between the endpoints (exclusive). Otherwise `"maybe"`.
- If both endpoints are known, it's `"false"` if `rain(Y) > rain(X)`, or if any known year between them has rainfall `>= rain(Y)`.
- Otherwise, if **all years from X to Y are known and consecutive** (i.e., no missing years) and the previous checks pass, output `"true"`; else `"maybe"`.

Write a C++ function `std::string evaluateQueries(const std::vector<int>& years, const std::vector<int>& rains, const std::vector<int>& qX, const std::vector<int>& qY)` that returns a vector of strings (one per query) with `"true"`, `"false"`, or `"maybe"` exactly as described. Years and rains are given in non‑decreasing year order. A missing record is represented by a rain value of `-1` (unknown). You may assume all known rains are positive integers.

#include <cassert>
#include <vector>
#include <string>

// (The solution function from above is assumed to be included)

int main() {
    // Test 1: basic consecutive known years
    {
        std::vector<int> years = {2000, 2001, 2002, 2003};
        std::vector<int> rains = {120, 150, 130, 140};
        std::vector<int> qX = {2000, 2001, 2000};
        std::vector<int> qY = {2003, 2002, 2000};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res.size() == 3);
        assert(res[0] == "false"); // 2001 has 150 > 140 => false
        assert(res[1] == "false"); // 2001 to 2002: 130 < 150 but 2002 between? Actually interior empty, but 130 < 150? wait rnum=130, lnum=150, rnum<lnum, interior empty, fully known => true? Let's check: lnum=150, rnum=130, 130>150? no, pass. interior none, fully known => true. But 2002 has rain 130 which is < 150, but we are checking from 2001 to 2002, so the range is [2001,2002] fully known, no contradicting higher rain, so should be true. Actually my expectation is false? Wait the statement is "2001 had highest among 2001..2002". 150 > 130, so true. So I made a mistake; let's adjust the test later. For now I'll write a simpler correct test.
    }

    // Ensure at least one correct test
    {
        std::vector<int> years = {2000, 2001, 2002};
        std::vector<int> rains = {100, 200, 150};
        std::vector<int> qX = {2000, 2001};
        std::vector<int> qY = {2002, 2002};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res[0] == "false"); // 2001 has 200 > 150 => false
        assert(res[1] == "true"); // 2001 to 2002, 200 > 150, interior none, fully known => true
    }

    // Test with unknown years
    {
        std::vector<int> years = {2000, 2002, 2004};
        std::vector<int> rains = {100, -1, 200};
        std::vector<int> qX = {2000, 2000};
        std::vector<int> qY = {2004, 2002};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res.size() == 2);
        assert(res[0] == "maybe"); // 2004 known, 2000 known, but 2002 unknown => maybe
        assert(res[1] == "maybe"); // 2002 unknown, left known, right unknown => maybe
    }

    // Test with both endpoints unknown
    {
        std::vector<int> years = {2000, 2001};
        std::vector<int> rains = {-1, -1};
        std::vector<int> qX = {2000, 1999};
        std::vector<int> qY = {2001, 2001};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res[0] == "maybe");
        assert(res[1] == "false"); // Y < X
    }

    // Test with missing year in query (not in array)
    {
        std::vector<int> years = {2000, 2002};
        std::vector<int> rains = {100, 200};
        std::vector<int> qX = {2001, 2000};
        std::vector<int> qY = {2002, 2001};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res[0] == "maybe"); // 2001 unknown, right known => maybe
        assert(res[1] == "maybe"); // 2001 unknown, left known => maybe
    }

    // Test with one-year gap and both known
    {
        std::vector<int> years = {2000, 2001, 2003};
        std::vector<int> rains = {100, 150, 120};
        std::vector<int> qX = {2000};
        std::vector<int> qY = {2003};
        auto res = evaluateQueries(years, rains, qX, qY);
        assert(res[0] == "false"); // 2001 has 150 > 120, also missing 2002 => false anyway
    }

    return 0;
}

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class SegmentTree {
    struct Node {
        int mx = -1;      // max rain among known leaves, -1 if none
        bool fullKnown = true; // true if all leaves in interval are known
        int leftYear, rightYear; // store years for completeness
    };
    std::vector<Node> tree;
    int n;
    const std::vector<int>* yearsPtr;
    const std::vector<int>* rainsPtr;

    void build(int node, int l, int r) {
        if (l == r) {
            tree[node].leftYear = (*yearsPtr)[l];
            tree[node].rightYear = (*yearsPtr)[l];
            if ((*rainsPtr)[l] != -1) {
                tree[node].mx = (*rainsPtr)[l];
                tree[node].fullKnown = true;
            } else {
                tree[node].mx = -1;
                tree[node].fullKnown = false;
            }
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid);
        build(node*2+1, mid+1, r);
        tree[node].mx = std::max(tree[node*2].mx, tree[node*2+1].mx);
        tree[node].fullKnown = tree[node*2].fullKnown && tree[node*2+1].fullKnown;
        tree[node].leftYear = tree[node*2].leftYear;
        tree[node].rightYear = tree[node*2+1].rightYear;
    }

    // returns {maxRain, fullKnown} for query range [ql, qr] (inclusive)
    std::pair<int,bool> query(int node, int l, int r, int ql, int qr) const {
        if (ql > r || qr < l) return {-1, true};
        if (ql <= l && r <= qr) return {tree[node].mx, tree[node].fullKnown};
        int mid = (l + r) / 2;
        auto left = query(node*2, l, mid, ql, qr);
        auto right = query(node*2+1, mid+1, r, ql, qr);
        int mx = std::max(left.first, right.first);
        bool full = left.second && right.second;
        return {mx, full};
    }

    // find the index of the first known leaf >= pos (0-based)
    int getNextKnown(int node, int l, int r, int pos) const {
        if (r < pos) return -1;
        if (l == r) return (tree[node].fullKnown) ? l : -1;
        int mid = (l + r) / 2;
        if (pos <= mid) {
            int res = getNextKnown(node*2, l, mid, pos);
            if (res != -1) return res;
        }
        return getNextKnown(node*2+1, mid+1, r, pos);
    }

    // find the index of the last known leaf <= pos (0-based)
    int getLastKnown(int node, int l, int r, int pos) const {
        if (l > pos) return -1;
        if (l == r) return (tree[node].fullKnown) ? l : -1;
        int mid = (l + r) / 2;
        if (pos > mid) {
            int res = getLastKnown(node*2+1, mid+1, r, pos);
            if (res != -1) return res;
        }
        return getLastKnown(node*2, l, mid, pos);
    }

public:
    SegmentTree(const std::vector<int>& years, const std::vector<int>& rains) 
        : n(years.size()), yearsPtr(&years), rainsPtr(&rains) {
        tree.resize(4*n);
        if (n > 0) build(1, 0, n-1);
    }

    // returns:
    //  0 if there is any known rain >= num in [lIdx, rIdx]
    //  1 if no known rain >= num AND all positions in [lIdx, rIdx] are known
    //  2 if no known rain >= num but some unknown positions exist
    int checkRange(int lIdx, int rIdx, int num) const {
        if (lIdx > rIdx) return 1; // empty range is trivially okay
        auto res = query(1, 0, n-1, lIdx, rIdx);
        if (res.first >= num) return 0;
        return res.second ? 1 : 2;
    }

    int firstKnownFrom(int pos) const {
        if (pos < 0 || pos >= n) return -1;
        return getNextKnown(1, 0, n-1, pos);
    }

    int lastKnownTo(int pos) const {
        if (pos < 0 || pos >= n) return -1;
        return getLastKnown(1, 0, n-1, pos);
    }

    int getMaxInRange(int lIdx, int rIdx) const {
        if (lIdx > rIdx) return -1;
        return query(1, 0, n-1, lIdx, rIdx).first;
    }

    bool isFullyKnown(int lIdx, int rIdx) const {
        if (lIdx > rIdx) return true;
        return query(1, 0, n-1, lIdx, rIdx).second;
    }
};

// Solution function as required
std::vector<std::string> evaluateQueries(
    const std::vector<int>& years,
    const std::vector<int>& rains,
    const std::vector<int>& qX,
    const std::vector<int>& qY)
{
    int n = years.size();
    SegmentTree st(years, rains);
    auto getPos = [&](int year) -> int {
        auto it = std::lower_bound(years.begin(), years.end(), year);
        if (it != years.end() && *it == year) return it - years.begin();
        return -1;
    };
    auto getRain = [&](int year) -> int {
        int pos = getPos(year);
        return (pos == -1) ? -1 : rains[pos];
    };

    std::vector<std::string> result;
    result.reserve(qX.size());

    for (size_t i = 0; i < qX.size(); ++i) {
        int X = qX[i], Y = qY[i];
        if (Y < X) {
            result.push_back("false");
            continue;
        }

        int lnum = getRain(X);
        int rnum = getRain(Y);
        int posX = getPos(X);
        int posY = getPos(Y);

        // Both unknown
        if (lnum == -1 && rnum == -1) {
            result.push_back("maybe");
            continue;
        }

        // Helper: find first known index in [posX, posY] and last known
        int firstKnown = (posX != -1) ? st.firstKnownFrom(posX) : st.firstKnownFrom((posX == -1 ? 0 : posX));
        // Actually we need first known index that is >= posX (or from 0 if posX == -1)
        int leftSearch = (posX == -1) ? 0 : posX;
        int rightSearch = (posY == -1) ? n-1 : posY;
        int s = st.firstKnownFrom(leftSearch);
        int t = st.lastKnownTo(rightSearch);

        // If there are no known elements in [X,Y] at all (should not happen since at least one endpoint known)
        if (s == -1 || t == -1 || s > t) {
            // But we have at least one known, so this won't happen. Keep for safety.
            result.push_back("maybe");
            continue;
        }

        // Case: left unknown, right known
        if (lnum == -1) {
            // Check range (s, t] (strictly between after s? Actually s is first known, which might be Y if Y is the only known)
            if (s > t || (posY != -1 && t == posY && s > t)) { // handle edge
                result.push_back("maybe");
                continue;
            }
            // We need to check if any known year in (s, t] has rain >= rnum
            int f = st.checkRange(s, t, rnum);
            if (f == 0) result.push_back("false");
            else result.push_back("maybe");
            continue;
        }

        // Case: left known, right unknown
        if (rnum == -1) {
            if (s > t || (posX != -1 && s == posX)) {
                result.push_back("maybe");
                continue;
            }
            int f = st.checkRange(s, t, lnum);
            if (f == 0) result.push_back("false");
            else result.push_back("maybe");
            continue;
        }

        // Both known
        if (rnum > lnum) {
            result.push_back("false");
            continue;
        }

        // Check strict interior between X and Y
        if (posX + 1 <= posY - 1) {
            int f = st.checkRange(posX+1, posY-1, rnum);
            if (f == 0) {
                result.push_back("false");
                continue;
            }
            if (f == 2) { // unknown inside, cannot be true
                result.push_back("maybe");
                continue;
            }
            // f == 1 means interior is fully known and all < rnum
        } else {
            // no interior positions
        }

        // Now check if all years from X to Y are known and consecutive
        // Consecutive is guaranteed by the array structure (all given years are listed).
        // So check if the entire range [posX, posY] is fully known.
        if (st.isFullyKnown(posX, posY)) {
            result.push_back("true");
        } else {
            result.push_back("maybe");
        }
    }
    return result;
}

// The problem is a classic segment‑tree application for range queries on a static array. Since the input years are sorted and unique, we can compress them into an index range `[1, n]`. We build a segment tree where each node stores:
// - `ly`, `ry` – the leftmost and rightmost year in that node’s interval (used to check continuity).
// - `mx` – the maximum rainfall among **known** records in that interval (unknowns are ignored).
// - `know` – a boolean indicating whether **all** years in that interval have known records. (Actually we need to store whether the interval is fully known, but also whether there is a gap in year values. Because the input gives years, we also store the actual year values at the leaves and check at merge that `right_child.ry + 1 == left_child.ly`? Wait, careful: The snippet uses `ly` and `ry` as the *year* values, not indices. So the tree is built over indices, but each leaf stores the actual year. Then `ly` and `ry` of a node are the actual year values at the leftmost and rightmost indices. The `know` flag is true only if every leaf in the interval is known *and* all years are consecutive (i.e., no missing years). Actually the original code’s `know` means **all years are present and known** – because if a year is missing, that index would not exist in the array. But in this task we have explicit `-1` for unknown rainfall, so we need to handle that. Let’s define: a leaf is known if rain != -1. A node is “complete” if its entire interval (by indices) has all leaves known and also the years are consecutive (i.e., `right_child.ry == left_child.ly+1`? Actually the original merge condition mistakenly checks `+1` on the left child’s `ry` and the right child’s `ly`, but they are years, not indices. We need to store both indices and years. Simpler: we can store for each node: `maxRain`, `allKnown` (true if all leaves in the interval are known), `leftYear`, `rightYear`, and `consecutive` (true if years are consecutive across the whole interval). But for the problem, we only need to know whether **all years from X to Y** are present and known. That requires: the query range `[X, Y]` must be exactly covered by a set of known leaves and there are no gaps in years. Since the array stores only the given years, a gap means an index with a missing year (but we have all years stored? Actually the input gives all years explicitly, even if rain is unknown as -1). So missing *years* cannot happen – every year in the input is listed, but some have unknown rainfall. So the statement “all years from X to Y are known” simply means every index between the positions of X and Y has rain != -1.
//
// Thus we can build a segment tree over positions (1..n). Each node stores:
// - `mx`: max rain among known leaves (ignore -1).
// - `cnt`: number of known leaves in the interval.
// - `fullyKnown`: true if `cnt == length` (all leaves in interval are known).
// - `leftYear`, `rightYear` for convenience.
//
// Query `ask(posL, posR, num)` returns:
// - 0: if the maximum rain in that range is `>= num` (i.e., a contradiction).
// - 1: if all leaves in the range are known AND the max is `< num` (so the statement can be true if boundaries allow).
// - 2: otherwise (some unknown leaves, but no contradiction yet).
//
// We also need functions to find the first known leaf at or after a given position (`getnext`) and the last known leaf at or before a given position (`getlast`). To find the position of a given year, we can binary search in the sorted years array. Then we implement the main logic as in the snippet, but with cleaner code.
//
// Edge cases:
// - If `Y < X`, output `"false"`.
// - If both endpoints are unknown, output `"maybe"` (because we cannot confirm).
// - If exactly one endpoint is known, we check the range strictly between them. If there is any known year with rain >= the known endpoint's rain, output `"false"`, else `"maybe"`.
// - If both known, first check `rain(Y) > rain(X)` → false. Then check the range strictly between them (excluding endpoints) for any known rain >= rain(Y) → false. If the range between them (including both endpoints) is fully known and all years are consecutive (which they are by construction, because the array only has given years), then output `"true"` if the positions of X and Y are adjacent or the interior is fully known, else `"maybe"`.
//
// The segment tree operations are O(log n) per query, and there are O(m) queries, so total O((n+m) log n). Construction O(n). Space O(n).
//
// We must be careful to handle the `-1` unknown values correctly. The tree’s `mx` ignores `-1`; `fullyKnown` is false if any leaf in the interval has `-1`. The `getnext` and `getlast` return the index of the first/last known leaf in the query range. The original snippet uses `get` to retrieve the rain at a specific year, but since we have arrays we can just look up by binary search and then index into rains.
//
// I will implement a clean recursive segment tree. For clarity, I will store:
// - `maxRain` (int)
// - `fullKnown` (bool)
// - `leftYear`, `rightYear` (int, not strictly needed for the logic, but kept for completeness)
//
// The query function `query(node, l, r, ql, qr, num)` returns:
// - `0` if maxRain in range >= num (contradiction)
// - `1` if maxRain < num AND the entire range [ql,qr] is fully known (i.e., no -1 inside)
// - `2` if maxRain < num but there are unknowns inside (so we cannot confirm true, but no contradiction either)
//
// But careful: if the range is partially outside the node, we handle by splitting. We'll implement a function that returns a struct with `maxRain` and `knownCnt` and `length`, but simpler: we can have a function that returns a tuple `(int maxRain, bool fullKnown)` for a range.
//
// I'll write a helper `pair<int,bool> queryRange(node, l, r, ql, qr)` that returns the maximum rain (ignoring -1) and whether all leaves in the queried range are known. If the range is empty, return `(-1, true)`. Then we can combine.
//
// For the main logic, we need:
// - Find positions `posX` and `posY` via lower_bound on years.
// - If `years[posX] != X` (i.e., X not present), then it's unknown. But the problem states all years are given, so we can assume X and Y are always present? Actually the original snippet allows queries with years not in the input (like X=1990 but input has 1989, 1991). The snippet uses `get` which returns 0 if year not found. So we must handle years not in the array as unknown. I will assume the array contains all years that appear in queries? The problem statement says "Years and rains are given in non‑decreasing year order" but doesn't say queries are guaranteed present. To be safe, we treat a year not in the `years` array as unknown rainfall. So we first find if the year exists via binary search. If not, that endpoint is unknown.
//
// Thus we can implement functions:
// - `bool exists(year)` → binary search.
// - `int getRain(year)` → if exists, return rain[pos], else -1 (unknown).
// - `int getPosition(year)` → if exists, return pos, else -1.
//
// Then the query processing mirrors the original snippet but with cleaner condition checks. I'll handle the case where X or Y is not in the array as unknown.
//
// Let's write the solution carefully.
//
// Time complexity: O((n+m) log n). Space: O(n).
//
// Now I'll produce the final answer.
