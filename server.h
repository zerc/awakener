//
// Created by Vladimir Savin on 02/06/2022.
//

#ifndef AWAKENER_SERVER_H
#define AWAKENER_SERVER_H

struct request {
    char method[10];
    char path[100];
};

struct response {
    char *code;
    char *body;
};

struct response handle_request(struct request req);
int start_server(void);
#endif //AWAKENER_SERVER_H
