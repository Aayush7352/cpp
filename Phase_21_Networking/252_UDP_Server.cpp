/*
THEORY: UDP Server
- Listens for UDP datagrams on specified port
- Connectionless: receives from any client
- Echo service: sends back the received datagram
- Single socket handles all clients (no accept needed)
- Must handle datagram boundaries

Complexity: O(1) per datagram
Interview Questions:
1. Can multiple clients use the same UDP port?
2. How to handle large messages exceeding MTU? (application-level fragmentation)
3. How to implement reliability over UDP?
Common Mistakes:
- Not checking recvfrom source address before responding
- Assuming one recvfrom = one complete message
- Not setting receive buffer size sufficiently large
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class UDPServer {
    int sock_{-1};
    int port_;

public:
    explicit UDPServer(int port) : port_(port) {}

    ~UDPServer() { stop(); }

    bool start() {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            perror("socket");
            return false;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            return false;
        }

        // Increase receive buffer
        int rcvbuf = 256 * 1024;
        setsockopt(sock_, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));

        std::cout << "UDP Server listening on port " << port_ << "\n";
        return true;
    }

    void run() {
        char buffer[65507];
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);

            ssize_t received = recvfrom(sock_, buffer, sizeof(buffer) - 1, 0,
                                         (struct sockaddr*)&clientAddr, &addrLen);
            if (received < 0) {
                perror("recvfrom");
                continue;
            }
            buffer[received] = '\0';

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            int clientPort = ntohs(clientAddr.sin_port);

            std::cout << "Received " << received << " bytes from "
                      << clientIP << ":" << clientPort << ": "
                      << buffer << "\n";

            // Echo back
            std::string response = "Echo: " + std::string(buffer);
            sendto(sock_, response.c_str(), response.size(), 0,
                   (struct sockaddr*)&clientAddr, addrLen);
        }
    }

    void stop() {
        if (sock_ >= 0) {
            close(sock_);
            sock_ = -1;
        }
    }
};

int main(int argc, char* argv[]) {
    int port = argc > 1 ? std::stoi(argv[1]) : 8081;
    UDPServer server(port);
    if (!server.start()) return 1;
    server.run();
    return 0;
}

/*
SAMPLE OUTPUT:
UDP Server listening on port 8081
Received 16 bytes from 127.0.0.1:54322: Hello UDP server!
*/
