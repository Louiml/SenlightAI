/*
Write a C++ function that takes a point cloud stored as a `pcl::PCLPointCloud2` object containing XYZ coordinates and normal components (normal_x, normal_y, normal_z) and computes a FPFH (Fast Point Feature Histogram) descriptor for each point. The function should accept two additional parameters: an integer `k` for the number of nearest neighbors and a double `radius` for the search radius (either may be zero, indicating the other should be used exclusively). The function must return a `pcl::PCLPointCloud2` object that contains all the original fields from the input cloud concatenated with the 33-dimensional FPFH signature fields (f1 through f33). The function must validate that the input contains normal information; if not, it should throw a `std::invalid_argument` exception. The implementation should use `pcl::FPFHEstimation` with a kd-tree search method, setting either `setKSearch` or `setRadiusSearch` based on which parameter is non-zero. The input cloud should be passed as a const reference, and the function should be named `computeFPFHDescriptors`. You may include necessary PCL headers and use the `pcl::fromPCLPointCloud2`, `pcl::toPCLPointCloud2`, and `pcl::concatenateFields` utilities for conversion and merging. The function should be self-contained and not require any external entry point.
*/
#include <pcl/PCLPointCloud2.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/features/fpfh.h>
#include <pcl/features/normal_3d.h>  // for PointNormal conversion
#include <pcl/kdtree/kdtree.h>
#include <pcl/search/kdtree.h>
#include <pcl/io/pcd_io.h>
#include <pcl/conversions.h>
#include <stdexcept>

// Compute FPFH descriptors (33 dimensions) and return them concatenated with input fields.
pcl::PCLPointCloud2 computeFPFHDescriptors(const pcl::PCLPointCloud2& input, int k, double radius) {
    // Validate that the input has normal fields
    if (pcl::getFieldIndex(input, "normal_x") == -1 ||
        pcl::getFieldIndex(input, "normal_y") == -1 ||
        pcl::getFieldIndex(input, "normal_z") == -1) {
        throw std::invalid_argument("Input cloud does not contain normal information");
    }

    // Convert to PointCloud<PointNormal> (contains XYZ + normals)
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud(new pcl::PointCloud<pcl::PointNormal>);
    pcl::fromPCLPointCloud2(input, *cloud);

    // Prepare FPFH estimation
    pcl::FPFHEstimation<pcl::PointNormal, pcl::PointNormal, pcl::FPFHSignature33> fpfh_estimator;
    fpfh_estimator.setInputCloud(cloud);
    fpfh_estimator.setInputNormals(cloud);
    fpfh_estimator.setSearchMethod(pcl::search::KdTree<pcl::PointNormal>::Ptr(new pcl::search::KdTree<pcl::PointNormal>));

    // Set neighborhood parameters; prioritize k if non-zero, else radius
    if (k > 0) {
        fpfh_estimator.setKSearch(k);
    } else if (radius > 0.0) {
        fpfh_estimator.setRadiusSearch(radius);
    } else {
        throw std::invalid_argument("Either k or radius must be positive");
    }

    // Compute FPFH signatures
    pcl::PointCloud<pcl::FPFHSignature33> fpfhs;
    fpfh_estimator.compute(fpfhs);

    // Convert FPFH output to PCLPointCloud2 and concatenate with original fields
    pcl::PCLPointCloud2 fpfh_cloud;
    pcl::toPCLPointCloud2(fpfhs, fpfh_cloud);

    pcl::PCLPointCloud2 output;
    pcl::concatenateFields(input, fpfh_cloud, output);
    return output;
}
#include <cassert>
#include <pcl/PCLPointCloud2.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <iostream>

// Declare the function (from solution)
pcl::PCLPointCloud2 computeFPFHDescriptors(const pcl::PCLPointCloud2& input, int k, double radius);

