/*
Write a C++ function that reads a sequence of undirected graph edges from a text stream, where each line contains five integers: source vertex id, destination vertex id, source label, edge label, and destination label. A new graph begins whenever an edge connects vertex 0 to vertex 1. The function must process all graphs and return a vector of strings, where each string is the canonical DFS code (as a space-separated list of integers) for the corresponding graph. For each graph, you must track vertex labels in a map, add new vertices as needed, add the undirected edge (both directions) between the two vertices, and after the graph is complete (either when the next graph starts or at end of input), compute and output its canonical DFS code using a provided function `std::string compute_min_dfs_code(const std::vector<std::tuple<int,int,int,int,int>>& edges)` that takes the raw edge list and returns the canonical code string. The input may contain blank lines that should be skipped. Edge cases: a graph may have multiple edges between the same pair of vertices, self-loops (source equals destination), or isolated vertices not appearing in any edge (these are impossible since vertices only appear in edges). The last line may or may not have a trailing newline. Your function signature: `std::vector<std::string> extract_graph_codes(std::istream& in)`.
*/

#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <iostream>
#include <map>
#include <utility>

// Provided externally: computes canonical DFS code from a list of undirected edges.
// Each tuple stores (source_id, dest_id, source_label, edge_label, dest_label).
std::string compute_min_dfs_code(const std::vector<std::tuple<int,int,int,int,int>>& edges);

// Process a stream containing undirected graph edges, where a new graph starts
// whenever an edge connects vertex 0 to vertex 1. Returns canonical DFS codes for each graph.
std::vector<std::string> extract_graph_codes(std::istream& in) {
    std::vector<std::string> codes;
    std::vector<std::tuple<int,int,int,int,int>> current_edges;
    std::map<int, int> vertex_labels; // source->label tracking (kept for completeness)
    std::string line;

    // Helper lambda to finalize a graph
    auto finalize_graph = [&]() {
        if (!current_edges.empty()) {
            codes.push_back(compute_min_dfs_code(current_edges));
            current_edges.clear();
            vertex_labels.clear();
        }
    };

    while (std::getline(in, line)) {
        if (line.empty()) continue; // skip blank lines

        std::istringstream iss(line);
        int s_id, d_id, s_lbl, e_lbl, d_lbl;
        if (!(iss >> s_id >> d_id >> s_lbl >> e_lbl >> d_lbl)) {
            // Malformed line: skip (or could throw); here we treat as invalid and ignore.
            continue;
        }

        if (s_id == 0 && d_id == 1) {
            // Start of a new graph: finalize previous graph if any
            finalize_graph();
            // Begin new graph with this edge
            current_edges.emplace_back(s_id, d_id, s_lbl, e_lbl, d_lbl);
            vertex_labels[s_id] = s_lbl;
            vertex_labels[d_id] = d_lbl;
        } else {
            // Add to current graph
            current_edges.emplace_back(s_id, d_id, s_lbl, e_lbl, d_lbl);
            // Track vertex labels (for completeness; not used in code computation here)
            if (vertex_labels.find(s_id) == vertex_labels.end()) {
                vertex_labels[s_id] = s_lbl;
            }
            if (vertex_labels.find(d_id) == vertex_labels.end()) {
                vertex_labels[d_id] = d_lbl;
            }
        }
    }

    // Finalize last graph
    finalize_graph();

    return codes;
}

#include <cassert>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

// Mock implementation of compute_min_dfs_code for testing.
// Returns a deterministic string based on edge count and labels (not actual DFS code).
std::string compute_min_dfs_code(const std::vector<std::tuple<int,int,int,int,int>>& edges) {
    std::string result = "G" + std::to_string(edges.size());
    for (const auto& e : edges) {
        result += " e" + std::to_string(std::get<0>(e)) + "-" + std::to_string(std::get<1>(e)) +
                  ":" + std::to_string(std::get<3>(e));
    }
    return result;
}

// The actual solution function (assume it is included from the solution section)

int main() {
    // Test 1: Two simple graphs
    {
        std::istringstream input(
            "0 1 10 5 20\n"
            "1 2 20 3 30\n"
            "0 1 1 1 1\n"
            "1 3 1 2 4\n"
        );
        auto codes = extract_graph_codes(input);
        assert(codes.size() == 2);
        assert(codes[0] == "G2 e0-1:5 e1-2:3");
        assert(codes[1] == "G2 e0-1:1 e1-3:2");
    }

    // Test 2: Single graph, no trailing newline, blank lines
    {
        std::istringstream input(
            "0 1 5 9 6\n"
            "\n"
            "1 2 6 4 7\n"
            "2 0 7 8 5"
        );
        auto codes = extract_graph_codes(input);
        assert(codes.size() == 1);
        assert(codes[0] == "G3 e0-1:9 e1-2:4 e2-0:8");
    }

    // Test 3: Three graphs, including a self-loop and multiple edges
    {
        std::istringstream input(
            "0 1 1 1 2\n"
            "1 1 2 5 2\n"          // self-loop
            "0 1 3 2 4\n"          // start of graph 2
            "1 2 4 7 5\n"
            "1 2 4 8 5\n"          // parallel edge
            "0 1 9 9 8\n"          // start of graph 3
            "2 3 8 1 7\n"
        );
        auto codes = extract_graph_codes(input);
        assert(codes.size() == 3);
        assert(codes[0] == "G2 e0-1:1 e1-1:5");
        assert(codes[1] == "G3 e0-1:2 e1-2:7 e1-2:8");
        assert(codes[2] == "G2 e0-1:9 e2-3:1");
    }

    // Test 4: Edge case: graph with only boundary edge (single node-pair graph)
    {
        std::istringstream input("0 1 1 2 3\n");
        auto codes = extract_graph_codes(input);
        assert(codes.size() == 1);
        assert(codes[0] == "G1 e0-1:2");
    }

    // Test 5: Empty input (no graphs)
    {
        std::istringstream input("");
        auto codes = extract_graph_codes(input);
        assert(codes.empty());
    }

    return 0;
}

// The solution processes the input line by line. For each non-blank line, parse five integers with a `std::istringstream`. If the edge connects vertex 0 to vertex 1 (i.e., s_id==0 && d_id==1), this indicates the start of a new graph. If we already have collected edges for a previous graph, finalize that graph: call `compute_min_dfs_code` on the accumulated edge list, push the result into the result vector, clear the edge list, and reset the vertex-label map. If the edge is not a graph boundary, add it to the current edge list as a tuple `(s_id, d_id, s_lbl, e_lbl, d_lbl)`. After reading all lines, if there are any remaining edges (i.e., the last graph), finalize it similarly. The main complexity is correctly detecting boundaries and handling the final graph. Time complexity is O(E + G*C) where E is total edges, G is number of graphs, and C is cost of computing canonical code (assume provided). Space usage is O(E) to store edges of the current graph plus O(V) for the vertex map. The map is technically not needed for the provided solution because `compute_min_dfs_code` takes the raw edge list, but we maintain it anyway to mirror the original snippet’s tracking — in a real extension you’d verify vertex labels against the map. The main edge cases: blank lines (skip), malformed lines (assume input is well-formed; if not, you could throw or skip), and a graph boundary where s_id==0 && d_id==1 but there were no previous edges (just start a new graph without finalizing). Also handle the case where input ends with no trailing newline.
