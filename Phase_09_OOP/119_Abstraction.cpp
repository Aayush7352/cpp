/*
THEORY: Abstraction

Abstraction: exposing only essential features while hiding implementation details.

Interface design: abstract base class (or concept) with pure virtual functions.
Implementation hiding: users interact through pointer/reference to interface.

Pimpl Idiom (Pointer to Implementation):
- Header exposes only public interface.
- Implementation details hidden in .cpp file.
- Benefits: compile firewall, ABI stability, true encapsulation.
- Cost: runtime indirection via pointer.

Design patterns for abstraction:
- Strategy, Observer, Factory — all rely on abstraction.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class IShape {
public:
    virtual ~IShape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public IShape {
    double radius_;
public:
    explicit Circle(double r) : radius_{r} {}
    double area() const override { return 3.14159 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public IShape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_{w}, h_{h} {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "Rectangle"; }
};

void printShapeInfo(const IShape& shape) {
    std::cout << shape.name() << " area: " << shape.area() << '\n';
}

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual bool pay(double amount) = 0;
    virtual std::string name() const = 0;
};

class CreditCardProcessor : public PaymentProcessor {
    std::string cardNumber_;
public:
    explicit CreditCardProcessor(std::string cn) : cardNumber_{std::move(cn)} {}
    bool pay(double amount) override {
        std::cout << "Processing $" << amount << " via Credit Card " << cardNumber_.substr(0,4) << "****\n";
        return true;
    }
    std::string name() const override { return "CreditCard"; }
};

class PayPalProcessor : public PaymentProcessor {
    std::string email_;
public:
    explicit PayPalProcessor(std::string email) : email_{std::move(email)} {}
    bool pay(double amount) override {
        std::cout << "Processing $" << amount << " via PayPal (" << email_ << ")\n";
        return true;
    }
    std::string name() const override { return "PayPal"; }
};

void processPayment(PaymentProcessor& proc, double amount) {
    std::cout << "Using " << proc.name() << ": ";
    proc.pay(amount);
}

// --- PIMPL Idiom (complete) ---
class Database {
    class Impl;
    std::unique_ptr<Impl> pImpl;

public:
    Database(const std::string& connStr);
    ~Database();
    Database(Database&&) noexcept;
    Database& operator=(Database&&) noexcept;

    bool connect();
    void query(const std::string& sql);
    void disconnect();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};

class Database::Impl {
public:
    std::string connectionString;
    bool connected{false};

    explicit Impl(const std::string& cs) : connectionString{cs} {}
    bool doConnect() {
        std::cout << "Connecting to " << connectionString << "...\n";
        connected = true;
        return true;
    }
    void doQuery(const std::string& sql) {
        std::cout << "Executing: " << sql << '\n';
    }
    void doDisconnect() {
        if (connected) {
            std::cout << "Disconnecting...\n";
            connected = false;
        }
    }
};

Database::Database(const std::string& cs) : pImpl{std::make_unique<Impl>(cs)} {}
Database::~Database() = default;
Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;
bool Database::connect() { return pImpl->doConnect(); }
void Database::query(const std::string& sql) { pImpl->doQuery(sql); }
void Database::disconnect() { pImpl->doDisconnect(); }

int main() {
    // 1. Interface abstraction
    std::cout << "=== Shape Interface ===\n";
    Circle circle{5.0};
    Rectangle rect{4.0, 6.0};
    printShapeInfo(circle);
    printShapeInfo(rect);

    std::vector<std::unique_ptr<IShape>> shapes;
    shapes.push_back(std::make_unique<Circle>(3.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 5.0));
    std::cout << "\nPolymorphic shapes:\n";
    for (const auto& s : shapes) {
        std::cout << "  " << s->name() << " area: " << s->area() << '\n';
    }

    // 2. Strategy pattern abstraction
    std::cout << "\n=== Payment Strategy ===\n";
    CreditCardProcessor cc{"1234567890123456"};
    PayPalProcessor pp{"alice@example.com"};

    processPayment(cc, 99.99);
    processPayment(pp, 49.99);

    // 3. PIMPL abstraction
    std::cout << "\n=== PIMPL Idiom ===\n";
    Database db{"host=localhost;db=mydb"};
    db.connect();
    db.query("SELECT * FROM users");
    db.disconnect();

    // Implementation completely hidden:
    // Can't access db.pImpl, db.pImpl->connectionString, etc.

    return 0;
}

/*
SAMPLE I/O:
=== Shape Interface ===
Circle area: 78.5397
Rectangle area: 24

Polymorphic shapes:
  Circle area: 28.2743
  Rectangle area: 10

=== Payment Strategy ===
Using CreditCard: Processing $99.99 via Credit Card 1234****
Using PayPal: Processing $49.99 via PayPal (alice@example.com)

=== PIMPL Idiom ===
Connecting to host=localhost;db=mydb...
Executing: SELECT * FROM users
Disconnecting...

COMPLEXITY:
- Virtual function call: O(1) via vtable
- PIMPL: O(1) per operation (one extra indirection)

INTERVIEW QUESTIONS:
1. What is the difference between abstraction and encapsulation?
   Encapsulation: hiding data (private, getters/setters).
   Abstraction: hiding implementation (interface, pure virtual functions).
   Encapsulation is about data hiding; abstraction is about complexity hiding.

2. When would you use PIMPL?
   When you need:
   - Compile-time firewall (change impl without recompiling headers)
   - Binary compatibility (library evolution)
   - True implementation hiding (no leak of private members in header)

3. What are the costs of abstraction?
   Runtime: virtual dispatch (vtable lookup, prevents inlining).
   Memory: vtable pointer per object (typically 8 bytes).
   Code complexity: more files, more indirection.

COMMON MISTAKES:
- Making an entire class abstract when only some parts need abstraction
- Not declaring virtual destructor in interface classes
- Using PIMPL where not needed (unnecessary complexity)
- Breaking the interface contract in derived classes
- Exposing implementation details in header despite using PIMPL
- Forgetting =0 on pure virtual functions, making class concrete unintentionally
*/