int main() {
    // Create a synthetic cloud with 3 points, all with normals
    pcl::PointCloud<pcl::PointNormal> cloud;
    cloud.width = 3;
    cloud.height = 1;
    cloud.points.resize(cloud.width * cloud.height);
    
    cloud.points[0].x = 0.0f; cloud.points[0].y = 0.0f; cloud.points[0].z = 0.0f;
    cloud.points[0].normal_x = 1.0f; cloud.points[0].normal_y = 0.0f; cloud.points[0].normal_z = 0.0f;
    cloud.points[1].x = 1.0f; cloud.points[1].y = 0.0f; cloud.points[1].z = 0.0f;
    cloud.points[1].normal_x = 1.0f; cloud.points[1].normal_y = 0.0f; cloud.points[1].normal_z = 0.0f;
    cloud.points[2].x = 0.0f; cloud.points[2].y = 1.0f; cloud.points[2].z = 0.0f;
    cloud.points[2].normal_x = 0.0f; cloud.points[2].normal_y = 1.0f; cloud.points[2].normal_z = 0.0f;

    pcl::PCLPointCloud2 input2;
    pcl::toPCLPointCloud2(cloud, input2);

    // Test 1: k-nearest neighbor search (k=2)
    pcl::PCLPointCloud2 output = computeFPFHDescriptors(input2, 2, 0.0);
    // Check output has original fields plus fpfh fields (33 new fields)
    assert(pcl::getFieldIndex(output, "f1") != -1);
    assert(pcl::getFieldIndex(output, "f33") != -1);
    assert(output.width * output.height == 3);  // same number of points

    // Test 2: radius search (radius = 2.0)
    pcl::PCLPointCloud2 output_radius = computeFPFHDescriptors(input2, 0, 2.0);
    assert(output_radius.width * output_radius.height == 3);

    // Test 3: both k and radius zero -> throws
    bool threw = false;
    try {
        computeFPFHDescriptors(input2, 0, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 4: missing normals -> throws
    pcl::PointCloud<pcl::PointXYZ> no_normals;
    no_normals.width = 1; no_normals.height = 1;
    no_normals.points.resize(1);
    no_normals.points[0].x = 1.0f; no_normals.points[0].y = 2.0f; no_normals.points[0].z = 3.0f;
    pcl::PCLPointCloud2 input_xyz;
    pcl::toPCLPointCloud2(no_normals, input_xyz);
    threw = false;
    try {
        computeFPFHDescriptors(input_xyz, 2, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: empty cloud with normals -> returns empty output
    pcl::PointCloud<pcl::PointNormal> empty_cloud;
    empty_cloud.width = 0; empty_cloud.height = 0;
    pcl::PCLPointCloud2 input_empty;
    pcl::toPCLPointCloud2(empty_cloud, input_empty);
    // Add normal fields manually? Actually toPCLPointCloud2 preserves fields from PointNormal even if points empty
    // Check that output has 0 points
    pcl::PCLPointCloud2 output_empty = computeFPFHDescriptors(input_empty, 2, 0.0);
    assert(output_empty.width * output_empty.height == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution converts the input `PCLPointCloud2` to a `pcl::PointCloud<PointNormal>` using `fromPCLPointCloud2`, which preserves XYZ and normal data. We then instantiate `FPFHEstimation<PointNormal, PointNormal, FPFHSignature33>`, set the input cloud and normals (both to the converted cloud since it contains normals), and configure a kd-tree search. The key logic is choosing between k-nearest neighbors and radius search: if `k > 0`, call `setKSearch(k)`; else if `radius > 0.0`, call `setRadiusSearch(radius)`; if both are zero or both are non-zero, throw an exception or prioritize one (here we prioritize k if non-zero, else require radius). After computing FPFH signatures into a `PointCloud<FPFHSignature33>`, convert it to `PCLPointCloud2` and concatenate with the original input fields to produce the output. Edge cases include missing normal fields (throw), empty input (the output will also be empty), and contradictory parameters. Time complexity is `O(n log n)` for kd-tree construction plus `O(n * k)` for neighborhood queries, where `n` is the number of points; space complexity is `O(n)` for the output cloud.
