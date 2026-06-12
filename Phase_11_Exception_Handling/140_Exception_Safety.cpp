/*
==================================================================
140. Exception Safety
==================================================================
Theory:
- Basic guarantee: invariants preserved, no resource leaks
- Strong guarantee: operation succeeds or state is unchanged
  (commit-or-rollback)
- Nothrow guarantee: operation never throws (noexcept)
- RAII (Resource Acquisition Is Initialization): tie resource
  lifetime to object lifetime
- The key: always use RAII wrappers (vector, string, unique_ptr)
- Copy-and-swap idiom for strong guarantee
*/

#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <cstring>
#include <utility>

// ---- Nothrow Guarantee ----
int safeAdd(int a, int b) noexcept {
    // Integer addition doesn't throw
    return a + b;
}

// ---- Basic Guarantee ----
class StringStorage {
    // Basic: if operation fails, object is still usable (no leak)
    std::unique_ptr<char[]> m_data;
    size_t m_size = 0;

public:
    explicit StringStorage(const char* s)
        : m_size(std::strlen(s))
        , m_data(std::make_unique<char[]>(m_size + 1))
    {
        std::strcpy(m_data.get(), s);
    }

    void append(const char* suffix) {
        size_t newLen = m_size + std::strlen(suffix);
        auto newData = std::make_unique<char[]>(newLen + 1);
        std::strcpy(newData.get(), m_data.get());
        std::strcat(newData.get(), suffix);
        // Only after successful allocation do we replace
        m_data = std::move(newData);
        m_size = newLen;
    }

    const char* c_str() const noexcept { return m_data.get(); }
};

// ---- Strong Guarantee (Copy-and-Swap) ----
class Widget {
    std::vector<int> m_data;
    int m_id;

public:
    Widget(int id, std::initializer_list<int> il)
        : m_id(id), m_data(il) {}

    // Copy-and-swap assignment for strong guarantee
    Widget& operator=(Widget other) noexcept {
        // 'other' is constructed via copy (might throw)
        // but once here, swap is noexcept
        swap(other);
        return *this;
    }

    void swap(Widget& other) noexcept {
        using std::swap;
        swap(m_data, other.m_data);
        swap(m_id, other.m_id);
    }

    // Operation with strong guarantee
    void addValue(int val) {
        // First compute on a copy; swap only if success
        Widget tmp = *this;  // copy might throw
        tmp.m_data.push_back(val); // might throw
        swap(tmp); // noexcept -> strong guarantee
    }

    void display() const {
        std::cout << "Widget(" << m_id << "): {";
        for (size_t i = 0; i < m_data.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << m_data[i];
        }
        std::cout << "}\n";
    }
};

// ---- RAII example ----
class DatabaseConnection {
    bool m_open = false;
public:
    DatabaseConnection() {
        std::cout << "  DB: Acquire connection\n";
        m_open = true;
    }
    ~DatabaseConnection() {
        if (m_open) {
            std::cout << "  DB: Release connection\n";
            m_open = false;
        }
    }
    void query(const char* q) {
        if (!m_open)
            throw std::logic_error("Not connected");
        std::cout << "  DB: Query '" << q << "'\n";
    }
};

void databaseOperation() {
    DatabaseConnection conn; // RAII: always released
    conn.query("SELECT * FROM users");
    // If query throws, conn destructor still runs
    std::cout << "  DB: Operation done\n";
}

int main() {
    std::cout << "=== Exception Safety Guarantees\n\n";

    // Nothrow
    std::cout << "Nothrow: safeAdd(3,4) = "
              << safeAdd(3, 4) << "\n\n";

    // Basic guarantee
    std::cout << "Basic guarantee (no leaks on failure):\n";
    StringStorage ss("Hello");
    ss.append(" World");
    std::cout << "  String: " << ss.c_str() << "\n\n";

    // Strong guarantee
    std::cout << "Strong guarantee (commit-or-rollback):\n";
    Widget w(1, {1, 2, 3});
    w.display();
    try {
        w.addValue(4); // succeeds
        w.display();
        // Simulate failure by running out of memory
        // (In practice, push_back might throw bad_alloc)
    } catch (...) {
        std::cout << "  Failed, but Widget unchanged\n";
    }
    std::cout << "\n";

    // RAII
    std::cout << "RAII exception safety:\n";
    databaseOperation();

    // Exception during construction
    std::cout << "\nException during construction (no leak):\n";
    try {
        std::vector<int> big(1000000); // might throw bad_alloc
        std::cout << "  Success\n";
    } catch (const std::bad_alloc&) {
        std::cout << "  bad_alloc caught (no leak)\n";
    }

    return 0;
}

/*
==================================================================
Sample I/O:
Nothrow: safeAdd(3,4) = 7

Basic guarantee (no leaks on failure):
  String: Hello World

Strong guarantee (commit-or-rollback):
Widget(1): {1, 2, 3}
Widget(1): {1, 2, 3, 4}

RAII exception safety:
  DB: Acquire connection
  DB: Query 'SELECT * FROM users'
  DB: Operation done
  DB: Release connection

Exception during construction (no leak):
  Success
==================================================================
Complexity:
- RAII: O(1) overhead for destructor
- Copy-and-swap: O(n) for copy + O(1) swap
==================================================================
Interview Questions:
1. What are the three exception safety levels?
   -> Basic, Strong, Nothrow. Basic: no leaks, valid state.
      Strong: commit-or-rollback. Nothrow: never throws.

2. How is the strong guarantee achieved?
   -> Copy-and-swap: make changes on a temp, then noexcept swap.

3. Why is RAII critical for exception safety?
   -> Destructors run during stack unwinding, automatically
      releasing resources regardless of how function exits.

4. Can a noexcept function still throw?
   -> If it does, std::terminate is called. No stack unwinding
      guaranteed.
==================================================================
Common Mistakes:
- Leaking memory when new throws (use vector/unique_ptr)
- Throwing from destructor (double fault)
- Assigning to *this before the operation completes
- Assuming new(nothrow) returns null instead of throwing bad_alloc
- Not providing basic guarantee after partial modification
==================================================================
*/
