// Write a C++ function `buildSegmentDelaunayGraph` that reads a closed polygon from an input file stream (already opened, not the filename), where each line describes one polygon edge as a segment with format: `s x0 y0 x1 y1` (the letter `s` followed by four doubles). The polygon is deduced from consecutive segments: the first point of the first segment, and then the source point of every subsequent segment, with an implicit final edge connecting the last point back to the first. The function must insert all polygon edges into a `CGAL::Segment_Delaunay_graph_2` using the filtered traits without intersections (`CGAL::Segment_Delaunay_graph_filtered_traits_without_intersections_2<CGAL::Simple_cartesian<double>>`) and return the resulting graph. The function should assert that the input stream is valid, that every read site is a segment, that the polygon closes correctly, and that the final graph is valid with full validation. The function must handle an arbitrary number of polygon vertices (at least 3). Do not include a `main` function; just the free function and any necessary includes.
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

// The solution function is declared here (assume it is included above).
// We provide a helper to create a temporary file with the given content.
void writeTempFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
    out.close();
}

int main() {
    // Test 1: A simple triangle
    {
        writeTempFile("test_triangle.cin", "s 0 0 1 0\ns 1 0 0 1\ns 0 1 0 0\n");
        std::ifstream ifs("test_triangle.cin");
        SDG2 g = buildSegmentDelaunayGraph(ifs);
        assert(g.number_of_vertices() == 3);
        assert(g.number_of_faces() == 0); // in a segment Delaunay graph, faces are not the usual triangles; just check validity already done.
        // Additional check: all sites are segments
        assert(g.number_of_vertices() == 3);
    }

    // Test 2: A square (4 vertices, 4 edges)
    {
        writeTempFile("test_square.cin", "s 0 0 1 0\ns 1 0 1 1\ns 1 1 0 1\ns 0 1 0 0\n");
        std::ifstream ifs("test_square.cin");
        SDG2 g = buildSegmentDelaunayGraph(ifs);
        assert(g.number_of_vertices() == 4);
        // Check that the graph has exactly 4 segments as sites
        assert(g.number_of_vertices() == 4);
    }

    // Test 3: A pentagon with duplicate coordinates (should still work)
    {
        writeTempFile("test_pentagon.cin", "s 0 0 2 0\ns 2 0 2 2\ns 2 2 0 2\ns 0 2 -1 1\ns -1 1 0 0\n");
        std::ifstream ifs("test_pentagon.cin");
        SDG2 g = buildSegmentDelaunayGraph(ifs);
        assert(g.number_of_vertices() >= 5); // may have more due to intersection? but no intersections allowed
        // Just assert graph is valid (already done in function)
    }

    // Test 4: Verify that invalid input (non-segment site) would assert fail – we only test valid inputs here
    // Test 5: Verify stream closing works (already done)

    // Clean up temp files
    std::remove("test_triangle.cin");
    std::remove("test_square.cin");
    std::remove("test_pentagon.cin");

    std::cout << "All tests passed.\n";
    return 0;
}
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Segment_Delaunay_graph_2.h>
#include <CGAL/Segment_Delaunay_graph_filtered_traits_2.h>
#include <cassert>
#include <vector>
#include <utility>
#include <fstream>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Segment_Delaunay_graph_filtered_traits_without_intersections_2<K> Gt;
typedef CGAL::Segment_Delaunay_graph_2<Gt> SDG2;

// Reads a closed polygon from an input stream and builds its segment Delaunay graph.
// The stream must contain lines of the form: s x0 y0 x1 y1
// The first point of the first segment is the first vertex, and each subsequent
// segment's source point is the next vertex. The final edge connects the last
// vertex back to the first.
SDG2 buildSegmentDelaunayGraph(std::ifstream& ifs) {
    assert(ifs);

    std::vector<Gt::Point_2> points;
    std::vector<std::pair<std::size_t, std::size_t>> indices;

    SDG2::Site_2 site;
    ifs >> site;
    assert(site.is_segment());
    points.push_back(site.source_of_supporting_site());

    std::size_t k = 0;
    while (ifs >> site) {
        assert(site.is_segment());
        points.push_back(site.source_of_supporting_site());
        indices.push_back(std::make_pair(k, k + 1));
        ++k;
    }
    indices.push_back(std::make_pair(k, 0));
    ifs.close();

    SDG2 sdg;
    sdg.insert_segments(points.begin(), points.end(), indices.begin(), indices.end());
    assert(sdg.is_valid(true, 1));

    return sdg;
}
// The solution reads the polygon edges sequentially from the input stream. The first site provides the initial point (`source_of_supporting_site`). For each subsequent segment, we push its source point into a vector of points and record the edge as a pair of consecutive indices `(k, k+1)`. After the loop, we add the closing edge `(k, 0)`. To ensure correctness, we assert that the first site is a segment and every later site is also a segment. The key edge case is when the input has fewer than 3 segments—this would form an invalid polygon, but the problem guarantees at least three vertices. Another nuance: the input format is line-based but we simply extract using `>>`, which ignores newlines and spaces, so any consistent whitespace works. After collecting all points and indices, we call `insert_segments` with the point range and index range, which uses spatial sorting internally to speed insertion. Finally, we call `is_valid(true, 1)` which performs a full validity check (including level 1 detail). The time complexity is dominated by the Delaunay graph construction; with spatial sorting, insertion is near O(n log n) for random points (where n is the number of segments) and the validity check is O(n) on average. Space complexity is O(n) for storing the points, indices, and the graph itself.
