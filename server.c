//
// Created by Vladimir Savin on 02/06/2022.
//
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <libc.h>
#include <errno.h>

#include "server.h"


void sigchld_handler(int s)
{
    // https://beej.us/guide/bgnet/html/#listen
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int parse_request(char *buf, struct request *r) {
    sscanf(buf, "%s %s", r->method, r->path);
    return 0;
}

int start_server() {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo("127.0.0.1", "8888", &hints, &res) != 0) {
        perror("getaddrinfo error");
        return 2;
    }
    printf("info collected\n");

    int s;
    if ((s = socket(PF_INET, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket error");
        return 2;
    }

    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        return 2;
    }
    printf("socket created\n");

    if (bind(s, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind error");
        close(s);
        return 2;
    }

    printf("socket binded\n");

    if (listen(s, 10) == -1) {
        perror("listen error");
        close(s);
        return 2;
    }

    printf("socket is listening\n");

    int s_new;
    struct sockaddr_storage inc_addr;
    socklen_t inc_add_size;
    inc_add_size = sizeof inc_addr;

    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    char buff[1000];
    int bytes_read;

    printf("waiting for new connections\n");

    struct request r;
    memset(&r, 0, sizeof r);

    while (1) {
        if ((s_new = accept(s, (struct sockaddr *) &inc_addr, &inc_add_size)) == -1) {
            perror("accept error");
            close(s);
            return 2;
        }

        if (!fork()) {
            close(s);

            if ((bytes_read = recv(s_new, buff, 999, 0)) == -1) {
                perror("recv error");
                close(s_new);
                exit(1);
            }

            buff[bytes_read] = '\0';
            parse_request(buff, &r);
            printf("method %s path %s", r.method, r.path);

            struct response resp;
            resp = handle_request(r);

            char full_resp[2000];
            sprintf(full_resp, "HTTP/1.1 %s\nContent-Type: text/html;charset=utf-8\n\n%s\n", resp.code, resp.body);
            int bytes_sent;

            if ((bytes_sent = send(s_new, full_resp, resp.body_size+55, 0)) == -1) {
                perror("send error");
                close(s_new);
                exit(1);
            }
            close(s_new);
            exit(0);
        }

        close(s_new);
    }
}