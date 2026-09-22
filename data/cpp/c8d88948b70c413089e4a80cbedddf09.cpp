// Given a 3D unstructured mesh file path and a maximum multigrid level, write a C++ function that sets up a P2 finite element Laplace operator on a scalar finite element space (with both vertex and edge degrees of freedom), initializes a solution field with a random interior guess and a known exact solution on Dirichlet boundaries, sets the right-hand side to zero, and performs a fixed number of V-cycle geometric multigrid iterations using Gauss–Seidel smoothing and a conjugate gradient coarse solver. After each V-cycle, the function must compute the residual (operator applied to current solution) and the error against the exact solution, both normalized by the number of interior degrees of freedom, and return the convergence rate (residual ratio between consecutive iterations) after the final V-cycle. The function must verify that each convergence rate is below a given tolerance (e.g., 0.03) by returning a boolean success flag, and optionally write VTK output files for visualization.
The main algorithm is a geometric multigrid V-cycle solver for the P2 constant Laplace operator on a 3D unstructured mesh. The setup involves: (1) reading the mesh and constructing a `PrimitiveStorage` with Dirichlet boundary flags; (2) creating a `P2ConstantLaplaceOperator` for levels 0 to maxLevel; (3) defining the exact solution as `sin(x)*sinh(y)*z` and zero right-hand side; (4) interpolating the random initial guess on interior DoFs and the exact solution on Dirichlet boundary DoFs; (5) configuring a Gauss–Seidel smoother, CG coarse solver, and quadratic prolongation/restriction operators; (6) constructing a `GeometricMultigridSolver` with 2 pre- and post-smoothing steps; (7) performing the specified number of V-cycles, where each iteration applies the solver, computes the residual and error norms (normalized by the number of interior points), and checks the residual convergence rate against the tolerance. Edge cases include handling empty meshes, ensuring the mesh is 3D with cells, and managing boundary flags correctly. Time complexity is dominated by the multigrid iterations, typically \(O(N \log N)\) for \(N\) degrees of freedom on the finest level due to grid transfers, while space complexity is \(O(N)\) for storing the P2 functions and operator matrices.
#include <memory>
#include <functional>
#include <cmath>
#include <cassert>

#include "core/Environment.h"
#include "core/math/Random.h"
#include "core/timing/Timer.h"
#include "core/logging/Logging.h"

#include "hyteg/p2functionspace/P2Function.hpp"
#include "hyteg/p2functionspace/P2ConstantOperator.hpp"
#include "hyteg/gridtransferoperators/P2toP2QuadraticProlongation.hpp"
#include "hyteg/gridtransferoperators/P2toP2QuadraticRestriction.hpp"
#include "hyteg/solvers/CGSolver.hpp"
#include "hyteg/solvers/EmptySolver.hpp"
#include "hyteg/solvers/GeometricMultigridSolver.hpp"
#include "hyteg/solvers/GaussSeidelSmoother.hpp"
#include "hyteg/primitivestorage/SetupPrimitiveStorage.hpp"
#include "hyteg/primitivestorage/PrimitiveStorage.hpp"
#include "hyteg/primitivestorage/Visualization.hpp"
#include "hyteg/dataexport/VTKOutput.hpp"

using walberla::real_t;
using walberla::uint_t;

