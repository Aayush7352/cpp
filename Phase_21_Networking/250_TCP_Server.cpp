/*
THEORY: TCP Server
- Listens for incoming TCP connections, accepts clients
- Echo server: sends back what client sent
- Fork-based: one child process per client (simplified)
- Steps: socket() -> bind() -> listen() -> accept() -> recv()/send()
- Signal handling for zombie process cleanup

Complexity: O(1) per connection accept
Interview Questions:
1. What are the TCP states? (LISTEN, SYN_SENT, ESTABLISHED, etc.)
2. Thread vs fork vs select/epoll for multiple clients?
3. How to handle SIGPIPE when client disconnects?
Common Mistakes:
- Not setting SO_REUSEADDR causing "Address already in use"
- Not handling partial send()/recv() calls
- Fork bombing without connection limits
*/

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string>

class TCPServer {
    int serverSock_{-1};
    int port_;

public:
    explicit TCPServer(int port) : port_(port) {}

    ~TCPServer() { stop(); }

    bool start() {
        signal(SIGCHLD, SIG_IGN); // prevent zombie processes

        serverSock_ = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock_ < 0) {
            perror("socket");
            return false;
        }

        int opt = 1;
        if (setsockopt(serverSock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            return false;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(serverSock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            return false;
        }

        if (listen(serverSock_, 5) < 0) {
            perror("listen");
            return false;
        }

        std::cout << "TCP Server listening on port " << port_ << "\n";
        return true;
    }

    void run() {
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            int clientSock = accept(serverSock_, (struct sockaddr*)&clientAddr, &addrLen);

            if (clientSock < 0) {
                perror("accept");
                continue;
            }

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Client connected: " << clientIP << ":" 
                      << ntohs(clientAddr.sin_port) << "\n";

            pid_t pid = fork();
            if (pid == 0) {
                close(serverSock_);
                handleClient(clientSock);
                exit(0);
            } else if (pid > 0) {
                close(clientSock);
            } else {
                perror("fork");
                close(clientSock);
            }
        }
    }

    void stop() {
        if (serverSock_ >= 0) {
            close(serverSock_);
            serverSock_ = -1;
        }
    }

private:
    void handleClient(int clientSock) {
        char buffer[4096];
        while (true) {
            ssize_t received = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) {
                if (received == 0)
                    std::cout << "Client disconnected\n";
                else
                    perror("recv");
                break;
            }
            buffer[received] = '\0';
            std::cout << "Received: " << buffer << "\n";

            std::string response = "Echo: " + std::string(buffer);
            send(clientSock, response.c_str(), response.size(), 0);
        }
        close(clientSock);
    }
};

int main(int argc, char* argv[]) {
    int port = argc > 1 ? std::stoi(argv[1]) : 8080;
    TCPServer server(port);
    if (!server.start()) return 1;
    server.run();
    return 0;
}

/*
SAMPLE OUTPUT:
TCP Server listening on port 8080
Client connected: 127.0.0.1:54321
Received: Hello from TCP client!
Client disconnected
*/
