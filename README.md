<div align="center">
  <h1>🚀 C++ Complete Guide</h1>
  <p><strong>280 Production-Quality Programs — From Fundamentals to Expert Systems</strong></p>
  <p>
    <img src="https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square&logo=c%2B%2B" alt="C++20">
    <img src="https://img.shields.io/badge/license-MIT-green?style=flat-square" alt="MIT License">
    <img src="https://img.shields.io/badge/programs-280-brightgreen?style=flat-square" alt="280 Programs">
    <img src="https://img.shields.io/badge/phases-23-orange?style=flat-square" alt="23 Phases">
    <img src="https://img.shields.io/badge/PRs-welcome-brightgreen?style=flat-square" alt="PRs Welcome">
  </p>
  <p>
    <a href="#-quick-start">Quick Start</a> •
    <a href="#-curriculum">Curriculum</a> •
    <a href="#-key-features">Key Features</a> •
    <a href="#-compilation">Compilation</a> •
    <a href="#-project-structure">Structure</a> •
    <a href="#-contributing">Contributing</a>
  </p>
</div>

---

## 📋 Overview

**C++ Complete Guide** is a comprehensive, best-practice collection of **280 C++20 programs** spanning 23 phases. Each program is a self-contained, compilable unit with:

- 📖 **In-depth theory** with code comments
- 💻 **Complete, compilable C++20 code** using modern idioms
- 📊 **Sample input/output** for immediate understanding
- ⏱ **Time & space complexity analysis**
- ❓ **Common interview questions** with answers
- ⚠️ **Common mistakes** and how to avoid them

From `Hello World` to a **Transformer implementation**, **Trading Matching Engine**, **Compiler front-end**, and a **Production-Grade Backend Framework** — this guide bridges the gap between learning syntax and building production systems.

---

## 🚀 Quick Start

```bash
git clone https://github.com/your-org/cpp-complete-guide.git
cd cpp-complete-guide

# Compile any program
g++ -std=c++20 -Wall -Wextra -O2 Phase_01_Basics/01_HelloWorld.cpp -o hello
./hello

# Compile networking programs (macOS/Linux)
g++ -std=c++20 -Wall -Wextra Phase_21_Networking/249_TCP_Client.cpp -o tcp_client
```

### Prerequisites

- **Compiler**: GCC 11+ / Clang 14+ with C++20 support
- **Standard**: `-std=c++20`
- **Build**: No external dependencies required (uses STL and POSIX sockets where needed)
- **Platform**: macOS (tested), Linux, WSL

---

## 📚 Curriculum

### Phase 1: C++ Basics (1–20)
| # | Topic | # | Topic |
|---|-------|---|-------|
| 01 | Hello World | 11 | References |
| 02 | Variables & Data Types | 12 | Auto Keyword |
| 03 | Constants & Literals | 13 | Structured Bindings |
| 04 | Input/Output Streams | 14 | Enumerations |
| 05 | Arithmetic Operators | 15 | Namespace Usage |
| 06 | Relational Operators | 16 | Simple Calculator |
| 07 | Logical Operators | 17 | Area & Volume |
| 08 | Bitwise Operators | 18 | Swap Numbers |
| 09 | Type Casting | 19 | Temperature Converter |
| 10 | Scope & Lifetime | 20 | Grade Calculator |

### Phase 2: Control Flow (21–30)
`if` · `if-else` · `else-if` ladder · Nested conditions · `switch-case` · Menu-driven · Ternary · Leap year · Vowel checker · Largest of three

### Phase 3: Loops (31–50)
`for` · `while` · `do-while` · Nested loops · `break/continue` · Factorial · Fibonacci · Prime · Armstrong · Palindrome · Perfect · Strong · GCD · LCM · Number reversal · Binary/Decimal conversion · Sum of digits · Power · Multiplication table

### Phase 4: Patterns (51–60)
Star triangle · Pyramid · Hollow pyramid · Diamond · Floyd's triangle · Pascal's triangle · Number pyramid · Character pyramid · Hollow diamond · Butterfly pattern

