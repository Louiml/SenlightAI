/*
Write a C++ function that takes a vector of 3D points (represented as a struct with `double` x, y, z coordinates) and a positive integer `maxPointsPerNode`, and builds an octree (a tree data structure where each internal node has exactly 8 children) such that each leaf node (a node without children) contains at most `maxPointsPerNode` points. The function should return the total number of leaf nodes in the octree. The octree must partition the 3D space recursively: each node represents a cubic region defined by its bottom-left-front corner and side length; when a node accumulates more than `maxPointsPerNode` points, it is subdivided into 8 equal-sized child cubes (by halving the side length along each axis), and the points are distributed to the appropriate child based on their coordinates relative to the cube’s center. Points on the boundary (e.g., exactly at the mid-value along an axis) are assigned to the child whose range includes the lower bound (i.e., use `<=` for the lower half). The function should handle an empty input vector by returning 0, and should not modify the input vector. Assume all points are within the cube defined by the minimum and maximum coordinate values of the input points, and that the initial cube is the axis-aligned bounding box of all points (using the smallest side length sufficient to contain them).
*/
#include <vector>
#include <cmath>
#include <algorithm>

struct Point3 {
    double x, y, z;
    Point3(double _x = 0, double _y = 0, double _z = 0) : x(_x), y(_y), z(_z) {}
};

struct OctreeNode {
    Point3 bottomLeft;
    double h;
    std::vector<Point3> points;
    std::vector<OctreeNode*> children; // 8 children, empty if leaf

    OctreeNode(const Point3& bl, double side) : bottomLeft(bl), h(side) {
        children.resize(8, nullptr);
    }

    ~OctreeNode() {
        for (auto* child : children) {
            delete child;
        }
    }
};

// Helper: recursively subdivide node and count leaves
void subdivide(OctreeNode* node, int maxPoints) {
    if (node->points.size() <= maxPoints) {
        return; // becomes a leaf (or remains leaf)
    }

    // Create 8 children with halved side length
    double half = node->h / 2.0;
    double x0 = node->bottomLeft.x;
    double y0 = node->bottomLeft.y;
    double z0 = node->bottomLeft.z;
    node->children[0] = new OctreeNode(Point3(x0, y0, z0), half);
    node->children[1] = new OctreeNode(Point3(x0 + half, y0, z0), half);
    node->children[2] = new OctreeNode(Point3(x0, y0, z0 + half), half);
    node->children[3] = new OctreeNode(Point3(x0 + half, y0, z0 + half), half);
    node->children[4] = new OctreeNode(Point3(x0, y0 + half, z0), half);
    node->children[5] = new OctreeNode(Point3(x0 + half, y0 + half, z0), half);
    node->children[6] = new OctreeNode(Point3(x0, y0 + half, z0 + half), half);
    node->children[7] = new OctreeNode(Point3(x0 + half, y0 + half, z0 + half), half);

    // Distribute points to children using <= for lower half
    double midx = x0 + half;
    double midy = y0 + half;
    double midz = z0 + half;
    for (const auto& p : node->points) {
        int idx = 0;
        if (p.x > midx) idx += 1;
        if (p.y > midy) idx += 4;
        if (p.z > midz) idx += 2;
        node->children[idx]->points.push_back(p);
    }

    // Clear current node's points (it becomes an internal node)
    node->points.clear();

    // Recurse into each child
    for (auto* child : node->children) {
        subdivide(child, maxPoints);
    }
}

// Count leaf nodes (nodes with no children)
int countLeaves(const OctreeNode* node) {
    if (!node) return 0;
    if (node->children.empty() || node->children[0] == nullptr) {
        return 1; // leaf (no children)
    }
    int leaves = 0;
    for (auto* child : node->children) {
        leaves += countLeaves(child);
    }
    return leaves;
}

