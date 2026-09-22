Write a standalone C++ function `color extvol_shade(const extvol& xvol, const Vector& ray_origin, const Vector& ray_dir, int samples)` that computes the accumulated color contribution from a volume density field contained within an axis-aligned box, given a ray defined by an origin and direction. The volume has a scalar field evaluator function `flt (*evaluator)(flt, flt, flt)` that returns a density value in `[0,1]` at normalized coordinates `(u,v,w)` inside the box (where `u,v,w` range from `0` at the box min to `1` at the box max). The output color is produced by stepping along the ray through the box, accumulating density-weighted color using a fixed colormap: red = density, green = max(0, (density – 0.5)*2), blue = 1 – density/2. The accumulation terminates early when the opacity sum reaches or exceeds 1.0; otherwise, if the ray exits the box before the sum reaches 1.0, the contribution from the outside is considered zero (i.e., we ignore any transmitted light). The function must handle rays that miss the box entirely (returning black) and rays that start inside or outside the box. Use constant step increments: exactly `samples` steps along the ray segment inside the box, with step size `dt = 1.0 / samples`. The total opacity accumulation at each step is `(xvol.opacity / samples) * scalar` where `xvol.opacity` is a constant in `[0,1]`. The final color is the sum over all steps of `(transval * col2 * xvol.ambient)` where `transval` is that per-step opacity and `col2` is the colormap value. Ignore any diffuse lighting or transmission calculations. Provide a self-contained implementation with a `struct extvol` that holds `min`, `max`, `evaluator`, `opacity`, `ambient`, and `samples` (use `double` for floating-point values). Also define a `Vector` struct with `x,y,z` doubles and standard arithmetic operators. The solution must be compilable standalone without external libraries beyond `<cmath>` and `<algorithm>`.

// The algorithm first determines the intersection interval `[tnear, tfar]` of the ray with the axis-aligned bounding box using the slab method. For each axis, if the ray direction component is zero, the ray must lie between the box bounds on that axis or it misses entirely. Otherwise, compute the two intersection parameters, swap so the smaller is first, and update `tnear` and `tfar` with the maximum and minimum respectively. If `tnear > tfar` or `tfar < 0.0`, return black. Clamp `tnear` to 0 if it is negative (ray starts inside the box). Set the total distance to integrate as `tfar – tnear`, and decide the step count `samples` from the `xvol.samples` field (if it is 0 or negative, treat as 1 to avoid division by zero). Compute `dt = (tfar – tnear) / samples` (note: the original snippet uses `dt = 1.0/tdist` which is only valid if the box length is normalized to 1; here we correctly use the actual ray segment length). Loop `i` from 0 to `samples-1`, compute `t = tnear + i*dt + 0.5*dt` (sample at the midpoint of each step for better accuracy). For each sample, compute the normalized coordinates `pnt = (ray_origin + t*ray_dir – min) / (max – min)` component-wise. If any coordinate is outside [0,1] due to numerical precision, clamp to [0,1]. Call the evaluator to get `scalar`, clamp it to [0,1]. Compute `transval = (opacity / samples) * scalar`. Add `transval` to `sum`. Compute the colormap color `col2` from `scalar`. Accumulate `col += transval * col2 * ambient`. If `sum >= 1.0`, break early (clamp sum to 1.0). Return the accumulated color. Edge cases: ray parallel to a slab axis and outside bounds; ray origin inside the box (tnear clamped to 0); zero-length ray direction (treat as miss if any component is zero and the ray is outside the corresponding slab, but if all components are zero and the origin is inside the box, return black after one sample at t=0); negative `samples` or `opacity` (clamp to 0 or 1 respectively); and non-finite values (assume valid input). Time complexity is O(samples) per ray, space is O(1) auxiliary.

#include <cmath>
#include <algorithm>

