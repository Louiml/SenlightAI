// Create a C++ function named `rayTraceScene` that, given a bounding sphere center and radius, builds a simple hierarchical scene of spheres (a "sphere tree") and returns the maximum number of sphere intersection tests (i.e., calls to a sphere's `intersect` method) required to trace a single ray through the scene using a depth-first, bounding-volume-hierarchy (BVH) style traversal with skip pointers. The function should take the root sphere's center (as three doubles), its radius, and the desired tree depth as parameters, and return the worst-case (maximum) depth of recursion or number of visited nodes during a ray traversal, whichever is more meaningful to the problem. For simplicity, assume the ray always starts outside the root bounding sphere and travels in an arbitrary direction. The function should not actually perform image rendering; it should only construct the BVH and compute the maximum recursion depth or the maximum number of `intersect` calls along any path, given that the ray may hit all leaf spheres in the worst case. Include the BVH construction logic similar to the provided snippet: each node stores a bounding sphere (radius = 2*leaf radius), a leaf sphere, and a `diff` skip value that points to the next sibling in a flat array. At each internal node, generate 6 lower-ring children and 3 upper-ring children recursively, using the provided angular increments (`2π/6` and `2π/3`) and a child radius of `r/3`. The worst-case traversal count is the number of nodes visited when the ray intersects every bounding sphere and every leaf sphere along a path that visits all nodes (since the skip pointer is only used when a bounding sphere is missed). Implement the function so that it returns the total number of nodes in the tree for a given depth, and verify correctness by comparing with the expected count formula. The solution must be self-contained, use only standard C++ headers, and avoid global mutable state.
#include <cassert>
#include <cmath>

// The expected total node count for a full 9-ary tree of given depth.
int expectedNodes(int depth) {
    // sum_{i=1}^{depth} 9^(i-1) = (9^depth - 1) / 8
    int pow = 1;
    for (int i = 1; i <= depth; ++i) pow *= 9;
    return (pow - 1) / 8;
}

int main() {
    // depth 1: only root
    assert(buildSphereTree(0,0,0, 1.0, 1) == 1);
    // depth 2: root + 9 children = 10
    assert(buildSphereTree(0,0,0, 1.0, 2) == 10);
    // depth 3: 1 + 9 + 81 = 91
    assert(buildSphereTree(0,0,0, 1.0, 3) == 91);
    // depth 4: 1 + 9 + 81 + 729 = 820
    assert(buildSphereTree(0,0,0, 1.0, 4) == 820);
    // depth 5: 1+9+81+729+6561 = 7381
    assert(buildSphereTree(0,0,0, 1.0, 5) == 7381);
    // depth 6: 1+9+81+729+6561+59049 = 66430
    assert(buildSphereTree(0,0,0, 1.0, 6) == 66430);

    // Verify for several depths using the closed-form formula
    for (int d = 1; d <= 6; ++d) {
        int got = buildSphereTree(0,0,0, 1.0, d);
        assert(got == expectedNodes(d));
    }
    return 0;
}
#include <vector>
#include <cmath>
#include <cstddef>
#include <algorithm>

// Minimal 3D vector for internal use.
struct Vec3 {
    double x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double a, double b, double c) : x(a), y(b), z(c) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator*(double s) const { return Vec3(x*s, y*s, z*s); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    double dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    double magsqr() const { return x*x + y*y + z*z; }
    Vec3 norm() const {
        double inv = 1.0 / std::sqrt(magsqr());
        return *this * inv;
    }
};

// Node in the flat BVH.
struct Node {
    Vec3 bound_center;
    double bound_radius;
    Vec3 leaf_center;
    double leaf_radius;
    long diff; // skip count for traversal
};

// Return a pair of vectors orthogonal to the given normalized direction.
static void makeBasis(const Vec3& d, Vec3& b1, Vec3& b2) {
    Vec3 arbitrary = (std::fabs(d.x) < 0.9) ? Vec3(1,0,0) : Vec3(0,1,0);
    b1 = arbitrary.cross(d).norm();
    b2 = d.cross(b1).norm();
}