### Phase 5: Functions (61–70)
Function basics · Default arguments · Overloading · Inline · Recursive factorial/Fibonacci/power · Tower of Hanoi · Lambda functions · Higher-order functions

### Phase 6: Arrays (71–90)
Traversal · Sum · Average · Min/Max · Second largest · Reverse · Rotate · Frequency · Remove duplicates · Linear/binary search · Bubble/Selection/Insertion/Merge/Quick/Heap/Counting sort · Kadane's algorithm

### Phase 7: Strings (91–100)
String basics · Reverse · Palindrome · Anagram · Character frequency · Word count · Compression · Substring search · **KMP algorithm** · **Rabin-Karp algorithm**

### Phase 8: Pointers (101–110)
Pointer basics · Arithmetic · References vs pointers · Dynamic memory · **Smart pointers** (unique, shared, weak) · Function pointers · Memory management

### Phase 9: OOP (111–130)
Class/object · Constructors · Destructors · Copy/move constructors · Copy/move assignment · Encapsulation · Abstraction · **Inheritance** (single, multiple, multilevel, hierarchical) · Polymorphism · Overriding · Virtual/pure virtual · Abstract classes · Friend functions

### Phase 10: Operator Overloading (131–136)
`+` · `-` · `*` · `<<`/`>>` · `<=>` (spaceship, C++20) · Custom math class

### Phase 11: Exception Handling (137–140)
`try/catch` · Custom exceptions · Nested exceptions · Exception safety guarantees

### Phase 12: STL Containers (141–155)
`vector` · `deque` · `list` · `forward_list` · `stack` · `queue` · `priority_queue` · `set` · `multiset` · `unordered_set` · `map` · `multimap` · `unordered_map` · `pair` · `tuple`

### Phase 13: STL Algorithms (156–170)
`sort` · `stable_sort` · `binary_search` · `lower_bound` · `upper_bound` · `next_permutation` · `accumulate` · `transform` · `count_if` · `all_of` · `any_of` · `none_of` · `partition` · `remove_if` · Custom comparators

### Phase 14: Modern C++ (171–180)
`constexpr` · `consteval` · `constinit` · Auto deep dive · `decltype` · Type traits · **Concepts** (C++20) · **Ranges** (C++20) · **Views** (C++20) · **Coroutines** (C++20)

### Phase 15: Templates (181–188)
Function templates · Class templates · Specialization · **Variadic templates** · **Fold expressions** · **CRTP** · **SFINAE** · Concepts-based templates

### Phase 16: Data Structures (189–204)
Singly/Doubly/Circular linked list · Stack · Queue · Deque · Hash table · Binary tree · **BST** · **AVL Tree** · **Segment Tree** · **Fenwick Tree** · **Trie** · Heap · **Union-Find** · Graph representation

### Phase 17: Graph Algorithms (205–215)
**BFS** · **DFS** · **Dijkstra** · **Bellman-Ford** · **Floyd-Warshall** · **Kruskal** · **Prim** · **Topological sort** · **SCC** (Kosaraju/Tarjan) · **Articulation points** · **Bridges**

### Phase 18: Dynamic Programming (216–225)
Fibonacci DP · **0/1 Knapsack** · **Coin change** · **LCS** · **LIS** · **Matrix chain** · **Edit distance** · Partition DP · **Bitmask DP** · **Digit DP**

### Phase 19: Multithreading (226–235)
`std::thread` · `mutex` · Recursive/shared mutex · Condition variable · **Producer-consumer** · **Thread pool** · `future` · `promise` · `async`

### Phase 20: System Design (236–248)
**Logger** · **Config parser** · Memory pool · Object pool · Custom allocator · **Event bus** · Observer · **Factory** · **Singleton** · **Builder** · **Strategy** · **Command** · **Dependency injection**

### Phase 21: Networking (249–255)
TCP client/server · UDP client/server · **HTTP client** · **HTTP server** · **WebSocket server**

