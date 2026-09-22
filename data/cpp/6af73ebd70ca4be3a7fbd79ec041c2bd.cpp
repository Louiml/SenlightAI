Write a C++ function `computeSimpleFlowParameters(int layers, int averaging_radius, int max_flow, double sigma_dist, double sigma_color, int postprocess_window, double sigma_dist_fix, double sigma_color_fix, double occ_thr, int upscale_averaging_radius, double upscale_sigma_dist, double upscale_sigma_color, double speed_up_thr)` that simulates the parameter validation and default-value logic of the OpenCV `SimpleFlow` optical flow algorithm. The function should take these 13 parameters as inputs and return a `std::string` describing the effective configuration, formatted as `"layers=3 averaging_radius=2 max_flow=4 sigma_dist=4.1 sigma_color=25.5 postprocess_window=18 sigma_dist_fix=55 sigma_color_fix=25.5 occ_thr=0.35 upscale_averaging_radius=18 upscale_sigma_dist=55 upscale_sigma_color=25.5 speed_up_thr=10"`. For any parameter that is `0` (or `0.0` for doubles), the function should replace it with the corresponding default value from the `OpticalFlowSimpleFlow` constructor defaults: `layers=3`, `averaging_radius=2`, `max_flow=4`, `sigma_dist=4.1`, `sigma_color=25.5`, `postprocess_window=18`, `sigma_dist_fix=55.0`, `sigma_color_fix=25.5`, `occ_thr=0.35`, `upscale_averaging_radius=18`, `upscale_sigma_dist=55.0`, `upscale_sigma_color=25.5`, `speed_up_thr=10`. Negative values should be rejected (throw `std::invalid_argument`), and non-zero positive values should be kept as-is. The output format must use fixed precision with exactly one decimal place for double parameters (e.g., `4.1`, `25.5`), and integer values printed as integers without decimal point.

#include <cassert>
#include <string>
#include <stdexcept>

