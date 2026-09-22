/*
Write a C++ function named `isValidPolygonWkt` that takes a single `std::string` argument containing a Well-Known Text (WKT) representation of a polygon. The function must return `true` if the polygon is valid according to the OGC validity rules (no self-intersections, rings are closed, at least 4 points per ring, correct orientation of exterior and interior rings), and `false` otherwise. The input may be empty or malformed (e.g., missing parentheses, invalid coordinates). You may not use external libraries—implement all geometry parsing and validation yourself. The function should be case-insensitive for the WKT keywords (`POLYGON`, `EMPTY`, etc.). Assume coordinates are decimal numbers (including negatives and scientific notation). Empty polygons (`POLYGON EMPTY`) are considered valid. The function must handle nested parentheses for interior rings correctly.
*/

#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <sstream>
#include <algorithm>

// A simple 2D point.
struct Point {
    double x, y;
    bool operator==(const Point& other) const {
        return std::fabs(x - other.x) < 1e-9 && std::fabs(y - other.y) < 1e-9;
    }
};

// Helper: trim whitespace.
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Helper: check if two segments intersect (including touching endpoints).
static bool segmentsIntersect(const Point& a, const Point& b, const Point& c, const Point& d) {
    auto cross = [](const Point& o, const Point& p, const Point& q) -> double {
        return (p.x - o.x) * (q.y - o.y) - (p.y - o.y) * (q.x - o.x);
    };
    auto onSegment = [](const Point& p, const Point& q, const Point& r) -> bool {
        return std::min(p.x, r.x) <= q.x && q.x <= std::max(p.x, r.x) &&
               std::min(p.y, r.y) <= q.y && q.y <= std::max(p.y, r.y) &&
               std::fabs(cross(p, q, r)) < 1e-9;
    };

    double d1 = cross(c, d, a);
    double d2 = cross(c, d, b);
    double d3 = cross(a, b, c);
    double d4 = cross(a, b, d);

    if (((d1 > 1e-9 && d2 < -1e-9) || (d1 < -1e-9 && d2 > 1e-9)) &&
        ((d3 > 1e-9 && d4 < -1e-9) || (d3 < -1e-9 && d4 > 1e-9)))
        return true;

    if (std::fabs(d1) < 1e-9 && onSegment(c, a, d)) return true;
    if (std::fabs(d2) < 1e-9 && onSegment(c, b, d)) return true;
    if (std::fabs(d3) < 1e-9 && onSegment(a, c, b)) return true;
    if (std::fabs(d4) < 1e-9 && onSegment(a, d, b)) return true;
    return false;
}

// Helper: compute signed area (positive if counterclockwise).
static double signedArea(const std::vector<Point>& ring) {
    double area = 0.0;
    for (size_t i = 0; i + 1 < ring.size(); ++i) {
        area += ring[i].x * ring[i+1].y - ring[i+1].x * ring[i].y;
    }
    return area / 2.0;
}

// Check if a point is inside a polygon (ray casting).
static bool pointInRing(const Point& p, const std::vector<Point>& ring) {
    bool inside = false;
    for (size_t i = 0, j = ring.size() - 1; i < ring.size(); j = i++) {
        if (((ring[i].y > p.y) != (ring[j].y > p.y)) &&
            (p.x < (ring[j].x - ring[i].x) * (p.y - ring[i].y) / (ring[j].y - ring[i].y) + ring[i].x))
            inside = !inside;
    }
    return inside;
}

// Validate a single ring: closed, at least 4 points, no self-intersections.
static bool validRing(const std::vector<Point>& ring, bool isExterior) {
    if (ring.size() < 4) return false;
    if (!(ring.front() == ring.back())) return false;
    // Remove duplicate consecutive points (except closure).
    for (size_t i = 1; i + 1 < ring.size(); ++i) {
        if (ring[i] == ring[i-1]) return false;
    }
    // Check for self-intersections among non-adjacent segments.
    for (size_t i = 0; i + 1 < ring.size(); ++i) {
        for (size_t j = i + 1; j + 1 < ring.size(); ++j) {
            if (i == j || i + 1 == j || (i == 0 && j + 1 == ring.size() - 1)) continue;
            if (segmentsIntersect(ring[i], ring[i+1], ring[j], ring[j+1]))
                return false;
        }
    }
    // Orientation check.
    double area = signedArea(ring);
    if (isExterior && area < -1e-9) return false; // exterior must be CCW (positive)
    if (!isExterior && area > 1e-9) return false; // interior must be CW (negative)
    return true;
}

