// Write a C++ function `nms_rotated_cpu` that performs greedy non-maximum suppression (NMS) on axis-aligned bounding boxes given as `(x1, y1, x2, y2)` tensors, where each row represents one box. The function takes three arguments: a contiguous 2D `at::Tensor` of boxes (`dets`, shape `[N, 4]`), a 1D `at::Tensor` of scores (`scores`, length `N`), and a float `iou_threshold`. It must return a 1D `at::Tensor` of type `at::kLong` containing the indices of the boxes kept, sorted by descending score. Boxes are suppressed if their Intersection-over-Union (IoU) with a higher-scoring kept box is greater than or equal to the threshold. The function must handle empty input gracefully by returning an empty tensor. Assume boxes are axis-aligned so IoU computation is straightforward: intersection area = max(0, min(x2) - max(x1)) * max(0, min(y2) - max(y1)), union = area_i + area_j - intersection. Use only `ATen`/`torch` tensor operations (e.g., `at::sort`, `at::zeros`, `at::narrow`). The function must be templated on a scalar type internally and dispatched using `AT_DISPATCH_FLOATING_TYPES` for float and double. Ensure all tensors are CPU, contiguous, and both `dets` and `scores` share the same floating type.
#include <ATen/ATen.h>
#include <cassert>
#include <vector>

int main() {
  using namespace detectron2;

  // Test 1: basic suppression with two overlapping boxes
  at::Tensor dets1 = at::tensor({{0, 0, 10, 10}, {1, 1, 9, 9}}, at::kFloat);
  at::Tensor scores1 = at::tensor({0.9, 0.8}, at::kFloat);
  auto keep1 = nms_rotated_cpu(dets1, scores1, 0.5);
  assert(keep1.equal(at::tensor({0}, at::kLong)));

  // Test 2: non-overlapping boxes, both kept
  at::Tensor dets2 = at::tensor({{0, 0, 1, 1}, {10, 10, 11, 11}}, at::kFloat);
  at::Tensor scores2 = at::tensor({0.5, 0.4}, at::kFloat);
  auto keep2 = nms_rotated_cpu(dets2, scores2, 0.5);
  assert(keep2.equal(at::tensor({0, 1}, at::kLong)));

  // Test 3: empty input
  at::Tensor dets3 = at::empty({0, 4}, at::kFloat);
  at::Tensor scores3 = at::empty({0}, at::kFloat);
  auto keep3 = nms_rotated_cpu(dets3, scores3, 0.5);
  assert(keep3.numel() == 0);

  // Test 4: exactly threshold iou suppresses
  at::Tensor dets4 = at::tensor({{0, 0, 2, 2}, {0, 0, 2, 2}}, at::kFloat);
  at::Tensor scores4 = at::tensor({1.0, 0.9}, at::kFloat);
  auto keep4 = nms_rotated_cpu(dets4, scores4, 1.0);
  assert(keep4.equal(at::tensor({0}, at::kLong)));

  // Test 5: double type works
  at::Tensor dets5 = at::tensor({{0, 0, 10, 10}, {2, 2, 8, 8}}, at::kDouble);
  at::Tensor scores5 = at::tensor({0.9, 0.8}, at::kDouble);
  auto keep5 = nms_rotated_cpu(dets5, scores5, 0.3);
  assert(keep5.equal(at::tensor({0}, at::kLong)));

  // Test 6: three boxes, middle suppressed by first, third kept if iou low
  at::Tensor dets6 = at::tensor({{0, 0, 10, 10}, {1, 1, 9, 9}, {20, 20, 30, 30}}, at::kFloat);
  at::Tensor scores6 = at::tensor({1.0, 0.9, 0.8}, at::kFloat);
  auto keep6 = nms_rotated_cpu(dets6, scores6, 0.5);
  assert(keep6.equal(at::tensor({0, 2}, at::kLong)));

  // Test 7: scores not sorted, output sorted by score
  at::Tensor dets7 = at::tensor({{0, 0, 10, 10}, {1, 1, 9, 9}}, at::kFloat);
  at::Tensor scores7 = at::tensor({0.6, 0.9}, at::kFloat);
  auto keep7 = nms_rotated_cpu(dets7, scores7, 0.5);
  assert(keep7.equal(at::tensor({1}, at::kLong))); // higher score box kept

  // Test 8: large threshold keeps all distinct boxes even if overlapping
  at::Tensor dets8 = at::tensor({{0, 0, 10, 10}, {1, 1, 9, 9}}, at::kFloat);
  at::Tensor scores8 = at::tensor({0.9, 0.8}, at::kFloat);
  auto keep8 = nms_rotated_cpu(dets8, scores8, 1.1); // threshold >1, no suppression
  assert(keep8.equal(at::tensor({0, 1}, at::kLong)));

  return 0;
}
#include <ATen/ATen.h>
#include <ATen/Dispatch.h>
#include <c10/util/ArrayRef.h>
#include <algorithm>
#include <cmath>

