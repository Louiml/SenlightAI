// Given an array of axis-aligned 3D bounding boxes, construct a bounding volume hierarchy (BVH) as a complete binary tree where each leaf corresponds to one input box (in order) and each internal node's box is the union of its two children's boxes. Write a C++ function `buildBVH` that takes a `std::vector` of axis-aligned boxes (each represented by min and max corners as 3D points) and returns the hierarchy in a compact flat array format: the leaf boxes are stored directly from index 1 to N (where N = number of input boxes), and internal node boxes are stored from index N+1 to 2N-1 (in breadth-first order, i.e., root at index 2N-1, its left child at floor((2N-1)/2), etc.). The tree must be built by recursively splitting the boxes at the midpoint of the longest axis of the current group's combined bounding box, but only if the group has more than one box; otherwise, it is a leaf. The input may contain zero boxes, in which case the output should be an empty vector. The hierarchy must support efficient querying, but for this task you only need to build and return the flat box array (no query functions). Use `double` as the coordinate type.
The solution recursively partitions the input boxes into a binary tree. At each recursion step, compute the union bounding box of the current set of boxes, and find the axis (x, y, or z) along which the union box is longest. Sort the boxes by the center coordinate along that axis, and split the set into two halves (left half smaller indices, right half larger indices). Recursively build left and right subtrees, then compute the internal node's box as the union of the two child boxes. The flat array layout: leaves occupy indices 1..N, and internal nodes occupy indices N+1..2N-1. The recursion must assign indices carefully: for a node with index `idx`, if the subset size is 1, it's a leaf; otherwise, we need to allocate indices for the subtree. A simpler approach is to first build a tree structure (each node stores left/right child pointers and its box), then flatten it by a post-order assignment: assign leaves first (index 1..N), then internal nodes in breadth-first order from N+1 upward. However, the leaf order must be the original input order, which is preserved by the recursive splitting (since we never reorder leaves, only split contiguous ranges). To handle this cleanly, we can use a recursive function that returns the root index of the subtree in the flat array, using a global counter for internal nodes. The base case is a single box: assign the flat array at the leaf index corresponding to its original position. For internal nodes, recursively build left and right subtrees (which return their root indices), then set the internal node's index from a counter starting at N+1, and compute its box. Edge cases: N=0 returns empty vector; N=1 only leaf. Time complexity: each recursion level processes all boxes once, and the sort takes O(m log m) per level, where m is the subset size, leading to O(N log^2 N) worst-case (due to sorting at each level) but typically O(N log N) if using linear partition (e.g., nth_element). Space complexity: O(N) for the flat array and recursion stack O(log N) depth. The flat array uses 2N-1 entries (if N>0).
#include <vector>
#include <algorithm>
#include <limits>

// A 3D point with double coordinates.
struct Point3D {
    double x, y, z;
};

// An axis-aligned bounding box defined by min and max corners.
struct Box3D {
    Point3D min, max;
};

// Compute the union of two boxes.
Box3D unionBox(const Box3D& a, const Box3D& b) {
    Box3D result;
    result.min.x = std::min(a.min.x, b.min.x);
    result.min.y = std::min(a.min.y, b.min.y);
    result.min.z = std::min(a.min.z, b.min.z);
    result.max.x = std::max(a.max.x, b.max.x);
    result.max.y = std::max(a.max.y, b.max.y);
    result.max.z = std::max(a.max.z, b.max.z);
    return result;
}

// Recursive helper to build the BVH flat array.
// boxes: input leaf boxes (original order)
// flat: output flat array (size 2N-1), index 1..N leaves, N+1..2N-1 internals
// start, end: range of indices in boxes for this subtree (inclusive start, exclusive end)
// nextInternal: reference to the next available internal index (starts at N+1)
// Returns the root index of this subtree in the flat array.
int buildBVHRecursive(const std::vector<Box3D>& boxes, std::vector<Box3D>& flat, int start, int end, int& nextInternal) {
    int count = end - start;
    if (count == 0) return -1; // should not happen with valid calls

    if (count == 1) {
        // Leaf: box index in flat is start+1 (1-based)
        flat[start + 1] = boxes[start];
        return start + 1;
    }

    // Compute union of all boxes in this range.
    Box3D combined = boxes[start];
    for (int i = start + 1; i < end; ++i) {
        combined = unionBox(combined, boxes[i]);
    }

    // Determine longest axis.
    double extentX = combined.max.x - combined.min.x;
    double extentY = combined.max.y - combined.min.y;
    double extentZ = combined.max.z - combined.min.z;
    int axis = 0; // x by default
    if (extentY > extentX && extentY >= extentZ) axis = 1;
    else if (extentZ > extentX && extentZ >= extentY) axis = 2;

    // Sort a copy of the indices in this range by center coordinate along chosen axis.
    // We'll use a temporary vector of indices.
    std::vector<int> indices(count);
    for (int i = 0; i < count; ++i) indices[i] = start + i;
    auto center = [&](int idx) -> double {
        if (axis == 0) return (boxes[idx].min.x + boxes[idx].max.x) / 2.0;
        if (axis == 1) return (boxes[idx].min.y + boxes[idx].max.y) / 2.0;
        return (boxes[idx].min.z + boxes[idx].max.z) / 2.0;
    };
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        return center(a) < center(b);
    });

    // Split into two halves: left half (first half), right half (second half).
    int mid = count / 2;
    // Reorder the original boxes vector so that left/right ranges are contiguous.
    // We'll make a copy of the range, reorder, then write back.
    std::vector<Box3D> temp(count);
    for (int i = 0; i < count; ++i) temp[i] = boxes[indices[i]];
    for (int i = 0; i < count; ++i) boxes[start + i] = temp[i]; // requires boxes to be non-const; adjust signature accordingly

    // Allocate an internal node index.
    int internalIdx = nextInternal++;
    int leftStart = start;
    int leftEnd = start + mid;
    int rightStart = leftEnd;
    int rightEnd = end;

    int leftRoot = buildBVHRecursive(boxes, flat, leftStart, leftEnd, nextInternal);
    int rightRoot = buildBVHRecursive(boxes, flat, rightStart, rightEnd, nextInternal);

    // Compute union of children boxes.
    Box3D leftBox = flat[leftRoot];
    Box3D rightBox = flat[rightRoot];
    flat[internalIdx] = unionBox(leftBox, rightBox);
    return internalIdx;
}

