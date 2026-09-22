/*
You are given an `n x n` grid (where `n` is at least 1). Each cell `(i,j)` contains a price `p[i][j]` (a positive integer). You start at cell `(0,0)` with 0 money and 0 actions taken. You may move only right (increasing column by 1) or down (increasing row by 1). Moving right from cell `(x,y)` requires paying `r[x][y]` money, and moving down from `(x,y)` requires paying `d[x][y]` money. To pay when you have insufficient money, you can earn extra money by "working" at your current cell: each working action increases your money by the maximum price you have ever visited so far (including the start cell). Working can be done any number of times at any cell before moving. Each move costs 1 action; each working session costs `k` actions where `k` is the smallest non-negative integer such that `current_money + k*max_price >= required_cost`, then `k` is added to actions and `k*max_price` is added to money. After paying the cost, you move to the adjacent cell (costing 1 action), and your `max_price` becomes the maximum of its previous value and the price of the new cell. Write a C++ function `long long minimumActions(int n, const vector<vector<long long>>& p, const vector<vector<long long>>& r, const vector<vector<long long>>& d)` that returns the minimum total actions required to reach cell `(n-1,n-1)`. If the grid is `1x1`, return 0.
*/
#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <algorithm>
#include <limits>

using ll = long long;

// Returns minimum actions to reach (n-1,n-1) from (0,0)
long long minimumActions(int n, const std::vector<std::vector<ll>>& p,
                         const std::vector<std::vector<ll>>& r,
                         const std::vector<std::vector<ll>>& d) {
    if (n == 1) return 0;

    // Collect distinct prices
    std::vector<ll> prices;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            prices.push_back(p[i][j]);
    std::sort(prices.begin(), prices.end());
    prices.erase(std::unique(prices.begin(), prices.end()), prices.end());
    int m = (int)prices.size();

    // Map price to index
    std::map<ll, int> priceToIdx;
    for (int i = 0; i < m; ++i) priceToIdx[prices[i]] = i;

    // dist[x][y][k] = min actions to be at (x,y) with max_price = prices[k]
    const ll INF = std::numeric_limits<ll>::max() / 4;
    std::vector<std::vector<std::vector<ll>>> dist(
        n, std::vector<std::vector<ll>>(n, std::vector<ll>(m, INF)));

    int startIdx = priceToIdx[p[0][0]];
    dist[0][0][startIdx] = 0;

    // Min-heap tuple: (actions, money, max_price, cell_id)
    // cell_id = x*n + y
    using State = std::tuple<ll, ll, ll, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, 0, p[0][0], 0});

    while (!pq.empty()) {
        auto [curActions, curMoney, curMaxPrice, cellId] = pq.top();
        pq.pop();

        int x = cellId / n;
        int y = cellId % n;

        // Check if this state is outdated
        if (curActions > dist[x][y][priceToIdx[curMaxPrice]]) continue;

        // Move right
        if (y < n - 1) {
            ll cost = r[x][y];
            ll actionsNeeded = 0;
            ll newMoney = curMoney;
            if (newMoney < cost) {
                ll needed = cost - newMoney;
                ll k = (needed + curMaxPrice - 1) / curMaxPrice; // ceil
                actionsNeeded += k;
                newMoney += k * curMaxPrice;
            }
            newMoney -= cost;
            ll totalActions = curActions + actionsNeeded + 1;
            ll newMaxPrice = std::max(curMaxPrice, p[x][y+1]);
            int newIdx = priceToIdx[newMaxPrice];
            int newCell = x * n + (y + 1);
            if (totalActions < dist[x][y+1][newIdx]) {
                dist[x][y+1][newIdx] = totalActions;
                pq.push({totalActions, newMoney, newMaxPrice, newCell});
            }
        }

        // Move down
        if (x < n - 1) {
            ll cost = d[x][y];
            ll actionsNeeded = 0;
            ll newMoney = curMoney;
            if (newMoney < cost) {
                ll needed = cost - newMoney;
                ll k = (needed + curMaxPrice - 1) / curMaxPrice;
                actionsNeeded += k;
                newMoney += k * curMaxPrice;
            }
            newMoney -= cost;
            ll totalActions = curActions + actionsNeeded + 1;
            ll newMaxPrice = std::max(curMaxPrice, p[x+1][y]);
            int newIdx = priceToIdx[newMaxPrice];
            int newCell = (x + 1) * n + y;
            if (totalActions < dist[x+1][y][newIdx]) {
                dist[x+1][y][newIdx] = totalActions;
                pq.push({totalActions, newMoney, newMaxPrice, newCell});
            }
        }
    }

    ll ans = INF;
    for (int k = 0; k < m; ++k)
        ans = std::min(ans, dist[n-1][n-1][k]);
    return ans;
}
#include <cassert>
#include <vector>
using ll = long long;

