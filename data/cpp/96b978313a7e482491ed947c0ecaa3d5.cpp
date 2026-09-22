/*
Design a C++ function that simulates the core IMU pre-integration and state prediction logic from the given VINS-Mono code snippet, but in a standalone, self-contained manner without ROS dependencies. The function should take a sequence of IMU measurements (each containing timestamp, linear acceleration, and angular velocity) and an initial state (position, orientation as quaternion, velocity, accelerometer bias, gyroscope bias, and gravity vector), and return the predicted final state after processing all measurements using the mid-point integration method exactly as shown in the `predict` function. Specifically, implement the update equations: first compute `un_acc_0` using the previous orientation and bias-corrected acceleration, then update orientation using the average gyroscope, compute `un_acc_1` with the new orientation, average the two accelerations, and finally update position and velocity. The function must handle varying time steps between consecutive IMU messages and assume the first measurement's timestamp is the initial time. Edge cases include an empty input sequence (return initial state unchanged), constant zero acceleration/gyroscope (resulting in uniform motion), and a single IMU measurement (valid dt computation). The function must use `Eigen::Vector3d` for vectors and `Eigen::Quaterniond` for orientation, and ensure const-correctness for inputs.
*/

#include <Eigen/Dense>
#include <vector>
#include <cassert>

// IMU measurement structure
struct ImuMeasurement {
    double timestamp;
    Eigen::Vector3d linear_acceleration;
    Eigen::Vector3d angular_velocity;
};

// State structure
struct State {
    Eigen::Vector3d position;
    Eigen::Quaterniond orientation;
    Eigen::Vector3d velocity;
    Eigen::Vector3d accel_bias;
    Eigen::Vector3d gyro_bias;
    Eigen::Vector3d gravity;
};

// Helper to create a quaternion from angular velocity * dt (deltaQ)
Eigen::Quaterniond deltaQ(const Eigen::Vector3d& omega) {
    double angle = omega.norm();
    if (angle < 1e-12) {
        return Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
    }
    Eigen::Vector3d axis = omega / angle;
    double half_angle = angle * 0.5;
    double s = std::sin(half_angle);
    return Eigen::Quaterniond(std::cos(half_angle), axis.x() * s, axis.y() * s, axis.z() * s);
}

// Convert quaternion to rotation matrix (for ease, though not strictly needed)
Eigen::Matrix3d quatToRot(const Eigen::Quaterniond& q) {
    return q.toRotationMatrix();
}

// Function to predict the final state given a sequence of IMU measurements and initial state
State predictIMUState(const std::vector<ImuMeasurement>& imu_measurements, const State& initial_state) {
    if (imu_measurements.empty()) {
        return initial_state;
    }

    State state = initial_state;
    double latest_time = imu_measurements[0].timestamp;
    
    // Initialize previous acc_0 and gyr_0 from the first measurement
    Eigen::Vector3d acc_0 = imu_measurements[0].linear_acceleration;
    Eigen::Vector3d gyr_0 = imu_measurements[0].angular_velocity;

    for (const auto& imu_msg : imu_measurements) {
        double t = imu_msg.timestamp;
        double dt = t - latest_time;
        if (dt <= 0.0) {
            // Skip if dt is non-positive (same timestamp or out-of-order)
            continue;
        }
        latest_time = t;

        Eigen::Vector3d linear_acceleration = imu_msg.linear_acceleration;
        Eigen::Vector3d angular_velocity = imu_msg.angular_velocity;

        // Compute un_acc_0: previous orientation * (acc_0 - ba - Q^T * g)
        Eigen::Vector3d un_acc_0 = state.orientation * (acc_0 - state.accel_bias - state.orientation.inverse() * state.gravity);

        // Average gyroscope and update orientation
        Eigen::Vector3d un_gyr = 0.5 * (gyr_0 + angular_velocity) - state.gyro_bias;
        state.orientation = state.orientation * deltaQ(un_gyr * dt);
        state.orientation.normalize();

        // Compute un_acc_1 with new orientation
        Eigen::Vector3d un_acc_1 = state.orientation * (linear_acceleration - state.accel_bias - state.orientation.inverse() * state.gravity);

        // Average acceleration
        Eigen::Vector3d un_acc = 0.5 * (un_acc_0 + un_acc_1);

        // Update position and velocity
        state.position = state.position + dt * state.velocity + 0.5 * dt * dt * un_acc;
        state.velocity = state.velocity + dt * un_acc;

        // Store current measurements as previous
        acc_0 = linear_acceleration;
        gyr_0 = angular_velocity;
    }

    return state;
}

