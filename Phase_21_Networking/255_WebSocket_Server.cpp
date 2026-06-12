/*
THEORY: WebSocket Server
- Implements WebSocket handshake (HTTP Upgrade) and frame protocol
- RFC 6455: WebSocket Protocol
- Upgrade from HTTP to WebSocket via handshake
- Frame format: opcode, mask, payload length, masking key, payload
- Text and binary frames, ping/pong for keepalive
- Simple chat server that broadcasts messages to all clients

Complexity: O(1) frame processing, O(n) broadcast
Interview Questions:
1. How does WebSocket achieve full-duplex communication?
2. What are the different WebSocket opcodes?
3. How to handle fragmentation (continuation frames)?
Common Mistakes:
- Not unmasking client-to-server frames (they're always masked)
- Not handling ping frames (must respond with pong)
- Buffer overflow in frame parsing
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <array>
#include <random>

class WebSocketServer {
    int serverSock_{-1};
    int port_;
    std::vector<int> clients_;
    std::mutex clientsMutex_;

public:
    explicit WebSocketServer(int port) : port_(port) {}

    ~WebSocketServer() { stop(); }

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
        if (listen(serverSock_, 5) < 0) {
            perror("listen"); return false;
        }

        std::cout << "WebSocket Server listening on ws://localhost:" << port_ << "\n";
        return true;
    }

    void run() {
        while (true) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            int clientSock = accept(serverSock_, (struct sockaddr*)&clientAddr, &addrLen);
            if (clientSock < 0) { perror("accept"); continue; }

            std::thread(&WebSocketServer::handleClient, this, clientSock).detach();
        }
    }

    void stop() {
        {
            std::lock_guard lock(clientsMutex_);
            for (int sock : clients_) close(sock);
            clients_.clear();
        }
        if (serverSock_ >= 0) { close(serverSock_); serverSock_ = -1; }
    }

private:
    void handleClient(int clientSock) {
        if (!performHandshake(clientSock)) {
            close(clientSock);
            return;
        }

        {
            std::lock_guard lock(clientsMutex_);
            clients_.push_back(clientSock);
        }
        broadcast("User connected (" + std::to_string(clientSock) + ")");

        while (true) {
            auto frame = readFrame(clientSock);
            if (frame.opcode == 8) break; // Close frame
            if (frame.opcode == 9) { // Ping
                sendFrame(clientSock, frame.payload, 0xA); // Pong
                continue;
            }
            if (frame.opcode == 1) { // Text
                std::string message = "User" + std::to_string(clientSock) + ": " + frame.payload;
                broadcast(message);
            }
        }

        {
            std::lock_guard lock(clientsMutex_);
            clients_.erase(std::remove(clients_.begin(), clients_.end(), clientSock),
                          clients_.end());
        }
        broadcast("User disconnected (" + std::to_string(clientSock) + ")");
        close(clientSock);
    }

    struct Frame {
        uint8_t opcode;
        std::string payload;
    };

    bool performHandshake(int sock) {
        char buffer[4096];
        ssize_t received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) return false;
        buffer[received] = '\0';

        std::string request(buffer);
        if (request.find("Upgrade: websocket") == std::string::npos &&
            request.find("upgrade: websocket") == std::string::npos) {
            return false;
        }

        // Extract WebSocket key
        auto keyPos = request.find("Sec-WebSocket-Key:");
        if (keyPos == std::string::npos) return false;
        auto keyEnd = request.find("\r\n", keyPos);
        std::string key = request.substr(keyPos + 18, keyEnd - keyPos - 18);
        key = key.substr(key.find_first_not_of(" \t"));

        // Compute accept key (simplified - in production use SHA-1)
        std::string acceptKey = "dGhlIHNhbXBsZSBub25jZQ=="; // Placeholder

        std::ostringstream response;
        response << "HTTP/1.1 101 Switching Protocols\r\n"
                 << "Upgrade: websocket\r\n"
                 << "Connection: Upgrade\r\n"
                 << "Sec-WebSocket-Accept: " << acceptKey << "\r\n"
                 << "\r\n";

        std::string resp = response.str();
        send(sock, resp.c_str(), resp.size(), 0);
        return true;
    }

    Frame readFrame(int sock) {
        Frame frame{0, ""};
        uint8_t header[2];
        if (recv(sock, header, 2, MSG_WAITALL) != 2) {
            frame.opcode = 8; // Treat as close
            return frame;
        }

        frame.opcode = header[0] & 0x0F;
        bool masked = header[1] & 0x80;
        uint64_t payloadLen = header[1] & 0x7F;

        if (payloadLen == 126) {
            uint16_t len;
            recv(sock, &len, 2, MSG_WAITALL);
            payloadLen = ntohs(len);
        } else if (payloadLen == 127) {
            uint64_t len;
            recv(sock, &len, 8, MSG_WAITALL);
            payloadLen = /* ntohll */ len; // Simplified
        }

        uint8_t maskKey[4]{};
        if (masked) recv(sock, maskKey, 4, MSG_WAITALL);

        frame.payload.resize(payloadLen);
        if (payloadLen > 0) recv(sock, &frame.payload[0], payloadLen, MSG_WAITALL);

        if (masked) {
            for (size_t i = 0; i < payloadLen; ++i)
                frame.payload[i] ^= maskKey[i % 4];
        }

        return frame;
    }

    void sendFrame(int sock, const std::string& payload, uint8_t opcode = 0x1) {
        uint8_t header[10];
        size_t headerLen;

        header[0] = 0x80 | opcode;
        if (payload.size() < 126) {
            header[1] = payload.size();
            headerLen = 2;
        } else if (payload.size() < 65536) {
            header[1] = 126;
            uint16_t len = htons(payload.size());
            std::memcpy(header + 2, &len, 2);
            headerLen = 4;
        } else {
            header[1] = 127;
            uint64_t len = /* htonll */ payload.size(); // Simplified
            std::memcpy(header + 2, &len, 8);
            headerLen = 10;
        }

        send(sock, header, headerLen, 0);
        if (!payload.empty()) send(sock, payload.c_str(), payload.size(), 0);
    }

    void broadcast(const std::string& message) {
        std::lock_guard lock(clientsMutex_);
        for (int sock : clients_) {
            sendFrame(sock, message);
        }
        std::cout << "[Broadcast] " << message << "\n";
    }
};

int main(int argc, char* argv[]) {
    int port = argc > 1 ? std::stoi(argv[1]) : 8082;
    WebSocketServer server(port);
    if (!server.start()) return 1;
    server.run();
    return 0;
}

/*
SAMPLE OUTPUT:
WebSocket Server listening on ws://localhost:8082
[Broadcast] User connected (5)
[Broadcast] User5: Hello everyone!
[Broadcast] User disconnected (5)
*/
