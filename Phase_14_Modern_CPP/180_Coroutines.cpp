// ============================================================================
// Phase 14 — Modern C++
// Topic 180: Coroutines – C++20 Coroutines
// ============================================================================
//
// THEORY:
// --------
// Coroutines (C++20) are functions that can suspend execution and resume
// later, preserving state. They enable cooperative multitasking, lazy
// generators, and async operations.
//
// Three keywords:
//   co_await   — suspend execution, wait for an awaitable
//   co_yield   — produce a value and suspend (in generator)
//   co_return  — final return from a coroutine
//
// Coroutine machinery (the "promise_type" protocol):
//   A coroutine has a promise object that controls suspend/resume behavior.
//   The compiler generates the state machine; the user provides the promise
//   type via a customization point.
//
// Key components:
//   - promise_type: nested struct describing the coroutine protocol
//   - std::coroutine_handle<P>: handle to suspend/resume/destroy
//   - Awaitable: an object with await_ready/await_suspend/await_resume
//
// For simplicity, we demonstrate a generator<T> (co_yield) and a
// simple task-like coroutine (co_await).
//
// ============================================================================

#include <iostream>
#include <coroutine>
#include <optional>
#include <exception>
#include <thread>
#include <chrono>
#include <cassert>

// ==================== Generator<T> — co_yield based ====================

template <typename T>
struct Generator
{
    struct promise_type
    {
        T current_value;

        Generator get_return_object()
        {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept   { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }

        std::suspend_always yield_value(T value) noexcept
        {
            current_value = std::move(value);
            return {};
        }
    };

    std::coroutine_handle<promise_type> m_handle;

    explicit Generator(std::coroutine_handle<promise_type> h) noexcept
        : m_handle(h) {}

    ~Generator()
    {
        if (m_handle) m_handle.destroy();
    }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept
        : m_handle(std::exchange(other.m_handle, nullptr)) {}
    Generator& operator=(Generator&& other) noexcept
    {
        if (this != &other) {
            if (m_handle) m_handle.destroy();
            m_handle = std::exchange(other.m_handle, nullptr);
        }
        return *this;
    }

    // Iterator support for range-for
    struct Iterator
    {
        std::coroutine_handle<promise_type> handle;

        T& operator*()  { return handle.promise().current_value; }
        Iterator& operator++()
        {
            handle.resume();
            if (handle.done()) handle = nullptr;
            return *this;
        }
        bool operator!=(const Iterator&) const { return handle != nullptr; }
    };

    Iterator begin()
    {
        if (m_handle) m_handle.resume();
        return Iterator{m_handle};
    }
    Iterator end() { return Iterator{nullptr}; }
};

// Generator function: produces Fibonacci numbers lazily
Generator<int> fibonacci(int n)
{
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

// ==================== Task<T> — co_await based ====================

template <typename T>
struct Task
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        std::optional<T> result;
        std::exception_ptr exception;

        Task get_return_object()
        {
            return Task{handle_type::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept   { return {}; }
        void return_value(T value) { result = std::move(value); }
        void unhandled_exception() { exception = std::current_exception(); }
    };

    handle_type handle;
    explicit Task(handle_type h) : handle(h) {}
    ~Task() { if (handle) handle.destroy(); }
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&& o) noexcept : handle(std::exchange(o.handle, nullptr)) {}
    Task& operator=(Task&& o) noexcept
    {
        if (this != &o) {
            if (handle) handle.destroy();
            handle = std::exchange(o.handle, nullptr);
        }
        return *this;
    }

    T get()
    {
        if (!handle.done()) handle.resume();
        if (handle.promise().exception)
            std::rethrow_exception(handle.promise().exception);
        return std::move(*handle.promise().result);
    }
};

// Simple awaiter that resumes on a new thread (demo only)
struct resume_on_new_thread
{
    bool await_ready() noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) noexcept
    {
        std::thread([handle]() mutable {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            handle.resume();
        }).detach();
    }
    void await_resume() noexcept {}
};

Task<int> compute_on_thread(int x)
{
    co_await resume_on_new_thread{};
    co_return x * 2;
}

// ============================================================================
// SAMPLE I/O:
// -----------
// Fibonacci (10): 0 1 1 2 3 5 8 13 21 34
// compute_on_thread(21) = 42
// ============================================================================

int main()
{
    std::cout << "Fibonacci (10):";
    for (int val : fibonacci(10))
        std::cout << ' ' << val;
    std::cout << "\n";

    auto task = compute_on_thread(21);
    int result = task.get();
    std::cout << "compute_on_thread(21) = " << result << "\n";

    return 0;
}

// ============================================================================
// COMPLEXITY:
// -----------
// fibonacci: O(N) time, O(1) memory per suspension
// Task: O(1) allocation + thread overhead
// Coroutine state machine overhead: comparable to a function call
//
// INTERVIEW QUESTIONS:
// --------------------
// Q1: What are the three keywords added for coroutines?
// A1: co_await (suspend and wait), co_yield (produce value and suspend),
//     co_return (return from coroutine, possibly with value).
//
// Q2: How does a generator<T> differ from returning a vector<T>?
// A2: Generator produces values lazily — each value on demand, no storage
//     for all values at once. Vector computes all values eagerly.
//     Generators can produce infinite sequences.
//
// Q3: What are the required members of promise_type?
// A3: At minimum: get_return_object(), initial_suspend(), final_suspend(),
//     return_void() or return_value(), unhandled_exception(). Optionally
//     yield_value() for generators.
//
// COMMON MISTAKES:
// ----------------
// - Forgetting to include <coroutine> header.
// - Not destroying the coroutine_handle (memory leak).
// - Calling co_yield/co_await/co_return in non-coroutine functions
//   (compiler error).
// - Using co_return in a generator that uses co_yield (ambiguous).
// - Not handling exceptions via unhandled_exception() in promise_type.
// ============================================================================
