Write a standalone C++ function named `computeGranularContactForce` that models a simplified version of the granular Hookean contact law with history-dependent tangential friction between two spherical particles. The function should take the following inputs: positions `x_i[3]` and `x_j[3]`, velocities `v_i[3]` and `v_j[3]`, angular velocities `omega_i[3]` and `omega_j[3]` as `std::array<double,3>` (or C-style arrays), radii `r_i` and `r_j`, masses `m_i` and `m_j`, a time step `dt`, and the material parameters: normal stiffness `kn`, tangential stiffness `kt`, normal damping coefficient `gamman`, tangential damping coefficient `gammat`, and friction coefficient `mu`. Additionally, pass a reference to a shear displacement history vector `shear[3]` (which is updated in place), and a boolean `shear_update` indicating whether shear history should be updated (set to `false` for the first call after initialization). The function should return a `std::array<double,6>` containing the force vector on particle i `{fx, fy, fz}` and the torque vector on particle i `{tx, ty, tz}` (with torques computed about the particle center). The interaction uses: normal force `fn = kn * overlap - meff * gamman * vn` where `overlap = r_i + r_j - distance`, `vn` is the normal relative velocity, and `meff = m_i*m_j/(m_i+m_j)`. The tangential force is `fs = -kt * shear - meff * gammat * vt` where `vt` is the tangential relative velocity at the contact point (including rotational contributions). Apply Coulomb friction limit: if `|fs| > mu * |fn|`, scale both the tangential force and the shear history accordingly. Update the shear history by adding `vt * dt`, then rotate it to remain perpendicular to the normal direction. If `shear_update` is false, skip the update but still compute forces. If particles are not overlapping (distance >= r_i + r_j), return zero forces/torques, and zero out the shear history.
The solution involves decomposing the relative motion into normal and tangential components at the contact point. First compute the center-to-center displacement vector and its magnitude. If the distance is greater than or equal to the sum of radii, there is no contact: return zeros and reset shear. Otherwise, compute the overlap and the unit normal vector. Compute relative translational velocity and split into normal and tangential parts. The tangential relative velocity also includes the effect of both particles spinning: `vt_contact = vt_translational + (r_i * omega_i + r_j * omega_j) cross normal` (with appropriate sign conventions). The effective mass is used for damping. The normal force is a combination of a linear spring (Hooke) and a dashpot. The tangential force uses a history-dependent spring (the shear displacement) plus a dashpot. After updating shear (if enabled), rotate the shear vector to be perpendicular to the normal to avoid spurious tangential forces due to rotation of the contact frame. Then apply the Coulomb friction limit: if the magnitude of the tangential force exceeds `mu * |normal_force|`, scale the tangential force down to the limiting magnitude and also adjust the shear history so that future increments remain consistent. The torque is computed as the cross product of the contact vector (from center to contact point) with the tangential force: `torque = -r_i * (normal cross fs)` for particle i, and the opposite for particle j (but only i is returned). Complexity is O(1) time and O(1) extra space. Edge cases include zero relative normal velocity, zero tangential relative velocity (no shear update), and the first call where shear may be zero.
#include <array>
#include <cmath>

