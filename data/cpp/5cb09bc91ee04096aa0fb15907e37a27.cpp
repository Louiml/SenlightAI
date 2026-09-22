Write a C++ function `bool isComplexLocus(const SVLocusSet& cset, const EdgeInfo& edge)` that determines whether a given self-edge (where `edge.nodeIndex1 == edge.nodeIndex2`) belongs to a "complex" locus. A locus is considered complex if it fails a specific heuristic: it is NOT the case that the node has between 1 and 2 bidirectional edges (excluding the self-edge) AND at most 4 total outgoing edges (excluding the self-edge). In other words, return `true` if the node does NOT satisfy both `(biEdgeCount >= 1 && biEdgeCount <= 2)` and `(edgeCount <= 4)`. Assume `SVLocusSet`, `SVLocus`, `SVLocusNode`, `SVLocusEdgeManager`, `SVLocusEdgesType`, `EdgeInfo`, and `isBidirectionalEdge` are already defined (do not implement them). You must implement the function body using the provided data structures, iterating over the node's edge map. Handle the case where no other edges exist (edgeCount = 0) by returning `true`. Include necessary headers and use `const` correctness.
// Assume SVLocusSet, SVLocus, etc. are defined and we have a way to construct one.
// Here we provide a minimal mock to make the test runnable. 
// In a real exercise, these types would be provided. For illustration, define stubs.

#include <cassert>
#include <map>
#include <vector>

// Minimal stubs for demonstration – in a real task, these would be given.
struct EdgeInfo { int locusIndex; int nodeIndex1; int nodeIndex2; };
struct SVLocusNode {
    std::map<int, int> edgeMap; // key: neighbor node index, value: dummy
    const std::map<int, int>& getEdgeManager() const { return edgeMap; }
};
struct SVLocus {
    std::vector<SVLocusNode> nodes;
    const SVLocusNode& getNode(int i) const { return nodes[i]; }
    std::size_t size() const { return nodes.size(); }
};
struct SVLocusSet {
    std::vector<SVLocus> loci;
    const SVLocus& getLocus(int i) const { return loci[i]; }
};
using SVLocusEdgesType = std::map<int, int>;
struct SVLocusEdgeManager { const std::map<int,int>& getMap() const; };

// Global flag for bidirectional check (for test)
bool g_biEdges[100] = {};

bool isBidirectionalEdge(const SVLocusSet&, const EdgeInfo& e) {
    // Simple: return true if edge's nodeIndex2 is marked as bidirectional
    // For test, we use a global array indexed by nodeIndex2 (simplified)
    return g_biEdges[e.nodeIndex2];
}

// The solution function (copy from solution section)
bool isComplexLocus(const SVLocusSet& cset, const EdgeInfo& edge) {
    if (edge.nodeIndex1 != edge.nodeIndex2) return false;
    const SVLocus& locus = cset.getLocus(edge.locusIndex);
    const SVLocusNode& node1 = locus.getNode(edge.nodeIndex1);
    const auto& edgeMap = node1.edgeMap;
    EdgeInfo testEdge = edge;
    unsigned edgeCount = 0;
    unsigned biEdgeCount = 0;
    for (const auto& kv : edgeMap) {
        testEdge.nodeIndex2 = kv.first;
        if (testEdge.nodeIndex1 == testEdge.nodeIndex2) continue;
        edgeCount++;
        if (isBidirectionalEdge(cset, testEdge)) biEdgeCount++;
    }
    bool isLowBiEdge = (biEdgeCount >= 1) && (biEdgeCount <= 2);
    bool isLowTotalEdge = (edgeCount <= 4);
    return !(isLowBiEdge && isLowTotalEdge);
}

