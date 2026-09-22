Given a list of real numbers representing a sensor signal that oscillates around zero, write a C++ function that detects complete cycles by identifying zero-crossings with direction changes. A cycle is counted when the signal crosses zero from positive to negative (while the initial trend was downward) or from negative to positive (while the initial trend was upward). Collect consecutive samples between cycle boundaries into segments. After reading all samples, return a list of structs, where each struct contains: the segment's absolute average (mean of the absolute values of the samples in that segment) and a factor computed as (segment length) / (average_segment_length + 1). The detection must start by establishing the initial trend: ignore samples until the first nonzero difference between consecutive samples; use that difference's sign as the starting direction. If no complete cycle is ever established, return an empty list. Use `double` for all numeric values, and handle edge cases such as flat sections (zero differences) without miscounting cycles.
#include <cassert>
#include <cmath>
#include <vector>

// Assume solution function and struct are available via include or above.

int main() {
    // Single upward cycle: start flag = +1, crossing neg->pos
    std::vector<double> data1 = {-1.0, 0.0, 1.0, 2.0, -3.0};
    auto res1 = analyze_curve(data1);
    // Expect one segment: {-1,0,1,2,-3} length 5
    assert(res1.size() == 1);
    // abs_avg = (1+0+1+2+3)/5 = 7/5 = 1.4
    assert(std::fabs(res1[0].abs_avg - 1.4) < 1e-9);
    // factor = 5/(5+1) = 5/6
    assert(std::fabs(res1[0].factor - (5.0/6.0)) < 1e-9);

    // Two cycles: initial downward, then up-down crossing
    std::vector<double> data2 = {1.0, -1.0, 0.0, -2.0, 1.0, -1.0};
    // start_flag: first diff -2 -> -1 (down)
    // seg1: samples [1,-1,0,-2]? Wait: crossing pos->neg occurs when prev>0 and val<0.
    // Let's step:
    // val=1 (prev=1? Actually first val=1 sets prev=1)
    // val=-1: diff=-2 -> start=-1; prev=1>0 and val<0 => boundary. So segment = [1,-1]
    // Then val=0: no boundary (0 is not <0)
    // val=-2: prev=0 not >0, no boundary
    // val=1: prev=-2<0 but start_flag=-1, rule (a) requires prev>0, so no boundary
    // val=-1: prev=1>0 and val<0 => boundary. segment2 = [0,-2,1,-1]
    // So two segments lengths 2 and 4.
    auto res2 = analyze_curve(data2);
    assert(res2.size() == 2);
    assert(res2[0].abs_avg == 1.0); // (1+1)/2
    assert(std::fabs(res2[1].abs_avg - (0+2+1+1)/4.0) < 1e-9); // (4)/4=1
    double avg_len = (2+4)/2.0 = 3.0;
    assert(std::fabs(res2[0].factor - (2.0/(3.0+1.0))) < 1e-9); // 0.5
    assert(std::fabs(res2[1].factor - (4.0/(3.0+1.0))) < 1e-9); // 1.0

    // No cycles (constant signal)
    std::vector<double> data3 = {5.0, 5.0, 5.0};
    assert(analyze_curve(data3).empty());

    // Start flag is upward but no completion
    std::vector<double> data4 = {-1.0, 0.0, 1.0, 2.0}; // no boundary because never crosses from neg to pos after start? Actually start up, then prev<0? Let's step: first val=-1 sets prev=-1. next val=0: diff=1 -> start=1. no boundary (prev=-1<0 but val=0 not >0). next val=1: prev=0 not <0. next val=2: prev=1>0. No boundary. So empty.
    assert(analyze_curve(data4).empty());

    // Multiple flat regions but still cycles
    std::vector<double> data5 = {1.0, 1.0, -1.0, -1.0, 0.0, 1.0, -1.0};
    // start: first diff 0, then 0, then -2 => start=-1 at val=-1 (second -1? Actually data[0]=1, data[1]=1 diff=0, data[2]=-1 diff=-2 -> start=-1; prev=1>0 and val<0 => boundary. segment=[1,1,-1] length3.
    // Then data[3]=-1, data[4]=0, data[5]=1: at val=1, prev=0 no boundary; data[6]=-1: prev=1>0 and val<0 => boundary. segment=[-1,0,1,-1] length4.
    auto res5 = analyze_curve(data5);
    assert(res5.size() == 2);
    assert(res5[0].abs_avg == 1.0); // (1+1+1)/3
    assert(std::fabs(res5[1].abs_avg - (1+0+1+1)/4.0) < 1e-9); // 0.75
    double avg_len5 = (3+4)/2.0 = 3.5;
    assert(std::fabs(res5[0].factor - (3.0/(3.5+1.0))) < 1e-9);
    assert(std::fabs(res5[1].factor - (4.0/(3.5+1.0))) < 1e-9);

    return 0;
}
#include <vector>
#include <cmath>
#include <cstddef>

