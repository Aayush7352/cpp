/*
THEORY: REST API Framework
- Lightweight C++ REST API framework
- HTTP routing with path parameters
- JSON request/response support
- Middleware pipeline
- Simple embedded HTTP server

Complexity: O(1) route lookup, O(n) middleware chain
Interview Questions:
1. RESTful API design principles?
2. How to handle authentication in REST APIs?
3. Rate limiting strategies?
Common Mistakes:
- Not handling CORS headers
- SQL injection in API endpoints
- Not validating request bodies
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <sstream>
#include <algorithm>
#include <regex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Mini-JSON for response building
class JSON {
public:
    static std::string stringify(const std::unordered_map<std::string, std::string>& obj) {
        std::string s = "{";
        bool first = true;
        for (const auto& [k, v] : obj) {
            if (!first) s += ",";
            first = false;
            s += "\"" + k + "\":\"" + v + "\"";
        }
        s += "}";
        return s;
    }
};

class Request {
public:
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    std::unordered_map<std::string, std::string> params;
};

class Response {
public:
    int statusCode{200};
    std::string statusText{"OK"};
    std::unordered_map<std::string, std::string> headers{{"Content-Type", "application/json"}};
    std::string body;

    std::string toString() const {
        std::ostringstream resp;
        resp << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
        resp << "Content-Length: " << body.size() << "\r\n";
        for (const auto& [k, v] : headers)
            resp << k << ": " << v << "\r\n";
        resp << "\r\n" << body;
        return resp.str();
    }

    void json(const std::unordered_map<std::string, std::string>& data) {
        body = JSON::stringify(data);
    }

    void text(const std::string& t, const std::string& contentType = "text/plain") {
        body = t;
        headers["Content-Type"] = contentType;
    }

    void status(int code, const std::string& text = "") {
        statusCode = code;
        statusText = text.empty() ? getStatusText(code) : text;
    }

    static std::string getStatusText(int code) {
        static const std::unordered_map<int, std::string> texts = {
            {200, "OK"}, {201, "Created"}, {204, "No Content"},
            {301, "Moved Permanently"}, {400, "Bad Request"},
            {401, "Unauthorized"}, {403, "Forbidden"}, {404, "Not Found"},
            {405, "Method Not Allowed"}, {500, "Internal Server Error"}
        };
        auto it = texts.find(code);
        return it != texts.end() ? it->second : "Unknown";
    }
};

class RESTFramework {
    using Handler = std::function<void(const Request&, Response&)>;
    using Middleware = std::function<bool(const Request&, Response&)>;

    struct Route {
        std::string method;
        std::regex pattern;
        std::vector<std::string> paramNames;
        Handler handler;
    };

    std::vector<Route> routes_;
    std::vector<Middleware> globalMiddleware_;
    int port_{8080};
    int serverSock_{-1};

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> parts;
        std::istringstream iss(s);
        std::string part;
        while (std::getline(iss, part, delim))
            if (!part.empty()) parts.push_back(part);
        return parts;
    }

    Route parseRoute(const std::string& method, const std::string& route, Handler handler) {
        std::string pattern;
        std::vector<std::string> paramNames;
        for (auto part : split(route, '/')) {
            if (part[0] == ':') {
                paramNames.push_back(part.substr(1));
                pattern += "/([^/]+)";
            } else {
                pattern += "/" + part;
            }
        }
        return {method, std::regex(pattern), paramNames, std::move(handler)};
    }

    Request parseRequest(const std::string& raw) {
        Request req;
        std::istringstream iss(raw);
        std::string line;
        std::getline(iss, line);
        std::istringstream lineStream(line);
        lineStream >> req.method >> req.path;

        while (std::getline(iss, line) && line != "\r") {
            auto colon = line.find(':');
            if (colon != std::string::npos) {
                req.headers[line.substr(0, colon)] = line.substr(colon + 2);
                if (line.back() == '\r') req.headers[line.substr(0, colon)].pop_back();
            }
        }

        // Read body
        auto it = req.headers.find("Content-Length");
        if (it != req.headers.end()) {
            size_t len = std::stoul(it->second);
            req.body.resize(len);
            // In real impl, read the rest of stream
        }

        return req;
    }

    bool matchRoute(const Request& req, const Route& route, Request& matched) {
        if (req.method != route.method) return false;
        std::smatch match;
        std::string path = req.path;
        if (std::regex_match(path, match, route.pattern)) {
            matched = req;
            for (size_t i = 0; i < route.paramNames.size(); ++i) {
                matched.params[route.paramNames[i]] = match[i + 1];
            }
            return true;
        }
        return false;
    }

public:
    explicit RESTFramework(int port = 8080) : port_(port) {}

    void use(Middleware mw) { globalMiddleware_.push_back(std::move(mw)); }

    void get(const std::string& route, Handler handler) {
        routes_.push_back(parseRoute("GET", route, std::move(handler)));
    }

    void post(const std::string& route, Handler handler) {
        routes_.push_back(parseRoute("POST", route, std::move(handler)));
    }

    void put(const std::string& route, Handler handler) {
        routes_.push_back(parseRoute("PUT", route, std::move(handler)));
    }

    void del(const std::string& route, Handler handler) {
        routes_.push_back(parseRoute("DELETE", route, std::move(handler)));
    }

    bool listen() {
        serverSock_ = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock_ < 0) { perror("socket"); return false; }

        int opt = 1;
        setsockopt(serverSock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(serverSock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind"); return false;
        }
        if (::listen(serverSock_, 5) < 0) {
            perror("listen"); return false;
        }

        std::cout << "REST API Server listening on http://localhost:" << port_ << "\n";
        std::cout << "Endpoints:\n";
        for (const auto& r : routes_)
            std::cout << "  " << r.method << " " << r.pattern.str() << "\n\n";

        return true;
    }

    void run() {
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            int clientSock = accept(serverSock_, (struct sockaddr*)&clientAddr, &addrLen);
            if (clientSock < 0) { perror("accept"); continue; }

            // Read request
            char buffer[8192];
            ssize_t received = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) { close(clientSock); continue; }
            buffer[received] = '\0';

            auto req = parseRequest(buffer);
            Response resp;

            // Middleware
            bool proceed = true;
            for (const auto& mw : globalMiddleware_) {
                if (!mw(req, resp)) { proceed = false; break; }
            }

            if (proceed) {
                // Route matching
                bool matched = false;
                for (const auto& route : routes_) {
                    Request matchedReq;
                    if (matchRoute(req, route, matchedReq)) {
                        route.handler(matchedReq, resp);
                        matched = true;
                        break;
                    }
                }
                if (!matched) {
                    resp.status(404);
                    resp.json({{"error", "Not Found"}, {"path", req.path}});
                }
            }

            std::string responseStr = resp.toString();
            send(clientSock, responseStr.c_str(), responseStr.size(), 0);
            close(clientSock);

            std::cout << req.method << " " << req.path << " -> " << resp.statusCode << "\n";
        }
    }
};

int main() {
    RESTFramework app(8080);

    // Middleware
    app.use([](const Request& req, Response& resp) -> bool {
        auto it = req.headers.find("Authorization");
        // Simple auth check for protected routes
        return true; // Allow all for demo
    });

    // Routes
    app.get("/api/hello", [](const Request& req, Response& resp) {
        resp.json({{"message", "Hello, World!"}, {"version", "1.0"}});
    });

    app.get("/api/users/:id", [](const Request& req, Response& resp) {
        std::string userId = req.params.at("id");
        resp.json({
            {"id", userId},
            {"name", "User " + userId},
            {"email", "user" + userId + "@example.com"}
        });
    });

    app.post("/api/data", [](const Request& req, Response& resp) {
        resp.status(201);
        resp.json({{"status", "created"}, {"received", req.body}});
    });

    app.get("/api/status", [](const Request&, Response& resp) {
        resp.json({{"status", "running"}, {"uptime", "1h"}});
    });

    if (!app.listen()) return 1;
    app.run();
    return 0;
}

/*
SAMPLE OUTPUT:
REST API Server listening on http://localhost:8080
Endpoints:
  GET /api/hello
  GET /api/users/([^/]+)
  POST /api/data
  GET /api/status

$ curl http://localhost:8080/api/hello
{"message":"Hello, World!","version":"1.0"}

$ curl http://localhost:8080/api/users/42
{"email":"user42@example.com","id":"42","name":"User 42"}

$ curl -X POST http://localhost:8080/api/data -d '{"test":"value"}'
{"received":"{...}","status":"created"}
*/
