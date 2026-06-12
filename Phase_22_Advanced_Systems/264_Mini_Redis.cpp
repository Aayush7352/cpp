/*
THEORY: Mini Redis
- In-memory key-value store inspired by Redis
- Supports strings, lists, sets, hashes
- RESP (Redis Serialization Protocol) compatible commands
- Commands: SET, GET, DEL, LPUSH, LRANGE, SADD, SMEMBERS, HSET, HGET
- Single-threaded event loop for simplicity

Complexity: O(1) for most operations, O(n) for range queries
Interview Questions:
1. How does Redis achieve single-threaded high performance? (I/O multiplexing)
2. Redis persistence: RDB vs AOF trade-offs?
3. What is Redis pub/sub?
Common Mistakes:
- Blocking the event loop with slow operations
- Memory management for large values
- Not handling command parsing edge cases
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <list>
#include <sstream>
#include <algorithm>

class MiniRedis {
    std::unordered_map<std::string, std::string> strings_;
    std::unordered_map<std::string, std::list<std::string>> lists_;
    std::unordered_map<std::string, std::set<std::string>> sets_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hashes_;

    std::vector<std::string> tokenize(const std::string& cmd) {
        std::vector<std::string> tokens;
        std::istringstream iss(cmd);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    std::string execute(const std::string& command) {
        auto tokens = tokenize(command);
        if (tokens.empty()) return "-ERR empty command\r\n";

        std::string cmd = tokens[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        try {
            if (cmd == "SET" && tokens.size() >= 3) {
                strings_[tokens[1]] = tokens[2];
                return "+OK\r\n";
            }
            if (cmd == "GET" && tokens.size() == 2) {
                auto it = strings_.find(tokens[1]);
                if (it == strings_.end()) return "$-1\r\n";
                return "$" + std::to_string(it->second.size()) + "\r\n" + it->second + "\r\n";
            }
            if (cmd == "DEL" && tokens.size() >= 2) {
                int count = 0;
                for (size_t i = 1; i < tokens.size(); ++i) {
                    count += strings_.erase(tokens[i]);
                    count += lists_.erase(tokens[i]);
                    count += sets_.erase(tokens[i]);
                }
                return ":" + std::to_string(count) + "\r\n";
            }
            if (cmd == "LPUSH" && tokens.size() >= 3) {
                for (size_t i = 2; i < tokens.size(); ++i)
                    lists_[tokens[1]].push_front(tokens[i]);
                return ":" + std::to_string(lists_[tokens[1]].size()) + "\r\n";
            }
            if (cmd == "LRANGE" && tokens.size() == 4) {
                auto it = lists_.find(tokens[1]);
                if (it == lists_.end()) return "*0\r\n";
                int start = std::stoi(tokens[2]);
                int stop = std::stoi(tokens[3]);
                if (start < 0) start = it->second.size() + start;
                if (stop < 0) stop = it->second.size() + stop;
                int idx = 0;
                std::string result = "*" + std::to_string(stop - start + 1) + "\r\n";
                for (const auto& v : it->second) {
                    if (idx >= start && idx <= stop) {
                        result += "$" + std::to_string(v.size()) + "\r\n" + v + "\r\n";
                    }
                    ++idx;
                    if (idx > stop) break;
                }
                return result;
            }
            if (cmd == "SADD" && tokens.size() >= 3) {
                int added = 0;
                for (size_t i = 2; i < tokens.size(); ++i) {
                    added += sets_[tokens[1]].insert(tokens[i]).second;
                }
                return ":" + std::to_string(added) + "\r\n";
            }
            if (cmd == "SMEMBERS" && tokens.size() == 2) {
                auto it = sets_.find(tokens[1]);
                if (it == sets_.end()) return "*0\r\n";
                std::string result = "*" + std::to_string(it->second.size()) + "\r\n";
                for (const auto& v : it->second) {
                    result += "$" + std::to_string(v.size()) + "\r\n" + v + "\r\n";
                }
                return result;
            }
            if (cmd == "HSET" && tokens.size() == 4) {
                hashes_[tokens[1]][tokens[2]] = tokens[3];
                return ":1\r\n";
            }
            if (cmd == "HGET" && tokens.size() == 3) {
                auto it = hashes_.find(tokens[1]);
                if (it == hashes_.end()) return "$-1\r\n";
                auto fit = it->second.find(tokens[2]);
                if (fit == it->second.end()) return "$-1\r\n";
                return "$" + std::to_string(fit->second.size()) + "\r\n" + fit->second + "\r\n";
            }
            if (cmd == "PING") {
                return "+PONG\r\n";
            }
            if (cmd == "INFO") {
                std::string info = "# Server\r\nredis_version:0.1.0 (MiniRedis)\r\n"
                                   "# Keyspace\r\n";
                info += "strings:" + std::to_string(strings_.size()) + "\r\n";
                info += "lists:" + std::to_string(lists_.size()) + "\r\n";
                info += "sets:" + std::to_string(sets_.size()) + "\r\n";
                info += "hashes:" + std::to_string(hashes_.size()) + "\r\n";
                return "$" + std::to_string(info.size()) + "\r\n" + info + "\r\n";
            }
            return "-ERR unknown command '" + cmd + "'\r\n";
        } catch (const std::exception& e) {
            return "-ERR " + std::string(e.what()) + "\r\n";
        }
    }
};

int main() {
    MiniRedis redis;

    auto run = [&](const std::string& cmd) {
        std::cout << "> " << cmd << "\n";
        std::string resp = redis.execute(cmd);
        // Print response in a clean format
        std::cout << "  ";
        if (resp[0] == '+') std::cout << resp.substr(1, resp.size() - 3);
        else if (resp[0] == ':') std::cout << "(integer) " << resp.substr(1, resp.size() - 3);
        else if (resp[0] == '$' && resp[1] == '-') std::cout << "(nil)";
        else if (resp[0] == '$') {
            auto crlf = resp.find("\r\n", 1);
            auto len = std::stoi(resp.substr(1, crlf - 1));
            if (len == -1) std::cout << "(nil)";
            else std::cout << '"' << resp.substr(crlf + 2, len) << '"';
        }
        else if (resp[0] == '*') std::cout << "(array)";
        else std::cout << resp;
        std::cout << "\n";
    };

    run("SET name Alice");
    run("GET name");
    run("GET nonexistent");
    run("LPUSH fruits apple");
    run("LPUSH fruits banana");
    run("LPUSH fruits cherry");
    run("LRANGE fruits 0 -1");
    run("SADD colors red");
    run("SADD colors blue green");
    run("SMEMBERS colors");
    run("HSET user:1 name Alice");
    run("HSET user:1 age 30");
    run("HGET user:1 name");
    run("PING");
    run("INFO");
    run("DEL name");
    run("GET name");

    return 0;
}

/*
SAMPLE OUTPUT:
> SET name Alice
  OK
> GET name
  "Alice"
> GET nonexistent
  (nil)
> LPUSH fruits apple
  (integer) 1
> LPUSH fruits banana
  (integer) 2
> LPUSH fruits cherry
  (integer) 3
> LRANGE fruits 0 -1
  (array)
...
*/
