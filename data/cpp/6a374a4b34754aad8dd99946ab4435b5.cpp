/*
Write a C++ function `countFaces(const std::vector<std::string>& shapes)` that takes a vector of shape names (each equal to one of: "Tetrahedron", "Cube", "Octahedron", "Dodecahedron", "Icosahedron") and returns the total number of faces as an `int`. Each shape has a fixed number of faces: Tetrahedron=4, Cube=6, Octahedron=8, Dodecahedron=12, Icosahedron=20. The input may contain duplicate names and any order; assume all names are valid and non-empty. The function should be efficient in time and use constant extra space.
*/

#include <string>
#include <vector>

// Compute total number of faces for a list of polyhedron shape names.
int countFaces(const std::vector<std::string>& shapes) {
    int total_faces = 0;
    for (const std::string& shape : shapes) {
        if (shape == "Tetrahedron") {
            total_faces += 4;
        } else if (shape == "Cube") {
            total_faces += 6;
        } else if (shape == "Octahedron") {
            total_faces += 8;
        } else if (shape == "Dodecahedron") {
            total_faces += 12;
        } else if (shape == "Icosahedron") {
            total_faces += 20;
        }
    }
    return total_faces;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Mixed order and duplicates
    assert(countFaces({"Tetrahedron", "Cube", "Octahedron", "Dodecahedron", "Icosahedron"}) == 4 + 6 + 8 + 12 + 20);
    // Single shape
    assert(countFaces({"Cube"}) == 6);
    // Empty input
    assert(countFaces({}) == 0);
    // Repeated same shape
    assert(countFaces({"Tetrahedron", "Tetrahedron", "Tetrahedron"}) == 12);
    // Only one occurrence of each
    assert(countFaces({"Icosahedron", "Dodecahedron", "Octahedron"}) == 20 + 12 + 8);
    // Larger random mix
    std::vector<std::string> big = {"Cube", "Tetrahedron", "Icosahedron", "Tetrahedron", "Cube"};
    assert(countFaces(big) == 6 + 4 + 20 + 4 + 6);
    return 0;
}

// The solution simply iterates over each string in the vector, and for each one, adds the corresponding face count to a running total. Since the mapping is fixed, we can use a series of `if`/`else if` comparisons or a lookup structure (like `std::map` or `std::unordered_map`) for clarity. For only 5 possible names, direct comparisons are simpler and avoid overhead. Edge cases: an empty vector returns 0; duplicate names are handled naturally by summing each occurrence. Time complexity is \(O(n)\) where \(n\) is the number of shapes, because each string is examined once. Space complexity is \(O(1)\) auxiliary (ignoring the input vector itself), as we only maintain an integer accumulator and a temporary string reference.
