You are given a rectangular grid of size \( n \times m \) representing a building floor plan. The grid cells can be: `.` (empty space), `*` (wall), `$` (a document to collect), uppercase letters `A`–`Z` (locked doors, where the corresponding lowercase letter is the key), and lowercase letters `a`–`z` (keys that open the corresponding uppercase letter door). You start outside the building at any of the border cells, which are all considered accessible empty space (treat the outside as a one-cell-thick border of `.` around the grid). You may move up, down, left, or right through empty cells, keys, documents, and doors only if you already have the matching key. Initially you are given a string of keys (possibly empty), and each key can be used unlimited times. Write a C++ function that takes the grid, the number of rows `n`, the number of columns `m`, and the initial keys string, and returns the maximum number of documents (`$`) you can collect. The grid will always be surrounded by a border of at least one cell of `.` representing the outside, and you can enter through any border cell. The function must handle multiple test cases in a loop, but as a standalone function it processes a single case.

The solution uses a BFS that starts from the outside border (an artificial border of size `n+2` by `m+2` where all border cells are `.`). The visited array covers this extended grid. The algorithm maintains two queues: the main BFS queue `q1` for cells to process, and an array of queues `q2[26]` for each uppercase letter to store door positions that were encountered but could not be opened due to missing keys. When a key is found, we mark it as owned and push the key cell into the main queue; then we transfer all queued door cells for that key from `q2` into `q1` because they become accessible. For each cell popped, we inspect its four neighbors: skip walls and visited cells; if it's `.` or `$` we mark visited and push; if it's an uppercase door, we mark visited, and if the key is owned we push, otherwise we store in `q2`; if it's a lowercase key, we mark visited, and if it's a new key we update keys and move its queued doors. Documents are counted when first encountered. Edge cases: initial keys may be empty string or "0" (the original code treats "0" as no keys); the grid may have doors without keys, so those are stored and later opened if the key appears later; keys can appear multiple times; the outside border allows starting from any point, so the BFS begins from one artificial corner with an extended grid. Time complexity is \(O((n+2)(m+2))\) because each cell is visited at most once and each door is moved at most once, and space is \(O((n+2)(m+2) + 26)\) for the visited array and queues.

#include <queue>
#include <vector>
#include <string>
#include <cstring>

// Coordinate structure for BFS.
struct Cell {
    int y, x;
};

// Count the maximum number of documents collectable in a building grid.
// grid: vector of strings of size n, each of length m, containing '.', '*', '$', 'A'-'Z', 'a'-'z'.
// n, m: dimensions of the grid (>=1).
// initialKeys: string of lowercase letters; an empty string or "0" means no keys.
int collectDocuments(const std::vector<std::string>& grid, int n, int m, const std::string& initialKeys) {
    const int rows = n + 2;  // include outside border top/bottom
    const int cols = m + 2;  // include outside border left/right

    // Build extended grid with border of '.'.
    char a[105][105];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                a[i][j] = '.';
            } else {
                a[i][j] = grid[i - 1][j - 1];
            }
        }
    }

    // Key ownership.
    bool key[26];
    std::memset(key, false, sizeof(key));
    if (!initialKeys.empty() && initialKeys != "0") {
        for (char ch : initialKeys) {
            if (ch >= 'a' && ch <= 'z') {
                key[ch - 'a'] = true;
            }
        }
    }

    // Visited markers.
    bool visited[105][105];
    std::memset(visited, false, sizeof(visited));

    // BFS queues: main queue and per-door queues.
    std::queue<Cell> q1;
    std::queue<Cell> q2[26];

    // Start from the top-left artificial border corner.
    visited[0][0] = true;
    q1.push({0, 0});

    int dy[4] = {0, 0, 1, -1};
    int dx[4] = {1, -1, 0, 0};

    int documentCount = 0;

    while (!q1.empty()) {
        Cell c = q1.front();
        q1.pop();
        int y = c.y;
        int x = c.x;

        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny < 0 || nx < 0 || ny >= rows || nx >= cols) continue;
            if (a[ny][nx] == '*') continue;
            if (visited[ny][nx]) continue;

            visited[ny][nx] = true;

            char ch = a[ny][nx];
            if (ch == '.') {
                q1.push({ny, nx});
            } else if (ch >= 'A' && ch <= 'Z') {
                int doorIdx = ch - 'A';
                if (key[doorIdx]) {
                    q1.push({ny, nx});
                } else {
                    q2[doorIdx].push({ny, nx});
                }
            } else if (ch >= 'a' && ch <= 'z') {
                int keyIdx = ch - 'a';
                if (!key[keyIdx]) {
                    key[keyIdx] = true;
                    // Transfer all previously locked doors for this key.
                    while (!q2[keyIdx].empty()) {
                        q1.push(q2[keyIdx].front());
                        q2[keyIdx].pop();
                    }
                }
                q1.push({ny, nx});
            } else if (ch == '$') {
                ++documentCount;
                q1.push({ny, nx});
            }
        }
    }

    return documentCount;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (not repeated here).

