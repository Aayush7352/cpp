/*
THEORY: Observer Pattern
- Behavioral design pattern for one-to-many dependency
- Subject notifies observers of state changes
- Uses std::function callbacks for flexibility
- Supports attach/detach at runtime
- Decouples subject from observer implementations

Complexity: O(n) notify where n = observers
Interview Questions:
1. Difference between Observer and Pub-Sub?
2. How to handle observer lifetime? (weak_ptr)
3. How to avoid notification storms?
Common Mistakes:
- Modifying observer list during notification
- Forgetting to detach causing dangling references
- Thread safety issues with concurrent attach/notify
*/

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>

template<typename T>
class Observable {
    std::vector<std::function<void(const T&)>> observers_;
    
public:
    void attach(std::function<void(const T&)> observer) {
        observers_.push_back(std::move(observer));
    }

    void detach(const std::function<void(const T&)>&) {
        // In practice, use tokens or IDs for proper detachment
    }

    void notify(const T& data) {
        for (const auto& observer : observers_) {
            observer(data);
        }
    }
};

struct StockPrice {
    std::string symbol;
    double price;
    double change;
};

int main() {
    Observable<StockPrice> stockTicker;

    stockTicker.attach([](const StockPrice& sp) {
        std::cout << "[Display] " << sp.symbol << ": $" << sp.price
                  << " (" << (sp.change >= 0 ? "+" : "") << sp.change << "%)\n";
    });

    stockTicker.attach([](const StockPrice& sp) {
        if (sp.change > 5.0) {
            std::cout << "[Alert] " << sp.symbol << " surged " << sp.change << "%!\n";
        }
    });

    stockTicker.attach([](const StockPrice& sp) {
        std::cout << "[Logger] " << sp.symbol << " price updated\n";
    });

    stockTicker.notify({"AAPL", 175.50, 2.3});
    stockTicker.notify({"TSLA", 245.00, 7.8});
    stockTicker.notify({"MSFT", 380.20, -0.5});

    return 0;
}

/*
SAMPLE OUTPUT:
[Display] AAPL: $175.5 (+2.3%)
[Logger] AAPL price updated
[Display] TSLA: $245 (+7.8%)
[Alert] TSLA surged 7.8%!
[Logger] TSLA price updated
[Display] MSFT: $380.2 (-0.5%)
[Logger] MSFT price updated
*/
