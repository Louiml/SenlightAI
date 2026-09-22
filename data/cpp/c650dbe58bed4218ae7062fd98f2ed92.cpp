// You are given a grid of up to 20×20 cells containing walkable positions ('.'), a start position 'P', and up to 8 numbered item cells ('0'–'7'). Each item `i` appears once and has a value `c_i`, an activation start time `s_i` (inclusive) and end time `t_i` (exclusive). You start at 'P' at time 0. At each time step you may move to an adjacent cell (up/down/left/right, staying within the grid). You may only move into a numbered cell if you have not yet collected that item; when you enter that cell at a time `T` such that `s_i <= T < t_i`, you immediately collect it and gain its value. You are not allowed to move into a cell that is neither '.' nor a numbered cell not yet collected (i.e., you cannot enter already-collected item cells). Given a maximum time `LIMIT` equal to the maximum `t_i` among all items plus one (or 1 if no items), find the maximum total value you can collect by time `LIMIT` (i.e., after exactly `LIMIT` moves). Write a function `int maxCollectedValue(int H, int W, const std::vector<std::string>& grid, int startX, int startY, const std::vector<Item>& items)` where `Item` has fields `value`, `start`, `end` (with `start < end`), and the indices correspond to the character '0'–'7'. You may assume the grid is rectangular, contains exactly one 'P', and at most 8 distinct item numbers. Return the maximum sum of values of items collected by time `LIMIT`.
#include <cassert>
#include <vector>
#include <string>

// Include the function definition here (or link) – assumed to be above.

int main() {
    // Example 1: 2 items, both active early, reachable.
    {
        int H = 2, W = 3;
        std::vector<std::string> grid = {
            "P0.",
            "..1"
        };
        // Items: 0 at (1,0) value 5 start 0 end 2; 1 at (2,1) value 3 start 1 end 3
        std::vector<Item> items = {
            {5, 0, 2},
            {3, 1, 3}
        };
        // LIMIT = max(2,3)+1 = 4
        // Path: (0,0)->(1,0) collect item0 at t=0 (gain5), then (1,1)->(2,1) collect item1 at t=2? wait: after t=0 move to (1,0), t=1 move to (1,1), t=2 move to (2,1) collect item1 (gain3) total 8. 
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 8);
    }
    
    // Example 2: only one item, but its activation window is late.
    {
        int H = 1, W = 2;
        std::vector<std::string> grid = {"P0"};
        // item 0 value 10 start 5 end 6 -> LIMIT=7, cannot collect because only 1 move available? Actually you can stay? No, you must move each step. From P to 0 takes 1 move, at t=1 not active (need t>=5). So no collection.
        std::vector<Item> items = {{10, 5, 6}};
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 0);
    }
    
    // Example 3: no items.
    {
        int H = 2, W = 2;
        std::vector<std::string> grid = {"P.", ".."};
        std::vector<Item> items;
        // LIMIT=1, can make at most one move, no items, answer 0.
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 0);
    }
    
    // Example 4: two items, but one is blocked by the other? Actually they are all reachable (grid empty).
    {
        int H = 1, W = 3;
        std::vector<std::string> grid = {"P01"};
        // 0 at x=1 value 2 start0 end2, 1 at x=2 value 4 start1 end3 -> LIMIT=4
        // Path: t=0 move to 0 collect (2), t=1 move to 1 collect (4) total 6.
        std::vector<Item> items = {{2,0,2},{4,1,3}};
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 6);
    }
    
    // Example 5: cannot collect because activation times don't match movement timing.
    {
        int H = 1, W = 2;
        std::vector<std::string> grid = {"P0"};
        // item start 1 end 2, move at t=1 collect. LIMIT = 3
        std::vector<Item> items = {{7,1,2}};
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 7);
    }
    
    // Example 6: complex grid, ensure bitmask works.
    {
        int H = 2, W = 3;
        std::vector<std::string> grid = {
            "P.0",
            "1.."
        };
        // 0 at (2,0) value 1 start0 end2, 1 at (0,1) value 100 start1 end3
        // LIMIT=4
        // Best: go to 0 first at t=1? Actually start at (0,0) t=0 -> (1,0) t=1 -> (2,0) collect 0 at t=2? But 0 active 0-2, so t=2 not active (end 2 exclusive). So cannot collect 0 at t=2. Could go to 1 first: (0,0)->(0,1) at t=1 collect 1 (active 1-3) gain100, then go to 0? from (0,1)->(0,0)->(1,0)->(2,0) t=4? But LIMIT=4, at t=4 we are at time equal to LIMIT, cannot move further. So total 100 only.
        std::vector<Item> items = {{1,0,2},{100,1,3}};
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 100);
    }
    
    // Example 7: max 8 items, small grid, ensure no crash.
    {
        int H = 3, W = 3;
        std::vector<std::string> grid = {
            "P..",
            "...",
            "0.."
        };
        // Only one item, but push 8 items with some impossible times.
        std::vector<Item> items;
        items.push_back({10,0,1}); // active at t=0 only? Actually start 0 end1 means collect only at t=0. From P at (0,0) to item at (0,2) takes 2 moves, so no.
        for (int i = 1; i < 8; ++i) {
            // These items are not present in grid characters, so they are unreachable.
            items.push_back({i*2, 0, 100});
        }
        // Only the first item exists in grid at (0,2). It cannot be collected. So answer 0.
        assert(maxCollectedValue(H, W, grid, 0, 0, items) == 0);
    }
    
    return 0;
}
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// Item describes a collectible: value, start time (inclusive), end time (exclusive)
struct Item {
    int value;
    int start;
    int end;
};

