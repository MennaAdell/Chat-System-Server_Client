#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

SOCKET g_clientSocket = INVALID_SOCKET;

bool Client_InitWSA(int major, int minor) {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(major, minor), &wsa) == 0;
}

bool Client_CreateSocket() {
    g_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return g_clientSocket != INVALID_SOCKET;
}

bool Client_PrepareAddress(const char* ip, unsigned short port, sockaddr_in& addr) {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    return true;
}

bool Client_ConnectToServer(const sockaddr_in& addr) {
    return connect(g_clientSocket, (sockaddr*)&addr, sizeof(addr)) == 0;
}

bool Client_ConnectAndInit(const char* ip, unsigned short port) {
    sockaddr_in addr;
    if (!Client_InitWSA(2, 2)) return false;
    if (!Client_CreateSocket()) return false;
    Client_PrepareAddress(ip, port, addr);
    return Client_ConnectToServer(addr);
}

void Client_Cleanup() {
    if (g_clientSocket != INVALID_SOCKET) {
        closesocket(g_clientSocket);
        g_clientSocket = INVALID_SOCKET;
    }
    WSACleanup();
}