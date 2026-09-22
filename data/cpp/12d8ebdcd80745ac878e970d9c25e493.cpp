/*
Write a standalone C++ function `reorder_vertices_faces_first` that takes a dense matrix `V` (double, each row is a vertex coordinate) and a dense matrix `F` (integer, each row is a face/triangle with vertex indices) and returns three outputs: a reordered vertex matrix `RV`, a reindexed face matrix `RF`, and an index mapping vector `IM` such that:
- All vertices that appear in `F` (`in_face` vertices) are placed at the top of `RV`, in their original relative order.
- The remaining vertices (not referenced by any face) are placed after them, also preserving original relative order.
- The face indices in `RF` are updated so that `RF(i,j) == IM(F(i,j))`.
- The reordering must be stable with respect to original vertex order, and the function must not modify the input arguments.
*/

#include <vector>
#include <cassert>
#include <cstddef>

// Reorder vertices so that those appearing in F come first.
// V: input vertex matrix (N x dim), F: face matrix (M x k) with vertex indices.
// RV: reordered vertex matrix (same size), RF: reindexed face matrix, IM: mapping old->new.
template <typename MatV, typename MatF, typename VecI>
void reorder_vertices_faces_first(
    const MatV& V,
    const MatF& F,
    MatV& RV,
    MatF& RF,
    VecI& IM)
{
    const std::size_t n = static_cast<std::size_t>(V.rows());
    const std::size_t m = static_cast<std::size_t>(F.rows());

    // Step 1: mark which vertices are referenced by any face.
    std::vector<bool> in_face(n, false);
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < static_cast<std::size_t>(F.cols()); ++j) {
            const auto idx = static_cast<std::size_t>(F(i, j));
            assert(idx < n && "Face index out of bounds");
            in_face[idx] = true;
        }
    }

    // Step 2: count and collect used and unused original indices in stable order.
    std::size_t num_used = 0;
    for (const bool b : in_face) if (b) ++num_used;

    std::vector<std::size_t> used_indices;
    used_indices.reserve(num_used);
    std::vector<std::size_t> unused_indices;
    unused_indices.reserve(n - num_used);

    for (std::size_t i = 0; i < n; ++i) {
        if (in_face[i]) used_indices.push_back(i);
        else unused_indices.push_back(i);
    }

    // Step 3: build mapping from original index to new index.
    IM.resize(n);
    for (std::size_t i = 0; i < used_indices.size(); ++i) {
        IM[used_indices[i]] = static_cast<typename VecI::Scalar>(i);
    }
    const typename VecI::Scalar offset = static_cast<typename VecI::Scalar>(used_indices.size());
    for (std::size_t i = 0; i < unused_indices.size(); ++i) {
        IM[unused_indices[i]] = static_cast<typename VecI::Scalar>(i) + offset;
    }

    // Step 4: reindex faces.
    RF.resizeLike(F);
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < static_cast<std::size_t>(F.cols()); ++j) {
            RF(i, j) = IM[F(i, j)];
        }
    }

    // Step 5: reorder vertices.
    RV.resizeLike(V);
    for (std::size_t old_idx = 0; old_idx < n; ++old_idx) {
        const std::size_t new_idx = static_cast<std::size_t>(IM[old_idx]);
        RV.row(new_idx) = V.row(old_idx);
    }
}

#include <iostream>
#include <cassert>
#include <Eigen/Dense>

