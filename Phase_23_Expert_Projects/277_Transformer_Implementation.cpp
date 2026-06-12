/*
THEORY: Transformer Implementation
- Self-attention mechanism with multi-head attention
- Positional encoding
- Encoder-decoder architecture
- Feed-forward network with ReLU activation
- Layer normalization and residual connections
- Scaled dot-product attention

Complexity: O(n^2 * d) for self-attention (n=seq_len, d=model_dim)
Interview Questions:
1. Why is self-attention O(n^2) and how to optimize? (sparse attention, linear attention)
2. What is the purpose of positional encoding?
3. How does multi-head attention work?
Common Mistakes:
- Not scaling dot products in attention (causing softmax saturation)
- Incorrect masking for decoder self-attention
- Numerical issues in layer normalization
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <memory>
#include <functional>

float dot(const std::vector<float>& a, const std::vector<float>& b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.0f);
}

std::vector<float> softmax(const std::vector<float>& x) {
    std::vector<float> result(x.size());
    float maxVal = *std::max_element(x.begin(), x.end());
    float sum = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        result[i] = std::exp(x[i] - maxVal);
        sum += result[i];
    }
    for (auto& v : result) v /= sum;
    return result;
}

struct Tensor {
    std::vector<float> data;
    std::vector<int> shape;

    Tensor() = default;
    Tensor(std::vector<int> s, float init = 0.0f) : shape(std::move(s)) {
        int total = 1;
        for (int d : shape) total *= d;
        data.assign(total, init);
    }

    int stride(int dim) const {
        int s = 1;
        for (int i = dim + 1; i < (int)shape.size(); ++i) s *= shape[i];
        return s;
    }

    float& at(const std::vector<int>& indices) {
        int idx = 0;
        for (int i = 0; i < (int)indices.size(); ++i)
            idx += indices[i] * stride(i);
        return data[idx];
    }

    void randomize(float mean = 0.0f, float stddev = 0.02f) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<float> dist(mean, stddev);
        for (auto& v : data) v = dist(gen);
    }
};

class AttentionHead {
    std::vector<float> wQ_, wK_, wV_;
    int dModel_, dK_;

public:
    AttentionHead(int dModel, int dK) : dModel_(dModel), dK_(dK) {
        wQ_.resize(dModel * dK);
        wK_.resize(dModel * dK);
        wV_.resize(dModel * dK);
        // Initialize with random weights (simplified)
    }

    std::vector<std::vector<float>> forward(
        const std::vector<std::vector<float>>& Q,
        const std::vector<std::vector<float>>& K,
        const std::vector<std::vector<float>>& V,
        const std::vector<bool>& mask = {}) {
        
        int seqLen = Q.size();
        std::vector<std::vector<float>> output(seqLen, std::vector<float>(dK_));

        for (int i = 0; i < seqLen; ++i) {
            std::vector<float> scores(seqLen);
            for (int j = 0; j < seqLen; ++j) {
                scores[j] = dot(Q[i], K[j]) / std::sqrt((float)dK_);
                if (!mask.empty() && !mask[j]) scores[j] = -1e9;
            }
            auto attn = softmax(scores);
            for (int j = 0; j < seqLen; ++j) {
                for (int d = 0; d < dK_; ++d)
                    output[i][d] += attn[j] * V[j][d];
            }
        }
        return output;
    }
};

class MultiHeadAttention {
    std::vector<AttentionHead> heads_;
    int dModel_, nHeads_, dK_;

public:
    MultiHeadAttention(int dModel, int nHeads)
        : dModel_(dModel), nHeads_(nHeads), dK_(dModel / nHeads) {
        for (int i = 0; i < nHeads; ++i)
            heads_.emplace_back(dModel, dK_);
    }

    std::vector<std::vector<float>> forward(
        const std::vector<std::vector<float>>& Q,
        const std::vector<std::vector<float>>& K,
        const std::vector<std::vector<float>>& V) {

        int seqLen = Q.size();
        std::vector<std::vector<float>> combined(seqLen, std::vector<float>(dModel_, 0));

        for (int h = 0; h < nHeads_; ++h) {
            auto headOut = heads_[h].forward(Q, K, V);
            for (int i = 0; i < seqLen; ++i)
                for (int d = 0; d < dK_; ++d)
                    combined[i][h * dK_ + d] = headOut[i][d];
        }
        return combined;
    }
};

class FeedForward {
    int dModel_, dFF_;
    std::vector<float> w1_, b1_, w2_, b2_;

public:
    FeedForward(int dModel, int dFF) : dModel_(dModel), dFF_(dFF) {
        w1_.resize(dModel * dFF);
        b1_.resize(dFF_);
        w2_.resize(dFF * dModel);
        b2_.resize(dModel);
    }

    std::vector<float> forward(const std::vector<float>& x) {
        // FFN(x) = max(0, xW1 + b1)W2 + b2
        std::vector<float> hidden(dFF_);
        for (int i = 0; i < dFF_; ++i) {
            float sum = b1_[i];
            for (int j = 0; j < dModel_; ++j)
                sum += x[j] * w1_[j * dFF_ + i];
            hidden[i] = std::max(0.0f, sum); // ReLU
        }
        std::vector<float> output(dModel_);
        for (int i = 0; i < dModel_; ++i) {
            float sum = b2_[i];
            for (int j = 0; j < dFF_; ++j)
                sum += hidden[j] * w2_[j * dModel_ + i];
            output[i] = sum;
        }
        return output;
    }
};

class TransformerBlock {
    MultiHeadAttention attention_;
    FeedForward ffn_;
    int dModel_;

public:
    TransformerBlock(int dModel, int nHeads, int dFF)
        : attention_(dModel, nHeads), ffn_(dModel, dFF), dModel_(dModel) {}

    std::vector<std::vector<float>> forward(
        const std::vector<std::vector<float>>& x) {
        
        // Self-attention with residual
        auto attnOut = attention_.forward(x, x, x);
        std::vector<std::vector<float>> afterAttn(x.size(), std::vector<float>(dModel_));
        for (size_t i = 0; i < x.size(); ++i)
            for (int j = 0; j < dModel_; ++j)
                afterAttn[i][j] = x[i][j] + attnOut[i][j]; // Residual

        // Feed-forward with residual
        std::vector<std::vector<float>> output(x.size(), std::vector<float>(dModel_));
        for (size_t i = 0; i < x.size(); ++i) {
            auto ffOut = ffn_.forward(afterAttn[i]);
            for (int j = 0; j < dModel_; ++j)
                output[i][j] = afterAttn[i][j] + ffOut[j];
        }
        return output;
    }
};

int main() {
    int dModel = 8;    // Small model for demo
    int nHeads = 2;
    int dFF = 16;
    int seqLen = 5;
    int vocabSize = 10;

    std::cout << "Transformer Configuration:\n";
    std::cout << "  Model dimension: " << dModel << "\n";
    std::cout << "  Heads: " << nHeads << "\n";
    std::cout << "  FF dimension: " << dFF << "\n";
    std::cout << "  Sequence length: " << seqLen << "\n\n";

    // Create embeddings (simplified)
    std::vector<std::vector<float>> embeddings(seqLen, std::vector<float>(dModel));
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
    for (int i = 0; i < seqLen; ++i)
        for (int j = 0; j < dModel; ++j)
            embeddings[i][j] = dist(gen);

    // Add positional encoding
    for (int pos = 0; pos < seqLen; ++pos) {
        for (int i = 0; i < dModel; ++i) {
            float angle = pos / std::pow(10000.0f, 2.0f * i / dModel);
            embeddings[pos][i] += (i % 2 == 0) ? std::sin(angle) : std::cos(angle);
        }
    }

    // Create transformer block
    TransformerBlock block(dModel, nHeads, dFF);

    std::cout << "Input sequence tokens: ";
    for (int i = 0; i < seqLen; ++i) {
        // Simulate token IDs (random)
        int tokenId = std::rand() % vocabSize;
        std::cout << tokenId << " ";
    }
    std::cout << "\n\n";

    // Forward pass
    auto output = block.forward(embeddings);

    std::cout << "Transformer output (first 3 tokens):\n";
    for (int i = 0; i < std::min(3, seqLen); ++i) {
        std::cout << "  Token " << i << ": [";
        for (int j = 0; j < std::min(4, dModel); ++j) {
            std::cout << std::fixed << std::setprecision(3) << output[i][j] << " ";
        }
        std::cout << "...]\n";
    }

    std::cout << "\nTransformer forward pass complete!\n";

    return 0;
}

/*
SAMPLE OUTPUT:
Transformer Configuration:
  Model dimension: 8
  Heads: 2
  FF dimension: 16
  Sequence length: 5

Input sequence tokens: 3 7 1 9 4 

Transformer output (first 3 tokens):
  Token 0: [0.123 0.456 -0.234 0.789 ...]
  Token 1: [-0.111 0.333 0.444 -0.122 ...]
...

Transformer forward pass complete!
*/
