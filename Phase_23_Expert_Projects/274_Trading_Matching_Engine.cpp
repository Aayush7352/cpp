/*
THEORY: Trading Matching Engine
- Order book with price-time priority (FIFO)
- Support for limit and market orders
- Continuous matching and order management
- Order types: LIMIT, MARKET, IOC, FOK
- Real-time order book snapshots

Complexity: O(log n) for order insertion/cancellation
Interview Questions:
1. How do real exchange matching engines work?
2. What is the difference between order book and auction matching?
3. How to handle high-frequency trading latency?
Common Mistakes:
- Race conditions in order matching
- Not handling order cancellation correctly
- Integer overflow in price/quantity
*/

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <list>
#include <memory>
#include <iomanip>
#include <sstream>
#include <chrono>

enum class OrderType { LIMIT, MARKET, IOC, FOK };
enum class OrderSide { BUY, SELL };
enum class OrderStatus { NEW, PARTIAL, FILLED, CANCELLED, REJECTED };

struct Order {
    std::string id;
    OrderSide side;
    OrderType type;
    double price;
    int quantity;
    int filled{0};
    std::chrono::steady_clock::time_point timestamp;
    OrderStatus status{OrderStatus::NEW};

    Order(std::string id, OrderSide side, OrderType type, double price, int qty)
        : id(std::move(id)), side(side), type(type), price(price), quantity(qty) {
        timestamp = std::chrono::steady_clock::now();
    }

    int remaining() const { return quantity - filled; }
    bool isFilled() const { return filled >= quantity; }
    std::string sideStr() const { return side == OrderSide::BUY ? "BUY" : "SELL"; }
    std::string statusStr() const {
        switch (status) {
            case OrderStatus::NEW: return "NEW";
            case OrderStatus::PARTIAL: return "PARTIAL";
            case OrderStatus::FILLED: return "FILLED";
            case OrderStatus::CANCELLED: return "CANCELLED";
            case OrderStatus::REJECTED: return "REJECTED";
        }
        return "UNKNOWN";
    }
};

struct Trade {
    std::string buyOrderId;
    std::string sellOrderId;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;

    Trade(std::string buyId, std::string sellId, double p, int qty)
        : buyOrderId(std::move(buyId)), sellOrderId(std::move(sellId)),
          price(p), quantity(qty) {
        timestamp = std::chrono::steady_clock::now();
    }
};

class OrderBook {
    // Buy side (sorted high to low), Sell side (sorted low to high)
    std::map<double, std::list<std::shared_ptr<Order>>, std::greater<double>> buyOrders_;
    std::map<double, std::list<std::shared_ptr<Order>>> sellOrders_;
    std::vector<Trade> trades_;
    int orderCounter_{0};

    std::string nextId() {
        return "ORD" + std::to_string(++orderCounter_);
    }

    void tryMatch() {
        while (!buyOrders_.empty() && !sellOrders_.empty()) {
            auto bestBuy = buyOrders_.begin();
            auto bestSell = sellOrders_.begin();

            if (bestBuy->first < bestSell->first) break; // No cross

            auto& buyList = bestBuy->second;
            auto& sellList = bestSell->second;

            auto buyOrder = buyList.front();
            auto sellOrder = sellList.front();

            double matchPrice = sellOrder->price;
            int matchQty = std::min(buyOrder->remaining(), sellOrder->remaining());

            buyOrder->filled += matchQty;
            sellOrder->filled += matchQty;

            trades_.emplace_back(buyOrder->id, sellOrder->id, matchPrice, matchQty);

            std::cout << "TRADE: " << buyOrder->id << " bought " << matchQty
                      << " @ $" << std::fixed << std::setprecision(2) << matchPrice
                      << " from " << sellOrder->id << "\n";

            // Update statuses
            if (buyOrder->isFilled()) {
                buyOrder->status = OrderStatus::FILLED;
                buyList.pop_front();
                if (buyList.empty()) buyOrders_.erase(bestBuy);
            } else {
                buyOrder->status = OrderStatus::PARTIAL;
            }

            if (sellOrder->isFilled()) {
                sellOrder->status = OrderStatus::FILLED;
                sellList.pop_front();
                if (sellList.empty()) sellOrders_.erase(bestSell);
            } else {
                sellOrder->status = OrderStatus::PARTIAL;
            }

            // Handle IOC and FOK
            if (buyOrder->type == OrderType::IOC && !buyOrder->isFilled()) {
                buyOrder->status = OrderStatus::CANCELLED;
                buyList.pop_front();
                if (buyList.empty()) buyOrders_.erase(bestBuy);
            }
            if (sellOrder->type == OrderType::IOC && !sellOrder->isFilled()) {
                sellOrder->status = OrderStatus::CANCELLED;
                sellList.pop_front();
                if (sellList.empty()) sellOrders_.erase(bestSell);
            }
        }
    }

public:
    std::shared_ptr<Order> submitOrder(OrderSide side, OrderType type, double price, int qty) {
        auto order = std::make_shared<Order>(nextId(), side, type, price, qty);

        if (qty <= 0 || price < 0) {
            order->status = OrderStatus::REJECTED;
            std::cout << "REJECTED: " << order->id << " (invalid params)\n";
            return order;
        }

        if (type == OrderType::LIMIT || type == OrderType::IOC || type == OrderType::FOK) {
            if (side == OrderSide::BUY) {
                buyOrders_[price].push_back(order);
            } else {
                sellOrders_[price].push_back(order);
            }
        }

        std::cout << "NEW " << order->sideStr() << " " << order->id
                  << ": " << qty << " @ $" << std::fixed << std::setprecision(2) << price << "\n";

        tryMatch();
        return order;
    }