// Run a P2 geometric multigrid V-cycle convergence test on a 3D mesh.
// Returns true if all residual convergence rates are below the given tolerance.
// The final convergence rate is returned via the optional output parameter.
bool runP2MultigridConvergence(
    const std::string& meshFile,
    const uint_t minLevel,
    const uint_t maxLevel,
    const uint_t numVCycles,
    const bool writeVTK,
    const real_t tolerance,
    real_t* finalConvRate = nullptr)
{
    // Load the mesh and set up the storage
    const auto meshInfo = hyteg::MeshInfo::fromGmshFile(meshFile);
    hyteg::SetupPrimitiveStorage setupStorage(meshInfo, uint_c(walberla::MPIManager::instance()->numProcesses()));
    setupStorage.setMeshBoundaryFlagsOnBoundary(1, 0, true);
    const auto storage = std::make_shared<hyteg::PrimitiveStorage>(setupStorage);

    WALBERLA_CHECK(storage->hasGlobalCells());

    if (writeVTK) {
        hyteg::writeDomainPartitioningVTK(storage, "../../output", "P2_GMG_convergence_partitioning");
    }

    // Create the Laplace operator
    hyteg::P2ConstantLaplaceOperator laplaceOperator(storage, minLevel, maxLevel);

    // Define exact solution and helper functions
    std::function<real_t(const hyteg::Point3D&)> exact = [](const hyteg::Point3D& p) -> real_t {
        return std::sin(p[0]) * std::sinh(p[1]) * p[2];
    };
    std::function<real_t(const hyteg::Point3D&)> zero = [](const hyteg::Point3D&) -> real_t {
        return 0.0;
    };
    std::function<real_t(const hyteg::Point3D&)> one = [](const hyteg::Point3D&) -> real_t {
        return 1.0;
    };
    std::function<real_t(const hyteg::Point3D&)> rand = [](const hyteg::Point3D&) -> real_t {
        return walberla::math::realRandom(0.0, 1.0);
    };

    // Allocate P2 functions
    hyteg::P2Function<real_t> res("res", storage, minLevel, maxLevel);
    hyteg::P2Function<real_t> f("f", storage, minLevel, maxLevel);
    hyteg::P2Function<real_t> u("u", storage, minLevel, maxLevel);
    hyteg::P2Function<real_t> uExact("uExact", storage, minLevel, maxLevel);
    hyteg::P2Function<real_t> err("err", storage, minLevel, maxLevel);
    hyteg::P2Function<real_t> oneFunction("one", storage, minLevel, maxLevel);

    // Initialize fields
    u.interpolate(rand, maxLevel, hyteg::DoFType::Inner);
    u.interpolate(exact, maxLevel, hyteg::DoFType::DirichletBoundary);
    f.interpolate(zero, maxLevel, hyteg::DoFType::All);
    res.interpolate(zero, maxLevel, hyteg::DoFType::All);
    uExact.interpolate(exact, maxLevel, hyteg::DoFType::All);
    oneFunction.interpolate(one, maxLevel, hyteg::DoFType::All);

    // Set up multigrid components
    auto smoother = std::make_shared<hyteg::GaussSeidelSmoother<hyteg::P2ConstantLaplaceOperator>>();
    auto coarseGridSolver = std::make_shared<hyteg::CGSolver<hyteg::P2ConstantLaplaceOperator>>(storage, minLevel, maxLevel);
    auto restriction = std::make_shared<hyteg::P2toP2QuadraticRestriction>();
    auto prolongation = std::make_shared<hyteg::P2toP2QuadraticProlongation>();

    auto gmgSolver = hyteg::GeometricMultigridSolver<hyteg::P2ConstantLaplaceOperator>(
        storage, smoother, coarseGridSolver, restriction, prolongation, minLevel, maxLevel, 2, 2);

    // Normalization factor: number of interior DoFs on maxLevel
    const real_t numPoints = oneFunction.dotGlobal(oneFunction, maxLevel, hyteg::DoFType::Inner);

    // optional VTK output
    hyteg::VTKOutput vtkOutput("../../output", "P2GMGConvergenceTest", storage);
    vtkOutput.add(u);
    vtkOutput.add(err);
    if (writeVTK) {
        vtkOutput.write(maxLevel, 0);
    }

    // Initial residual and error
    err.assign({1.0, -1.0}, {u, uExact}, maxLevel);
    laplaceOperator.apply(u, res, maxLevel, hyteg::DoFType::Inner);
    real_t discrL2Res = res.dotGlobal(res, maxLevel, hyteg::DoFType::Inner) / numPoints;
    real_t lastResidual = discrL2Res;
    real_t currentConvRate = 1.0;

    // Run V-cycles
    for (uint_t i = 1; i <= numVCycles; ++i) {
        gmgSolver.solve(laplaceOperator, u, f, maxLevel);
        err.assign({1.0, -1.0}, {u, uExact}, maxLevel);
        laplaceOperator.apply(u, res, maxLevel, hyteg::DoFType::Inner);

        if (writeVTK) {
            vtkOutput.write(maxLevel, i);
        }

        const real_t discrL2Err = err.dotGlobal(err, maxLevel, hyteg::DoFType::Inner) / numPoints;
        discrL2Res = res.dotGlobal(res, maxLevel, hyteg::DoFType::Inner) / numPoints;
        currentConvRate = discrL2Res / lastResidual;
        lastResidual = discrL2Res;

        WALBERLA_LOG_INFO_ON_ROOT("After " << i << " VCycles: Residual: " << std::scientific
            << discrL2Res << " | convRate: " << currentConvRate << " | Error L2: " << discrL2Err);

        if (currentConvRate >= tolerance) {
            if (finalConvRate != nullptr) {
                *finalConvRate = currentConvRate;
            }
            return false;
        }
    }

    if (finalConvRate != nullptr) {
        *finalConvRate = currentConvRate;
    }
    return true;
}
#include <cassert>
#include <string>
#include "core/Environment.h"

// Forward declaration of the solution function
bool runP2MultigridConvergence(
    const std::string& meshFile,
    const uint_t minLevel,
    const uint_t maxLevel,
    const uint_t numVCycles,
    const bool writeVTK,
    const real_t tolerance,
    real_t* finalConvRate = nullptr);

int main() {
    walberla::Environment walberlaEnv(1, nullptr);
    walberla::MPIManager::instance()->useWorldComm();

    // Test 1: Run convergence test with tight tolerance on a small mesh
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 3, 10, false, 3.0e-02, &finalRate);
        assert(success);
        assert(finalRate < 3.0e-02);
    }

    // Test 2: Fewer V-cycles should still converge but with looser tolerance
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 2, 5, false, 1.0e-01, &finalRate);
        assert(success);
        assert(finalRate < 1.0e-01);
    }

    // Test 3: More V-cycles should yield even faster convergence
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 3, 20, false, 1.0e-03, &finalRate);
        assert(success);
        assert(finalRate < 1.0e-03);
    }

    // Test 4: With only one V-cycle, convergence may still be acceptably slow
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 2, 1, false, 2.0e-01, &finalRate);
        assert(success);
        assert(finalRate < 2.0e-01);
    }

    // Test 5: Zero tolerance should fail (unless perfect convergence impossible)
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 2, 5, false, 0.0, &finalRate);
        assert(!success); // Will never reach zero residual exactly
    }

    // Test 6: Very loose tolerance always passes
    {
        real_t finalRate = 0.0;
        bool success = runP2MultigridConvergence(
            "../../data/meshes/3D/regular_octahedron_8el.msh",
            0, 2, 3, false, 5.0, &finalRate);
        assert(success);
        assert(finalRate <= 5.0);
    }

    return 0;
}