struct SegmentResult {
    double abs_avg;
    double factor;
};

// Detect complete cycles in a sensor signal and return segment statistics.
std::vector<SegmentResult> analyze_curve(const std::vector<double>& data) {
    std::vector<std::vector<double>> segments;
    std::vector<double> current_segment;
    int start_flag = 0; // 0 = unknown, 1 = upward, -1 = downward
    double prev = 0.0;
    bool has_prev = false;

    for (double val : data) {
        if (!has_prev) {
            prev = val;
            has_prev = true;
            current_segment.push_back(val);
            continue;
        }

        double diff = val - prev;
        if (start_flag == 0) {
            if (diff > 0) start_flag = 1;
            else if (diff < 0) start_flag = -1;
        }

        bool cycle_boundary = false;
        if (start_flag == -1 && prev > 0 && val < 0) {
            cycle_boundary = true;
        } else if (start_flag == 1 && prev < 0 && val > 0) {
            cycle_boundary = true;
        }

        current_segment.push_back(val);
        if (cycle_boundary) {
            segments.push_back(current_segment);
            current_segment.clear();
        }

        prev = val;
    }

    // If a partial segment remains without a following boundary, ignore it
    // because it does not represent a complete cycle.

    if (segments.empty()) return {};

    // Compute average segment length
    double total_length = 0.0;
    for (const auto& seg : segments) total_length += seg.size();
    double avg_len = total_length / static_cast<double>(segments.size());

    std::vector<SegmentResult> results;
    results.reserve(segments.size());
    for (const auto& seg : segments) {
        double sum_abs = 0.0;
        for (double v : seg) sum_abs += std::fabs(v);
        double abs_avg = sum_abs / static_cast<double>(seg.size());
        double factor = static_cast<double>(seg.size()) / (avg_len + 1.0);
        results.push_back({abs_avg, factor});
    }
    return results;
}
// The algorithm processes the input sequentially. Maintain a "start_flag" that is 0 until the first nonzero difference between consecutive samples determines whether the signal initially trends upward (+1) or downward (-1). Then, for each new sample, compare it with the previous sample. A cycle boundary is triggered when: (a) the start_flag is -1 (initial downward trend) and the previous sample was positive (>0) and the current sample is negative (<0) — that means the signal crossed from positive to negative after starting downward; or (b) the start_flag is +1 and the previous sample was negative (<0) and the current sample is positive (>0) — crossing from negative to positive after starting upward. Note that zero values are not treated as crossings; only strict signs matter. Each time a boundary is detected, the collected samples so far (including the current sample) form a segment. For each segment, compute the mean of the absolute values (sum of abs / count). Also compute the factor as segment_length / (average_segment_length + 1), where "average_segment_length" is the mean of all segment lengths after processing (so this requires two passes: first collect segments, then compute average length, then compute factors). If no segments exist, return empty. Edge cases: if the first two samples are equal, keep looking until a difference appears; if all samples are equal, start_flag remains 0 and no cycles are detected. Time complexity is O(n) for collecting segments and O(n) again for computing factors, so O(n) overall. Space complexity is O(n) for storing the segments.
