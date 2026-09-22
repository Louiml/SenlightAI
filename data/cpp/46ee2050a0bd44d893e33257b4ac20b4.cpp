Write a C++ function that computes and returns the bounding box of a set of axis-aligned 3D boxes after merging any overlapping or touching boxes. Given a vector of boxes, each defined by minimum and maximum coordinates (x, y, z), the function must merge boxes that intersect or touch (i.e., share any volume or face) into larger composite boxes, repeating until no further merges are possible. The output should be a vector of merged boxes, each represented by its min and max coordinates. Boxes are axis-aligned and coordinates are integers. The order of the output does not matter.

// The core algorithm is iterative merging. Start with the list of input boxes. For each pair of boxes, if they overlap or touch (i.e., for every dimension, max(min1, min2) ≤ min(max1, max2)), merge them into a new box whose min is the component-wise minimum of the two and max is the component-wise maximum of the two. Replace the two boxes with the merged one, and restart the scan from the beginning, because the new box might overlap with others. Continue until a full pass over all pairs finds no merges. Edge cases include empty input (return empty vector), a single box (return it unchanged), and boxes that touch only at a face or edge — they should be merged (this is handled by the ≤ condition). Time complexity is O(m²) per pass and worst-case O(k·m²) where k is the number of merge passes, which can be up to m-1 in pathological cases, leading to O(m³) worst-case. Space complexity is O(m) for the result vector, not counting input storage.

#include <vector>
#include <cstddef>

struct Box {
    int minX, minY, minZ;
    int maxX, maxY, maxZ;
};

// Check if two boxes overlap or touch (share any volume or face).
bool boxesOverlapOrTouch(const Box& a, const Box& b) {
    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
           (a.minY <= b.maxY && a.maxY >= b.minY) &&
           (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}

// Merge two boxes into the smallest enclosing box.
Box mergeBoxes(const Box& a, const Box& b) {
    return {
        std::min(a.minX, b.minX),
        std::min(a.minY, b.minY),
        std::min(a.minZ, b.minZ),
        std::max(a.maxX, b.maxX),
        std::max(a.maxY, b.maxY),
        std::max(a.maxZ, b.maxZ)
    };
}

// Merge all overlapping/touching boxes into a minimal set of disjoint boxes.
std::vector<Box> mergeAllOverlappingBoxes(const std::vector<Box>& input) {
    if (input.empty()) return {};

    std::vector<Box> result = input;
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t i = 0; i < result.size(); ++i) {
            for (std::size_t j = i + 1; j < result.size(); ++j) {
                if (boxesOverlapOrTouch(result[i], result[j])) {
                    // Merge box j into box i, then erase box j.
                    result[i] = mergeBoxes(result[i], result[j]);
                    result.erase(result.begin() + j);
                    changed = true;
                    // Restart scanning because the merged box may overlap others.
                    i = 0; // will be incremented to 0 next loop
                    j = result.size(); // break inner loop
                    break;
                }
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume Box and the solution function are defined above.

int main() {
    // Empty input returns empty.
    std::vector<Box> empty;
    assert(mergeAllOverlappingBoxes(empty).empty());

    // Single box unchanged.
    std::vector<Box> single = {{0, 0, 0, 1, 1, 1}};
    auto singleResult = mergeAllOverlappingBoxes(single);
    assert(singleResult.size() == 1);
    assert(singleResult[0].minX == 0 && singleResult[0].minY == 0 && singleResult[0].minZ == 0);
    assert(singleResult[0].maxX == 1 && singleResult[0].maxY == 1 && singleResult[0].maxZ == 1);

    // Two overlapping boxes merge into one.
    std::vector<Box> overlapping = {{0, 0, 0, 2, 2, 2}, {1, 1, 1, 3, 3, 3}};
    auto ovResult = mergeAllOverlappingBoxes(overlapping);
    assert(ovResult.size() == 1);
    assert(ovResult[0].minX == 0 && ovResult[0].minY == 0 && ovResult[0].minZ == 0);
    assert(ovResult[0].maxX == 3 && ovResult[0].maxY == 3 && ovResult[0].maxZ == 3);

    // Two touching boxes (face contact) merge.
    std::vector<Box> touching = {{0, 0, 0, 1, 1, 1}, {1, 0, 0, 2, 1, 1}};
    auto touchResult = mergeAllOverlappingBoxes(touching);
    assert(touchResult.size() == 1);
    assert(touchResult[0].minX == 0 && touchResult[0].minY == 0 && touchResult[0].minZ == 0);
    assert(touchResult[0].maxX == 2 && touchResult[0].maxY == 1 && touchResult[0].maxZ == 1);

    // Disjoint boxes remain separate.
    std::vector<Box> disjoint = {{0, 0, 0, 1, 1, 1}, {5, 5, 5, 6, 6, 6}};
    auto disResult = mergeAllOverlappingBoxes(disjoint);
    assert(disResult.size() == 2);

    // Chain of boxes: A touches B, B touches C, all merge into one.
    std::vector<Box> chain = {{0, 0, 0, 1, 1, 1}, {1, 0, 0, 2, 1, 1}, {2, 0, 0, 3, 1, 1}};
    auto chainResult = mergeAllOverlappingBoxes(chain);
    assert(chainResult.size() == 1);
    assert(chainResult[0].minX == 0 && chainResult[0].maxX == 3);
    assert(chainResult[0].minY == 0 && chainResult[0].maxY == 1);
    assert(chainResult[0].minZ == 0 && chainResult[0].maxZ == 1);

    // A box completely contained in another merges into the larger.
    std::vector<Box> contained = {{0, 0, 0, 10, 10, 10}, {2, 2, 2, 3, 3, 3}};
    auto contResult = mergeAllOverlappingBoxes(contained);
    assert(contResult.size() == 1);
    assert(contResult[0].minX == 0 && contResult[0].minY == 0 && contResult[0].minZ == 0);
    assert(contResult[0].maxX == 10 && contResult[0].maxY == 10 && contResult[0].maxZ == 10);

    // Mixed: box 1 and 2 overlap, box 3 is disjoint.
    std::vector<Box> mixed = {{0, 0, 0, 2, 2, 2}, {1, 1, 1, 3, 3, 3}, {10, 10, 10, 11, 11, 11}};
    auto mixedResult = mergeAllOverlappingBoxes(mixed);
    assert(mixedResult.size() == 2);

    // Negative coordinates work as expected.
    std::vector<Box> negative = {{-5, -5, -5, -1, -1, -1}, {-3, -3, -3, 0, 0, 0}};
    auto negResult = mergeAllOverlappingBoxes(negative);
    assert(negResult.size() == 1);
    assert(negResult[0].minX == -5 && negResult[0].minY == -5 && negResult[0].minZ == -5);
    assert(negResult[0].maxX == 0 && negResult[0].maxY == 0 && negResult[0].maxZ == 0);

    return 0;
}
