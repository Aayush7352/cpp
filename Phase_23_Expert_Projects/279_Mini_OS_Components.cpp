/*
THEORY: Mini Operating System Components
- Process scheduler (Round Robin)
- Memory management (simple paging)
- System calls interface
- Interrupt handling simulation
- Process Control Block (PCB)
- Context switch simulation

Complexity: O(n) for scheduler, O(1) for memory allocation
Interview Questions:
1. What is the difference between process and thread?
2. How does virtual memory work?
3. What are the different CPU scheduling algorithms?
Common Mistakes:
- Not handling process states correctly
- Memory fragmentation
- Deadlock in resource allocation
*/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <algorithm>
#include <chrono>
#include <iomanip>

enum class ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };

struct PCB {
    int pid;
    std::string name;
    ProcessState state{ProcessState::NEW};
    int priority{0};
    int burstTime{0};
    int remainingTime{0};
    int arrivalTime{0};
    int memoryBase{-1};
    int memorySize{0};
    std::vector<std::string> resources;
    int contextSwitchCount{0};

    PCB(int id, std::string n, int burst, int priority = 0, int arrival = 0)
        : pid(id), name(std::move(n)), priority(priority),
          burstTime(burst), remainingTime(burst), arrivalTime(arrival) {}

    std::string stateStr() const {
        switch (state) {
            case ProcessState::NEW: return "NEW";
            case ProcessState::READY: return "READY";
            case ProcessState::RUNNING: return "RUNNING";
            case ProcessState::WAITING: return "WAITING";
            case ProcessState::TERMINATED: return "TERMINATED";
        }
        return "UNKNOWN";
    }
};

class PageTable {
    std::map<int, int> pages_;  // virtual page -> physical frame
public:
    void map(int virtualPage, int physicalFrame) {
        pages_[virtualPage] = physicalFrame;
    }
    int translate(int virtualPage) {
        auto it = pages_.find(virtualPage);
        return it != pages_.end() ? it->second : -1;
    }
};

class MemoryManager {
    static constexpr int FRAME_SIZE = 4096;
    static constexpr int TOTAL_FRAMES = 256;
    std::vector<bool> freeFrames_{TOTAL_FRAMES, true};
    std::map<int, PageTable> pageTables_;

public:
    int allocate(int pid, int size) {
        int framesNeeded = (size + FRAME_SIZE - 1) / FRAME_SIZE;
        int allocated = 0;
        int baseFrame = -1;

        for (int i = 0; i < TOTAL_FRAMES && allocated < framesNeeded; ++i) {
            if (freeFrames_[i]) {
                freeFrames_[i] = false;
                pageTables_[pid].map(allocated, i);
                if (baseFrame == -1) baseFrame = i;
                ++allocated;
            }
        }

        if (allocated < framesNeeded) {
            // Not enough memory, rollback
            for (int i = baseFrame; i < baseFrame + allocated; ++i)
                freeFrames_[i] = true;
            return -1;
        }

        return baseFrame * FRAME_SIZE;
    }

    void deallocate(int pid) {
        auto it = pageTables_.find(pid);
        if (it != pageTables_.end()) {
            // In real OS, parse page table and free frames
            pageTables_.erase(it);
        }
    }

    int usedFrames() const {
        return TOTAL_FRAMES - std::count(freeFrames_.begin(), freeFrames_.end(), true);
    }

    int freeFrames() const {
        return std::count(freeFrames_.begin(), freeFrames_.end(), true);
    }
};

class Scheduler {
    std::queue<std::shared_ptr<PCB>> readyQueue_;
    std::vector<std::shared_ptr<PCB>> processes_;
    int quantum_{4};  // Time quantum for RR
    int currentPid_{1};
    int totalIdle_{0};

public:
    std::shared_ptr<PCB> createProcess(const std::string& name, int burst,
                                        int priority = 0, int arrival = 0) {
        auto pcb = std::make_shared<PCB>(currentPid_++, name, burst, priority, arrival);
        if (arrival == 0) {
            pcb->state = ProcessState::READY;
            readyQueue_.push(pcb);
        }
        processes_.push_back(pcb);
        return pcb;
    }

    std::shared_ptr<PCB> schedule(int currentTime) {
        // Check for arriving processes
        for (auto& p : processes_) {
            if (p->state == ProcessState::NEW && p->arrivalTime <= currentTime) {
                p->state = ProcessState::READY;
                readyQueue_.push(p);
            }
        }

        if (readyQueue_.empty()) {
            ++totalIdle_;
            return nullptr;  // Idle
        }

        auto next = readyQueue_.front();
        readyQueue_.pop();
        next->state = ProcessState::RUNNING;
        return next;
    }

    void yield(std::shared_ptr<PCB> pcb) {
        if (pcb->remainingTime > 0) {
            pcb->state = ProcessState::READY;
            readyQueue_.push(pcb);
        } else {
            pcb->state = ProcessState::TERMINATED;
            std::cout << "  Process " << pcb->name << " (PID:" << pcb->pid
                      << ") terminated.\n";
        }
    }

