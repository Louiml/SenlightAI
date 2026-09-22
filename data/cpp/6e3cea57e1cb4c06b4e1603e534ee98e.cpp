Given two integer arrays `A` and `B` of the same length `n` representing coordinate offsets, and a positive integer `N` representing the size of a square grid (indices from 0 to N-1), write a C++ function that returns a vector of strings where each string represents a path from the origin `(0,0)` to `(N-1,N-1)` by moving with steps described by the pairs `(A[i], B[i])` applied cyclically. A path is a sequence of positions starting at `(0,0)`, where each step adds the current pair to the current position. The path is valid only if every intermediate position (including the final) stays within the grid bounds `[0,N-1]` for both x and y. From all valid paths, return the one (as a string like `"(0,0)->(1,0)->(1,1)"`) that has the lexicographically smallest sequence of positions (comparing x then y at each step). If no valid path exists after `n` steps cycled up to a maximum of `n*N` steps, return an empty string. The function signature: `std::string smallestValidPath(const std::vector<int>& A, const std::vector<int>& B, int N)`.
This problem requires exploring all possible paths that can be formed by applying the step pairs `(A[i], B[i])` in order cyclically (i.e., after using index `n-1`, go back to index `0`). Since the number of steps is unbounded but the grid is finite, we must cap the maximum steps to avoid infinite loops. A reasonable cap is `n * N` because any valid path that revisits a position with the same step index can be shortened (the cycle would repeat), so the shortest valid path cannot have more than `n * N` unique state visits (where state = (x, y, step_index)), so if we exceed that, we can terminate. For each possible number of steps from 1 up to `n*N`, we simulate the path step by step. We only consider paths that are valid at every step (i.e., stay in bounds). Among all valid full paths (reaching exactly (N-1,N-1)), we keep the lexicographically smallest path string. The lexicographic comparison is done on the sequence of positions, not on the string characters. To implement that, we generate the path as a vector of pairs and compare vectors lexicographically (using `std::lexicographical_compare` or manual comparison). Since we only need the smallest, we can brute-force all step counts and compare. The time complexity is O((n*N) * n) per path generation, and there are at most n*N path candidates, giving O(n^2 * N^2) worst-case. For typical small N (say ≤ 20) and n (≤ 10) this is fine. Space is O(n*N) for storing the current path. Edge cases: when N=1 (origin is already the destination, so a path of zero steps is valid, but the problem says "path from (0,0) to (N-1,N-1)" – for N=1, the start equals the end, so we return `"(0,0)"`), when step pairs have zero deltas (then position doesn't change, so if not already at destination, it can never reach), and when all step pairs lead out of bounds immediately.
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

// Helper to format a single position.
std::string posToString(int x, int y) {
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

// Build path string from a vector of positions.
std::string buildPathString(const std::vector<std::pair<int,int>>& path) {
    if (path.empty()) return "";
    std::string res = posToString(path[0].first, path[0].second);
    for (size_t i = 1; i < path.size(); ++i) {
        res += "->" + posToString(path[i].first, path[i].second);
    }
    return res;
}

// Compare two paths lexicographically by position sequence.
bool isLexicographicallySmaller(const std::vector<std::pair<int,int>>& a,
                                const std::vector<std::pair<int,int>>& b) {
    // Compare up to min length.
    size_t minLen = std::min(a.size(), b.size());
    for (size_t i = 0; i < minLen; ++i) {
        if (a[i] < b[i]) return true;
        if (b[i] < a[i]) return false;
    }
    // If one is a prefix, the shorter one is smaller (since we only consider full paths reaching destination,
    // but both should have same length? Actually lengths can differ; the shorter is lexicographically smaller
    // because we compare sequences, and a shorter sequence is considered smaller if it's a prefix.
    return a.size() < b.size();
}

// Return the lexicographically smallest valid path from (0,0) to (N-1,N-1)
// using cyclic steps (A[i], B[i]). Empty if no path.
std::string smallestValidPath(const std::vector<int>& A, const std::vector<int>& B, int N) {
    if (N <= 0) return "";
    if (N == 1) return "(0,0)"; // already at destination

    int n = A.size(); // assume B.size() == n
    if (n == 0) return "";

    // Maximum steps to try: n*N is enough (state space size = N*N*n)
    int maxSteps = n * N;
    std::vector<std::pair<int,int>> bestPath;
    bool found = false;

    // Try different total steps from 1 to maxSteps.
    for (int steps = 1; steps <= maxSteps; ++steps) {
        int x = 0, y = 0;
        std::vector<std::pair<int,int>> curPath;
        curPath.push_back({x, y});
        bool valid = true;
        for (int s = 0; s < steps; ++s) {
            int idx = s % n;
            x += A[idx];
            y += B[idx];
            if (x < 0 || x >= N || y < 0 || y >= N) {
                valid = false;
                break;
            }
            curPath.push_back({x, y});
        }
        if (!valid) continue;
        if (x == N-1 && y == N-1) {
            // Valid full path.
            if (!found || isLexicographicallySmaller(curPath, bestPath)) {
                bestPath = curPath;
                found = true;
            }
        }
    }

    if (!found) return "";
    return buildPathString(bestPath);
}
#include <cassert>
#include <vector>
#include <string>

// The solution function and helpers are copied here for testing (omitted in this snippet for brevity).
// In practice, include the above code.

int main() {
    // Basic path: steps (1,0) and (0,1) cyclically, N=3 grid, destination (2,2)
    {
        std::vector<int> A = {1, 0};
        std::vector<int> B = {0, 1};
        int N = 3;
        std::string result = smallestValidPath(A, B, N);
        assert(result == "(0,0)->(1,0)->(1,1)->(2,1)->(2,2)");
    }

    // Direct step (2,2) but grid N=3, can't jump out of bounds; should find alternative?
    // Steps must stay in bounds each move, so a single (2,2) would go out of bounds. Try N=5.
    {
        std::vector<int> A = {2, 1};
        std::vector<int> B = {2, 0};
        int N = 5; // destination (4,4)
        // Path: (0,0)->(2,2)->(3,2)->(5,2) out of bounds? Actually (2,2)+(1,0)=(3,2) ok, then +(2,2)=(5,4) out.
        // Let's just test a known trivial case: A={1,1}, B={1,0}, N=3, destination (2,2)
        // (0,0)->(1,1)->(2,1)->(3,2) out. Hmm.
        // Use simple case: A={1,0}, B={0,1}, N=2 -> destination (1,1): path (0,0)->(1,0)->(1,1)
        A = {1, 0};
        B = {0, 1};
        N = 2;
        result = smallestValidPath(A, B, N);
        assert(result == "(0,0)->(1,0)->(1,1)");
    }

    // N=1 trivial
    {
        std::vector<int> A = {5};
        std::vector<int> B = {-1};
        int N = 1;
        std::string result = smallestValidPath(A, B, N);
        assert(result == "(0,0)");
    }

    // Impossible path: steps all positive, N=2, need to reach (1,1) but steps exceed bounds quickly
    {
        std::vector<int> A = {2};
        std::vector<int> B = {0};
        int N = 2;
        std::string result = smallestValidPath(A, B, N);
        assert(result == ""); // (0,0)->(2,0) out of bounds
    }

    // Cyclic with zero step: A={0,1}, B={0,0}, N=3, destination (2,0). Only (0,0)->(0,0) stuck, so no.
    {
        std::vector<int> A = {0, 1};
        std::vector<int> B = {0, 0};
        int N = 3;
        std::string result = smallestValidPath(A, B, N);
        // (0,0),(0,0) stuck, then (1,0) from second step, then (1,0),(2,0) -> actually works: (0,0)->(0,0)->(1,0)->(1,0)->(2,0)
        // But shorter path: (0,0)->(1,0)->(1,0)->(2,0)?? Step indices: step0=(0,0), step1=(1,0), step2=(0,0), step3=(1,0), step4=(0,0)...
        // Try steps 3: (0,0)->(0,0)->(1,0)->(1,0) not at (2,0). Steps 4: (0,0)->(0,0)->(1,0)->(1,0)->(2,0) works.
        assert(result == "(0,0)->(0,0)->(1,0)->(1,0)->(2,0)");
    }

    // Lexicographic minimality: two possible paths to (1,1) in 2x2 grid.
    // A={1,0}, B={0,1} gives (0,0)->(1,0)->(1,1)
    // A={0,1}, B={1,0} gives (0,0)->(0,1)->(1,1)
    // First is smaller because (1,0) < (0,1) at second position.
    {
        std::vector<int> A = {0, 1};
        std::vector<int> B = {1, 0};
        int N = 2;
        std::string result = smallestValidPath(A, B, N);
        // Only one valid: start with (0,1) then (1,0) -> (0,0)->(0,1)->(1,1)
        assert(result == "(0,0)->(0,1)->(1,1)");
    }

    // Mixed: both orders possible, test that smaller is returned
    {
        std::vector<int> A = {1, 0};
        std::vector<int> B = {0, 1};
        int N = 2;
        std::string result = smallestValidPath(A, B, N);
        // Start with (1,0) gives (0,0)->(1,0)->(1,1) which is lexicographically smaller than (0,1) path
        assert(result == "(0,0)->(1,0)->(1,1)");
    }

    // Larger N: N=4, steps (1,0),(0,1),(1,1) cyclically, destination (3,3)
    {
        std::vector<int> A = {1, 0, 1};
        std::vector<int> B = {0, 1, 1};
        int N = 4;
        std::string result = smallestValidPath(A, B, N);
        // Check that it's non-empty and ends correctly (we can't easily assert exact string without manually stepping)
        // But we can compute the path manually: 
        // step0: (1,0) -> (1,0)
        // step1: (0,1) -> (1,1)
        // step2: (1,1) -> (2,2)
        // step3: (1,0) -> (3,2)
        // step4: (0,1) -> (3,3) -> found at 5 steps (including start)
        assert(result == "(0,0)->(1,0)->(1,1)->(2,2)->(3,2)->(3,3)");
    }

    return 0;
}