// Compute force and torque on particle i due to contact with particle j.
// Shear history is updated in place if shear_update is true.
// Returns {fx, fy, fz, tx, ty, tz} for particle i.
std::array<double, 6> computeGranularContactForce(
    const std::array<double, 3>& x_i, const std::array<double, 3>& x_j,
    const std::array<double, 3>& v_i, const std::array<double, 3>& v_j,
    const std::array<double, 3>& omega_i, const std::array<double, 3>& omega_j,
    double r_i, double r_j, double m_i, double m_j, double dt,
    double kn, double kt, double gamman, double gammat, double mu,
    std::array<double, 3>& shear, bool shear_update) {

    // Displacement from j to i
    double dx = x_i[0] - x_j[0];
    double dy = x_i[1] - x_j[1];
    double dz = x_i[2] - x_j[2];
    double dist_sq = dx*dx + dy*dy + dz*dz;
    double dist = std::sqrt(dist_sq);
    double rad_sum = r_i + r_j;

    // No contact condition
    if (dist >= rad_sum) {
        shear = {0.0, 0.0, 0.0};
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    // Unit normal vector from j to i
    double inv_dist = 1.0 / dist;
    double nx = dx * inv_dist;
    double ny = dy * inv_dist;
    double nz = dz * inv_dist;

    // Relative translational velocity (i - j)
    double vr_x = v_i[0] - v_j[0];
    double vr_y = v_i[1] - v_j[1];
    double vr_z = v_i[2] - v_j[2];

    // Normal component of relative velocity
    double vn = vr_x*nx + vr_y*ny + vr_z*nz;
    double vn_x = vn * nx;
    double vn_y = vn * ny;
    double vn_z = vn * nz;

    // Tangential relative velocity (translational component)
    double vt_x = vr_x - vn_x;
    double vt_y = vr_y - vn_y;
    double vt_z = vr_z - vn_z;

    // Relative rotational velocity contribution
    // vr_rot = (r_i*w_i + r_j*w_j) cross n
    double rot_x = (r_i*omega_i[1] + r_j*omega_j[1]) * nz - (r_i*omega_i[2] + r_j*omega_j[2]) * ny;
    double rot_y = (r_i*omega_i[2] + r_j*omega_j[2]) * nx - (r_i*omega_i[0] + r_j*omega_j[0]) * nz;
    double rot_z = (r_i*omega_i[0] + r_j*omega_j[0]) * ny - (r_i*omega_i[1] + r_j*omega_j[1]) * nx;

    // Total tangential relative velocity at contact
    vt_x -= rot_x;
    vt_y -= rot_y;
    vt_z -= rot_z;

    // Effective mass
    double meff = m_i * m_j / (m_i + m_j);

    // Overlap
    double overlap = rad_sum - dist;

    // Normal force magnitude (spring + dashpot)
    double fn_mag = kn * overlap - meff * gamman * vn;
    if (fn_mag < 0.0) fn_mag = 0.0;  // no tensile forces
    double fn_x = fn_mag * nx;
    double fn_y = fn_mag * ny;
    double fn_z = fn_mag * nz;

    // Update shear history if requested
    if (shear_update) {
        // Add incremental tangential displacement
        shear[0] += vt_x * dt;
        shear[1] += vt_y * dt;
        shear[2] += vt_z * dt;

        // Rotate shear to be perpendicular to normal (remove radial component)
        double dot = shear[0]*nx + shear[1]*ny + shear[2]*nz;
        shear[0] -= dot * nx;
        shear[1] -= dot * ny;
        shear[2] -= dot * nz;
    }

    // Tangential force (spring + dashpot)
    double fs_x = -(kt * shear[0] + meff * gammat * vt_x);
    double fs_y = -(kt * shear[1] + meff * gammat * vt_y);
    double fs_z = -(kt * shear[2] + meff * gammat * vt_z);

    // Coulomb friction limit
    double fs_mag = std::sqrt(fs_x*fs_x + fs_y*fs_y + fs_z*fs_z);
    double fn_for_friction = mu * fn_mag;
    if (fs_mag > fn_for_friction && fs_mag > 0.0) {
        double scale = fn_for_friction / fs_mag;
        fs_x *= scale;
        fs_y *= scale;
        fs_z *= scale;

        // Also scale the shear history to keep consistency
        // First remove the damping part from shear, then scale, then re-add
        if (shear_update) {
            double damping_term_x = meff * gammat * vt_x / kt;
            double damping_term_y = meff * gammat * vt_y / kt;
            double damping_term_z = meff * gammat * vt_z / kt;
            shear[0] = scale * (shear[0] + damping_term_x) - damping_term_x;
            shear[1] = scale * (shear[1] + damping_term_y) - damping_term_y;
            shear[2] = scale * (shear[2] + damping_term_z) - damping_term_z;
        }
    }

    // Total force on particle i
    double fx = fn_x + fs_x;
    double fy = fn_y + fs_y;
    double fz = fn_z + fs_z;

    // Torque on particle i: tau_i = -r_i * (n cross fs)
    // n cross fs
    double cross_x = ny*fs_z - nz*fs_y;
    double cross_y = nz*fs_x - nx*fs_z;
    double cross_z = nx*fs_y - ny*fs_x;
    double tx = -r_i * cross_x;
    double ty = -r_i * cross_y;
    double tz = -r_i * cross_z;

    return {fx, fy, fz, tx, ty, tz};
}
#include <cassert>
#include <cmath>
#include <array>

// Function declaration (copied from solution)
std::array<double, 6> computeGranularContactForce(
    const std::array<double, 3>& x_i, const std::array<double, 3>& x_j,
    const std::array<double, 3>& v_i, const std::array<double, 3>& v_j,
    const std::array<double, 3>& omega_i, const std::array<double, 3>& omega_j,
    double r_i, double r_j, double m_i, double m_j, double dt,
    double kn, double kt, double gamman, double gammat, double mu,
    std::array<double, 3>& shear, bool shear_update);

int main() {
    // Test 1: No contact
    {
        std::array<double,3> xi = {0,0,0}, xj = {3,0,0};
        std::array<double,3> vi = {0,0,0}, vj = {0,0,0};
        std::array<double,3> wi = {0,0,0}, wj = {0,0,0};
        std::array<double,3> shear = {1,2,3};
        auto res = computeGranularContactForce(xi, xj, vi, vj, wi, wj,
                                               1.0, 1.0, 1.0, 1.0, 0.01,
                                               100.0, 50.0, 0.0, 0.0, 0.5,
                                               shear, true);
        assert(res[0] == 0.0 && res[1] == 0.0 && res[2] == 0.0);
        assert(res[3] == 0.0 && res[4] == 0.0 && res[5] == 0.0);
        assert(shear[0] == 0.0 && shear[1] == 0.0 && shear[2] == 0.0);
    }

    // Test 2: Static overlap, no motion, no shear history
    {
        std::array<double,3> xi = {0,0,0}, xj = {1.5,0,0};  // overlap = 0.5
        std::array<double,3> vi = {0,0,0}, vj = {0,0,0};
        std::array<double,3> wi = {0,0,0}, wj = {0,0,0};
        std::array<double,3> shear = {0,0,0};
        double kn_val = 100.0, kt_val = 50.0;
        double gamman_val = 0.0, gammat_val = 0.0;
        auto res = computeGranularContactForce(xi, xj, vi, vj, wi, wj,
                                               1.0, 1.0, 2.0, 2.0, 0.01,
                                               kn_val, kt_val, gamman_val, gammat_val, 0.5,
                                               shear, true);
        // Normal force = kn * overlap = 100 * 0.5 = 50
        // direction from j to i: ( -0.5,0,0 ) / 0.5 = (-1,0,0)?? Wait xi=(0,0,0), xj=(1.5,0,0) => dx=-1.5, nx=-1
        // fn_x = 50 * (-1) = -50
        assert(std::abs(res[0] - (-50.0)) < 1e-9);
        assert(std::abs(res[1]) < 1e-9 && std::abs(res[2]) < 1e-9);
        // No tangential force
        assert(std::abs(res[3]) < 1e-9 && std::abs(res[4]) < 1e-9 && std::abs(res[5]) < 1e-9);
    }

    // Test 3: Tangential motion with shear update
    {
        // Place particles at y=0, x=0 and x=2 (just touching: r=1 each)
        std::array<double,3> xi = {0,0,0}, xj = {2,0,0};
        // i moves upward, j stationary
        std::array<double,3> vi = {0,1,0}, vj = {0,0,0};
        std::array<double,3> wi = {0,0,0}, wj = {0,0,0};
        std::array<double,3> shear = {0,0,0};
        double dt = 0.01;
        double kn_val = 1000.0, kt_val = 500.0;
        double gamman_val = 0.0, gammat_val = 0.0;
        // With no damping and mu = large, force = -kt * shear
        // First call: shear update: shear_y += vt_y * dt = 1*0.01 = 0.01
        // Force_y = -kt * 0.01 = -5.0
        auto res = computeGranularContactForce(xi, xj, vi, vj, wi, wj,
                                               1.0, 1.0, 1.0, 1.0, dt,
                                               kn_val, kt_val, gamman_val, gammat_val, 100.0,
                                               shear, true);
        // Normal force (no overlap since dist=2, rad_sum=2) => fn=0
        // Tangential force: -kt*shear = -500*0.01 = -5.0 in y
        assert(std::abs(res[1] - (-5.0)) < 1e-9);
        assert(std::abs(res[0]) < 1e-9 && std::abs(res[2]) < 1e-9);
        // Shear has been updated
        assert(std::abs(shear[1] - 0.01) < 1e-9);
        // Torque: tau_y? Let's compute: n=( -1,0,0 )? Actually dx=0-2=-2, ny=0, nz=0, nx=-1. fs=(0,-5,0). n cross fs = (ny*fs_z - nz*fs_y, nz*fs_x - nx*fs_z, nx*fs_y - ny*fs_x) = (0,0, (-1)*(-5))=5. tau = -r_i * that = -1*5 = -5.
        assert(std::abs(res[5] - (-5.0)) < 1e-9);
    }

    // Test 4: shear_update false does not modify shear
    {
        std::array<double,3> xi = {0,0,0}, xj = {1.9,0,0}; // overlap 0.1
        std::array<double,3> vi = {0,1,0}, vj = {0,0,0};
        std::array<double,3> wi = {0,0,0}, wj = {0,0,0};
        std::array<double,3> shear = {0.5, 0, 0};  // pre-existing
        std::array<double,3> shear_before = shear;
        auto res = computeGranularContactForce(xi, xj, vi, vj, wi, wj,
                                               1.0, 1.0, 1.0, 1.0, 0.01,
                                               100.0, 50.0, 0.0, 0.0, 100.0,
                                               shear, false);
        // Shear should remain unchanged
        assert(shear[0] == shear_before[0] && shear[1] == shear_before[1] && shear[2] == shear_before[2]);
        // Force uses the old shear
        // tangential force = -kt*shear_x = -50*0.5 = -25 in x
        // But also damping term: vt_y=1, so -gammat*meff*vt_y = 0
        // Since normal has overlap, fn = 100*0.1=10, direction n? dx=-1.9, nx=-1
        // fn_x = -10
        // Total fx = -10 + (-25) = -35
        assert(std::abs(res[0] - (-35.0)) < 1e-9);
    }

    // Test 5: Coulomb friction limit
    {
        std::array<double,3> xi = {0,0,0}, xj = {1.99,0,0}; // overlap 0.01
        std::array<double,3> vi = {0,10,0}, vj = {0,0,0};
        std::array<double,3> wi = {0,0,0}, wj = {0,0,0};
        std::array<double,3> shear = {0,0,0};
        double dt = 0.01;
        double kn_val = 1000.0, kt_val = 100.0;
        double gamman_val = 0.0, gammat_val = 0.0;
        double mu_val = 0.1;
        // Normal force = 1000*0.01 = 10. Limit = 0.1*10 = 1.0
        // Shear after update: shear_y = 10*0.01 = 0.1, force_y = -100*0.1 = -10
        // |fs| = 10 > 1, so scale to 1.0.
        auto res = computeGranularContactForce(xi, xj, vi, vj, wi, wj,
                                               1.0, 1.0, 1.0, 1.0, dt,
                                               kn_val, kt_val, gamman_val, gammat_val, mu_val,
                                               shear, true);
        // Resultant tangential force magnitude should be 1.0
        double fs_mag = std::sqrt(res[1]*res[1]); // only y component
        assert(std::abs(fs_mag - 1.0) < 1e-9);
        // Normal force magnitude = 10 (negative x direction)
        assert(std::abs(res[0] - (-10.0)) < 1e-9);
    }

    return 0;
}
