Given an undirected unweighted graph represented as an adjacency list (vector of vectors of ints, 0-indexed nodes) and two distinct nodes `source` and `target`, write a C++ function `countChordlessPaths` that returns the number of distinct simple chordless paths from `source` to `target`. A path is a sequence of distinct nodes starting at `source` and ending at `target`. A path is *chordless* if no two non-consecutive nodes on the path are adjacent in the graph (i.e., the path is an induced path). The graph is guaranteed to be connected and have no self-loops or multiple edges. The function must enumerate all such paths without generating duplicates, even if the graph is symmetric. Handle graphs of size up to 20 nodes efficiently; the total number of chordless paths is guaranteed to fit in an `int`. The function signature must be `int countChordlessPaths(const std::vector<std::vector<int>>& graph, int source, int target)`. The implementation should be self-contained and avoid using global state. For example, for the graph with edges {0-1, 1-2, 2-3, 0-3} and source=0, target=3, the only chordless path is the direct edge 0-3 (the path 0-1-2-3 has chord 0-3; the path 0-3-2-1 is just reversed). For source=0, target=2 in the same graph, there is exactly one chordless path: 0-1-2 (since 0-3-2 is also chordless, but it is a different path; both are valid because the chord is only between non-consecutive nodes on the path itself, so 0-3-2 is chordless as well, giving 2 paths). The function must treat the paths as ordered from source to target, so the reverse path is distinct only if source and target are different, which they always are.

#include <cassert>
#include <vector>
#include <iostream>

int countChordlessPaths(const std::vector<std::vector<int>>& graph, int source, int target);

