#include <iostream>         // For console output
#include <sstream>          // For building JSON strings
#include <unistd.h>         // For read(), close(), sleep()
#include <cstring>          // For strcpy()
#include <cstdlib>          // For system()
#include <net/if.h>         // For network interface info (struct ifreq)
#include <sys/ioctl.h>      // For ioctl()
#include <sys/socket.h>     // For socket functions
#include <linux/can.h>      // For struct can_frame
#include <linux/can/raw.h>  // For raw CAN sockets

int main() {
    // Create a raw CAN socket
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("Socket");
        return 1;
    }

    // Specify which CAN interface to use (e.g., can0)
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "can0");
    ioctl(sock, SIOCGIFINDEX, &ifr);  // Get index of can0

    // Set up socket address structure for CAN
    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Bind socket to can0 interface
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        return 1;
    }

    struct can_frame frame{};
    std::cout << "Listening on CAN0..." << std::endl;

    // Continuously read incoming CAN frames
    while (true) {
        int bytes_read = read(sock, &frame, sizeof(frame));
        if (bytes_read > 0) {
            // Extract speed and RPM from frame data
            int speed = frame.data[0];         // Byte 0 = Speed in km/h
            int rpm = frame.data[1] * 100;     // Byte 1 = RPM / 100

            // Format values as JSON
            std::ostringstream json;
            json << "{ \"speed\": " << speed << ", \"rpm\": " << rpm << " }";

            // Send JSON via MQTT to topic `sensor/can`
            std::string cmd = "mosquitto_pub -t sensor/can -m '" + json.str() + "'";
            system(cmd.c_str());

            // Print to console for verification
            std::cout << "[CAN->MQTT] " << json.str() << std::endl;
        }
    }

    // Close the socket when done (never reached in this loop)
    close(sock);
    return 0;
}