// Main function: build octree and return number of leaves
int countOctreeLeaves(const std::vector<Point3>& points, int maxPointsPerNode) {
    if (points.empty() || maxPointsPerNode <= 0) {
        return 0;
    }

    // Compute bounding box
    double minx = points[0].x, maxx = points[0].x;
    double miny = points[0].y, maxy = points[0].y;
    double minz = points[0].z, maxz = points[0].z;
    for (const auto& p : points) {
        minx = std::min(minx, p.x);
        maxx = std::max(maxx, p.x);
        miny = std::min(miny, p.y);
        maxy = std::max(maxy, p.y);
        minz = std::min(minz, p.z);
        maxz = std::max(maxz, p.z);
    }

    // Determine side length of the bounding cube
    double side = std::max({maxx - minx, maxy - miny, maxz - minz});
    if (side == 0) {
        // All points identical
        return 1; // single leaf
    }

    // Create root node with min corner and side length
    OctreeNode* root = new OctreeNode(Point3(minx, miny, minz), side);
    root->points = points;

    // Build tree
    subdivide(root, maxPointsPerNode);

    // Count leaves
    int leafCount = countLeaves(root);

    // Clean up
    delete root;

    return leafCount;
}
#include <cassert>
#include <vector>

// Provide the struct and function definitions above (or include them here)

int main() {
    // Test 1: Empty input
    std::vector<Point3> empty;
    assert(countOctreeLeaves(empty, 1) == 0);

    // Test 2: Single point with maxPoints=1 -> one leaf
    std::vector<Point3> single = {Point3(1,1,1)};
    assert(countOctreeLeaves(single, 1) == 1);

    // Test 3: Two identical points, maxPoints=1 -> one leaf (no subdivision possible)
    std::vector<Point3> identical = {Point3(0,0,0), Point3(0,0,0)};
    assert(countOctreeLeaves(identical, 1) == 1);

    // Test 4: Four points far apart, maxPoints=1 -> multiple leaves
    std::vector<Point3> four = {
        Point3(0,0,0), Point3(10,0,0), Point3(0,10,0), Point3(0,0,10)
    };
    // Initial side = 10, maxPoints=1, so root splits; each child has 1 point -> 4 leaves
    assert(countOctreeLeaves(four, 1) == 4);

    // Test 5: Eight points, each in a distinct octant, maxPoints=1 -> 8 leaves
    std::vector<Point3> eight;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                eight.push_back(Point3(i*10, j*10, k*10));
    // Points at corners of a cube of side 10, each in different child of root
    assert(countOctreeLeaves(eight, 1) == 8);

    // Test 6: Boundary condition: point exactly at midpoint goes to lower half
    std::vector<Point3> boundary = {Point3(0,0,0), Point3(5,0,0), Point3(10,0,0)};
    // side=10, root splits; point (5,0,0) goes to child 0 (x<=mid), (10,0,0) to child 1
    // Each child has at most 2 points, but maxPoints=2, so no further split
    assert(countOctreeLeaves(boundary, 2) == 2);

    // Test 7: maxPoints larger than point count -> root is the only leaf
    std::vector<Point3> many = {Point3(1,2,3), Point3(4,5,6), Point3(7,8,9)};
    assert(countOctreeLeaves(many, 10) == 1);

    // Test 8: maxPoints=0 treated as invalid -> 0
    assert(countOctreeLeaves(single, 0) == 0);

    return 0;
}
// The solution involves constructing an octree from a set of points. First, compute the bounding box of all points: find the minimum and maximum x, y, z coordinates. The initial cube’s bottom-left-front corner is the minimum point, and its side length is the maximum of the three axis spans (max-min). If the input is empty, return 0. Then, recursively subdivide: for a node representing a cube with a side length `h`, if the node currently has more than `maxPointsPerNode` points, create 8 child cubes each with side length `h/2` and corners offset accordingly. Distribute the node’s points to the children based on comparisons with the cube’s midpoint (`midx = bottomLeft.x + h/2`, etc.), using `<=` for the lower-half children. After distribution, clear the node’s own point list and recurse into each child. A leaf node is any node that has no children (i.e., either has not been subdivided or, after subdivision, has no points). Count the total number of leaf nodes by traversing the tree and counting nodes with empty children arrays (or nodes that never subdivided). Edge cases: duplicate points are allowed and are simply stored together; points exactly on boundaries go to the lower-half child; the initial cube may be degenerate (all points identical) in which case side length is 0—then no subdivision occurs and the root is the only leaf (if it has points) or count 0 if empty. Time complexity: each insertion/subdivision may take O(n) per level in the worst case due to point redistribution, and the tree depth is bounded by the number of subdivisions needed to isolate points; worst-case for n points is O(n * depth) where depth is O(log(max side length / minimum separation)), but in practice it’s near O(n log n) for typical distributions. Space complexity: O(n) for storing all points in the leaves plus overhead for internal nodes, which is at most O(n * (8/7)) in the worst case due to octree splitting.
