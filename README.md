# Multi-Client Chat System with Error Detection & Correction

[![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)](https://www.linux.org/)
[![TCP/IP](https://img.shields.io/badge/Protocol-TCP%2FIP-blue)](https://en.wikipedia.org/wiki/Internet_protocol_suite)
[![Socket Programming](https://img.shields.io/badge/Networking-Socket_Programming-green)](https://en.wikipedia.org/wiki/Network_socket)

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)
- [System Components](#system-components)
- [Error Detection & Correction](#error-detection--correction)
- [Testing](#testing)
- [Future Enhancements](#future-enhancements)
- [Author](#author)

## 🎯 Overview

A comprehensive **systems engineering project** implementing a multi-client chat application using **Linux TCP sockets** with integrated **error detection and correction mechanisms**. This project demonstrates low-level network programming, concurrent connection handling, and data integrity verification through CRC32 and Hamming Code implementations.

### What This Project Does

This system provides:
- **Real-time multi-client chat server** supporting concurrent connections
- **Public and private messaging** between connected users
- **CRC32 error detection** for data transmission integrity
- **Hamming Code implementation** for single-bit error correction
- **Chat history logging** for message persistence
- **User management** with login/logout notifications

## ✨ Features

### Chat Server Features
- ✅ **Multi-client support** - Handle multiple simultaneous connections
- ✅ **Public messaging** - Broadcast messages to all connected clients
- ✅ **Private messaging** - Send direct messages to specific users
- ✅ **User authentication** - Username-based identification
- ✅ **Connection notifications** - Real-time alerts for user joins/leaves
- ✅ **Active user list** - View all connected users
- ✅ **Chat history** - Persistent message logging to `history.txt`
- ✅ **Colored console output** - Enhanced readability with color coding

### Error Control Features
- ✅ **CRC32 implementation** - Cyclic Redundancy Check for error detection
- ✅ **Hamming Code** - Single-bit error detection and correction
- ✅ **Parity checking** - Odd parity bit verification
- ✅ **Frame construction** - SYN character and length-based framing
- ✅ **Error simulation** - Introduce and correct transmission errors

## 🏗️ System Architecture

### High-Level Data Flow

```
┌─────────────────────────────────────────────────────────┐
│                    Chat Server System                    │
├─────────────────────────────────────────────────────────┤
│                                                          │
│  ┌──────────┐         ┌──────────────┐                 │
│  │ Client 1 │◄───────►│              │                  │
│  └──────────┘         │              │                  │
│                       │    Server    │                  │
│  ┌──────────┐         │   (select()  │                  │
│  │ Client 2 │◄───────►│   TCP/IP)    │                  │
│  └──────────┘         │              │                  │
│                       │              │                  │
│  ┌──────────┐         │              │   ┌───────────┐  │
│  │ Client N │◄───────►│              │──►│ history.  │  │
│  └──────────┘         └──────────────┘   │   txt     │  │
│                                           └───────────┘  │
│                                                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │        Error Detection & Correction Layer          │ │
│  ├────────────────────────────────────────────────────┤ │
│  │  CRC32 Module          Hamming Code Module         │ │
│  │  └─ Checksum calc      └─ Encode 4-bit → 7-bit     │ │
│  │  └─ Error detection    └─ Detect errors            │ │
│  │  └─ Error correction   └─ Correct single-bit       │ │
│  └────────────────────────────────────────────────────┘ │
│                                                          │
└─────────────────────────────────────────────────────────┘
```

### Communication Flow

```
Client                    Server                    Other Clients
  │                         │                            │
  │──── Connect ───────────>│                            │
  │<─── Accept ─────────────│                            │
  │                         │                            │
  │──── Username ──────────>│                            │
  │                         │──── Join Notify ──────────>│
  │                         │                            │
  │──── Public Message ────>│──── Broadcast ────────────>│
  │                         │                            │
  │──── Private Message ───>│──── Route to User ────────>│
  │                         │                            │
  │──── LOGOUT ────────────>│──── Leave Notify ─────────>│
  │<─── Disconnect ─────────│                            │
  │                         │                            │
```

## 🛠️ Technologies Used

| Component | Technology |
|-----------|------------|
| **Language** | C (ANSI/ISO C99) |
| **Operating System** | Linux (Ubuntu, Debian, RHEL) |
| **Networking** | TCP/IP, BSD Sockets |
| **Concurrency** | select() system call (I/O multiplexing) |
| **Error Detection** | CRC32 (Cyclic Redundancy Check) |
| **Error Correction** | Hamming(7,4) Code |
| **Compiler** | GCC (GNU Compiler Collection) |

## 📁 Project Structure

```
chat-server-tcp/
│
├── server.c                  # Main server implementation
├── client.c                  # Client application
├── handle.h                  # Header file with structures and utilities
├── crcErrorDetection.c       # CRC32 error detection module
├── hamming.c                 # Hamming code implementation
│
├── result.txt                # Output file for general results
├── resultcrc.txt             # CRC error detection results
├── history.txt               # Chat message history log
│
├── README.md                 # Project documentation
└── Makefile                  # Build automation (optional)
```

## 🚀 Installation

### Prerequisites

- **Linux/Unix Operating System** (Ubuntu 18.04+, Debian 10+, or similar)
- **GCC Compiler** (version 7.0 or higher)
- **Standard C Library** (glibc)
- **Make** (optional, for automated builds)

### Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/varunjose/chat-server-tcp.git
   cd chat-server-tcp
   ```

2. **Verify GCC installation**
   ```bash
   gcc --version
   ```

3. **Compile the project components**
   ```bash
   # Compile server
   gcc server.c -o server
   
   # Compile client
   gcc client.c -o client
   
   # Compile CRC module
   gcc crcErrorDetection.c -o crc
   
   # Compile Hamming module
   gcc hamming.c -o hamming
   ```

## 📖 Usage

### Running the Server

1. **Start the server on a specific port**
   ```bash
   ./server 9898
   ```
   - Server listens on port **9898** (you can choose any port > 1024)
   - Server starts accepting client connections
   - Console displays connection status and messages

### Connecting Clients

2. **Launch client instances** (in separate terminal windows)
   ```bash
   ./client 127.0.0.1 9898
   ```
   - Replace `127.0.0.1` with server IP address for remote connections
   - Enter a username when prompted
   - Start chatting!

3. **Connect multiple clients**
   ```bash
   # Terminal 1
   ./client 127.0.0.1 9898
   # Username: Alice
   
   # Terminal 2
   ./client 127.0.0.1 9898
   # Username: Bob
   
   # Terminal 3
   ./client 127.0.0.1 9898
   # Username: Charlie
   ```

### Error Detection Modules

4. **Run CRC Error Detection**
   ```bash
   ./crc
   ```
   - Computes CRC32 checksum for data
   - Simulates transmission error
   - Detects and corrects errors
   - Outputs results to `resultcrc.txt`

5. **Run Hamming Code**
   ```bash
   ./hamming <input_file> <output_file>
   ```
   Example:
   ```bash
   ./hamming data.txt encoded.txt
   ```
   - Encodes 4-bit data chunks into 7-bit Hamming code
   - Introduces simulated errors
   - Detects and corrects single-bit errors
   - Outputs results to specified file

## 💬 Commands

### Client Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| **Send Public Message** | `<message>` | Broadcast message to all users |
| **Send Private Message** | `MSG <username> <message>` | Send direct message to specific user |
| **View User List** | `LOGIN_LIST` | Display all connected users |
| **Get Help** | `/help` | Show available commands |
| **Logout** | `LOGOUT` | Disconnect from server |

### Example Usage

```bash
# Alice's terminal
> Hello everyone!                    # Public message
> MSG Bob Hey Bob, how are you?      # Private message to Bob
> LOGIN_LIST                          # View connected users
> LOGOUT                              # Disconnect
```

```bash
# Bob's terminal
> Hi Alice!                           # Public message
> MSG Alice I'm good, thanks!         # Private message to Alice
```

## 🔧 System Components

### 1. Server Component (`server.c`)

**Key Functions:**

- `initialize_server()` - Sets up server socket and binds to port
- `construct_fd_set()` - Builds file descriptor set for select()
- `handle_new_connection()` - Accepts and registers new clients
- `handle_client_message()` - Processes incoming messages
- `send_public_message()` - Broadcasts to all clients
- `send_private_message()` - Routes message to specific user
- `send_connect_message()` - Notifies users of new connection
- `send_disconnect_message()` - Notifies users of disconnection
- `send_user_list()` - Sends list of active users
- `stop_server()` - Gracefully shuts down server

**Features:**
- Uses `select()` for concurrent I/O multiplexing
- Maintains array of `connection_info` structures
- Logs all messages to `history.txt`
- Handles up to `MAX_CLIENTS` simultaneous connections

### 2. Client Component (`client.c`)

**Key Functions:**

- `connect_to_server()` - Establishes TCP connection
- `set_username()` - Registers user identity
- `handle_user_input()` - Processes user commands
- `handle_server_message()` - Receives and displays messages
- `stop_client()` - Closes connection and cleans up

**Features:**
- Non-blocking input/output
- Color-coded message display
- Command parsing for special operations
- Connection state management

### 3. Header File (`handle.h`)

**Structures:**

```c
typedef struct {
    int socket;                    // Socket file descriptor
    struct sockaddr_in address;    // Client address info
    char username[MAX_USERNAME];   // User identifier
    int active;                    // Connection status
} connection_info;

typedef struct {
    int type;                      // Message type enum
    char username[MAX_USERNAME];   // Sender username
    char text[MAX_MESSAGE];        // Message content
} message;
```

**Enumerations:**

```c
enum message_type {
    PUBLIC_MESSAGE,
    PRIVATE_MESSAGE,
    CONNECT_MESSAGE,
    DISCONNECT_MESSAGE,
    USER_LIST,
    // ... more types
};
```

**Utilities:**
- Color code definitions (ANSI escape sequences)
- String manipulation functions
- Buffer clearing utilities

### 4. CRC Module (`crcErrorDetection.c`)

**Functions:**

- `calculate_crc32()` - Computes CRC32 checksum
- `introduce_error()` - Simulates transmission error
- `detect_error()` - Compares checksums
- `correct_error()` - Corrects single-bit errors

**Algorithm:**
- Implements polynomial division
- Uses lookup table for efficiency
- Outputs error position and corrected data

### 5. Hamming Module (`hamming.c`)

**Functions:**

- `calculateHammingCode()` - Encodes 4-bit data to 7-bit code
- `detectError()` - Identifies error position
- `correctError()` - Flips erroneous bit
- `frameContent()` - Adds SYN and length fields
- `deFrame()` - Removes frame metadata
- `calculateParity()` - Computes odd parity bit
- `byte_to_binary_with_parity()` - Converts bytes to binary with parity
- `checkParity()` - Validates parity bits

**Hamming(7,4) Code Structure:**
```
Data bits: D1 D2 D3 D4
Parity bits: P1 P2 P4
Code: P1 P2 D1 P4 D2 D3 D4

Position:  1  2  3  4  5  6  7
Bit:      P1 P2 D1 P4 D2 D3 D4
```

## 🛡️ Error Detection & Correction

### CRC32 (Cyclic Redundancy Check)

**Purpose:** Detect errors in data transmission

**How it works:**
1. Compute 32-bit checksum from original data
2. Append checksum to data
3. Transmit data + checksum
4. Receiver recomputes checksum
5. Compare checksums - mismatch indicates error

**Capabilities:**
- Detects all single-bit errors
- Detects all double-bit errors
- Detects burst errors up to 32 bits
- High error detection rate (99.9999%+)

### Hamming Code

**Purpose:** Detect and correct single-bit errors

**How it works:**
1. Take 4 data bits (D1, D2, D3, D4)
2. Calculate 3 parity bits (P1, P2, P4)
3. Construct 7-bit code word
4. Detect error using syndrome calculation
5. Correct error by flipping bit at error position

**Capabilities:**
- **Detect** up to 2-bit errors
- **Correct** single-bit errors
- Overhead: 3 parity bits for 4 data bits (43% overhead)

**Example:**

```
Data: 1011
Hamming Code: 0101011
      P1 P2 D1 P4 D2 D3 D4

Error introduced at position 5:
Received: 0101111 (D2 flipped)

Syndrome calculation:
S1 = P1 ⊕ D1 ⊕ D2 ⊕ D4 = 1
S2 = P2 ⊕ D1 ⊕ D3 ⊕ D4 = 0
S4 = P4 ⊕ D2 ⊕ D3 ⊕ D4 = 1

Error position = S4 S2 S1 = 101 = 5 (decimal)
Correct bit 5 → Original data recovered
```

## 🧪 Testing

### Test Scenarios Covered

1. ✅ **Server Connection** - Server starts and listens on specified port
2. ✅ **Single Client Login** - Client connects successfully with username registration
3. ✅ **Multiple Client Connections** - Tested with 6 concurrent clients
4. ✅ **User List Display** - `LOGIN_LIST` command shows all active users
5. ✅ **Public Messaging** - Messages broadcast to all connected clients
6. ✅ **Private Messaging** - One-to-one communication between specific users
7. ✅ **Client Logout** - Graceful disconnection with notifications
8. ✅ **Server Shutdown** - All clients disconnected, resources released
9. ✅ **CRC Error Detection** - Errors introduced, detected, and corrected
10. ✅ **Hamming Code Operations** - Data encoded, errors detected and corrected

### Test Output Files

- **`history.txt`** - Complete chat message log
- **`result.txt`** - General test results
- **`resultcrc.txt`** - CRC error detection results

### Sample Test Output

```bash
# history.txt
[2024-02-08 14:32:15] Alice: Hello everyone!
[2024-02-08 14:32:23] Bob: Hi Alice!
[2024-02-08 14:32:30] [PRIVATE] Alice -> Bob: How are you?
[2024-02-08 14:32:35] [PRIVATE] Bob -> Alice: I'm good!
[2024-02-08 14:33:01] Charlie has joined the chat
[2024-02-08 14:35:12] Alice has left the chat
```

## 🚀 Future Enhancements

### Planned Features

- [ ] **Encrypted communication** - Implement SSL/TLS
- [ ] **File transfer** - Send files between clients
- [ ] **Group chat rooms** - Create separate chat channels
- [ ] **Persistent user accounts** - Database integration
- [ ] **Message history** - Store and retrieve past messages
- [ ] **GUI client** - GTK+ or Qt interface
- [ ] **Web interface** - WebSocket-based web client
- [ ] **Mobile app** - Android/iOS clients
- [ ] **Voice chat** - UDP-based audio streaming
- [ ] **End-to-end encryption** - Client-side encryption

### Code Improvements

- [ ] **Error handling** - More robust error checking
- [ ] **Logging framework** - Structured logging (syslog)
- [ ] **Configuration file** - External config (JSON/YAML)
- [ ] **Unit tests** - Automated testing suite
- [ ] **Memory leak detection** - Valgrind integration
- [ ] **Code documentation** - Doxygen comments
- [ ] **Makefile** - Automated build system

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m "Add amazing feature"`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Varun Jose Madanu**
- GitHub: [@varunjose](https://github.com/varunjose)
- NetID: 001617451

**Academic Project**
- Course: Computer Networks / Systems Engineering
- Category: Systems Engineering, Network Programming

## 🙏 Acknowledgments

- **Linux/Unix community** for socket programming resources
- **Beej's Guide to Network Programming** for excellent tutorials
- **TCP/IP protocol designers** for robust networking standards
- **Error correction pioneers** (Richard Hamming, etc.)
- **Open source community** for tools and libraries

## 📚 References

### Books
- "Unix Network Programming" by W. Richard Stevens
- "Computer Networks" by Andrew S. Tanenbaum
- "The Linux Programming Interface" by Michael Kerrisk

### Online Resources
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Linux man pages](https://man7.org/linux/man-pages/)
- [RFC 793 - TCP Protocol](https://tools.ietf.org/html/rfc793)

---

<div align="center">

**⭐ Star this repository if you find it helpful!**

Made with ❤️ for learning systems programming

</div>