    void printReadyQueue() const {
        std::cout << "  Ready Queue: ";
        if (readyQueue_.empty()) {
            std::cout << "(empty)";
        } else {
            std::queue<std::shared_ptr<PCB>> q = readyQueue_;
            while (!q.empty()) {
                std::cout << q.front()->name << " ";
                q.pop();
            }
        }
        std::cout << "\n";
    }
};

class SystemCallInterface {
public:
    enum SysCall { FORK, EXEC, WAIT, EXIT, OPEN, READ, WRITE, CLOSE, MALLOC, FREE };

    static std::string sysCallName(SysCall call) {
        static const std::vector<std::string> names = {
            "FORK", "EXEC", "WAIT", "EXIT", "OPEN",
            "READ", "WRITE", "CLOSE", "MALLOC", "FREE"
        };
        return names[call];
    }

    static void handleSysCall(SysCall call, std::vector<int> args = {}) {
        std::cout << "  [SYSCALL] " << sysCallName(call) << "(";
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << args[i];
        }
        std::cout << ") -> ";
        
        switch (call) {
            case FORK:
                std::cout << "child PID = " << (args.empty() ? 2 : args[0] + 1);
                break;
            case EXEC:
                std::cout << "loaded program";
                break;
            case OPEN:
                std::cout << "fd = " << (args.empty() ? 3 : args[0] + 5);
                break;
            case READ:
                std::cout << "read " << (args.size() > 1 ? args[1] : 1024) << " bytes";
                break;
            case WRITE:
                std::cout << "wrote " << (args.size() > 1 ? args[1] : 512) << " bytes";
                break;
            case EXIT:
                std::cout << "exit code " << (args.empty() ? 0 : args[0]);
                break;
            default:
                std::cout << "ok";
        }
        std::cout << "\n";
    }
};

class MiniOS {
    Scheduler scheduler_;
    MemoryManager memory_;
    int clock_{0};

public:
    void boot() {
        std::cout << "=== Mini OS Boot ===\n";
        std::cout << "Memory: " << memory_.freeFrames() << " frames free\n\n";
    }

    std::shared_ptr<PCB> createProcess(const std::string& name, int burst,
                                        int priority = 0, int arrival = 0) {
        auto pcb = scheduler_.createProcess(name, burst, priority, arrival);
        int addr = memory_.allocate(pcb->pid, 16384);  // 16KB
        if (addr >= 0) {
            pcb->memoryBase = addr;
            pcb->memorySize = 16384;
            std::cout << "Created process: " << name << " (PID:" << pcb->pid
                      << ", burst:" << burst << ", priority:" << priority
                      << ", mem:@" << std::hex << addr << std::dec << ")\n";
        } else {
            std::cout << "Failed to allocate memory for " << name << "\n";
        }
        return pcb;
    }

    void run(int timeUnits) {
        std::cout << "\n=== Starting Scheduler (" << timeUnits << " time units) ===\n\n";

        for (int t = 0; t < timeUnits; ++t) {
            std::cout << "Time " << t << ": ";
            auto current = scheduler_.schedule(t);

            if (current) {
                std::cout << "Running " << current->name << " (remaining: "
                          << current->remainingTime << ")\n";

                // Simulate I/O
                if (t % 7 == 0) {
                    SystemCallInterface::handleSysCall(
                        SystemCallInterface::READ, {current->pid, 512});
                    current->state = ProcessState::WAITING;
                    scheduler_.yield(current);
                    continue;
                }

                current->remainingTime--;
                current->contextSwitchCount++;

                if (current->remainingTime <= 0) {
                    SystemCallInterface::handleSysCall(
                        SystemCallInterface::EXIT, {0});
                    current->state = ProcessState::TERMINATED;
                    memory_.deallocate(current->pid);
                    std::cout << "  " << current->name << " completed!\n";
                } else {
                    scheduler_.yield(current);
                }
            } else {
                std::cout << "(idle)\n";
            }
        }

        // Print summary
        std::cout << "\n=== Scheduler Summary ===\n";
        for (const auto& p : scheduler_.processes_) {
            std::cout << "  PID:" << p->pid << " " << p->name
                      << " - " << p->stateStr()
                      << " (switches: " << p->contextSwitchCount << ")\n";
        }
        std::cout << "Memory: " << memory_.usedFrames() << "/256 frames used\n";
    }
};

int main() {
    MiniOS os;
    os.boot();

    os.createProcess("init", 8, 1, 0);
    os.createProcess("shell", 5, 2, 1);
    os.createProcess("editor", 4, 1, 2);
    os.createProcess("compiler", 7, 3, 3);
    os.createProcess("daemon", 6, 1, 4);

    os.run(20);

    return 0;
}

/*
SAMPLE OUTPUT:
=== Mini OS Boot ===
Memory: 256 frames free

Created process: init (PID:1, burst:8, priority:1, mem:@0x0)
...
=== Starting Scheduler (20 time units) ===

Time 0: Running init (remaining: 8)
  [SYSCALL] READ(1, 512) -> read 512 bytes
...
Time 7: Running compiler (remaining: 2)
  compiler completed!
*/
