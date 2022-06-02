#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <libc.h>
#include <errno.h>

int main(void) {
    const char input[] = "aa:bb:cc";
    char out[10];
    sscanf(input, "%s", &out[0], &out[1]);
    printf(out);
    return 0;
}

void sigchld_handler(int s)
{
    // https://beej.us/guide/bgnet/html/#listen
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// Web server MVP
int main2() {
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

    while(1) {
        if ((s_new = accept(s, (struct sockaddr *)&inc_addr, &inc_add_size)) == -1) {
            perror("accept error");
            close(s);
            return 2;
        }

        if (!fork()) {
            close(s);

            if ((bytes_read = recv(s_new, buff, 999, 0)) == -1) {
                perror('recv error');
                close(s_new);
                exit(1);
            }

            buff[bytes_read] = '\0';
            printf("received '%s'\n", buff);

            char *msg = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=utf-8\n\nHello!\n";
            int len, bytes_sent;
            len = strlen(msg);

            if ((bytes_sent = send(s_new, msg, len, 0)) == -1) {
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