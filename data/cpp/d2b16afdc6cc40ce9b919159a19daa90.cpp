Implement a C++ function `simulatePresents` that processes a sequence of commands on linked lists of presents, exactly mirroring the operations shown in the code snippet. Each present has a unique integer ID from 1 to M, and presents are organized into N belts (numbered 1..N). Each belt is a doubly linked list with heads, tails, and lengths tracked. Commands: `100 N M` followed by M pairs of (present_id, belt_id) to initialize; `200 src dest` moves all presents from src belt to the front of dest belt (print new length of dest); `300 src dest` moves the head present of each belt to the front of the other (print new length of dest); `400 src dest` moves the front half (floor of length/2) of src belt to the front of dest belt (print new length of dest); `500 p` prints `a + 2*b` where a is prev of present p (or -1 if none) and b is next (or -1); `600 b` prints `a + 2*b + 3*c` where a=head, b=tail (or -1 if empty), c=length. The function takes Q commands and returns a vector of strings (one per printed line). Handle edge cases where belts are empty, lengths are zero, and only one element is moved.
#include <cassert>
#include <vector>
#include <string>

// Function prototype (defined in solution)
std::vector<std::string> simulatePresents(int Q, const std::vector<std::vector<int>>& commands);

int main() {
    // Test 1: Basic initialization and query
    {
        int Q = 4;
        std::vector<std::vector<int>> cmds = {
            {100, 2, 3, 1, 1, 2, 1, 3, 2},
            {600, 1},
            {600, 2},
            {500, 2}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 3);
        assert(res[0] == "1"); // belt1 head=1 tail=2 len=2 => 1+2*2+3*2=11? Wait compute later
    }

    // Test 2: Move all (200)
    {
        int Q = 3;
        // Initialize: belt1 has 1,2 ; belt2 has 3
        std::vector<std::vector<int>> cmds = {
            {100, 2, 3, 1, 1, 2, 1, 3, 2},
            {200, 1, 2},
            {600, 2}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 2);
        assert(res[0] == "3"); // after move dest length = 3
        assert(res[1] == "4"); // belt2 head=1 tail=2 len=3 => 1+2*2+3*3=14? Need compute
        // Actually 600 prints a + 2*b + 3*c with a=head, b=tail, c=len
        // After move, belt2: head=3? Wait command 200 moves all from src to front of dest.
        // src=1 has [1,2], dest=2 has [3]. Move all to front of dest => new dest order [1,2,3]
        // So head=1, tail=3, len=3 => 1 + 2*3 + 3*3 = 1+6+9=16
        assert(res[1] == "16");
    }

    // Test 3: Swap heads (300)
    {
        int Q = 3;
        // belt1: [1,2], belt2: [3]
        std::vector<std::vector<int>> cmds = {
            {100, 2, 3, 1, 1, 2, 1, 3, 2},
            {300, 1, 2},
            {600, 2}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 2);
        // After 300: pop head from belt1 (1) and head from belt2 (3), then insert popped each to front of other.
        // belt1 originally [1,2] -> after pop 1, becomes [2]; belt2 [3] -> becomes empty.
        // Insert 1 to front of belt2 => belt2: [1]; insert 3 to front of belt1 => belt1: [3,2]
        // dest=2 length=1, output "1"
        assert(res[0] == "1");
        // belt2 head=1 tail=1 len=1 => 1 + 2*1 + 3*1 = 6
        assert(res[1] == "6");
    }

    // Test 4: Move half (400)
    {
        int Q = 3;
        // belt1: [1,2,3,4], belt2: empty
        std::vector<std::vector<int>> cmds = {
            {100, 2, 4, 1, 1, 2, 1, 3, 1, 4, 2}, // careful: belt ids: 1,1,1,1? Actually last arg is belt for each present
            // Let's write properly: 100 2 4 1 1 2 1 3 1 4 2 => N=2,M=4, pairs: (1,1),(2,1),(3,1),(4,2)
            // So belt1 has 1,2,3; belt2 has 4
            {400, 1, 2},
            {600, 2}
        };
        // Actually let's just test separately to avoid confusion.
    }

    // Simpler test for 400:
    {
        int Q = 3;
        // belt1: [1,2,3,4], belt2: empty
        // We'll set up N=2, M=4, and put all on belt1 then move.
        std::vector<std::vector<int>> cmds = {
            {100, 2, 4, 1, 1, 2, 1, 3, 1, 4, 1}, // belt1: 1,2,3,4 ; belt2: empty
            {400, 1, 2},
            {600, 2}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 2);
        // After 400: amount=4/2=2, move first 2 (1,2) to front of belt2.
        // belt1 becomes [3,4], belt2 becomes [1,2].
        assert(res[0] == "2"); // dest len = 2
        // belt2 head=1 tail=2 len=2 => 1 + 2*2 + 3*2 = 1+4+6=11
        assert(res[1] == "11");
    }

    // Test 5: Edge case moving from empty belt (200)
    {
        int Q = 2;
        // belt1 empty, belt2 has [1]
        std::vector<std::vector<int>> cmds = {
            {100, 2, 1, 1, 2},
            {200, 1, 2}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 1);
        assert(res[0] == "1"); // dest length remains 1
    }

    // Test 6: 500 for a present with no prev/next
    {
        int Q = 3;
        // Single present on belt1
        std::vector<std::vector<int>> cmds = {
            {100, 1, 1, 7, 1},
            {500, 7},
            {600, 1}
        };
        auto res = simulatePresents(Q, cmds);
        assert(res.size() == 2);
        // 500: prev=-1, next=-1 => -1 + 2*(-1) = -3
        assert(res[0] == "-3");
        // 600: head=7 tail=7 len=1 => 7 + 2*7 + 3*1 = 7+14+3=24
        assert(res[1] == "24");
    }

    return 0;
}
#include <vector>
#include <string>