// Main function as required.
bool isValidPolygonWkt(const std::string& input) {
    std::string s = trim(input);
    if (s.empty()) return false;

    // Convert to uppercase for keyword check.
    std::string upper;
    upper.reserve(s.size());
    for (char c : s) upper.push_back(std::toupper(static_cast<unsigned char>(c)));

    // Must start with POLYGON.
    if (upper.rfind("POLYGON", 0) != 0) return false;
    s = trim(s.substr(7)); // remove "POLYGON"
    upper = trim(upper.substr(7));

    // Handle EMPTY.
    if (upper == "EMPTY") return true;

    // Must start with '('.
    if (s.empty() || s[0] != '(') return false;
    s = s.substr(1); // remove '('
    s = trim(s);

    // Parse rings.
    std::vector<std::vector<Point>> rings;
    std::string token;
    size_t pos = 0;
    while (pos < s.size()) {
        // Expect '(' for ring.
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == ',' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) pos++;
        if (pos >= s.size()) break; // trailing spaces
        if (s[pos] != '(') return false;
        pos++; // consume '('
        std::vector<Point> ring;
        std::string coordToken;
        while (pos < s.size() && s[pos] != ')') {
            // Extract coordinate pair.
            // Skip whitespace and commas.
            while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == ',')) pos++;
            if (pos >= s.size() || s[pos] == ')') break;
            // Read first double.
            size_t startPos = pos;
            while (pos < s.size() && s[pos] != ' ' && s[pos] != ',' && s[pos] != ')') pos++;
            std::string xStr = s.substr(startPos, pos - startPos);
            if (xStr.empty()) return false;
            // Skip spaces.
            while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) pos++;
            if (pos >= s.size() || s[pos] == ')' || s[pos] == ',') return false; // need y
            startPos = pos;
            while (pos < s.size() && s[pos] != ' ' && s[pos] != ',' && s[pos] != ')') pos++;
            std::string yStr = s.substr(startPos, pos - startPos);
            if (yStr.empty()) return false;
            try {
                size_t idx1, idx2;
                double x = std::stod(xStr, &idx1);
                double y = std::stod(yStr, &idx2);
                if (idx1 != xStr.size() || idx2 != yStr.size()) return false;
                ring.push_back({x, y});
            } catch (...) {
                return false;
            }
            // Position now at whitespace, comma, or ')'. Loop will handle.
        }
        if (pos >= s.size() || s[pos] != ')') return false;
        pos++; // consume ')'
        // After ring, expect either ',' (more rings) or ')' (end of polygon).
        // Skip whitespace.
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) pos++;
        if (pos < s.size() && s[pos] == ',') {
            pos++; // skip comma
            continue;
        }
        // Otherwise, we must be at the end or at a final ')'
        break;
    }

    // After all rings, expect a closing ')' and nothing else.
    // Skip whitespace.
    while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n' || s[pos] == '\r')) pos++;
    if (pos >= s.size() || s[pos] != ')') return false;
    pos++;
    // Trim remaining; should be empty.
    std::string rest = trim(s.substr(pos));
    if (!rest.empty()) return false;

    if (rings.empty()) return false; // must have at least one ring

    // Validate each ring.
    for (size_t i = 0; i < rings.size(); ++i) {
        if (!validRing(rings[i], i == 0)) return false;
    }

    // Check interior rings are inside exterior and do not intersect each other.
    if (rings.size() > 1) {
        const auto& exterior = rings[0];
        for (size_t i = 1; i < rings.size(); ++i) {
            // Check every point of interior ring is inside exterior (or on boundary? For validity, must be strictly inside).
            for (const auto& p : rings[i]) {
                if (!pointInRing(p, exterior)) return false;
            }
            // Check no intersections with exterior ring.
            for (size_t a = 0; a + 1 < rings[i].size(); ++a) {
                for (size_t b = 0; b + 1 < exterior.size(); ++b) {
                    if (segmentsIntersect(rings[i][a], rings[i][a+1], exterior[b], exterior[b+1]))
                        return false;
                }
            }
            // Check against other interior rings.
            for (size_t j = i + 1; j < rings.size(); ++j) {
                for (size_t a = 0; a + 1 < rings[i].size(); ++a) {
                    for (size_t b = 0; b + 1 < rings[j].size(); ++b) {
                        if (segmentsIntersect(rings[i][a], rings[i][a+1], rings[j][b], rings[j][b+1]))
                            return false;
                    }
                }
            }
        }
    }

    return true;
}

