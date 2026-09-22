// Write a C++ function that simulates the learning rate computation and gradient update scheduling from a simplified stochastic gradient descent (SGD) solver. Given a configuration struct with fields for `base_lr`, `gamma`, `power`, `stepsize`, `max_iter`, and a policy string, plus a vector of per-parameter learning rate multipliers and a vector of gradient values, the function should compute the current learning rate according to the policy (supporting `fixed`, `step`, `exp`, `inv`, `multistep`, `poly`, and `sigmoid`), then apply an SGD update with momentum to each parameter. The function should maintain state (current iteration, current step for multistep policy, and history/momentum buffers) across calls via a state struct passed by reference. Return a pair: the computed learning rate and the updated parameter values. The function must handle edge cases such as `max_iter` being zero (for `poly`, return `base_lr` if `iter` == 0, otherwise throw an invalid_argument), unknown policies (throw `invalid_argument`), negative or zero `stepsize` for `step` and `sigmoid` policies (throw `invalid_argument`), and empty gradient/parameter vectors (return empty updated parameters). Use `std::pow`, `std::exp`, `std::floor` from `<cmath>`, and require all inputs to be `double`.
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Fixed policy with no momentum.
    {
        SGDConfig cfg{"fixed", 0.1, 0.0, 0.0, 0, 0, {}, 0.0};
        SGDState state{0, 0, {}};
        auto result = sgd_step(cfg, state, {1.0, 2.0}, {0.5, -1.0}, {1.0, 1.0});
        assert(std::fabs(result.first - 0.1) < 1e-9);
        assert(std::fabs(result.second[0] - 0.95) < 1e-9); // 1.0 - 0.1*0.5 = 0.95
        assert(std::fabs(result.second[1] - 2.1) < 1e-9);  // 2.0 - 0.1*(-1.0) = 2.1
        assert(state.iter == 1);
    }

    // Test 2: Step policy with gamma=0.5, stepsize=2, base_lr=1.
    {
        SGDConfig cfg{"step", 1.0, 0.5, 0.0, 2, 0, {}, 0.0};
        SGDState state{0, 0, {}};
        auto r1 = sgd_step(cfg, state, {0.0}, {1.0}, {1.0});
        assert(std::fabs(r1.first - 1.0) < 1e-9); // iter 0, floor(0/2)=0
        auto r2 = sgd_step(cfg, state, {0.0}, {1.0}, {1.0});
        assert(std::fabs(r2.first - 1.0) < 1e-9); // iter 1, still step 0
        auto r3 = sgd_step(cfg, state, {0.0}, {1.0}, {1.0});
        assert(std::fabs(r3.first - 0.5) < 1e-9); // iter 2, floor(2/2)=1
        assert(state.iter == 3);
    }

    // Test 3: Exp policy with gamma=2, base_lr=0.5.
    {
        SGDConfig cfg{"exp", 0.5, 2.0, 0.0, 0, 0, {}, 0.0};
        SGDState state{3, 0, {}};
        auto result = sgd_step(cfg, state, {1.0}, {0.0}, {1.0});
        assert(std::fabs(result.first - 0.5 * std::pow(2.0, 3)) < 1e-9);
    }

    // Test 4: Inv policy.
    {
        SGDConfig cfg{"inv", 1.0, 0.1, 2.0, 0, 0, {}, 0.0};
        SGDState state{5, 0, {}};
        auto result = sgd_step(cfg, state, {1.0}, {0.0}, {1.0});
        double expected = std::pow(1.0 + 0.1 * 5, -2.0);
        assert(std::fabs(result.first - expected) < 1e-9);
    }

    // Test 5: Multistep policy.
    {
        SGDConfig cfg{"multistep", 1.0, 0.1, 0.0, 0, 0, {2, 4}, 0.0};
        SGDState state{0, 0, {}};
        auto r0 = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        assert(std::fabs(r0.first - 1.0) < 1e-9); // step 0
        auto r1 = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        assert(std::fabs(r1.first - 1.0) < 1e-9); // iter 1, still below 2
        auto r2 = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        assert(std::fabs(r2.first - 0.1) < 1e-9); // iter 2, reaches stepvalue 2 -> step 1
        auto r3 = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        assert(std::fabs(r3.first - 0.1) < 1e-9); // iter 3, still step 1
        auto r4 = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        assert(std::fabs(r4.first - 0.01) < 1e-9); // iter 4, reaches stepvalue 4 -> step 2
    }

    // Test 6: Poly policy.
    {
        SGDConfig cfg{"poly", 1.0, 0.0, 2.0, 0, 10, {}, 0.0};
        SGDState state{5, 0, {}};
        auto result = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        double expected = std::pow(1.0 - 5.0/10.0, 2.0); // 0.25
        assert(std::fabs(result.first - 0.25) < 1e-9);
    }

    // Test 7: Sigmoid policy.
    {
        SGDConfig cfg{"sigmoid", 1.0, 0.5, 0.0, 3, 0, {}, 0.0};
        SGDState state{1, 0, {}};
        auto result = sgd_step(cfg, state, {0.0}, {0.0}, {1.0});
        double expected = 1.0 / (1.0 + std::exp(-0.5 * (1 - 3)));
        assert(std::fabs(result.first - expected) < 1e-9);
    }

    // Test 8: Momentum update.
    {
        SGDConfig cfg{"fixed", 0.1, 0.0, 0.0, 0, 0, {}, 0.9};
        SGDState state{0, 0, {}};
        // First update: history = 0.9*0 + 0.1*1 = 0.1, param = 1.0 - 0.1 = 0.9
        auto r1 = sgd_step(cfg, state, {1.0}, {1.0}, {1.0});
        assert(std::fabs(r1.second[0] - 0.9) < 1e-9);
        // Second update: history = 0.9*0.1 + 0.1*1 = 0.19, param = 0.9 - 0.19 = 0.71
        auto r2 = sgd_step(cfg, state, {r1.second[0]}, {1.0}, {1.0});
        assert(std::fabs(r2.second[0] - 0.71) < 1e-9);
    }

    // Test 9: Per-parameter learning rate multipliers.
    {
        SGDConfig cfg{"fixed", 0.1, 0.0, 0.0, 0, 0, {}, 0.0};
        SGDState state{0, 0, {}};
        auto res = sgd_step(cfg, state, {0.0, 0.0}, {1.0, 1.0}, {0.5, 2.0});
        assert(std::fabs(res.second[0] - (-0.05)) < 1e-9);
        assert(std::fabs(res.second[1] - (-0.2)) < 1e-9);
    }

    // Test 10: Error handling - unknown policy.
    {
        SGDConfig cfg{"unknown", 0.1, 0.0, 0.0, 0, 0, {}, 0.0};
        SGDState state{0, 0, {}};
        bool threw = false;
        try {
            sgd_step(cfg, state, {}, {}, {});
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>

// Configuration for the SGD solver simulation.
struct SGDConfig {
    std::string lr_policy;
    double base_lr;
    double gamma;
    double power;
    int stepsize;          // used for step and sigmoid policies
    int max_iter;          // used for poly policy
    std::vector<int> stepvalues; // used for multistep policy
    double momentum;
};

// Mutable state maintained across calls.
struct SGDState {
    int iter;
    int current_step;
    std::vector<double> history; // same size as parameter/gradient vectors
};

// Computes the learning rate based on the policy and updates the state.
// Returns a pair: (computed learning rate, updated parameter vector).
std::pair<double, std::vector<double>> sgd_step(
    const SGDConfig& cfg,
    SGDState& state,
    const std::vector<double>& params,
    const std::vector<double>& gradients,
    const std::vector<double>& lr_multipliers) 
{
    // Validate that parameter, gradient, and multiplier sizes match.
    if (params.size() != gradients.size() || params.size() != lr_multipliers.size()) {
        throw std::invalid_argument("Parameter, gradient, and multiplier sizes must match.");
    }
    
    // Ensure history buffer is sized correctly.
    if (state.history.size() != params.size()) {
        state.history.assign(params.size(), 0.0);
    }

    // Compute learning rate based on policy.
    double rate;
    const std::string& policy = cfg.lr_policy;
    if (policy == "fixed") {
        rate = cfg.base_lr;
    } else if (policy == "step") {
        if (cfg.stepsize <= 0) {
            throw std::invalid_argument("Step policy requires positive stepsize.");
        }
        int step = state.iter / cfg.stepsize;
        rate = cfg.base_lr * std::pow(cfg.gamma, step);
    } else if (policy == "exp") {
        rate = cfg.base_lr * std::pow(cfg.gamma, state.iter);
    } else if (policy == "inv") {
        rate = cfg.base_lr * std::pow(1.0 + cfg.gamma * state.iter, -cfg.power);
    } else if (policy == "multistep") {
        // Advance current_step if appropriate.
        while (state.current_step < static_cast<int>(cfg.stepvalues.size()) &&
               state.iter >= cfg.stepvalues[state.current_step]) {
            state.current_step++;
        }
        rate = cfg.base_lr * std::pow(cfg.gamma, state.current_step);
    } else if (policy == "poly") {
        if (cfg.max_iter == 0) {
            if (state.iter == 0) {
                // Undefined fraction but convention: treat 1 - 0/0 as 1.
                rate = cfg.base_lr;
            } else {
                throw std::invalid_argument("Poly policy requires positive max_iter for iter > 0.");
            }
        } else {
            rate = cfg.base_lr * std::pow(1.0 - static_cast<double>(state.iter) / cfg.max_iter, cfg.power);
        }
    } else if (policy == "sigmoid") {
        if (cfg.stepsize <= 0) {
            throw std::invalid_argument("Sigmoid policy requires positive stepsize.");
        }
        rate = cfg.base_lr / (1.0 + std::exp(-cfg.gamma * (state.iter - cfg.stepsize)));
    } else {
        throw std::invalid_argument("Unknown learning rate policy: " + policy);
    }

    // Apply momentum-based SGD update to each parameter.
    std::vector<double> updated_params(params.size());
    for (size_t i = 0; i < params.size(); ++i) {
        double local_rate = rate * lr_multipliers[i];
        // Update the momentum history: history = momentum * history + local_rate * gradient
        state.history[i] = cfg.momentum * state.history[i] + local_rate * gradients[i];
        // The parameter is updated by subtracting the momentum history (since gradient descent)
        updated_params[i] = params[i] - state.history[i];
    }

    // Advance iteration counter.
    state.iter++;

    return {rate, updated_params};
}
// The core challenge is implementing the learning rate formula for each policy exactly as in the original Caffe code, while maintaining internal state for the multistep policy. For `fixed`, the rate is simply `base_lr`. For `step`, the rate is `base_lr * gamma^(floor(iter/stepsize))`, but we must pre-check that `stepsize` is positive to avoid division by zero or negative steps. For `exp`, rate = `base_lr * gamma^iter`. For `inv`, rate = `base_lr * (1 + gamma*iter)^(-power)`. For `multistep`, we maintain a `current_step` counter that increments whenever `iter` reaches or exceeds the next step value from the `stepvalues` vector; the rate is `base_lr * gamma^current_step`. For `poly`, rate = `base_lr * (1 - iter/max_iter)^power`, but if `max_iter` is 0, we must handle the division by zero—if `iter` is 0, the rate is `base_lr` (since 1 - 0/0 is undefined, but conventionally we treat the fraction as 0); otherwise throw. For `sigmoid`, rate = `base_lr * 1/(1 + exp(-gamma*(iter-stepsize)))`, with `stepsize` positive. After computing the rate, for each parameter `i`, we compute `local_rate = rate * lr_multiplier[i]` and update the momentum history as `history[i] = momentum * history[i] + local_rate * gradient[i]`, then the new parameter value is `parameter[i] = parameter[i] - history[i]` (since the gradient is typically subtracted). The state struct must hold `iter_`, `current_step_`, and `history_` (a vector of doubles). The function must increment `iter_` at the end of each call (to simulate advancing to the next iteration). Edge cases: unknown policy throws `std::invalid_argument`; `stepsize` ≤ 0 for step/sigmoid throws; `max_iter == 0` for poly with `iter_ > 0` throws. Time complexity is O(P) where P is the number of parameters, because for each parameter we do constant work. Space complexity is O(P) for the history vector.
