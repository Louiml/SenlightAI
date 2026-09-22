// Write a standalone C++ function `generate_prior_boxes` that takes as input: the feature map width and height (`layer_width`, `layer_height`), the image width and height (`img_width`, `img_height`), a vector of positive `min_sizes`, a vector of `aspect_ratios` (all positive, may contain duplicates, should be deduplicated and optionally include reciprocals), a boolean `flip` (whether to add reciprocal aspect ratios), a `clip` boolean (whether to clamp coordinates to [0,1]), a single `variance` value, and a `step` (positive float) plus `offset` (default 0.5). The function must output a `std::vector<float>` containing, for each spatial cell `(h,w)`, and for each prior box (in the exact order: first the square of size `min_size`, then if `max_sizes` is provided (same length as `min_sizes`) the square with size `sqrt(min*max)`, then for each aspect ratio `ar != 1` a box with width = `min_size*sqrt(ar)` and height = `min_size/sqrt(ar)`), the four normalized coordinates `[xmin, ymin, xmax, ymax]` (each divided by image width/height respectively). The output vector must contain **only** the mean coordinates (no variance), and the order must iterate over `h` from 0 to `layer_height-1`, then `w` from 0 to `layer_width-1`, and within each cell follow the sequence specified. The aspect ratio list must be built as follows: start with `[1.0]`; for each input ratio, add it if not already present (within 1e-6 tolerance), and if `flip` is true, also add `1/ratio` if not already present. The output size must be exactly `layer_height * layer_width * num_priors * 4`, where `num_priors` is computed appropriately. If `clip` is true, clamp all coordinates to [0,1]. Handle edge cases: if `min_sizes` is empty, throw `std::invalid_argument`; if `max_sizes` is non-empty and its size differs from `min_sizes`, throw; if any `min_size` is non-positive or any aspect ratio is non-positive, throw. The function should be `const`-correct and use `double` internally for precision, but return a `std::vector<float>`.

// The solution processes each spatial cell independently. For each cell, we compute its center as `(w + offset) * step` and `(h + offset) * step` (using `float` or `double`). Then we generate boxes in a strict order: first the square with side `min_size`, then if `max_sizes` is given, the square with side `sqrt(min_size*max_size)`, then for each aspect ratio in the deduplicated list (excluding 1.0) we generate a rectangle with width `min_size*sqrt(ar)` and height `min_size/sqrt(ar)`. The coordinates are normalized by dividing by image width/height. We must ensure `num_priors` is computed as: `min_sizes.size()` (for the first square) + (if `max_sizes` non-empty) `min_sizes.size()` (for the sqrt boxes) + (for each aspect ratio !=1) `min_sizes.size()`. But note: in the original snippet, the order is: for each `min_size`, output the square, then if max_sizes exist output the sqrt box, then for all aspect ratios (except 1) output the rectangle. So `num_priors = min_sizes.size() * (1 + (max_sizes.empty()?0:1) + (number of distinct aspect ratios !=1))`. Because `aspect_ratios_` may include 1, we count only those where `fabs(ar-1)>1e-6`. Edge cases: empty `min_sizes` → throw; max_sizes size mismatch → throw; non-positive min_size or aspect_ratio → throw; if `step` is zero or negative → throw (the snippet assumes positive). The deduplication uses a tolerance of 1e-6. The final vector must be exactly the required length; we can reserve it. Complexity: For each cell, we generate `num_priors` boxes, each O(1), so total time is O(layer_height * layer_width * num_priors), which is linear in output size. Space is O(1) additional beyond the output vector. Use `double` for intermediate arithmetic to avoid precision loss, but cast to `float` when storing.

#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