// Direction vectors for 4-neighbor movement
static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = {0, -1, 0, 1};

// Global parameters to avoid passing many arguments (used only within solve function)
static int H, W, LIMIT;
static std::vector<std::string> grid;
static std::vector<Item> items;
static int memo[20][20][150][1 << 8];

// Helper: check if (x,y) is inside the grid
static inline bool inField(int x, int y) {
    return x >= 0 && x < W && y >= 0 && y < H;
}

// Helper: check if item index idx is active at time t
static inline bool isActive(int idx, int t) {
    return items[idx].start <= t && t < items[idx].end;
}

// Recursive memoized search
static int search(int x, int y, int t, int mask) {
    if (t == LIMIT) return 0;
    int &res = memo[x][y][t][mask];
    if (res != -1) return res;
    
    int best = 0;
    // For each neighbor
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!inField(nx, ny)) continue;
        char c = grid[ny][nx];
        if (c == '.') {
            // Plain move
            best = std::max(best, search(nx, ny, t + 1, mask));
        } else if (c >= '0' && c <= '7') {
            int idx = c - '0';
            // Can only move if not collected and active at this time
            if (!(mask >> idx & 1) && isActive(idx, t)) {
                int newMask = mask | (1 << idx);
                int gain = items[idx].value;
                best = std::max(best, gain + search(nx, ny, t + 1, newMask));
            }
        }
        // If c is anything else (e.g., '#'), we ignore because problem guarantees only '.' and digits are walkable
    }
    return res = best;
}

// Main solution function
int maxCollectedValue(int H_, int W_, const std::vector<std::string>& grid_,
                      int startX, int startY, const std::vector<Item>& items_) {
    H = H_;
    W = W_;
    grid = grid_;
    items = items_;
    
    // Determine LIMIT: max end time among items, plus one; if no items, set 1
    LIMIT = 1;
    for (const auto &it : items) {
        LIMIT = std::max(LIMIT, it.end + 1);
    }
    
    // Initialize memo table
    std::memset(memo, -1, sizeof(memo));
    
    // Start from the given position, time 0, no items collected
    return search(startX, startY, 0, 0);
}
// This is a state-space search problem with a small state: position (x,y), current time (0..LIMIT-1), and a bitmask of which items have been collected (up to 8 bits). The key observation is that the order and timing of collection matter because items are only available during a time window; you may also pass through an available item cell without collecting if you don't move into it? Actually you must decide: to collect an item, you must move into its cell at a time when `start <= t < end`. You can also avoid collecting by not moving into that cell. The state space has size `W*H*LIMIT*2^8`. With LIMIT ≤ 150, W,H ≤ 20, and 2^8=256, the worst case is 20*20*150*256 = 15,360,000 states, which is feasible with memoization. The transition: from state (x,y,t,bitmask), for each of the 4 neighbors, if it's outside grid, skip. If neighbor is a numbered cell `k` and `bitmask` already has bit k set, skip (cannot re-enter). If neighbor is a numbered cell `k` not yet collected and `check(t,k)` is true (i.e., `start_k <= t < end_k`), then we may move into that cell, set bit k, and add its value to the reward. If neighbor is '.', we move without reward. Importantly, the reward is collected at the moment of moving into the cell; after that, the state's bitmask includes the item and the cell becomes impassable in future (since we never enter already collected cells). We must also consider that we might want to collect multiple items in successive moves, but each move advances time by one. The base case is `t == LIMIT` returning 0. Because the same (x,y,t,bitmask) may be reached via different paths, memoization yields the optimal result. Edge cases: no items → LIMIT=1 (since max t_i+1, but if no items, maybe define LIMIT=1), answer is 0. Also ensure that when moving into an item cell at a time when it is not active, you cannot collect it; you may still move into it? According to the problem, you may only move into a numbered cell if it is not yet collected, but you can move into it even if the time is outside the active window; however, you then fail to collect it and the cell becomes "visited" (but you cannot re-enter). To simplify, the original code only allows moving into an item cell if it is not in the set and if it is active at that time; but actually it allows moving into an item cell regardless of time? Looking at the original: the first loop checks if neighbor is an item, if not collected and `check(t, neighborIndex)` is true, it sets the bit and adds cost. That means it only allows movement into that cell if it is currently collectible. If the time is outside the window, you cannot move into that cell (since the condition fails). So we adopt that: you can only move into an item cell at a time when it is collectible. This is a constraint to avoid wasting a move on a non-collectible item. Time complexity is O(W*H*LIMIT*2^8*4) per test case, which is acceptable. Space is O(W*H*LIMIT*2^8) for the memo table.
