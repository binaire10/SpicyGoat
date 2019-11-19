//
// Created by thekinrar on 30/03/19.
//

#include "TCPServer.h"

#include <iostream>
#include <thread>

TCPServer::TCPServer() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sin;
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(25566);

    errno = 0;
    bind(sock, (sockaddr*) &sin, sizeof(sin));

    if(errno) {
        std::cerr << "bind failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    errno = 0;
    listen(sock, 5);

    if(errno) {
        std::cerr << "listen failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    new std::thread(&TCPServer::keepAliveTask, this);
}

TCPServer::~TCPServer() {
    close(sock);
}

void TCPServer::accept() {
    while(running) {
        std::cout << "accept" << std::endl;

        sockaddr_in csin;
        socklen_t csinlen = sizeof(csin);
        int csock = ::accept(sock, (sockaddr *) &csin, &csinlen);

        auto conn = new TCPConnection(csock, csin);
        connections.push_back(conn);
    }
}

void TCPServer::keepAliveTask() {
    while(running) {
        int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        for(auto connection : connections) {
            if(connection->getState() == ConnectionState::PLAY) {
                connection->keepAlive(millis);
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

bool TCPServer::isRunning() const {
    return running;
}

void TCPServer::stop() {
    running = false;
    close(sock);
}
