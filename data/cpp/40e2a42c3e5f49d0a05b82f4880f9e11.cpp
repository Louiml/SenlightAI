// Implement a C++ function `processDoubleEndedList` that simulates a doubly linked list supporting five operations: insert at left end (`L x`), insert at right end (`R x`), delete the k-th inserted element (`D k`), insert a new element to the immediate left of the k-th inserted element (`IL k x`), and insert a new element to the immediate right of the k-th inserted element (`IR k x`). The function receives an integer `m` representing the number of operations, followed by `m` strings/values as specified, and returns a string containing the final list elements from left to right, separated by spaces. The list is initially empty. Elements are indexed by insertion order starting at 1. Demonstrate correctness with `assert` tests.
The solution uses a doubly linked list with sentinel head (node 0) and tail (node 1) to simplify boundary operations. Each node is stored in static arrays `e` (value), `l` (left pointer), `r` (right pointer), and an `idx` counter starting at 2. Initialization sets `r[0]=1` and `l[1]=0`. For left insert, we call `insertR(0,x)` which adds a new node after the head. For right insert, we call `insertL(1,x)` which adds before the tail. For deletion, we call `dele(k+1)` because node indices are offset by 2 (dummy head at 0, dummy tail at 1, first inserted element at 2). For IL and IR, we offset `k` by 1 as well. The insertion and deletion functions update the four relevant links in constant time. After processing all operations, we traverse from `r[0]` until reaching the tail sentinel `1`, collecting each element. Edge cases: deleting the only element, inserting before/after the first/last element, and repeated operations. Time complexity is O(m) total, space O(m) for the arrays (since each insertion adds a node). The solution is robust and follows the original snippet's logic exactly.
#include <string>
#include <vector>

// Simulates a doubly linked list with operations L, R, D, IL, IR.
// Input: m = number of operations, ops = vector of operation strings.
// For D/IL/IR, the operation string includes k and possibly x as trailing integers.
// Returns a string of final list values separated by spaces.
std::string processDoubleEndedList(int m, const std::vector<std::string>& ops) {
    const int MAXN = 100010;
    static int e[MAXN], l[MAXN], r[MAXN];
    int idx = 2; // 0 = head dummy, 1 = tail dummy
    r[0] = 1;
    l[1] = 0;

    auto insertR = [&](int k, int x) {
        e[idx] = x;
        l[idx] = k;
        r[idx] = r[k];
        l[r[k]] = idx;
        r[k] = idx;
        idx++;
    };

    auto insertL = [&](int k, int x) {
        e[idx] = x;
        l[idx] = l[k];
        r[idx] = k;
        r[l[k]] = idx;
        l[k] = idx;
        idx++;
    };

    auto dele = [&](int k) {
        r[l[k]] = r[k];
        l[r[k]] = l[k];
    };

    for (int i = 0; i < m; ++i) {
        const std::string& op = ops[i];
        if (op == "L") {
            int x = std::stoi(ops[++i]);
            insertR(0, x);
        } else if (op == "R") {
            int x = std::stoi(ops[++i]);
            insertL(1, x);
        } else if (op == "D") {
            int k = std::stoi(ops[++i]);
            dele(k + 1);
        } else if (op == "IL") {
            int k = std::stoi(ops[++i]);
            int x = std::stoi(ops[++i]);
            insertL(k + 1, x);
        } else if (op == "IR") {
            int k = std::stoi(ops[++i]);
            int x = std::stoi(ops[++i]);
            insertR(k + 1, x);
        }
    }

    std::string result;
    for (int cur = r[0]; cur != 1; cur = r[cur]) {
        if (!result.empty()) result += " ";
        result += std::to_string(e[cur]);
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// (Include the solution function here)

int main() {
    // Test 1: Empty list
    assert(processDoubleEndedList(0, {}) == "");

    // Test 2: Only left/right inserts
    std::vector<std::string> ops2 = {"L", "1", "R", "2", "L", "3"};
    assert(processDoubleEndedList(ops2.size(), ops2) == "3 1 2");

    // Test 3: Delete middle
    std::vector<std::string> ops3 = {"L", "10", "R", "20", "R", "30", "D", "2"};
    assert(processDoubleEndedList(ops3.size(), ops3) == "10 30");

    // Test 4: Insert left of first (IL with k=1)
    std::vector<std::string> ops4 = {"R", "5", "IL", "1", "7"};
    assert(processDoubleEndedList(ops4.size(), ops4) == "7 5");

    // Test 5: Insert right of last (IR with k=2 after two inserts)
    std::vector<std::string> ops5 = {"L", "1", "L", "2", "IR", "2", "9"};
    assert(processDoubleEndedList(ops5.size(), ops5) == "2 1 9");

    // Test 6: Delete only element
    std::vector<std::string> ops6 = {"R", "42", "D", "1"};
    assert(processDoubleEndedList(ops6.size(), ops6) == "");

    // Test 7: Mixed operations
    std::vector<std::string> ops7 = {"L", "1", "R", "2", "L", "3", "IR", "2", "4", "IL", "3", "5", "D", "4"};
    assert(processDoubleEndedList(ops7.size(), ops7) == "3 5 1 4");

    // Test 8: Repeated deletes and inserts
    std::vector<std::string> ops8 = {"R", "10", "R", "20", "D", "1", "L", "30", "R", "40", "D", "2"};
    assert(processDoubleEndedList(ops8.size(), ops8) == "30 20");

    // Test 9: Large values
    std::vector<std::string> ops9 = {"L", "1000000", "R", "-1000000"};
    assert(processDoubleEndedList(ops9.size(), ops9) == "1000000 -1000000");

    // Test 10: Sequential left then right
    std::vector<std::string> ops10 = {"L", "1", "L", "2", "L", "3"};
    assert(processDoubleEndedList(ops10.size(), ops10) == "3 2 1");

    return 0;
}
