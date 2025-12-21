#include <iostream>
using namespace std;

extern bool Client_ConnectAndInit(const char* ip, unsigned short port);
extern void Client_Cleanup();
extern void Client_StartChat();

int main() {
    cout << "Client starting..." << endl;

    if (!Client_ConnectAndInit("192.168.1.2", 5555)) {
        cout << "Connection failed" << endl;
        return 1;
    }

    cout << "Connected successfully" << endl;

    Client_StartChat();

    cout << "\nChat ended. Cleaning up..." << endl;
    Client_Cleanup();
    return 0;
}