int main() {
    // Test 1: Node with 1 bi edge and 3 total edges -> low complexity -> return false
    {
        SVLocusSet cset;
        SVLocus locus;
        SVLocusNode node;
        node.edgeMap[1] = 0; // bi
        node.edgeMap[2] = 0; // not bi
        node.edgeMap[3] = 0; // not bi
        locus.nodes.push_back(node);
        cset.loci.push_back(locus);
        g_biEdges[1] = true; g_biEdges[2] = false; g_biEdges[3] = false;
        EdgeInfo e{0, 0, 0};
        assert(isComplexLocus(cset, e) == false);
    }

    // Test 2: Node with 2 bi edges and 4 total edges -> low complexity -> return false
    {
        SVLocusSet cset;
        SVLocus locus;
        SVLocusNode node;
        node.edgeMap[1] = 0; node.edgeMap[2] = 0; node.edgeMap[3] = 0; node.edgeMap[4] = 0;
        locus.nodes.push_back(node);
        cset.loci.push_back(locus);
        g_biEdges[1] = true; g_biEdges[2] = true; g_biEdges[3] = false; g_biEdges[4] = false;
        EdgeInfo e{0, 0, 0};
        assert(isComplexLocus(cset, e) == false);
    }

    // Test 3: Node with 0 other edges -> not low bi (biCount=0) -> returns true (complex)
    {
        SVLocusSet cset;
        SVLocus locus;
        SVLocusNode node;
        locus.nodes.push_back(node);
        cset.loci.push_back(locus);
        EdgeInfo e{0, 0, 0};
        assert(isComplexLocus(cset, e) == true);
    }

    // Test 4: Node with 3 bi edges -> biCount=3 >2 -> returns true (complex)
    {
        SVLocusSet cset;
        SVLocus locus;
        SVLocusNode node;
        node.edgeMap[1]=0; node.edgeMap[2]=0; node.edgeMap[3]=0;
        locus.nodes.push_back(node);
        cset.loci.push_back(locus);
        g_biEdges[1]=true; g_biEdges[2]=true; g_biEdges[3]=true;
        EdgeInfo e{0,0,0};
        assert(isComplexLocus(cset, e) == true);
    }

    // Test 5: Node with 5 total edges (even if 1 bi) -> edgeCount>4 -> returns true
    {
        SVLocusSet cset;
        SVLocus locus;
        SVLocusNode node;
        for (int i=1; i<=5; i++) node.edgeMap[i]=0;
        locus.nodes.push_back(node);
        cset.loci.push_back(locus);
        g_biEdges[1]=true; for (int i=2;i<=5;i++) g_biEdges[i]=false;
        EdgeInfo e{0,0,0};
        assert(isComplexLocus(cset, e) == true);
    }

    // Test 6: Non-self-edge returns false
    {
        SVLocusSet cset;
        EdgeInfo e{0,0,1};
        assert(isComplexLocus(cset, e) == false);
    }
    return 0;
}
#include <cstddef> // for size_t if needed, but not strictly necessary
#include "svgraph/EdgeInfoUtil.hpp" // assume declarations here

// Determine if a self-edge belongs to a complex locus.
// Returns true if the node does NOT have low bidirectional edge count (1-2) AND low total edge count (<=4).
bool isComplexLocus(const SVLocusSet& cset, const EdgeInfo& edge) {
    if (edge.nodeIndex1 != edge.nodeIndex2) return false;

    const SVLocus& locus = cset.getLocus(edge.locusIndex);
    const SVLocusNode& node1 = locus.getNode(edge.nodeIndex1);
    const SVLocusEdgeManager node1Manager = node1.getEdgeManager();

    using edgeiter_t = SVLocusEdgesType::const_iterator;
    edgeiter_t edgeIter = node1Manager.getMap().begin();
    const edgeiter_t edgeIterEnd = node1Manager.getMap().end();

    EdgeInfo testEdge = edge;

    unsigned edgeCount = 0;
    unsigned biEdgeCount = 0;

    for (; edgeIter != edgeIterEnd; ++edgeIter) {
        testEdge.nodeIndex2 = edgeIter->first;
        if (testEdge.nodeIndex1 == testEdge.nodeIndex2) continue;
        edgeCount++;
        if (isBidirectionalEdge(cset, testEdge)) biEdgeCount++;
    }

    const bool isLowBiEdge = (biEdgeCount >= 1) && (biEdgeCount <= 2);
    const bool isLowTotalEdge = (edgeCount <= 4);
    return !(isLowBiEdge && isLowTotalEdge);
}
// The task is to replicate the logic from the given snippet. The main algorithm is:
// 1. Check if the edge is a self-edge (`nodeIndex1 == nodeIndex2`); if not, return `false` (or you could treat as not complex, but the snippet returns false early).
// 2. Retrieve the locus from the set and then the node.
// 3. Get the edge manager's map (which maps node indices to something) and iterate over all entries.
// 4. For each entry, set `testEdge.nodeIndex2` to the map key (the neighbor index). Skip if the neighbor is the same as `nodeIndex1` (self-edge).
// 5. Count total edges (`edgeCount`) and bidirectional edges (`biEdgeCount`) using `isBidirectionalEdge`.
// 6. After the loop, compute `isLowBiEdge` and `isLowTotalEdge` as in snippet.
// 7. Return `!(isLowBiEdge && isLowTotalEdge)`, which means return `true` if the node is NOT low-complexity. Edge cases: zero other edges → `edgeCount=0` and `biEdgeCount=0`, then `isLowBiEdge` is false (since `biEdgeCount>=1` fails) → `isLowBiEdge` false → `(false && ...)` = false → return `true`. If exactly one bidirectional edge and total edges <=4, then `isLowBiEdge` true and `isLowTotalEdge` true → return `false`. If more than 2 bidirectional edges or total >4, return `true`. Time complexity is O(E) where E is the number of edges from the node. Space complexity is O(1) excluding input.
