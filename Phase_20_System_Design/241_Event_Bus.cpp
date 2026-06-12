/*
THEORY: Event Bus (Pub-Sub)
- Decouples event producers from consumers
- Type-erased event handlers using std::function
- Supports any event type via template methods
- Synchronous dispatch by default, async option
- Thread-safe subscription and dispatch

Complexity: O(n) dispatch where n = subscribers
Interview Questions:
1. How to avoid dangling handlers? (weak_ptr, token-based unsub)
2. How to handle event prioritization?
3. How to implement async event bus? (event queue + worker)
Common Mistakes:
- Subscriber lifetime issues (dangling pointers)
- Modifying subscriber list during dispatch
- Forgetting to unsubscribe causing memory leaks
*/

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <typeindex>
#include <mutex>
#include <any>

class EventBus {
    using Handler = std::function<void(const std::any&)>;
    std::unordered_map<std::type_index, std::vector<Handler>> handlers_;
    mutable std::mutex mutex_;

public:
    template<typename Event>
    void subscribe(std::function<void(const Event&)> handler) {
        std::lock_guard lock(mutex_);
        handlers_[std::type_index(typeid(Event))].push_back(
            [handler = std::move(handler)](const std::any& event) {
                handler(std::any_cast<const Event&>(event));
            }
        );
    }

    template<typename Event>
    void publish(const Event& event) {
        std::lock_guard lock(mutex_);
        auto it = handlers_.find(std::type_index(typeid(event)));
        if (it != handlers_.end()) {
            for (const auto& handler : it->second) {
                handler(std::any(event));
            }
        }
    }

    template<typename Event>
    size_t subscriberCount() const {
        std::lock_guard lock(mutex_);
        auto it = handlers_.find(std::type_index(typeid(Event)));
        return it != handlers_.end() ? it->second.size() : 0;
    }
};

struct UserLoggedIn {
    std::string username;
    int userId;
};

struct DataSaved {
    std::string collection;
    size_t recordCount;
};

int main() {
    EventBus bus;

    bus.subscribe<UserLoggedIn>([](const UserLoggedIn& e) {
        std::cout << "User logged in: " << e.username 
                  << " (ID: " << e.userId << ")\n";
    });

    bus.subscribe<UserLoggedIn>([](const UserLoggedIn& e) {
        std::cout << "[Audit] Login event: " << e.username << "\n";
    });

    bus.subscribe<DataSaved>([](const DataSaved& e) {
        std::cout << "Data saved: " << e.recordCount << " records in " 
                  << e.collection << "\n";
    });

    bus.publish(UserLoggedIn{"alice", 1});
    bus.publish(DataSaved{"users", 5});
    
    std::cout << "UserLoggedIn subscribers: " 
              << bus.subscriberCount<UserLoggedIn>() << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
User logged in: alice (ID: 1)
[Audit] Login event: alice
Data saved: 5 records in users
UserLoggedIn subscribers: 2
*/
