#include <iostream>         // For console output
#include <unistd.h>         // For sleep(), write()
#include <cstring>          // For strcpy()
#include <cstdlib>          // For rand(), srand()
#include <ctime>            // For time() to seed randomness
#include <net/if.h>         // For network interface config
#include <sys/ioctl.h>      // For ioctl()
#include <sys/socket.h>     // For socket functions
#include <linux/can.h>      // For CAN frame struct
#include <linux/can/raw.h>  // For raw CAN socket

int main() {
    std::srand(std::time(nullptr)); // Initialize random seed

    // Create a CAN raw socket
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("Socket");
        return 1;
    }

    // Specify interface (can0)
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "can0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    // Bind socket to CAN interface
    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        return 1;
    }

    struct can_frame frame{};
    frame.can_id = 0x123;        // Arbitrary CAN ID for testing
    frame.can_dlc = 2;           // Number of data bytes (speed + rpm)

    // Continuously send fake data
    while (true) {
        // Generate random speed (0-180 km/h)
        int speed = std::rand() % 181;

        // Generate random rpm (0-6000, steps of 100)
        int rpm = (std::rand() % 61) * 100;

        // Fill CAN data frame
        frame.data[0] = static_cast<uint8_t>(speed);        // Byte 0 = speed
        frame.data[1] = static_cast<uint8_t>(rpm / 100);     // Byte 1 = rpm / 100

        // Send frame over CAN bus
        int bytes_sent = write(sock, &frame, sizeof(frame));
        if (bytes_sent != sizeof(frame)) {
            std::cerr << "Error sending CAN frame" << std::endl;
        } else {
            std::cout << "[CAN SENT] ID: 0x" << std::hex << frame.can_id
                      << " Data: " << std::dec << speed << " " << rpm << std::endl;
        }

        // Delay before sending next message
        sleep(1);
    }

    // Close the socket (loop never exits)
    close(sock);
    return 0;
}
