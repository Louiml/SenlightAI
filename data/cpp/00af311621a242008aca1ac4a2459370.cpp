// Write a standalone C++ function named `computeSymmetricDifferenceArea` that accepts two custom polygon set types represented as `std::deque<std::list<CPoint>>` (where `CPoint` is a simple struct with `int x; int y;`), and returns the total area of the symmetric difference (`A XOR B`) of the two polygon sets as an `int`. The function must handle arbitrary polygon sets that may contain overlapping polygons, self-intersecting shapes, or holes (the Boost.Polygon library normalizes these into valid polygons). You must implement the function generically using the Boost.Polygon library, but you are allowed to define the `CPoint`, `CPolygon`, and `CPolygonSet` types and their necessary traits mappings inside your solution file, just like the snippet does. The input polygon sets are assumed to be valid (already normalized), and the output area must be computed exactly using integer arithmetic. The function signature is: `int computeSymmetricDifferenceArea(const CPolygonSet& A, const CPolygonSet& B);`.

// The core algorithm leverages the Boost.Polygon library, which provides robust Boolean operations on polygon sets. The symmetric difference (`A XOR B`) can be computed as `(A + B) - (A * B)`, where `+` is union, `*` is intersection, and `-` is difference. However, Boost.Polygon also directly supports the `^` operator for symmetric difference. We can compute `A ^ B` directly using the library, then compute its area using `gtl::area()`. Since `CPolygonSet` is a `std::deque` of `std::list<CPoint>`, we need to ensure the polygon set traits are defined so that Boost.Polygon can treat it as a polygon set. The solution defines the necessary trait specializations for `CPoint`, `CPolygon`, and `CPolygonSet` exactly as in the snippet, but we must be careful to include the required headers and use the `gtl::polygon_set_data<int>` for intermediate computations to handle normalization. For the function itself, we first convert each input `CPolygonSet` into a `gtl::polygon_set_data<int>` (or use the `^` operator directly on `CPolygonSet` if traits are fully defined, but the snippet shows that `CPolygonSet` can be used directly with operators after defining traits). We compute `result = A ^ B` using the library operators, then compute `area = gtl::area(result)`. Edge cases: empty sets, sets with polygons that overlap, holes, and degenerate (zero-area) polygons. The area is guaranteed to be non-negative for symmetric difference. Time complexity is dominated by the polygon Boolean operation, which is roughly `O((N+M) log (N+M))` for `N` and `M` vertices across the two sets, with a constant factor from the scanline algorithm. Space complexity is `O(N+M)` for storing intermediate polygon sets.

#include <boost/polygon/polygon.hpp>
#include <list>
#include <deque>
#include <cstddef>

namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

// Custom point type
struct CPoint {
    int x;
    int y;
};

namespace boost { namespace polygon {
    template <>
    struct geometry_concept<CPoint> { typedef point_concept type; };
    template <>
    struct point_traits<CPoint> {
        typedef int coordinate_type;
        static inline coordinate_type get(const CPoint& point, orientation_2d orient) {
            if (orient == HORIZONTAL) return point.x;
            return point.y;
        }
    };
    template <>
    struct point_mutable_traits<CPoint> {
        typedef int coordinate_type;
        static inline void set(CPoint& point, orientation_2d orient, int value) {
            if (orient == HORIZONTAL) point.x = value;
            else point.y = value;
        }
        static inline CPoint construct(int x_value, int y_value) {
            CPoint retval;
            retval.x = x_value;
            retval.y = y_value;
            return retval;
        }
    };
} }

// Custom polygon type as list of CPoint
typedef std::list<CPoint> CPolygon;

namespace boost { namespace polygon {
    template <>
    struct geometry_concept<CPolygon> { typedef polygon_concept type; };
    template <>
    struct polygon_traits<CPolygon> {
        typedef int coordinate_type;
        typedef CPolygon::const_iterator iterator_type;
        typedef CPoint point_type;
        static inline iterator_type begin_points(const CPolygon& t) { return t.begin(); }
        static inline iterator_type end_points(const CPolygon& t) { return t.end(); }
        static inline std::size_t size(const CPolygon& t) { return t.size(); }
        static inline winding_direction winding(const CPolygon& t) { return unknown_winding; }
    };
    template <>
    struct polygon_mutable_traits<CPolygon> {
        template <typename iT>
        static inline CPolygon& set_points(CPolygon& t, iT input_begin, iT input_end) {
            t.clear();
            while (input_begin != input_end) {
                t.push_back(CPoint());
                gtl::assign(t.back(), *input_begin);
                ++input_begin;
            }
            return t;
        }
    };
} }

// Custom polygon set type as deque of CPolygon
typedef std::deque<CPolygon> CPolygonSet;

namespace boost { namespace polygon {
    template <>
    struct geometry_concept<CPolygonSet> { typedef polygon_set_concept type; };
    template <>
    struct polygon_set_traits<CPolygonSet> {
        typedef int coordinate_type;
        typedef CPolygonSet::const_iterator iterator_type;
        typedef CPolygonSet operator_arg_type;
        static inline iterator_type begin(const CPolygonSet& polygon_set) { return polygon_set.begin(); }
        static inline iterator_type end(const CPolygonSet& polygon_set) { return polygon_set.end(); }
        static inline bool clean(const CPolygonSet& polygon_set) { return false; }
        static inline bool sorted(const CPolygonSet& polygon_set) { return false; }
    };
    template <>
    struct polygon_set_mutable_traits<CPolygonSet> {
        template <typename input_iterator_type>
        static inline void set(CPolygonSet& polygon_set, input_iterator_type input_begin, input_iterator_type input_end) {
            polygon_set.clear();
            gtl::polygon_set_data<int> ps;
            ps.insert(input_begin, input_end);
            ps.get(polygon_set);
        }
    };
} }

