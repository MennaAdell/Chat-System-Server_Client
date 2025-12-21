#include <iostream>
#include <string>
using namespace std;

// Function declarations from Server_Network.cpp
bool startServer(unsigned short port);
void stopServer();

int main() {
    const unsigned short port = 5555;

    cout << "Starting server on port " << port << " ..." << endl;
    if (!startServer(port)) {
        cerr << "Failed to start server." << endl;
        return 1;
    }

    cout << "Server started. Type 'q' and Enter to stop the server." << endl;

    string cmd;
    while (true) {
        getline(cin, cmd);
        if (!cmd.empty() && (cmd[0] == 'q' || cmd[0] == 'Q')) break;
    }

    cout << "Shutting down server..." << endl;
    stopServer();
    cout << "Server stopped. Bye." << endl;
    return 0;
}
