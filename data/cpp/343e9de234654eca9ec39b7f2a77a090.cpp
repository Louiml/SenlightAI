// Write a C++ function `analyzeGraph(int type, const std::vector<std::vector<int>>& adjMatrix)` that takes an integer `type` (either 1 or 2) and a square adjacency matrix of size `n x n` (where `n` is inferred from the matrix dimensions). The matrix entries are non‑negative integers, with a value of `0` meaning no direct edge, a value greater than `50` meaning an “invalid” edge (should be ignored), and values in the range `[1, 50]` representing valid weighted edges. For `type == 1`, the function must return a vector of strings, where each string for row `i` (1‑based) is `"inDegree outDegree"` — here `inDegree` for vertex `i` is the number of valid edges pointing **into** vertex `i` (i.e., matrix entries `a[j][i]` in the valid range), and `outDegree` is the number of valid edges pointing **out** of vertex `i` (i.e., matrix entries `a[i][j]` in the valid range). For `type == 2`, the function must return a string in the format:  
// `"n m\n"` followed by `m` lines, each containing three integers: `u v w`, where `u` and `v` are 1‑based vertex indices and `w` is the weight of a valid edge (entries in the valid range), listed in row‑major order (for each row from 1 to n, and for each column from 1 to n). The function must handle `n` between 1 and 100, and any values outside the valid edge range are ignored. The return type for type 1 is `std::vector<std::string>`, and for type 2 is `std::string` — the function should return an appropriate structure depending on `type`.

// The main algorithm is straightforward: iterate over all matrix entries exactly once. For each cell `(i, j)` (using 0‑based indices internally), check if the value is strictly greater than 0 and less than or equal to 50. If so, it is a valid directed edge from vertex `i+1` to vertex `j+1`. For type 1, maintain two arrays `inDeg` and `outDeg` of size `n` (initialised to 0). When a valid edge is found at `(i,j)`, increment `outDeg[i]` and `inDeg[j]`. After scanning the whole matrix, build the output vector of strings, where for each vertex `k` (1‑based) the string is `"inDeg[k] outDeg[k]"` (note the output order per the original snippet: first incoming, then outgoing). For type 2, maintain a counter `cnt` for the number of valid edges, and while scanning, accumulate triples in a vector of tuples. After the scan, construct the output string: first line `"n cnt"`, then each triple as `"u v w"` in row‑major order (which is exactly the order we encounter them). Edge cases: `n` can be 1 (then only self-loops might be valid); a value of 0 or any value >50 is ignored; the matrix might contain no valid edges, in which case `cnt` is 0 and the output for type 2 is `"n 0"` with no following lines. Time complexity is O(n²) for both types, and auxiliary space is O(n) for type 1 (the two degree arrays) and O(cnt) for type 2 (storing edges), which is at most O(n²). The function uses `const` references to avoid copying the matrix.

#include <string>
#include <vector>

// Analyze a directed graph represented by an adjacency matrix.
// type == 1: return per-vertex in-degree and out-degree (as strings "in out").
// type == 2: return a string with n, m, and all m edges (u v w) in row-major order.
// Valid edges have weight in [1, 50]; all other entries are ignored.
std::vector<std::string> analyzeGraph(
    int type,
    const std::vector<std::vector<int>>& adjMatrix
) {
    const int n = static_cast<int>(adjMatrix.size());
    std::vector<std::string> result_type1;
    std::string result_type2;
    if (type == 1) {
        std::vector<int> inDeg(n, 0);
        std::vector<int> outDeg(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] > 0 && adjMatrix[i][j] <= 50) {
                    ++outDeg[i];
                    ++inDeg[j];
                }
            }
        }
        result_type1.reserve(n);
        for (int i = 0; i < n; ++i) {
            result_type1.push_back(
                std::to_string(inDeg[i]) + " " + std::to_string(outDeg[i])
            );
        }
        return result_type1;
    } else {
        int cnt = 0;
        std::vector<std::tuple<int, int, int>> edges;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] > 0 && adjMatrix[i][j] <= 50) {
                    ++cnt;
                    edges.emplace_back(i + 1, j + 1, adjMatrix[i][j]);
                }
            }
        }
        result_type2 = std::to_string(n) + " " + std::to_string(cnt) + "\n";
        for (const auto& edge : edges) {
            result_type2 += std::to_string(std::get<0>(edge)) + " " +
                            std::to_string(std::get<1>(edge)) + " " +
                            std::to_string(std::get<2>(edge)) + "\n";
        }
        return result_type2;  // Note: return type is vector<string>, but we return a string.
    }
}
*Correction*: The function signature in the task says it returns `std::vector<std::string>` for both types, but for type 2 we need a single string. To reconcile, I will define the function to return `std::pair<std::vector<std::string>, std::string>` where one is empty. Alternatively, the task allows returning different types — I will adjust the solution to use an `std::variant` or overload. For clarity, I’ll provide two separate functions: `analyzeDegrees` and `analyzeEdgesString`. However, to strictly follow the task, I’ll use `std::variant`. Below is the corrected, runnable solution.

