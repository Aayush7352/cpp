/*
THEORY: TCP Client
- Creates socket, connects to server, sends/receives data
- Connection-oriented, reliable byte stream protocol
- Uses BSD socket API on macOS/Linux
- Steps: socket() -> connect() -> send()/recv() -> close()
- Blocking I/O with error handling

Complexity: O(1) connection setup, O(n) data transfer
Interview Questions:
1. TCP vs UDP differences?
2. How to handle partial recv()?
3. What is Nagle's algorithm?
Common Mistakes:
- Not checking return values of send()/recv()
- Forgetting to convert hostname to IP (gethostbyname)
- Blocking indefinitely without timeouts
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class TCPClient {
    int sock_{-1};
    bool connected_{false};

public:
    ~TCPClient() { disconnect(); }

    bool connect(const std::string& host, int port) {
        sock_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_ < 0) {
            perror("socket");
            return false;
        }

        struct hostent* server = gethostbyname(host.c_str());
        if (!server) {
            std::cerr << "DNS lookup failed for: " << host << "\n";
            close(sock_);
            return false;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        std::memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
        serverAddr.sin_port = htons(port);

        if (::connect(sock_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("connect");
            close(sock_);
            return false;
        }

        connected_ = true;
        std::cout << "Connected to " << host << ":" << port << "\n";
        return true;
    }

    bool send(const std::string& data) {
        if (!connected_) return false;
        ssize_t sent = ::send(sock_, data.c_str(), data.size(), 0);
        if (sent < 0) {
            perror("send");
            return false;
        }
        return true;
    }

    std::string receive() {
        if (!connected_) return "";
        char buffer[4096];
        ssize_t received = ::recv(sock_, buffer, sizeof(buffer) - 1, 0);
        if (received < 0) {
            perror("recv");
            return "";
        }
        if (received == 0) {
            std::cout << "Server closed connection\n";
            connected_ = false;
            return "";
        }
        buffer[received] = '\0';
        return std::string(buffer);
    }

    void disconnect() {
        if (sock_ >= 0) {
            if (connected_) {
                ::shutdown(sock_, SHUT_RDWR);
                connected_ = false;
            }
            close(sock_);
            sock_ = -1;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string host = argc > 1 ? argv[1] : "127.0.0.1";
    int port = argc > 2 ? std::stoi(argv[2]) : 8080;

    TCPClient client;
    if (!client.connect(host, port)) return 1;

    std::string message = "Hello from TCP client!";
    if (client.send(message)) {
        std::cout << "Sent: " << message << "\n";
        auto response = client.receive();
        if (!response.empty()) {
            std::cout << "Received: " << response << "\n";
        }
    }

    client.disconnect();
    return 0;
}

/*
SAMPLE OUTPUT (with TCP_Server running):
Connected to 127.0.0.1:8080
Sent: Hello from TCP client!
Received: Echo: Hello from TCP client!
*/