// Recursively append nodes to 'nodes' and return the number of nodes added.
static int buildSubtree(std::vector<Node>& nodes,
                        const Vec3& center,
                        const Vec3& dir,
                        double radius,
                        int level) {
    Node nd;
    nd.leaf_center = center;
    nd.leaf_radius = radius;
    nd.bound_center = center;
    nd.bound_radius = 2.0 * radius; // bounding sphere always double the leaf
    std::size_t self_idx = nodes.size();
    nodes.push_back(nd); // place the node first

    if (level <= 1) {
        nodes[self_idx].diff = 1;
        return 1;
    }

    // Generate 9 children: 6 lower ring + 3 upper ring
    Vec3 b1, b2;
    makeBasis(dir, b1, b2);
    double child_radius = radius / 3.0;
    int total_child_nodes = 0;

    const double TWO_PI = 2.0 * M_PI;
    // Lower ring: 6 children
    double angle_lower_step = TWO_PI / 6.0;
    double a = 0.0;
    for (int i = 0; i < 6; ++i) {
        Vec3 child_dir = (dir * (-0.2) + b1 * std::sin(a) + b2 * std::cos(a)).norm();
        Vec3 child_center = center + child_dir * (radius + child_radius);
        total_child_nodes += buildSubtree(nodes, child_center, child_dir, child_radius, level - 1);
        a += angle_lower_step;
    }

    // Upper ring: 3 children (offset as in the original snippet)
    a -= angle_lower_step / 3.0;
    double angle_upper_step = TWO_PI / 3.0;
    for (int i = 0; i < 3; ++i) {
        Vec3 child_dir = (dir * 0.6 + b1 * std::sin(a) + b2 * std::cos(a)).norm();
        Vec3 child_center = center + child_dir * (radius + child_radius);
        total_child_nodes += buildSubtree(nodes, child_center, child_dir, child_radius, level - 1);
        a += angle_upper_step;
    }

    // After all children, know the subtree size for the skip pointer.
    nodes[self_idx].diff = 1 + total_child_nodes;
    return 1 + total_child_nodes;
}

