/*
THEORY: Builder Pattern
- Separates object construction from representation
- Step-by-step construction with fluent interface
- Same construction process can create different representations
- Immutable object creation with validation
- Useful for complex objects with many optional parameters

Complexity: O(1) per builder step
Interview Questions:
1. Builder vs Factory difference?
2. When to use Director in Builder pattern?
3. How to make builder thread-safe?
Common Mistakes:
- Not validating at build() time
- Mutable builder after build() (should reset or disable)
- Over-engineering for simple objects (use designated initializers in C++20)
*/

#include <iostream>
#include <string>
#include <vector>
#include <optional>

class Pizza {
public:
    enum Size { SMALL, MEDIUM, LARGE };
    enum Crust { THIN, THICK, STUFFED };
    enum Topping { CHEESE, PEPPERONI, MUSHROOMS, ONIONS, SAUSAGE, BACON };

private:
    Size size_;
    Crust crust_;
    std::vector<Topping> toppings_;
    bool extraCheese_;
    std::string specialInstructions_;

    Pizza(Size size, Crust crust, std::vector<Topping> toppings, 
          bool extraCheese, std::string instructions)
        : size_(size), crust_(crust), toppings_(std::move(toppings)),
          extraCheese_(extraCheese), specialInstructions_(std::move(instructions)) {}

    static std::string sizeName(Size s) {
        switch (s) { case SMALL: return "Small"; case MEDIUM: return "Medium"; case LARGE: return "Large"; }
    }
    static std::string crustName(Crust c) {
        switch (c) { case THIN: return "Thin"; case THICK: return "Thick"; case STUFFED: return "Stuffed"; }
    }
    static std::string toppingName(Topping t) {
        switch (t) {
            case CHEESE: return "Cheese"; case PEPPERONI: return "Pepperoni";
            case MUSHROOMS: return "Mushrooms"; case ONIONS: return "Onions";
            case SAUSAGE: return "Sausage"; case BACON: return "Bacon";
        }
    }

public:
    void describe() const {
        std::cout << sizeName(size_) << " " << crustName(crust_) << " pizza";
        if (!toppings_.empty()) {
            std::cout << " with ";
            for (size_t i = 0; i < toppings_.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << toppingName(toppings_[i]);
            }
        }
        if (extraCheese_) std::cout << " (+extra cheese)";
        if (!specialInstructions_.empty()) 
            std::cout << " [" << specialInstructions_ << "]";
        std::cout << "\n";
    }

    class Builder {
        Size size_{MEDIUM};
        Crust crust_{THIN};
        std::vector<Topping> toppings_;
        bool extraCheese_{false};
        std::string specialInstructions_;
        bool used_{false};
    public:
        Builder& setSize(Size s) { size_ = s; return *this; }
        Builder& setCrust(Crust c) { crust_ = c; return *this; }
        Builder& addTopping(Topping t) { toppings_.push_back(t); return *this; }
        Builder& extraCheese(bool v = true) { extraCheese_ = v; return *this; }
        Builder& instructions(const std::string& i) { specialInstructions_ = i; return *this; }
        
        Pizza build() {
            if (used_) throw std::runtime_error("Builder already used");
            used_ = true;
            if (toppings_.empty() && crust_ != THIN) {
                std::cout << "Warning: no toppings on special crust\n";
            }
            return Pizza(size_, crust_, std::move(toppings_), 
                        extraCheese_, std::move(specialInstructions_));
        }
    };
};

int main() {
    auto customPizza = Pizza::Builder()
        .setSize(Pizza::LARGE)
        .setCrust(Pizza::STUFFED)
        .addTopping(Pizza::PEPPERONI)
        .addTopping(Pizza::MUSHROOMS)
        .addTopping(Pizza::BACON)
        .extraCheese(true)
        .instructions("Cut into 8 slices")
        .build();

    auto basicPizza = Pizza::Builder()
        .setSize(Pizza::SMALL)
        .addTopping(Pizza::CHEESE)
        .build();

    std::cout << "Order 1: "; customPizza.describe();
    std::cout << "Order 2: "; basicPizza.describe();
    return 0;
}

/*
SAMPLE OUTPUT:
Order 1: Large Stuffed pizza with Pepperoni, Mushrooms, Bacon (+extra cheese) [Cut into 8 slices]
Order 2: Small Thin pizza with Cheese
*/