int main() {
    using MatV = Eigen::MatrixXd;
    using MatF = Eigen::MatrixXi;
    using VecI = Eigen::VectorXi;

    // Case 1: typical triangle mesh, some vertices unused.
    MatV V(5, 3);
    V << 0, 0, 0,
         1, 0, 0,
         1, 1, 0,
         0, 1, 0,
         9, 9, 9; // vertex 4 not in any face
    MatF F(2, 3);
    F << 0, 1, 2,
         0, 2, 3;
    MatV RV;
    MatF RF;
    VecI IM;
    reorder_vertices_faces_first(V, F, RV, RF, IM);

    // Used vertices (0,1,2,3) appear first, then unused (4).
    assert(RV.rows() == 5 && RV.cols() == 3);
    assert(RV.row(0) == V.row(0));
    assert(RV.row(1) == V.row(1));
    assert(RV.row(2) == V.row(2));
    assert(RV.row(3) == V.row(3));
    assert(RV.row(4) == V.row(4));
    assert(RF.rows() == 2 && RF.cols() == 3);
    assert(RF(0,0) == 0 && RF(0,1) == 1 && RF(0,2) == 2);
    assert(RF(1,0) == 0 && RF(1,1) == 2 && RF(1,2) == 3);
    assert(IM.size() == 5);
    assert(IM(0)==0 && IM(1)==1 && IM(2)==2 && IM(3)==3 && IM(4)==4);

    // Case 2: vertex 0 is unused, others used.
    MatV V2(4, 2);
    V2 << 7, 7,
          0, 0,
          1, 0,
          0, 1;
    MatF F2(1, 3);
    F2 << 1, 2, 3;
    MatV RV2; MatF RF2; VecI IM2;
    reorder_vertices_faces_first(V2, F2, RV2, RF2, IM2);
    assert(RV2.row(0) == V2.row(1));
    assert(RV2.row(1) == V2.row(2));
    assert(RV2.row(2) == V2.row(3));
    assert(RV2.row(3) == V2.row(0));
    assert(RF2(0,0) == 0 && RF2(0,1) == 1 && RF2(0,2) == 2);
    assert(IM2(1)==0 && IM2(2)==1 && IM2(3)==2 && IM2(0)==3);

    // Case 3: all vertices used.
    MatV V3(3, 2);
    V3 << 0,0,
          1,0,
          1,1;
    MatF F3(1, 3);
    F3 << 0, 1, 2;
    MatV RV3; MatF RF3; VecI IM3;
    reorder_vertices_faces_first(V3, F3, RV3, RF3, IM3);
    assert(RV3 == V3);
    assert(RF3 == F3);
    assert(IM3(0)==0 && IM3(1)==1 && IM3(2)==2);

    // Case 4: no faces, all vertices unused -> identity mapping.
    MatV V4(3, 1);
    V4 << 1.0, 2.0, 3.0;
    MatF F4(0, 3); // empty matrix with 3 columns
    MatV RV4; MatF RF4; VecI IM4;
    reorder_vertices_faces_first(V4, F4, RV4, RF4, IM4);
    assert(RV4 == V4);
    assert(RF4.rows() == 0 && RF4.cols() == 3);
    assert(IM4(0)==0 && IM4(1)==1 && IM4(2)==2);

    // Case 5: duplicate vertex in same face.
    MatV V5(2, 2);
    V5 << 0,0,
          1,1;
    MatF F5(1, 4); // quad with repeated vertex 0
    F5 << 0, 1, 0, 1;
    MatV RV5; MatF RF5; VecI IM5;
    reorder_vertices_faces_first(V5, F5, RV5, RF5, IM5);
    assert(RV5 == V5);
    assert(RF5(0,0)==0 && RF5(0,1)==1 && RF5(0,2)==0 && RF5(0,3)==1);
    assert(IM5(0)==0 && IM5(1)==1);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The algorithm first marks which vertices are used by scanning every entry of `F` and setting a boolean flag in a `vector<bool>` of size `V.rows()`. Then it counts how many vertices are used, and partitions original indices into two lists: `U` (used) and `NU` (not used), each preserving original order. Next, it constructs a mapping vector `IM` of size `V.rows()` where `IM[orig_index] = new_index`. For used vertices, new indices are 0..num_used-1; for unused, num_used..V.rows()-1. Then it builds `RF` by applying `IM` to every entry of `F`. Finally, it builds `RV` by copying each row of `V` from original position `i` to new row `IM[i]`. Edge cases: empty `F` (then `U` is empty, all vertices move to the tail, `IM` is identity), duplicate vertex indices in faces (handled naturally by boolean flags), and `V` with zero rows (trivially works). Complexity: O(n + m) time where n = V.rows() and m = F.rows()*F.cols(); auxiliary space O(n) for flags, lists, and `IM`.