// Build the sphere tree and return the total number of nodes (worst-case
// number of intersection tests during a traversal that hits every node).
int buildSphereTree(double cx, double cy, double cz, double radius, int depth) {
    std::vector<Node> nodes;
    Vec3 root_dir(0.25, 1.0, -0.5);
    root_dir = root_dir.norm();
    buildSubtree(nodes, Vec3(cx, cy, cz), root_dir, radius, depth);
    return static_cast<int>(nodes.size());
}
// The task essentially asks to reconstruct the BVH construction from the snippet and compute the total number of nodes for a given depth. The provided code builds a tree where each node has 9 children (6 lower ring + 3 upper ring) at each internal level. The snippet uses a flat array `pool` and `node_t` with a `diff` field that is set to `dist` (the number of nodes in the subtree) for internal nodes, and 1 for leaf nodes. The total count is computed by the code: `int count=childs, dec=lvl; while(--dec > 1) count=(count*childs)+childs; ++count;` which yields the total number of nodes in a full 9-ary tree of depth `lvl` (where root is at level 1). The recursive `create` function places nodes in a depth-first order, and each internal node's `diff` is set to the total size of the subtree (including itself). The `intersect` traversal uses `p->diff` to skip over a subtree when the bounding sphere is missed. The maximum number of `intersect` calls in the worst case (ray hits every node) equals the total number of nodes in the tree, because the traversal visits every node when no bounding sphere is missed (since the skip is never taken). Therefore, the solution should implement the recursive construction and return the number of nodes. The main algorithm: define a `Sphere` struct with center and radius, and an `intersect` method that returns a boolean (or a distance) for simplicity. Define a `Node` struct that holds a bounding sphere (radius = 2*leaf radius), a leaf sphere, and a `diff` jump. Recursively build the tree in a flat vector, using a pre-order traversal. At each node, generate child directions using the basis vectors from the parent direction, with angles as in the snippet. Use `std::sin` and `std::cos` directly (no need for the LLVM rounding). The base case is `lvl <= 1`, where the node is a leaf and `diff = 1`. For internal nodes, after placing the node itself, compute `dist = max((dist - childs)/childs, 1)` where `childs=9` and `dist` initially is the total count. Then recursively create children. The total count for depth `lvl` is: base count = 1 for root, then each level adds 9 times the previous count times? Actually the formula from snippet: `int count=childs, dec=lvl; while(--dec > 1) count=(count*childs)+childs; ++count;` For lvl=6, count starts at 9, dec=6, loop runs for dec=5,4,3,2: count = 9*9+9=90, then 90*9+9=819, then 819*9+9=7380, then 7380*9+9=66429? Wait that seems too large. Actually the given code also has `++count` after the loop, so final count = 66430? But the tree depth is `lvl`, and each internal node has 9 children. The number of nodes in a full 9-ary tree of depth lvl (root at depth 1, leaves at depth lvl) is (9^lvl - 1)/8. Let's check: for lvl=2, (81-1)/8=10, but snippet gives count = childs=9, dec=2, loop: dec=2, condition `--dec > 1` is false because after `--dec` it's 1, so loop does not run. Then `++count` gives 10. Yes matches. For lvl=3: count=9, dec=3, loop: dec becomes 2 (2>1 true), count=9*9+9=90, then dec becomes 1, loop ends, `++count`=91. Formula (9^3-1)/8 = (729-1)/8=728/8=91. Correct. So the total node count is (9^lvl - 1)/8. The `create` function uses `dist` to set the `diff` for skip; initially `dist = count` (total nodes), and each recursion sets `dist = max((dist-childs)/childs, 1)` which gives the size of each subtree. For a full tree, (count - 9)/9 = (total-9)/9 = (9^lvl - 1)/8 - 9 = (9^lvl - 73)/8? That doesn't simplify nicely, but the code uses integer division and `max` to ensure at least 1. The important point is that the recursion builds the tree correctly. The task asks to return the maximum number of intersection tests required to trace a single ray. Since the traversal visits every node when no bounding sphere is missed, that number equals the total number of nodes. So our function will simply build the tree and return the total node count. Edge cases: depth 1 gives 1 node; depth 0 is not valid; negative radius not allowed. The construction uses floating-point arithmetic; for simplicity we can ignore numerical issues. Complexity: building the tree visits each node once, O(N) time where N is the number of nodes, and O(depth) recursion stack space. The number of nodes grows exponentially with depth, so depth should be moderate (e.g., up to 6 is fine). The solution must avoid global mutable state, so we implement a class or free function that builds a local vector of nodes. We'll provide a `Sphere` struct with an `intersect` method that returns a double (distance) or bool, but since we don't actually trace a ray, we can just focus on construction. However, the task says "returns the maximum number of sphere intersection tests" – to make it meaningful, we can implement a simple traversal that counts how many nodes' bounding spheres are intersected by an example ray (say along the x-axis) and returns that count, but since the worst-case is all nodes, the answer is simply the total node count. To make the test meaningful, we can compute the total node count using the formula and compare with our constructed count. Alternatively, we can simulate a ray that goes through the center of the root sphere and hits all spheres? That might not happen because not all spheres contain the center. So the safest is to define the function to return the total number of nodes in the constructed tree, and phrase the task as "the maximum number of intersection tests in the worst-case traversal". The reference solution will build the tree and return its size. We'll include a helper recursive function that appends nodes to a vector and sets `diff`. We'll use `std::vector<Node>` to avoid manual memory management. The `Node` struct holds bounding center, leaf center, bounding radius, leaf radius, and diff. For simplicity, we don't need to implement actual ray intersection because the task only asks for the count. But to match the snippet, we can include an `intersect` method that returns a boolean (whether a ray hits the sphere) for completeness, but not use it. The solution must be self-contained and include necessary headers (`<vector>`, `<cmath>`, `<algorithm>`, `<cstddef>`). We'll write the free function `int buildSphereTree(double cx, double cy, double cz, double radius, int depth)` that returns the number of nodes. The function will internally construct the tree using a recursive helper. We'll use `std::sin` and `std::cos` directly. The basis computation from the snippet can be simplified: given a parent direction `d`, we need an orthonormal basis `(b1, b2)` perpendicular to `d`. We can pick any two orthogonal vectors to `d`. The snippet uses a hacky method; we can use a more robust approach: pick an arbitrary vector not parallel to `d`, compute `b1 = normalize(cross(d, arbitrary))`, `b2 = cross(d, b1)`. For our reference solution, we can just use a fixed arbitrary vector like `(1,0,0)` and adjust if parallel. But since we aren't actually tracing rays, we could skip the basis entirely and just generate child positions using simple deterministic patterns, but to match the snippet we should replicate the sphere tree geometry. However, the task says "inspired by a given code snippet" and the core is the BVH construction with 9 children per node. The exact positions are not crucial for the count; any full 9-ary tree will have the same node count. So we can simplify: generate child centers at a fixed offset from parent center, maybe using unit sphere distribution. But to be faithful, we'll use the snippet's direction generation using angles and basis. For the reference solution, we can implement a simpler version: for each node, create 9 children with centers placed at distance `(parent_radius + child_radius)` from parent center, in directions forming a pattern. We'll use the same angle increments: 6 in a lower ring (angle step 2π/6) with a slight downward tilt, and 3 in an upper ring (step 2π/3) with an upward tilt. The direction vector is computed using parent direction as the "up" reference, but we need an orthonormal basis. We'll write a small `orthoBasis` function that returns two perpendicular vectors to a given direction. Then child direction is `(d*coef + b1*sin(a) + b2*cos(a)).norm()`. This matches the snippet. The recursion stops at depth 1 (leaf). We'll use `depth` as `lvl` in the snippet. We'll store nodes in a vector and use `diff` to indicate the number of nodes to skip if the bounding sphere is missed. In a pre-order flat array, for a node at index `i`, its subtree occupies a contiguous range. The `diff` for a leaf is 1; for an internal node, it's the size of its entire subtree (including itself). We can compute this during recursion by appending the node first, then recursing into children, and then setting `diff` after children are added? Actually in the snippet, the node is placed first, and then children are placed after it, and the `diff` is set to the total number of nodes in the subtree, which is known beforehand if we compute the count via formula, but the snippet uses `dist` parameter passed down. In our solution, we can simply compute the total node count using the formula and pass it as `dist` to the recursive builder, similar to the snippet. Alternatively, we can build the tree recursively and after building children, compute `diff` as `1 + sum of children subtrees' sizes`. But since we are returning only the total count, we can just compute the count directly without building the full tree. However, the task says "builds a simple hierarchical scene" and "returns the maximum number of sphere intersection tests" – to make it non-trivial, we should actually construct the tree and then simulate a traversal that counts nodes visited. But a worst-case traversal (ray hits every bounding sphere) will visit all nodes, so we can just count nodes. So the solution can be: recursively count nodes in a full 9-ary tree of given depth. But that's too trivial for the task. To make it more aligned with the snippet, we can actually construct the node array and then count the number of nodes by traversing the array using the `diff` pointers, but that is just the array size. So perhaps the intended task is to implement the BVH construction and return the maximum recursion depth or the maximum number of `intersect` calls along any path, not the total. But the snippet's traversal uses a while loop and skip pointers; the number of `intersect` calls in the worst case is the total nodes visited, which is all nodes. So the answer is simply the total node count. To provide a meaningful test, we can verify that the construction produces the correct number of nodes using the formula. Our function will build the tree into a vector and return its size. The test will call it for several depths and compare with the formula `(pow(9, depth) - 1) / 8`. We'll also test that depth 1 returns 1, depth 2 returns 10, etc. For depth 6, the count is 597871? Let's compute: 9^6=531441, (531441-1)/8=531440/8=66430. So for depth 6, count = 66430. The snippet's TEST_SIZE 1024 but that's for rendering; the sphere count is 66430 for lvl=6. That's feasible to build. Our function can handle depth up to maybe 8 (9^8=43046721, count~5 million) which is okay. But we'll keep test depth small. The reference solution will include a `Sphere` struct with an `intersect` method that returns a double (distance) or bool for completeness, but the main function just builds the tree and returns the size. We'll use `std::vector<Node>` with `emplace_back`. No global state. We'll use `static`? No, avoid global. We'll implement a helper class `SphereTreeBuilder` with a member vector. But the task requires a free function, so we can define a namespace or just a function that internally uses a recursive lambda or a static helper function. We'll define a recursive function `buildNodes(std::vector<Node>& nodes, int depth, double radius, ...)` that appends nodes and returns the index of the first node of the subtree? Actually easier: we'll write a recursive function that takes references to the vector and a parent direction, center, radius, level, and returns the number of nodes in the subtree it just created. It first appends a node (bounding sphere radius=2r, leaf sphere radius=r), then if level>1, it computes `dist = max((remaining?)` but we don't need `dist` for the count. We'll just append children recursively. Since the vector grows in pre-order, we don't need `diff` for counting. But to be faithful, we'll set `diff` appropriately: after we know the total number of nodes in this subtree (which we can compute via formula for that subtree), we can set it. But simpler: after building children, we know the number of children's nodes from returns, so we can set `diff = 1 + sum(child_counts)` for internal nodes, and `diff = 1` for leaves. That works. We'll implement that. The function signature: `int buildSphereTree(double cx, double cy, double cz, double radius, int depth)`. It will create a vector, call a recursive helper, and return the vector size. The recursive helper takes `std::vector<Node>&`, parent center, parent direction, radius, current level, and returns the number of nodes added in this subtree. It adds a node with leaf sphere at given center and radius, bounding sphere radius=2*radius. If level==1, set diff=1 and return 1. Otherwise, compute child radius = radius/3, and for 9 children, generate directions using basis and angles, compute child center = parent_center + dir*(radius+child_radius), recurse, and accumulate child counts. After all children, set this node's diff = 1 + sum(child_counts). Return that sum. We need an orthonormal basis. We'll write a function `std::array<Vec3,2> makeBasis(Vec3 d)` that returns two perpendicular vectors. Use a robust method: choose an arbitrary vector `a = (1,0,0)` if `|d.x| < 0.9` else `(0,1,0)`. Then `b1 = normalize(cross(d,a))`, `b2 = cross(d,b1)`. All vectors normalized. Define a `Vec3` struct with `+`, `*`, `cross`, `dot`, `norm`. We'll use `std::sin`, `std::cos`. The angles: for lower ring, `a` starts at 0, increment `2π/6`; for upper ring, start at `a - daL/3` as in snippet, then increment `2π/3`. But the exact angles don't matter for count. We'll just use the snippet's loop: lower ring 6 times, upper ring 3 times. For the upper ring, we use `d*0.6 + b1*sin(a) + b2*cos(a)`. For lower ring, `d*-0.2 + ...`. The child radius is `r/3`. The parent direction is normalized. The root direction from snippet is `v_t(+.25,+1,-.5).norm()`. We'll use that as default. The function will take the root center and radius and depth, and return the total node count. We'll also include a comment that the maximum number of intersection tests in the worst case equals the total number of nodes. The solution must not include a main function; only the free function and helper structs. The test will include assertions comparing the returned count with the formula `(pow(9, depth)-1)/8` for several depths. We'll need to include `<cmath>` for pow, but we can compute integer power manually. Use a loop to compute integer power of 9 to avoid floating-point issues. The test will use `assert` from `<cassert>`.