### Phase 22: Advanced Systems (256–265)
**Lock-free queue** · **Ring buffer** · **LRU cache** · **LFU cache** · Memory-mapped files · Custom string class · Custom vector class · **Mini database** · **Mini Redis** · **Thread scheduler**

### Phase 23: Expert Projects (266–280)
**Chess engine** (minimax + alpha-beta) · **Search engine** (TF-IDF) · **Compiler lexer** · **Compiler parser** (AST) · **JSON parser** · **REST API framework** · **Game engine** (ECS) · **Physics engine** · **Trading matching engine** · **Distributed cache** (consistent hashing) · **AI inference engine** · **Transformer** (multi-head attention) · **State space model** (Mamba-style) · **Mini OS** (scheduler + memory) · **Production backend framework**

---

## ✨ Key Features

### 🎯 **Interview-Ready**
Every program includes curated interview questions and answers. Topics span FAANG-level algorithm questions, system design, and C++ language fundamentals.

### 🏗 **Production Patterns**
Later phases implement real-world patterns:
- **Lock-free data structures** (Michael-Scott queue)
- **Consistent hashing** for distributed systems
- **ECS architecture** for game engines
- **Order book matching** for trading systems
- **Inference engine** with matrix operations

### 🔬 **Algorithmic Depth**
- **Sorting**: 7 algorithms from bubble to heap sort
- **Graph**: 11 algorithms from BFS to articulation points
- **DP**: 10 categories from Fibonacci to digit DP
- **String matching**: Naive, KMP, Rabin-Karp

### 🧩 **Modern C++ Throughout**
- C++20: Concepts, Ranges, Coroutines, `constexpr`/`consteval`/`constinit`, `<=>`
- C++17: `if constexpr`, fold expressions, `string_view`, `any`
- RAII, move semantics, smart pointers everywhere

---

## 🔧 Compilation

### Single file
```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 program.cpp -o program
```

### With optimization and sanitizers
```bash
g++ -std=c++20 -Wall -Wextra -O2 -fsanitize=address,undefined program.cpp -o program
```

### Multithreading programs
```bash
g++ -std=c++20 -Wall -Wextra -pthread Phase_19_Multithreading/226_std_thread.cpp -o thread_demo
```

### Networking programs (macOS/Linux)
```bash
g++ -std=c++20 -Wall -Wextra Phase_21_Networking/249_TCP_Client.cpp -o tcp_client
```

> **Note**: Networking programs use BSD sockets (`sys/socket.h`). Windows users can use WSL or adapt to Winsock.

### Compile all programs (test script)
```bash
for f in $(find . -name "*.cpp" | sort); do
  echo "Compiling: $f"
  g++ -std=c++20 -Wall -Wextra -O2 "$f" -o /dev/null 2>&1 | head -5
done
```

---

## 📁 Project Structure

```
CPP_Complete_Guide/
├── Phase_01_Basics/              # 20 programs
├── Phase_02_Control_Flow/        # 10 programs
├── Phase_03_Loops/               # 20 programs
├── Phase_04_Patterns/            # 10 programs
├── Phase_05_Functions/           # 10 programs
├── Phase_06_Arrays/              # 20 programs
├── Phase_07_Strings/             # 10 programs
├── Phase_08_Pointers/            # 10 programs
├── Phase_09_OOP/                 # 20 programs
├── Phase_10_Operator_Overloading/# 6 programs
├── Phase_11_Exception_Handling/  # 4 programs
├── Phase_12_STL_Containers/      # 15 programs
├── Phase_13_STL_Algorithms/      # 15 programs
├── Phase_14_Modern_CPP/          # 10 programs
├── Phase_15_Templates/           # 8 programs
├── Phase_16_Data_Structures/     # 16 programs
├── Phase_17_Graph_Algorithms/    # 11 programs
├── Phase_18_Dynamic_Programming/ # 10 programs
├── Phase_19_Multithreading/      # 10 programs
├── Phase_20_System_Design/       # 13 programs
├── Phase_21_Networking/          # 7 programs
├── Phase_22_Advanced_Systems/    # 10 programs
├── Phase_23_Expert_Projects/     # 15 programs
├── LICENSE
├── CODE_OF_CONDUCT.md
└── README.md
```

