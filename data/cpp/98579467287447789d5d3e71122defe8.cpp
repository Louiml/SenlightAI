// Write a C++ function `classify_boundary_cells` that takes a vector of vectors of integers `cells` (each inner vector represents a simplex: either a triangle with 3 vertex indices or a tetrahedron with 4 vertex indices) and returns a pair of vectors: (1) a vector `is_boundary` of booleans, where `is_boundary[i]` is `true` if the i-th simplex has at least one face on the boundary of the mesh, and (2) a vector of vector of booleans `face_on_boundary`, where `face_on_boundary[i][j]` indicates whether the j-th face of the i-th simplex is on the boundary (a face is on the boundary if it appears only once among all faces of all simplices, assuming the mesh is closed/watertight, meaning each interior face appears exactly twice). All simplices in the input must have the same dimension (3 or 4 vertices). The function should handle an empty input by returning empty vectors. Assume the mesh is manifold and each face appears either once (boundary) or twice (interior). The orientation of the faces is fixed: for a triangle `(a,b,c)`, its faces are `(b,c)`, `(c,a)`, `(a,b)`; for a tetrahedron `(a,b,c,d)`, the faces are `(b,d,c)`, `(a,c,d)`, `(a,d,b)`, `(a,b,c)` as given in the snippet. Write the solution as a free function with a descriptive name, using only the C++ standard library (no external libraries like Eigen or IGL).

