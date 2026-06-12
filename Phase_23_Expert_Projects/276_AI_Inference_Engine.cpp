/*
THEORY: AI Inference Engine
- Simple neural network inference engine
- Matrix operations with template
- Activation functions: ReLU, Sigmoid, Tanh, Softmax
- Feedforward network with multiple layers
- Supports batch inference

Complexity: O(n*m) for matrix multiply
Interview Questions:
1. How to optimize matrix multiplication? (SIMD, cache blocking)
2. What is quantization and how does it speed up inference?
3. How do inference engines like TensorRT work?
Common Mistakes:
- Numerical stability issues (log-sum-exp trick)
- Not handling batch normalization during inference
- Memory layout issues (row-major vs column-major)
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <stdexcept>

class Matrix {
public:
    size_t rows_, cols_;
    std::vector<float> data_;

    Matrix(size_t r = 0, size_t c = 0, float init = 0.0f)
        : rows_(r), cols_(c), data_(r * c, init) {}

    float& at(size_t r, size_t c) { return data_[r * cols_ + c]; }
    const float& at(size_t r, size_t c) const { return data_[r * cols_ + c]; }

    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_)
            throw std::runtime_error("Matrix dimension mismatch for multiply");
        Matrix result(rows_, other.cols_, 0.0f);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t k = 0; k < cols_; ++k)
                for (size_t j = 0; j < other.cols_; ++j)
                    result.at(i, j) += at(i, k) * other.at(k, j);
        return result;
    }

    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_)
            throw std::runtime_error("Matrix dimension mismatch for add");
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] + other.data_[i];
        return result;
    }

    void apply(std::function<float(float)> fn) {
        for (auto& v : data_) v = fn(v);
    }

    std::vector<float> row(size_t r) const {
        return std::vector<float>(data_.begin() + r * cols_,
                                  data_.begin() + (r + 1) * cols_);
    }

    void randomize(float mean = 0.0f, float stddev = 0.1f) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<float> dist(mean, stddev);
        for (auto& v : data_) v = dist(gen);
    }

    void print(const std::string& name = "") const {
        if (!name.empty()) std::cout << name << ":\n";
        for (size_t i = 0; i < rows_; ++i) {
            std::cout << "  [";
            for (size_t j = 0; j < cols_; ++j) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(4) << at(i, j);
            }
            std::cout << " ]\n";
        }
    }
};

struct Layer {
    Matrix weights;
    Matrix biases;
    std::string activation;

    Layer(size_t inputSize, size_t outputSize, std::string act = "relu")
        : weights(outputSize, inputSize), biases(outputSize, 1), activation(std::move(act)) {
        weights.randomize();
        biases.randomize(0, 0.01f);
    }

    Matrix forward(const Matrix& input) {
        // output = activation(weights * input + biases)
        auto z = weights * input;
        for (size_t i = 0; i < z.rows_; ++i)
            z.at(i, 0) += biases.at(i, 0);

        if (activation == "relu") {
            z.apply([](float x) { return x > 0 ? x : 0.0f; });
        } else if (activation == "sigmoid") {
            z.apply([](float x) { return 1.0f / (1.0f + std::exp(-x)); });
        } else if (activation == "tanh") {
            z.apply([](float x) { return std::tanh(x); });
        } else if (activation == "softmax") {
            float maxVal = *std::max_element(z.data_.begin(), z.data_.end());
            float sum = 0;
            for (auto& v : z.data_) { v = std::exp(v - maxVal); sum += v; }
            for (auto& v : z.data_) v /= sum;
        }
        return z;
    }
};

class NeuralNetwork {
    std::vector<Layer> layers_;
    size_t inputSize_;

public:
    explicit NeuralNetwork(size_t inputSize) : inputSize_(inputSize) {}

    void addLayer(size_t size, const std::string& activation = "relu") {
        size_t prevSize = layers_.empty() ? inputSize_ : layers_.back().weights.rows_;
        layers_.emplace_back(prevSize, size, activation);
    }

    Matrix predict(const Matrix& input) {
        if (input.rows_ != inputSize_ || input.cols_ != 1)
            throw std::runtime_error("Input dimension mismatch");
        Matrix current = input;
        for (auto& layer : layers_)
            current = layer.forward(current);
        return current;
    }

    std::vector<float> predict(const std::vector<float>& input) {
        Matrix m(input.size(), 1);
        for (size_t i = 0; i < input.size(); ++i) m.at(i, 0) = input[i];
        auto result = predict(m);
        std::vector<float> output(result.rows_);
        for (size_t i = 0; i < result.rows_; ++i) output[i] = result.at(i, 0);
        return output;
    }

    void print() const {
        std::cout << "Neural Network (" << layers_.size() << " layers):\n";
        for (size_t i = 0; i < layers_.size(); ++i)
            std::cout << "  Layer " << (i + 1) << ": "
                      << layers_[i].weights.cols_ << " -> "
                      << layers_[i].weights.rows_
                      << " (" << layers_[i].activation << ")\n";
    }
};

int main() {
    // XOR problem - classic test
    NeuralNetwork nn(2);
    nn.addLayer(4, "relu");
    nn.addLayer(4, "relu");
    nn.addLayer(1, "sigmoid");
    nn.print();

    // Test data
    struct TestCase { float x1, x2; float expected; };
    std::vector<TestCase> tests = {
        {0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 0}
    };

    std::cout << "\nXOR Predictions (untrained network):\n";
    for (const auto& t : tests) {
        auto output = nn.predict({t.x1, t.x2});
        std::cout << "  " << t.x1 << " XOR " << t.x2 << " = "
                  << std::fixed << std::setprecision(4) << output[0]
                  << " (expected: " << t.expected << ")\n";
    }

    // Image classification example (28x28 -> 10 classes - MNIST like)
    std::cout << "\nMNIST-like classifier:\n";
    NeuralNetwork classifier(784); // 28x28
    classifier.addLayer(128, "relu");
    classifier.addLayer(64, "relu");
    classifier.addLayer(10, "softmax");
    classifier.print();

    // Simulate classifying a random image
    std::vector<float> image(784);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (auto& p : image) p = dist(gen);

    auto probs = classifier.predict(image);
    auto maxIt = std::max_element(probs.begin(), probs.end());
    int predicted = std::distance(probs.begin(), maxIt);

    std::cout << "Predicted digit: " << predicted
              << " (confidence: " << std::fixed << std::setprecision(2)
              << (*maxIt * 100) << "%)\n";

    std::cout << "Class probabilities:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "  " << i << ": " << std::setw(6)
                  << std::fixed << std::setprecision(2) << (probs[i] * 100) << "%\n";
    }

    return 0;
}

/*
SAMPLE OUTPUT:
Neural Network (3 layers):
  Layer 1: 2 -> 4 (relu)
  Layer 2: 4 -> 4 (relu)
  Layer 3: 4 -> 1 (sigmoid)

XOR Predictions (untrained network):
  0 XOR 0 = 0.5123 (expected: 0)
  0 XOR 1 = 0.4876 (expected: 1)
  ...
*/
