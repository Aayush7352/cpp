/*
THEORY: Hierarchical Inheritance

Hierarchical inheritance: one base class, multiple derived classes.
- All derived classes share the same base interface/implementation.
- Each derived class can extend/override independently.
- Common base class ensures consistent interface.
- Upcasting works for all derived types.

Real-world examples:
- Shape -> Circle, Rectangle, Triangle
- Animal -> Dog, Cat, Bird
- Employee -> Manager, Engineer, Salesperson
*/

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Payment {
protected:
    double amount_;
    std::string transactionId_;

public:
    Payment(double amount, const std::string& txnId)
        : amount_{amount}, transactionId_{txnId} {
        std::cout << "Payment ctor: $" << amount << '\n';
    }

    virtual ~Payment() { std::cout << "Payment dtor\n"; }

    virtual void process() const {
        std::cout << "Processing payment...\n";
    }

    virtual std::string receipt() const {
        return "Txn: " + transactionId_ + " | $" + std::to_string(amount_);
    }

    double getAmount() const { return amount_; }
};

class CreditCardPayment : public Payment {
    std::string cardLast4_;

public:
    CreditCardPayment(double amount, const std::string& txnId, const std::string& cardLast4)
        : Payment{amount, txnId}
        , cardLast4_{cardLast4} {
        std::cout << "CreditCardPayment ctor\n";
    }

    ~CreditCardPayment() override { std::cout << "CreditCardPayment dtor\n"; }

    void process() const override {
        std::cout << "Charging $" << amount_
                  << " to card ending in " << cardLast4_ << "...\n";
    }

    std::string receipt() const override {
        return Payment::receipt() + " | Card: ****" + cardLast4_;
    }
};

class PayPalPayment : public Payment {
    std::string email_;

public:
    PayPalPayment(double amount, const std::string& txnId, const std::string& email)
        : Payment{amount, txnId}
        , email_{email} {
        std::cout << "PayPalPayment ctor\n";
    }

    ~PayPalPayment() override { std::cout << "PayPalPayment dtor\n"; }

    void process() const override {
        std::cout << "Processing PayPal payment from " << email_ << "...\n";
        std::cout << "  $" << amount_ << " transferred.\n";
    }

    std::string receipt() const override {
        return Payment::receipt() + " | PayPal: " + email_;
    }
};

class CashPayment : public Payment {
public:
    CashPayment(double amount, const std::string& txnId)
        : Payment{amount, txnId} {
        std::cout << "CashPayment ctor\n";
    }

    ~CashPayment() override { std::cout << "CashPayment dtor\n"; }

    void process() const override {
        std::cout << "Collecting $" << amount_ << " in cash.\n";
    }

    std::string receipt() const override {
        return Payment::receipt() + " | Cash";
    }
};

void checkout(Payment& payment) {
    std::cout << "\n--- Checkout ---\n";
    payment.process();
    std::cout << "Receipt: " << payment.receipt() << '\n';
}

int main() {
    // 1. Individual derived objects
    std::cout << "=== Hierarchical Payment Types ===\n";
    CreditCardPayment cc{99.99, "TXN001", "1234"};
    PayPalPayment pp{49.99, "TXN002", "alice@example.com"};
    CashPayment cash{20.00, "TXN003"};

    std::cout << "\n=== Processing Each ===\n";
    cc.process();
    pp.process();
    cash.process();

    // 2. Polymorphic collection
    std::cout << "\n=== Payment Collection ===\n";
    std::vector<std::unique_ptr<Payment>> payments;
    payments.push_back(std::make_unique<CreditCardPayment>(150.00, "TXN004", "5678"));
    payments.push_back(std::make_unique<PayPalPayment>(75.00, "TXN005", "bob@example.com"));
    payments.push_back(std::make_unique<CashPayment>(30.00, "TXN006"));

    for (const auto& p : payments) {
        checkout(*p);
    }

    // 3. Base pointer/reference
    std::cout << "\n=== Base Pointer ===\n";
    Payment* ptr{&cc};
    ptr->process();                                  // CreditCardPayment::process

    Payment& ref{pp};
    ref.process();                                   // PayPalPayment::process

    return 0;
}

/*
SAMPLE I/O:
=== Hierarchical Payment Types ===
Payment ctor: $99.99
CreditCardPayment ctor
Payment ctor: $49.99
PayPalPayment ctor
Payment ctor: $20
CashPayment ctor

=== Processing Each ===
Charging $99.99 to card ending in 1234...
Processing PayPal payment from alice@example.com...
  $49.99 transferred.
Collecting $20.00 in cash.

=== Payment Collection ===
Payment ctor: $150
CreditCardPayment ctor
Payment ctor: $75
PayPalPayment ctor
Payment ctor: $30
CashPayment ctor

--- Checkout ---
Charging $150.00 to card ending in 5678...
Receipt: Txn: TXN004 | $150.000000 | Card: ****5678

--- Checkout ---
Processing PayPal payment from bob@example.com...
  $75.00 transferred.
Receipt: Txn: TXN005 | $75.000000 | PayPal: bob@example.com

--- Checkout ---
Collecting $30.00 in cash.
Receipt: Txn: TXN006 | $30.000000 | Cash

=== Base Pointer ===
Charging $99.99 to card ending in 1234...
Processing PayPal payment from alice@example.com...

COMPLEXITY:
- Construction: O(1) per object
- Virtual dispatch: O(1) per call

INTERVIEW QUESTIONS:
1. What is hierarchical inheritance vs multilevel?
   Hierarchical: one base, many derived (1 parent, N children).
   Multilevel: chain (grandparent -> parent -> child).

2. Can derived classes from the same base share data?
   Not directly. They share the base interface but have independent instances.
   For shared data, use static members in base.

3. When is hierarchical inheritance appropriate?
   When you have a common concept (Shape, Payment, Animal) with multiple
   concrete specializations. Follows the "is-a" relationship naturally.

COMMON MISTAKES:
- Duplicating code across derived classes (should be in base if common)
- Making base class too specific (violates Liskov substitution)
- Forgetting virtual destructor when deleting via base pointer
- Not overriding all pure virtual functions (class becomes abstract)
- Assuming all derived classes need the same interface
- Using dynamic_cast too often (design smell — prefer virtual functions)
*/
