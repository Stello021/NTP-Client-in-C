#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif //_WIN32

#ifndef NTP_PACKET_H
#define NTP_PACKET_H
#include <time.h>
#include <stdio.h>

#define NTP_TIMESTAMP_DELTA 2208988800

typedef struct 
{
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

extern ntp_packet packet;
extern struct sockaddr_in destination_server; 

int CreateSocket(const char* host_IP);
#endif //NTP_PACKET_H