std::vector<float> generate_prior_boxes(
    int layer_width,
    int layer_height,
    int img_width,
    int img_height,
    const std::vector<float>& min_sizes,
    const std::vector<float>& aspect_ratios,
    bool flip,
    bool clip,
    const std::vector<float>& max_sizes,   // empty if not provided
    float variance,                        // unused for output (only means)
    float step,
    float offset = 0.5f) {

    if (min_sizes.empty()) {
        throw std::invalid_argument("min_sizes must not be empty");
    }
    if (layer_width <= 0 || layer_height <= 0) {
        throw std::invalid_argument("layer dimensions must be positive");
    }
    if (img_width <= 0 || img_height <= 0) {
        throw std::invalid_argument("image dimensions must be positive");
    }
    if (step <= 0.0f) {
        throw std::invalid_argument("step must be positive");
    }
    for (float ms : min_sizes) {
        if (ms <= 0.0f) {
            throw std::invalid_argument("min_sizes must be positive");
        }
    }
    if (!max_sizes.empty() && max_sizes.size() != min_sizes.size()) {
        throw std::invalid_argument("max_sizes must be empty or same length as min_sizes");
    }
    for (float mxs : max_sizes) {
        if (mxs <= 0.0f) {
            throw std::invalid_argument("max_sizes must be positive");
        }
    }
    for (float ar : aspect_ratios) {
        if (ar <= 0.0f) {
            throw std::invalid_argument("aspect_ratios must be positive");
        }
    }

    // Build deduplicated aspect ratio list, starting with 1.0
    std::vector<double> aspect_list;
    aspect_list.push_back(1.0);
    for (float ar : aspect_ratios) {
        double a = static_cast<double>(ar);
        bool exists = false;
        for (double existing : aspect_list) {
            if (std::fabs(existing - a) < 1e-6) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            aspect_list.push_back(a);
            if (flip) {
                double reciprocal = 1.0 / a;
                bool recip_exists = false;
                for (double existing : aspect_list) {
                    if (std::fabs(existing - reciprocal) < 1e-6) {
                        recip_exists = true;
                        break;
                    }
                }
                if (!recip_exists) {
                    aspect_list.push_back(reciprocal);
                }
            }
        }
    }

    // Count number of distinct aspect ratios that are not 1.0
    int non_one_aspects = 0;
    for (double a : aspect_list) {
        if (std::fabs(a - 1.0) >= 1e-6) {
            ++non_one_aspects;
        }
    }

    int num_priors_per_cell = static_cast<int>(min_sizes.size()) * (1 + (max_sizes.empty() ? 0 : 1) + non_one_aspects);
    if (num_priors_per_cell <= 0) {
        throw std::invalid_argument("No priors generated");
    }

    std::vector<float> result;
    result.reserve(static_cast<size_t>(layer_height) * layer_width * num_priors_per_cell * 4);

    double step_d = static_cast<double>(step);
    double offset_d = static_cast<double>(offset);

    for (int h = 0; h < layer_height; ++h) {
        for (int w = 0; w < layer_width; ++w) {
            double center_x = (w + offset_d) * step_d;
            double center_y = (h + offset_d) * step_d;

            for (size_t s = 0; s < min_sizes.size(); ++s) {
                double min_size = static_cast<double>(min_sizes[s]);

                // First: square of size min_size
                {
                    double box_w = min_size;
                    double box_h = min_size;
                    double xmin = (center_x - box_w / 2.0) / img_width;
                    double ymin = (center_y - box_h / 2.0) / img_height;
                    double xmax = (center_x + box_w / 2.0) / img_width;
                    double ymax = (center_y + box_h / 2.0) / img_height;
                    result.push_back(static_cast<float>(xmin));
                    result.push_back(static_cast<float>(ymin));
                    result.push_back(static_cast<float>(xmax));
                    result.push_back(static_cast<float>(ymax));
                }

                // Second: if max_sizes given, square of size sqrt(min*max)
                if (!max_sizes.empty()) {
                    double max_size = static_cast<double>(max_sizes[s]);
                    double box_w = std::sqrt(min_size * max_size);
                    double box_h = box_w;
                    double xmin = (center_x - box_w / 2.0) / img_width;
                    double ymin = (center_y - box_h / 2.0) / img_height;
                    double xmax = (center_x + box_w / 2.0) / img_width;
                    double ymax = (center_y + box_h / 2.0) / img_height;
                    result.push_back(static_cast<float>(xmin));
                    result.push_back(static_cast<float>(ymin));
                    result.push_back(static_cast<float>(xmax));
                    result.push_back(static_cast<float>(ymax));
                }

                // Remaining priors for each non-1 aspect ratio
                for (double ar : aspect_list) {
                    if (std::fabs(ar - 1.0) < 1e-6) {
                        continue;
                    }
                    double box_w = min_size * std::sqrt(ar);
                    double box_h = min_size / std::sqrt(ar);
                    double xmin = (center_x - box_w / 2.0) / img_width;
                    double ymin = (center_y - box_h / 2.0) / img_height;
                    double xmax = (center_x + box_w / 2.0) / img_width;
                    double ymax = (center_y + box_h / 2.0) / img_height;
                    result.push_back(static_cast<float>(xmin));
                    result.push_back(static_cast<float>(ymin));
                    result.push_back(static_cast<float>(xmax));
                    result.push_back(static_cast<float>(ymax));
                }
            }
        }
    }

    if (clip) {
        for (float& val : result) {
            val = std::min(1.0f, std::max(0.0f, val));
        }
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

int main() {
    using std::vector;
    // Helper to compare with tolerance
    auto close = [](float a, float b) { return std::fabs(a - b) < 1e-5f; };
    auto check_vector = [&](const vector<float>& got, const vector<float>& expected) {
        assert(got.size() == expected.size());
        for (size_t i = 0; i < got.size(); ++i) {
            assert(close(got[i], expected[i]));
        }
    };

    // Test 1: Simple 1x1 feature, image 100x100, min_size=30, no max, aspect 1 only, step=100, offset=0.5, no flip
    {
        vector<float> min_sizes = {30.0f};
        vector<float> aspect_ratios = {1.0f};
        vector<float> max_sizes; // empty
        vector<float> result = generate_prior_boxes(1, 1, 100, 100, min_sizes, aspect_ratios, false, false, max_sizes, 0.1f, 100.0f, 0.5f);
        // center at (0.5*100, 0.5*100) = (50,50), box 30x30 -> xmin=35,ymin=35,xmax=65,ymax=65 normalized by 100
        vector<float> expected = {0.35f, 0.35f, 0.65f, 0.65f};
        check_vector(result, expected);
    }

    // Test 2: 1x1 feature, image 100x100, min_size=30, max_size=50, aspect 1, step=100, offset=0.5
    {
        vector<float> min_sizes = {30.0f};
        vector<float> max_sizes = {50.0f};
        vector<float> aspect_ratios = {1.0f};
        vector<float> result = generate_prior_boxes(1, 1, 100, 100, min_sizes, max_sizes, aspect_ratios, false, false, max_sizes, 0.1f, 100.0f, 0.5f);
        // First box: square 30 -> as above
        // Second box: sqrt(30*50)=sqrt(1500)=38.7298... center 50,50 -> xmin=50-19.3649=30.6351, etc.
        // Expected manually calculated with tolerance
        vector<float> expected = {0.35f, 0.35f, 0.65f, 0.65f,
                                  0.306351f, 0.306351f, 0.693649f, 0.693649f};
        check_vector(result, expected);
    }

    // Test 3: Aspect ratios deduplication and flip
    {
        vector<float> min_sizes = {20.0f};
        vector<float> aspect_ratios = {2.0f, 2.0f, 0.5f};
        vector<float> max_sizes; // empty
        // flip true: start [1], add 2, add 1/2=0.5, then 0.5 already exists, so final [1,2,0.5]
        // non-one aspects: 2 and 0.5 -> 2 priors per min_size
        vector<float> result = generate_prior_boxes(1, 1, 100, 100, min_sizes, aspect_ratios, true, false, max_sizes, 0.1f, 100.0f, 0.5f);
        // Expected: first square 20 -> (50-10)/100, ... = 0.4,0.4,0.6,0.6
        // For ar=2: width=20*sqrt(2)=28.2843, height=20/sqrt(2)=14.1421 -> xmin=50-14.1421=35.8579/100=0.358579, etc.
        // For ar=0.5: width=20*sqrt(0.5)=14.1421, height=20/sqrt(0.5)=28.2843 -> xmin=50-7.0711=42.9289/100=0.429289, etc.
        vector<float> expected = {0.4f, 0.4f, 0.6f, 0.6f,
                                  0.358579f, 0.429289f, 0.641421f, 0.570711f,
                                  0.429289f, 0.358579f, 0.570711f, 0.641421f};
        check_vector(result, expected);
    }

    // Test 4: Multiple cells and clip
    {
        vector<float> min_sizes = {10.0f};
        vector<float> aspect_ratios = {}; // empty, still gets [1]
        vector<float> max_sizes; // empty
        // 2x2 feature, image 20x20, step=10, offset=0.0 so centers at (0,0), (10,0), etc.
        vector<float> result = generate_prior_boxes(2, 2, 20, 20, min_sizes, aspect_ratios, false, true, max_sizes, 0.1f, 10.0f, 0.0f);
        // For each cell, box 10x10, center (5,5) normalized? Actually step=10, offset=0 -> center_x = w*10.
        // For w=0,h=0: center (0,0), box 10x10 -> xmin=-5/20=-0.25, ymin=-0.25, but clip -> 0,0, xmax=5/20=0.25, ymax=0.25
        // For w=1,h=0: center (10,0) -> xmin=5/20=0.25, ymin=-0.25->0, xmax=15/20=0.75, ymax=0.25
        // For w=0,h=1: center (0,10) -> xmin=0, ymin=0.25, xmax=0.25, ymax=0.75
        // For w=1,h=1: center (10,10) -> 0.25,0.25,0.75,0.75
        vector<float> expected = {0.0f,0.0f,0.25f,0.25f,
                                  0.25f,0.0f,0.75f,0.25f,
                                  0.0f,0.25f,0.25f,0.75f,
                                  0.25f,0.25f,0.75f,0.75f};
        check_vector(result, expected);
    }

    // Test 5: Error handling for empty min_sizes
    {
        bool threw = false;
        try {
            vector<float> empty;
            generate_prior_boxes(1, 1, 10, 10, empty, {1.0f}, false, false, {}, 0.1f, 1.0f);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 6: Mismatched max_sizes
    {
        bool threw = false;
        try {
            vector<float> min_sizes = {10.0f, 20.0f};
            vector<float> max_sizes = {30.0f}; // wrong length
            generate_prior_boxes(1, 1, 10, 10, min_sizes, {1.0f}, false, false, max_sizes, 0.1f, 1.0f);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }
}
