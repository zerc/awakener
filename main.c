#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <libc.h>
#include <errno.h>

#include "server.h"






//char *msg = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=utf-8\n\nHello!\n";
// Web server MVP



int main() {
    start_sever();
}

// Magic packet MVP
int main1() {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo("255.255.255.255", "9", &hints, &res) != 0) {
        perror("getaddrinfo error");
        return 2;
    }
    printf("info collected\n");

    int s;
    struct protoent *p;
    p = getprotobyname("udp");
    if ((s = socket(PF_INET, SOCK_DGRAM, p->p_proto)) == -1) {
        perror("socket error");
        return 2;
    }
    printf("socket created\n");

    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        return 2;
    }

    char msg[102] = {
            0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
            0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
    };

    if (sendto(s, &msg, 102, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("send error");
        close(s);
        return 2;
    }

    printf("sent the message");
    close(s);
    return 0;
}