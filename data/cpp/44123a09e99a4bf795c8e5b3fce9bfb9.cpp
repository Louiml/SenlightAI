// Given a directed or undirected graph represented by the classes `Node`, `Edge`, and `Graph` (as provided in the snippet), write a standalone C++ function `bool hasCycle(const Graph& g)` that returns `true` if the graph contains at least one cycle, and `false` otherwise. The graph is stored with edges in an adjacency list, and the `directed` flag indicates whether the graph is directed. Your function must work correctly for both directed and undirected graphs, handle disconnected graphs, and not modify the graph. Assume node labels are unique and edge weights are non-negative (weights do not affect cycle detection). You may use the provided class methods and members (including private members via `friend` if needed, but prefer using public interface where possible).

// Cycle detection in a directed graph can be performed using DFS with three states per node: unvisited, in current recursion stack, and fully processed. If a back edge points to a node currently in the recursion stack, a cycle exists. For an undirected graph, a cycle exists if during DFS we encounter an edge to a previously visited node that is not the immediate parent (i.e., not the node we came from). Since the graph may be disconnected, we must start DFS from every unvisited node. The provided `Graph` exposes nodes through `nodes` and adjacency through `Edge` objects; we can access the adjacency from `nodes` via iteration over `Edge*` and use the `finish` pointer. We need to distinguish parent for undirected case: keep track of the previous node in the DFS call. Time complexity is O(V+E) for both cases, space O(V) for the visited/recursion-stack markers.

#include <set>
#include <map>
#include <string>
#include <queue>
#include <iostream>
#include <limits>
#include <list>

class Node;
class Edge;

class Node {
public:
    Node(std::string label) : label(label) {}
    std::string getLabel() const { return label; }
private:
    std::string label;
    std::set<Edge*> adjacency;
    friend class Graph;
};

class Edge {
public:
    Edge(Node* s, Node* f, double w) : start(s), finish(f), weight(w) {}
    double cost() const { return weight; }
private:
    Node *start, *finish;
    double weight;
    friend class Graph;
};

class Graph {
public:
    Graph(bool directed) : directed(directed) {}
    void addNode(const std::string& name) {
        Node* n = new Node(name);
        nodes.insert(n);
        nodeMap[name] = n;
    }
    void addEdge(const std::string& l1, const std::string& l2, double w) {
        Node* n1 = nodeMap[l1];
        Node* n2 = nodeMap[l2];
        createEdge(n1, n2, w);
        if (!directed) createEdge(n2, n1, w);
    }
    bool isDirected() const { return directed; }
    const std::set<Node*>& getNodes() const { return nodes; }
private:
    void createEdge(Node* s, Node* f, double w) {
        Edge* e = new Edge(s, f, w);
        edges.insert(e);
        s->adjacency.insert(e);
    }
    bool directed;
    std::map<std::string, Node*> nodeMap;
    std::set<Node*> nodes;
    std::set<Edge*> edges;
};

// Helper for directed DFS
bool dfsDirected(Node* u, std::map<Node*, int>& state) {
    state[u] = 1; // in stack
    for (Edge* e : u->adjacency) {
        Node* v = e->finish;
        if (state[v] == 1) return true;
        if (state[v] == 0 && dfsDirected(v, state)) return true;
    }
    state[u] = 2; // done
    return false;
}

// Helper for undirected DFS
bool dfsUndirected(Node* u, Node* parent, std::map<Node*, bool>& visited) {
    visited[u] = true;
    for (Edge* e : u->adjacency) {
        Node* v = e->finish;
        if (v == parent) continue;
        if (visited[v]) return true;
        if (dfsUndirected(v, u, visited)) return true;
    }
    return false;
}

// Returns true if the graph contains a cycle
bool hasCycle(const Graph& g) {
    const auto& nodes = g.getNodes();
    if (nodes.empty()) return false;
    
    if (g.isDirected()) {
        std::map<Node*, int> state; // 0=unvisited, 1=inStack, 2=done
        for (Node* n : nodes) state[n] = 0;
        for (Node* n : nodes) {
            if (state[n] == 0) {
                if (dfsDirected(n, state)) return true;
            }
        }
        return false;
    } else {
        std::map<Node*, bool> visited;
        for (Node* n : nodes) visited[n] = false;
        for (Node* n : nodes) {
            if (!visited[n]) {
                if (dfsUndirected(n, nullptr, visited)) return true;
            }
        }
        return false;
    }
}

#include <cassert>

int main() {
    // Directed graph with a cycle: A->B, B->C, C->A
    Graph g1(true);
    g1.addNode("A");
    g1.addNode("B");
    g1.addNode("C");
    g1.addEdge("A", "B", 1);
    g1.addEdge("B", "C", 1);
    g1.addEdge("C", "A", 1);
    assert(hasCycle(g1) == true);

    // Directed acyclic graph
    Graph g2(true);
    g2.addNode("A");
    g2.addNode("B");
    g2.addNode("C");
    g2.addEdge("A", "B", 1);
    g2.addEdge("B", "C", 1);
    assert(hasCycle(g2) == false);

    // Undirected graph with a cycle (triangle)
    Graph g3(false);
    g3.addNode("A");
    g3.addNode("B");
    g3.addNode("C");
    g3.addEdge("A", "B", 1);
    g3.addEdge("B", "C", 1);
    g3.addEdge("C", "A", 1);
    assert(hasCycle(g3) == true);

    // Undirected tree (no cycle)
    Graph g4(false);
    g4.addNode("A");
    g4.addNode("B");
    g4.addNode("C");
    g4.addEdge("A", "B", 1);
    g4.addEdge("B", "C", 1);
    assert(hasCycle(g4) == false);

    // Single node, no edges
    Graph g5(true);
    g5.addNode("X");
    assert(hasCycle(g5) == false);

    // Directed self-loop
    Graph g6(true);
    g6.addNode("A");
    g6.addEdge("A", "A", 1);
    assert(hasCycle(g6) == true);

    // Disconnected undirected graph with cycle in one component
    Graph g7(false);
    g7.addNode("A");
    g7.addNode("B");
    g7.addNode("C");
    g7.addNode("D");
    g7.addEdge("A", "B", 1);
    g7.addEdge("B", "C", 1);
    g7.addEdge("C", "A", 1); // cycle
    g7.addNode("D"); // isolated
    assert(hasCycle(g7) == true);

    return 0;
}