namespace detectron2 {

template <typename scalar_t>
static inline scalar_t single_box_iou(
    const scalar_t* box1,
    const scalar_t* box2) {
  // box format: (x1, y1, x2, y2)
  scalar_t x1_inter = std::max(box1[0], box2[0]);
  scalar_t y1_inter = std::max(box1[1], box2[1]);
  scalar_t x2_inter = std::min(box1[2], box2[2]);
  scalar_t y2_inter = std::min(box1[3], box2[3]);

  scalar_t inter_w = std::max(static_cast<scalar_t>(0), x2_inter - x1_inter);
  scalar_t inter_h = std::max(static_cast<scalar_t>(0), y2_inter - y1_inter);
  scalar_t inter_area = inter_w * inter_h;

  scalar_t area1 = (box1[2] - box1[0]) * (box1[3] - box1[1]);
  scalar_t area2 = (box2[2] - box2[0]) * (box2[3] - box2[1]);
  scalar_t union_area = area1 + area2 - inter_area;

  if (union_area <= static_cast<scalar_t>(0)) {
    return static_cast<scalar_t>(0);
  }
  return inter_area / union_area;
}

template <typename scalar_t>
at::Tensor nms_cpu_kernel(
    const at::Tensor& dets,
    const at::Tensor& scores,
    const float iou_threshold) {
  if (dets.numel() == 0) {
    return at::empty({0}, dets.options().dtype(at::kLong));
  }

  auto order_t = std::get<1>(scores.sort(0, /*descending=*/true));
  auto ndets = dets.size(0);

  at::Tensor suppressed_t = at::zeros({ndets}, dets.options().dtype(at::kByte));
  at::Tensor keep_t = at::zeros({ndets}, dets.options().dtype(at::kLong));

  auto suppressed = suppressed_t.data_ptr<uint8_t>();
  auto keep = keep_t.data_ptr<int64_t>();
  auto order = order_t.data_ptr<int64_t>();

  int64_t num_to_keep = 0;

  for (int64_t _i = 0; _i < ndets; _i++) {
    auto i = order[_i];
    if (suppressed[i] == 1) {
      continue;
    }

    keep[num_to_keep++] = i;

    const scalar_t* box_i = dets[i].data_ptr<scalar_t>();

    for (int64_t _j = _i + 1; _j < ndets; _j++) {
      auto j = order[_j];
      if (suppressed[j] == 1) {
        continue;
      }

      const scalar_t* box_j = dets[j].data_ptr<scalar_t>();
      auto ovr = single_box_iou<scalar_t>(box_i, box_j);
      if (ovr >= static_cast<scalar_t>(iou_threshold)) {
        suppressed[j] = 1;
      }
    }
  }

  return keep_t.narrow(/*dim=*/0, /*start=*/0, /*length=*/num_to_keep);
}

at::Tensor nms_rotated_cpu(
    const at::Tensor& dets,
    const at::Tensor& scores,
    const float iou_threshold) {
  AT_ASSERTM(dets.device().is_cpu(), "dets must be a CPU tensor");
  AT_ASSERTM(scores.device().is_cpu(), "scores must be a CPU tensor");
  AT_ASSERTM(dets.scalar_type() == scores.scalar_type(),
             "dets should have the same type as scores");
  AT_ASSERTM(dets.dim() == 2 && dets.size(1) == 4,
             "dets must be shape [N, 4]");
  AT_ASSERTM(scores.dim() == 1 && scores.size(0) == dets.size(0),
             "scores must be 1D with length N");
  AT_ASSERTM(dets.is_contiguous(), "dets must be contiguous");
  AT_ASSERTM(scores.is_contiguous(), "scores must be contiguous");

  auto result = at::empty({0}, dets.options());

  AT_DISPATCH_FLOATING_TYPES(dets.scalar_type(), "nms_rotated", [&] {
    result = nms_cpu_kernel<scalar_t>(dets, scores, iou_threshold);
  });
  return result;
}

} // namespace detectron2
// The algorithm follows the classic greedy NMS: first sort the indices of scores in descending order. Maintain a boolean `suppressed` array initialized to false. Iterate over the sorted indices; for each index `i` that is not suppressed, add it to the keep list, then iterate over all later indices `j` in the sorted order (i.e., boxes with lower scores) and compute the IoU between box `i` and box `j`. If the IoU >= threshold, mark `j` as suppressed. Finally, return the first `num_to_keep` entries of the keep tensor. Edge cases: empty input returns an empty `kLong` tensor; duplicate boxes with equal IoU are suppressed; boxes with IoU exactly equal to threshold are suppressed; the output indices are in decreasing score order (ties broken by original order because `at::sort` is stable? Actually stable not guaranteed, but we proceed by using the returned order directly). Time complexity is O(N^2) in the worst case because for each kept box we scan all remaining boxes; space complexity is O(N) for the suppressed and keep arrays and the sorting order. The solution uses ATen tensor APIs to sort scores, allocate byte and long tensors, and narrow the result, matching the provided snippet’s structure but with axis-aligned IoU.
