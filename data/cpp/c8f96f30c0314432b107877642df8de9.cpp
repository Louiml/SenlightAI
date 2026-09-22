// Implement a simplified version of a bounding volume hierarchy (BVH) for Axis-Aligned Bounding Boxes (AABBs). Write a C++ class `SimpleBVH` that stores a collection of 3D AABBs (each represented by a minimum and maximum point) and supports building a hierarchical tree structure using a median-split strategy, then provides a query method `queryAabb` that returns all indices of stored AABBs overlapping a given query AABB. The tree must be built using a stackless traversal at query time (similar to the `walkStacklessQuantizedTree` function in the provided snippet), where each internal node stores its combined child AABB and an escape index to skip entire subtrees that do not overlap the query. The implementation should use `std::vector` for storage, should not rely on external libraries beyond standard headers, and must handle edge cases such as an empty BVH, a single element, and duplicate AABBs.
The solution involves building a binary tree from a vector of AABBs. The construction process follows these steps:
1. **Node representation**: Define a simple struct `Node` containing a minimum vector, maximum vector, an integer `escapeIndex` (the number of nodes from this node to skip if no overlap, or -1 to indicate a leaf), and a leaf index (valid only for leaves).
2. **Build algorithm**: Use a recursive helper function that operates on a subrange of the input AABB array. If the range contains one element, create a leaf node with that index. Otherwise, compute the centroids of all AABBs, select the axis with the largest variance (similar to `calcSplittingAxis`), sort/partition the AABBs along that axis around the mean centroid (similar to `sortAndCalcSplittingIndex`), and recursively build left and right children. After building children, compute the parent's combined AABB as the union of both children's AABBs, and set its escape index to the total number of nodes in its subtree (so a non-overlapping query can skip the entire subtree).
3. **Query algorithm**: To find overlapping AABBs, perform a stackless traversal using the escape index. Starting at index 0, while the current index is within the node vector, check if the query AABB overlaps the current node's combined AABB. If it does, and the node is a leaf, record the leaf index. If the node is internal and overlaps, move to the next node (left child). If it does not overlap, skip the entire subtree using the escape index (add it to the current index). This avoids recursion and is efficient.
4. **Edge cases**: 
   - Empty BVH: The query should return an empty result.
   - Single AABB: The root is a leaf; query returns it if overlapping.
   - Duplicates: All duplicate AABBs are stored as separate leaves; they will all be reported.
   - Degenerate AABBs (min == max) are valid.
5. **Complexity**: Building takes \(O(n \log n)\) on average (due to partitioning) and up to \(O(n^2)\) in the worst case if partitions are highly unbalanced. Query time is \(O(k + \log n)\) in the best case and \(O(n)\) in the worst case for degenerate queries. Space complexity is \(O(n)\) for the node storage (approximately \(2n\) nodes in the worst case for a balanced tree, but the vector grows dynamically).
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>

struct Aabb {
    double minX, minY, minZ;
    double maxX, maxY, maxZ;
    
    Aabb() : minX(0), minY(0), minZ(0), maxX(0), maxY(0), maxZ(0) {}
    Aabb(double loX, double loY, double loZ, double hiX, double hiY, double hiZ)
        : minX(loX), minY(loY), minZ(loZ), maxX(hiX), maxY(hiY), maxZ(hiZ) {}
};

class SimpleBVH {
public:
    explicit SimpleBVH(const std::vector<Aabb>& boxes) : boxes_(boxes) {
        if (!boxes.empty()) {
            // Initialize leaf order indices
            indices_.resize(boxes.size());
            for (size_t i = 0; i < indices_.size(); i++) indices_[i] = static_cast<int>(i);
            build(0, static_cast<int>(indices_.size()));
        }
    }
    
    std::vector<size_t> queryAabb(const Aabb& query) const {
        std::vector<size_t> result;
        if (nodes_.empty()) return result;
        
        size_t cur = 0;
        while (cur < nodes_.size()) {
            const Node& n = nodes_[cur];
            if (overlap(query, n.minX, n.minY, n.minZ, n.maxX, n.maxY, n.maxZ)) {
                if (n.escapeIndex == -1) {
                    result.push_back(n.leafIndex);
                    cur++;
                } else {
                    cur++;
                }
            } else {
                cur += n.escapeIndex;
            }
        }
        return result;
    }
    
private:
    struct Node {
        double minX, minY, minZ;
        double maxX, maxY, maxZ;
        int escapeIndex; // -1 for leaf, otherwise number of nodes to skip
        size_t leafIndex; // valid only for leaf
    };
    
