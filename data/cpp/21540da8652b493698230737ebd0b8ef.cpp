/*
Write a C++ function `std::string classifyGraph(const std::vector<std::vector<int>>& adjacencyMatrix)` that takes a square adjacency matrix of a directed graph (vertices labeled 0 to n-1 internally, but conceptually 1 to n in the problem) and returns one of three strings: `"Hamiltonian Circuit"` if every vertex has exactly one incoming and one outgoing edge (a 1-regular directed graph forming a single cycle covering all vertices), `"Hamiltonian Path"` if the graph is a directed path covering all vertices (exactly one vertex with out-degree 1 and in-degree 0, exactly one vertex with out-degree 0 and in-degree 1, and all other vertices have both degrees equal to 1), or `"Neither Hamiltonian Path nor Hamiltonian Circuit"` otherwise. The matrix contains only 0 and 1 entries. If the matrix is empty, return the "Neither" message. The function must be `const`-correct and not modify the input.
*/

#include <string>
#include <vector>

// Classify a directed graph given by an adjacency matrix.
// Returns "Hamiltonian Circuit", "Hamiltonian Path", or "Neither Hamiltonian Path nor Hamiltonian Circuit".
std::string classifyGraph(const std::vector<std::vector<int>>& mat) {
    int n = mat.size();
    if (n == 0) return "Neither Hamiltonian Path nor Hamiltonian Circuit";

    std::vector<int> outDeg(n, 0), inDeg(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (mat[i][j] == 1) {
                outDeg[i]++;
                inDeg[j]++;
            }
        }
    }

    bool allOne = true;
    for (int i = 0; i < n; ++i) {
        if (outDeg[i] != 1 || inDeg[i] != 1) {
            allOne = false;
            break;
        }
    }
    if (allOne) return "Hamiltonian Circuit";

    int startCount = 0, endCount = 0, midCount = 0;
    for (int i = 0; i < n; ++i) {
        if (outDeg[i] == 1 && inDeg[i] == 0) startCount++;
        else if (outDeg[i] == 0 && inDeg[i] == 1) endCount++;
        else if (outDeg[i] == 1 && inDeg[i] == 1) midCount++;
    }
    if (startCount == 1 && endCount == 1 && midCount == (n - 2)) {
        return "Hamiltonian Path";
    }

    return "Neither Hamiltonian Path nor Hamiltonian Circuit";
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test
std::string classifyGraph(const std::vector<std::vector<int>>& mat);

int main() {
    // Empty matrix
    std::vector<std::vector<int>> empty;
    assert(classifyGraph(empty) == "Neither Hamiltonian Path nor Hamiltonian Circuit");

    // Single vertex with self-loop -> circuit
    std::vector<std::vector<int>> selfLoop = {{1}};
    assert(classifyGraph(selfLoop) == "Hamiltonian Circuit");

    // Two-vertex directed cycle: 0->1, 1->0
    std::vector<std::vector<int>> cycle2 = {{0,1}, {1,0}};
    assert(classifyGraph(cycle2) == "Hamiltonian Circuit");

    // Directed path 0->1->2 (3 vertices)
    std::vector<std::vector<int>> path3 = {{0,1,0}, {0,0,1}, {0,0,0}};
    assert(classifyGraph(path3) == "Hamiltonian Path");

    // Disconnected: 0->1, 2->3 (4 vertices) -> neither
    std::vector<std::vector<int>> disconnected = {{0,1,0,0}, {0,0,0,0}, {0,0,0,1}, {0,0,0,0}};
    assert(classifyGraph(disconnected) == "Neither Hamiltonian Path nor Hamiltonian Circuit");

    // Cycle with extra edge: 0->1, 1->2, 2->0, 0->2 (3 vertices) -> neither (0 has out-degree 2)
    std::vector<std::vector<int>> extraEdge = {{0,1,1}, {0,0,1}, {1,0,0}};
    assert(classifyGraph(extraEdge) == "Neither Hamiltonian Path nor Hamiltonian Circuit");

    // Proper 4-cycle: 0->1,1->2,2->3,3->0
    std::vector<std::vector<int>> cycle4 = {{0,1,0,0}, {0,0,1,0}, {0,0,0,1}, {1,0,0,0}};
    assert(classifyGraph(cycle4) == "Hamiltonian Circuit");

    // Proper 4-path: 0->1->2->3
    std::vector<std::vector<int>> path4 = {{0,1,0,0}, {0,0,1,0}, {0,0,0,1}, {0,0,0,0}};
    assert(classifyGraph(path4) == "Hamiltonian Path");

    // Graph where a middle vertex has both degrees 1 but start/end counts wrong
    std::vector<std::vector<int>> weird = {{0,0,1}, {1,0,0}, {0,1,0}}; // 0->2, 1->0, 2->1 => cycle
    assert(classifyGraph(weird) == "Hamiltonian Circuit");

    // Graph with one vertex but no edge -> neither (out=0,in=0)
    std::vector<std::vector<int>> singleNoEdge = {{0}};
    assert(classifyGraph(singleNoEdge) == "Neither Hamiltonian Path nor Hamiltonian Circuit");

    return 0;
}

// The solution computes the out-degree and in-degree of every vertex by iterating over rows and columns of the adjacency matrix. For out-degree, count the number of 1s in each row; for in-degree, count the number of 1s in each column. Then apply the classification: (1) If every vertex has out-degree == 1 and in-degree == 1, it is a Hamiltonian Circuit. (2) Otherwise, check for a Hamiltonian Path: exactly one vertex with (out=1, in=0), exactly one vertex with (out=0, in=1), and all other vertices with (out=1, in=1). Note that the original snippet incorrectly printed in-degree array but used out-degree in the output; we correct that. Edge cases: empty matrix returns "Neither". Also note that a graph with one vertex and a self-loop (mat[0][0]=1) has out-degree=1 and in-degree=1, so it is classified as both a circuit and a path; here we give circuit priority. Time complexity is O(n^2) for degree computation, space O(n) for degree arrays.
