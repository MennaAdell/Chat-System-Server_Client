#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <atomic>
using namespace std;

static map<SOCKET, string> g_clients;
static mutex g_clientsMutex;


extern atomic_bool g_serverRunning;

static void BroadcastMessage(const string& msg, SOCKET sender);

void ClientThreadProc(SOCKET clientSocket) {
    char nameBuf[128];
    int r = recv(clientSocket, nameBuf, sizeof(nameBuf) - 1, 0);
    if (r <= 0) { closesocket(clientSocket); return; }
    nameBuf[r] = '\0';
    string clientName(nameBuf);

    {
        lock_guard<mutex> lock(g_clientsMutex);
        g_clients[clientSocket] = clientName;
    }

    cout << "[JOIN] " << clientName << endl;
    BroadcastMessage("[JOIN] " + clientName, clientSocket);

    char buffer[1024];
    while (g_serverRunning.load()) {
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';
        string fullMsg = clientName + ": " + buffer;
        cout << fullMsg << endl;
        BroadcastMessage(fullMsg, clientSocket);
    }

    {
        lock_guard<mutex> lock(g_clientsMutex);
        cout << "[LEFT] " << g_clients[clientSocket] << endl;
        g_clients.erase(clientSocket);
    }
    closesocket(clientSocket);
}

static void BroadcastMessage(const string& msg, SOCKET sender) {
    lock_guard<mutex> lock(g_clientsMutex);
    for (auto& c : g_clients) {
        if (c.first != sender)
            send(c.first, msg.c_str(), (int)msg.size(), 0);
    }
}

void StopAllClients() {
    lock_guard<mutex> lock(g_clientsMutex);
    for (auto& c : g_clients) closesocket(c.first);
    g_clients.clear();
}