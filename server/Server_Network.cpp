#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <iostream>
#include <atomic>
using namespace std;

void ClientThreadProc(SOCKET clientSocket);
void StopAllClients();


atomic_bool g_serverRunning(false);

static SOCKET g_listenSocket = INVALID_SOCKET;
static void AcceptLoop(unsigned short port);

bool startServer(unsigned short port) {
    if (g_serverRunning.load()) return true;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cerr << "WSAStartup failed!" << endl;
        return false;
    }

    g_serverRunning.store(true);
    try {
        thread t(AcceptLoop, port);
        t.detach();
    }
    catch (...) {
        cerr << "Failed to start accept thread" << endl;
        g_serverRunning.store(false);
        WSACleanup();
        return false;
    }

    return true;
}

void stopServer() {
    if (!g_serverRunning.load()) return;
    g_serverRunning.store(false);

    if (g_listenSocket != INVALID_SOCKET) {
        closesocket(g_listenSocket);
        g_listenSocket = INVALID_SOCKET;
    }

    StopAllClients();
    WSACleanup();
}

static void AcceptLoop(unsigned short port) {
    g_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_listenSocket == INVALID_SOCKET) return;

    int opt = 1;
    setsockopt(g_listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in srvAddr;
    ZeroMemory(&srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = INADDR_ANY;
    srvAddr.sin_port = htons(port);

    if (bind(g_listenSocket, (sockaddr*)&srvAddr, sizeof(srvAddr)) == SOCKET_ERROR) return;
    if (listen(g_listenSocket, SOMAXCONN) == SOCKET_ERROR) return;

    cout << "Server running on port " << port << " ..." << endl;

    while (g_serverRunning.load()) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(g_listenSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET) { Sleep(100); continue; }

        char* clientIp = inet_ntoa(clientAddr.sin_addr);
        cout << "New client connected from "
            << (clientIp ? clientIp : "unknown")
            << ":" << ntohs(clientAddr.sin_port)
            << " socket=" << clientSock << endl;

        try {
            thread t(ClientThreadProc, clientSock);
            t.detach();
        }
        catch (...) { closesocket(clientSock); }
    }
}