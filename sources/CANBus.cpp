#include "CANBus.hpp"

// Constructor: Initializes the CAN interface
CANBus::CANBus() {
    _socket_fd = -1;
}

CANBus::CANBus(const std::string& interface, int bitrate) {
    // Create a CAN RAW socket
    _socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (_socket_fd < 0) {
        throw std::runtime_error("Failed to create CAN socket");
    }

    // Bind the socket to the CAN interface
    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
    if (ioctl(_socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        close(_socket_fd);
        throw std::runtime_error("Failed to get interface index for: " + interface);
    }

    struct sockaddr_can addr = {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(_socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(_socket_fd);

        throw std::runtime_error("Failed to bind CAN socket to interface: " + interface);
    }

    // Optionally set bitrate (requires `ip link` configuration, not directly in code)
    std::cout << "Initialized CAN interface: " << interface << " at bitrate: " << bitrate << " bps" << std::endl;
}

// Destructor: Closes the CAN socket
CANBus::~CANBus() {
    if (_socket_fd >= 0) {
        close(_socket_fd);
    }
}

// Sends a message on the CAN bus
bool CANBus::sendMessage(uint32_t id, const std::vector<uint8_t>& data) {
    if (data.size() > 8) {
        std::cerr << "Error: CAN frame data exceeds 8 bytes!" << std::endl;
        return false;
    }

    struct can_frame frame = {};
    frame.can_id = id;
    frame.can_dlc = data.size(); // Data Length Code (number of bytes)

    std::copy(data.begin(), data.end(), frame.data);

    // Send the CAN frame
    if (write(_socket_fd, &frame, sizeof(frame)) != sizeof(frame)) {
        std::cerr << "Error: Failed to send CAN message!" << std::endl;
        return false;
    }

    return true;
}

// Receives a message from the CAN bus
bool CANBus::receiveMessage(uint32_t& id, std::vector<uint8_t>& data) {
    struct can_frame frame = {};

    // Read a CAN frame
    ssize_t nbytes = read(_socket_fd, &frame, sizeof(frame));
    std::cout << "Received " << nbytes << " bytes" << std::endl;
    if (nbytes < 0) {
        std::cerr << "Error: Failed to read CAN message!" << std::endl;
        return false;
    }

    if (nbytes < sizeof(frame)) {
        std::cerr << "Error: Incomplete CAN frame received!" << std::endl;
        return false;
    }

    id = frame.can_id;
    data.assign(frame.data, frame.data + frame.can_dlc);

    return true;
}
