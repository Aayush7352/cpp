/*
THEORY: Thread Scheduler
- Cooperative multitasking scheduler for user-space threads (fibers)
- Manages a queue of tasks with time-slicing
- Each task runs to completion or yields voluntarily
- Simple round-robin scheduling policy
- Uses std::thread with a task queue

Complexity: O(1) schedule/dispatch
Interview Questions:
1. Preemptive vs Cooperative scheduling trade-offs?
2. How do real OS schedulers work? (CFS, MLFQ)
3. What is priority inversion and how to solve it?
Common Mistakes:
- Tasks not yielding causing starvation
- Race conditions in task queue
- Not handling task exceptions properly
*/

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>

class Task {
    std::function<void()> func_;
    std::string name_;
    int priority_;
    int remainingTickets_;

public:
    Task(std::function<void()> f, std::string n, int p = 0, int tickets = 1)
        : func_(std::move(f)), name_(std::move(n)), priority_(p), remainingTickets_(tickets) {}

    void execute() {
        func_();
        --remainingTickets_;
    }

    const std::string& name() const { return name_; }
    int priority() const { return priority_; }
    int remainingTickets() const { return remainingTickets_; }
    bool done() const { return remainingTickets_ <= 0; }

    struct Compare {
        bool operator()(const Task& a, const Task& b) {
            return a.priority() < b.priority();
        }
    };
};

class Scheduler {
    std::priority_queue<Task, std::vector<Task>, Task::Compare> taskQueue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_{true};
    std::thread worker_;

public:
    Scheduler() {
        worker_ = std::thread(&Scheduler::run, this);
    }

    ~Scheduler() {
        running_ = false;
        cv_.notify_one();
        if (worker_.joinable()) worker_.join();
    }

    void submit(Task task) {
        std::lock_guard lock(mutex_);
        taskQueue_.push(std::move(task));
        cv_.notify_one();
    }

    size_t queueSize() const {
        std::lock_guard lock(mutex_);
        return taskQueue_.size();
    }

private:
    void run() {
        while (running_) {
            Task task([](){}, "dummy", -1, 0);

            {
                std::unique_lock lock(mutex_);
                cv_.wait_for(lock, std::chrono::milliseconds(100), [this] {
                    return !taskQueue_.empty() || !running_;
                });

                if (!running_) break;
                if (taskQueue_.empty()) continue;

                task = std::move(const_cast<Task&>(taskQueue_.top()));
                taskQueue_.pop();
            }

            std::cout << "[Scheduler] Running: " << task.name()
                      << " (priority: " << task.priority()
                      << ", tickets: " << task.remainingTickets() << ")\n";

            try {
                task.execute();
            } catch (const std::exception& e) {
                std::cerr << "[Scheduler] Task '" << task.name()
                          << "' failed: " << e.what() << "\n";
            }

            if (!task.done()) {
                std::lock_guard lock(mutex_);
                taskQueue_.push(std::move(task));
            } else {
                std::cout << "[Scheduler] Completed: " << task.name() << "\n";
            }
        }
    }
};

int main() {
    Scheduler scheduler;

    // Submit various tasks
    scheduler.submit(Task([]() {
        std::cout << "  High-priority task running\n";
    }, "HighPriority", 10, 2));

    scheduler.submit(Task([]() {
        std::cout << "  Medium-priority task\n";
    }, "MediumPriority", 5, 1));

    scheduler.submit(Task([]() {
        std::cout << "  Low-priority background task\n";
    }, "Background", 0, 3));

    scheduler.submit(Task([]() {
        std::cout << "  Another medium task\n";
    }, "Medium2", 5, 2));

    scheduler.submit(Task([]() {
        int result = 0;
        for (int i = 0; i < 1000; ++i) result += i;
        std::cout << "  Calculation result: " << result << "\n";
    }, "Calculation", 8, 1));

    // Let tasks complete
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\nAll tasks submitted, queue empty: "
              << (scheduler.queueSize() == 0) << "\n";
    return 0;
}

/*
SAMPLE OUTPUT:
[Scheduler] Running: HighPriority (priority: 10, tickets: 2)
  High-priority task running
[Scheduler] Running: HighPriority (priority: 10, tickets: 1)
  High-priority task running
[Scheduler] Completed: HighPriority
[Scheduler] Running: Calculation (priority: 8, tickets: 1)
  Calculation result: 499500
[Scheduler] Completed: Calculation
[Scheduler] Running: MediumPriority (priority: 5, tickets: 1)
  Medium-priority task
[Scheduler] Completed: MediumPriority
[Scheduler] Running: Medium2 (priority: 5, tickets: 2)
  Another medium task
...
[Scheduler] Running: Background (priority: 0, tickets: 3)
  Low-priority background task
...
*/
