# Chat-System (C++ Windows Client-Server)

A **multi-threaded Client-Server Chat System** for Windows, implemented in C++ using the **Winsock2 API**.  
The system allows multiple clients to connect to a central server and chat in real-time with thread-safe connection management.

---

## 🚀 Features

- **Multi-client support** – multiple clients can connect simultaneously
- **Real-time chat** – messages are broadcast instantly to all connected clients
- **Join/Leave notifications** – see when users connect or disconnect
- **Thread-safe client management** using **mutex** and **atomic flags**
- **Console-based interface** – lightweight and easy to run

---

## 🛠️ Technologies & Libraries

- **C++**
- **Winsock2 (Windows Sockets API)**
- **Windows API** – for threading and synchronization
- `<thread>`, `<mutex>`, `<atomic>` for concurrency
- **TCP sockets** for client-server communication

---

## 🗂️ Project Structure

### Server
- `main.cpp` – main server loop, start/stop server
- `Server_Network.cpp` – socket initialization, accept loop
- `Server_Chat.cpp` – manages client threads and message broadcasting

### Client
- `main.cpp` – client entry point, connects to server
- `Client_Network.cpp` – socket initialization, connection, cleanup
- `Client_Chat.cpp` – handles sending and receiving messages

---

## ⚡ How to Run

1. Build and start the **Server** first.
2. Build and start **one or more Clients**.
3. Enter a name on the client prompt to join the chat.
4. Type messages to chat in real-time.
5. On the client, type `exit` to leave.
6. On the server, type `q` + Enter to stop the server.

---

## 💡 Example Usage

```
Server started on port 5555...
New client connected from 192.168.1.2:62054 socket=248
[JOIN] Alice
Alice: Hello everyone!
[LEFT] Alice
Server stopped.
```

---

## 📝 What I Learned

- Designing a **Client-Server architecture** in C++  
- Implementing **multi-threaded TCP communication**  
- Using **mutexes and atomic flags** for thread-safe operations  
- Handling **real-time messaging** and dynamic client connections  
- Practical Windows programming with **Winsock2 API**

---

## 👩‍💻 Author

**Menna Adel**
