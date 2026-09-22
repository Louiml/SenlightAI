// Write a C++ function `areSymbologiesUnique` that takes a `std::vector<std::string>` representing a list of symbology names (e.g., "UPCA", "EAN13", "CODE128") and a `std::string` representing a newly requested symbology name. The function should return `true` if the new symbology is **not** already present in the list (i.e., it is unique), and `false` if it already exists. The comparison must be case-sensitive. The function should handle an empty list correctly (returning `true` for any new symbology). No other processing or side effects are needed; this is a pure helper extracted from the `addSymbology` logic in the provided snippet.
// The solution directly mirrors the "check if decoder for this symbology already exists" loop from the original `_BLaDE::addSymbology(BarcodeSymbology*)` method. We iterate through the vector of existing symbology names, comparing each element to the new symbology string using `==`. If any match is found, we return `false` immediately (duplicate detected). If the loop completes without a match, we return `true` (unique). Edge cases: an empty vector immediately returns `true`; case-sensitive comparison means "upca" and "UPCA" are treated as different; a single-element vector with the same name returns `false`. Time complexity is O(n) where n is the number of existing symbologies, and space complexity is O(1) beyond the input.
#include <string>
#include <vector>

// Returns true if the given symbologyName is not already present in the list of existingSymbologies.
// The comparison is case-sensitive.
bool areSymbologiesUnique(const std::vector<std::string>& existingSymbologies,
                          const std::string& symbologyName) {
    for (const auto& existing : existingSymbologies) {
        if (existing == symbologyName) {
            return false;  // Duplicate found.
        }
    }
    return true;  // No duplicate or empty list.
}
#include <cassert>
#include <string>
#include <vector>

// Function under test (declared above for completeness in a single file).
bool areSymbologiesUnique(const std::vector<std::string>& existingSymbologies,
                          const std::string& symbologyName);

int main() {
    // Empty list — any name is unique.
    assert(areSymbologiesUnique({}, "UPCA") == true);

    // Single element, no match.
    assert(areSymbologiesUnique({"EAN13"}, "UPCA") == true);

    // Single element, exact match.
    assert(areSymbologiesUnique({"UPCA"}, "UPCA") == false);

    // Multiple elements, no duplicate.
    assert(areSymbologiesUnique({"EAN13", "CODE128", "QR"}, "UPCA") == true);

    // Multiple elements, duplicate in the middle.
    assert(areSymbologiesUnique({"EAN13", "CODE128", "QR"}, "CODE128") == false);

    // Duplicate at the end.
    assert(areSymbologiesUnique({"EAN13", "QR", "UPCA"}, "UPCA") == false);

    // Case sensitivity: lowercase is distinct from uppercase.
    assert(areSymbologiesUnique({"UPCA"}, "upca") == true);

    // Empty string as a valid symbology name.
    assert(areSymbologiesUnique({"", "UPCA"}, "") == false);
    assert(areSymbologiesUnique({"UPCA"}, "") == true);

    return 0;
}