#include <Eigen/Dense>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

// Include the solution code here (in a real test, you'd include the header)

int main() {
    // Test 1: Empty sequence returns initial state
    {
        State init;
        init.position = Eigen::Vector3d(1.0, 2.0, 3.0);
        init.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
        init.velocity = Eigen::Vector3d(0.1, 0.2, 0.3);
        init.accel_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gyro_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gravity = Eigen::Vector3d(0.0, 0.0, -9.81);
        std::vector<ImuMeasurement> imu;
        State result = predictIMUState(imu, init);
        assert(result.position.isApprox(init.position, 1e-9));
        assert(result.orientation.coeffs().isApprox(init.orientation.coeffs(), 1e-9));
        assert(result.velocity.isApprox(init.velocity, 1e-9));
    }

    // Test 2: Constant zero acceleration and zero gyro, with gravity bias removed, should drift at constant velocity
    {
        State init;
        init.position = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
        init.velocity = Eigen::Vector3d(1.0, 0.0, 0.0);
        init.accel_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gyro_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gravity = Eigen::Vector3d(0.0, 0.0, -9.81);
        
        // After zeroing the gravity effect by setting accel to (0,0,9.81) (i.e., accelerometer measures specific force opposite gravity)
        // Actually, the model subtracts gravity from accel, so if accel = (0,0,9.81), then un_acc = Q*(acc - ba) - g? Wait, code subtracts g inside: Q*(acc - ba - Q^T*g) => Q*(acc - ba) - g
        // To get zero acceleration, set accel = (0,0,0) and gravity = (0,0,9.81) so Q*(0 - 0 - Q^T*0) = 0? Let's just set gravity = 0 for simplicity.
        init.gravity = Eigen::Vector3d(0.0, 0.0, 0.0);
        
        std::vector<ImuMeasurement> imu;
        imu.push_back({0.0, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0)});
        imu.push_back({1.0, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0)});
        State result = predictIMUState(imu, init);
        // After 1 second, velocity stays 1.0, position moves by 1.0
        assert(std::fabs(result.velocity.x() - 1.0) < 1e-9);
        assert(std::fabs(result.position.x() - 1.0) < 1e-9);
        assert(result.orientation.coeffs().isApprox(init.orientation.coeffs(), 1e-9));
    }

    // Test 3: Constant acceleration in x direction with no gravity, should produce quadratic motion
    {
        State init;
        init.position = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
        init.velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.accel_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gyro_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gravity = Eigen::Vector3d(0.0, 0.0, 0.0);
        
        std::vector<ImuMeasurement> imu;
        // 2 seconds, 1 measurement per second, acceleration = 2 m/s^2 in x
        imu.push_back({0.0, Eigen::Vector3d(2.0, 0.0, 0.0), Eigen::Vector3d(0,0,0)});
        imu.push_back({1.0, Eigen::Vector3d(2.0, 0.0, 0.0), Eigen::Vector3d(0,0,0)});
        State result = predictIMUState(imu, init);
        // Mid-point integration over two 1s intervals: After first dt, v = 2, p = 1; after second dt, v = 4, p = 1 + 2*1 + 0.5*2*1 = 4? Actually compute: 
        // For dt1=1: un_acc0 = 2 (acc0 - 0 - Q^T*0), un_acc1 = 2, un_acc=2, V=2, P=0+0+0.5*2*1=1
        // For dt2=1: acc_0=2, un_acc0=2, un_acc1=2, un_acc=2, V=2+2=4, P=1 + 1*2 + 0.5*2*1=4
        assert(std::fabs(result.velocity.x() - 4.0) < 1e-9);
        assert(std::fabs(result.position.x() - 4.0) < 1e-9);
    }

    // Test 4: Single measurement with dt=1, checks update formulas manually
    {
        State init;
        init.position = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
        init.velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.accel_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gyro_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gravity = Eigen::Vector3d(0.0, 0.0, -9.81);
        
        std::vector<ImuMeasurement> imu;
        // IMU measures specific force = (0,0,9.81), which cancels gravity, so no acceleration in world frame
        imu.push_back({0.0, Eigen::Vector3d(0.0, 0.0, 9.81), Eigen::Vector3d(0.0, 0.0, 0.0)});
        State result = predictIMUState(imu, init);
        // Since there is only one measurement, no dt is applied (dt from latest_time=0 to t=0 is 0), so state unchanged
        assert(result.position.isApprox(init.position, 1e-9));
        assert(result.orientation.coeffs().isApprox(init.orientation.coeffs(), 1e-9));
    }

    // Test 5: Rotation around z-axis with constant angular velocity, using midpoint integration
    {
        State init;
        init.position = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.orientation = Eigen::Quaterniond(1.0, 0.0, 0.0, 0.0);
        init.velocity = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.accel_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gyro_bias = Eigen::Vector3d(0.0, 0.0, 0.0);
        init.gravity = Eigen::Vector3d(0.0, 0.0, 0.0);
        
        std::vector<ImuMeasurement> imu;
        // Two measurements: at t=0 and t=1, angular velocity = (0,0,1) rad/s
        imu.push_back({0.0, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0.0, 0.0, 1.0)});
        imu.push_back({1.0, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0.0, 0.0, 1.0)});
        State result = predictIMUState(imu, init);
        // First dt=1: un_gyr = 0.5*(0+1)=0.5, applied rotation about z by 0.5 rad
        // Second dt=1: un_gyr = 0.5*(1+1)=1.0, applied rotation about z by 1.0 rad, total 1.5 rad
        double expected_angle = 1.5;
        Eigen::AngleAxisd angle_axis(expected_angle, Eigen::Vector3d::UnitZ());
        Eigen::Quaterniond expected_q(angle_axis);
        assert(result.orientation.coeffs().isApprox(expected_q.coeffs(), 1e-6));
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution should define a struct or simple class to represent the IMU measurement and a struct for the state. The core algorithm follows the mid-point integration: for each IMU message with timestamp `t`, compute `dt = t - latest_time`, update `latest_time = t`. Then using the current state (position `P`, quaternion `Q`, velocity `V`, biases `Ba`, `Bg`, gravity `g`, and previous acceleration `acc_0`, previous gyro `gyr_0`), compute the unbiased acceleration in the world frame using the previous orientation: `un_acc_0 = Q * (acc_0 - Ba - Q.inverse() * g)`. Then average the gyro: `un_gyr = 0.5 * (gyr_0 + angular_velocity) - Bg`, update orientation via `Q = Q * deltaQ(un_gyr * dt)`, where `deltaQ` constructs a quaternion from a 3-vector using the exponential map `(1, 0.5*omega)`. Then compute `un_acc_1 = Q * (linear_acceleration - Ba - Q.inverse() * g)`, average accelerations `un_acc = 0.5 * (un_acc_0 + un_acc_1)`, update position `P += dt * V + 0.5 * dt * dt * un_acc`, velocity `V += dt * un_acc`, and finally store `acc_0 = linear_acceleration`, `gyr_0 = angular_velocity`. Important edge cases: whether `dt` is zero (should skip to avoid division by zero), negative dt (should be handled gracefully or ignored), and initialization of `latest_time` from the first measurement's timestamp. Time complexity is O(n) for n IMU messages, space complexity O(1) ignoring input storage. The function should be `const`-correct, taking a `const std::vector<ImuMeasurement>&` and returning the final state by value or via a reference parameter.