#include <cassert>
#include <string>

// Function declaration (in real test, include the solution file or header).
bool isValidPolygonWkt(const std::string&);

int main() {
    // Valid simple square (CCW exterior).
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 1, 1 1, 1 0, 0 0))") == true);

    // Valid with interior ring (CW interior, inside exterior).
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 10, 10 10, 10 0, 0 0), (2 2, 2 4, 4 4, 4 2, 2 2))") == true);

    // Empty polygon.
    assert(isValidPolygonWkt("POLYGON EMPTY") == true);

    // Invalid: not closed.
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 1, 1 1))") == false);

    // Invalid: self-intersection (bowtie).
    assert(isValidPolygonWkt("POLYGON ((0 0, 1 1, 1 0, 0 1, 0 0))") == false);

    // Invalid: interior ring outside exterior.
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 1, 1 1, 1 0, 0 0), (10 10, 10 11, 11 11, 11 10, 10 10))") == false);

    // Invalid: interior ring intersects exterior.
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 10, 10 10, 10 0, 0 0), (5 -1, 5 5, 6 5, 6 -1, 5 -1))") == false);

    // Invalid: wrong orientation (exterior clockwise).
    assert(isValidPolygonWkt("POLYGON ((0 0, 1 0, 1 1, 0 1, 0 0))") == false);

    // Invalid: malformed WKT.
    assert(isValidPolygonWkt("POLYGON ((0 0, 0 1, 1 1, 1 0))") == false);

    // Invalid: empty string.
    assert(isValidPolygonWkt("") == false);

    return 0;
}

// The approach involves writing a parser for the WKT polygon syntax. First, trim leading/trailing whitespace and convert the keyword to uppercase. If the string is empty or doesn't start with `POLYGON`, return `false`. After the keyword, expect either `EMPTY` (valid) or a `(` starting the polygon body. Parse the outer parenthesis, then parse one or more ring groups, each enclosed in parentheses and containing coordinate pairs separated by commas. For each coordinate, parse two double values separated by whitespace or comma; ensure exactly two numbers per coordinate. After parsing all coordinates for a ring, validate: at least 4 points (3 distinct + closure), first and last points must be exactly equal (within a small epsilon to handle floating-point rounding), and no self-intersections or duplicate consecutive points. For validity, each ring must be closed, and interior rings must not intersect the exterior ring or each other, and must be inside the exterior ring. Also check ring orientation (exterior counterclockwise, interiors clockwise) using the shoelace formula—although many libraries permit either orientation, for OGC strictness we enforce it. For simplicity, we can skip orientation enforcement if not specified; but the task mentions correct orientation, so we'll implement it. Edge cases: malformed WKT (missing commas, extra tokens), empty polygon, polygon with only one ring (valid), polygon with interior rings that touch the exterior at a point (invalid). Time complexity: O(P) for parsing, O(R * N^2) for intersection checks across all rings (where R is number of rings, N points per ring) in the worst case; space O(total points).
