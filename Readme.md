# NTP Client

This project implements a simple NTP (Network Time Protocol) client in C to retrieve the current time from a remote NTP server.

## File Structure

1. `ntp_packet.h`: Contains the NTP packet structure and main function declarations.

2.  `ntp_packet.c`: Implements the logic for creating the socket and initializing the NTP packet.

3. `what_time_is_it.c`: Main file that handles the communication flow with the NTP server and display the results.

## Key Points

### NTP Pakcet Structure

```c
typedef struct {
    unsigned char leap_version_mode;
    unsigned char stratum;
    unsigned char poll;
    unsigned char precision;
    unsigned int root_delay;
    unsigned int root_dispersion;
    char rfid[4];
    unsigned long ref_timestamp_seconds;
    unsigned long ref_timestamp_fraction;
    unsigned long original_timestamp_seconds;
    unsigned long original_timestamp_fraction;
    unsigned long receive_timestamp_seconds;
    unsigned long receive_timestamp_fraction;
    unsigned long transmit_timestamp_seconds;
    unsigned long transmit_timestamp_fraction;
} ntp_packet;

```

Compare the fields to the official NTP protocol specifications for further understanding

### Socket Initialization

Creates a UDP socket and configures the destination server. Cross-platform logic (`#ifdef _WIN32`) is used to handle differences between operating systems.

```c
int CreateSocket(const char* host_IP) {
    #ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll2
    WSADATA wsa_data;
    if (WSAStartup(0x0202, &wsa_data)) {
        printf("Unable to initialize Winsock2.\n");
        return -1;
    }
    #endif

    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) {
        printf("Unable to initialize the UDP Socket.\n");
        return -1;
    }
    printf("Socket %d created.\n", s);

    inet_pton(AF_INET, host_IP, &destination_server.sin_addr);
    destination_server.sin_family = AF_INET;
    destination_server.sin_port = htons(123);

    return s;
}
```

### Bit Manipulation

Extracts information from the first 8 bits of the NTP packet (Leap, Version, Mode). Bits are masked and shifted to extract meaningful details. 

```c
printf("Leap second: %d \nVersion: %d \nMode: %d\n",
    packet.leap_version_mode >> 6,               // Leap
    (packet.leap_version_mode >> 3) & 0b111,    // Version
    packet.leap_version_mode & 0b111);          // Mode
```





### Parsing NTP Timestamps

Converts the NTP server timestamp (offset 1900) into a human-readable format, adapted to the UNIX Epoch system

```c
packet.transmit_timestamp_seconds = ntohl(packet.transmit_timestamp_seconds);
packet.transmit_timestamp_fraction = ntohl( packet.transmit_timestamp_fraction);
time_t transmit_timestamp = (time_t)(packet.transmit_timestamp_seconds) - NTP_TIMESTAMP_DELTA;

struct tm local_time;
if (localtime_s(&local_time, &transmit_timestamp) == 0) {
    printf("Time: %04d-%02d-%02d %02d:%02d:%02d\n",
        local_time.tm_year + 1900, local_time.tm_mon + 1,
        local_time.tm_mday, local_time.tm_hour,
        local_time.tm_min, local_time.tm_sec);
} else {
    printf("Error getting local time.\n");
}
```

where ` NTP_TIMESTAMP_DELTA` is defined as `#define NTP_TIMESTAMP_DELTA 2208988800`