int main() {
    // Test 1: Basic path with no keys.
    {
        std::vector<std::string> grid = {
            "...$.",
            ".$.*.",
            "....."
        };
        int n = 3, m = 5;
        assert(collectDocuments(grid, n, m, "0") == 2);
    }

    // Test 2: Door unlocks with initial key.
    {
        std::vector<std::string> grid = {
            "aA$",
            "...$"
        };
        int n = 2, m = 3;
        // initial key 'a' opens door 'A'
        assert(collectDocuments(grid, n, m, "a") == 2);
        assert(collectDocuments(grid, n, m, "0") == 0); // cannot enter door
    }

    // Test 3: Key found inside unlocks a previously blocked door.
    {
        std::vector<std::string> grid = {
            ".a..",
            ".A.$",
            "...."
        };
        int n = 3, m = 4;
        // Start outside, reach key 'a', then open door 'A', get doc
        assert(collectDocuments(grid, n, m, "0") == 1);
    }

    // Test 4: Multiple doors of same letter after key discovery.
    {
        std::vector<std::string> grid = {
            ".b..",
            ".B.B",
            "...$",
            "...."
        };
        int n = 4, m = 4;
        assert(collectDocuments(grid, n, m, "b") == 1);
        assert(collectDocuments(grid, n, m, "0") == 0);
    }

    // Test 5: All walls and no documents.
    {
        std::vector<std::string> grid = {
            "***",
            "*.*",
            "***"
        };
        int n = 3, m = 3;
        assert(collectDocuments(grid, n, m, "0") == 0);
    }

    // Test 6: Key appears after door is enqueued (door stored, then opened).
    {
        std::vector<std::string> grid = {
            "..A",
            "..a",
            "$.."
        };
        int n = 3, m = 3;
        // Start outside, reach 'a' first, open 'A', then reach '$'
        assert(collectDocuments(grid, n, m, "0") == 1);
    }

    // Test 7: Multiple documents in separate accessible branches.
    {
        std::vector<std::string> grid = {
            ".$.$",
            "....",
            "$a$",
            ".A.",
            "..."
        };
        int n = 5, m = 3;
        // Use key 'a' to open door 'A', collect all 4 documents
        assert(collectDocuments(grid, n, m, "a") == 4);
        assert(collectDocuments(grid, n, m, "0") == 3); // only one branch without key
    }

    // Test 8: Single cell open area with doc.
    {
        std::vector<std::string> grid = {"$"};
        int n = 1, m = 1;
        // Border allows entry into the only cell
        assert(collectDocuments(grid, n, m, "0") == 1);
    }

    // Test 9: Large border with initial key 'z' but no relevant doors.
    {
        std::vector<std::string> grid = {
            "..$..",
            ".$.$.",
            "..$.."
        };
        int n = 3, m = 5;
        assert(collectDocuments(grid, n, m, "z") == 3);
    }

    // Test 10: Door that is physically unreachable even with key.
    {
        std::vector<std::string> grid = {
            "...",
            ".A*",
            ".*."
        };
        int n = 3, m = 3;
        assert(collectDocuments(grid, n, m, "a") == 0); // door surrounded by walls except unreachable
    }

    return 0;
}
