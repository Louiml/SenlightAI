Write a C++ function that simulates the gravity-driven deformation of a cantilever beam made of isotropic hyperelastic material using the Absolute Nodal Coordinate Formulation (ANCF) with 3-node 8-degree-of-freedom brick elements. The function should take as input the beam length, number of elements along the length, material density, Young's modulus, Poisson's ratio, and a simulation duration, then return the vertical displacement (z-coordinate) of the free tip at the final simulation time. The beam is clamped at one end (x=0), has rectangular cross-section (height=0.1, thickness=0.01), is discretized with ANCF brick elements, and is subjected to gravity (g=-9.81 m/s² in the z-direction). The solution must use Chrono::Engine FEA capabilities, including a ChSystemNSC with HHT time integration, and must compute the deformation with sufficient accuracy using at least 10 elements along the length and appropriate material parameters such that the static deflection is approximately 0.5% of the beam length for the given test case.
// The solution requires carefully constructing a finite element model of a cantilever beam using Chrono's ANCF brick elements. The key steps are: (1) create a physical system with gravity, (2) define mesh parameters matching the given beam dimensions and discretization (10 elements along length, 1 each in y and z), (3) generate node coordinates systematically: x varies from 0 to length in increments of dx, y alternates between 0 and dy in a pattern that creates a structured brick grid, z is either 0 or dz depending on the node's row, (4) identify and fix nodes where x=0 (the clamp), (5) create ChNodeFEAxyz nodes and ChElementHexaANCF_3813 elements with proper node ordering, (6) assign material properties (density, Young's modulus, Poisson's ratio) using ChContinuumElastic, (7) set up an HHT integrator with alpha=-0.01, and (8) advance the simulation step-by-step until the specified duration, tracking the tip node's z-coordinate. Important edge cases: the node numbering scheme must correctly map the 8-node brick connectivity (the index pattern in the original code must be replicated), the y-coordinate assignment alternates based on integer division, and the tip node is identified as the last node in the mesh. Time complexity is O(steps × elements × solver_iterations) since each time step requires solving the nonlinear FEA system, and space complexity is O(elements + nodes) for storing mesh data.
#include <chrono/physics/ChSystemNSC.h>
#include <chrono/solver/ChIterativeSolverLS.h>
#include <chrono/fea/ChElementHexaANCF_3813.h>
#include <chrono/fea/ChContinuumElastic.h>
#include <chrono/fea/ChNodeFEAxyz.h>
#include <chrono/fea/ChMesh.h>
#include <chrono/timestepper/ChTimestepper.h>
#include <chrono/solver/ChSolverMINRES.h>
#include <memory>
#include <cmath>
#include <stdexcept>

// Simulate gravity deformation of a cantilever beam and return tip z-displacement
double simulateCantileverDeflection(
    double beam_length,      // Beam length in x-direction (m)
    int num_elements_x,      // Number of elements along length
    double density,          // Material density (kg/m^3)
    double young_modulus,    // Young's modulus (Pa)
    double poisson_ratio,    // Poisson's ratio
    double sim_time          // Total simulation time (s)
) {
    // Validate inputs
    if (beam_length <= 0 || num_elements_x < 1 || density <= 0 || 
        young_modulus <= 0 || poisson_ratio < 0 || poisson_ratio >= 0.5 || sim_time <= 0) {
        throw std::invalid_argument("Invalid simulation parameters");
    }

    // Beam cross-section dimensions (fixed as per original problem)
    const double beam_height_y = 0.1;  // y-dimension
    const double beam_thickness_z = 0.01;  // z-dimension

    // Discretization
    const int num_div_y = 1;
    const int num_div_z = 1;
    const int N_x = num_elements_x + 1;

    // Total number of elements and nodes
    const int total_elements = num_elements_x;
    const int total_nodes = (num_elements_x + 1) * 4;  // 4 nodes per brick cross-section

    // Element dimensions
    const double dx = beam_length / num_elements_x;
    const double dy = beam_height_y / num_div_y;
    const double dz = beam_thickness_z / num_div_z;

    // Create physical system with gravity
    chrono::ChSystemNSC sys;
    sys.SetGravitationalAcceleration(chrono::ChVector3d(0, 0, -9.81));

    // Create mesh
    auto my_mesh = chrono::chrono_types::make_shared<chrono::fea::ChMesh>();

    // Material properties
    auto mmaterial = chrono::chrono_types::make_shared<chrono::fea::ChContinuumElastic>();
    mmaterial->SetDensity(density);
    mmaterial->SetYoungModulus(young_modulus);
    mmaterial->SetPoissonRatio(poisson_ratio);
    mmaterial->SetRayleighDampingAlpha(0.0);
    mmaterial->SetRayleighDampingBeta(0.0);

    // Create nodes
    std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes;
    nodes.reserve(total_nodes);

    for (int i = 0; i < total_nodes; ++i) {
        double coord_x = (i % N_x) * dx;
        double coord_y = ((i / N_x + 1) % 2 == 0) ? dy : 0.0;
        double coord_z = (i / (N_x * 2)) * dz;

        auto node = chrono::chrono_types::make_shared<chrono::fea::ChNodeFEAxyz>(
            chrono::ChVector3d(coord_x, coord_y, coord_z));
        node->SetMass(0.0);
        
        // Fix nodes at x=0 (clamped end)
        if (i % N_x == 0) {
            node->SetFixed(true);
        }
        
        my_mesh->AddNode(node);
        nodes.push_back(node);
    }

    // Tip node is the last node in the mesh
    auto nodetip = nodes.back();

    // Create elements
    for (int elem = 0; elem < total_elements; ++elem) {
        auto element = chrono::chrono_types::make_shared<chrono::fea::ChElementHexaANCF_3813>();
        
        // Set element dimensions
        chrono::ChVectorN<double, 3> InertFlexVec;
        InertFlexVec.setZero();
        InertFlexVec(0) = dx;
        InertFlexVec(1) = dy;
        InertFlexVec(2) = dz;
        element->SetInertFlexVec(InertFlexVec);

        // Define node connectivity for this 8-node brick
        int n0 = elem;
        int n1 = elem + 1;
        int n2 = elem + 1 + N_x;
        int n3 = elem + N_x;
        int n4 = elem + 2 * N_x;
        int n5 = elem + 2 * N_x + 1;
        int n6 = elem + 3 * N_x + 1;
        int n7 = elem + 3 * N_x;

        element->SetNodes(nodes[n0], nodes[n1], nodes[n2], nodes[n3],
                          nodes[n4], nodes[n5], nodes[n6], nodes[n7]);
        element->SetMaterial(mmaterial);
        element->SetMooneyRivlin(false);

        // Zero out EAS stabilization parameters
        chrono::ChVectorN<double, 9> alpha;
        alpha.setZero();
        element->SetStockAlpha(alpha(0), alpha(1), alpha(2), alpha(3), alpha(4),
                               alpha(5), alpha(6), alpha(7), alpha(8));

        my_mesh->AddElement(element);
    }

    // Add mesh to system
    sys.Add(my_mesh);

    // Solver setup (MINRES for robustness without MKL)
    auto solver = chrono::chrono_types::make_shared<chrono::ChSolverMINRES>();
    sys.SetSolver(solver);
    solver->SetMaxIterations(200);
    solver->SetTolerance(1e-10);
    solver->EnableDiagonalPreconditioner(true);
    solver->SetVerbose(false);

    // Time integration (HHT)
    sys.SetTimestepperType(chrono::ChTimestepper::Type::HHT);
    auto mystepper = std::static_pointer_cast<chrono::ChTimestepperHHT>(sys.GetTimestepper());
    mystepper->SetAlpha(-0.01);
    mystepper->SetMaxIters(10000);
    mystepper->SetAbsTolerances(1e-5);

    // Simulation step size
    const double step_size = 1e-3;

    // Run simulation
    int num_steps = static_cast<int>(sim_time / step_size);
    for (int step = 0; step < num_steps; ++step) {
        sys.DoStepDynamics(step_size);
    }

    // Return tip z-displacement (position after deformation)
    return nodetip->GetPos().z();
}
#include <cassert>
#include <cmath>
#include <iostream>