int main() {
    // Test 1: Simple path graph 0-1-2, source=0 target=2. Only one chordless path: 0-1-2.
    {
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1}};
        assert(countChordlessPaths(g, 0, 2) == 1);
    }

    // Test 2: Triangle 0-1-2-0, source=0 target=1. Direct edge 0-1 is chordless; path 0-2-1 has chord 0-1? No, 0-1 is the edge itself? Actually path 0-2-1: nodes 0,2,1; non-consecutive pair (0,1) is an edge (since triangle), so it's a chord. So only one path.
    {
        std::vector<std::vector<int>> g = {{1,2}, {0,2}, {0,1}};
        assert(countChordlessPaths(g, 0, 1) == 1);
    }

    // Test 3: Square 0-1-2-3-0, source=0 target=2. Possible chordless paths: 0-1-2 and 0-3-2. Both are chordless? Check 0-1-2: edges 0-1,1-2, non-consecutive (0,2) is not an edge (diagonal). So yes. 0-3-2: edges 0-3,3-2, non-consecutive (0,2) is not an edge. So 2 paths.
    {
        std::vector<std::vector<int>> g = {{1,3}, {0,2}, {1,3}, {0,2}};
        assert(countChordlessPaths(g, 0, 2) == 2);
    }

    // Test 4: Complete graph K4, source=0 target=3. All edges exist, so any path of length 2 or more has a chord. Only direct edge 0-3 is chordless. Paths like 0-1-3 have chord 0-3. So answer is 1.
    {
        std::vector<std::vector<int>> g = {{1,2,3}, {0,2,3}, {0,1,3}, {0,1,2}};
        assert(countChordlessPaths(g, 0, 3) == 1);
    }

    // Test 5: Star graph with center 0 and leaves 1,2,3,4. Source=1 target=2. Only path 1-0-2 is chordless? Check: nodes 1,0,2; non-consecutive pair (1,2) – there is no edge between leaves, so it's chordless. Also direct edge? No. So 1 path.
    {
        std::vector<std::vector<int>> g = {{1,2,3,4}, {0}, {0}, {0}, {0}};
        assert(countChordlessPaths(g, 1, 2) == 1);
    }

    // Test 6: Path 0-1-2-3, source=0 target=3. Only one chordless path: 0-1-2-3. Answer 1.
    {
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1,3}, {2}};
        assert(countChordlessPaths(g, 0, 3) == 1);
    }

    // Test 7: Two paths: 0-1-3 and 0-2-3, with no chords. Graph: edges 0-1,1-3,0-2,2-3. Also possibly 1-2? If we add 1-2, then path 0-1-3 has chord 1-2? No, chord is between non-consecutive nodes on path: 0-1-3 has nodes 0,1,3; non-consecutive (0,3) is not edge, so fine. But if we add 1-2, then path 0-1-2-3? Let's just test a simple case with two disjoint paths.
    {
        std::vector<std::vector<int>> g = {{1,2}, {0,3}, {0,3}, {1,2}};
        assert(countChordlessPaths(g, 0, 3) == 2);
    }

    // Test 8: Source and target adjacent with a triangle in between: 0-1,1-2,2-0, target=1, source=0. Only direct edge? Also 0-2-1? Path 0-2-1 has chord 0-1 (since 0-1 is an edge), so not chordless. So answer 1.
    {
        std::vector<std::vector<int>> g = {{1,2}, {0,2}, {0,1}};
        assert(countChordlessPaths(g, 0, 1) == 1);
    }

    // Test 9: Graph with a chordless cycle of length 4 but with an extra diagonal? Already tested square. Test a graph where multiple paths exist: e.g., a "theta" graph: nodes 0,1,2,3,4 with edges 0-1,1-3,3-4,4-2,2-0? That's a cycle. Better: two parallel paths between 0 and 4: path A: 0-1-4, path B: 0-2-3-4, and also edge 0-4 directly. Then chordless paths are: 0-4 (direct), 0-1-4, 0-2-3-4. But check 0-2-3-4: non-consecutive nodes (0,3) not edge, (2,4) not edge, (0,4) is edge? Actually 0 and 4 are adjacent because we have direct edge 0-4, so that is a chord! So 0-2-3-4 is not chordless because of the chord (0,4). Similarly 0-1-4 has chord (0,4). So only direct edge 0-4. That would be 1. For a better test, remove the direct edge: graph with two disjoint paths between 0 and 3: 0-1-3 and 0-2-3, and no other edges. Then both are chordless, answer 2. Already tested in Test 7.
    // Add a test for a pentagon: cycle 0-1-2-3-4-0, source=0 target=2. Chordless paths: 0-1-2 and 0-4-3-2. Check 0-4-3-2: nodes 0,4,3,2; non-consecutive pairs: (0,3) not edge (in pentagon), (4,2) not edge, (0,2) not edge (since pentagon has no chord). So it's chordless. Also 0-1-2 is chordless. Also maybe 0-4-3-2 is the reverse of 2-3-4-0, but we start at 0, so it's distinct. So answer 2.
    {
        std::vector<std::vector<int>> g = {{1,4}, {0,2}, {1,3}, {2,4}, {0,3}};
        assert(countChordlessPaths(g, 0, 2) == 2);
    }

    // Test 10: Disconnected? Problem says connected, but test invalid input: source=-1 should return 0.
    {
        std::vector<std::vector<int>> g = {{1}, {0}};
        assert(countChordlessPaths(g, -1, 1) == 0);
        assert(countChordlessPaths(g, 0, 0) == 0);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <functional>
#include <algorithm>

// Count the number of distinct simple chordless paths from source to target.
// A chordless path is an induced path: no two non-consecutive nodes on the path are adjacent.
// The graph is undirected, represented as adjacency lists. Nodes are 0-indexed.
// Returns 0 if source and target are the same or invalid.
int countChordlessPaths(const std::vector<std::vector<int>>& graph, int source, int target) {
    const int n = static_cast<int>(graph.size());
    if (source < 0 || source >= n || target < 0 || target >= n) return 0;
    if (source == target) return 0;

    std::vector<bool> visited(n, false);
    std::vector<int> path;
    int count = 0;

    // DFS that maintains the current induced path (path vector) and visited flags.
    // The last element of path is the current node being explored.
    std::function<void(int)> dfs = [&](int current) {
        if (current == target) {
            ++count;
            return;
        }

        for (int next : graph[current]) {
            if (visited[next]) continue;

            // Check if next is adjacent to any node on the current path except the last one.
            // path.back() == current, so we check indices 0..path.size()-2.
            bool formsChord = false;
            for (size_t i = 0; i + 1 < path.size(); ++i) {
                int earlier = path[i];
                if (std::find(graph[earlier].begin(), graph[earlier].end(), next) != graph[earlier].end()) {
                    formsChord = true;
                    break;
                }
            }
            if (formsChord) continue;

            // Extend the path with next.
            visited[next] = true;
            path.push_back(next);
            dfs(next);
            path.pop_back();
            visited[next] = false;
        }
    };

    visited[source] = true;
    path.push_back(source);
    dfs(source);
    return count;
}

// The problem is to count all simple induced paths between two vertices in a graph. A direct approach is to perform a depth-first search (DFS) from the source, maintaining the current path as a vector of nodes and a set of visited nodes. At each step, we only extend to a neighbor `v` if `v` is not visited and if adding `v` does not create a chord: we must ensure that `v` is not adjacent to any node in the current path except the last node (the one we are extending from). This is because if `v` were adjacent to an earlier node `w` on the path (other than the immediate predecessor), then the subpath from `w` to `v` would be a chord. Additionally, we must ensure that the current path itself is chordless; but that is guaranteed inductively: when we extend a chordless path with a new node that is not adjacent to any earlier path node except the last, the new path remains chordless. When we reach the target, we count it. We must avoid revisiting nodes; since we are looking for simple paths, each path uses distinct nodes. To avoid duplicate exploration, we mark nodes as visited when they are on the current path and unmark when backtracking. The graph is undirected, so we must be careful not to count both directions of the same path; but because we start at the fixed source and end at the fixed target, each unordered path corresponds to exactly one ordered sequence from source to target, so no duplication arises from symmetry. Edge cases include: when `source` and `target` are adjacent, the direct edge is a chordless path (length 1). If `source` equals `target`, return 0 because the problem assumes distinct nodes. The graph is connected, so at least one path exists, but not necessarily chordless; for example, a triangle with source and target adjacent has two chordless paths? Actually in a triangle with nodes 0,1,2, source=0, target=2: the direct edge 0-2 is chordless, and the path 0-1-2 has chord 0-2 because 0 and 2 are adjacent? In a triangle, nodes 0-2 are adjacent, so the path 0-1-2 has a chord between 0 and 2, so it is not chordless. So only the direct edge counts. The DFS branching factor is up to degree of the node, and the depth is up to the number of nodes. In the worst case, the number of induced paths can be exponential, but for n<=20 it is manageable. Time complexity is O(number of chordless paths * n) due to adjacency checks, and space is O(n) for the path and visited set. We can optimize adjacency checks using a bitset of neighbors per node for O(1) lookups, but for simplicity we use the adjacency list and check membership in the current path prefix (excluding the last node). We can keep a boolean array `onPath` to mark visited nodes, and for checking chord condition, we need to see if the candidate neighbor `v` is adjacent to any node in `path` except the last one. We can iterate over `path[0..path.size()-2]` and check if `v` is in the adjacency list of that node. This is O(path_length) per candidate. Since path length ≤ n, total time is O(n * number_of_paths * n) = O(n^2 * paths). For n=20, worst-case paths are limited (maybe ≤ 2^20), but that's acceptable. We'll implement a recursive DFS function.
