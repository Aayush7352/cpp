/*
THEORY: Strategy Pattern
- Behavioral pattern for interchangeable algorithms
- Encapsulates algorithms in separate strategy classes
- Runtime selection of behavior via composition
- Open/Closed Principle: new strategies without modifying context
- Can use std::function for lightweight strategies

Complexity: O(1) strategy switch
Interview Questions:
1. Strategy vs State pattern difference?
2. How to pass strategy-specific parameters?
3. When to use lambda vs strategy class?
Common Mistakes:
- Making strategies stateful causing thread-safety issues
- Tight coupling between context and strategy interfaces
- Over-engineering simple conditionals into strategies
*/

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>

class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<int>& data) const = 0;
    virtual std::string name() const = 0;
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override {
        for (size_t i = 0; i < data.size() - 1; ++i)
            for (size_t j = 0; j < data.size() - i - 1; ++j)
                if (data[j] > data[j + 1]) std::swap(data[j], data[j + 1]);
    }
    std::string name() const override { return "Bubble Sort"; }
};

class QuickSort : public SortStrategy {
    void quickSort(std::vector<int>& data, int low, int high) const {
        if (low < high) {
            int pivot = partition(data, low, high);
            quickSort(data, low, pivot - 1);
            quickSort(data, pivot + 1, high);
        }
    }
    int partition(std::vector<int>& data, int low, int high) const {
        int pivot = data[high];
        int i = low - 1;
        for (int j = low; j < high; ++j)
            if (data[j] <= pivot) std::swap(data[++i], data[j]);
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }
public:
    void sort(std::vector<int>& data) const override {
        quickSort(data, 0, data.size() - 1);
    }
    std::string name() const override { return "Quick Sort"; }
};

class STLSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override {
        std::sort(data.begin(), data.end());
    }
    std::string name() const override { return "STL Sort"; }
};

class Sorter {
    std::unique_ptr<SortStrategy> strategy_;
public:
    explicit Sorter(std::unique_ptr<SortStrategy> strategy) 
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<SortStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    void execute(std::vector<int>& data) const {
        std::cout << "Using " << strategy_->name() << ": ";
        auto copy = data;
        strategy_->sort(copy);
        for (int x : copy) std::cout << x << " ";
        std::cout << "\n";
    }
};

int main() {
    std::vector<int> data = {64, 34, 25, 12, 22, 11, 90};
    std::cout << "Original: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n\n";

    Sorter sorter(std::make_unique<BubbleSort>());
    sorter.execute(data);

    sorter.setStrategy(std::make_unique<QuickSort>());
    sorter.execute(data);

    sorter.setStrategy(std::make_unique<STLSort>());
    sorter.execute(data);

    // Lambda-based strategy
    std::cout << "\nLambda strategy (descending): ";
    auto copy = data;
    std::sort(copy.begin(), copy.end(), std::greater{});
    for (int x : copy) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}

/*
SAMPLE OUTPUT:
Original: 64 34 25 12 22 11 90 

Using Bubble Sort: 11 12 22 25 34 64 90 
Using Quick Sort: 11 12 22 25 34 64 90 
Using STL Sort: 11 12 22 25 34 64 90 

Lambda strategy (descending): 90 64 34 25 22 12 11
*/