// Forward declaration of the solution function
double simulateCantileverDeflection(
    double beam_length, int num_elements_x, double density,
    double young_modulus, double poisson_ratio, double sim_time);

int main() {
    // Test case 1: Classic cantilever with given parameters (from original code)
    double deflection = simulateCantileverDeflection(1.0, 10, 500.0, 2.1e7, 0.3, 2.0);
    // Expected static deflection ~0.005 m (±5% tolerance)
    assert(std::abs(deflection - (-0.005)) < 0.00025);
    std::cout << "Test 1 passed: deflection = " << deflection << "\n";

    // Test case 2: Stiffer beam (higher Young's modulus) deflects less
    double deflection_stiff = simulateCantileverDeflection(1.0, 10, 500.0, 2.1e8, 0.3, 2.0);
    assert(std::abs(deflection_stiff) < std::abs(deflection));
    std::cout << "Test 2 passed: stiff beam deflects less\n";

    // Test case 3: Longer beam deflects more for same material
    double deflection_long = simulateCantileverDeflection(2.0, 10, 500.0, 2.1e7, 0.3, 2.0);
    assert(std::abs(deflection_long) > std::abs(deflection));
    std::cout << "Test 3 passed: longer beam deflects more\n";

    // Test case 4: Zero-length beam should produce zero deflection (or near zero)
    double deflection_zero = simulateCantileverDeflection(0.01, 1, 500.0, 2.1e7, 0.3, 1.0);
    assert(std::abs(deflection_zero) < 1e-4);
    std::cout << "Test 4 passed: near-zero length beam deflects negligibly\n";

    // Test case 5: Different Poisson ratio (0.0) should still work
    double deflection_nu0 = simulateCantileverDeflection(1.0, 10, 500.0, 2.1e7, 0.0, 2.0);
    assert(std::isfinite(deflection_nu0));
    std::cout << "Test 5 passed: Poisson ratio 0 produces finite result\n";

    // Test case 6: More elements should converge to the static solution
    double deflection_fine = simulateCantileverDeflection(1.0, 20, 500.0, 2.1e7, 0.3, 2.0);
    assert(std::abs(deflection_fine - deflection) < 0.001);
    std::cout << "Test 6 passed: finer mesh converges\n";

    // Test case 7: Very short simulation time still produces near-zero deflection
    double deflection_short = simulateCantileverDeflection(1.0, 10, 500.0, 2.1e7, 0.3, 0.01);
    assert(deflection_short > -0.001);
    std::cout << "Test 7 passed: short time gives small deflection\n";

    // Test case 8: Invalid inputs throw exceptions
    bool threw = false;
    try {
        simulateCantileverDeflection(0, 10, 500, 2.1e7, 0.3, 2.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    std::cout << "Test 8 passed: invalid length throws\n";

    std::cout << "All tests passed!\n";
    return 0;
}