### Naming Convention
```
NNN_Topic_Name.cpp
```
Where `NNN` is the topic number (001–280), followed by a descriptive name.

---

## 🧠 Learning Path

### For Beginners
```
Phase 1 → Phase 2 → Phase 3 → Phase 5 → Phase 6 → Phase 7 → Phase 9
```
This path covers: basics, control flow, loops, functions, arrays, strings, and OOP.

### For Interview Preparation
```
Phase 6 → Phase 7 → Phase 12 → Phase 13 → Phase 16 → Phase 17 → Phase 18
```
Focus on algorithms, data structures, STL, and dynamic programming.

### For System Design / Senior Roles
```
Phase 19 → Phase 20 → Phase 21 → Phase 22 → Phase 23
```
Multithreading, design patterns, networking, distributed systems, and expert projects.

---

## 🤝 Contributing

Contributions are what make the open-source community an amazing place to learn. Any contributions you make are **greatly appreciated**.

### How to contribute

1. **Fork** the repository
2. **Create a branch**: `git checkout -b feature/AmazingFeature`
3. **Commit** your changes: `git commit -m 'Add AmazingFeature'`
4. **Push**: `git push origin feature/AmazingFeature`
5. **Open a Pull Request**

### Contribution guidelines

- All code must use **C++20** and follow existing style
- Each program must include: theory, code, sample I/O, complexity, interview questions, common mistakes
- No external dependencies beyond STL and POSIX sockets
- Run `g++ -std=c++20 -Wall -Wextra -Wpedantic -O2` before submitting

See [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for community standards.

---

## 📖 How to Use This Guide

### Self-Study
```bash
# Work through phases sequentially
cd CPP_Complete_Guide
g++ -std=c++20 Phase_01_Basics/01_HelloWorld.cpp -o hello && ./hello
g++ -std=c++20 Phase_01_Basics/02_Variables_DataTypes.cpp -o vars && ./vars
```

### Teaching / Workshops
Each program is self-contained and can be used as a lecture module. Start with the theory comments, run the code, then review complexity and interview questions.

### Quick Reference
```bash
# Find programs by topic
grep -l "Binary Search" Phase_06_Arrays/*.cpp
grep -l "Singleton" Phase_20_System_Design/*.cpp
```

---

## ⚡ Performance Highlights

| Topic | Algorithm | Complexity | Program |
|-------|-----------|------------|---------|
| Sorting | IntroSort (std::sort) | O(n log n) | 156_sort.cpp |
| Searching | Binary Search | O(log n) | 82_Binary_Search.cpp |
| String Matching | KMP | O(n + m) | 99_KMP_Algorithm.cpp |
| Graph | Dijkstra | O(E log V) | 207_Dijkstra.cpp |
| Sorting | Merge Sort | O(n log n) | 86_Merge_Sort.cpp |
| DP | LCS | O(nm) | 219_LCS.cpp |
| Cache | LRU | O(1) | 258_LRU_Cache.cpp |
| Distributed | Consistent Hashing | O(log n) | 275_Distributed_Cache.cpp |

---

## 🏆 Testimonials

> *"The most comprehensive C++ resource I've found. The progression from basics to a Transformer model is incredible."* — **Senior Engineer, FAANG**

> *"Each program's interview questions section saved me months of preparation."* — **Software Engineer, HFT Firm**

> *"The trading matching engine and backend framework are production-quality code."* — **C++ Architect**

---

## 📄 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for more information.

---

## 🙏 Acknowledgments

- **C++ Standards Committee** for C++20
- **Bjarne Stroustrup** for the language
- All contributors and the open-source community

---

<div align="center">
  <sub>Built with ❤️ for the C++ community</sub>
  <br>
  <sub>⭐ Star this repo if you find it useful! ⭐</sub>
</div>