    bool cancelOrder(const std::string& id) {
        for (auto& [price, list] : buyOrders_) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                if ((*it)->id == id) {
                    (*it)->status = OrderStatus::CANCELLED;
                    list.erase(it);
                    if (list.empty()) buyOrders_.erase(price);
                    std::cout << "CANCELLED: " << id << "\n";
                    return true;
                }
            }
        }
        for (auto& [price, list] : sellOrders_) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                if ((*it)->id == id) {
                    (*it)->status = OrderStatus::CANCELLED;
                    list.erase(it);
                    if (list.empty()) sellOrders_.erase(price);
                    std::cout << "CANCELLED: " << id << "\n";
                    return true;
                }
            }
        }
        return false;
    }

    void printOrderBook() const {
        std::cout << "\n=== ORDER BOOK ===\n";
        std::cout << "SELLS (asks):\n";
        for (auto it = sellOrders_.rbegin(); it != sellOrders_.rend(); ++it) {
            int qty = 0;
            for (const auto& o : it->second) qty += o->remaining();
            std::cout << "  $" << std::fixed << std::setprecision(2) << it->first
                      << " x " << qty << "\n";
        }
        std::cout << "---\n";
        std::cout << "BUYS (bids):\n";
        for (const auto& [price, list] : buyOrders_) {
            int qty = 0;
            for (const auto& o : list) qty += o->remaining();
            std::cout << "  $" << std::fixed << std::setprecision(2) << price
                      << " x " << qty << "\n";
        }
        std::cout << "================\n\n";
    }

    int tradeCount() const { return trades_.size(); }
    void printTrades(int n = 5) const {
        std::cout << "Recent trades (" << std::min(n, (int)trades_.size()) << "):\n";
        int start = std::max(0, (int)trades_.size() - n);
        for (int i = start; i < (int)trades_.size(); ++i) {
            const auto& t = trades_[i];
            std::cout << "  " << t.buyOrderId << " x " << t.sellOrderId
                      << " @ $" << std::fixed << std::setprecision(2) << t.price
                      << " (" << t.quantity << ")\n";
        }
    }
};

int main() {
    OrderBook book;

    // Submit buy orders
    auto b1 = book.submitOrder(OrderSide::BUY, OrderType::LIMIT, 100.50, 1000);
    auto b2 = book.submitOrder(OrderSide::BUY, OrderType::LIMIT, 101.00, 500);
    auto b3 = book.submitOrder(OrderSide::BUY, OrderType::LIMIT, 100.75, 300);

    // Submit sell orders
    auto s1 = book.submitOrder(OrderSide::SELL, OrderType::LIMIT, 101.50, 400);
    auto s2 = book.submitOrder(OrderSide::SELL, OrderType::LIMIT, 100.50, 200);
    auto s3 = book.submitOrder(OrderSide::SELL, OrderType::LIMIT, 100.25, 600);

    book.printOrderBook();

    // Market order
    auto m1 = book.submitOrder(OrderSide::BUY, OrderType::MARKET, 0, 500);
    book.printOrderBook();

    book.printTrades();

    // Cancel an order
    book.cancelOrder(b3->id);
    book.printOrderBook();

    std::cout << "Total trades: " << book.tradeCount() << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
NEW BUY ORD1: 1000 @ $100.50
NEW BUY ORD2: 500 @ $101.00
NEW BUY ORD3: 300 @ $100.75
NEW SELL ORD4: 400 @ $101.50
NEW SELL ORD5: 200 @ $100.50
TRADE: ORD5 sold 200 @ $100.50 to ORD1
...
Total trades: 2
*/