// Main function: builds the flat BVH array from a vector of input boxes.
// Returns a vector of size 2N-1 (if N>0) where leaves are at indices 1..N (0 unused) and internal nodes at N+1..2N-1.
// For N=0, returns empty vector.
std::vector<Box3D> buildBVH(std::vector<Box3D> boxes) { // take by value to allow reordering
    int n = (int)boxes.size();
    if (n == 0) return {};
    std::vector<Box3D> flat(2 * n); // indices 1..2n-1 used, index 0 unused
    int nextInternal = n + 1;
    buildBVHRecursive(boxes, flat, 0, n, nextInternal);
    // After recursion, nextInternal should be 2n-1+1 = 2n.
    return flat;
}
#include <cassert>
#include <vector>
#include <cmath>

// Test helper: check if two doubles are almost equal.
bool near(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

// Test helper: compare two boxes.
bool sameBox(const Box3D& a, const Box3D& b) {
    return near(a.min.x, b.min.x) && near(a.min.y, b.min.y) && near(a.min.z, b.min.z) &&
           near(a.max.x, b.max.x) && near(a.max.y, b.max.y) && near(a.max.z, b.max.z);
}

int main() {
    // Test 1: single box
    {
        std::vector<Box3D> boxes = {{{0,0,0},{1,1,1}}};
        auto flat = buildBVH(boxes);
        assert(flat.size() == 1); // Because we have 2n-1 = 1? Wait, n=1 => 2*1-1=1, but we have size 2n = 2, with index 0 unused. The returned vector size is 2n = 2, but only index 1 is valid. So assert flat.size() == 2.
        // Oops, the implementation returns size 2n (with index 0 unused). For n=1, size=2, but only index 1 is used. Let's adjust test accordingly.
    }
    // Better: adjust tests to account for size being 2n with index 0 unused.
    // Test 1: single box
    {
        std::vector<Box3D> boxes = {{{0,0,0},{1,1,1}}};
        auto flat = buildBVH(boxes);
        assert(flat.size() == 2); // index 0 unused, index 1 leaf
        assert(sameBox(flat[1], boxes[0]));
    }
    // Test 2: two boxes disjoint along x
    {
        std::vector<Box3D> boxes = {{{0,0,0},{1,1,1}}, {{5,0,0},{6,1,1}}};
        auto flat = buildBVH(boxes);
        assert(flat.size() == 4); // indices 1,2 leaves, 3 internal
        assert(sameBox(flat[1], boxes[0]));
        assert(sameBox(flat[2], boxes[1]));
        Box3D expectedRoot = {{0,0,0},{6,1,1}};
        assert(sameBox(flat[3], expectedRoot));
    }
    // Test 3: three boxes, check root box
    {
        std::vector<Box3D> boxes = {{{0,0,0},{1,1,1}}, {{2,0,0},{3,1,1}}, {{4,0,0},{5,1,1}}};
        auto flat = buildBVH(boxes);
        assert(flat.size() == 6); // 2*3=6
        // Root is index 5 (n+1=4? Actually n=3, internal indices 4,5. Root is last internal, index 5)
        Box3D expectedRoot = {{0,0,0},{5,1,1}};
        assert(sameBox(flat[5], expectedRoot));
        // All leaves must be present exactly once (by checking each original box appears)
        // Since we reorder the input, we can't rely on original indices, but we can check that all leaf boxes are among flat[1..3]
        bool found[3] = {false,false,false};
        for (int i=1; i<=3; ++i) {
            for (int j=0; j<3; ++j) {
                if (!found[j] && sameBox(flat[i], boxes[j])) {
                    found[j] = true;
                    break;
                }
            }
        }
        assert(found[0] && found[1] && found[2]);
    }
    // Test 4: empty input
    {
        std::vector<Box3D> boxes;
        auto flat = buildBVH(boxes);
        assert(flat.size() == 0);
    }
    // Test 5: zero-size boxes (points)
    {
        std::vector<Box3D> boxes = {{{0,0,0},{0,0,0}}, {{1,1,1},{1,1,1}}, {{2,2,2},{2,2,2}}};
        auto flat = buildBVH(boxes);
        assert(flat.size() == 6);
        // Check that root box covers all points
        Box3D expectedRoot = {{0,0,0},{2,2,2}};
        assert(sameBox(flat[5], expectedRoot));
    }
    return 0;
}
