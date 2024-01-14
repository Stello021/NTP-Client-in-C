#include <ntp_packet.h>

int main(int argc, char const *argv[])
{
    int socket = CreateSocket("216.239.35.12");
    int sent_packet = sendto(socket, (char*)&packet, sizeof(ntp_packet), 0, (struct sockaddr*) &destination_server, sizeof(destination_server));
    if(sent_packet < 0)
    {
        printf("Failed to sent packet!\n");
        return -1;
    }

    int destination_server_size = sizeof(destination_server);
    int packet_received = recvfrom(socket, (char*)&packet, sizeof(ntp_packet), 0, (struct sockaddr*) &destination_server, &destination_server_size);
    if(packet_received < 0)
    {
        printf("Failed to receive packet!\n");
        return -1;
    }
    printf("Leap second: %d \nVersion: %d \nMode: %d\n", packet.leap_version_mode >> 6, (packet.leap_version_mode >> 3) & 0b111, packet.leap_version_mode & 0b111);
    printf("Stratum: %d \nPoll: %d \nPrecision: %d\n", packet.stratum, packet.poll, packet.precision);
    printf("Root Delay: %u \nRoot Dispersion: %lu \n", packet.root_delay, ntohl(packet.root_dispersion));
    printf("Ref ID: ");
    for (size_t i = 0; i < 4; i++)
    {
        printf("%c", packet.rfid[i]);
    }
    printf("\n");
    
    packet.transmit_timestamp_seconds = ntohl( packet.transmit_timestamp_seconds); 
    packet.transmit_timestamp_fraction = ntohl( packet.transmit_timestamp_fraction);
    time_t transmit_timestamp = (time_t)(packet.transmit_timestamp_seconds) - NTP_TIMESTAMP_DELTA;
    struct tm local_time;
    if (localtime_s(&local_time ,&transmit_timestamp) == 0) 
    {
        printf("Time: %04d-%02d-%02d %02d:%02d:%02d\n", local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday, local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
    } 
    else 
    {
        printf("Error getting local time.\n");
    }
    return 0;
}
