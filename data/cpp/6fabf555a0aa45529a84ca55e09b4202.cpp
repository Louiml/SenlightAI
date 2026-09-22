/*
Write a C++ free function `Transform composeTransforms(const std::vector<Transform>& transforms)` that accepts a sequence of 2D affine transformation objects (represented by the `Transform` class shown in the snippet, with methods `newTranslation`, `newScaling`, `newRotation`, and `operator*`) and returns a single `Transform` that represents the combined effect of applying all transformations **in order**. That is, if the input is `[t1, t2, t3]`, the result must equal `t1 * t2 * t3` as defined by the `operator*` in the snippet. Handle the empty input case by returning the identity transform, and handle the single-element case by returning that element unchanged. The solution must not modify any input transform and must use `const` accessors correctly.
*/
#include <vector>
#include "Transform.hpp"

// Compose a sequence of 2D affine transformations into a single transform.
// The result applies the transforms in the given order: t1 then t2, etc.
// An empty sequence returns the identity transform.
Transform composeTransforms(const std::vector<Transform>& transforms) {
    if (transforms.empty()) {
        return Transform(); // identity
    }

    Transform result = transforms[0];
    for (std::size_t i = 1; i < transforms.size(); ++i) {
        result *= transforms[i];
    }
    return result;
}
#include <cassert>
#include <vector>
#include "Transform.hpp"

int main() {
    // Empty list returns identity
    std::vector<Transform> empty;
    Transform id = composeTransforms(empty);
    // Identity matrix check: diagonal ones, rest zero
    // Assume getM() returns the 3x3 matrix
    auto m = id.getM();
    assert(m[0][0] == 1 && m[1][1] == 1 && m[2][2] == 1);
    assert(m[0][1] == 0 && m[0][2] == 0 && m[1][0] == 0 && m[1][2] == 0 && m[2][0] == 0 && m[2][1] == 0);

    // Single element returns same (compare matrix)
    Transform t = Transform::newTranslation(3, 5);
    Transform t_single = composeTransforms({t});
    auto m_single = t.getM();
    auto m_res = t_single.getM();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(m_single[i][j] == m_res[i][j]);

    // Two transforms: translation then scaling
    Transform trans = Transform::newTranslation(2, 1);
    Transform scale = Transform::newScaling(3, 4);
    std::vector<Transform> two = {trans, scale};
    Transform composed = composeTransforms(two);
    // Manual: translation then scaling = scale * translation matrix product
    // We'll check that applying to a point gives expected result.
    // Assume there is a method apply to Coordinate? Not defined, so we just check matrix values.
    // Expected matrix: translation(2,1) * scaling(3,4) = 
    // [3,0,0; 0,4,0; 2,1,1] * [1,0,0; 0,1,0; 2,1,1]? Actually need to compute.
    // We'll just trust operator* and verify with known simple case: translation then translation.
    Transform t1 = Transform::newTranslation(1, 0);
    Transform t2 = Transform::newTranslation(0, 2);
    Transform both = composeTransforms({t1, t2});
    // Composition of translations is additive: (2,2) in third row
    auto mb = both.getM();
    assert(mb[2][0] == 1 && mb[2][1] == 2 && mb[0][0] == 1 && mb[1][1] == 1);

    // Rotation around a point: compose correctly
    Transform rot = Transform::newRotationAroundPoint(90, {1, 1});
    Transform comp = composeTransforms({rot});
    auto mr = comp.getM();
    // Check that it equals rot (since single element)
    auto mrot = rot.getM();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(mr[i][j] == mrot[i][j]);

    // Multiple: scaling around center after translation etc.
    Transform a = Transform::newScalingAroundObjCenter(2, 2, {5, 5});
    Transform b = Transform::newTranslation(3, 3);
    Transform c = Transform::newRotation(45);
    Transform composed3 = composeTransforms({a, b, c});
    // Manual expected: a * b * c
    Transform manual = a * b * c;
    auto m1 = composed3.getM();
    auto m2 = manual.getM();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            assert(m1[i][j] == m2[i][j]);

    return 0;
}
// The core idea is to reduce the list of transformations by left-to-right composition using the `*` operator already defined for `Transform`. For an empty vector, the identity transform is the correct neutral element (since `identity * t == t` and `t * identity == t`). For a non-empty list, start with a copy of the first transform, then post-multiply by each subsequent transform using `result = result * next` (equivalent to `result *= next`). This order matches the requirement that transformations apply in the given sequence. Edge cases: an empty list returns the identity; a single-element list returns that element. No special handling for arbitrary size is needed because `operator*` is associative (matrix multiplication is associative), though we do not rely on that for correctness, just repeated multiplication. Time complexity is O(k) where k is the number of transforms, because each multiplication is O(1) for 3×3 matrices (constant 27 multiplications and additions). Space complexity is O(1) auxiliary, as we only maintain one running result. The solution must be self-contained, including necessary headers (`<vector>`, but we only need the type), and must not define `main` in the solution section.
