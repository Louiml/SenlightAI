/*
Write a C++ function named `fuzzyLocation` that, given a `const geos::geom::Geometry&` and a positive double tolerance value, returns a `geos::geom::Location::Value` indicating whether a user-supplied 2D point (provided as the second parameter, a `geos::geom::Coordinate`) is considered to be on the boundary of the geometry (within tolerance), or clearly inside/outside the geometry (using exact point-location logic). The function must first extract all boundary linework only from polygonal components (dimension 2) of the geometry, and then compute the minimum distance from the query point to that linework. If that distance is less than the given tolerance, return `Location::BOUNDARY`. Otherwise, use an internal `geos::algorithm::PointLocator` to locate the point relative to the original geometry and return the corresponding `Location::Value`. Your implementation must handle empty geometries, geometries with no polygonal parts, and must not leak memory, using RAII (e.g., `std::unique_ptr` or `std::auto_ptr`). The function should be free of any global state and be const-correct with respect to the input geometry.
*/
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Location.h>
#include <geos/algorithm/PointLocator.h>

#include <memory>
#include <vector>

namespace geos {
namespace operation {
namespace overlay {
namespace validate {

// Returns the fuzzy location of a point relative to the geometry's polygonal boundaries.
geos::geom::Location::Value fuzzyLocation(const geos::geom::Geometry& geom,
                                          const geos::geom::Coordinate& pt,
                                          double tolerance) {
    // Extract boundary linework from all polygonal components.
    std::vector<geos::geom::Geometry*> lineGeoms;
    try {
        for (size_t i = 0; i < geom.getNumGeometries(); ++i) {
            const geos::geom::Geometry* comp = geom.getGeometryN(i);
            if (comp->getDimension() == 2) {
                geos::geom::Geometry* boundary = comp->getBoundary();
                lineGeoms.push_back(boundary);
            }
        }

        // Build a single geometry from the boundaries. If none, create an empty collection.
        std::unique_ptr<geos::geom::Geometry> linework(
            geom.getFactory()->buildGeometry(lineGeoms));

        // Create a point geometry for distance computation.
        std::unique_ptr<geos::geom::Geometry> pointGeom(
            geom.getFactory()->createPoint(pt));

        // Compute distance to boundary linework.
        double dist = linework->distance(pointGeom.get());

        // If within tolerance, consider it on the boundary.
        if (dist < tolerance) {
            return geos::geom::Location::BOUNDARY;
        }

        // Otherwise, exact point location using the original geometry.
        geos::algorithm::PointLocator locator;
        return static_cast<geos::geom::Location::Value>(
            locator.locate(pt, &geom));
    } catch (...) {
        // Clean up allocated boundary geometries on any exception.
        for (geos::geom::Geometry* g : lineGeoms) {
            delete g;
        }
        throw;
    }
}

} // namespace validate
} // namespace overlay
} // namespace operation
} // namespace geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Location.h>
#include <cassert>
#include <memory>

// Include the solution header (adjust path as needed)
#include "fuzzy_location.h"

int main() {
    using namespace geos::geom;

    // Create a square polygon from (0,0) to (10,10).
    GeometryFactory::Ptr factory = GeometryFactory::create();
    CoordinateSequence* coords = factory->getCoordinateSequenceFactory()->create();
    coords->add(Coordinate(0,0));
    coords->add(Coordinate(10,0));
    coords->add(Coordinate(10,10));
    coords->add(Coordinate(0,10));
    coords->add(Coordinate(0,0)); // close ring
    LinearRing* ring = factory->createLinearRing(coords);
    Polygon* poly = factory->createPolygon(ring, nullptr);

    // Use the function via a simple wrapper (here directly call the namespace function)
    // Since the function is in a namespace, alias or refer to it.
    auto fuzzyLoc = [&](const Coordinate& p, double tol) {
        return geos::operation::overlay::validate::fuzzyLocation(*poly, p, tol);
    };

    // Inside point, far from boundary
    assert(fuzzyLoc(Coordinate(5,5), 0.1) == Location::INTERIOR);

    // Outside point, far from boundary
    assert(fuzzyLoc(Coordinate(20,20), 0.1) == Location::EXTERIOR);

    // Point on exact boundary -> distance 0 < tolerance
    assert(fuzzyLoc(Coordinate(0,5), 0.1) == Location::BOUNDARY);

    // Point just inside boundary but within tolerance
    assert(fuzzyLoc(Coordinate(0.05,5), 0.1) == Location::BOUNDARY);

    // Point just outside boundary but within tolerance
    assert(fuzzyLoc(Coordinate(-0.05,5), 0.1) == Location::BOUNDARY);

    // Point on corner, with tolerance
    assert(fuzzyLoc(Coordinate(0,0), 0.5) == Location::BOUNDARY);

    // Point far above, large tolerance
    assert(fuzzyLoc(Coordinate(5,12), 1.0) == Location::BOUNDARY);
    assert(fuzzyLoc(Coordinate(5,12), 0.5) == Location::EXTERIOR);

    // Clean up
    delete poly; // polygon owns ring and coords

    return 0;
}
// The solution requires building a temporary geometry that contains only the boundary lines of all polygonal components (e.g., polygons, multi-polygons) of the input geometry. For each sub-geometry, if its dimension is 2, extract its boundary using `getBoundary()`, which returns a linear geometry (e.g., `LineString` or `MultiLineString`). Collect these boundary geometries into a vector, then combine them into a single geometry using the geometry factory's `buildGeometry` method. For non-polygonal components (points, lines), they are ignored for boundary distance calculation because the fuzzy point locator specifically considers proximity to the polygonal boundaries only. After constructing the linework, create a point geometry from the input coordinate via the factory, compute the distance from that point to the linework using `Geometry::distance()`, and compare it with the tolerance (strictly less than). If the distance is below tolerance, return `Location::BOUNDARY`. Otherwise, use a `PointLocator` object to determine the exact location of the point relative to the original geometry (inside/outside). The function must handle edge cases: an empty geometry or a geometry with no polygonal parts should produce a linework that, when queried with `distance()`, returns a large value (or possibly NaN) — but we must ensure that the distance call is safe; `buildGeometry` on an empty vector may return a null pointer or an empty GeometryCollection, and `distance` on such a geometry with a point will likely return `std::numeric_limits<double>::infinity()` or some large default, which will not be less than tolerance, so the function falls back to the point locator. Also, the tolerance must be positive; if zero or negative, the function should treat it as zero or handle gracefully but the specification assumes positive. Time complexity is O(N) for extracting boundaries and O(P) for distance computation, where P is the number of vertices in the linework, and O(log P) for point-location if the geometry is indexed, but without indexing it is O(V) where V is the total number of vertices. Space complexity is O(V) for the temporary linework geometry.
