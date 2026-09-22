// Write a standalone C++ function named `downsampleImuSamples` that processes a sequence of IMU (Inertial Measurement Unit) samples and accumulates them into a single downsampled sample. The function must take a vector of `ImuSample` structs (each containing `delta_ang` as a 3D vector in radians, `delta_vel` as a 3D vector in m/s, `delta_ang_dt` and `delta_vel_dt` as floats in seconds, and `delta_vel_clipping` as an array of 3 booleans), plus a target time interval `target_dt_us` in microseconds. It should return a single `ImuSample` representing the accumulated result after processing samples until the accumulated time reaches or exceeds the target interval (using a similar logic to the snippet: accumulate a quaternion for angular changes, rotate velocity into the updated frame, combine velocities, and combine clipping flags). The function may process all samples if needed, and should reset its internal state for each call. Edge cases include empty input (return a zero-initialized sample), very small or very large target intervals, and samples with zero or negative time deltas.
The solution mirrors the core accumulation logic from the snippet but without state persistence between calls—each invocation starts fresh. The algorithm iterates through the input samples, accumulating: 1) total `delta_ang_dt` and `delta_vel_dt`; 2) combined clipping flags via logical OR; 3) a normalized quaternion product representing the aggregate rotation (using `AxisAnglef` and `Quatf` concepts, which can be replaced with simple quaternion multiplication and normalization, or represented as a 3D axis-angle vector combined using quaternion math); 4) rotating the previously accumulated velocity into the new frame (using the inverse of the current delta quaternion) and then adding the sample’s velocity, with the midpoint approximation for effective sample time. To avoid external libraries, we implement minimal quaternion helpers (multiplication, normalization, rotation of a vector by a quaternion, and conversion between axis-angle and quaternion). The accumulation continues until either the number of samples meets a required count derived from the average delta-angular-dt vs target, or the accumulated time exceeds the target. Required sample count is computed from the average `delta_ang_dt` seen so far, clamped to a minimum 1. Time complexity is O(n) where n is the number of samples in the input vector, and O(1) space beyond the returned sample. Edge cases: if input is empty, return a zero sample; if target_dt_us is less than 1000 or more than 100000, clamp it; if a sample has non-positive deltas, they are still accumulated but could lead to zero average; the function must handle that gracefully (e.g., ensure at least one sample accumulates; if `delta_ang_dt` is zero, fall back to accumulating at least one sample). The quaternion rotation logic must correctly normalize after each multiplication to prevent drift.
#include <vector>
#include <cmath>
#include <algorithm>

struct ImuSample {
    float delta_ang[3] = {0.0f, 0.0f, 0.0f}; // radians
    float delta_vel[3] = {0.0f, 0.0f, 0.0f}; // m/s
    float delta_ang_dt = 0.0f; // seconds
    float delta_vel_dt = 0.0f; // seconds
    bool delta_vel_clipping[3] = {false, false, false};
};

