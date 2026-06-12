/*
THEORY: Config Parser
- Parses key=value configuration files
- Supports sections [section] and comments (# //)
- Type-safe value retrieval with automatic conversion
- Error handling for malformed entries
- Uses STL containers for storage

Complexity: O(n) parse time, O(1) lookup
Interview Questions:
1. How would you support JSON-like nested configs?
2. How to handle include directives? (recursive parsing)
3. How to support environment variable expansion? ($VAR)
Common Mistakes:
- Not trimming whitespace from keys/values
- Ignoring empty lines and comments
- Not handling duplicate keys properly
*/

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <stdexcept>

class ConfigParser {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data_;
    std::string currentSection_;

    std::string trim(const std::string& s) const {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

public:
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Cannot open file: " + filename);

        std::string line;
        int lineNum = 0;
        while (std::getline(file, line)) {
            ++lineNum;
            line = trim(line);
            if (line.empty() || line[0] == '#' || line.substr(0, 2) == "//") continue;

            if (line[0] == '[') {
                auto end = line.find(']');
                if (end == std::string::npos)
                    throw std::runtime_error("Invalid section at line " + std::to_string(lineNum));
                currentSection_ = line.substr(1, end - 1);
                continue;
            }

            auto eq = line.find('=');
            if (eq == std::string::npos)
                throw std::runtime_error("Invalid entry at line " + std::to_string(lineNum));
            
            auto key = trim(line.substr(0, eq));
            auto value = trim(line.substr(eq + 1));
            data_[currentSection_][key] = value;
        }
    }

    template<typename T>
    T get(const std::string& section, const std::string& key, const T& defaultVal = T{}) const {
        auto secIt = data_.find(section);
        if (secIt == data_.end()) return defaultVal;
        auto keyIt = secIt->second.find(key);
        if (keyIt == secIt->second.end()) return defaultVal;
        
        std::istringstream iss(keyIt->second);
        T value;
        if (iss >> value) return value;
        return defaultVal;
    }

    bool hasKey(const std::string& section, const std::string& key) const {
        auto secIt = data_.find(section);
        return secIt != data_.end() && secIt->second.count(key);
    }

    void print() const {
        for (const auto& [section, keys] : data_) {
            std::cout << "[" << section << "]\n";
            for (const auto& [key, value] : keys) {
                std::cout << "  " << key << " = " << value << "\n";
            }
        }
    }
};

/*
Sample config file (config.ini):
[server]
host = 127.0.0.1
port = 8080

[database]
url = localhost
pool_size = 10
debug = true

SAMPLE OUTPUT:
Parsed config:
[server]
  host = 127.0.0.1
  port = 8080
[database]
  url = localhost
  pool_size = 10
  debug = true
Server port: 8080
Pool size: 10
*/

int main() {
    ConfigParser config;
    try {
        config.load("config.ini");
        std::cout << "Parsed config:\n";
        config.print();
        
        auto port = config.get<int>("server", "port", 3000);
        auto pool = config.get<int>("database", "pool_size", 5);
        std::cout << "Server port: " << port << "\n";
        std::cout << "Pool size: " << pool << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