struct Vector {
    double x, y, z;
    Vector operator+(const Vector& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vector operator-(const Vector& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vector operator*(double s) const { return {x*s, y*s, z*s}; }
    Vector operator/(double s) const { return {x/s, y/s, z/s}; }
};

struct Color {
    double r, g, b;
    Color operator*(double s) const { return {r*s, g*s, b*s}; }
    Color operator+(const Color& o) const { return {r+o.r, g+o.g, b+o.b}; }
};

using scalar_func = double (*)(double, double, double);

struct Extvol {
    Vector min, max;
    scalar_func evaluator;
    double opacity;
    double ambient;
    int samples;
};

// Map a scalar density in [0,1] to a color.
Color extvol_colormap(double scalar) {
    if (scalar < 0.0) scalar = 0.0;
    if (scalar > 1.0) scalar = 1.0;
    double green = (scalar < 0.5) ? 0.0 : (scalar - 0.5) * 2.0;
    return {scalar, green, 1.0 - scalar / 2.0};
}

// Accumulate color from a ray passing through an extvol box.
Color extvol_shade(const Extvol& xvol, const Vector& origin, const Vector& dir) {
    Color black{0.0, 0.0, 0.0};
    const Vector& lo = xvol.min;
    const Vector& hi = xvol.max;

    double tnear = -1e30;
    double tfar = 1e30;
    bool miss = false;

    // Slab method for each axis.
    if (dir.x == 0.0) {
        if (origin.x < lo.x || origin.x > hi.x) miss = true;
    } else {
        double t1 = (lo.x - origin.x) / dir.x;
        double t2 = (hi.x - origin.x) / dir.x;
        if (t1 > t2) std::swap(t1, t2);
        tnear = std::max(tnear, t1);
        tfar = std::min(tfar, t2);
        if (tnear > tfar) miss = true;
    }
    if (!miss && dir.y == 0.0) {
        if (origin.y < lo.y || origin.y > hi.y) miss = true;
    } else if (!miss) {
        double t1 = (lo.y - origin.y) / dir.y;
        double t2 = (hi.y - origin.y) / dir.y;
        if (t1 > t2) std::swap(t1, t2);
        tnear = std::max(tnear, t1);
        tfar = std::min(tfar, t2);
        if (tnear > tfar) miss = true;
    }
    if (!miss && dir.z == 0.0) {
        if (origin.z < lo.z || origin.z > hi.z) miss = true;
    } else if (!miss) {
        double t1 = (lo.z - origin.z) / dir.z;
        double t2 = (hi.z - origin.z) / dir.z;
        if (t1 > t2) std::swap(t1, t2);
        tnear = std::max(tnear, t1);
        tfar = std::min(tfar, t2);
        if (tnear > tfar) miss = true;
    }

    if (miss || tfar < 0.0) return black;

    if (tnear < 0.0) tnear = 0.0; // ray starts inside the box
    if (tnear > tfar) return black; // degenerate

    double length = tfar - tnear;
    int steps = xvol.samples;
    if (steps <= 0) steps = 1;
    double dt = length / steps;
    if (dt <= 0.0) return black;

    double sum = 0.0;
    Color accum{0.0, 0.0, 0.0};
    double opacity = std::clamp(xvol.opacity, 0.0, 1.0);
    double ambient = std::clamp(xvol.ambient, 0.0, 1.0);
    double per_step_base = opacity / steps;

    // Bounding box size for normalization.
    Vector size = hi - lo;
    if (size.x == 0.0) size.x = 1.0;
    if (size.y == 0.0) size.y = 1.0;
    if (size.z == 0.0) size.z = 1.0;

    for (int i = 0; i < steps; ++i) {
        if (sum >= 1.0) break;
        double t = tnear + (i + 0.5) * dt;
        Vector p = origin + dir * t;
        double u = (p.x - lo.x) / size.x;
        double v = (p.y - lo.y) / size.y;
        double w = (p.z - lo.z) / size.z;
        u = std::clamp(u, 0.0, 1.0);
        v = std::clamp(v, 0.0, 1.0);
        w = std::clamp(w, 0.0, 1.0);

        double scalar = xvol.evaluator(u, v, w);
        if (scalar < 0.0) scalar = 0.0;
        if (scalar > 1.0) scalar = 1.0;

        double transval = per_step_base * scalar;
        sum += transval;
        Color col2 = extvol_colormap(scalar);
        accum = accum + (col2 * (transval * ambient));
        if (sum > 1.0) sum = 1.0;
    }
    return accum;
}

#include <cassert>
#include <cmath>

// Test evaluators
double constant_zero(double, double, double) { return 0.0; }
double constant_half(double u, double v, double w) { (void)u; (void)v; (void)w; return 0.5; }
double constant_one(double, double, double) { return 1.0; }
double diagonal_ramp(double u, double v, double w) { return (u + v + w) / 3.0; }

// Helper for approximate comparison
bool close(double a, double b, double eps=1e-9) { return std::fabs(a - b) < eps; }
bool close_color(const Color& a, const Color& b, double eps=1e-9) {
    return close(a.r, b.r, eps) && close(a.g, b.g, eps) && close(a.b, b.b, eps);
}

int main() {
    Extvol vol;
    vol.min = {0.0, 0.0, 0.0};
    vol.max = {1.0, 1.0, 1.0};
    vol.opacity = 1.0;
    vol.ambient = 1.0;

    // Test miss: ray parallel to x-axis outside box
    vol.samples = 10;
    vol.evaluator = constant_one;
    Color c = extvol_shade(vol, {2.0, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.0, 0.0, 0.0}));

    // Test hit from outside with constant zero density -> black
    vol.evaluator = constant_zero;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.0, 0.0, 0.0}));

    // Test constant one density with 10 samples, opacity 1, ambient 1
    // Ray passes through entire box length 1.0, dt=0.1, transval=0.1 each step
    // sum reaches 1.0 after 10 steps, color per step = colormap(1.0) = (1,1,0.5)
    // Accumulated = 0.1 * (1,1,0.5) * 10 = (1.0, 1.0, 0.5)
    vol.samples = 10;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {1.0, 1.0, 0.5}, 1e-6));

    // Test constant 0.5 density, 2 samples, opacity 1, ambient 1
    // Each step transval = 0.5/2 = 0.25, sum=0.5 after both steps < 1
    // colormap(0.5) = (0.5, 0.0, 0.75)
    // Accum = 0.25*(0.5,0,0.75)*2 = (0.25, 0.0, 0.375)
    vol.samples = 2;
    vol.evaluator = constant_half;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.25, 0.0, 0.375}, 1e-6));

    // Test early termination: constant one density, 100 samples, opacity 1, ambient 1
    // After 1 step: sum=0.01, not terminated; after 100 steps sum=1.0, but break at step 100 only after adding.
    // Total color = 100 * 0.01 * (1,1,0.5) = (1,1,0.5). No difference from 10 samples.
    vol.samples = 100;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {1.0, 1.0, 0.5}, 1e-6));

    // Test ray starts inside box: origin at (0.2,0.5,0.5) moving +x
    // Box spans [0,1] so tnear=0, tfar=0.8, steps=4, dt=0.2
    // Sample at t=0.1,0.3,0.5,0.7 all with constant 1 density
    // Each transval = 1/4=0.25, sum=1.0 after 4 steps, color = 4*0.25*(1,1,0.5) = (1,1,0.5)
    vol.samples = 4;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {0.2, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {1.0, 1.0, 0.5}, 1e-6));

    // Test ray that starts inside but exits before summing to 1: constant 0.25 density, 4 samples, opacity 1
    // Box length 0.8, dt=0.2, each transval = 0.25/4=0.0625, sum=0.25<1
    // Color = 4 * 0.0625 * colormap(0.25) = 0.25 * (0.25, 0.0, 0.875) = (0.0625, 0.0, 0.21875)
    vol.samples = 4;
    vol.evaluator = [](double, double, double) -> double { return 0.25; };
    c = extvol_shade(vol, {0.2, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.0625, 0.0, 0.21875}, 1e-6));

    // Test ramp function: asymmetric density, just check it returns something non-negative and finite
    vol.samples = 16;
    vol.evaluator = diagonal_ramp;
    c = extvol_shade(vol, {-1.0, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(c.r >= 0.0 && c.g >= 0.0 && c.b >= 0.0);
    assert(c.r + c.g + c.b < 1e6); // finite

    // Test opacity less than 1: constant 1 density, opacity 0.5, 4 samples, ambient 1
    // per_step_base = 0.125, sum = 0.5 <1, color = 4*0.125*(1,1,0.5) = (0.5,0.5,0.25)
    vol.samples = 4;
    vol.opacity = 0.5;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.5, 0.5, 0.25}, 1e-6));

    // Test ambient scaling: same as above but ambient 0.5 -> (0.25,0.25,0.125)
    vol.ambient = 0.5;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {0.25, 0.25, 0.125}, 1e-6));

    // Reset for further tests
    vol.ambient = 1.0;
    vol.opacity = 1.0;

    // Test zero samples: should treat as 1 step, ray length 1.0, dt=1.0, sample at t=0.5
    // constant 1 density, transval=1.0, sum=1.0, color = (1,1,0.5)
    vol.samples = 0;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {-0.5, 0.5, 0.5}, {1.0, 0.0, 0.0});
    assert(close_color(c, {1.0, 1.0, 0.5}, 1e-6));

    // Test ray direction with negative components: from (1.5,1.5,1.5) to origin
    // Box [0,1]^3, should intersect
    vol.samples = 4;
    vol.evaluator = constant_one;
    c = extvol_shade(vol, {1.5, 1.5, 1.5}, {-1.0, -1.0, -1.0});
    // The ray passes through the box diagonal. Length inside box? For constant 1, color should sum to (1,1,0.5) because total opacity reaches 1.
    assert(close_color(c, {1.0, 1.0, 0.5}, 1e-6));

    return 0;
}