// Declare the function from the solution
long long minimumActions(int n, const std::vector<std::vector<ll>>& p,
                         const std::vector<std::vector<ll>>& r,
                         const std::vector<std::vector<ll>>& d);

int main() {
    // Test 1: 1x1 grid, no moves needed
    {
        int n = 1;
        std::vector<std::vector<ll>> p = {{5}};
        std::vector<std::vector<ll>> r = {};
        std::vector<std::vector<ll>> d = {};
        assert(minimumActions(n, p, r, d) == 0);
    }

    // Test 2: 2x2, all costs low, no working needed
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{1,1},{1,1}};
        std::vector<std::vector<ll>> r = {{1,1}}; // right costs from (0,0) and (1,0)
        std::vector<std::vector<ll>> d = {{1,1}}; // down costs from (0,0) and (0,1)
        // Path: right then down: cost 1+1 = 2 actions
        // Path: down then right: also 2 actions
        assert(minimumActions(n, p, r, d) == 2);
    }

    // Test 3: Need to work once because cost > money
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{1,10},{10,10}};
        std::vector<std::vector<ll>> r = {{100,1}};
        std::vector<std::vector<ll>> d = {{1,1}};
        // From (0,0) have 0 money, max_price=1. To go right cost 100: need 100 working actions, then move. Then from (0,1) have 0 money and max_price=10, cost down 1 => need 1 working action (since 10>=1? Actually 0<1, need ceil(1/10)=1), then move. Total = 100+1+1+1+1 = 104? Wait: right: working 100 actions, +1 move = 101. At (0,1) money=0, max=10, down cost 1, need 1 working action, +1 move = 2 more, total 103. Alternative: down first cost 1: need 1 working +1 move = 2, at (1,0) money=0, max=10, right cost 1: need 1 working +1 move = 2, total 4. So answer should be 4.
        assert(minimumActions(n, p, r, d) == 4);
    }

    // Test 4: Higher price helps later
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{5,1},{1,100}};
        std::vector<std::vector<ll>> r = {{10,1}};
        std::vector<std::vector<ll>> d = {{10,1}};
        // From (0,0) max=5, money=0. Right cost 10: need ceil(10/5)=2 working +1 move = 3 actions, then at (0,1) max=5? actually max(5,1)=5. Down cost 1: money after right = 0? after working 2*5=10, pay 10 -> 0, then move. Then down cost 1: need 1 working action? ceil(1/5)=1, +1 move = 2, total 5.
        // Alternative: down first cost 10: need 2 working +1 =3, at (1,0) max=5, right cost 1: need 1 working +1 =2, total 5. Both 5.
        assert(minimumActions(n, p, r, d) == 5);
    }

    // Test 5: 3x3 grid with varied prices
    {
        int n = 3;
        std::vector<std::vector<ll>> p = {{1,2,3},{4,5,6},{7,8,9}};
        // r[x][y] for moving right from (x,y) for y=0..1
        std::vector<std::vector<ll>> r = {{1,1},{1,1},{1,1}};
        // d[x][y] for moving down from (x,y) for x=0..1
        std::vector<std::vector<ll>> d = {{1,1,1},{1,1,1}};
        // All costs 1, money always enough after first? Start 0, max=1, right cost 1: need 1 working +1 move =2, then money=0, max=2, etc. Actually each move needs at least 1 working because money resets to 0 after paying, but max grows. Path length is 4 moves, each move needs working: actions = 4*(working +1). Working for first cost 1 with max=1 =>1, second cost 1 with max=2 =>1 (since needed 1, ceil(1/2)=1), third max=3=>1, fourth max=4=>1. So total = 4*(1+1)=8. Check if there's a better path? No, all paths have 4 moves, each with cost 1, money always 0 before working, max_price increases along path. Minimum working per move is ceil(1/max_price) =1 for max_price>=1. So 8.
        assert(minimumActions(n, p, r, d) == 8);
    }

    // Test 6: Large costs force many working actions
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{2,2},{2,2}};
        std::vector<std::vector<ll>> r = {{100,1}};
        std::vector<std::vector<ll>> d = {{100,1}};
        // From (0,0) max=2, need cost 100: ceil(100/2)=50 working +1 move =51. After paying, money=0, max=2. Next move cost 1: need 1 working +1 move =2. Total 53. Any path same.
        assert(minimumActions(n, p, r, d) == 53);
    }

    // Test 7: Already enough money from previous cell
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{10,10},{10,10}};
        std::vector<std::vector<ll>> r = {{5,1}};
        std::vector<std::vector<ll>> d = {{5,1}};
        // Start with 0 money, max=10. Right cost 5: need 0 working (0>=5? no, but 0<5, need ceil(5/10)=1 working) actually still need 1. But if we go down then right, same. So each move costs 1 working +1 move. Total 4 actions for 2 moves. But wait, if we could earn money? No, working only when needed. So answer 4.
        assert(minimumActions(n, p, r, d) == 4);
    }

    // Test 8: No working ever needed if start money is enough? But start 0, so always need at least one working for first move if cost>0. Ensure correct.
    {
        int n = 2;
        std::vector<std::vector<ll>> p = {{1,1},{1,1}};
        std::vector<std::vector<ll>> r = {{0,1}};
        std::vector<std::vector<ll>> d = {{0,1}};
        // Right from (0,0) cost 0: no working, move =1 action, money stays 0, max=1. Down cost 1: need 1 working +1 move =2. Total 3. Alternative down first cost 0 (1 action), then right cost 1 (2 actions) => 3.
        assert(minimumActions(n, p, r, d) == 3);
    }

    return 0;
}
// This is a shortest-path problem on a state space where each state is defined by the current cell `(x,y)` and the current `max_price` value. However, `max_price` can take only values that are among the distinct prices in the grid (since it is always the maximum of visited prices). We compress these distinct prices into indices. For each cell and each possible `max_price` index, we store the minimum number of actions needed to reach that state. We use a priority queue (Dijkstra) because each action (working or moving) has positive cost (at least 1). Initial state: at `(0,0)`, actions=0, money=0, max_price=p[0][0]. From any state, two possible moves (right and down, if within bounds). For a move requiring cost `c`, we compute `k = ceil((c - current_money) / max_price)` but only if current_money < c; otherwise k=0. Then new_actions = old_actions + k + 1 (the +1 for the move itself). new_money = current_money + k*max_price - c. Then we relax to the neighbor state with updated max_price = max(old_max_price, p[neighbor]). If the new actions is less than the stored distance for that state, update and push. At the end, the answer is the minimum distance among all max_price indices at `(n-1,n-1)`. Edge cases: `n=1` returns 0; all costs are positive; max_price is always at least 1 because prices are positive. Time complexity: Each cell can be visited with each distinct price index, so at most `n^2 * m` states where `m` is number of distinct prices (≤ n^2). Each state generates at most 2 edges. Each heap operation is O(log states). So overall O(n^2 * m * log(n^2 * m)) ~ O(n^4 log n) worst-case, which is acceptable for n up to about 50 in typical competitive settings. Space O(n^2 * m).