// Minimal quaternion representation (x,y,z,w)
struct Quat {
    float x, y, z, w;
    Quat() : x(0), y(0), z(0), w(1) {}
    Quat(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

// Helper to build a quaternion from an axis-angle vector
Quat quatFromAxisAngle(const float* axisAngle) {
    float angle = std::sqrt(axisAngle[0]*axisAngle[0] + axisAngle[1]*axisAngle[1] + axisAngle[2]*axisAngle[2]);
    if (angle < 1e-8f) {
        return Quat();
    }
    float half = angle * 0.5f;
    float s = std::sin(half) / angle;
    return Quat(axisAngle[0] * s, axisAngle[1] * s, axisAngle[2] * s, std::cos(half));
}

// Helper to extract axis-angle vector from quaternion
void axisAngleFromQuat(const Quat& q, float* out) {
    float angle = 2.0f * std::acos(std::min(1.0f, std::max(-1.0f, q.w)));
    if (angle < 1e-8f) {
        out[0] = out[1] = out[2] = 0.0f;
        return;
    }
    float s = std::sqrt(1.0f - q.w*q.w);
    float scale = (s > 1e-6f) ? angle / s : 0.0f;
    out[0] = q.x * scale;
    out[1] = q.y * scale;
    out[2] = q.z * scale;
}

// Quaternion multiplication
Quat quatMultiply(const Quat& a, const Quat& b) {
    return Quat(
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    );
}

// Normalize quaternion
Quat quatNormalize(const Quat& q) {
    float norm = std::sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (norm < 1e-8f) return Quat();
    return Quat(q.x/norm, q.y/norm, q.z/norm, q.w/norm);
}

// Rotate a 3D vector by a quaternion (assuming quaternion is normalized)
void rotateVectorByQuat(const Quat& q, const float* v, float* out) {
    // q * v * q^-1, using formula: out = v + 2*q_cross(q_cross(v) + q_w*v)
    float qv[3] = {q.x, q.y, q.z};
    float cross1[3];
    cross1[0] = qv[1]*v[2] - qv[2]*v[1];
    cross1[1] = qv[2]*v[0] - qv[0]*v[2];
    cross1[2] = qv[0]*v[1] - qv[1]*v[0];

    float tmp[3];
    tmp[0] = cross1[0] + q.w * v[0];
    tmp[1] = cross1[1] + q.w * v[1];
    tmp[2] = cross1[2] + q.w * v[2];

    float cross2[3];
    cross2[0] = qv[1]*tmp[2] - qv[2]*tmp[1];
    cross2[1] = qv[2]*tmp[0] - qv[0]*tmp[2];
    cross2[2] = qv[0]*tmp[1] - qv[1]*tmp[0];

    out[0] = v[0] + 2.0f * cross2[0];
    out[1] = v[1] + 2.0f * cross2[1];
    out[2] = v[2] + 2.0f * cross2[2];
}

// Main downsample function
ImuSample downsampleImuSamples(const std::vector<ImuSample>& samples, int32_t target_dt_us) {
    ImuSample result; // zero-initialized by default member initializers
    if (samples.empty()) {
        return result;
    }

    // Clamp target based on snippet constraints
    int32_t clamped_target_us = std::clamp(target_dt_us, (int32_t)1000, (int32_t)100000);
    float target_dt_s = clamped_target_us * 1e-6f;

    // State for accumulation
    float dt_avg = 1e-3f; // default 1ms if samples have zero dt initially? Better: use first sample's dt if positive
    int num_samples = 0;
    Quat total_q; // identity
    float accumulated_vel[3] = {0.0f, 0.0f, 0.0f};
    float accumulated_ang_dt = 0.0f;
    float accumulated_vel_dt = 0.0f;
    bool clipping[3] = {false, false, false};

    // Determine average dt from first sample if it has positive delta_ang_dt
    if (samples[0].delta_ang_dt > 0.0f) {
        dt_avg = samples[0].delta_ang_dt;
    }

    // Compute required samples based on initial dt_avg (will be updated)
    int required_samples = std::max((int)std::round(target_dt_s / dt_avg), 1);
    float min_dt_s = std::max(dt_avg * (required_samples - 1.0f), dt_avg * 0.5f);

    for (const auto& sample : samples) {
        // Update average dt
        if (sample.delta_ang_dt > 0.0f) {
            dt_avg = 0.9f * dt_avg + 0.1f * sample.delta_ang_dt;
        }

        // Accumulate time deltas
        accumulated_ang_dt += sample.delta_ang_dt;
        accumulated_vel_dt += sample.delta_vel_dt;
        clipping[0] |= sample.delta_vel_clipping[0];
        clipping[1] |= sample.delta_vel_clipping[1];
        clipping[2] |= sample.delta_vel_clipping[2];

        // Accumulate rotation via quaternion multiplication
        Quat delta_q = quatFromAxisAngle(sample.delta_ang);
        total_q = quatNormalize(quatMultiply(total_q, delta_q));

        // Rotate previously accumulated velocity into new frame (inverse of delta_q)
        // Use conjugate for inverse on unit quaternion
        Quat delta_q_inv = Quat(-delta_q.x, -delta_q.y, -delta_q.z, delta_q.w);
        float rotated_vel[3];
        rotateVectorByQuat(delta_q_inv, accumulated_vel, rotated_vel);
        accumulated_vel[0] = rotated_vel[0];
        accumulated_vel[1] = rotated_vel[1];
        accumulated_vel[2] = rotated_vel[2];

        // Add current sample's velocity with midpoint approximation
        float rotated_sample_vel[3];
        rotateVectorByQuat(delta_q_inv, sample.delta_vel, rotated_sample_vel);
        accumulated_vel[0] += (sample.delta_vel[0] + rotated_sample_vel[0]) * 0.5f;
        accumulated_vel[1] += (sample.delta_vel[1] + rotated_sample_vel[1]) * 0.5f;
        accumulated_vel[2] += (sample.delta_vel[2] + rotated_sample_vel[2]) * 0.5f;

        num_samples++;

        // Update required samples based on current average
        required_samples = std::max((int)std::round(target_dt_s / dt_avg), 1);
        min_dt_s = std::max(dt_avg * (required_samples - 1.0f), dt_avg * 0.5f);

        // Check if we have enough
        if ((num_samples >= required_samples && accumulated_ang_dt > min_dt_s) ||
            (accumulated_ang_dt > target_dt_s)) {
            break;
        }
    }

    // Build result
    result.delta_ang_dt = accumulated_ang_dt;
    result.delta_vel_dt = accumulated_vel_dt;
    axisAngleFromQuat(total_q, result.delta_ang);
    std::copy(accumulated_vel, accumulated_vel+3, result.delta_vel);
    std::copy(clipping, clipping+3, result.delta_vel_clipping);

    return result;
}
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    // Test 1: Empty input returns zero-initialized sample
    {
        ImuSample out = downsampleImuSamples({}, 1000);
        assert(out.delta_ang[0] == 0.0f && out.delta_ang[1] == 0.0f && out.delta_ang[2] == 0.0f);
        assert(out.delta_vel[0] == 0.0f && out.delta_vel[1] == 0.0f && out.delta_vel[2] == 0.0f);
        assert(out.delta_ang_dt == 0.0f && out.delta_vel_dt == 0.0f);
        assert(!out.delta_vel_clipping[0] && !out.delta_vel_clipping[1] && !out.delta_vel_clipping[2]);
    }

    // Test 2: Single sample accumulates to itself (within float tolerance)
    {
        ImuSample s;
        s.delta_ang[0] = 0.1f; s.delta_ang[1] = 0.0f; s.delta_ang[2] = 0.0f;
        s.delta_vel[0] = 1.0f; s.delta_vel[1] = 2.0f; s.delta_vel[2] = 3.0f;
        s.delta_ang_dt = 0.005f; s.delta_vel_dt = 0.005f;
        s.delta_vel_clipping[1] = true;
        std::vector<ImuSample> v = {s};
        ImuSample out = downsampleImuSamples(v, 10000); // target 10ms > sample dt
        assert(std::fabs(out.delta_ang[0] - 0.1f) < 1e-4f);
        assert(std::fabs(out.delta_vel[0] - 1.0f) < 1e-3f);
        assert(std::fabs(out.delta_vel[1] - 2.0f) < 1e-3f);
        assert(std::fabs(out.delta_vel[2] - 3.0f) < 1e-3f);
        assert(std::fabs(out.delta_ang_dt - 0.005f) < 1e-6f);
        assert(out.delta_vel_clipping[1] == true);
    }

    // Test 3: Two samples with same dt and rotated frame; check time accumulation and clipping OR
    {
        ImuSample s1, s2;
        s1.delta_ang = {0.0f, 0.0f, 0.1f};
        s1.delta_vel = {1.0f, 0.0f, 0.0f};
        s1.delta_ang_dt = 0.01f; s1.delta_vel_dt = 0.01f;
        s1.delta_vel_clipping[0] = true;

        s2.delta_ang = {0.0f, 0.0f, 0.1f};
        s2.delta_vel = {1.0f, 0.0f, 0.0f};
        s2.delta_ang_dt = 0.01f; s2.delta_vel_dt = 0.01f;
        s2.delta_vel_clipping[0] = false; // should still be true due to OR

        std::vector<ImuSample> v = {s1, s2};
        ImuSample out = downsampleImuSamples(v, 5000); // target 5ms? but total dt 20ms, should accumulate both
        // Since target 5ms and total 20ms, it will accumulate all because accumulated time exceeds target
        assert(std::fabs(out.delta_ang_dt - 0.02f) < 1e-6f);
        assert(out.delta_vel_clipping[0] == true);
        // Total rotation approximately 0.2 radians around z (small angle, but check magnitude)
        float ang_mag = std::sqrt(out.delta_ang[0]*out.delta_ang[0] + out.delta_ang[1]*out.delta_ang[1] + out.delta_ang[2]*out.delta_ang[2]);
        assert(std::fabs(ang_mag - 0.2f) < 1e-3f);
        // Velocity should be roughly 2 m/s in the final frame (small rotation effects)
        float vel_mag = std::sqrt(out.delta_vel[0]*out.delta_vel[0] + out.delta_vel[1]*out.delta_vel[1] + out.delta_vel[2]*out.delta_vel[2]);
        assert(std::fabs(vel_mag - 2.0f) < 1e-2f);
    }

    // Test 4: Target interval enforcement: very short target stops after one sample
    {
        ImuSample s1, s2;
        s1.delta_ang = {0.0f, 0.0f, 0.0f};
        s1.delta_vel = {1.0f, 0.0f, 0.0f};
        s1.delta_ang_dt = 0.01f; s1.delta_vel_dt = 0.01f;

        s2.delta_ang = {0.0f, 0.0f, 0.0f};
        s2.delta_vel = {1.0f, 0.0f, 0.0f};
        s2.delta_ang_dt = 0.01f; s2.delta_vel_dt = 0.01f;

        std::vector<ImuSample> v = {s1, s2};
        ImuSample out = downsampleImuSamples(v, 1000); // target 1ms but min clamp is 1000us => 1ms? Actually clamp to 1000us = 1ms < 10ms, so should only accumulate first sample because time exceeds target
        assert(std::fabs(out.delta_ang_dt - 0.01f) < 1e-6f);
        assert(std::fabs(out.delta_vel[0] - 1.0f) < 1e-3f);
    }

    // Test 5: Target larger than all samples accumulates all
    {
        ImuSample s;
        s.delta_ang = {0.0f, 0.0f, 0.0f};
        s.delta_vel = {0.5f, 0.0f, 0.0f};
        s.delta_ang_dt = 0.001f; s.delta_vel_dt = 0.001f;
        std::vector<ImuSample> v;
        for (int i=0; i<10; ++i) v.push_back(s);
        ImuSample out = downsampleImuSamples(v, 100000); // max clamp 100000us = 100ms, total 10ms, so accumulate all
        assert(std::fabs(out.delta_ang_dt - 0.01f) < 1e-6f);
        assert(std::fabs(out.delta_vel[0] - 5.0f) < 1e-2f);
    }

    // Test 6: Clipping combination with OR
    {
        ImuSample s1, s2;
        s1.delta_vel_clipping[2] = true;
        s2.delta_vel_clipping[2] = false;
        s1.delta_ang_dt = 0.01f; s2.delta_ang_dt = 0.01f;
        std::vector<ImuSample> v = {s1, s2};
        ImuSample out = downsampleImuSamples(v, 100000);
        assert(out.delta_vel_clipping[2] == true);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
