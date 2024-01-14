#include <ntp_packet.h>

ntp_packet packet = {0x23, //first byte's bits to 00,100,011 for leap = 0, version = 4 and mode = 3 
                     0,    //stratum
                     0,    //poll
                     0,    //precision
                     0,    //root delay
                     0,    //root dispersion
                     '\0\0\0\0', //refid
                     //timestamps
                     0,
                     0,
                     0,
                     0};


struct sockaddr_in destination_server; 

int CreateSocket(const char* host_IP)
{
    #ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data ;
    if (WSAStartup (0x0202 , &wsa_data ))
    {
        printf ("unable to initialize winsock2 \n");
        return -1;
    }
    #endif
    int s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    if(s < 0)
    {
        printf("Unable to initialize the UDP Socket\n");
        return - 1;
    }
    printf ("socket %d created \n", s);

    inet_pton(AF_INET, host_IP, &destination_server.sin_addr);
    destination_server.sin_family = AF_INET;
    destination_server.sin_port = htons(123);

    return s;

}