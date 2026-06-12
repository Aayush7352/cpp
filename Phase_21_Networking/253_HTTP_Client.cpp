/*
THEORY: HTTP Client
- Implements HTTP/1.1 GET and POST requests over TCP
- Parses URL to extract host, port, path
- Sends properly formatted HTTP request
- Parses response headers and body
- Connection: close for simplicity

Complexity: O(n) for response parsing
Interview Questions:
1. HTTP persistent connections vs Connection: close?
2. How to handle chunked transfer encoding?
3. What are the HTTP methods and their properties?
Common Mistakes:
- Not handling HTTP redirects (3xx)
- Not checking Content-Length for complete body
- Forgetting to send Host header (required in HTTP/1.1)
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>

class HTTPClient {
    struct URL {
        std::string host;
        int port{80};
        std::string path;
    };

    URL parseURL(const std::string& url) {
        URL result;
        std::string u = url;
        
        // Strip http://
        auto pos = u.find("://");
        if (pos != std::string::npos) u = u.substr(pos + 3);

        // Extract path
        pos = u.find('/');
        if (pos != std::string::npos) {
            result.path = u.substr(pos);
            u = u.substr(0, pos);
        } else {
            result.path = "/";
        }

        // Extract host and port
        pos = u.find(':');
        if (pos != std::string::npos) {
            result.host = u.substr(0, pos);
            result.port = std::stoi(u.substr(pos + 1));
        } else {
            result.host = u;
        }

        return result;
    }

public:
    std::string get(const std::string& url) {
        URL parsed = parseURL(url);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) { perror("socket"); return ""; }

        struct hostent* server = gethostbyname(parsed.host.c_str());
        if (!server) { std::cerr << "DNS lookup failed\n"; close(sock); return ""; }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
        addr.sin_port = htons(parsed.port);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("connect"); close(sock); return "";
        }

        std::ostringstream request;
        request << "GET " << parsed.path << " HTTP/1.1\r\n"
                << "Host: " << parsed.host << "\r\n"
                << "Connection: close\r\n"
                << "\r\n";

        std::string reqStr = request.str();
        send(sock, reqStr.c_str(), reqStr.size(), 0);

        std::string response;
        char buffer[4096];
        ssize_t bytes;
        while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes] = '\0';
            response += buffer;
        }

        close(sock);
        return response;
    }

    std::string post(const std::string& url, const std::string& body,
                     const std::string& contentType = "application/json") {
        URL parsed = parseURL(url);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) { perror("socket"); return ""; }

        struct hostent* server = gethostbyname(parsed.host.c_str());
        if (!server) { std::cerr << "DNS lookup failed\n"; close(sock); return ""; }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
        addr.sin_port = htons(parsed.port);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("connect"); close(sock); return "";
        }

        std::ostringstream request;
        request << "POST " << parsed.path << " HTTP/1.1\r\n"
                << "Host: " << parsed.host << "\r\n"
                << "Content-Type: " << contentType << "\r\n"
                << "Content-Length: " << body.size() << "\r\n"
                << "Connection: close\r\n"
                << "\r\n"
                << body;

        std::string reqStr = request.str();
        send(sock, reqStr.c_str(), reqStr.size(), 0);

        std::string response;
        char buffer[4096];
        ssize_t bytes;
        while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes] = '\0';
            response += buffer;
        }

        close(sock);
        return response;
    }
};

int main() {
    HTTPClient client;

    std::cout << "=== HTTP GET ===\n";
    auto response = client.get("http://httpbin.org/get");
    if (!response.empty()) {
        // Print first few lines
        auto pos = response.find("\r\n\r\n");
        if (pos != std::string::npos) {
            std::cout << "Headers:\n" << response.substr(0, pos) << "\n";
            std::cout << "Body:\n" << response.substr(pos + 4) << "\n";
        } else {
            std::cout << response << "\n";
        }
    }

    std::cout << "\n=== HTTP POST ===\n";
    auto postResponse = client.post("http://httpbin.org/post",
                                     R"({"name": "test", "value": 42})");
    auto pos = postResponse.find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::cout << postResponse.substr(pos + 4) << "\n";
    }

    return 0;
}

/*
SAMPLE OUTPUT:
=== HTTP GET ===
Headers:
HTTP/1.1 200 OK
Content-Type: application/json
...
Body:
{...json response...}
*/
