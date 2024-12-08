# CANBus Class Usage Manual

This manual describes how to use the `CANBus` class, which provides a simple interface for sending and receiving messages on a CAN (Controller Area Network) bus in Linux environments. It is based on the SocketCAN infrastructure.

---

## Requirements

- **Linux** with SocketCAN enabled.
- **C++17** or higher.
- A configured CAN interface (e.g., `can0`).

### Install Dependencies

To use this class, ensure you have the necessary system tools and libraries. Install the required packages:

```bash
sudo apt install build-essential libsocketcan-dev
```

## Class Structure
### CANBus Class

The CANBus class provides methods to initialize a CAN interface, send messages, and receive messages.
Attributes

    int _socket_fd: File descriptor for the CAN socket.

Methods
CANBus()

Default constructor. Initializes the object without binding to any interface.
CANBus(const std::string& interface, int bitrate)

Parameterized constructor to initialize and bind the CAN socket to a specified interface.

    Parameters:
        interface: Name of the CAN interface (e.g., "can0").
        bitrate: CAN bus bitrate in bits per second (bps).

~CANBus()

Destructor. Ensures that the CAN socket is properly closed when the object is destroyed.
bool sendMessage(uint32_t id, const std::vector<uint8_t>& data)

Sends a CAN message with the specified ID and payload.

    Parameters:
        id: Message ID (11-bit or 29-bit CAN ID).
        data: A std::vector<uint8_t> containing the message payload (up to 8 bytes).

    Returns:
        true: If the message was sent successfully.
        false: If an error occurred (e.g., payload exceeds 8 bytes).

bool receiveMessage(uint32_t& id, std::vector<uint8_t>& data)

Receives a CAN message from the interface.

    Parameters:
        id: Reference to store the received message ID.
        data: A std::vector<uint8_t> to store the received message payload.

    Returns:
        true: If a message was successfully received.
        false: If an error occurred during reception.