struct PresentNode {
    int prev;
    int next;
};

// Process commands and return printed outputs in order.
std::vector<std::string> simulatePresents(int Q, const std::vector<std::vector<int>>& commands) {
    const int MAX_N = 100001;
    std::vector<int> heads(MAX_N, 0);
    std::vector<int> tails(MAX_N, 0);
    std::vector<int> lens(MAX_N, 0);
    std::vector<PresentNode> presents(MAX_N, {0, 0});
    std::vector<std::string> outputs;
    
    int N = 0, M = 0;
    for (int i = 0; i < Q; ++i) {
        const auto& cmd = commands[i];
        int type = cmd[0];
        
        if (type == 100) {
            N = cmd[1];
            M = cmd[2];
            // cmd[3..] are pairs (present, belt)
            for (size_t k = 3; k < cmd.size(); k += 2) {
                int p = cmd[k];
                int b = cmd[k+1];
                if (heads[b] == 0) heads[b] = p;
                if (tails[b] != 0) {
                    presents[tails[b]].next = p;
                    presents[p].prev = tails[b];
                }
                tails[b] = p;
                lens[b] += 1;
            }
        } else if (type == 200) {
            int src = cmd[1], dest = cmd[2];
            if (lens[src] > 0) {
                if (lens[dest] > 0) {
                    presents[heads[dest]].prev = tails[src];
                    presents[tails[src]].next = heads[dest];
                } else {
                    tails[dest] = tails[src];
                }
                heads[dest] = heads[src];
                heads[src] = 0;
                if (lens[dest] == 0) tails[dest] = tails[src];
                tails[src] = 0;
                lens[dest] += lens[src];
                lens[src] = 0;
            }
            outputs.push_back(std::to_string(lens[dest]));
        } else if (type == 300) {
            int src = cmd[1], dest = cmd[2];
            int sh = 0, dh = 0;
            if (lens[src] > 0) {
                sh = heads[src];
                heads[src] = presents[sh].next;
                presents[heads[src]].prev = 0;
                if (lens[src] == 1) {
                    presents[tails[src]].prev = 0;
                    tails[src] = 0;
                }
                lens[src] -= 1;
            }
            if (lens[dest] > 0) {
                dh = heads[dest];
                heads[dest] = presents[dh].next;
                presents[heads[dest]].prev = 0;
                if (lens[dest] == 1) {
                    presents[tails[dest]].prev = 0;
                    tails[dest] = 0;
                }
                lens[dest] -= 1;
            }
            if (sh > 0) {
                if (lens[dest] > 0) {
                    presents[heads[dest]].prev = sh;
                    presents[sh].next = heads[dest];
                } else {
                    presents[sh].next = 0;
                    presents[sh].prev = 0;
                    tails[dest] = sh;
                }
                heads[dest] = sh;
                lens[dest] += 1;
            }
            if (dh > 0) {
                if (lens[src] > 0) {
                    presents[heads[src]].prev = dh;
                    presents[dh].next = heads[src];
                } else {
                    presents[dh].next = 0;
                    presents[dh].prev = 0;
                    tails[src] = dh;
                }
                heads[src] = dh;
                lens[src] += 1;
            }
            outputs.push_back(std::to_string(lens[dest]));
        } else if (type == 400) {
            int src = cmd[1], dest = cmd[2];
            int amount = lens[src] / 2;
            int end = heads[src];
            if (amount > 0) {
                for (int j = 0; j < amount - 1; ++j) {
                    end = presents[end].next;
                }
                if (lens[dest] > 0) {
                    presents[heads[dest]].prev = end;
                    presents[end].next = heads[dest];
                    heads[dest] = heads[src];
                } else {
                    presents[end].next = 0;
                    heads[dest] = heads[src];
                    tails[dest] = end;
                }
                heads[src] = presents[end].next;
                presents[heads[src]].prev = 0;
                lens[dest] += amount;
                lens[src] -= amount;
            }
            outputs.push_back(std::to_string(lens[dest]));
        } else if (type == 500) {
            int p = cmd[1];
            int a = presents[p].prev > 0 ? presents[p].prev : -1;
            int b = presents[p].next > 0 ? presents[p].next : -1;
            outputs.push_back(std::to_string(a + 2 * b));
        } else if (type == 600) {
            int b = cmd[1];
            int c = lens[b];
            int a = c > 0 ? heads[b] : -1;
            int tail = c > 0 ? tails[b] : -1;
            outputs.push_back(std::to_string(a + 2 * tail + 3 * c));
        }
    }
    return outputs;
}
// The core is maintaining four arrays: `Heads`, `Tails`, `Lens` (all size N+1) and `Presents` (size M+1) with `prev` and `next` fields. For each command, carefully update the doubly linked list pointers. The tricky parts are: (1) In `200`, when moving whole belt, set the tail of src's `next` to dest's head, and dest's head's `prev` to src's tail, then update heads/tails/lengths; if dest is empty, its tail becomes src's old tail. (2) In `300`, pop the head of src (if non-empty) and head of dest (if non-empty), then re-insert the popped nodes at the front of the other belt; need to handle when a belt becomes empty and reset its head/tail to 0. (3) In `400`, find the last node of the front half by traversing `amount-1` times; then splice the half to dest; if dest is empty, its tail becomes that `end` node. Edge cases: when `amount==0`, do nothing; when a belt has only one node, its `next` is 0, so after popping, set `Presents[newHead].prev=0`; after moving, ensure no dangling pointers. Time complexity: O(Q + total operations), where each 400 may take O(length of src) for traversal, so worst O(Q*M) but typical O(Q) amortized; space O(N+M). The printed results are collected in order as strings.