The goal is to identify boundary faces by counting how many times each unique face appears across all simplices. For a closed manifold mesh, each interior face is shared by exactly two simplices, so a face appearing exactly once is on the boundary. The algorithm proceeds in three steps: (1) Build a list of all faces in the fixed orientation order from each simplex. (2) Count occurrences of each face using a hash map (e.g., `std::map` or `std::unordered_map` with a custom key). Because vertex indices can be arbitrary integers, a simple approach is to convert each face to a normalized string or tuple (e.g., `std::vector<int>` sorted? Wait—careful: the orientation is fixed and we must count exact occurrences; the same face appearing in two simplices may have opposite orientation in the list? In the snippet, they gather faces in a specific orientation, and then `face_occurrences` counts how many times that exact ordered triple appears. But in a consistent mesh, the same face will appear exactly twice with the same orientation? Actually, in a properly oriented tetrahedral mesh, each interior face appears twice but with opposite orientations (one from each adjacent tetrahedron). The snippet's `face_occurrences` likely treats faces as unordered (i.e., it normalizes by sorting the vertex indices) because boundary detection should be independent of orientation. Looking at the snippet: they gather faces like `F[i*4+0] = {T[i][1], T[i][3], T[i][2]}` etc., and then count occurrences. If the mesh is consistent, each interior face will appear exactly twice regardless of orientation if they normalize. But the code's `face_occurrences` is not shown; however, from the assert `FC[i*4+j] == 2 || FC[i*4+j] == 1`, it implies that the count treats a face and its reverse as the same? Wait, the assert says counts are either 1 or 2, which means they treat the unordered face (set of vertices) for counting. Because if they counted ordered faces, an interior face would appear once with one orientation and once with opposite, giving count 1 for each ordered variant, but then the total ordered count for a given specific ordered triple could be 1 even for interior faces, leading to false positives. So the intended meaning is that `face_occurrences` counts occurrences of the *set* of vertices, ignoring order. Therefore, in our implementation we must normalize each face by sorting its vertex indices before counting. For a triangle, sort the three; for a tetrahedron face, sort the three vertices of the face. After counting, for each simplex, for each of its faces (in original order), we check the count of that normalized face: if count == 1, then it's boundary; if count == 2, interior. Edge cases: empty input returns empty vectors. Input may contain duplicate simplices? Possibly, but then counts might be >2; we can either assert or just treat any count >1 as interior? The task statement says assume each face appears once or twice, so we can assert that. Time complexity: Building all faces takes O(N * d) where N is number of simplices and d is 3 or 4. Counting via hash map is O(N) average. Space: O(N * d) for faces and counts. For simplicity, we can use `std::map` with a `std::vector<int>` key (after sorting) giving O(N log N) time, but that's acceptable. We'll write a helper to normalize a face.

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

// Given a vector of simplices (each either a triangle with 3 vertex indices
// or a tetrahedron with 4), returns a pair:
//   first: is_boundary[i] = true if simplex i has at least one boundary face
//   second: face_on_boundary[i][j] = true if the j-th face of simplex i is on the boundary
// A face is on the boundary if it appears exactly once among all simplices.
// It is assumed that every face appears either once (boundary) or twice (interior).
// For a triangle (a,b,c), faces are (b,c), (c,a), (a,b).
// For a tetrahedron (a,b,c,d), faces are (b,d,c), (a,c,d), (a,d,b), (a,b,c).
std::pair<std::vector<bool>, std::vector<std::vector<bool>>>
classify_boundary_cells(const std::vector<std::vector<int>>& cells) {
    // Empty input -> empty output
    if (cells.empty()) {
        return { {}, {} };
    }

    const int dim = static_cast<int>(cells[0].size());
    assert(dim == 3 || dim == 4);

    // Build list of all faces (as normalized sorted vectors) and count occurrences
    // We use a map with vector<int> as key (faces have 3 vertices always)
    std::map<std::vector<int>, int> face_count;
    // Also store the original (non-normalized) faces for each simplex in order
    std::vector<std::vector<std::vector<int>>> original_faces(cells.size());

    for (size_t i = 0; i < cells.size(); ++i) {
        assert(cells[i].size() == static_cast<size_t>(dim));
        std::vector<std::vector<int>> faces;
        if (dim == 3) {
            // Triangle: faces are (b,c), (c,a), (a,b)
            std::vector<std::vector<int>> tri_faces = {
                {cells[i][1], cells[i][2]},
                {cells[i][2], cells[i][0]},
                {cells[i][0], cells[i][1]}
            };
            faces = tri_faces;
        } else { // dim == 4
            // Tetrahedron: faces as in the snippet
            std::vector<std::vector<int>> tet_faces = {
                {cells[i][1], cells[i][3], cells[i][2]},
                {cells[i][0], cells[i][2], cells[i][3]},
                {cells[i][0], cells[i][3], cells[i][1]},
                {cells[i][0], cells[i][1], cells[i][2]}
            };
            faces = tet_faces;
        }
        original_faces[i] = faces;
        // Count normalized (sorted) version of each face
        for (const auto& face : faces) {
            std::vector<int> sorted_face = face;
            std::sort(sorted_face.begin(), sorted_face.end());
            face_count[sorted_face]++;
        }
    }

    // Now determine boundary status for each face and each simplex
    std::vector<bool> is_boundary(cells.size(), false);
    std::vector<std::vector<bool>> face_on_boundary(cells.size());

    for (size_t i = 0; i < cells.size(); ++i) {
        face_on_boundary[i].resize(dim == 3 ? 3 : 4);
        for (size_t j = 0; j < original_faces[i].size(); ++j) {
            std::vector<int> sorted_face = original_faces[i][j];
            std::sort(sorted_face.begin(), sorted_face.end());
            int cnt = face_count[sorted_face];
            // Expect only 1 or 2
            assert(cnt == 1 || cnt == 2);
            bool is_boundary_face = (cnt == 1);
            face_on_boundary[i][j] = is_boundary_face;
            if (is_boundary_face) {
                is_boundary[i] = true;
            }
        }
    }

    return { is_boundary, face_on_boundary };
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
// For the test, we include the necessary header and implementation.

int main() {
    // Test 1: Empty input
    {
        std::vector<std::vector<int>> cells;
        auto result = classify_boundary_cells(cells);
        assert(result.first.empty());
        assert(result.second.empty());
    }

    // Test 2: Single triangle (all faces boundary)
    {
        std::vector<std::vector<int>> cells = {{0, 1, 2}};
        auto result = classify_boundary_cells(cells);
        assert(result.first.size() == 1);
        assert(result.first[0] == true);
        assert(result.second.size() == 1);
        assert(result.second[0] == std::vector<bool>({true, true, true}));
    }

    // Test 3: Two triangles sharing an edge (interior edge, boundary other two edges)
    // Triangle0: (0,1,2) -> faces (1,2), (2,0), (0,1)
    // Triangle1: (1,3,2) -> faces (3,2), (2,1), (1,3)
    // Shared edge (1,2) appears twice -> interior; others once -> boundary
    {
        std::vector<std::vector<int>> cells = {{0,1,2}, {1,3,2}};
        auto result = classify_boundary_cells(cells);
        assert(result.first == std::vector<bool>({true, true})); // both have boundary faces
        // Triangle0: face (1,2) is interior -> false, others true
        assert(result.second[0] == std::vector<bool>({false, true, true}));
        // Triangle1: face (3,2) true, (2,1) false, (1,3) true
        assert(result.second[1] == std::vector<bool>({true, false, true}));
    }

    // Test 4: Single tetrahedron (all 4 faces boundary)
    {
        std::vector<std::vector<int>> cells = {{0,1,2,3}};
        auto result = classify_boundary_cells(cells);
        assert(result.first == std::vector<bool>({true}));
        assert(result.second[0] == std::vector<bool>({true, true, true, true}));
    }

    // Test 5: Two tetrahedra sharing one face (interior face, other faces boundary)
    // Tet0: (0,1,2,3) -> faces: (1,3,2), (0,2,3), (0,3,1), (0,1,2)
    // Tet1: (0,1,4,3) -> faces: (1,3,4)? Wait need to define carefully.
    // Let's construct: Tet0 (0,1,2,3) and Tet1 (0,2,3,4) share face (0,2,3)? 
    // Actually easier: use the snippet's face ordering. Let's do:
    // Tet0: (0,1,2,3) -> faces: {1,3,2}, {0,2,3}, {0,3,1}, {0,1,2}
    // Tet1: (0,2,3,4) -> faces: {2,4,3}, {0,3,4}, {0,4,2}, {0,2,3}
    // Shared face {0,2,3} appears in both -> interior; others appear once.
    {
        std::vector<std::vector<int>> cells = {{0,1,2,3}, {0,2,3,4}};
        auto result = classify_boundary_cells(cells);
        assert(result.first == std::vector<bool>({true, true}));
        // Tet0 faces: (1,3,2)->boundary, (0,2,3)->interior, (0,3,1)->boundary, (0,1,2)->boundary
        assert(result.second[0] == std::vector<bool>({true, false, true, true}));
        // Tet1 faces: (2,4,3)->boundary, (0,3,4)->boundary, (0,4,2)->boundary, (0,2,3)->interior
        assert(result.second[1] == std::vector<bool>({true, true, true, false}));
    }

    // Test 6: Closed mesh of 2 triangles (two triangles forming a "pillow" with no boundary?)
    // Actually two triangles can't form a closed surface, but for triangles assume only one layer.
    // Instead test a closed tetrahedral mesh? Hard to construct simple. Skip.

    // Test 7: Mixed dimension input should trigger assert, but we don't test that.

    return 0;
}
