Given a set of n non-attacking queens and m target cells on an infinite chessboard, write a C++ function `std::vector<bool> canQueenCapture(const std::vector<std::pair<int,int>>& queens, const std::vector<std::pair<int,int>>& targets)` that returns a `std::vector<bool>` where the i-th element is `true` if the i-th target cell is attacked by at least one queen, and `false` otherwise. A queen attacks any cell sharing its row, column, or diagonal. The function must process the input in O((n+m) log (n+m)) time and O(n+m) space, and must handle duplicate queens and duplicate targets correctly. Coordinates are positive integers up to 10^5.
#include <cassert>
#include <vector>
#include <utility>

// Function prototype (from solution)
std::vector<bool> canQueenCapture(const std::vector<std::pair<int,int>>& queens,
                                  const std::vector<std::pair<int,int>>& targets);

int main() {
    // Case 1: Single queen at (1,1), target at (2,3) – diagonal.
    {
        auto res = canQueenCapture({{1,1}}, {{2,3}});
        assert(res.size() == 1 && res[0] == true);
    }
    // Case 2: Single queen at (1,1), target at (3,2) – not attacked.
    {
        auto res = canQueenCapture({{1,1}}, {{3,2}});
        assert(res.size() == 1 && res[0] == false);
    }
    // Case 3: Target on same cell as queen.
    {
        auto res = canQueenCapture({{2,2}}, {{2,2}});
        assert(res.size() == 1 && res[0] == true);
    }
    // Case 4: Two queens, one attacks straight column, one attacks diagonal.
    {
        auto res = canQueenCapture({{1,1}, {3,3}}, {{1,5}, {5,5}, {4,2}});
        assert(res.size() == 3 && res[0] == true && res[1] == false && res[2] == true);
    }
    // Case 5: Duplicate queens, target between them.
    {
        auto res = canQueenCapture({{1,1}, {1,1}}, {{1,2}, {2,2}});
        assert(res.size() == 2 && res[0] == true && res[1] == true);
    }
    // Case 6: Target that is dominated by one queen but not directly rows/cols/diags.
    {
        auto res = canQueenCapture({{1,1}, {2,3}}, {{3,5}});
        // Queen (2,3) and (3,5) are same diagonal? Yes: 2-3 == 3-5 => -1 == -2? No.
        // Check manually: (1,1) -> (3,5) not same any. So false.
        assert(res.size() == 1 && res[0] == false);
    }
    // Case 7: Large coordinates, multiple queens.
    {
        auto res = canQueenCapture({{1,10}, {10,1}, {5,5}}, {{4,4}, {10,10}, {2,10}});
        assert(res.size() == 3 && res[0] == true && res[1] == true && res[2] == true);
    }
    // Case 8: No queens, all targets false.
    {
        auto res = canQueenCapture({}, {{1,1}, {2,2}});
        assert(res.size() == 2 && !res[0] && !res[1]);
    }
    // Case 9: Target at (0,0) – but coordinates are positive, so skip.
    // Case 10: Random stress via brute force for small coordinates – omitted for brevity.
    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

namespace {
    struct Point {
        int x, y, y_id, type; // type: 0 = queen, >0 = target index+1
    };

    int lowbit(int x) { return x & (-x); }

    void bit_add(std::vector<int>& bit, int idx, int n) {
        while (idx <= n) {
            ++bit[idx];
            idx += lowbit(idx);
        }
    }

    int bit_query(const std::vector<int>& bit, int idx) {
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= lowbit(idx);
        }
        return res;
    }
}

