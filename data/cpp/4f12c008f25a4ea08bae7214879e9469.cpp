// Write a C++ function `updateSGDParams` that takes a `std::vector<Tensor*>` of parameter tensors (where each `Tensor` is a simple struct containing a `std::vector<double>* data` and a `std::vector<double>* gradients`, both of equal length) and a `double learning_rate`. The function must perform one step of stochastic gradient descent: for each parameter tensor, update each element `data[i]` by subtracting `learning_rate * gradients[i]`. After updating, the gradients must be set to zero (all elements to `0.0`). The function returns `void`. Ensure the function handles empty vectors gracefully (no operation) and does not modify the learning rate or the overall structure of vectors. Use `const` for the learning rate parameter only, not for the tensor pointers (since their contents are mutated).

The solution iterates over every tensor in the input vector. For each tensor, it first checks that the `data` and `gradients` pointers are non-null and that the sizes of `data` and `gradients` are equal (to avoid out-of-bounds access). If sizes are mismatched, the safest approach is to skip that tensor (or optionally throw; but for simplicity, we skip). For each valid tensor, we loop over each index `i` from 0 to `data->size()-1`, apply the update `(*data)[i] -= learning_rate * (*gradients)[i]`, and simultaneously set `(*gradients)[i] = 0.0` after reading the gradient value. Edge cases: empty input vector – no work; tensors with zero-length vectors – loops do nothing; duplicate tensors (same pointer appearing multiple times) – behavior is deterministic but the second occurrence would see zero gradients and update data by zero, which is fine. Time complexity is O(T * N) where T is the number of tensors and N is the average size of each tensor's data/gradient vectors. Space complexity is O(1) additional space, since we only use loop indices and a temporary variable for the gradient value (if needed). The update and zeroing can be done in a single pass to avoid iterating twice. We must be careful to read the gradient before zeroing it; for example, `double gradVal = (*gradients)[i]; (*data)[i] -= learning_rate * gradVal; (*gradients)[i] = 0.0;`. Alternatively, we can write `(*data)[i] -= learning_rate * (*gradients)[i]; (*gradients)[i] = 0;` since the multiplication uses the old value before assignment, but it's clearer to store in a local variable for readability.

#include <vector>
#include <cstddef>

// Minimal Tensor struct for this task.
struct Tensor {
    std::vector<double>* data;
    std::vector<double>* gradients;
};

// Perform one SGD step: update data by subtracting learning_rate * gradients, then zero the gradients.
void updateSGDParams(const std::vector<Tensor*>& parameters, const double learning_rate) {
    for (Tensor* param : parameters) {
        if (param == nullptr || param->data == nullptr || param->gradients == nullptr) {
            continue; // Skip invalid tensors
        }
        const std::size_t n = param->data->size();
        if (n != param->gradients->size()) {
            continue; // Skip mismatched tensors to avoid out-of-bounds
        }
        for (std::size_t i = 0; i < n; ++i) {
            double grad = (*param->gradients)[i];
            (*param->data)[i] -= learning_rate * grad;
            (*param->gradients)[i] = 0.0;
        }
    }
}

#include <cassert>
#include <vector>
#include <memory>

// Include the solution function (or copy here for standalone).
// Assuming the above code is available.

int main() {
    // Helper to create a tensor with given data and gradients.
    auto makeTensor = [](std::vector<double> data, std::vector<double> grads) {
        Tensor* t = new Tensor();
        t->data = new std::vector<double>(data);
        t->gradients = new std::vector<double>(grads);
        return t;
    };

    // Case 1: Basic update with two tensors.
    Tensor* t1 = makeTensor({1.0, 2.0, 3.0}, {0.1, 0.2, 0.3});
    Tensor* t2 = makeTensor({10.0, 20.0}, {1.0, 0.5});
    std::vector<Tensor*> params = {t1, t2};
    updateSGDParams(params, 0.5);

    assert((*t1->data)[0] == 1.0 - 0.5*0.1); // 0.95
    assert((*t1->data)[1] == 2.0 - 0.5*0.2); // 1.9
    assert((*t1->data)[2] == 3.0 - 0.5*0.3); // 2.85
    assert((*t1->gradients)[0] == 0.0);
    assert((*t1->gradients)[1] == 0.0);
    assert((*t1->gradients)[2] == 0.0);

    assert((*t2->data)[0] == 10.0 - 0.5*1.0); // 9.5
    assert((*t2->data)[1] == 20.0 - 0.5*0.5); // 19.75
    assert((*t2->gradients)[0] == 0.0);
    assert((*t2->gradients)[1] == 0.0);

    // Case 2: Zero learning rate (no change in data, gradients still zeroed).
    Tensor* t3 = makeTensor({5.0, 6.0}, {2.0, 3.0});
    std::vector<Tensor*> params2 = {t3};
    updateSGDParams(params2, 0.0);
    assert((*t3->data)[0] == 5.0);
    assert((*t3->data)[1] == 6.0);
    assert((*t3->gradients)[0] == 0.0);
    assert((*t3->gradients)[1] == 0.0);

    // Case 3: Empty vector of parameters (no crash).
    std::vector<Tensor*> empty;
    updateSGDParams(empty, 0.1);

    // Case 4: Tensor with zero-length vectors.
    Tensor* t4 = makeTensor({}, {});
    std::vector<Tensor*> params4 = {t4};
    updateSGDParams(params4, 0.1); // no crash, nothing to update

    // Case 5: Tensor with mismatched sizes (should be skipped).
    Tensor* t5 = makeTensor({1.0}, {2.0, 3.0});
    std::vector<Tensor*> params5 = {t5};
    updateSGDParams(params5, 1.0);
    assert((*t5->data)[0] == 1.0); // unchanged because skipped
    assert((*t5->gradients)[0] == 2.0); // unchanged

    // Cleanup (not necessary in test but good practice).
    delete t1->data; delete t1->gradients; delete t1;
    delete t2->data; delete t2->gradients; delete t2;
    delete t3->data; delete t3->gradients; delete t3;
    delete t4->data; delete t4->gradients; delete t4;
    delete t5->data; delete t5->gradients; delete t5;

    return 0;
}
