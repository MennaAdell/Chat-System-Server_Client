#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <mutex>
#include <winsock2.h>
using namespace std;

extern SOCKET g_clientSocket;

atomic_bool chatRunning(false);
thread recvThread;
mutex coutMutex;
string clientName;
constexpr int BUF_SIZE = 1024;

void printPrompt() {
    lock_guard<mutex> lk(coutMutex);
    cout << "You: ";
    cout.flush();
}

void Client_ReceiverLoop() {
    char buffer[BUF_SIZE];
    while (chatRunning.load()) {
        int bytes = recv(g_clientSocket, buffer, BUF_SIZE - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = 0;
            lock_guard<mutex> lk(coutMutex);
            cout << "\r" << buffer << "\n";
            printPrompt();
        }
        else {
            chatRunning.store(false);
            break;
        }
    }
}

void Client_SendLoop() {
    string line;
    while (chatRunning.load()) {
        printPrompt();
        if (!getline(cin, line)) {
            chatRunning.store(false);
            break;
        }

        if (line == "exit") {
            chatRunning.store(false);
            shutdown(g_clientSocket, SD_SEND);
            break;
        }


        send(g_clientSocket, line.c_str(), (int)line.size(), 0);
    }
}

void Client_StartChat() {
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    send(g_clientSocket, name.c_str(), (int)name.size(), 0);

    if (chatRunning.load()) return;
    chatRunning.store(true);

    recvThread = thread(Client_ReceiverLoop);

    Client_SendLoop();

    if (recvThread.joinable())
        recvThread.join();
}