int main() {
    // All defaults when all zeros.
    std::string expected_default = "layers=3 averaging_radius=2 max_flow=4 sigma_dist=4.1 sigma_color=25.5 postprocess_window=18 sigma_dist_fix=55.0 sigma_color_fix=25.5 occ_thr=0.35 upscale_averaging_radius=18 upscale_sigma_dist=55.0 upscale_sigma_color=25.5 speed_up_thr=10.0";
    assert(computeSimpleFlowParameters(0,0,0,0.0,0.0,0,0.0,0.0,0.0,0,0.0,0.0,0.0) == expected_default);

    // Mixed: some nonzero, some zero.
    std::string expected_mixed = "layers=5 averaging_radius=2 max_flow=8 sigma_dist=4.1 sigma_color=10.0 postprocess_window=18 sigma_dist_fix=55.0 sigma_color_fix=30.0 occ_thr=0.5 upscale_averaging_radius=20 upscale_sigma_dist=60.0 upscale_sigma_color=25.5 speed_up_thr=15.0";
    assert(computeSimpleFlowParameters(5,0,8,0.0,10.0,0,0.0,30.0,0.5,20,60.0,0.0,15.0) == expected_mixed);

    // All nonzero kept as is.
    std::string expected_all = "layers=1 averaging_radius=1 max_flow=1 sigma_dist=1.0 sigma_color=1.0 postprocess_window=1 sigma_dist_fix=1.0 sigma_color_fix=1.0 occ_thr=0.1 upscale_averaging_radius=1 upscale_sigma_dist=1.0 upscale_sigma_color=1.0 speed_up_thr=1.0";
    assert(computeSimpleFlowParameters(1,1,1,1.0,1.0,1,1.0,1.0,0.1,1,1.0,1.0,1.0) == expected_all);

    // Negative values throw.
    bool threw = false;
    try { computeSimpleFlowParameters(-1,0,0,0.0,0.0,0,0.0,0.0,0.0,0,0.0,0.0,0.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { computeSimpleFlowParameters(0,0,0,-1.0,0.0,0,0.0,0.0,0.0,0,0.0,0.0,0.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Very large positive values are preserved.
    std::string expected_large = "layers=100 averaging_radius=50 max_flow=1000 sigma_dist=10.0 sigma_color=20.0 postprocess_window=100 sigma_dist_fix=200.0 sigma_color_fix=300.0 occ_thr=0.9 upscale_averaging_radius=40 upscale_sigma_dist=500.0 upscale_sigma_color=600.0 speed_up_thr=100.0";
    assert(computeSimpleFlowParameters(100,50,1000,10.0,20.0,100,200.0,300.0,0.9,40,500.0,600.0,100.0) == expected_large);

    return 0;
}

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Returns a formatted string of the effective SimpleFlow parameters. Zero inputs are replaced by defaults; negative inputs throw.
std::string computeSimpleFlowParameters(
    int layers, int averaging_radius, int max_flow,
    double sigma_dist, double sigma_color,
    int postprocess_window,
    double sigma_dist_fix, double sigma_color_fix,
    double occ_thr,
    int upscale_averaging_radius,
    double upscale_sigma_dist, double upscale_sigma_color,
    double speed_up_thr)
{
    // Defaults from the OpticalFlowSimpleFlow constructor.
    const int    def_layers = 3;
    const int    def_averaging_radius = 2;
    const int    def_max_flow = 4;
    const double def_sigma_dist = 4.1;
    const double def_sigma_color = 25.5;
    const int    def_postprocess_window = 18;
    const double def_sigma_dist_fix = 55.0;
    const double def_sigma_color_fix = 25.5;
    const double def_occ_thr = 0.35;
    const int    def_upscale_averaging_radius = 18;
    const double def_upscale_sigma_dist = 55.0;
    const double def_upscale_sigma_color = 25.5;
    const double def_speed_up_thr = 10.0;

    // Validate negative values.
    if (layers < 0) throw std::invalid_argument("layers must be non-negative");
    if (averaging_radius < 0) throw std::invalid_argument("averaging_radius must be non-negative");
    if (max_flow < 0) throw std::invalid_argument("max_flow must be non-negative");
    if (sigma_dist < 0.0) throw std::invalid_argument("sigma_dist must be non-negative");
    if (sigma_color < 0.0) throw std::invalid_argument("sigma_color must be non-negative");
    if (postprocess_window < 0) throw std::invalid_argument("postprocess_window must be non-negative");
    if (sigma_dist_fix < 0.0) throw std::invalid_argument("sigma_dist_fix must be non-negative");
    if (sigma_color_fix < 0.0) throw std::invalid_argument("sigma_color_fix must be non-negative");
    if (occ_thr < 0.0) throw std::invalid_argument("occ_thr must be non-negative");
    if (upscale_averaging_radius < 0) throw std::invalid_argument("upscale_averaging_radius must be non-negative");
    if (upscale_sigma_dist < 0.0) throw std::invalid_argument("upscale_sigma_dist must be non-negative");
    if (upscale_sigma_color < 0.0) throw std::invalid_argument("upscale_sigma_color must be non-negative");
    if (speed_up_thr < 0.0) throw std::invalid_argument("speed_up_thr must be non-negative");

    // Replace zeros with defaults.
    if (layers == 0) layers = def_layers;
    if (averaging_radius == 0) averaging_radius = def_averaging_radius;
    if (max_flow == 0) max_flow = def_max_flow;
    if (sigma_dist == 0.0) sigma_dist = def_sigma_dist;
    if (sigma_color == 0.0) sigma_color = def_sigma_color;
    if (postprocess_window == 0) postprocess_window = def_postprocess_window;
    if (sigma_dist_fix == 0.0) sigma_dist_fix = def_sigma_dist_fix;
    if (sigma_color_fix == 0.0) sigma_color_fix = def_sigma_color_fix;
    if (occ_thr == 0.0) occ_thr = def_occ_thr;
    if (upscale_averaging_radius == 0) upscale_averaging_radius = def_upscale_averaging_radius;
    if (upscale_sigma_dist == 0.0) upscale_sigma_dist = def_upscale_sigma_dist;
    if (upscale_sigma_color == 0.0) upscale_sigma_color = def_upscale_sigma_color;
    if (speed_up_thr == 0.0) speed_up_thr = def_speed_up_thr;

    // Format the output.
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "layers=" << layers
        << " averaging_radius=" << averaging_radius
        << " max_flow=" << max_flow
        << " sigma_dist=" << sigma_dist
        << " sigma_color=" << sigma_color
        << " postprocess_window=" << postprocess_window
        << " sigma_dist_fix=" << sigma_dist_fix
        << " sigma_color_fix=" << sigma_color_fix
        << " occ_thr=" << occ_thr
        << " upscale_averaging_radius=" << upscale_averaging_radius
        << " upscale_sigma_dist=" << upscale_sigma_dist
        << " upscale_sigma_color=" << upscale_sigma_color
        << " speed_up_thr=" << speed_up_thr;
    return oss.str();
}

// The solution approach is to define a struct or use a helper function that holds the default values and then iterates over the input parameters, replacing any zero value with the stored default. For validation, check each parameter: if it is negative, throw an `std::invalid_argument` with a descriptive message indicating which parameter is invalid. Since the parameters come in a fixed order, we can process them sequentially. To maintain accuracy in formatting, use `std::fixed` and `std::setprecision(1)` on a `std::ostringstream` for building the result string. For doubles, compare to `0.0` exactly (they are passed as `double` so zero is `0.0`). For integers, compare to `0`. The function returns the formatted string. Edge cases include all-zero inputs (must return all defaults), a mix of zeros and valid positives, and negative values (which must throw). Time complexity is O(1) since there are exactly 13 parameters, and space complexity is O(1) for storage plus the output string length.
