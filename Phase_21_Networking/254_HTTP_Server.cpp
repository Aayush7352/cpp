/*
THEORY: HTTP Server
- Simple HTTP server that serves static files and handles basic routes
- Parses HTTP request line and headers
- Supports GET method with routing
- Returns proper HTTP responses with status codes and MIME types
- Multi-threaded: one thread per connection

Complexity: O(1) request parsing, O(n) file serving
Interview Questions:
1. What are the main HTTP status codes and their meanings?
2. How to implement keep-alive connections?
3. What is CGI and how does it work?
Common Mistakes:
- Not sanitizing file paths (directory traversal attacks)
- Not handling concurrent connections properly
- Buffer overflow in request parsing
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

class HTTPServer {
    int serverSock_{-1};
    int port_;
    std::string documentRoot_;

    static std::unordered_map<std::string, std::string> mimeTypes;

public:
    HTTPServer(int port, std::string docRoot = ".")
        : port_(port), documentRoot_(std::move(docRoot)) {}

    ~HTTPServer() { stop(); }

    bool start() {
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
        if (listen(serverSock_, 10) < 0) {
            perror("listen"); return false;
        }

        std::cout << "HTTP Server listening on http://localhost:" << port_ << "\n";
        return true;
    }

    void run() {
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            int clientSock = accept(serverSock_, (struct sockaddr*)&clientAddr, &addrLen);
            if (clientSock < 0) { perror("accept"); continue; }

            std::thread(&HTTPServer::handleClient, this, clientSock).detach();
        }
    }

    void stop() {
        if (serverSock_ >= 0) { close(serverSock_); serverSock_ = -1; }
    }

private:
    void handleClient(int clientSock) {
        char buffer[8192];
        ssize_t received = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) { close(clientSock); return; }
        buffer[received] = '\0';

        std::string request(buffer);
        auto response = processRequest(request);
        send(clientSock, response.c_str(), response.size(), 0);
        close(clientSock);
    }

    std::string processRequest(const std::string& request) {
        std::istringstream iss(request);
        std::string method, path, version;
        iss >> method >> path >> version;

        if (method != "GET") {
            return makeResponse(405, "Method Not Allowed", "Only GET supported");
        }

        // Simple routing
        if (path == "/" || path == "/index.html") {
            return serveFile(documentRoot_ + "/index.html");
        } else if (path == "/api/hello") {
            return makeResponse(200, "OK", R"({"message": "Hello, World!"})", "application/json");
        } else if (path == "/api/status") {
            return makeResponse(200, "OK",
                R"({"status": "running", "version": "1.0"})", "application/json");
        } else {
            return serveFile(documentRoot_ + path);
        }
    }

    std::string serveFile(const std::string& filepath) {
        // Security: prevent directory traversal
        std::string realPath = fs::canonical(fs::absolute(filepath));
        if (realPath.find(fs::canonical(fs::absolute(documentRoot_))) != 0) {
            return makeResponse(403, "Forbidden", "Access denied");
        }

        std::ifstream file(realPath, std::ios::binary);
        if (!file) {
            return makeResponse(404, "Not Found", "File not found");
        }

        std::ostringstream content;
        content << file.rdbuf();
        file.close();

        auto ext = fs::path(filepath).extension().string();
        auto mime = mimeTypes.count(ext) ? mimeTypes[ext] : "application/octet-stream";
        return makeResponse(200, "OK", content.str(), mime);
    }

    std::string makeResponse(int statusCode, const std::string& statusText,
                              const std::string& body,
                              const std::string& contentType = "text/html") {
        std::ostringstream response;
        response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n"
                 << "Content-Type: " << contentType << "\r\n"
                 << "Content-Length: " << body.size() << "\r\n"
                 << "Connection: close\r\n"
                 << "Server: MiniC++Server/1.0\r\n"
                 << "\r\n"
                 << body;
        return response.str();
    }
};

std::unordered_map<std::string, std::string> HTTPServer::mimeTypes = {
    {".html", "text/html"}, {".css", "text/css"}, {".js", "application/javascript"},
    {".json", "application/json"}, {".png", "image/png"}, {".jpg", "image/jpeg"},
    {".txt", "text/plain"}, {".xml", "application/xml"}
};

int main(int argc, char* argv[]) {
    int port = argc > 1 ? std::stoi(argv[1]) : 8080;
    std::string docRoot = argc > 2 ? argv[2] : ".";

    HTTPServer server(port, docRoot);
    if (!server.start()) return 1;
    server.run();
    return 0;
}

/*
SAMPLE OUTPUT:
$ ./254_HTTP_Server
HTTP Server listening on http://localhost:8080

$ curl http://localhost:8080/api/hello
{"message": "Hello, World!"}

$ curl http://localhost:8080/api/status
{"status": "running", "version": "1.0"}
*/
