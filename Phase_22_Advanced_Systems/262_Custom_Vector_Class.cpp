/*
THEORY: Custom Vector Class
- Dynamic array with amortized O(1) push_back
- RAII-compliant memory management
- Template for type safety
- Iterator support for range-based for loops
- Provides essential vector operations

Complexity: O(1) amortized push_back, O(n) insert/erase
Interview Questions:
1. What is the growth factor used by std::vector? (typically 1.5-2x)
2. Why does vector need both size and capacity?
3. What is the difference between reserve() and resize()?
Common Mistakes:
- Iterator invalidation after reallocation
- Not providing noexcept move operations
- Forgetting to call destructors before deallocation
*/

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <algorithm>

template<typename T>
class Vector {
    T* data_{nullptr};
    size_t size_{0};
    size_t capacity_{0};

    void reallocate(size_t newCap) {
        T* newData = static_cast<T*>(::operator new(newCap * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (newData + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = newData;
        capacity_ = newCap;
    }

public:
    Vector() = default;

    explicit Vector(size_t count, const T& value = T()) {
        reserve(count);
        for (size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    Vector(const Vector& other) {
        reserve(other.size_);
        for (const auto& v : other) push_back(v);
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            clear();
            reserve(other.size_);
            for (const auto& v : other) push_back(v);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~Vector() {
        clear();
        ::operator delete(data_);
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ ? capacity_ * 2 : 1);
        }
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ ? capacity_ * 2 : 1);
        }
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            reallocate(capacity_ ? capacity_ * 2 : 1);
        }
        T* ptr = new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
        return *ptr;
    }

    void pop_back() {
        if (size_ > 0) {
            data_[size_ - 1].~T();
            --size_;
        }
    }

    void reserve(size_t newCap) {
        if (newCap > capacity_) reallocate(newCap);
    }

    void resize(size_t newSize, const T& value = T()) {
        if (newSize < size_) {
            for (size_t i = newSize; i < size_; ++i) data_[i].~T();
        } else if (newSize > size_) {
            reserve(newSize);
            for (size_t i = size_; i < newSize; ++i) new (data_ + i) T(value);
        }
        size_ = newSize;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) data_[i].~T();
        size_ = 0;
    }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    T& at(size_t i) {
        if (i >= size_) throw std::out_of_range("Vector::at");
        return data_[i];
    }

    T* data() { return data_; }
    const T* data() const { return data_; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    T& front() { return data_[0]; }
    T& back() { return data_[size_ - 1]; }

    // Iterator support
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }
};

int main() {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.emplace_back(40);

    std::cout << "Vector: ";
    for (const auto& v : vec) std::cout << v << " ";
    std::cout << "| size: " << vec.size() << " cap: " << vec.capacity() << "\n";

    vec.pop_back();
    std::cout << "After pop_back: ";
    for (size_t i = 0; i < vec.size(); ++i) std::cout << vec[i] << " ";
    std::cout << "\n";

    Vector<int> vec2 = vec; // Copy
    vec2.push_back(99);
    std::cout << "Copied: ";
    for (auto v : vec2) std::cout << v << " ";
    std::cout << "\n";

    Vector<int> vec3 = std::move(vec); // Move
    std::cout << "Moved (vec3): ";
    for (auto v : vec3) std::cout << v << " ";
    std::cout << "\nOriginal (vec) empty: " << vec.empty() << "\n";

    return 0;
}

/*
SAMPLE OUTPUT:
Vector: 10 20 30 40 | size: 4 cap: 4
After pop_back: 10 20 30 
Copied: 10 20 30 99 
Moved (vec3): 10 20 30 
Original (vec) empty: 1
*/
