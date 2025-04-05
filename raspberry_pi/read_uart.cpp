#include "read_uart.h"

#include <cstdlib>      // For rand(), srand()
#include <ctime>        // For time() seed
#include <string>       // For std::string
#include <iostream>     // For console logging
#include <sstream>      // For building JSON string
#include <unistd.h>     // For POSIX read(), close()
#include <fcntl.h>      // For file descriptor flags (open)
#include <termios.h>    // For serial port configuration

void readUART()
{
    // Specify the UART device path (default for Pi)
    const char* device = "/dev/serial0";

    // Open UART device for reading only, and disable controlling terminal
    int serial_port = open(device, O_RDONLY | O_NOCTTY);

    // Check if the port was opened successfully
    if (serial_port < 0) {
        std::cerr << "Failed to open serial port!\n";
        return;
    }

    // Configure the UART parameters
    termios tty{};
    tcgetattr(serial_port, &tty);              // Get current UART config
    cfsetispeed(&tty, B9600);                  // Set input baud rate to 9600
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 data bits
    tty.c_cflag |= CREAD | CLOCAL;             // Enable receiver, ignore modem lines
    tty.c_lflag = 0;                           // Disable canonical mode and echo
    tty.c_oflag = 0;                           // Raw output
    tty.c_cc[VMIN] = 1;                        // Minimum 1 character to read
    tty.c_cc[VTIME] = 0;                       // No read timeout
    tcsetattr(serial_port, TCSANOW, &tty);     // Apply UART config immediately

    std::cout << "Listening on UART...\n";

    std::string line;      // Store a full line from UART
    int x = 0, y = 0;      // Store parsed X and Y values
    bool hasX = false, hasY = false;

    std::srand(std::time(nullptr));  // Seed RNG for simulated Z values

    char ch;
    // Read one character at a time from UART
    while (read(serial_port, &ch, 1) == 1)
    {
        if (ch == '\r') continue;  // Skip carriage return

        if (ch == '\n') {
            if (!line.empty()) {
                std::cout << "[UART] " << line << std::endl;

                // Check for X axis line
                if (line.find("X:") != std::string::npos) {
                    sscanf(line.c_str(), "X: %d", &x);
                    hasX = true;
                }
                // Check for Y axis line
                else if (line.find("Y:") != std::string::npos) {
                    sscanf(line.c_str(), "Y: %d", &y);
                    hasY = true;
                }

                // Once X and Y are available, simulate Z and publish
                if (hasX && hasY) {
                    int z = (std::rand() % 601) - 300;  // Simulate Z between -300 and +300

                    // Build a JSON string: { "x": 123, "y": -45, "z": 88 }
                    std::ostringstream json;
                    json << "{ \"x\": " << x << ", \"y\": " << y << ", \"z\": " << z << " }";

                    // Construct MQTT publish command
                    std::string cmd = "mosquitto_pub -t sensor/accel -m '" + json.str() + "'";
                    system(cmd.c_str());  // Publish to MQTT

                    std::cout << "[MQTT] " << json.str() << std::endl;

                    // Reset state for next cycle
                    hasX = hasY = false;
                }

                line.clear();  // Clear line buffer for next input
            }
        } else {
            line += ch;  // Accumulate characters into current line
        }
    }

    close(serial_port);  // Clean up
}