#include <string>
#include <vector>
#include <variant>
#include <tuple>

// Analyze a directed graph represented by an adjacency matrix.
// Returns either a vector of strings (type=1) or a string (type=2).
// Valid edges have weight in [1, 50]; all other entries are ignored.
std::variant<std::vector<std::string>, std::string> analyzeGraph(
    int type,
    const std::vector<std::vector<int>>& adjMatrix
) {
    const int n = static_cast<int>(adjMatrix.size());
    if (type == 1) {
        std::vector<int> inDeg(n, 0);
        std::vector<int> outDeg(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] > 0 && adjMatrix[i][j] <= 50) {
                    ++outDeg[i];
                    ++inDeg[j];
                }
            }
        }
        std::vector<std::string> result;
        result.reserve(n);
        for (int i = 0; i < n; ++i) {
            result.push_back(
                std::to_string(inDeg[i]) + " " + std::to_string(outDeg[i])
            );
        }
        return result;
    } else {
        int cnt = 0;
        std::string result = std::to_string(n) + " " + std::to_string(0) + "\n";
        std::string edgesStr;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] > 0 && adjMatrix[i][j] <= 50) {
                    ++cnt;
                    edgesStr += std::to_string(i + 1) + " " +
                                std::to_string(j + 1) + " " +
                                std::to_string(adjMatrix[i][j]) + "\n";
                }
            }
        }
        result = std::to_string(n) + " " + std::to_string(cnt) + "\n" + edgesStr;
        return result;
    }
}

#include <cassert>
#include <string>
#include <vector>
#include <variant>

int main() {
    // Example from the problem statement
    std::vector<std::vector<int>> adj = {
        {0, 1, 10000, 2},
        {10000, 0, 3, 4},
        {5, 6, 0, 10000},
        {10000, 10000, 7, 0}
    };

    auto res1 = std::get<std::vector<std::string>>(analyzeGraph(1, adj));
    assert((res1 == std::vector<std::string>{"1 2", "2 2", "2 2", "2 1"}));

    auto res2 = std::get<std::string>(analyzeGraph(2, adj));
    assert(res2 == "4 7\n1 2 1\n1 4 2\n2 3 3\n2 4 4\n3 1 5\n3 2 6\n4 3 7\n");

    // Case: no valid edges (all 0 or >50)
    std::vector<std::vector<int>> adj2 = {
        {0, 100, 0},
        {0, 0, 0},
        {10000, 0, 0}
    };
    auto res1b = std::get<std::vector<std::string>>(analyzeGraph(1, adj2));
    assert((res1b == std::vector<std::string>{"0 0", "0 0", "0 0"}));
    auto res2b = std::get<std::string>(analyzeGraph(2, adj2));
    assert(res2b == "3 0\n");

    // Case: 1 vertex, self-loop valid
    std::vector<std::vector<int>> adj3 = {{50}};
    auto res1c = std::get<std::vector<std::string>>(analyzeGraph(1, adj3));
    assert((res1c == std::vector<std::string>{"1 1"}));
    auto res2c = std::get<std::string>(analyzeGraph(2, adj3));
    assert(res2c == "1 1\n1 1 50\n");

    // Case: weight exactly 50 is valid, 51 invalid
    std::vector<std::vector<int>> adj4 = {{0, 50, 51}, {0, 0, 49}, {0, 0, 0}};
    auto res1d = std::get<std::vector<std::string>>(analyzeGraph(1, adj4));
    assert((res1d == std::vector<std::string>{"0 1", "1 1", "1 0"}));
    auto res2d = std::get<std::string>(analyzeGraph(2, adj4));
    assert(res2d == "3 2\n1 2 50\n2 3 49\n");

    return 0;
}