    const std::vector<Aabb>& boxes_;
    std::vector<int> indices_; // order of box indices for construction
    std::vector<Node> nodes_;
    
    static bool overlap(const Aabb& a, double minX, double minY, double minZ,
                        double maxX, double maxY, double maxZ) {
        return a.minX <= maxX && a.maxX >= minX &&
               a.minY <= maxY && a.maxY >= minY &&
               a.minZ <= maxZ && a.maxZ >= minZ;
    }
    
    static double centroid(const Aabb& b, int axis) {
        if (axis == 0) return (b.minX + b.maxX) * 0.5;
        if (axis == 1) return (b.minY + b.maxY) * 0.5;
        return (b.minZ + b.maxZ) * 0.5;
    }
    
    int build(int start, int end) {
        int curNodeIndex = static_cast<int>(nodes_.size());
        int numElems = end - start;
        
        if (numElems == 1) {
            int boxIdx = indices_[start];
            const Aabb& b = boxes_[boxIdx];
            Node node;
            node.minX = b.minX; node.minY = b.minY; node.minZ = b.minZ;
            node.maxX = b.maxX; node.maxY = b.maxY; node.maxZ = b.maxZ;
            node.escapeIndex = -1;
            node.leafIndex = static_cast<size_t>(boxIdx);
            nodes_.push_back(node);
            return 1;
        }
        
        // Compute mean centroid and variance
        double mean[3] = {0,0,0};
        for (int i = start; i < end; i++) {
            const Aabb& b = boxes_[indices_[i]];
            mean[0] += centroid(b,0);
            mean[1] += centroid(b,1);
            mean[2] += centroid(b,2);
        }
        double inv = 1.0 / numElems;
        mean[0] *= inv; mean[1] *= inv; mean[2] *= inv;
        
        double var[3] = {0,0,0};
        for (int i = start; i < end; i++) {
            const Aabb& b = boxes_[indices_[i]];
            for (int axis = 0; axis < 3; axis++) {
                double diff = centroid(b,axis) - mean[axis];
                var[axis] += diff * diff;
            }
        }
        
        int splitAxis = 0;
        if (var[1] > var[splitAxis]) splitAxis = 1;
        if (var[2] > var[splitAxis]) splitAxis = 2;
        
        // Partition indices_ around mean centroid
        double splitValue = mean[splitAxis];
        int splitIndex = start;
        for (int i = start; i < end; i++) {
            int idx = indices_[i];
            if (centroid(boxes_[idx], splitAxis) > splitValue) {
                std::swap(indices_[i], indices_[splitIndex]);
                splitIndex++;
            }
        }
        
        // Ensure balanced by checking the middle third
        int range = numElems / 3;
        if (splitIndex <= start + range || splitIndex >= end - range) {
            splitIndex = start + (numElems / 2);
            // Use nth_element to get median at splitIndex
            std::nth_element(indices_.begin() + start, indices_.begin() + splitIndex,
                             indices_.begin() + end,
                             [&](int a, int b) {
                                 return centroid(boxes_[a], splitAxis) < centroid(boxes_[b], splitAxis);
                             });
        }
        
        // Create internal node placeholder (will fill AABB after children)
        Node placeholder;
        placeholder.minX = placeholder.minY = placeholder.minZ = std::numeric_limits<double>::infinity();
        placeholder.maxX = placeholder.maxY = placeholder.maxZ = -std::numeric_limits<double>::infinity();
        placeholder.escapeIndex = 0;
        placeholder.leafIndex = 0;
        nodes_.push_back(placeholder);
        
        // Build children
        int leftSize = build(start, splitIndex);
        int rightSize = build(splitIndex, end);
        int totalSize = leftSize + rightSize + 1;
        
        // Compute union AABB of both children
        Node& parent = nodes_[curNodeIndex];
        parent.minX = parent.minY = parent.minZ = std::numeric_limits<double>::infinity();
        parent.maxX = parent.maxY = parent.maxZ = -std::numeric_limits<double>::infinity();
        for (int i = 1; i <= leftSize; i++) {
            const Node& child = nodes_[curNodeIndex + i];
            parent.minX = std::min(parent.minX, child.minX);
            parent.minY = std::min(parent.minY, child.minY);
            parent.minZ = std::min(parent.minZ, child.minZ);
            parent.maxX = std::max(parent.maxX, child.maxX);
            parent.maxY = std::max(parent.maxY, child.maxY);
            parent.maxZ = std::max(parent.maxZ, child.maxZ);
        }
        for (int i = leftSize + 1; i <= leftSize + rightSize; i++) {
            const Node& child = nodes_[curNodeIndex + i];
            parent.minX = std::min(parent.minX, child.minX);
            parent.minY = std::min(parent.minY, child.minY);
            parent.minZ = std::min(parent.minZ, child.minZ);
            parent.maxX = std::max(parent.maxX, child.maxX);
            parent.maxY = std::max(parent.maxY, child.maxY);
            parent.maxZ = std::max(parent.maxZ, child.maxZ);
        }
        parent.escapeIndex = totalSize;
        parent.leafIndex = 0;
        
        return totalSize;
    }
};
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Empty BVH
    SimpleBVH empty({});
    assert(empty.queryAabb(Aabb(0,0,0,1,1,1)).empty());
    
    // Single AABB
    SimpleBVH single({Aabb(0,0,0,1,1,1)});
    auto r1 = single.queryAabb(Aabb(0.5,0.5,0.5,0.6,0.6,0.6));
    assert(r1.size() == 1 && r1[0] == 0);
    auto r1b = single.queryAabb(Aabb(2,2,2,3,3,3));
    assert(r1b.empty());
    
    // Multiple non-overlapping boxes
    std::vector<Aabb> boxes = {
        Aabb(0,0,0,1,1,1),
        Aabb(2,2,2,3,3,3),
        Aabb(10,10,10,11,11,11)
    };
    SimpleBVH bvh(boxes);
    auto r2 = bvh.queryAabb(Aabb(0.5,0.5,0.5,2.5,2.5,2.5));
    assert(r2.size() == 2);
    // Should contain indices 0 and 1
    bool has0 = std::find(r2.begin(), r2.end(), 0) != r2.end();
    bool has1 = std::find(r2.begin(), r2.end(), 1) != r2.end();
    assert(has0 && has1);
    
    // Duplicate boxes
    std::vector<Aabb> dups = {
        Aabb(0,0,0,1,1,1),
        Aabb(0,0,0,1,1,1),
        Aabb(5,5,5,6,6,6)
    };
    SimpleBVH dupBvh(dups);
    auto r3 = dupBvh.queryAabb(Aabb(0.2,0.2,0.2,0.8,0.8,0.8));
    assert(r3.size() == 2);
    
    // Degenerate AABB (point)
    SimpleBVH pointBvh({Aabb(1,1,1,1,1,1)});
    auto r4 = pointBvh.queryAabb(Aabb(0,0,0,2,2,2));
    assert(r4.size() == 1 && r4[0] == 0);
    
    // Query that touches boundary (touching counts as overlap)
    SimpleBVH touchBvh({Aabb(0,0,0,1,1,1)});
    auto r5 = touchBvh.queryAabb(Aabb(1,0.5,0.5,2,1.5,1.5));
    assert(r5.size() == 1);
    
    // Query with negative coordinates
    SimpleBVH negativeBvh({Aabb(-5,-5,-5,-1,-1,-1)});
    auto r6 = negativeBvh.queryAabb(Aabb(-6,-6,-6,0,0,0));
    assert(r6.size() == 1);
    
    // Larger set for balanced tree testing
    std::vector<Aabb> many;
    for (int i = 0; i < 100; i++) {
        many.push_back(Aabb(i, i, i, i+0.5, i+0.5, i+0.5));
    }
    SimpleBVH manyBvh(many);
    auto r7 = manyBvh.queryAabb(Aabb(10,10,10,50.5,50.5,50.5));
    // Should include indices 10 through 50 inclusive
    for (size_t idx = 10; idx <= 50; idx++) {
        assert(std::find(r7.begin(), r7.end(), idx) != r7.end());
    }
    
    return 0;
}
