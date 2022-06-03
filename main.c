#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <libc.h>

#include "server.h"

int send_magic_packet();

size_t read_public_file(char *file_name, char *buffer) {
    FILE *f;
    size_t total_read, read;

    char actual_path[100];
    char *path;
    path = realpath(file_name, actual_path);

    if ((f = fopen(path, "r")) == NULL) {
        perror("open file");
        exit(1);
    }
    while ((read = fread(buffer, 1,RESPONSE_BODY_SIZE, f)) > 0) {
        total_read += read;
    }
    fclose(f);
    return total_read;
}


struct response handle_request(const struct request req) {
    struct response resp;

    if (strcmp(req.path, "/") == 0) {
        resp.body_size = read_public_file("./public/index.html", resp.body);
        resp.code = "200 OK";
    } else if ((
            strcmp(req.method, "POST") +
            strcmp(req.path, "/awake/")) == 0) {
        if (send_magic_packet() != 0) {
            strcpy(resp.body, "error!");
        } else {
            strcpy(resp.body, "awoken!");
        }
        resp.body_size = 8;
        resp.code = "200 OK";
    } else {
        strcpy(resp.body, "Not Found");
        resp.body_size = 10;
        resp.code = "404 NOT FOUND";
    }

    return resp;
}


int main() {
    return start_server();
}

int send_magic_packet() {
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