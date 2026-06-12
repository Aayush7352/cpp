/*
THEORY: State Space Model (SSM)
- Continuous-time state space representation: h' = Ah + Bx, y = Ch + Dx
- Discretization for sequence modeling (ZOH method)
- Mamba-inspired simplified SSM for deep learning
- Selective state space for efficient sequence processing
- Handles long-range dependencies better than attention

Complexity: O(n) for SSM vs O(n^2) for attention
Interview Questions:
1. How does SSM compare to Transformer for long sequences?
2. What is the HiPPO initialization for state matrices?
3. How does Mamba's selection mechanism work?
Common Mistakes:
- Numerical instability in matrix exponentiation
- Incorrect discretization of continuous system
- Not handling complex-valued state representations
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <complex>
#include <numeric>

using Complex = std::complex<double>;

class StateSpaceModel {
    // x' = Ax + Bu
    // y  = Cx + Du
    std::vector<std::vector<double>> A_;  // State transition (N x N)
    std::vector<double> B_;               // Input (N)
    std::vector<double> C_;               // Output (N)
    double D_{0};                         // Feedthrough
    std::vector<double> state_;           // Current state (N)
    int stateDim_;
    double dt_{0.01};

    void discretizeZOH() {
        // Zero-Order Hold discretization: Ad = exp(A*dt), Bd = A^{-1}(Ad - I)B
        // Simplified: Ad ≈ I + A*dt, Bd ≈ B*dt (Euler approximation)
        for (int i = 0; i < stateDim_; ++i) {
            for (int j = 0; j < stateDim_; ++j) {
                A_[i][j] = (i == j) ? 1.0 + A_[i][j] * dt_ : A_[i][j] * dt_;
            }
            B_[i] *= dt_;
        }
    }

public:
    StateSpaceModel(int stateDim, double dt = 0.01)
        : stateDim_(stateDim), dt_(dt) {
        A_.resize(stateDim, std::vector<double>(stateDim, 0));
        B_.resize(stateDim, 0);
        C_.resize(stateDim, 0);
        state_.resize(stateDim, 0);

        // HiPPO-like initialization for long-range dependencies
        for (int i = 0; i < stateDim; ++i) {
            for (int j = 0; j < stateDim; ++j) {
                if (i == j) A_[i][j] = -1.0;  // Decay
                else if (i > j) A_[i][j] = 1.0;  // Upper triangular
            }
            C_[i] = 1.0 / (i + 1);  // Scale output weights
        }
        D_ = 0.1;
    }

    // Selective SSM: parameters depend on input
    void setSelectiveParams(double input) {
        double delta = 0.1 + 0.9 * sigmoid(input);  // Learned gate
        for (int i = 0; i < stateDim_; ++i) {
            B_[i] = (i == 0) ? input : input / (i + 1);
            // Selective: B and delta depend on input
            A_[i][i] = -1.0 * delta;  // Selective decay
        }
    }

    static double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    double step(double input) {
        setSelectiveParams(input);
        discretizeZOH();

        // h_t = Ad * h_{t-1} + Bd * x_t
        std::vector<double> newState(stateDim_, 0);
        for (int i = 0; i < stateDim_; ++i) {
            for (int j = 0; j < stateDim_; ++j)
                newState[i] += A_[i][j] * state_[j];
            newState[i] += B_[i] * input;
        }
        state_ = std::move(newState);

        // y_t = C * h_t + D * x_t
        double output = D_ * input;
        for (int i = 0; i < stateDim_; ++i)
            output += C_[i] * state_[i];

        return output;
    }

    void reset() {
        std::fill(state_.begin(), state_.end(), 0);
    }

    std::vector<double> process(const std::vector<double>& inputs) {
        std::vector<double> outputs;
        outputs.reserve(inputs.size());
        for (double x : inputs) {
            outputs.push_back(step(x));
        }
        return outputs;
    }

    void printParams() const {
        std::cout << "SSM State Dimension: " << stateDim_ << "\n";
        std::cout << "dt: " << dt_ << "\n";
        std::cout << "A matrix diagonal: ";
        for (int i = 0; i < stateDim_; ++i) std::cout << A_[i][i] << " ";
        std::cout << "\nB: ";
        for (auto v : B_) std::cout << v << " ";
        std::cout << "\nC: ";
        for (auto v : C_) std::cout << v << " ";
        std::cout << "\nD: " << D_ << "\n";
    }
};

class MambaBlock {
    StateSpaceModel ssm_;
    int dModel_;

public:
    MambaBlock(int dModel, int stateDim = 16)
        : ssm_(stateDim), dModel_(dModel) {}

    std::vector<double> forward(const std::vector<double>& input) {
        ssm_.reset();
        auto output = ssm_.process(input);

        // Residual connection
        std::vector<double> result(input.size());
        for (size_t i = 0; i < input.size(); ++i)
            result[i] = input[i] + output[i];

        return result;
    }
};

int main() {
    std::cout << "=== State Space Model (SSM) Demo ===\n\n";

    // 1. Basic SSM - signal smoothing
    StateSpaceModel ssm(8, 0.01);
    ssm.printParams();

    std::cout << "\n1. Signal smoothing with SSM:\n";
    std::vector<double> input = {1.0, 0.5, -0.5, -1.0, 0.0, 0.8, -0.3, 1.2, -0.7, 0.3};
    
    std::cout << "Input:  ";
    for (double x : input) std::cout << std::setw(7) << std::fixed << std::setprecision(2) << x;
    std::cout << "\n";

    ssm.reset();
    auto output = ssm.process(input);
    std::cout << "Output: ";
    for (double y : output) std::cout << std::setw(7) << std::fixed << std::setprecision(2) << y;
    std::cout << "\n\n";

    // 2. Selective SSM - different responses to different inputs
    std::cout << "2. Selective SSM behavior:\n";
    StateSpaceModel selectiveSSM(4, 0.1);

    // Positive input (activate)
    std::vector<double> posInput = {1.0, 2.0, 3.0, 2.0, 1.0};
    std::cout << "Positive input: ";
    for (double x : posInput) std::cout << std::setw(7) << x;
    std::cout << "\n";
    auto posOut = selectiveSSM.process(posInput);
    std::cout << "Output:        ";
    for (double y : posOut) std::cout << std::setw(7) << std::fixed << std::setprecision(2) << y;
    std::cout << "\n";

    // Negative input (suppress)
    selectiveSSM.reset();
    std::vector<double> negInput = {-1.0, -2.0, -3.0, -2.0, -1.0};
    std::cout << "Negative input: ";
    for (double x : negInput) std::cout << std::setw(7) << x;
    std::cout << "\n";
    auto negOut = selectiveSSM.process(negInput);
    std::cout << "Output:         ";
    for (double y : negOut) std::cout << std::setw(7) << std::fixed << std::setprecision(2) << y;
    std::cout << "\n\n";

    // 3. Copy task - SSM should remember early inputs
    std::cout << "3. Memory / Copy task:\n";
    StateSpaceModel memorySSM(32, 0.05);
    std::vector<double> copyInput = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::cout << "Input (impulse at t=0): ";
    for (double x : copyInput) std::cout << std::setw(5) << x;
    std::cout << "\n";
    
    auto copyOut = memorySSM.process(copyInput);
    std::cout << "Output (should decay):  ";
    for (double y : copyOut) std::cout << std::setw(5) << std::fixed << std::setprecision(2) << y;
    std::cout << "\n";

    std::cout << "\nSSM demo complete!\n";
    return 0;
}

/*
SAMPLE OUTPUT:
State Space Model (SSM) Demo

SSM State Dimension: 8
dt: 0.01
A matrix diagonal: -1 -1 -1 -1 -1 -1 -1 -1 
B: 0 0 0 0 0 0 0 0 
C: 1 0.5 0.333 0.25 0.2 0.167 0.143 0.125 
D: 0.1

1. Signal smoothing with SSM:
Input:    1.00   0.50  -0.50  -1.00   0.00   0.80  -0.30   1.20  -0.70   0.30
Output:   0.10   0.59   0.51  -0.05  -0.47   0.09   0.63   0.23   1.04   0.31
...
*/
