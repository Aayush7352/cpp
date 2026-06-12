/*
THEORY: Encapsulation

Encapsulation: bundling data and methods that operate on that data,
restricting direct access to internal representation.

Access Specifiers:
- private: implementation details, hidden from users.
- protected: accessible to derived classes.
- public: interface.

Benefits:
- Data hiding: prevent invalid states.
- Abstraction: users depend on interface, not implementation.
- Maintainability: internal changes don't affect users.

Getters (accessors): read private data. Usually const.
Setters (mutators): write private data. Can validate/enforce invariants.
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class BankAccount {
    std::string owner_;
    double balance_;            // invariant: balance_ >= 0
    std::vector<std::string> transactionLog_;

    void log(const std::string& action) {
        transactionLog_.push_back(action);
    }

public:
    BankAccount(const std::string& owner, double initialDeposit = 0)
        : owner_{owner}, balance_{initialDeposit} {
        if (initialDeposit < 0)
            throw std::invalid_argument{"Initial deposit cannot be negative"};
        log("Account created with " + std::to_string(initialDeposit));
    }

    // Getter (accessor) — const, doesn't modify
    const std::string& getOwner() const { return owner_; }
    double getBalance() const { return balance_; }

    // Setter (mutator) with validation
    void deposit(double amount) {
        if (amount <= 0)
            throw std::invalid_argument{"Deposit amount must be positive"};
        balance_ += amount;
        log("Deposited " + std::to_string(amount));
    }

    bool withdraw(double amount) {
        if (amount <= 0)
            throw std::invalid_argument{"Withdrawal amount must be positive"};
        if (amount > balance_)
            return false;                   // invariant: no negative balance

        balance_ -= amount;
        log("Withdrew " + std::to_string(amount));
        return true;
    }

    // Return read-only view of logs
    size_t transactionCount() const { return transactionLog_.size(); }

    // Const member function — guarantees no modification
    void display() const {
        std::cout << "Account[ " << owner_ << " ]: $" << balance_ << '\n';
    }
};

// --- Encapsulation with PIMPL (Pointer to IMPLementation) ---
class Widget {
    class Impl;                  // forward declaration
    std::unique_ptr<Impl> pImpl;

public:
    Widget(const std::string& name);
    ~Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    void setName(const std::string& name);
    std::string getName() const;
    void display() const;

    // Prevent copying (or implement deep copy)
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};

// Implementation
class Widget::Impl {
public:
    std::string name;
    explicit Impl(const std::string& n) : name{n} {}
};

Widget::Widget(const std::string& name) : pImpl{std::make_unique<Impl>(name)} {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::setName(const std::string& name) { pImpl->name = name; }
std::string Widget::getName() const { return pImpl->name; }
void Widget::display() const { std::cout << "Widget: " << pImpl->name << '\n'; }

int main() {
    // 1. Encapsulated BankAccount
    std::cout << "=== Encapsulated BankAccount ===\n";
    BankAccount acc{"Alice", 1000};
    acc.display();

    acc.deposit(500);
    acc.withdraw(200);

    // Invalid operations prevented
    try {
        acc.deposit(-100);       // throws
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    bool success = acc.withdraw(2000);  // insufficient funds
    std::cout << "Withdrawal of 2000: " << (success ? "OK" : "Denied") << '\n';

    acc.display();
    std::cout << "Transactions: " << acc.transactionCount() << '\n';

    // Cannot directly access private members:
    // acc.balance_ = -100;  // ERROR: private

    // 2. PIMPL encapsulation
    std::cout << "\n=== PIMPL ===\n";
    Widget w{"Gadget"};
    w.display();
    w.setName("Super Gadget");
    w.display();

    // 3. Const-correct encapsulation
    std::cout << "\n=== Const correctness ===\n";
    const BankAccount& constRef{acc};
    std::cout << "Const access: " << constRef.getOwner()
              << ", $" << constRef.getBalance() << '\n';
    // constRef.deposit(1);  // ERROR: cannot call non-const on const ref

    return 0;
}

/*
SAMPLE I/O:
=== Encapsulated BankAccount ===
Account[ Alice ]: $1000
Error: Deposit amount must be positive
Withdrawal of 2000: Denied
Account[ Alice ]: $1300
Transactions: 3

=== PIMPL ===
Widget: Gadget
Widget: Super Gadget

=== Const correctness ===
Const access: Alice, $1300

COMPLEXITY:
- Getters/Setters: O(1)
- deposit/withdraw: O(1)

INTERVIEW QUESTIONS:
1. Why is encapsulation important?
   Prevents invalid state, reduces coupling, allows changing implementation
   without affecting users, enforces invariants.

2. What is the PIMPL idiom and why use it?
   Pointer to Implementation — hides implementation details from header.
   Benefits: faster compilation, binary compatibility, true interface hiding.
   Cost: indirection (heap allocation), slightly more code.

3. Should getters return const reference or by value?
   const reference for large objects (avoid copy), by value for small types
   (int, char, etc.). Never return non-const reference unless intended.

COMMON MISTAKES:
- Making data members public (breaks encapsulation)
- Returning non-const reference from getter (exposes internals)
- Forgetting to mark getters as const
- Not validating input in setters (invariant violations)
- Using friends excessively (weakens encapsulation)
- PIMPL without defining destructor in .cpp (incomplete type error)
*/
