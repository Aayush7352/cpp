/*
THEORY: UDP Client
- Connectionless protocol, sends datagrams
- No handshake, no guaranteed delivery
- Steps: socket() -> sendto() -> recvfrom() -> close()
- Faster than TCP, used for streaming, DNS, DHCP
- Must handle packet loss, ordering, duplication

Complexity: O(1) send/receive
Interview Questions:
1. What is the maximum UDP datagram size? (65535 bytes, payload ~65507)
2. How does UDP provide partial reliability? (application-level ACKs)
3. What is UDP hole punching?
Common Mistakes:
- Assuming datagrams arrive in order
- Not handling partial datagram reception
- Not setting receive timeout
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class UDPClient {
    int sock_{-1};

public:
    ~UDPClient() { close(sock_); }

    bool init() {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0) {
            perror("socket");
            return false;
        }

        struct timeval tv{};
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        return true;
    }

    bool send(const std::string& host, int port, const std::string& message) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
            struct hostent* server = gethostbyname(host.c_str());
            if (!server) {
                std::cerr << "DNS lookup failed\n";
                return false;
            }
            std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
        }

        ssize_t sent = sendto(sock_, message.c_str(), message.size(), 0,
                               (struct sockaddr*)&addr, sizeof(addr));
        if (sent < 0) {
            perror("sendto");
            return false;
        }
        std::cout << "Sent " << sent << " bytes to " << host << ":" << port << "\n";
        return true;
    }

    std::string receive() {
        char buffer[65507];
        sockaddr_in from{};
        socklen_t fromLen = sizeof(from);

        ssize_t received = recvfrom(sock_, buffer, sizeof(buffer) - 1, 0,
                                     (struct sockaddr*)&from, &fromLen);
        if (received < 0) {
            perror("recvfrom");
            return "";
        }
        buffer[received] = '\0';

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &from.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "Response from " << clientIP << ":"
                  << ntohs(from.sin_port) << "\n";

        return std::string(buffer);
    }

    void close() {
        if (sock_ >= 0) {
            ::close(sock_);
            sock_ = -1;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string host = argc > 1 ? argv[1] : "127.0.0.1";
    int port = argc > 2 ? std::stoi(argv[2]) : 8081;

    UDPClient client;
    if (!client.init()) return 1;

    client.send(host, port, "Hello UDP server!");
    auto response = client.receive();
    if (!response.empty()) {
        std::cout << "Response: " << response << "\n";
    }

    client.close();
    return 0;
}

/*
SAMPLE OUTPUT (with UDP_Server running):
Sent 16 bytes to 127.0.0.1:8081
Response from 127.0.0.1:8081
Response: Echo: Hello UDP server!
*/