// Computes the area of the symmetric difference of two polygon sets.
int computeSymmetricDifferenceArea(const CPolygonSet& A, const CPolygonSet& B) {
    CPolygonSet result;
    assign(result, A ^ B);  // symmetric difference via library operator
    return gtl::area(result);
}

#include <cassert>

int main() {
    // Test 1: Two overlapping rectangles, symmetric difference area = (10*10 + 10*10 - 2*5*5) = 150
    {
        CPolygonSet A, B;
        CPolygon p1, p2;
        p1.push_back(CPoint{0,0}); p1.push_back(CPoint{10,0}); p1.push_back(CPoint{10,10}); p1.push_back(CPoint{0,10});
        p2.push_back(CPoint{5,5}); p2.push_back(CPoint{15,5}); p2.push_back(CPoint{15,15}); p2.push_back(CPoint{5,15});
        A.push_back(p1);
        B.push_back(p2);
        assert(computeSymmetricDifferenceArea(A, B) == 150);
    }

    // Test 2: Disjoint rectangles, symmetric difference area = sum of areas = 8 + 8 = 16
    {
        CPolygonSet A, B;
        CPolygon p1, p2;
        p1.push_back(CPoint{0,0}); p1.push_back(CPoint{2,0}); p1.push_back(CPoint{2,4}); p1.push_back(CPoint{0,4});
        p2.push_back(CPoint{5,5}); p2.push_back(CPoint{7,5}); p2.push_back(CPoint{7,9}); p2.push_back(CPoint{5,9});
        A.push_back(p1);
        B.push_back(p2);
        assert(computeSymmetricDifferenceArea(A, B) == 16);
    }

    // Test 3: Identical rectangles, symmetric difference area = 0
    {
        CPolygonSet A, B;
        CPolygon p;
        p.push_back(CPoint{0,0}); p.push_back(CPoint{4,0}); p.push_back(CPoint{4,4}); p.push_back(CPoint{0,4});
        A.push_back(p);
        B.push_back(p);
        assert(computeSymmetricDifferenceArea(A, B) == 0);
    }

    // Test 4: One empty set, other has area 9
    {
        CPolygonSet A, B;
        CPolygon p;
        p.push_back(CPoint{0,0}); p.push_back(CPoint{3,0}); p.push_back(CPoint{3,3}); p.push_back(CPoint{0,3});
        A.push_back(p);
        assert(computeSymmetricDifferenceArea(A, B) == 9);
    }

    // Test 5: Square minus hole inside another square (A contains a hole, B fills it)
    {
        CPolygonSet A, B;
        // A: outer square 0,0 to 10,10 with a hole (inner square 3,3 to 7,7)
        CPolygon outer, inner;
        outer.push_back(CPoint{0,0}); outer.push_back(CPoint{10,0}); outer.push_back(CPoint{10,10}); outer.push_back(CPoint{0,10});
        inner.push_back(CPoint{3,3}); inner.push_back(CPoint{7,3}); inner.push_back(CPoint{7,7}); inner.push_back(CPoint{3,7});
        // To represent a hole, we need to add polygon with opposite orientation, but Boost handles it via set operations.
        // Instead we construct A as a polygon set that includes outer and inner in difference order; simpler: create A as outer only and B as inner.
        A.push_back(outer);
        B.push_back(inner);
        // A XOR B = (outer - inner) union (inner - outer) = area of outer minus inner (since inner is subset) = 100 - 16 = 84
        assert(computeSymmetricDifferenceArea(A, B) == 84);
    }

    // Test 6: Overlapping triangles (simulate with polygons)
    {
        CPolygonSet A, B;
        CPolygon t1, t2;
        t1.push_back(CPoint{0,0}); t1.push_back(CPoint{4,0}); t1.push_back(CPoint{2,4});
        t2.push_back(CPoint{2,0}); t2.push_back(CPoint{6,0}); t2.push_back(CPoint{2,4});
        A.push_back(t1);
        B.push_back(t2);
        // Areas: t1 = 8, t2 = 8, intersection triangle area = 4 (vertices (2,0),(4,0),(2,4))
        // XOR area = 8+8-2*4 = 8
        assert(computeSymmetricDifferenceArea(A, B) == 8);
    }

    // Test 7: A contains B entirely, XOR area = area(A) - area(B)
    {
        CPolygonSet A, B;
        CPolygon big, small;
        big.push_back(CPoint{0,0}); big.push_back(CPoint{10,0}); big.push_back(CPoint{10,10}); big.push_back(CPoint{0,10});
        small.push_back(CPoint{2,2}); small.push_back(CPoint{4,2}); small.push_back(CPoint{4,4}); small.push_back(CPoint{2,4});
        A.push_back(big);
        B.push_back(small);
        assert(computeSymmetricDifferenceArea(A, B) == 100 - 4); // 96
    }

    // Test 8: Multiple polygons in one set
    {
        CPolygonSet A, B;
        CPolygon r1, r2;
        r1.push_back(CPoint{0,0}); r1.push_back(CPoint{2,0}); r1.push_back(CPoint{2,2}); r1.push_back(CPoint{0,2});
        r2.push_back(CPoint{3,3}); r2.push_back(CPoint{5,3}); r2.push_back(CPoint{5,5}); r2.push_back(CPoint{3,5});
        A.push_back(r1);
        A.push_back(r2);
        // B empty
        assert(computeSymmetricDifferenceArea(A, B) == 4 + 4); // 8
    }

    return 0;
}