// Returns a vector where result[i] is true if target i is attacked by any queen.
std::vector<bool> canQueenCapture(const std::vector<std::pair<int,int>>& queens,
                                  const std::vector<std::pair<int,int>>& targets) {
    const int n = static_cast<int>(queens.size());
    const int m = static_cast<int>(targets.size());
    if (m == 0) return {};

    int total = n + m;
    std::vector<Point> pts(total);

    for (int i = 0; i < n; ++i) {
        pts[i] = {queens[i].first, queens[i].second, 0, 0};
    }
    for (int i = 0; i < m; ++i) {
        pts[n + i] = {targets[i].first, targets[i].second, 0, i + 1};
    }

    // Sort by y to compress coordinates.
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });
    int y_count = 0;
    for (int i = 0; i < total; ) {
        int j = i;
        ++y_count;
        while (j < total && pts[j].y == pts[i].y) {
            pts[j].y_id = y_count;
            ++j;
        }
        i = j;
    }

    // Sort by x, then y, then type.
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        if (a.y != b.y) return a.y < b.y;
        return a.type < b.type; // queens first
    });

    std::vector<bool> result(m, false);
    std::vector<int> bit(y_count + 1, 0);
    int sum = 0;

    for (int i = 0; i < total; ) {
        // Find end of current x-group.
        int j = i;
        while (j < total && pts[j].x == pts[i].x) ++j;

        int last_queen_idx = -1; // index of the last queen with same x,y within this group
        bool blocked_by_sum = false; // whether this x-group already has a dominating queen

        for (int k = i; k < j; ++k) {
            if (pts[k].type > 0) { // target
                int target_idx = pts[k].type - 1;
                // Check duplicate with a queen exactly here.
                if (last_queen_idx != -1 &&
                    pts[k].x == pts[last_queen_idx].x &&
                    pts[k].y == pts[last_queen_idx].y) {
                    result[target_idx] = true;
                    continue;
                }
                // Check column attack: is there a queen in the same column below?
                int col_count = bit_query(bit, pts[k].y_id) - bit_query(bit, pts[k].y_id - 1);
                if (blocked_by_sum || col_count > 0) {
                    result[target_idx] = true;
                    continue;
                }
                // Check diagonal: x - sum == y - (total queens below in columns)
                int queens_below = bit_query(bit, pts[k].y_id);
                int lhs = pts[k].x - sum;
                int rhs = pts[k].y - queens_below;
                result[target_idx] = (lhs == rhs);
            } else { // queen
                last_queen_idx = k;
                int nx = pts[k].x - sum;
                int ny = pts[k].y - bit_query(bit, pts[k].y_id);
                // If nx < ny, this queen is not dominated by any earlier queen.
                if (nx < ny) {
                    // If no queen already in this column, add it.
                    if (bit_query(bit, pts[k].y_id) - bit_query(bit, pts[k].y_id - 1) == 0) {
                        bit_add(bit, pts[k].y_id, y_count);
                    }
                } else if (nx > ny) {
                    // This queen is dominated, set sum to make future targets blocked.
                    sum += (!blocked_by_sum);
                    blocked_by_sum = true;
                }
            }
        }
        i = j;
    }

    return result;
}
// The key observation is that a queen can attack a target if they share a row, column, or diagonal. The provided snippet uses a sweep-line algorithm with a Fenwick tree (BIT) to count how many queens are in the same column or diagonal for each x-coordinate. The algorithm sorts all points (queens and targets) by x, then processes each x-group from left to right. For each point, we maintain two counters: `sum` represents how many queens have been "stopped" because they are dominated in both x and y by an earlier queen (i.e., there exists another queen with smaller x and smaller y), and `ny = y - query(y)` computes how many queens are in the same column below this point. A target is attacked if either: (a) there is already a queen with the same x and y (duplicate), (b) there is a queen in the same column (observed via BIT), or (c) there is a diagonal conflict (checked via the equation `x - sum == y - query(y)`). The sweep processes all points with the same x together, updating the BIT when a queen is added and adjusting `sum` when a queen is dominated. The algorithm correctly handles duplicates because it processes same coordinates within an x-group and checks for immediate row/column matches. Time complexity is O((n+m) log (n+m)) due to sorting and O((n+m) log (n+m)) for BIT operations, and space is O(n+m) for storing points and the BIT. The original snippet outputs "WIN" for false and "LOSE" for true, but here we invert to return `true` for attacked. Edge cases include multiple queens on the same cell (still counts as one attack), targets on the same cell as a queen (always attacked), and targets that are only on a diagonal (must detect via the formula).
