Write a C++ function `classifySticks` that takes four stick lengths as integers and returns a string `"TRIANGLE"` if the sticks can form a non-degenerate triangle, `"SEGMENT"` if they can form a degenerate triangle (where the sum of the two shorter lengths equals the longest length), and `"IMPOSSIBLE"` otherwise. The function must handle any ordering of the input lengths and any positive integers, including edge cases where sticks have equal lengths or very large values that do not overflow an `int`.

#include <cassert>
#include <string>

// Declare the solution function (already provided above).
std::string classifySticks(int a, int b, int c, int d);

int main() {
    // Basic triangle (3-4-5 works, and any 4 of them form a triangle)
    assert(classifySticks(3, 4, 5, 6) == "TRIANGLE");

    // Degenerate segment: 1 + 1 = 2, and the fourth stick is small enough
    assert(classifySticks(1, 1, 2, 1) == "SEGMENT");

    // Impossible: 1, 1, 1, 100 (no three can form a triangle)
    assert(classifySticks(1, 1, 1, 100) == "IMPOSSIBLE");

    // Reversed input ordering should not matter
    assert(classifySticks(5, 3, 4, 2) == "TRIANGLE");

    // Edge case: all equal sticks (equilateral triangle)
    assert(classifySticks(7, 7, 7, 7) == "TRIANGLE");

    // Two equal sticks and one that equals their sum (degenerate)
    assert(classifySticks(2, 2, 4, 5) == "SEGMENT");

    // Very large values that fit in int (e.g., 2e9, 2e9, 2e9, 1)
    assert(classifySticks(2000000000, 2000000000, 2000000000, 1) == "TRIANGLE");

    // Zero-length sticks are not allowed (positive integers), but test 1,1,1,1
    assert(classifySticks(1, 1, 1, 1) == "TRIANGLE");

    // Impossible with a near miss: 1, 2, 3, 4 -> 1+2=3 degenerate, but also 2+3>4 triangle
    assert(classifySticks(1, 2, 3, 4) == "TRIANGLE");

    // Impossible: 1, 2, 3, 6 (all triples fai)
    assert(classifySticks(1, 2, 3, 6) == "IMPOSSIBLE");

    return 0;
}

#include <algorithm>
#include <string>

// Classifies four stick lengths as TRIANGLE, SEGMENT, or IMPOSSIBLE.
std::string classifySticks(int a, int b, int c, int d) {
    int lengths[4] = {a, b, c, d};
    std::sort(lengths, lengths + 4);

    bool hasTriangle = false;
    bool hasSegment = false;

    // Check all four possible triples formed by removing one stick.
    const int triples[4][3] = {
        {0, 1, 2},
        {0, 1, 3},
        {0, 2, 3},
        {1, 2, 3}
    };

    for (const auto& t : triples) {
        int x = lengths[t[0]];
        int y = lengths[t[1]];
        int z = lengths[t[2]];
        // Since sorted, x <= y <= z, so triangle condition is x + y > z.
        if (x + y > z) {
            hasTriangle = true;
        } else if (x + y == z) {
            hasSegment = true;
        }
    }

    if (hasTriangle) {
        return "TRIANGLE";
    }
    if (hasSegment) {
        return "SEGMENT";
    }
    return "IMPOSSIBLE";
}

// The key insight is that for any four sticks, if we sort them ascending, the most restrictive condition for forming a triangle is among the three shortest sticks (`length[0]`, `length[1]`, `length[2]`) and among the three longest sticks (`length[1]`, `length[2]`, `length[3]`). However, the original snippet only checks two specific combinations: `length[1]+length[2] > length[3]` and `length[0]+length[1] > length[2]`. This is insufficient because it misses cases like `1, 1, 1, 100` where `1+1 > 1` holds (true) but `1+1 > 100` does not, but the snippets logic might incorrectly classify. A correct approach: sort the four lengths, then check all four possible triples (by removing one stick each) to see if any forms a triangle. For a valid triangle, the sum of the two smaller sides must be strictly greater than the largest side; for a degenerate segment, it must be equal; otherwise impossible. To avoid duplication and ensure correctness, we check triples (0,1,2), (0,1,3), (0,2,3), and (1,2,3). If any strict inequality holds → `"TRIANGLE"`. If no strict but any equality holds → `"SEGMENT"`. Otherwise → `"IMPOSSIBLE"`. Time complexity is O(1) since sorting a fixed 4-element array and checking a constant number of triples; space is O(1).
