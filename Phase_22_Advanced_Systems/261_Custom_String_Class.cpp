/*
THEORY: Custom String Class
- Implements a basic string class with SSO (Small String Optimization)
- Rule of Five: destructor, copy/move constructor, copy/move assignment
- Provides common string operations
- RAII-compliant memory management
- SSO stores small strings inline to avoid heap allocation

Complexity: O(n) operations generally, O(1) for small strings
Interview Questions:
1. What is SSO and typical buffer sizes? (15-23 bytes)
2. COW (Copy on Write) vs SSO trade-offs?
3. When to use string_view instead of string?
Common Mistakes:
- Off-by-one with null terminator
- Not handling self-assignment in copy assignment
- Not providing noexcept move operations
*/

#include <iostream>
#include <cstring>
#include <algorithm>
#include <stdexcept>

class String {
    static constexpr size_t SSO_SIZE = 15;
    union {
        char stack_[SSO_SIZE + 1];
        struct {
            char* heap_;
            size_t capacity_;
        };
    };
    size_t size_{0};
    bool usingHeap() const { return size_ > SSO_SIZE; }

    void ensureCapacity(size_t needed) {
        if (needed <= SSO_SIZE) return;
        size_t newCap = std::max(needed, size_ * 2);
        char* newData = new char[newCap + 1];
        if (usingHeap()) {
            std::memcpy(newData, heap_, size_);
            delete[] heap_;
        } else {
            std::memcpy(newData, stack_, size_);
        }
        heap_ = newData;
        capacity_ = newCap;
    }

public:
    String() : stack_{0}, size_(0) {}

    String(const char* str) {
        size_ = std::strlen(str);
        if (size_ > SSO_SIZE) {
            capacity_ = size_;
            heap_ = new char[capacity_ + 1];
            std::strcpy(heap_, str);
        } else {
            std::strcpy(stack_, str);
        }
    }

    String(const String& other) : size_(other.size_) {
        if (other.usingHeap()) {
            capacity_ = other.capacity_;
            heap_ = new char[capacity_ + 1];
            std::strcpy(heap_, other.heap_);
        } else {
            std::strcpy(stack_, other.stack_);
        }
    }

    String(String&& other) noexcept : size_(other.size_) {
        if (other.usingHeap()) {
            heap_ = other.heap_;
            capacity_ = other.capacity_;
            other.heap_ = nullptr;
        } else {
            std::strcpy(stack_, other.stack_);
        }
        other.size_ = 0;
        other.stack_[0] = '\0';
    }

    String& operator=(const String& other) {
        if (this != &other) {
            String tmp(other);
            swap(tmp);
        }
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            if (usingHeap()) delete[] heap_;
            size_ = other.size_;
            if (other.usingHeap()) {
                heap_ = other.heap_;
                capacity_ = other.capacity_;
                other.heap_ = nullptr;
            } else {
                std::strcpy(stack_, other.stack_);
            }
            other.size_ = 0;
            other.stack_[0] = '\0';
        }
        return *this;
    }

    ~String() {
        if (usingHeap()) delete[] heap_;
    }

    void swap(String& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(stack_, other.stack_);
    }

    const char* c_str() const {
        if (usingHeap()) return heap_;
        return stack_;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    char& operator[](size_t i) {
        if (usingHeap()) return heap_[i];
        return stack_[i];
    }

    const char& operator[](size_t i) const {
        if (usingHeap()) return heap_[i];
        return stack_[i];
    }

    String& operator+=(const String& other) {
        size_t newSize = size_ + other.size_;
        ensureCapacity(newSize);
        if (usingHeap()) {
            std::memcpy(heap_ + size_, other.c_str(), other.size_);
            heap_[newSize] = '\0';
        } else {
            std::memcpy(stack_ + size_, other.c_str(), other.size_);
            stack_[newSize] = '\0';
        }
        size_ = newSize;
        return *this;
    }

    friend String operator+(const String& a, const String& b) {
        String result(a);
        result += b;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        os << s.c_str();
        return os;
    }

    bool operator==(const String& other) const {
        return std::strcmp(c_str(), other.c_str()) == 0;
    }
};

int main() {
    String s1("Hello");  // Small string (SSO)
    String s2("World!");
    String s3 = s1 + " " + s2;
    std::cout << "s3: " << s3 << " (size: " << s3.size() << ")\n";

    // Large string (heap-allocated)
    String s4("This is a very long string that exceeds the SSO buffer size");
    std::cout << "s4: " << s4 << " (size: " << s4.size() << ")\n";

    // Copy
    String s5 = s4;
    std::cout << "s5 (copy): " << s5 << "\n";

    // Move
    String s6 = std::move(s4);
    std::cout << "s6 (moved): " << s6 << "\n";
    std::cout << "s4 (empty): '" << s4 << "'\n";

    std::cout << "s1 == s1: " << (s1 == s1) << "\n";
    std::cout << "s1 == s2: " << (s1 == s2) << "\n";

    return 0;
}

/*
SAMPLE OUTPUT:
s3: Hello World! (size: 12)
s4: This is a very long string that exceeds the SSO buffer size (size: 60)
s5 (copy): This is a very long string that exceeds the SSO buffer size
s6 (moved): This is a very long string that exceeds the SSO buffer size
s4 (empty): ''
s1 == s1: 1
s1 == s2: 0
*/
